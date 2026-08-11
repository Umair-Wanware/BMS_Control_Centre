let selectedFile = null;

function handleFileSelect(e) {
  const file = e.target.files[0];
  const uploadBtn = document.getElementById('uploadBtn');
  const fileName = document.getElementById('fileName');

  if (file) {
    selectedFile = file;
    if (fileName) {
      fileName.innerText = `Selected: ${file.name}`;
      fileName.style.color = '#4ade80';
    }
    if (uploadBtn) uploadBtn.disabled = false;
  }
}

function checkForUpdates(btn) {
  btn.innerText = 'Checking...';
  btn.disabled = true;

  setTimeout(() => {
    btn.innerText = 'Check for Updates';
    btn.disabled = false;
    alert('System is already up to date! (v2.5.3)');
  }, 1500);
}

function startFlashing() {
  const isSimOn =
    localStorage.getItem('simulatedMode') === 'true' ||
    localStorage.getItem('systemStatus') === 'online';

  if (!isSimOn) {
    alert('System Offline hai! Pehle Dashboard se Online karein.');
    return;
  }

  const progressContainer = document.getElementById('progressContainer');
  const progressBar = document.getElementById('progressBar');
  const flashPercent = document.getElementById('flashPercent');
  const flashStatusText = document.getElementById('flashStatusText');
  const uploadBtn = document.getElementById('uploadBtn');

  if (progressContainer) progressContainer.style.display = 'block';
  if (uploadBtn) uploadBtn.disabled = true;

  let progress = 0;
  const interval = setInterval(() => {
    progress += 10;
    if (progressBar) progressBar.style.width = progress + '%';
    if (flashPercent) flashPercent.innerText = progress + '%';

    if (progress === 20 && flashStatusText) flashStatusText.innerText = 'Erasing flash memory...';
    if (progress === 50 && flashStatusText) flashStatusText.innerText = 'Writing binary blocks to ESP32...';
    if (progress === 80 && flashStatusText) flashStatusText.innerText = 'Verifying checksum...';

    if (progress >= 100) {
      clearInterval(interval);
      if (flashStatusText) {
        flashStatusText.innerText = '✅ Flashing Completed Successfully!';
        flashStatusText.style.color = '#4ade80';
      }
    }
  }, 400);
}

function syncFirmwarePage() {
  const isSimOn =
    localStorage.getItem('simulatedMode') === 'true' ||
    localStorage.getItem('systemStatus') === 'online';
  const statusTag = document.getElementById('status-pill') || document.getElementById('status-tag');

  if (statusTag) {
    statusTag.innerHTML = isSimOn ? '🟢 Online' : '🔴 Offline';
    statusTag.style.color = isSimOn ? '#4ade80' : '#f87171';
    statusTag.style.backgroundColor = isSimOn ? '#143823' : '#1e293b';
  }
}

document.addEventListener('DOMContentLoaded', syncFirmwarePage);
window.addEventListener('storage', syncFirmwarePage);
setInterval(syncFirmwarePage, 1000);
syncFirmwarePage();