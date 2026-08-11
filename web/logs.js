function updateLogsStatus() {
  // Both 'status-tag' and 'status-pill' support
  const tag = document.getElementById('status-tag') || document.getElementById('status-pill');
  if (!tag) return;

  const isSimOn =
    localStorage.getItem('simulatedMode') === 'true' ||
    localStorage.getItem('systemStatus') === 'online';

  if (isSimOn) {
    tag.innerHTML = '🟢 Online';
    tag.style.color = '#4ade80';
    tag.style.backgroundColor = '#143823';
  } else {
    tag.innerHTML = '🔴 Offline';
    tag.style.color = '#f87171';
    tag.style.backgroundColor = '#1e293b';
  }
}

// Page load aur storage change par sync
window.addEventListener('DOMContentLoaded', updateLogsStatus);
window.addEventListener('storage', updateLogsStatus);

let pageSimInterval = null;

// Live Naye Logs add karne ka logic
function updatePageData() {
  const isSimOn =
    localStorage.getItem('simulatedMode') === 'true' ||
    localStorage.getItem('systemStatus') === 'online';
  if (!isSimOn) return;

  const logTable = document.querySelector('tbody') || document.querySelector('.logs-list');
  if (!logTable) return;

  const levels = ['INFO', 'WARN', 'INFO', 'ERROR', 'INFO'];
  const messages = [
    'Cell voltage balanced (3.42V)',
    'Temp sensor 2 warning: 41°C',
    'BMS heartbeat ping OK',
    'Current spike detected (12.4A)',
    'SOC re-calibrated successfully',
  ];

  const randIdx = Math.floor(Math.random() * levels.length);
  const timeStr = new Date().toLocaleTimeString();

  const newRow = document.createElement('tr');
  newRow.innerHTML = `
    <td>${timeStr}</td>
    <td><span class="badge ${levels[randIdx].toLowerCase()}">${levels[randIdx]}</span></td>
    <td>${messages[randIdx]}</td>
  `;

  // Sabse upar naya log row add hoga
  logTable.insertBefore(newRow, logTable.firstChild);

  // Maximum 15 logs rakhenge taaki page scroll na kharab ho
  if (logTable.children.length > 15) {
    logTable.removeChild(logTable.lastChild);
  }
}

function startSimulation() {
  if (!pageSimInterval) {
    pageSimInterval = setInterval(updatePageData, 2000); // Har 2 sec me update
  }
}

function stopSimulation() {
  if (pageSimInterval) {
    clearInterval(pageSimInterval);
    pageSimInterval = null;
  }
}

// Simulation Start
startSimulation();