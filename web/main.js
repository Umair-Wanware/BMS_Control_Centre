let simInterval = null;
let animOffset = 0;
let secondsCounter = 0;

// Draw Dynamic Waves for Graphs
function drawGraphs() {
  animOffset += 0.15;

  // Graph 1 (Blue Wave)
  let d1 = 'M 0 ' + (50 + Math.sin(animOffset) * 20);
  for (let x = 15; x <= 300; x += 15) {
    let y = 50 + Math.sin(animOffset + x * 0.05) * 25 + Math.sin(x * 0.1) * 5;
    d1 += ' L ' + x + ' ' + y;
  }
  const g1 = document.getElementById('graph-path-1');
  if (g1) g1.setAttribute('d', d1);

  // Graph 2 (Red Wave)
  let d2 = 'M 0 ' + (60 + Math.cos(animOffset) * 15);
  for (let x = 15; x <= 300; x += 15) {
    let y = 55 + Math.cos(animOffset + x * 0.04) * 20 + Math.cos(x * 0.08) * 8;
    d2 += ' L ' + x + ' ' + y;
  }
  const g2 = document.getElementById('graph-path-2');
  if (g2) g2.setAttribute('d', d2);

  // SOC Wave (Orange Line)
  let dSOC = 'M 0 ' + (30 + Math.sin(animOffset * 0.5) * 3);
  for (let x = 30; x <= 300; x += 30) {
    let y = 30 + Math.sin(animOffset * 0.5 + x * 0.02) * 4;
    dSOC += ' L ' + x + ' ' + y;
  }
  const gsoc = document.getElementById('graph-soc-path');
  if (gsoc) gsoc.setAttribute('d', dSOC);
}

// Update Random Live Values
function updateMetrics() {
  // 1. Clock Update
  const now = new Date();
  const topClock = document.getElementById('top-clock');
  if (topClock) topClock.innerText = '🕒 ' + now.toLocaleTimeString();
  const lastUpdate = document.getElementById('last-update-time');
  if (lastUpdate) lastUpdate.innerText = now.toLocaleTimeString();

  // 2. Voltage (12.65 V to 12.82 V)
  const voltage = (12.65 + Math.random() * 0.17).toFixed(2);
  const vEl = document.getElementById('val-voltage');
  if (vEl) vEl.innerText = voltage + ' V';

  // 3. Current (1.90 A to 2.15 A)
  const current = (1.90 + Math.random() * 0.25).toFixed(2);
  const iEl = document.getElementById('val-current');
  if (iEl) iEl.innerText = current + ' A';

  // 4. Temperature (28.3 °C to 29.2 °C)
  const temp = (28.3 + Math.random() * 0.9).toFixed(1);
  const tEl = document.getElementById('val-temp');
  if (tEl) tEl.innerText = temp + ' °C';

  // 5. Cell Voltages
  const c1 = document.getElementById('cell-1');
  if (c1) c1.innerText = (3.105 + Math.random() * 0.015).toFixed(3) + ' V';
  const c2 = document.getElementById('cell-2');
  if (c2) c2.innerText = (3.115 + Math.random() * 0.012).toFixed(3) + ' V';
  const c3 = document.getElementById('cell-3');
  if (c3) c3.innerText = (3.095 + Math.random() * 0.018).toFixed(3) + ' V';
  const c4 = document.getElementById('cell-4');
  if (c4) c4.innerText = (3.108 + Math.random() * 0.014).toFixed(3) + ' V';

  // 6. Uptime Simulation Counter
  secondsCounter++;
  const secs = 15 + secondsCounter;
  const upEl = document.getElementById('val-uptime');
  if (upEl) upEl.innerText = `2d 14h 23m ${secs % 60}s`;

  // Draw Graphs Animation Frame
  drawGraphs();
}

function startSimulation() {
  if (!simInterval) {
    simInterval = setInterval(updateMetrics, 500); // Har 500ms par update Hoga
  }
}

function stopSimulation() {
  if (simInterval) {
    clearInterval(simInterval);
    simInterval = null; // Graph aur Values Ruk Jayenge
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
    startSimulation(); // Live Chalu Kar do
  } else {
    btn.innerText = 'Simulated Mode: Off';
    btn.style.backgroundColor = '#1e293b';
    btn.style.color = '#94a3b8';
    btn.style.borderColor = '#334155';
    stopSimulation(); // Sab Freeze Kar do
  }
}

function toggleSimulatedMode() {
  let isSimOn = localStorage.getItem('simulatedMode') === 'true';
  isSimOn = !isSimOn;
  localStorage.setItem('simulatedMode', isSimOn);
  updateSimModeUI(isSimOn);
}

// Initial Setup
window.addEventListener('DOMContentLoaded', () => {
  let savedSim = localStorage.getItem('simulatedMode');
  if (savedSim === null) {
    savedSim = 'true';
    localStorage.setItem('simulatedMode', 'true');
  }

  // Draw initial graph once
  drawGraphs();

  updateSimModeUI(savedSim === 'true');
});

let pageSimInterval = null;

// Har page ke apne custom updates yahan likhein (e.g. Logs update, Charts update)
function updatePageData() {
  // Example: Agar ye Telemetry / Chart page hai toh graphs re-render honge
  console.log('Simulated Data Updating...');

  // Aapka page-specific update code yahan aayega
  if (typeof drawGraphs === 'function') drawGraphs();
}

function startPageSimulation() {
  if (!pageSimInterval) {
    pageSimInterval = setInterval(updatePageData, 500); // 500ms par update
  }
}

function stopPageSimulation() {
  if (pageSimInterval) {
    clearInterval(pageSimInterval);
    pageSimInterval = null; // Freeze data
  }
}

// UI aur Simulation Sync Control
function syncSimMode() {
  const isSimOn = localStorage.getItem('simulatedMode') === 'true';
  const btn = document.getElementById('sim-mode-btn');

  if (btn) {
    if (isSimOn) {
      btn.innerText = 'Simulated Mode: On';
      btn.style.backgroundColor = '#083c5a';
      btn.style.color = '#38bdf8';
      btn.style.borderColor = '#162646';
    } else {
      btn.innerText = 'Simulated Mode: Off';
      btn.style.backgroundColor = '#1e293b';
      btn.style.color = '#94a3b8';
      btn.style.borderColor = '#334155';
    }
  }

  // On hone par run karo, Off hone par freeze
  if (isSimOn) {
    startSimulation();
  } else {
    stopSimulation();
  }
}

// Toggle Function (Button click karne par)
function toggleSimulatedModeUI() {
  let isSimOn = localStorage.getItem('simulatedMode') === 'true';
  localStorage.setItem('simulatedMode', (!isSimOn).toString());
  syncSimMode();
}

// 1. Page Load hote hi status check karo
window.addEventListener('DOMContentLoaded', syncSimMode);

// 2. Dusre Tab ya Window me change hone par automatic sync
window.addEventListener('storage', (e) => {
  if (e.key === 'simulatedMode') {
    syncSimMode();
  }
});

setInterval(function () {
  const btn = document.getElementById('sim-mode-btn');
  const isSimOn = btn ? (btn.innerText.includes('On') && !btn.innerText.includes('Off')) : true;

  if (isSimOn) {
    const v = document.getElementById('val-voltage');
    const i = document.getElementById('val-current');
    const t = document.getElementById('val-temp');

    if (v) v.innerText = (12.65 + Math.random() * 0.2).toFixed(2) + ' V';
    if (i) i.innerText = (1.95 + Math.random() * 0.15).toFixed(2) + ' A';
    if (t) t.innerText = (28.5 + Math.random() * 0.6).toFixed(1) + ' °C';
  }
}, 500);

const simBtn = document.getElementById('sim-mode-btn');
if (simBtn) {
  simBtn.addEventListener('click', function () {
    setTimeout(function () {
      const isSimOn = simBtn.innerText.toLowerCase().includes('on') && !simBtn.innerText.toLowerCase().includes('off');
      localStorage.setItem('simulatedMode', isSimOn ? 'true' : 'false');
      window.dispatchEvent(new Event('storage'));
    }, 100);
  });
}