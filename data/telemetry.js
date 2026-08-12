let pageSimInterval = null;
let liveInterval = null;

function updatePageData() {
  if (typeof drawGraphs === 'function') drawGraphs();
}

function startSimulation() {
  if (!pageSimInterval) {
    pageSimInterval = setInterval(updatePageData, 500);
  }
}

function stopSimulation() {
  if (pageSimInterval) {
    clearInterval(pageSimInterval);
    pageSimInterval = null;
  }
}

function applyTelemetryToPage(values) {
  const voltage = document.querySelector('.voltage-value') || document.getElementById('volt-val');
  const current = document.querySelector('.current-value') || document.getElementById('curr-val');
  const temp = document.querySelector('.temp-value') || document.getElementById('temp-val');
  const statusTag = document.getElementById('status-pill');

  if (voltage && typeof values.voltage === 'number') {
    voltage.innerText = values.voltage.toFixed(2) + ' V';
  }
  if (current && typeof values.current === 'number') {
    current.innerText = values.current.toFixed(2) + ' A';
  }
  if (temp && typeof values.temperature === 'number') {
    temp.innerText = values.temperature.toFixed(1) + ' °C';
  }
  if (statusTag) {
    statusTag.innerHTML = values.valid ? '🟢 Online' : '🔴 Offline';
    statusTag.style.color = values.valid ? '#4ade80' : '#f87171';
    statusTag.style.backgroundColor = values.valid ? '#143823' : '#1e293b';
  }
}

async function refreshLiveTelemetry() {
  try {
    const payload = await fetchLiveTelemetry();
    applyTelemetryToPage(telemetryToDisplayValues(payload));
  } catch (error) {
    applyTelemetryToPage({ valid: false });
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

function syncSimMode() {
  const isSimOn = isSimulatedMode();
  const btn = document.getElementById('sim-mode-btn');

  if (btn) {
    btn.innerText = isSimOn ? 'Simulated Mode: On' : 'Simulated Mode: Off';
    btn.style.backgroundColor = isSimOn ? '#083c5a' : '#1e293b';
    btn.style.color = isSimOn ? '#38bdf8' : '#94a3b8';
    btn.style.borderColor = isSimOn ? '#162646' : '#334155';
  }

  if (isSimOn) {
    stopLivePolling();
    startSimulation();
  } else {
    stopSimulation();
    startLivePolling();
  }
}

function toggleSimulatedMode() {
  setSimulatedMode(!isSimulatedMode());
  syncSimMode();
}

window.addEventListener('DOMContentLoaded', () => {
  ensureSimulatedModeDefault();
  syncSimMode();

  const stmLabel = document.getElementById('stm');
  if (stmLabel) {
    stmLabel.innerHTML = 'ESP32 Control Hub';
  }
});

window.addEventListener('storage', (event) => {
  if (event.key === 'simulatedMode') {
    syncSimMode();
  }
});
