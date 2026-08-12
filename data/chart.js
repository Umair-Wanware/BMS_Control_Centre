function updateChartsStatus() {
  const tag = document.getElementById('status-tag');
  if (!tag) return;
  const savedStatus = localStorage.getItem('systemStatus');
  if (savedStatus === 'online') {
    tag.innerHTML = '🟢 Online';
    tag.style.color = '#4ade80';
    tag.style.backgroundColor = '#143823';
  } else {
    tag.innerHTML = '🔴 Offline';
    tag.style.color = '#f87171';
    tag.style.backgroundColor = '#1e293b';
  }
}

document.getElementById('stm').innerHTML = "STM32 OTA System";
window.onload = updateChartsStatus;
window.addEventListener('storage', updateChartsStatus);

let pageSimInterval = null;
let phase = 0; // Wave animation ke liye

function drawGraphs() {
  phase += 0.2;

  // 1. Blue aur Green Lines ko wave ki tarah move karne ka logic
  const paths = document.querySelectorAll('.svg-container path');
  if (paths.length >= 2) {
    const v1 = 100 + Math.sin(phase) * 20;
    const v2 = 60 + Math.cos(phase) * 20;
    const v3 = 90 + Math.sin(phase + 1) * 20;
    paths[0].setAttribute(
      'd',
      `M0,${v1.toFixed(1)} Q60,${v2.toFixed(1)} 120,${v1.toFixed(1)} T240,${v2.toFixed(1)} T360,${v3.toFixed(1)} T500,50`
    );

    const c1 = 150 + Math.cos(phase) * 15;
    const c2 = 120 + Math.sin(phase) * 15;
    const c3 = 140 + Math.cos(phase + 1) * 15;
    paths[1].setAttribute(
      'd',
      `M0,${c1.toFixed(1)} Q60,${c2.toFixed(1)} 120,${c1.toFixed(1)} T240,${c2.toFixed(1)} T360,${c3.toFixed(1)} T500,110`
    );
  }

  // 2. Bars (Temperature aur State of Charge) ko upar-neeche hilaane ka logic
  const bars = document.querySelectorAll('.bar');
  bars.forEach((bar, i) => {
    const base = 60 + (i % 5) * 7;
    const val = Math.min(98, Math.max(30, base + Math.sin(phase + i) * 12));
    bar.style.height = `${val.toFixed(0)}%`;
    bar.style.transition = 'height 0.4s ease'; // Smooth animation
  });
}

// Har page ke apne custom updates yahan likhein (e.g. Logs update, Charts update)
function updatePageData() {
  // Example: Agar ye Telemetry / Chart page hai toh graphs re-render honge
  console.log('Simulated Data Updating...');

  // Aapka page-specific update code yahan aayega
  if (typeof drawGraphs === 'function') drawGraphs();
}

function startSimulation() {
  if (!pageSimInterval) {
    pageSimInterval = setInterval(updatePageData, 500); // 500ms par update
  }
}

function stopSimulation() {
  if (pageSimInterval) {
    clearInterval(pageSimInterval);
    pageSimInterval = null; // Freeze data
  }
}

// UI aur Simulation Sync Control
function syncSimMode() {
  const isSimOn = localStorage.getItem('simulatedMode') === 'true';
  const statusTag = document.getElementById('status-pill');

  // Status tag update
  if (statusTag) {
    statusTag.innerHTML = isSimOn ? '🟢 Online' : '🔴 Offline';
  }

  // Live simulation control
  if (isSimOn) {
    startSimulation();
  } else {
    stopSimulation();
  }
}

// Page load hone par sync karega
syncSimMode();

// Dashboard toggle par sync karega
window.addEventListener('storage', syncSimMode);

// Toggle Function (Button click karne par)
function toggleSimulatedMode() {
  const isSimOn = localStorage.getItem('simulatedMode') === 'true';
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