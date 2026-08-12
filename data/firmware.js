let selectedFile = null;
let firmwarePoller = null;

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

async function checkForUpdates(btn) {
  btn.innerText = 'Checking...';
  btn.disabled = true;

  try {
    const response = await fetch('/api/firmware', { cache: 'no-store' });
    const payload = await response.json();
    const status = payload.state || 'Idle';
    alert(`Firmware status: ${status}`);
  } catch (error) {
    alert('Firmware status endpoint is unavailable.');
  }

  btn.innerText = 'Check for Updates';
  btn.disabled = false;
}

async function uploadFirmwareFile() {
  if (!selectedFile) {
    throw new Error('No firmware file selected');
  }

  // Convert File to ArrayBuffer for proper transmission
  const arrayBuffer = await selectedFile.arrayBuffer();
  console.log('Uploading', selectedFile.name, 'size:', arrayBuffer.byteLength, 'bytes');
  
  const response = await fetch('/api/firmware/upload', {
    method: 'POST',
    headers: {
      'Content-Type': 'application/octet-stream'
    },
    body: arrayBuffer
  });

  if (!response.ok) {
    const text = await response.text();
    console.error('Upload HTTP error:', response.status, text);
    throw new Error(`Upload failed (${response.status}): ${text}`);
  }

  let payload;
  try {
    const text = await response.text();
    if (!text) {
      console.error('Upload response is empty');
      throw new Error('Server returned empty response');
    }
    console.log('Upload response:', text);
    payload = JSON.parse(text);
  } catch (e) {
    console.error('Failed to parse upload response:', e);
    throw new Error(`Invalid JSON: ${e.message}`);
  }

  if (payload.status !== 'uploaded') {
    throw new Error(payload.error || 'Upload status not confirmed');
  }
}

async function startFirmwareUpdate() {
  const body = JSON.stringify({ path: '/firmware.bin' });
  console.log('Starting firmware update with body:', body);
  
  const response = await fetch('/api/firmware/start', {
    method: 'POST',
    headers: {
      'Content-Type': 'application/json'
    },
    body: body
  });

  if (!response.ok) {
    const text = await response.text();
    console.error('Start HTTP error:', response.status, text);
    throw new Error(`Start failed (${response.status}): ${text}`);
  }

  let payload;
  try {
    const text = await response.text();
    if (!text) {
      console.error('Start response is empty');
      throw new Error('Server returned empty response');
    }
    console.log('Start response:', text);
    payload = JSON.parse(text);
  } catch (e) {
    console.error('Failed to parse start response:', e);
    throw new Error(`Invalid JSON: ${e.message}`);
  }

  if (payload.status !== 'started') {
    throw new Error(payload.error || 'Start status not confirmed');
  }
}

function updateProgressUI(state, progress) {
  const progressContainer = document.getElementById('progressContainer');
  const progressBar = document.getElementById('progressBar');
  const flashPercent = document.getElementById('flashPercent');
  const flashStatusText = document.getElementById('flashStatusText');
  const uploadBtn = document.getElementById('uploadBtn');

  if (progressContainer) progressContainer.style.display = 'block';
  if (progressBar) progressBar.style.width = `${progress}%`;
  if (flashPercent) flashPercent.innerText = `${progress}%`;

  if (flashStatusText) {
    const mapping = {
      Idle: 'Waiting for actions...',
      CalculatingChecksum: 'Calculating checksum...',
      Starting: 'Starting firmware update...',
      Transferring: 'Writing firmware blocks...',
      Verifying: 'Verifying image...',
      Complete: '✅ Firmware update completed successfully!',
      Failed: '❌ Firmware update failed.',
      Cancelled: '⚠️ Firmware update cancelled.'
    };
    flashStatusText.innerText = mapping[state] || state;
    flashStatusText.style.color = state === 'Complete' ? '#4ade80' : '#e2e8f0';
  }

  if (uploadBtn) {
    uploadBtn.disabled = state === 'Starting' || state === 'Transferring' || state === 'Verifying';
  }
}

function stopFirmwarePolling() {
  if (firmwarePoller) {
    clearInterval(firmwarePoller);
    firmwarePoller = null;
  }
}

async function pollFirmwareStatus() {
  try {
    const response = await fetch('/api/firmware', { cache: 'no-store' });
    const payload = await response.json();
    const progress = Number(payload.progress || 0);
    updateProgressUI(payload.state || 'Idle', Math.min(progress, 100));

    if (payload.running === false && payload.state !== 'Starting' && payload.state !== 'Transferring' && payload.state !== 'Verifying') {
      stopFirmwarePolling();
    }
  } catch (error) {
    updateProgressUI('Failed', 0);
    stopFirmwarePolling();
  }
}

async function startFlashing() {
  if (!selectedFile) {
    alert('Please choose a firmware binary first.');
    return;
  }

  const progressContainer = document.getElementById('progressContainer');
  const progressBar = document.getElementById('progressBar');
  const flashPercent = document.getElementById('flashPercent');
  const flashStatusText = document.getElementById('flashStatusText');
  const uploadBtn = document.getElementById('uploadBtn');

  if (progressContainer) progressContainer.style.display = 'block';
  if (uploadBtn) uploadBtn.disabled = true;
  if (flashStatusText) flashStatusText.innerText = 'Uploading firmware...';
  if (progressBar) progressBar.style.width = '0%';
  if (flashPercent) flashPercent.innerText = '0%';

  try {
    await uploadFirmwareFile();
    if (flashStatusText) flashStatusText.innerText = 'Starting update on STM32...';
    await startFirmwareUpdate();
    firmwarePoller = setInterval(pollFirmwareStatus, 1200);
    pollFirmwareStatus();
  } catch (error) {
    if (flashStatusText) {
      flashStatusText.innerText = `❌ ${error.message}`;
      flashStatusText.style.color = '#f87171';
    }
    if (uploadBtn) uploadBtn.disabled = false;
    stopFirmwarePolling();
  }
}

function syncFirmwarePage() {
  const statusTag = document.getElementById('status-pill') || document.getElementById('status-tag');
  const isOnline = !isSimulatedMode();

  if (statusTag) {
    statusTag.innerHTML = isOnline ? '🟢 Online' : '🟡 Simulated';
    statusTag.style.color = isOnline ? '#4ade80' : '#fbbf24';
    statusTag.style.backgroundColor = isOnline ? '#143823' : '#1e293b';
  }
}

document.addEventListener('DOMContentLoaded', () => {
  ensureSimulatedModeDefault();
  const stmLabel = document.getElementById('stm');
  if (stmLabel) {
    stmLabel.innerHTML = 'Select a .bin file to flash new firmware to STM32.';
  }
  syncFirmwarePage();
});

window.addEventListener('storage', syncFirmwarePage);
setInterval(syncFirmwarePage, 1000);