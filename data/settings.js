function syncSettings() {
    const isSimOn = localStorage.getItem('simulatedMode') === 'true' || localStorage.getItem('systemStatus') === 'online';
    const statusTag = document.getElementById('status-tag') || document.getElementById('status-pill');
    
    // Page ke saare inputs, selects aur buttons find karna
    const inputs = document.querySelectorAll('input, select, button');

    // 1. Status Tag Update
    if (statusTag) {
      statusTag.innerHTML = isSimOn ? '🟢 Online' : '🔴 Offline';
      statusTag.style.color = isSimOn ? '#4ade80' : '#f87171';
      statusTag.style.backgroundColor = isSimOn ? '#143823' : '#1e293b';
    }

    // 2. Offline me Settings lock / Online me unlock
    inputs.forEach(input => {
      // Agar tag/pill itself button hai toh use skip karo
      if (input.id === 'status-tag' || input.id === 'status-pill') return;

      input.disabled = !isSimOn;
      input.style.opacity = isSimOn ? '1' : '0.5';
      input.style.cursor = isSimOn ? 'pointer' : 'not-allowed';
    });
  }

  // Event Listeners for Live Sync
  document.addEventListener('DOMContentLoaded', syncSettings);
  window.addEventListener('storage', (e) => {
    if (e.key === 'simulatedMode' || e.key === 'systemStatus') {
      syncSettings();
    }
  });

  // Initial Call
  syncSettings();
  let currentAction = '';

  function toggleDangerZone(isUnlocked) {
    const lockText = document.getElementById('lockStatusText');
    const buttons = document.querySelectorAll('.danger-btn');

    if (isUnlocked) {
      lockText.innerText = '🔓 Unlocked';
      lockText.style.color = '#4ade80';
      buttons.forEach(btn => {
        btn.disabled = false;
        btn.style.opacity = '1';
        btn.style.cursor = 'pointer';
      });
    } else {
      lockText.innerText = '🔒 Locked';
      lockText.style.color = '#f87171';
      buttons.forEach(btn => {
        btn.disabled = true;
        btn.style.opacity = '0.5';
        btn.style.cursor = 'not-allowed';
      });
    }
  }

  function triggerAction(action) {
    if (action === 'reboot') {
      if (confirm('Reboot ESP32 microcontroller?')) {
        alert('Reboot signal sent to ESP32!');
      }
    } else if (action === 'clearLogs') {
      if (confirm('Clear all diagnostic logs?')) {
        alert('System logs cleared!');
      }
    }
  }

  function openConfirmModal(action) {
    currentAction = action;
    const modal = document.getElementById('dangerModal');
    const desc = document.getElementById('modalDesc');
    const reqText = document.getElementById('requiredText');
    const input = document.getElementById('confirmInput');
    const finalBtn = document.getElementById('finalConfirmBtn');

    input.value = '';
    finalBtn.disabled = true;
    finalBtn.style.opacity = '0.5';
    finalBtn.style.cursor = 'not-allowed';

    if (action === 'factoryReset') {
      desc.innerText = 'This will reset all cell voltage cutoffs, temperature limits, and user configurations to factory defaults.';
      reqText.innerText = 'RESET';
    } else if (action === 'cutoff') {
      desc.innerText = 'This will forcefully open all Charge/Discharge MOSFETs and stop battery output immediately.';
      reqText.innerText = 'STOP';
    }

    modal.style.display = 'flex';
  }

  function closeConfirmModal() {
    document.getElementById('dangerModal').style.display = 'none';
  }

  function validateConfirmInput() {
    const inputVal = document.getElementById('confirmInput').value.trim();
    const reqText = document.getElementById('requiredText').innerText;
    const finalBtn = document.getElementById('finalConfirmBtn');

    if (inputVal === reqText) {
      finalBtn.disabled = false;
      finalBtn.style.opacity = '1';
      finalBtn.style.cursor = 'pointer';
    } else {
      finalBtn.disabled = true;
      finalBtn.style.opacity = '0.5';
      finalBtn.style.cursor = 'not-allowed';
    }
  }

  function executeDangerAction() {
    closeConfirmModal();
    if (currentAction === 'factoryReset') {
      alert('✅ System restored to factory defaults!');
    } else if (currentAction === 'cutoff') {
      alert('🚨 Emergency Cutoff Activated! Relays Opened.');
    }
  }

  document.getElementById('stm').innerHTML = "STM32 OTA System";