#!/usr/bin/env python3
"""
Local mock BMS Control Hub for testing the web dashboard without ESP32/STM32 hardware.

Usage:
    python3 scripts/mock_hub_server.py
    python3 scripts/mock_hub_server.py --port 8090
    open http://127.0.0.1:8080/
"""

from __future__ import annotations

import argparse
import json
import math
import threading
import time
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer
from pathlib import Path
from urllib.parse import urlparse

PROJECT_DIR = Path(__file__).resolve().parents[1]
WEB_DIR = PROJECT_DIR / "web"
UPLOAD_DIR = PROJECT_DIR / ".mock_uploads"

_firmware_state = {
    "state": "Idle",
    "running": False,
    "progress": 0,
    "image_path": "/firmware.bin",
}
_state_lock = threading.Lock()
_sim_start = time.time()


def simulated_telemetry() -> dict:
    elapsed = time.time() - _sim_start
    return {
        "valid": True,
        "voltage_mv": int((12400 + math.sin(elapsed / 3.0) * 250) + 0.5),
        "current_ma": int((1500 + math.cos(elapsed / 2.5) * 400) + 0.5),
        "temperature_centi_c": int((2850 + math.sin(elapsed / 4.0) * 120) + 0.5),
        "soc_tenths_percent": int((820 + math.sin(elapsed / 8.0) * 50) + 0.5),
        "fault_flags": 0,
    }


def simulate_firmware_update() -> None:
    stages = [
        ("CalculatingChecksum", 5, 0.4),
        ("Starting", 10, 0.5),
        ("Transferring", 90, 2.0),
        ("Verifying", 98, 0.8),
        ("Complete", 100, 0.2),
    ]

    with _state_lock:
        _firmware_state["running"] = True
        _firmware_state["state"] = "CalculatingChecksum"
        _firmware_state["progress"] = 0

    for state, progress, delay in stages:
        time.sleep(delay)
        with _state_lock:
            _firmware_state["state"] = state
            _firmware_state["progress"] = progress
            _firmware_state["running"] = state not in {"Complete", "Failed", "Cancelled"}

    with _state_lock:
        _firmware_state["running"] = False


class MockHubHandler(BaseHTTPRequestHandler):
    server_version = "BMSMockHub/1.0"

    def log_message(self, format: str, *args) -> None:
        print(f"[mock] {self.address_string()} {format % args}")

    def _send_cors(self) -> None:
        self.send_header("Access-Control-Allow-Origin", "*")
        self.send_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS")
        self.send_header("Access-Control-Allow-Headers", "Content-Type")

    def _send_json(self, payload: dict, status: int = 200) -> None:
        body = json.dumps(payload).encode("utf-8")
        self.send_response(status)
        self.send_header("Content-Type", "application/json")
        self.send_header("Content-Length", str(len(body)))
        self._send_cors()
        self.end_headers()
        self.wfile.write(body)

    def _send_bytes(self, data: bytes, content_type: str, status: int = 200) -> None:
        self.send_response(status)
        self.send_header("Content-Type", content_type)
        self.send_header("Content-Length", str(len(data)))
        self._send_cors()
        self.end_headers()
        self.wfile.write(data)

    def do_OPTIONS(self) -> None:
        self.send_response(204)
        self._send_cors()
        self.end_headers()

    def do_GET(self) -> None:
        path = urlparse(self.path).path

        if path == "/api/health":
            self._send_json(
                {
                    "status": "ok",
                    "filesystem": True,
                    "telemetry_ready": True,
                    "telemetry_valid": True,
                    "updater_ready": True,
                    "embedded_ui": False,
                    "mode": "local-mock",
                }
            )
            return

        if path == "/api/telemetry":
            self._send_json(simulated_telemetry())
            return

        if path == "/api/firmware":
            with _state_lock:
                payload = dict(_firmware_state)
            self._send_json(payload)
            return

        if path in {"/", ""}:
            path = "/index.html"

        file_path = (WEB_DIR / path.lstrip("/")).resolve()
        if not str(file_path).startswith(str(WEB_DIR.resolve())) or not file_path.is_file():
            self._send_json({"error": "not found", "path": path}, status=404)
            return

        content_type = {
            ".html": "text/html",
            ".css": "text/css",
            ".js": "application/javascript",
            ".json": "application/json",
        }.get(file_path.suffix.lower(), "application/octet-stream")
        self._send_bytes(file_path.read_bytes(), content_type)

    def do_POST(self) -> None:
        path = urlparse(self.path).path
        length = int(self.headers.get("Content-Length", "0"))
        body = self.rfile.read(length) if length > 0 else b""

        if path == "/api/firmware/upload":
            UPLOAD_DIR.mkdir(parents=True, exist_ok=True)
            target = UPLOAD_DIR / "firmware.bin"
            target.write_bytes(body)
            self._send_json({"status": "uploaded", "path": "/firmware.bin", "bytes": len(body)})
            return

        if path == "/api/firmware/start":
            with _state_lock:
                if _firmware_state["running"]:
                    self._send_json({"error": "update already running"}, status=409)
                    return
            threading.Thread(target=simulate_firmware_update, daemon=True).start()
            self._send_json({"status": "started", "path": "/firmware.bin"})
            return

        self._send_json({"error": "not found", "path": path}, status=404)


def create_server(host: str, preferred_port: int) -> tuple[ThreadingHTTPServer, int]:
    for port in range(preferred_port, preferred_port + 20):
        try:
            server = ThreadingHTTPServer((host, port), MockHubHandler)
            server.daemon_threads = True
            server.allow_reuse_address = True
            return server, port
        except OSError as error:
            if error.errno != 48:  # Address already in use
                raise
            print(f"[mock] Port {port} is busy, trying next...")
    raise SystemExit(
        f"No free port found in range {preferred_port}-{preferred_port + 19}. "
        f"Stop the other process or run: python3 scripts/mock_hub_server.py --port 9000"
    )


def main() -> None:
    parser = argparse.ArgumentParser(description="Local mock BMS Control Hub server")
    parser.add_argument("--host", default="127.0.0.1", help="Bind address (default: 127.0.0.1)")
    parser.add_argument("--port", type=int, default=8080, help="Preferred port (default: 8080)")
    args = parser.parse_args()

    if not WEB_DIR.is_dir():
        raise SystemExit(f"web/ folder not found at {WEB_DIR}")

    UPLOAD_DIR.mkdir(parents=True, exist_ok=True)
    server, port = create_server(args.host, args.port)

    print("BMS Control Centre — local mock server (no hardware required)")
    print(f"Dashboard:  http://{args.host}:{port}/")
    print(f"Health:     http://{args.host}:{port}/api/health")
    print(f"Telemetry:  http://{args.host}:{port}/api/telemetry")
    print(f"Firmware:   http://{args.host}:{port}/firmware.html")
    if port != args.port:
        print(f"Note: port {args.port} was busy, using {port} instead.")
    print("Press Ctrl+C to stop.")
    try:
        server.serve_forever()
    except KeyboardInterrupt:
        print("\nStopped.")
    finally:
        server.server_close()


if __name__ == "__main__":
    main()
