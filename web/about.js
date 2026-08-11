function updateAboutStatus() {
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

window.onload = updateAboutStatus;
window.addEventListener('storage', updateAboutStatus);

function syncAboutPage() {
  const isSimOn =
    localStorage.getItem('simulatedMode') === 'true' ||
    localStorage.getItem('systemStatus') === 'online';
  const statusTag = document.getElementById('status-tag') || document.getElementById('status-pill');

  // 1. Status Tag Update (🟢 Online / 🔴 Offline)
  if (statusTag) {
    statusTag.innerHTML = isSimOn ? '🟢 Online' : '🔴 Offline';
    statusTag.style.color = isSimOn ? '#4ade80' : '#f87171';
    statusTag.style.backgroundColor = isSimOn ? '#143823' : '#1e293b';
  }
}

// 2. Page load hone par sync karo
document.addEventListener('DOMContentLoaded', syncAboutPage);

// 3. Dashboard se toggle hone par live sync karo
window.addEventListener('storage', (e) => {
  if (e.key === 'simulatedMode' || e.key === 'systemStatus') {
    syncAboutPage();
  }
});

// Initial Run
syncAboutPage();