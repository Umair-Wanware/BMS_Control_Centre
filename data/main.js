let simInterval = null;
let liveInterval = null;
let animOffset = 0;
let secondsCounter = 0;
let telemetryHistory = {
  voltage: [],
  current: [],
  temperature: [],
  soc: []
};

function drawGraphs() {
  animOffset += 0.15;

  let d1 = 'M 0 ' + (50 + Math.sin(animOffset) * 20);
  for (let x = 15; x <= 300; x += 15) {
    const y = 50 + Math.sin(animOffset + x * 0.05) * 25 + Math.sin(x * 0.1) * 5;
    d1 += ' L ' + x + ' ' + y;
  }
  const g1 = document.getElementById('graph-path-1');
  if (g1) g1.setAttribute('d', d1);

  let d2 = 'M 0 ' + (60 + Math.cos(animOffset) * 15);
  for (let x = 15; x <= 300; x += 15) {
    const y = 55 + Math.cos(animOffset + x * 0.04) * 20 + Math.cos(x * 0.08) * 8;
    d2 += ' L ' + x + ' ' + y;
  }
  const g2 = document.getElementById('graph-path-2');
  if (g2) g2.setAttribute('d', d2);

  let dSOC = 'M 0 ' + (30 + Math.sin(animOffset * 0.5) * 3);
  for (let x = 30; x <= 300; x += 30) {
    const y = 30 + Math.sin(animOffset * 0.5 + x * 0.02) * 4;
    dSOC += ' L ' + x + ' ' + y;
  }
  const gsoc = document.getElementById('graph-soc-path');
  if (gsoc) gsoc.setAttribute('d', dSOC);
}

function pushHistory(values) {
  if (typeof values.voltage === 'number') telemetryHistory.voltage.push(values.voltage);
  if (typeof values.current === 'number') telemetryHistory.current.push(values.current);
  if (typeof values.temperature === 'number') telemetryHistory.temperature.push(values.temperature);
  if (typeof values.soc === 'number') telemetryHistory.soc.push(values.soc);

  Object.keys(telemetryHistory).forEach((key) => {
    if (telemetryHistory[key].length > 24) {
      telemetryHistory[key].shift();
    }
  });
}

function updateConnectionStatus(connected) {
  const statusTag = document.getElementById('connection-status') ||
    document.querySelector('.top-tags .top-tag');
  const stmStatus = document.querySelector('.system-status-box .status-row:nth-child(2) .pill');
  if (statusTag) {
    statusTag.textContent = connected ? '🟢 Connected' : '🔴 Offline';
  }
  if (stmStatus) {
    stmStatus.textContent = connected ? 'Online' : 'Offline';
    stmStatus.classList.toggle('pill-green', connected);
    stmStatus.classList.toggle('pill-red', !connected);
  }
}

function applyTelemetryValues(values) {
  const now = new Date();
  const topClock = document.getElementById('top-clock');
  if (topClock) topClock.innerText = '🕒 ' + now.toLocaleTimeString();
  const lastUpdate = document.getElementById('last-update-time');
  if (lastUpdate) lastUpdate.innerText = now.toLocaleTimeString();

  const vEl = document.getElementById('val-voltage');
  const iEl = document.getElementById('val-current');
  const tEl = document.getElementById('val-temp');
  const socEl = document.getElementById('val-soc');
  const barSoc = document.getElementById('bar-soc');
  const currentStatus = document.getElementById('val-current-status');

  if (vEl && typeof values.voltage === 'number') {
    vEl.innerText = values.voltage.toFixed(2) + ' V';
  }
  if (iEl && typeof values.current === 'number') {
    iEl.innerText = values.current.toFixed(2) + ' A';
  }
  if (tEl && typeof values.temperature === 'number') {
    tEl.innerText = values.temperature.toFixed(1) + ' °C';
  }
  if (socEl && typeof values.soc === 'number') {
    socEl.innerText = Math.round(values.soc) + '%';
  }
  if (barSoc && typeof values.soc === 'number') {
    barSoc.style.width = Math.max(0, Math.min(100, values.soc)) + '%';
  }
  if (currentStatus && typeof values.current === 'number') {
    currentStatus.innerText = values.current >= 0 ? 'Discharging' : 'Charging';
  }

  if (typeof values.voltage === 'number') {
    const cellVoltage = values.voltage / 4;
    ['cell-1', 'cell-2', 'cell-3', 'cell-4'].forEach((id) => {
      const cell = document.getElementById(id);
      if (cell) cell.innerText = cellVoltage.toFixed(3) + ' V';
    });
  }

  pushHistory(values);
  drawGraphs();
}

function updateSimulatedMetrics() {
  const time = Date.now() / 1500;
  applyTelemetryValues({
    voltage: 12.4 + Math.sin(time) * 0.25,
    current: 2.1 + Math.cos(time * 1.2) * 0.18,
    temperature: 30 + Math.sin(time / 1.7) * 1.6,
    soc: 86 + Math.sin(time / 4) * 2.8
  });
  updateConnectionStatus(true);

  secondsCounter++;
  const upEl = document.getElementById('val-uptime');
  if (upEl) upEl.innerText = `2d 14h 23m ${(15 + secondsCounter) % 60}s`;
}

async function refreshLiveTelemetry() {
  try {
    const payload = await fetchLiveTelemetry();
    const values = telemetryToDisplayValues(payload);
    if (!values.valid) {
      updateConnectionStatus(false);
      return;
    }
    applyTelemetryValues(values);
    updateConnectionStatus(true);
  } catch (error) {
    updateConnectionStatus(false);
  }
}

function startSimulation() {
  if (!simInterval) {
    simInterval = setInterval(updateSimulatedMetrics, 1000);
  }
}

function stopSimulation() {
  if (simInterval) {
    clearInterval(simInterval);
    simInterval = null;
  }
}

function startLivePolling() {
  if (!liveInterval) {
    refreshLiveTelemetry();
    liveInterval = setInterval(refreshLiveTelemetry, 2000);
  }
}

function stopLivePolling() {
  if (liveInterval) {
    clearInterval(liveInterval);
    liveInterval = null;
  }
}

function updateSimModeUI(isSimOn) {
  const btn = document.getElementById('sim-mode-btn');
  if (!btn) return;

  if (isSimOn) {
    btn.innerText = 'Simulated Mode: On';
    btn.style.backgroundColor = '#083c5a';
    btn.style.color = '#38bdf8';
    btn.style.borderColor = '#162646';
    stopLivePolling();
    startSimulation();
  } else {
    btn.innerText = 'Simulated Mode: Off';
    btn.style.backgroundColor = '#1e293b';
    btn.style.color = '#94a3b8';
    btn.style.borderColor = '#334155';
    stopSimulation();
    startLivePolling();
  }
}

function toggleSimulatedMode() {
  setSimulatedMode(!isSimulatedMode());
  updateSimModeUI(isSimulatedMode());
}

window.addEventListener('DOMContentLoaded', () => {
  ensureSimulatedModeDefault();
  drawGraphs();
  updateSimModeUI(isSimulatedMode());

  const simBtn = document.getElementById('sim-mode-btn');
  if (simBtn) {
    simBtn.addEventListener('click', toggleSimulatedMode);
  }

  const otaLabel = document.getElementById('OTA');
  if (otaLabel) {
    otaLabel.innerHTML = 'ESP32 Control Hub';
  }
});

window.addEventListener('storage', (event) => {
  if (event.key === 'simulatedMode') {
    updateSimModeUI(isSimulatedMode());
  }
});
