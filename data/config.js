function updateConfigStatus() {
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

window.onload = updateConfigStatus;
window.addEventListener('storage', updateConfigStatus);

function syncSimMode() {
  const isSimOn = localStorage.getItem('simulatedMode') === 'true';
  const statusTag = document.getElementById('status-pill');
  const saveBtn = document.querySelector('button');
  const inputs = document.querySelectorAll('input, select');

  // 1. Status tag update (🟢 Online / 🔴 Offline)
  if (statusTag) {
    statusTag.innerHTML = isSimOn ? '🟢 Online' : '🔴 Offline';
  }

  // 2. Form Inputs aur Save Button disable/enable control
  if (isSimOn) {
    if (saveBtn) {
      saveBtn.disabled = false;
      saveBtn.style.opacity = '1';
      saveBtn.style.cursor = 'pointer';
    }
    inputs.forEach((input) => {
      input.disabled = false;
      input.style.opacity = '1';
    });
  } else {
    if (saveBtn) {
      saveBtn.disabled = true;
      saveBtn.style.opacity = '0.5';
      saveBtn.style.cursor = 'not-allowed';
    }
    inputs.forEach((input) => {
      input.disabled = true;
      input.style.opacity = '0.5';
    });
  }
}

// 1. Page load hone par sync karo
window.addEventListener('DOMContentLoaded', syncSimMode);
document.getElementById('stm').innerHTML = "STM32 OTA System";

// 2. Dashboard se toggle hone par sync karo
window.addEventListener('storage', (e) => {
  if (e.key === 'simulatedMode') {
    syncSimMode();
  }
});