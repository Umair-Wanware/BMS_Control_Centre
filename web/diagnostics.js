function updateDiagStatus() {
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

window.onload = updateDiagStatus;
window.addEventListener('storage', updateDiagStatus);

let diagInterval = null;

function updateDiagnosticsData() {
  const isSimOn =
    localStorage.getItem('simulatedMode') === 'true' ||
    localStorage.getItem('systemStatus') === 'online';
  if (!isSimOn) return;

  // Live values simulation logic (agar page me dynamic cards hain)
}

function syncDiagnostics() {
  const isSimOn =
    localStorage.getItem('simulatedMode') === 'true' ||
    localStorage.getItem('systemStatus') === 'online';
  const statusTag = document.getElementById('status-tag') || document.getElementById('status-pill');

  // 1. Status tag update
  if (statusTag) {
    statusTag.innerHTML = isSimOn ? '🟢 Online' : '🔴 Offline';
    statusTag.style.color = isSimOn ? '#4ade80' : '#f87171';
    statusTag.style.backgroundColor = isSimOn ? '#143823' : '#1e293b';
  }

  // 2. Simulation control
  if (isSimOn) {
    if (!diagInterval) {
      updateDiagnosticsData();
      diagInterval = setInterval(updateDiagnosticsData, 2000);
    }
  } else {
    if (diagInterval) {
      clearInterval(diagInterval);
      diagInterval = null;
    }
  }
}

// Event Listeners
document.addEventListener('DOMContentLoaded', syncDiagnostics);
window.addEventListener('storage', syncDiagnostics);

// Initial Run
syncDiagnostics();