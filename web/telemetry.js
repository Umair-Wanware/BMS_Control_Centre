let pageSimInterval = null;

function updatePageData() {
  console.log('Simulated Data Updating...');
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

  if (isSimOn) {
    startSimulation();
  } else {
    stopSimulation();
  }
}

function toggleSimulatedMode() {
  let isSimOn = localStorage.getItem('simulatedMode') === 'true';
  localStorage.setItem('simulatedMode', (!isSimOn).toString());
  syncSimMode();
}

window.addEventListener('DOMContentLoaded', syncSimMode);

window.addEventListener('storage', (e) => {
  if (e.key === 'simulatedMode') {
    syncSimMode();
  }
});

let simInterval = null;

function updateStatus() {
  const statusTag = document.getElementById('status-pill');
  const isSimOn = localStorage.getItem('simulatedMode') === 'true';

  if (statusTag) {
    if (isSimOn) {
      statusTag.innerHTML = '🟢 Online';
      startTelemetrySim();
    } else {
      statusTag.innerHTML = '🔴 Offline';
      stopTelemetrySim();
    }
  }
}

function startTelemetrySim() {
  if (simInterval) return;
  simInterval = setInterval(() => {
    const voltage = (12.3 + Math.random() * 0.4).toFixed(2);
    const current = (2.1 + Math.random() * 0.5).toFixed(2);
    const temp = (30.8 + Math.random() * 1.2).toFixed(1);

    const vCard = document.querySelector('.voltage-value') || document.getElementById('volt-val');
    const cCard = document.querySelector('.current-value') || document.getElementById('curr-val');
    const tCard = document.querySelector('.temp-value') || document.getElementById('temp-val');

    if (vCard) vCard.innerText = voltage + ' V';
    if (cCard) cCard.innerText = current + ' A';
    if (tCard) tCard.innerText = temp + ' °C';
  }, 1000);
}

function stopTelemetrySim() {
  if (simInterval) {
    clearInterval(simInterval);
    simInterval = null;
  }
}

updateStatus();
window.addEventListener('storage', updateStatus);