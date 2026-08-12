#include "embedded_web_assets.hpp"

#include <string.h>

namespace control_hub::dashboard {
namespace {

static const char kAsset_about_css[] = R"WEB_397790d5eb(
* {
  box-sizing: border-box;
  margin: 0;
  padding: 0;
  font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, sans-serif;
}

html,
body {
  height: 100vh;
  width: 100vw;
  overflow: hidden; /* Scrollbar off */
  background-color: #0b111e;
  color: #ffffff;
}

body {
  display: flex;
}

/* Sidebar Area */
.sidebar {
  width: 240px;
  height: 100vh;
  background-color: #0f172a;
  padding: 16px;
  display: flex;
  flex-direction: column;
  gap: 6px;
  border-right: 1px solid #1e293b;
}

.brand-box {
  background-color: #ffffff;
  color: #0f172a;
  padding: 12px;
  border-radius: 12px;
  margin-bottom: 12px;
}

.brand-box h3 {
  font-size: 14px;
  font-weight: 700;
}

.brand-box p {
  font-size: 11px;
  color: #64748b;
}

.nav-btn {
  display: flex;
  align-items: center;
  gap: 12px;
  padding: 10px 14px;
  border-radius: 10px;
  color: #94a3b8;
  text-decoration: none;
  font-size: 13px;
  font-weight: 500;
  transition: 0.2s;
}

.nav-btn.active {
  background-color: #0e3a40;
  color: #38bdf8;
}

.nav-btn:hover:not(.active) {
  background-color: #1e293b;
  color: #ffffff;
}

/* Main Workspace */
.main-content {
  flex: 1;
  height: 100vh;
  padding: 20px 28px;
  display: flex;
  flex-direction: column;
  gap: 16px;
}

.top-bar {
  display: flex;
  justify-content: space-between;
  align-items: center;
}

.page-title span {
  font-size: 11px;
  color: #38bdf8;
  text-transform: uppercase;
  letter-spacing: 1px;
  font-weight: 700;
}

.page-title h2 {
  font-size: 22px;
  margin-top: 2px;
}

.tag {
  background-color: #1e293b;
  padding: 6px 14px;
  border-radius: 20px;
  font-size: 12px;
  color: #cbd5e1;
}

/* About Content Grid */
.about-grid {
  display: grid;
  grid-template-columns: 1fr 1fr;
  gap: 16px;
  flex: 1;
}

.card {
  background-color: #111a2e;
  border: 1px solid #1e293b;
  border-radius: 16px;
  padding: 24px;
  display: flex;
  flex-direction: column;
  gap: 16px;
}

.card-title {
  font-size: 14px;
  font-weight: 600;
  color: #38bdf8;
  border-bottom: 1px solid #1e293b;
  padding-bottom: 8px;
}

.info-list {
  display: flex;
  flex-direction: column;
  gap: 12px;
}

.info-row {
  display: flex;
  justify-content: space-between;
  align-items: center;
  font-size: 13px;
}

.info-label {
  color: #94a3b8;
}

.info-val {
  color: #f8fafc;
  font-weight: 600;
}

.highlight-val {
  color: #38bdf8;
}

.app-description {
  font-size: 13px;
  color: #cbd5e1;
  line-height: 1.6;
}

.feature-tags {
  display: flex;
  flex-wrap: wrap;
  gap: 8px;
  margin-top: 8px;
}

.feature-chip {
  background-color: #0b111e;
  border: 1px solid #1e293b;
  padding: 6px 12px;
  border-radius: 8px;
  font-size: 12px;
  color: #38bdf8;
}
)WEB_397790d5eb";

static const char kAsset_about_html[] = R"WEB_8d27d8b4e8(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>About - BMS Control Centre</title>
  <link rel="stylesheet" href="about.css">
</head>
<body>

  <!-- Left Sidebar -->
  <div class="sidebar">
    <div class="brand-box">
      <h3>BMS Control Centre</h3>
      <p id="stm">ESP32 Control Hub</p>
    </div>

    <a href="index.html" class="nav-btn">Dashboard</a>
    <a href="telemetry.html" class="nav-btn">Telemetry</a>
    <a href="charts.html" class="nav-btn">Charts</a>
    <a href="firmware.html" class="nav-btn">Firmware Update</a>
    <a href="config.html" class="nav-btn">Configuration</a>
    <a href="logs.html" class="nav-btn">Logs</a>
    <a href="diagnostics.html" class="nav-btn">Diagnostics</a>
    <a href="settings.html" class="nav-btn">Settings</a>
    <a href="about.html" class="nav-btn active">About</a>
  </div>

  <!-- Main Content Area -->
  <div class="main-content">
    
    <!-- Top Header Bar -->
    <div class="top-bar">
      <div class="page-title">
        <span>SYSTEM INFORMATION</span>
        <h2>About BMS Control Centre</h2>
      </div>

      <div class="top-tags">
<<<<<<< HEAD
        <span id="status-tag" class="tag">🔴 Offline</span>
        <span class="tag">v2.5.3</span>
=======
        <span id="status-pill" class="tag">🔴 Offline</span>
        <span class="tag">v1.2.4</span>
>>>>>>> f9018d95f2a3fgfeF8667db79047735146f6b988951a
      </div>
    </div>

    <!-- About Grid -->
    <div class="about-grid">
      
      <!-- Card 1: System Specs & Build Info -->
      <div class="card">
        <div class="card-title">System & Microcontroller Info</div>
        
        <div class="info-list">
          <div class="info-row">
            <span class="info-label">Platform</span>
            <span class="info-val highlight-val">ESP32 Dual-Core (Xtensa 32-bit LX6)</span>
          </div>
          <div class="info-row">
            <span class="info-label">Firmware Version</span>
            <span class="info-val">v2.5.3-stable</span>
          </div>
          <div class="info-row">
            <span class="info-label">Web Interface Version</span>
            <span class="info-val">v2.0.0 (Dark Dashboard UI)</span>
          </div>
          <div class="info-row">
            <span class="info-label">Free Heap Memory</span>
            <span class="info-val">184 KB</span>
          </div>
          <div class="info-row">
            <span class="info-label">CPU Frequency</span>
            <span class="info-val">240 MHz</span>
          </div>
          <div class="info-row">
            <span class="info-label">Flash Memory Size</span>
            <span class="info-val">16 MB</span>
          </div>
        </div>
      </div>

      <!-- Card 2: Overview & Key Features -->
      <div class="card">
        <div class="card-title">Project Overview & Capabilities</div>

        <p class="app-description">
          BMS Control Centre is a real-time monitoring and control dashboard designed for Lithium Battery Management Systems (BMS) powered by ESP32 microcontrollers.
        </p>

        <div class="feature-tags">
          <span class="feature-chip">⚡ Realtime Telemetry</span>
          <span class="feature-chip">📊 Live Cell Voltage Charts</span>
          <span class="feature-chip">🛡️ Thermal Safety Protection</span>
          <span class="feature-chip">🔄 Active / Passive Balancing</span>
          <span class="feature-chip">🌐 Wi-Fi & WebSockets</span>
          <span class="feature-chip">📝 System Logging</span>
        </div>
      </div>
    </div>
  </div>
  <script src="about.js"></script>
</body>
</html>
)WEB_8d27d8b4e8";

static const char kAsset_about_js[] = R"WEB_229aedfe43(
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

document.getElementById('stm').innerHTML = "STM32 OTA System";
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
)WEB_229aedfe43";

static const char kAsset_chart_css[] = R"WEB_46de026062(
* {
  box-sizing: border-box;
  margin: 0;
  padding: 0;
  font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, sans-serif;
}

html,
body {
  height: 100vh;
  width: 100vw;
  overflow: hidden; /* Scrollbar off */
  background-color: #0b111e;
  color: #ffffff;
}

body {
  display: flex;
}

/* Sidebar Area */
.sidebar {
  width: 240px;
  height: 100vh;
  background-color: #0f172a;
  padding: 16px;
  display: flex;
  flex-direction: column;
  gap: 6px;
  border-right: 1px solid #1e293b;
}

.brand-box {
  background-color: #ffffff;
  color: #0f172a;
  padding: 12px;
  border-radius: 12px;
  margin-bottom: 12px;
}

.brand-box h3 {
  font-size: 14px;
  font-weight: 700;
}

.brand-box p {
  font-size: 11px;
  color: #64748b;
}

.nav-btn {
  display: flex;
  align-items: center;
  gap: 12px;
  padding: 10px 14px;
  border-radius: 10px;
  color: #94a3b8;
  text-decoration: none;
  font-size: 13px;
  font-weight: 500;
  transition: 0.2s;
}

.nav-btn.active {
  background-color: #0e3a40;
  color: #38bdf8;
}

.nav-btn:hover:not(.active) {
  background-color: #1e293b;
  color: #ffffff;
}

/* Main Workspace */
.main-content {
  flex: 1;
  height: 100vh;
  padding: 20px 28px;
  display: flex;
  flex-direction: column;
  gap: 16px;
}

.top-bar {
  display: flex;
  justify-content: space-between;
  align-items: center;
}

.page-title span {
  font-size: 11px;
  color: #38bdf8;
  text-transform: uppercase;
  letter-spacing: 1px;
  font-weight: 700;
}

.page-title h2 {
  font-size: 22px;
  margin-top: 2px;
}

.top-tags {
  display: flex;
  gap: 8px;
}

.tag {
  background-color: #1e293b;
  padding: 6px 14px;
  border-radius: 20px;
  font-size: 12px;
  color: #cbd5e1;
}

/* Charts Grid Layout */
.charts-grid {
  display: grid;
  grid-template-columns: 2fr 1fr;
  grid-template-rows: repeat(2, 1fr);
  gap: 16px;
  flex: 1;
}

.chart-card {
  background-color: #111a2e;
  border: 1px solid #1e293b;
  border-radius: 16px;
  padding: 18px;
  display: flex;
  flex-direction: column;
  justify-content: space-between;
}

.chart-header {
  display: flex;
  justify-content: space-between;
  align-items: center;
  margin-bottom: 10px;
}

.chart-title {
  font-size: 14px;
  font-weight: 600;
  color: #ffffff;
}

.chart-badge {
  font-size: 11px;
  color: #38bdf8;
  background: rgba(56, 189, 248, 0.1);
  padding: 4px 8px;
  border-radius: 6px;
}

/* Visual Mock Graph Containers */
.graph-area {
  flex: 1;
  display: flex;
  align-items: flex-end;
  gap: 12px;
  padding-top: 10px;
}

.svg-container {
  width: 100%;
  height: 100%;
  display: flex;
  align-items: center;
}

svg {
  width: 100%;
  height: 100%;
}

/* Bar Charts for SOC / Temp */
.bar-group {
  flex: 1;
  display: flex;
  flex-direction: column;
  align-items: center;
  gap: 6px;
  height: 100%;
  justify-content: flex-end;
}

.bar {
  width: 100%;
  max-width: 24px;
  background: linear-gradient(180deg, #38bdf8 0%, #0284c7 100%);
  border-radius: 4px 4px 0 0;
}

.bar-label {
  font-size: 10px;
  color: #64748b;
}
)WEB_46de026062";

static const char kAsset_chart_js[] = R"WEB_bfc4b38e92(
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
)WEB_bfc4b38e92";

static const char kAsset_charts_html[] = R"WEB_8499250fa2(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Charts - BMS Control Centre</title>
  <link rel="stylesheet" href="chart.css">
</head>
<body>

  <!-- Left Sidebar -->
  <div class="sidebar">
    <div class="brand-box">
      <h3>BMS Control Centre</h3>
      <p id="stm">ESP32 Control Hub</p>
    </div>

    <a href="index.html" class="nav-btn">Dashboard</a>
    <a href="telemetry.html" class="nav-btn">Telemetry</a>
    <a href="charts.html" class="nav-btn active">Charts</a>
    <a href="firmware.html" class="nav-btn">Firmware Update</a>
    <a href="config.html" class="nav-btn">Configuration</a>
    <a href="logs.html" class="nav-btn">Logs</a>
    <a href="diagnostics.html" class="nav-btn">Diagnostics</a>
    <a href="settings.html" class="nav-btn">Settings</a>
    <a href="about.html" class="nav-btn">About</a>
  </div>

  <!-- Main Content Area -->
  <div class="main-content">
    
    <!-- Top Header Bar -->
    <div class="top-bar">
      <div class="page-title">
        <span>ANALYTICS</span>
        <h2>Battery Performance Trends</h2>
      </div>

      <div class="top-tags">
        <span id="status-pill" class="tag">🔴 Offline</span>
        <span class="tag">Live Telemetry Graph</span>
      </div>
    </div>

    <!-- Grid Area for Graphs -->
    <div class="charts-grid">
      
      <!-- Chart 1: Main Voltage & Current Trend (Large) -->
      <div class="chart-card" style="grid-row: span 2;">
        <div class="chart-header">
          <div class="chart-title">Voltage & Current Over Time</div>
          <span class="chart-badge">Live Stream</span>
        </div>
        <div class="graph-area">
          <div class="svg-container">
            <svg viewBox="0 0 500 200" preserveAspectRatio="none">
              <!-- Grid lines -->
              <line x1="0" y1="40" x2="500" y2="40" stroke="#1e293b" stroke-dasharray="4"/>
              <line x1="0" y1="90" x2="500" y2="90" stroke="#1e293b" stroke-dasharray="4"/>
              <line x1="0" y1="140" x2="500" y2="140" stroke="#1e293b" stroke-dasharray="4"/>
              
              <!-- Voltage Line (Blue) -->
              <path d="M0,100 Q60,70 120,110 T240,60 T360,90 T500,50" fill="none" stroke="#38bdf8" stroke-width="3"/>
              
              <!-- Current Line (Green) -->
              <path d="M0,150 Q60,130 120,160 T240,120 T360,140 T500,110" fill="none" stroke="#4ade80" stroke-width="3"/>
            </svg>
          </div>
        </div>
        <div style="display: flex; gap: 20px; font-size: 12px; margin-top: 10px;">
          <span style="color: #38bdf8;">● Voltage (V)</span>
          <span style="color: #4ade80;">● Current (A)</span>
        </div>
      </div>

      <!-- Chart 2: Temperature History -->
      <div class="chart-card">
        <div class="chart-header">
          <div class="chart-title">Temperature Trend (°C)</div>
          <span class="chart-badge">Max: 31.2°C</span>
        </div>
        <div class="graph-area">
          <div class="bar-group"><div class="bar" style="height: 60%; background: #f97316;"></div><span class="bar-label">1m</span></div>
          <div class="bar-group"><div class="bar" style="height: 65%; background: #f97316;"></div><span class="bar-label">2m</span></div>
          <div class="bar-group"><div class="bar" style="height: 70%; background: #ef4444;"></div><span class="bar-label">3m</span></div>
          <div class="bar-group"><div class="bar" style="height: 68%; background: #f97316;"></div><span class="bar-label">4m</span></div>
          <div class="bar-group"><div class="bar" style="height: 62%; background: #f97316;"></div><span class="bar-label">5m</span></div>
        </div>
      </div>

      <!-- Chart 3: SOC Discharge History -->
      <div class="chart-card">
        <div class="chart-header">
          <div class="chart-title">State of Charge (%)</div>
          <span class="chart-badge">87% Remaining</span>
        </div>
        <div class="graph-area">
          <div class="bar-group"><div class="bar" style="height: 95%;"></div><span class="bar-label">10m</span></div>
          <div class="bar-group"><div class="bar" style="height: 92%;"></div><span class="bar-label">20m</span></div>
          <div class="bar-group"><div class="bar" style="height: 90%;"></div><span class="bar-label">30m</span></div>
          <div class="bar-group"><div class="bar" style="height: 88%;"></div><span class="bar-label">40m</span></div>
          <div class="bar-group"><div class="bar" style="height: 87%;"></div><span class="bar-label">50m</span></div>
        </div>
      </div>

    </div>

  </div>
  <script src="chart.js"></script>
</body>
</html>
)WEB_8499250fa2";

static const char kAsset_config_css[] = R"WEB_31665bf1ac(
* {
  box-sizing: border-box;
  margin: 0;
  padding: 0;
  font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, sans-serif;
}

html,
body {
  height: 100vh;
  width: 100vw;
  overflow: hidden; /* Scrollbar off */
  background-color: #0b111e;
  color: #ffffff;
}

body {
  display: flex;
}

/* Sidebar Area */
.sidebar {
  width: 240px;
  height: 100vh;
  background-color: #0f172a;
  padding: 16px;
  display: flex;
  flex-direction: column;
  gap: 6px;
  border-right: 1px solid #1e293b;
}

.brand-box {
  background-color: #ffffff;
  color: #0f172a;
  padding: 12px;
  border-radius: 12px;
  margin-bottom: 12px;
}

.brand-box h3 {
  font-size: 14px;
  font-weight: 700;
}

.brand-box p {
  font-size: 11px;
  color: #64748b;
}

.nav-btn {
  display: flex;
  align-items: center;
  gap: 12px;
  padding: 10px 14px;
  border-radius: 10px;
  color: #94a3b8;
  text-decoration: none;
  font-size: 13px;
  font-weight: 500;
  transition: 0.2s;
}

.nav-btn.active {
  background-color: #0e3a40;
  color: #38bdf8;
}

.nav-btn:hover:not(.active) {
  background-color: #1e293b;
  color: #ffffff;
}

/* Main Workspace */
.main-content {
  flex: 1;
  height: 100vh;
  padding: 20px 28px;
  display: flex;
  flex-direction: column;
  gap: 16px;
}

.top-bar {
  display: flex;
  justify-content: space-between;
  align-items: center;
}

.page-title span {
  font-size: 11px;
  color: #38bdf8;
  text-transform: uppercase;
  letter-spacing: 1px;
  font-weight: 700;
}

.page-title h2 {
  font-size: 22px;
  margin-top: 2px;
}

.tag {
  background-color: #1e293b;
  padding: 6px 14px;
  border-radius: 20px;
  font-size: 12px;
  color: #cbd5e1;
}

/* Configuration Form Layout Grid */
.config-grid {
  display: grid;
  grid-template-columns: 1fr 1fr;
  gap: 16px;
  flex: 1;
}

.card {
  background-color: #111a2e;
  border: 1px solid #1e293b;
  border-radius: 16px;
  padding: 20px;
  display: flex;
  flex-direction: column;
  gap: 14px;
}

.card-title {
  font-size: 14px;
  font-weight: 600;
  color: #38bdf8;
  border-bottom: 1px solid #1e293b;
  padding-bottom: 8px;
}

.form-group {
  display: flex;
  flex-direction: column;
  gap: 6px;
}

.form-group label {
  font-size: 12px;
  color: #94a3b8;
}

.form-group input,
.form-group select {
  background-color: #0b111e;
  border: 1px solid #1e293b;
  border-radius: 8px;
  padding: 10px;
  color: #ffffff;
  font-size: 13px;
  outline: none;
}

.form-group input:focus,
.form-group select:focus {
  border-color: #38bdf8;
}

.btn-save {
  background-color: #38bdf8;
  color: #0b111e;
  border: none;
  padding: 12px;
  border-radius: 8px;
  font-weight: 600;
  cursor: pointer;
  margin-top: auto;
  transition: 0.2s;
}

.btn-save:hover {
  background-color: #7dd3fc;
}
)WEB_31665bf1ac";

static const char kAsset_config_html[] = R"WEB_96a60a5d67(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Configuration - BMS Control Centre</title>
  <link rel="stylesheet" href="config.css">
</head>
<body>

  <!-- Left Sidebar -->
  <div class="sidebar">
    <div class="brand-box">
      <h3>BMS Control Centre</h3>
      <p id="stm">ESP32 Control Hub</p>
    </div>

    <a href="index.html" class="nav-btn">Dashboard</a>
    <a href="telemetry.html" class="nav-btn">Telemetry</a>
    <a href="charts.html" class="nav-btn">Charts</a>
    <a href="firmware.html" class="nav-btn">Firmware Update</a>
    <a href="config.html" class="nav-btn active">Configuration</a>
    <a href="logs.html" class="nav-btn">Logs</a>
    <a href="diagnostics.html" class="nav-btn">Diagnostics</a>
    <a href="settings.html" class="nav-btn">Settings</a>
    <a href="about.html" class="nav-btn">About</a>
  </div>

  <!-- Main Content Area -->
  <div class="main-content">
    
    <!-- Top Header Bar -->
    <div class="top-bar">
      <div class="page-title">
        <span>SETTINGS & LIMITS</span>
        <h2>System Configuration</h2>
      </div>

      <div class="top-tags">
        <span id="status-pill" class="tag">🔴 Offline</span>
        <span class="tag">BMS Parameter Setup</span>
      </div>
    </div>

    <!-- Configuration Grid -->
    <div class="config-grid">
      
      <!-- Card 1: Voltage & Current Thresholds -->
      <div class="card">
        <div class="card-title">Voltage & Current Limits</div>
        
        <div class="form-group">
          <label>Max Cell Voltage Cutoff (V)</label>
          <input type="number" step="0.01" value="3.65">
        </div>

        <div class="form-group">
          <label>Min Cell Voltage Cutoff (V)</label>
          <input type="number" step="0.01" value="2.50">
        </div>

        <div class="form-group">
          <label>Max Discharge Current (A)</label>
          <input type="number" value="30">
        </div>

        <button class="btn-save">Save Voltage Limits</button>
      </div>

      <!-- Card 2: Temperature & Cell Settings -->
      <div class="card">
        <div class="card-title">Thermal & Pack Safety</div>

        <div class="form-group">
          <label>Over-Temperature Cutoff (°C)</label>
          <input type="number" value="55">
        </div>

        <div class="form-group">
          <label>Under-Temperature Cutoff (°C)</label>
          <input type="number" value="0">
        </div>

        <div class="form-group">
          <label>Cell Balancing Mode</label>
          <select>
            <option value="auto">Automatic Balancing</option>
            <option value="charging">Only While Charging</option>
            <option value="disabled">Disabled</option>
          </select>
        </div>

        <button class="btn-save">Save Thermal Limits</button>
      </div>

    </div>

  </div>

  <!-- Status Sync Script -->
<!-- Baki Pages (e.g. telemetry.html / charts.html / logs.html) Script -->
<script src="config.js"></script>
</body>
</html>
)WEB_96a60a5d67";

static const char kAsset_config_js[] = R"WEB_67b3179b28(
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
)WEB_67b3179b28";

static const char kAsset_diagnostics_css[] = R"WEB_3c606d970a(
* {
      box-sizing: border-box;
      margin: 0;
      padding: 0;
      font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, sans-serif;
    }

    html, body {
      height: 100vh;
      width: 100vw;
      overflow: hidden; /* Scrollbar off */
      background-color: #0b111e;
      color: #ffffff;
    }

    body {
      display: flex;
    }

    /* Sidebar Area */
    .sidebar {
      width: 240px;
      height: 100vh;
      background-color: #0f172a;
      padding: 16px;
      display: flex;
      flex-direction: column;
      gap: 6px;
      border-right: 1px solid #1e293b;
    }

    .brand-box {
      background-color: #ffffff;
      color: #0f172a;
      padding: 12px;
      border-radius: 12px;
      margin-bottom: 12px;
    }

    .brand-box h3 { font-size: 14px; font-weight: 700; }
    .brand-box p { font-size: 11px; color: #64748b; }

    .nav-btn {
      display: flex;
      align-items: center;
      gap: 12px;
      padding: 10px 14px;
      border-radius: 10px;
      color: #94a3b8;
      text-decoration: none;
      font-size: 13px;
      font-weight: 500;
      transition: 0.2s;
    * {
      box-sizing: border-box;
      margin: 0;
      padding: 0;
      font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, sans-serif;
    }

    html,
    body {
      height: 100vh;
      width: 100vw;
      overflow: hidden; /* Scrollbar off */
      background-color: #0b111e;
      color: #ffffff;
    }

    body {
      display: flex;
    }

    /* Sidebar Area */
    .sidebar {
      width: 240px;
      height: 100vh;
      background-color: #0f172a;
      padding: 16px;
      display: flex;
      flex-direction: column;
      gap: 6px;
      border-right: 1px solid #1e293b;
    }

    .brand-box {
      background-color: #ffffff;
      color: #0f172a;
      padding: 12px;
      border-radius: 12px;
      margin-bottom: 12px;
    }

    .brand-box h3 {
      font-size: 14px;
      font-weight: 700;
    }

    .brand-box p {
      font-size: 11px;
      color: #64748b;
    }

    .nav-btn {
      display: flex;
      align-items: center;
      gap: 12px;
      padding: 10px 14px;
      border-radius: 10px;
      color: #94a3b8;
      text-decoration: none;
      font-size: 13px;
      font-weight: 500;
      transition: 0.2s;
    }

    .nav-btn.active {
      background-color: #0e3a40;
      color: #38bdf8;
    }

    .nav-btn:hover:not(.active) {
      background-color: #1e293b;
      color: #ffffff;
    }

    /* Main Workspace */
    .main-content {
      flex: 1;
      height: 100vh;
      padding: 20px 28px;
      display: flex;
      flex-direction: column;
      gap: 16px;
    }

    .top-bar {
      display: flex;
      justify-content: space-between;
      align-items: center;
    }

    .page-title span {
      font-size: 11px;
      color: #38bdf8;
      text-transform: uppercase;
      letter-spacing: 1px;
      font-weight: 700;
    }

    .page-title h2 {
      font-size: 22px;
      margin-top: 2px;
    }

    .tag {
      background-color: #1e293b;
      padding: 6px 14px;
      border-radius: 20px;
      font-size: 12px;
      color: #cbd5e1;
    }

    /* Diagnostics Grid */
    .diag-grid {
      display: grid;
      grid-template-columns: 1fr 1fr;
      gap: 16px;
      flex: 1;
    }

    .card {
      background-color: #111a2e;
      border: 1px solid #1e293b;
      border-radius: 16px;
      padding: 20px;
      display: flex;
      flex-direction: column;
      gap: 14px;
    }

    .card-title {
      font-size: 14px;
      font-weight: 600;
      color: #38bdf8;
      border-bottom: 1px solid #1e293b;
      padding-bottom: 8px;
      display: flex;
      justify-content: space-between;
      align-items: center;
    }

    .diag-list {
      display: flex;
      flex-direction: column;
      gap: 12px;
    }

    .diag-item {
      display: flex;
      justify-content: space-between;
      align-items: center;
      background-color: #0b111e;
      padding: 12px 14px;
      border-radius: 10px;
      border: 1px solid #1e293b;
    }

    .diag-info {
      display: flex;
      flex-direction: column;
      gap: 2px;
    }

    .diag-name {
      font-size: 13px;
      font-weight: 600;
      color: #f8fafc;
    }

    .diag-desc {
      font-size: 11px;
      color: #64748b;
    }

    .status-badge {
      padding: 4px 10px;
      border-radius: 20px;
      font-size: 11px;
      font-weight: 600;
    }

    .status-ok {
      background-color: rgba(74, 222, 128, 0.15);
      color: #4ade80;
    }

    .status-warn {
      background-color: rgba(245, 158, 11, 0.15);
      color: #f59e0b;
    }

    .status-error {
      background-color: rgba(248, 113, 113, 0.15);
      color: #f87171;
    }

    .btn-run {
      background-color: #38bdf8;
      color: #0b111e;
      /* Lines 179-186 omitted */
    }

    .btn-run:hover {
      background-color: #334155;
      color: #ffffff;
    }
}
)WEB_3c606d970a";

static const char kAsset_diagnostics_html[] = R"WEB_d862696357(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Diagnostics - BMS Control Centre</title>
  <link rel="stylesheet" href="diagnostics.css">
</head>
<body>

  <!-- Left Sidebar -->
  <div class="sidebar">
    <div class="brand-box">
      <h3>BMS Control Centre</h3>
      <p id="stm">ESP32 Control Hub</p>
    </div>

    <a href="index.html" class="nav-btn">Dashboard</a>
    <a href="telemetry.html" class="nav-btn">Telemetry</a>
    <a href="charts.html" class="nav-btn">Charts</a>
    <a href="firmware.html" class="nav-btn">Firmware Update</a>
    <a href="config.html" class="nav-btn">Configuration</a>
    <a href="logs.html" class="nav-btn">Logs</a>
    <a href="diagnostics.html" class="nav-btn active">Diagnostics</a>
    <a href="settings.html" class="nav-btn">Settings</a>
    <a href="about.html" class="nav-btn">About</a>
  </div>

  <!-- Main Content Area -->
  <div class="main-content">
    
    <!-- Top Header Bar -->
    <div class="top-bar">
      <div class="page-title">
        <span>HARDWARE & SELF-TEST</span>
        <h2>System Diagnostics</h2>
      </div>

      <div class="top-tags">
        <span id="status-pill" class="tag">🔴 Offline</span>
        <span class="tag">Self-Test Ready</span>
      </div>
    </div>

    <!-- Diagnostics Grid -->
    <div class="diag-grid">
      
      <!-- Card 1: Hardware Status -->
      <div class="card">
        <div class="card-title">
          <span>Hardware Subsystems</span>
          <span style="font-size: 11px; color: #64748b;">4 Active</span>
        </div>

        <div class="diag-list">
          <div class="diag-item">
            <div class="diag-info">
              <span class="diag-name">MOSFET Switch Array</span>
              <span class="diag-desc">Charge/Discharge Protection</span>
            </div>
            <span class="status-badge status-ok">NORMAL</span>
          </div>

          <div class="diag-item">
            <div class="diag-info">
              <span class="diag-name">Cell Balance Controller</span>
              <span class="diag-desc">Passive Balancing Circuit</span>
            </div>
            <span class="status-badge status-ok">NORMAL</span>
          </div>

          <div class="diag-item">
            <div class="diag-info">
              <span class="diag-name">Temperature Sensors</span>
              <span class="diag-desc">NTC Probe Array (4x)</span>
            </div>
            <span class="status-badge status-ok">4/4 OK</span>
          </div>

          <div class="diag-item">
            <div class="diag-info">
              <span class="diag-name">Current Shunt ADC</span>
              <span class="diag-desc">High Precision Current Sense</span>
            </div>
            <span class="status-badge status-ok">CALIBRATED</span>
          </div>
        </div>

        <button class="btn-run">Run Hardware Self-Test</button>
      </div>

      <!-- Card 2: Communication & Bus Health -->
      <div class="card">
        <div class="card-title">
          <span>Communication & Interfaces</span>
          <span style="font-size: 11px; color: #64748b;">Bus Status</span>
        </div>

        <div class="diag-list">
          <div class="diag-item">
            <div class="diag-info">
              <span class="diag-name">ESP32 Wi-Fi Module</span>
              <span class="diag-desc">Local Network Bridge</span>
            </div>
            <span class="status-badge status-ok">CONNECTED</span>
          </div>

          <div class="diag-item">
            <div class="diag-info">
              <span class="diag-name">CAN Bus Interface</span>
              <span class="diag-desc">Vehicle / Inverter Comms</span>
            </div>
            <span class="status-badge status-warn">IDLE</span>
          </div>

          <div class="diag-item">
            <div class="diag-info">
              <span class="diag-name">UART Serial Bus</span>
              <span class="diag-desc">BMS Core Microcontroller</span>
            </div>
            <span class="status-badge status-ok">OK (115200 baud)</span>
          </div>

          <div class="diag-item">
            <div class="diag-info">
              <span class="diag-name">EEPROM Storage</span>
              <span class="diag-desc">Configuration & Logs Memory</span>
            </div>
            <span class="status-badge status-ok">HEALTHY</span>
          </div>
        </div>

        <button class="btn-run">Ping Bus Interfaces</button>
      </div>

    </div>

  </div>

  <!-- Status Sync Script -->
  <script src="diagnostics.js"></script>
</body>
</html>
)WEB_d862696357";

static const char kAsset_diagnostics_js[] = R"WEB_012e23d14f(
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

document.getElementById('stm').innerHTML = "STM32 OTA System";
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
)WEB_012e23d14f";

static const char kAsset_firmware_css[] = R"WEB_97cac29636(
* { margin: 0; padding: 0; box-sizing: border-box; font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; }
    body { background-color: #0b1120; color: #f8fafc; min-height: 100vh; display: flex; }
    
    /* Sidebar Styling */
    .sidebar { width: 250px; background: #0f172a; border-right: 1px solid rgba(255, 255, 255, 0.05); display: flex; flex-direction: column; padding: 20px; }
    .sidebar-brand { font-size: 18px; font-weight: bold; color: #fff; margin-bottom: 30px; padding: 10px; background: rgba(255, 255, 255, 0.05); border-radius: 8px; text-align: center; }
    .sidebar-menu { display: flex; flex-direction: column; gap: 8px; }
    .sidebar-menu a { color: #94a3b8; text-decoration: none; padding: 10px 15px; border-radius: 6px; font-size: 14px; transition: 0.2s; }
    .sidebar-menu a:hover, .sidebar-menu a.active { background: #0284c7; color: #fff; }

    /* Main Content Area */
    .main-content { flex: 1; padding: 30px; overflow-y: auto; }
    .top-bar { display: flex; justify-content: space-between; align-items: center; margin-bottom: 25px; }
    .page-title h2 { color: #fff; font-size: 24px; }
    
    .tag { padding: 6px 14px; border-radius: 20px; font-size: 13px; font-weight: 600; background: #1e293b; color: #f87171; }
    
    .firmware-container { display: flex; flex-direction: column; gap: 20px; max-width: 650px; }
    .card { background: rgba(30, 41, 59, 0.7); border: 1px solid rgba(255, 255, 255, 0.1); border-radius: 12px; padding: 20px; }
    
    .info-row { display: flex; justify-content: space-between; margin-bottom: 12px; font-size: 15px; color: #cbd5e1; }
    .value { font-weight: 600; color: #fff; }
    
    .btn { background: #0284c7; color: white; border: none; padding: 10px 18px; border-radius: 6px; cursor: pointer; font-weight: 500; transition: 0.2s; margin-top: 10px; }
    .btn:hover { background: #0369a1; }
    .btn:disabled { opacity: 0.5; cursor: not-allowed; }
    
    .upload-box { border: 2px dashed rgba(255, 255, 255, 0.2); padding: 25px; text-align: center; border-radius: 8px; margin: 15px 0; background: rgba(15, 23, 42, 0.4); }
    .upload-label { cursor: pointer; color: #38bdf8; font-weight: 600; display: inline-block; }
    .upload-label:hover { text-decoration: underline; }
    
    .progress-bar-bg { width: 100%; background: #0f172a; height: 10px; border-radius: 5px; overflow: hidden; margin-top: 8px; border: 1px solid rgba(255, 255, 255, 0.05); }
    .progress-bar-fill { width: 0%; height: 100%; background: #4ade80; transition: width 0.3s ease; }
)WEB_97cac29636";

static const char kAsset_firmware_html[] = R"WEB_b49e545353(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Firmware Update - BMS Control Centre</title>
  <link rel="stylesheet" href="firmware.css">
</head>
<body>

  <!-- Left Sidebar -->
  <div class="sidebar">
    <div class="sidebar-brand">BMS Control Centre</div>
    <div class="sidebar-menu">
      <a href="index.html">Dashboard</a>
      <a href="telemetry.html">Telemetry</a>
      <a href="charts.html">Charts</a>
      <a href="firmware.html" class="active">Firmware Update</a>
      <a href="config.html">Configuration</a>
      <a href="logs.html">Logs</a>
      <a href="diagnostics.html">Diagnostics</a>
      <a href="settings.html">Settings</a>
      <a href="about.html">About</a>
    </div>
  </div>

  <!-- Main Content -->
  <div class="main-content">
    <div class="top-bar">
      <div class="page-title"><h2>Firmware Update</h2></div>
      <span id="status-pill" class="tag">🔴 Offline</span>
    </div>

    <div class="firmware-container">
      
      <!-- Card 1: System Info -->
      <div class="card">
        <div class="info-row"><span>Current Version:</span><span class="value">v2.5.3</span></div>
        <div class="info-row"><span>Last Checked:</span><span class="value">2 Hours ago</span></div>
        <div class="info-row"><span>Status:</span><span class="value" style="color: #4ade80;">Up to date</span></div>
        <button class="btn" onclick="checkForUpdates(this)">Check for Updates</button>
      </div>

      <!-- Card 2: Manual Flashing -->
      <div class="card">
        <h3 style="color: #fff; margin-bottom: 10px;">Manual Firmware Flashing</h3>
        <p style="font-size: 13px; color: #94a3b8;" id="stm">Select a <code>.bin</code> file to flash new firmware to ESP32.</p>
        
        <div class="upload-box">
          <input type="file" id="firmwareFile" accept=".bin" style="display: none;" onchange="handleFileSelect(event)">
          <label for="firmwareFile" class="upload-label">
            📁 Click to Choose <code>.bin</code> File
          </label>
          <p id="fileName" style="font-size: 13px; color: #94a3b8; margin-top: 8px;">No file selected</p>
        </div>

        <button id="uploadBtn" class="btn" onclick="startFlashing()" disabled>Flash Firmware</button>

        <!-- Live Progress Area -->
        <div id="progressContainer" style="display: none; margin-top: 20px;">
          <div style="display: flex; justify-content: space-between; font-size: 14px; color: #cbd5e1;">
            <span id="flashStatusText">Validating file...</span>
            <span id="flashPercent">0%</span>
          </div>
          <div class="progress-bar-bg">
            <div id="progressBar" class="progress-bar-fill"></div>
          </div>
        </div>
      </div>

    </div>
  </div>
  <script src="telemetry-api.js"></script>
  <script src="firmware.js"></script>
</body>
</html>
)WEB_b49e545353";

static const char kAsset_firmware_js[] = R"WEB_0738cfbe75(
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
)WEB_0738cfbe75";

static const char kAsset_index_html[] = R"WEB_14fe455902(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>BMS Control Centre</title>
  <link rel="stylesheet" href="style.css">
</head>
<body>

  <!-- Left Sidebar -->
  <div class="sidebar">
    <div class="brand-box">
      <h3>BMS Control Centre</h3>
      <p id="OTA">ESP32 Control Hub</p>
    </div>

    <a href="index.html" class="nav-btn active">Dashboard</a>
    <a href="telemetry.html" class="nav-btn">Telemetry</a>
    <a href="charts.html" class="nav-btn">Charts</a>
    <a href="firmware.html" class="nav-btn">Firmware Update</a>
    <a href="config.html" class="nav-btn">Configuration</a>
    <a href="logs.html" class="nav-btn">Logs</a>
    <a href="diagnostics.html" class="nav-btn">Diagnostics</a>
    <a href="settings.html" class="nav-btn">Settings</a>
    <a href="about.html" class="nav-btn">About</a>

    <!-- System Status Card -->
    <div class="system-status-box">
      <div class="card-title" style="margin-bottom: 4px;">System Status</div>
      <div class="status-row">
        <span>ESP32</span>
        <span class="pill pill-green">Online</span>
      </div>
      <div class="status-row">
        <span>STM32 BMS</span>
        <span class="pill pill-green">Online</span>
      </div>
      <div class="status-row" style="color: #64748b; margin-top: 2px;">
        <span>Last Update</span>
        <span id="last-update-time">12:30:45 PM</span>
      </div>
    </div>
  </div>

  <!-- Main Workspace -->
  <div class="main-workspace">

    <!-- Top Bar -->
    <div class="top-bar">
      <div class="page-title">
        <span>DASHBOARD</span>
        <h2>Live Battery Monitoring</h2>
      </div>

      <div class="top-tags">
        <span id="connection-status" class="top-tag">🟢 Connected</span>
        <span class="top-tag">📶 Wi-Fi: Home_Network</span>
        <span id="top-clock" class="top-tag">🕒 10:08:00 pm</span>
        
        <!-- Interactive Simulated Mode Toggle Button -->
        <span id="sim-mode-btn" class="top-tag toggle-btn">
          Simulated Mode: On
        </span>
      </div>
    </div>

    <!-- 6 Key Cards Row -->
    <div class="metrics-row">
      <div class="card">
        <div class="card-title">Battery Voltage</div>
        <div id="val-voltage" class="card-value">12.74 V</div>
        <div class="card-subtext">Range: 10.0 - 15.0 V</div>
      </div>

      <div class="card">
        <div class="card-title">Battery Current</div>
        <div id="val-current" class="card-value">2.02 A</div>
        <div id="val-current-status" class="card-subtext">Discharging</div>
      </div>

      <div class="card">
        <div class="card-title">Temperature</div>
        <div id="val-temp" class="card-value">28.8 °C</div>
        <div class="card-subtext">Range: 0 - 60 °C</div>
      </div>

      <div class="card">
        <div class="card-title">State of Charge</div>
        <div id="val-soc" class="card-value">87%</div>
        <div class="progress-bg"><div id="bar-soc" class="progress-fill" style="width: 87%;"></div></div>
      </div>

      <div class="card">
        <div class="card-title">Health</div>
        <div id="val-health" class="card-value">95%</div>
        <div class="progress-bg"><div id="bar-health" class="progress-fill" style="width: 95%;"></div></div>
      </div>

      <div class="card">
        <div class="card-title">Cycle Count</div>
        <div class="card-value">128</div>
        <div class="card-subtext">Optimal condition</div>
      </div>
    </div>

    <!-- Main Grid Sections -->
    <div class="dashboard-grid">

      <!-- Column 1: Live Telemetry & SOC Charts -->
      <div class="column">
        <!-- Live Telemetry Graph -->
        <div class="card" style="flex: 1.2;">
          <div class="card-title">
            <span>Live Telemetry</span>
            <span class="top-tag" style="padding: 2px 6px; font-size: 9px;">1 Minute</span>
          </div>
          <div class="chart-box">
            <svg viewBox="0 0 300 100" preserveAspectRatio="none">
              <path id="graph-path-1" d="" fill="none" stroke="#38bdf8" stroke-width="2"/>
              <path id="graph-path-2" d="" fill="none" stroke="#f43f5e" stroke-width="2"/>
            </svg>
          </div>
        </div>

        <!-- SOC Over Time Graph -->
        <div class="card" style="flex: 1;">
          <div class="card-title">SOC Over Time</div>
          <div class="chart-box">
            <svg viewBox="0 0 300 60" preserveAspectRatio="none">
              <path id="graph-soc-path" d="" fill="none" stroke="#f97316" stroke-width="2"/>
            </svg>
          </div>
        </div>
      </div>

      <!-- Column 2: Fault Status & Cell Voltages -->
      <div class="column">
        <!-- Fault Status -->
        <div class="card" style="flex: 1.3;">
          <div class="card-title" style="margin-bottom: 6px;">
            <span>Fault Status</span>
            <span class="ok-badge">No Faults</span>
          </div>
          <div style="display: flex; flex-direction: column; gap: 4px;">
            <div class="item-row"><span>Over Voltage</span><span class="ok-badge">OK</span></div>
            <div class="item-row"><span>Under Voltage</span><span class="ok-badge">OK</span></div>
            <div class="item-row"><span>Over Current</span><span class="ok-badge">OK</span></div>
            <div class="item-row"><span>Over Temperature</span><span class="ok-badge">OK</span></div>
            <div class="item-row"><span>Short Circuit</span><span class="ok-badge">OK</span></div>
            <div class="item-row"><span>Cell Imbalance</span><span class="ok-badge">OK</span></div>
          </div>
        </div>

        <!-- Cell Voltages -->
        <div class="card" style="flex: 1;">
          <div class="card-title" style="margin-bottom: 6px;">Cell Voltages</div>
          <div style="display: flex; flex-direction: column; gap: 4px;">
            <div class="item-row"><span>Cell 1</span><span id="cell-1" style="color: #38bdf8; font-weight: 600;">3.112 V</span></div>
            <div class="item-row"><span>Cell 2</span><span id="cell-2" style="color: #38bdf8; font-weight: 600;">3.120 V</span></div>
            <div class="item-row"><span>Cell 3</span><span id="cell-3" style="color: #38bdf8; font-weight: 600;">3.101 V</span></div>
            <div class="item-row"><span>Cell 4</span><span id="cell-4" style="color: #38bdf8; font-weight: 600;">3.113 V</span></div>
          </div>
        </div>
      </div>

      <!-- Column 3: Device Info & Recent Events -->
      <div class="column">
        <!-- Device Info -->
        <div class="card" style="flex: 1.1;">
          <div class="card-title" style="margin-bottom: 6px;">Device Information</div>
          <div style="display: flex; flex-direction: column; gap: 3px; font-size: 11px;">
            <div class="item-row" style="background: none; border: none; padding: 1px 0;">
              <span style="color: #64748b;">Device Name</span><span style="font-weight: 600;">STM32 BMS</span>
            </div>
            <div class="item-row" style="background: none; border: none; padding: 1px 0;">
              <span style="color: #64748b;">Firmware Version</span><span>v2.4.1</span>
            </div>
            <div class="item-row" style="background: none; border: none; padding: 1px 0;">
              <span style="color: #64748b;">Bootloader Version</span><span>v2.1.0</span>
            </div>
            <div class="item-row" style="background: none; border: none; padding: 1px 0;">
              <span style="color: #64748b;">Hardware Version</span><span>REV B</span>
            </div>
            <div class="item-row" style="background: none; border: none; padding: 1px 0;">
              <span style="color: #64748b;">Device ID</span><span>BMS-STM32-001</span>
            </div>
            <div class="item-row" style="background: none; border: none; padding: 1px 0;">
              <span style="color: #64748b;">Uptime</span><span id="val-uptime">2d 14h 23m 15s</span>
            </div>
            <div class="item-row" style="background: none; border: none; padding: 1px 0;">
              <span style="color: #64748b;">Last Reset</span><span>Power On</span>
            </div>
          </div>
        </div>

        <!-- Recent Events -->
        <div class="card" style="flex: 1.2;">
          <div class="card-title" style="margin-bottom: 6px;">Recent Events</div>
          <div style="display: flex; flex-direction: column; gap: 4px;">
            <div class="item-row">
              <span>System started</span>
              <span style="font-size: 9px; color: #64748b;">12:30:10 PM</span>
            </div>
            <div class="item-row">
              <span>Telemetry connected</span>
              <span style="font-size: 9px; color: #64748b;">12:30:09 PM</span>
            </div>
            <div class="item-row">
              <span>Config updated</span>
              <span style="font-size: 9px; color: #64748b;">12:29:02 PM</span>
            </div>
            <div class="item-row">
              <span>Firmware updated</span>
              <span style="font-size: 9px; color: #64748b;">10:15:33 AM</span>
            </div>
            <div class="item-row">
              <span>Temp warning</span>
              <span style="font-size: 9px; color: #64748b;">09:12:11 AM</span>
            </div>
          </div>
        </div>
      </div>
    </div>
  </div>
  <script src="telemetry-api.js"></script>
  <script src="main.js"></script> 
</body>
</html>
)WEB_14fe455902";

static const char kAsset_logs_css[] = R"WEB_495b0431cc(
* {
  box-sizing: border-box;
  margin: 0;
  padding: 0;
  font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, sans-serif;
}

html,
body {
  height: 100vh;
  width: 100vw;
  overflow: hidden; /* Scrollbar off */
  background-color: #0b111e;
  color: #ffffff;
}

body {
  display: flex;
}

/* Sidebar Area */
.sidebar {
  width: 240px;
  height: 100vh;
  background-color: #0f172a;
  padding: 16px;
  display: flex;
  flex-direction: column;
  gap: 6px;
  border-right: 1px solid #1e293b;
}

.brand-box {
  background-color: #ffffff;
  color: #0f172a;
  padding: 12px;
  border-radius: 12px;
  margin-bottom: 12px;
}

.brand-box h3 {
  font-size: 14px;
  font-weight: 700;
}

.brand-box p {
  font-size: 11px;
  color: #64748b;
}

.nav-btn {
  display: flex;
  align-items: center;
  gap: 12px;
  padding: 10px 14px;
  border-radius: 10px;
  color: #94a3b8;
  text-decoration: none;
  font-size: 13px;
  font-weight: 500;
  transition: 0.2s;
}

.nav-btn.active {
  background-color: #0e3a40;
  color: #38bdf8;
}

.nav-btn:hover:not(.active) {
  background-color: #1e293b;
  color: #ffffff;
}

/* Main Workspace */
.main-content {
  flex: 1;
  height: 100vh;
  padding: 20px 28px;
  display: flex;
  flex-direction: column;
  gap: 16px;
}

.top-bar {
  display: flex;
  justify-content: space-between;
  align-items: center;
}

.page-title span {
  font-size: 11px;
  color: #38bdf8;
  text-transform: uppercase;
  letter-spacing: 1px;
  font-weight: 700;
}

.page-title h2 {
  font-size: 22px;
  margin-top: 2px;
}

.tag {
  background-color: #1e293b;
  padding: 6px 14px;
  border-radius: 20px;
  font-size: 12px;
  color: #cbd5e1;
}

/* Logs Card & Table Area */
.card {
  background-color: #111a2e;
  border: 1px solid #1e293b;
  border-radius: 16px;
  padding: 20px;
  flex: 1;
  display: flex;
  flex-direction: column;
  gap: 12px;
}

.card-header {
  display: flex;
  justify-content: space-between;
  align-items: center;
}

.card-title {
  font-size: 14px;
  font-weight: 600;
  color: #38bdf8;
}

.btn-clear {
  background-color: #1e293b;
  color: #cbd5e1;
  border: none;
  padding: 6px 12px;
  border-radius: 6px;
  font-size: 12px;
  cursor: pointer;
}

.btn-clear:hover {
  background-color: #334155;
  color: #ffffff;
}

.table-container {
  flex: 1;
  overflow-y: auto;
}

table {
  width: 100%;
  border-collapse: collapse;
  text-align: left;
  font-size: 13px;
}

th {
  background-color: #0b111e;
  color: #64748b;
  padding: 10px 14px;
  font-weight: 600;
  border-bottom: 1px solid #1e293b;
}

td {
  padding: 12px 14px;
  border-bottom: 1px solid #1e293b;
  color: #cbd5e1;
}

.badge {
  padding: 3px 8px;
  border-radius: 6px;
  font-size: 11px;
  font-weight: 600;
}

.badge-info {
  background: rgba(56, 189, 248, 0.15);
  color: #38bdf8;
}

.badge-warn {
  background: rgba(245, 158, 11, 0.15);
  color: #f59e0b;
}

.badge-error {
  background: rgba(239, 68, 68, 0.15);
  color: #ef4444;
}
)WEB_495b0431cc";

static const char kAsset_logs_html[] = R"WEB_8ea4151ea5(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Logs - BMS Control Centre</title>
  <link rel="stylesheet" href="logs.css">
</head>
<body>

  <!-- Left Sidebar -->
  <div class="sidebar">
    <div class="brand-box">
      <h3>BMS Control Centre</h3>
      <p id="stm">ESP32 Control Hub</p>
    </div>

    <a href="index.html" class="nav-btn">Dashboard</a>
    <a href="telemetry.html" class="nav-btn">Telemetry</a>
    <a href="charts.html" class="nav-btn">Charts</a>
    <a href="firmware.html" class="nav-btn">Firmware Update</a>
    <a href="config.html" class="nav-btn">Configuration</a>
    <a href="logs.html" class="nav-btn active">Logs</a>
    <a href="diagnostics.html" class="nav-btn">Diagnostics</a>
    <a href="settings.html" class="nav-btn">Settings</a>
    <a href="about.css" class="nav-btn">About</a>
  </div>

  <!-- Main Content Area -->
  <div class="main-content">
    
    <!-- Top Header Bar -->
    <div class="top-bar">
      <div class="page-title">
        <span>EVENT MONITORING</span>
        <h2>System Logs</h2>
      </div>

      <div class="top-tags">
        <span id="status-pill" class="tag">🔴 Offline</span>
        <span class="tag">System Events</span>
      </div>
    </div>

    <!-- Logs Table Container -->
    <div class="card">
      <div class="card-header">
        <div class="card-title">Recent Event History</div>
        <button class="btn-clear">Clear Logs</button>
      </div>

      <div class="table-container">
        <table>
          <thead>
            <tr>
              <th>Time</th>
              <th>Type</th>
              <th>Message</th>
              <th>Source</th>
            </tr>
          </thead>
          <tbody>
            <tr>
              <td>10:14:32 AM</td>
              <td><span class="badge badge-info">INFO</span></td>
              <td>Telemetry data stream connected successfully</td>
              <td>ESP32_WiFi</td>
            </tr>
            <tr>
              <td>10:10:05 AM</td>
              <td><span class="badge badge-warn">WARN</span></td>
              <td>Cell 3 Voltage slightly below average (3.122V)</td>
              <td>BMS_Core</td>
            </tr>
            <tr>
              <td>09:45:18 AM</td>
              <td><span class="badge badge-info">INFO</span></td>
              <td>System boot sequence complete</td>
              <td>System</td>
            </tr>
            <tr>
              <td>08:30:00 AM</td>
              <td><span class="badge badge-error">ERROR</span></td>
              <td>Over-Current protection triggered briefly</td>
              <td>Safety_Module</td>
            </tr>
            <tr>
              <td>08:28:12 AM</td>
              <td><span class="badge badge-info">INFO</span></td>
              <td>Battery charging cycle started</td>
              <td>Power_Manager</td>
            </tr>
          </tbody>
        </table>
      </div>
    </div>

  </div>

  <!-- Status Sync Script -->
 <script src="logs.js"></script>
</body>
</html>
)WEB_8ea4151ea5";

static const char kAsset_logs_js[] = R"WEB_8cfaac20a8(
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

document.getElementById('stm').innerHTML = "STM32 OTA System";
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
)WEB_8cfaac20a8";

static const char kAsset_main_js[] = R"WEB_e46ea72cce(
let simInterval = null;
let liveInterval = null;
let animOffset = 0;
let secondsCounter = 0;
let telemetryHistory = {
  voltage: [],
  current: [],
  temperature: [],
  soc: []
};

function drawGraphs() {
  animOffset += 0.15;

  let d1 = 'M 0 ' + (50 + Math.sin(animOffset) * 20);
  for (let x = 15; x <= 300; x += 15) {
    const y = 50 + Math.sin(animOffset + x * 0.05) * 25 + Math.sin(x * 0.1) * 5;
    d1 += ' L ' + x + ' ' + y;
  }
  const g1 = document.getElementById('graph-path-1');
  if (g1) g1.setAttribute('d', d1);

  let d2 = 'M 0 ' + (60 + Math.cos(animOffset) * 15);
  for (let x = 15; x <= 300; x += 15) {
    const y = 55 + Math.cos(animOffset + x * 0.04) * 20 + Math.cos(x * 0.08) * 8;
    d2 += ' L ' + x + ' ' + y;
  }
  const g2 = document.getElementById('graph-path-2');
  if (g2) g2.setAttribute('d', d2);

  let dSOC = 'M 0 ' + (30 + Math.sin(animOffset * 0.5) * 3);
  for (let x = 30; x <= 300; x += 30) {
    const y = 30 + Math.sin(animOffset * 0.5 + x * 0.02) * 4;
    dSOC += ' L ' + x + ' ' + y;
  }
  const gsoc = document.getElementById('graph-soc-path');
  if (gsoc) gsoc.setAttribute('d', dSOC);
}

function pushHistory(values) {
  if (typeof values.voltage === 'number') telemetryHistory.voltage.push(values.voltage);
  if (typeof values.current === 'number') telemetryHistory.current.push(values.current);
  if (typeof values.temperature === 'number') telemetryHistory.temperature.push(values.temperature);
  if (typeof values.soc === 'number') telemetryHistory.soc.push(values.soc);

  Object.keys(telemetryHistory).forEach((key) => {
    if (telemetryHistory[key].length > 24) {
      telemetryHistory[key].shift();
    }
  });
}

function updateConnectionStatus(connected) {
  const statusTag = document.getElementById('connection-status') ||
    document.querySelector('.top-tags .top-tag');
  const stmStatus = document.querySelector('.system-status-box .status-row:nth-child(2) .pill');
  if (statusTag) {
    statusTag.textContent = connected ? '🟢 Connected' : '🔴 Offline';
  }
  if (stmStatus) {
    stmStatus.textContent = connected ? 'Online' : 'Offline';
    stmStatus.classList.toggle('pill-green', connected);
    stmStatus.classList.toggle('pill-red', !connected);
  }
}

function applyTelemetryValues(values) {
  const now = new Date();
  const topClock = document.getElementById('top-clock');
  if (topClock) topClock.innerText = '🕒 ' + now.toLocaleTimeString();
  const lastUpdate = document.getElementById('last-update-time');
  if (lastUpdate) lastUpdate.innerText = now.toLocaleTimeString();

  const vEl = document.getElementById('val-voltage');
  const iEl = document.getElementById('val-current');
  const tEl = document.getElementById('val-temp');
  const socEl = document.getElementById('val-soc');
  const barSoc = document.getElementById('bar-soc');
  const currentStatus = document.getElementById('val-current-status');

  if (vEl && typeof values.voltage === 'number') {
    vEl.innerText = values.voltage.toFixed(2) + ' V';
  }
  if (iEl && typeof values.current === 'number') {
    iEl.innerText = values.current.toFixed(2) + ' A';
  }
  if (tEl && typeof values.temperature === 'number') {
    tEl.innerText = values.temperature.toFixed(1) + ' °C';
  }
  if (socEl && typeof values.soc === 'number') {
    socEl.innerText = Math.round(values.soc) + '%';
  }
  if (barSoc && typeof values.soc === 'number') {
    barSoc.style.width = Math.max(0, Math.min(100, values.soc)) + '%';
  }
  if (currentStatus && typeof values.current === 'number') {
    currentStatus.innerText = values.current >= 0 ? 'Discharging' : 'Charging';
  }

  if (typeof values.voltage === 'number') {
    const cellVoltage = values.voltage / 4;
    ['cell-1', 'cell-2', 'cell-3', 'cell-4'].forEach((id) => {
      const cell = document.getElementById(id);
      if (cell) cell.innerText = cellVoltage.toFixed(3) + ' V';
    });
  }

  pushHistory(values);
  drawGraphs();
}

function updateSimulatedMetrics() {
  const time = Date.now() / 1500;
  applyTelemetryValues({
    voltage: 12.4 + Math.sin(time) * 0.25,
    current: 2.1 + Math.cos(time * 1.2) * 0.18,
    temperature: 30 + Math.sin(time / 1.7) * 1.6,
    soc: 86 + Math.sin(time / 4) * 2.8
  });
  updateConnectionStatus(true);

  secondsCounter++;
  const upEl = document.getElementById('val-uptime');
  if (upEl) upEl.innerText = `2d 14h 23m ${(15 + secondsCounter) % 60}s`;
}

async function refreshLiveTelemetry() {
  try {
    const payload = await fetchLiveTelemetry();
    const values = telemetryToDisplayValues(payload);
    if (!values.valid) {
      updateConnectionStatus(false);
      const vEl = document.getElementById('val-voltage');
      const iEl = document.getElementById('val-current');
      const tEl = document.getElementById('val-temp');
      const socEl = document.getElementById('val-soc');
      if (vEl) vEl.innerText = '-- V';
      if (iEl) iEl.innerText = '-- A';
      if (tEl) tEl.innerText = '-- °C';
      if (socEl) socEl.innerText = '--%';
      return;
    }
    applyTelemetryValues(values);
    updateConnectionStatus(true);
  } catch (error) {
    updateConnectionStatus(false);
  }
}

function startSimulation() {
  if (!simInterval) {
    simInterval = setInterval(updateSimulatedMetrics, 1000);
  }
}

function stopSimulation() {
  if (simInterval) {
    clearInterval(simInterval);
    simInterval = null;
  }
}

function startLivePolling() {
  if (!liveInterval) {
    refreshLiveTelemetry();
    liveInterval = setInterval(refreshLiveTelemetry, 2000);
  }
}

function stopLivePolling() {
  if (liveInterval) {
    clearInterval(liveInterval);
    liveInterval = null;
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
    stopLivePolling();
    startSimulation();
  } else {
    btn.innerText = 'Simulated Mode: Off';
    btn.style.backgroundColor = '#1e293b';
    btn.style.color = '#94a3b8';
    btn.style.borderColor = '#334155';
    stopSimulation();
    startLivePolling();
  }
}

function toggleSimulatedMode() {
  setSimulatedMode(!isSimulatedMode());
  updateSimModeUI(isSimulatedMode());
}

window.addEventListener('DOMContentLoaded', () => {
  ensureSimulatedModeDefault();
  drawGraphs();
  updateSimModeUI(isSimulatedMode());

  const simBtn = document.getElementById('sim-mode-btn');
  if (simBtn) {
    simBtn.addEventListener('click', toggleSimulatedMode);
  }

  const otaLabel = document.getElementById('OTA');
  if (otaLabel) {
    otaLabel.innerHTML = 'ESP32 Control Hub';
  }
});

window.addEventListener('storage', (event) => {
  if (event.key === 'simulatedMode') {
    updateSimModeUI(isSimulatedMode());
  }
});

)WEB_e46ea72cce";

static const char kAsset_setting_css[] = R"WEB_c6298f0807(
* {
      box-sizing: border-box;
      margin: 0;
      padding: 0;
      font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, sans-serif;
    }

    html, body {
      height: 100vh;
      width: 100vw;
      overflow: hidden; /* Scrollbar off */
      background-color: #0b111e;
      color: #ffffff;
    }

    body {
      display: flex;
    }

    /* Sidebar Area */
    .sidebar {
      width: 240px;
      height: 100vh;
      background-color: #0f172a;
      padding: 16px;
      display: flex;
      flex-direction: column;
      gap: 6px;
      border-right: 1px solid #1e293b;
    }

    .brand-box {
      background-color: #ffffff;
      color: #0f172a;
      padding: 12px;
      border-radius: 12px;
      margin-bottom: 12px;
    }

    .brand-box h3 { font-size: 14px; font-weight: 700; }
    .brand-box p { font-size: 11px; color: #64748b; }

    .nav-btn {
      display: flex;
      align-items: center;
      gap: 12px;
      padding: 10px 14px;
      border-radius: 10px;
      color: #94a3b8;
      text-decoration: none;
      font-size: 13px;
      font-weight: 500;
      transition: 0.2s;
    }

    .nav-btn.active {
      background-color: #0e3a40;
      color: #38bdf8;
    }

    .nav-btn:hover:not(.active) {
      background-color: #1e293b;
      color: #ffffff;
    }

    /* Main Workspace */
    .main-content {
      flex: 1;
      height: 100vh;
      padding: 20px 28px;
      display: flex;
      flex-direction: column;
      gap: 16px;
    }

    .top-bar {
      display: flex;
      justify-content: space-between;
      align-items: center;
    }

    .page-title span {
      font-size: 11px;
      color: #38bdf8;
      text-transform: uppercase;
      letter-spacing: 1px;
      font-weight: 700;
    }

    .page-title h2 {
      font-size: 22px;
      margin-top: 2px;
    }

    .tag {
      background-color: #1e293b;
      padding: 6px 14px;
      border-radius: 20px;
      font-size: 12px;
      color: #cbd5e1;
    }

    /* Settings Grid */
    .settings-grid {
      display: grid;
      grid-template-columns: 1fr 1fr;
      gap: 16px;
      flex: 1;
    }

    .card {
      background-color: #111a2e;
      border: 1px solid #1e293b;
      border-radius: 16px;
      padding: 20px;
      display: flex;
      flex-direction: column;
      gap: 14px;
    }

    .card-title {
      font-size: 14px;
      font-weight: 600;
      color: #38bdf8;
      border-bottom: 1px solid #1e293b;
      padding-bottom: 8px;
    }

    .form-group {
      display: flex;
      flex-direction: column;
      gap: 6px;
    }

    .form-group label {
      font-size: 12px;
      color: #94a3b8;
    }

    .form-group input, .form-group select {
      background-color: #0b111e;
      border: 1px solid #1e293b;
      border-radius: 8px;
      padding: 10px;
      color: #ffffff;
      font-size: 13px;
      outline: none;
    }

    .form-group input:focus, .form-group select:focus {
      border-color: #38bdf8;
    }

    .btn-save {
      background-color: #38bdf8;
      color: #0b111e;
      border: none;
      padding: 12px;
      border-radius: 8px;
      font-weight: 600;
      cursor: pointer;
      margin-top: auto;
      transition: 0.2s;
    }

    .btn-save:hover {
      background-color: #7dd3fc;
    }

    .btn-danger {
      background-color: #ef4444;
      color: #ffffff;
      border: none;
      padding: 10px;
      border-radius: 8px;
      font-weight: 600;
      cursor: pointer;
      font-size: 13px;
      transition: 0.2s;
    }

    .btn-danger:hover {
      background-color: #f87171;
    }
)WEB_c6298f0807";

static const char kAsset_settings_html[] = R"WEB_a698c015da(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Settings - BMS Control Centre</title>
  <link rel="stylesheet" href="setting.css">
</head>
<body>

  <!-- Left Sidebar -->
  <div class="sidebar">
    <div class="brand-box">
      <h3>BMS Control Centre</h3>
      <p id="stm">ESP32 Control Hub</p>
    </div>

    <a href="index.html" class="nav-btn">Dashboard</a>
    <a href="telemetry.html" class="nav-btn">Telemetry</a>
    <a href="charts.html" class="nav-btn">Charts</a>
    <a href="firmware.html" class="nav-btn">Firmware Update</a>
    <a href="config.html" class="nav-btn">Configuration</a>
    <a href="logs.html" class="nav-btn">Logs</a>
    <a href="diagnostics.html" class="nav-btn">Diagnostics</a>
    <a href="settings.html" class="nav-btn active">Settings</a>
    <a href="about.html" class="nav-btn">About</a>
  </div>

  <!-- Main Content Area -->
  <div class="main-content">
    
    <!-- Top Header Bar -->
    <div class="top-bar">
      <div class="page-title">
        <span>PREFERENCES & NETWORK</span>
        <h2>System Settings</h2>
      </div>

      <div class="top-tags">
        <span id="status-pill" class="tag">🔴 Offline</span>
        <span class="tag">Network Setup</span>
      </div>
    </div>

    <!-- Settings Grid -->
    <div class="settings-grid">
      
      <!-- Card 1: Network & WiFi Settings -->
      <div class="card">
        <div class="card-title">Wi-Fi & Network Settings</div>
        
        <div class="form-group">
          <label>Wi-Fi SSID</label>
          <input type="text" value="BMS_Home_Network">
        </div>

        <div class="form-group">
          <label>Wi-Fi Password</label>
          <input type="password" value="••••••••••••">
        </div>

        <div class="form-group">
          <label>IP Assignment Mode</label>
          <select>
            <option value="dhcp">DHCP (Automatic)</option>
            <option value="static">Static IP</option>
          </select>
        </div>

        <button class="btn-save">Save Network Settings</button>
      </div>

      <!-- Card 2: System & Maintenance -->
      <div class="card">
        <div class="card-title">System & Management</div>

        <div class="form-group">
          <label>Telemetry Refresh Rate</label>
          <select>
            <option value="1">1 Second (Realtime)</option>
            <option value="2" selected>2 Seconds</option>
            <option value="5">5 Seconds</option>
          </select>
        </div>

        <div class="form-group">
          <label>Device Name</label>
          <input type="text" value="ESP32_BMS_Controller">
        </div>

        <div class="form-group" style="margin-top: 10px;">
          <label style="color: #f87171; font-weight: 600;">Danger Zone</label>
          <button class="btn-danger">Restart ESP32 Device</button>
        </div>

        <button class="btn-save">Save System Settings</button>
      </div>

    </div>

  </div>
<div class="card" style="margin-top: 25px; border: 1px solid rgba(239, 68, 68, 0.4); background: rgba(239, 68, 68, 0.03); border-radius: 12px; padding: 20px;">
  <div style="display: flex; justify-content: space-between; align-items: center; border-bottom: 1px solid rgba(239, 68, 68, 0.2); padding-bottom: 15px; margin-bottom: 20px;">
    <div>
      <h3 style="color: #f87171; font-size: 18px; display: flex; align-items: center; gap: 8px;">⚠️ Danger Zone</h3>
      <p style="font-size: 12px; color: #94a3b8; margin-top: 4px;">High-impact system actions. Unlocking required to make changes.</p>
    </div>
    <label style="display: flex; align-items: center; gap: 10px; cursor: pointer; background: #1e293b; padding: 8px 12px; border-radius: 20px; border: 1px solid #334155;">
      <span id="lockStatusText" style="font-size: 12px; color: #f87171; font-weight: 600;">🔒 Locked</span>
      <input type="checkbox" id="dangerToggle" onchange="toggleDangerZone(this.checked)" style="cursor: pointer; width: 16px; height: 16px;">
    </label>
  </div>

  <div style="display: flex; flex-direction: column; gap: 15px;">
    <div style="display: flex; justify-content: space-between; align-items: center; border-bottom: 1px solid rgba(255, 255, 255, 0.05); padding-bottom: 12px;">
      <div>
        <strong style="color: #fff; font-size: 14px;">Reboot Microcontroller</strong>
        <p style="font-size: 12px; color: #64748b;">Restarts the ESP32 controller. Telemetry will pause briefly.</p>
      </div>
      <button class="danger-btn" onclick="triggerAction('reboot')" disabled style="background: #334155; color: #94a3b8; border: 1px solid #475569; padding: 8px 14px; border-radius: 6px; cursor: not-allowed; opacity: 0.5;">Reboot ESP32</button>
    </div>

    <div style="display: flex; justify-content: space-between; align-items: center; border-bottom: 1px solid rgba(255, 255, 255, 0.05); padding-bottom: 12px;">
      <div>
        <strong style="color: #fff; font-size: 14px;">Clear System Logs</strong>
        <p style="font-size: 12px; color: #64748b;">Deletes all saved system diagnostics and error history.</p>
      </div>
      <button class="danger-btn" onclick="triggerAction('clearLogs')" disabled style="background: #334155; color: #94a3b8; border: 1px solid #475569; padding: 8px 14px; border-radius: 6px; cursor: not-allowed; opacity: 0.5;">Clear Logs</button>
    </div>

    <div style="display: flex; justify-content: space-between; align-items: center; border-bottom: 1px solid rgba(255, 255, 255, 0.05); padding-bottom: 12px;">
      <div>
        <strong style="color: #fff; font-size: 14px;">Reset to Factory Defaults</strong>
        <p style="font-size: 12px; color: #64748b;">Restores default voltage thresholds, balancing limits, and configs.</p>
      </div>
      <button class="danger-btn" onclick="openConfirmModal('factoryReset')" disabled style="background: #dc2626; color: white; border: none; padding: 8px 14px; border-radius: 6px; cursor: not-allowed; opacity: 0.5;">Factory Reset</button>
    </div>

    <div style="display: flex; justify-content: space-between; align-items: center;">
      <div>
        <strong style="color: #f87171; font-size: 14px;">Force Emergency Cutoff</strong>
        <p style="font-size: 12px; color: #64748b;">Immediately disconnects MOSFETs/Relays to stop power output.</p>
      </div>
      <button class="danger-btn" onclick="openConfirmModal('cutoff')" disabled style="background: #991b1b; color: white; border: none; padding: 8px 14px; border-radius: 6px; cursor: not-allowed; opacity: 0.5; font-weight: 600;">Emergency Stop</button>
    </div>
  </div>
</div>

<!-- Confirm Modal -->
<div id="dangerModal" style="display: none; position: fixed; top: 0; left: 0; width: 100%; height: 100%; background: rgba(0,0,0,0.8); z-index: 1000; justify-content: center; align-items: center;">
  <div style="background: #0f172a; border: 1px solid #ef4444; border-radius: 12px; padding: 25px; max-width: 420px; width: 90%; text-align: center;">
    <h3 style="color: #f87171; font-size: 18px; margin-bottom: 10px;">🚨 Are you absolutely sure?</h3>
    <p id="modalDesc" style="font-size: 13px; color: #cbd5e1; margin-bottom: 15px;"></p>
    <p style="font-size: 12px; color: #94a3b8; margin-bottom: 10px;">To confirm, type <strong id="requiredText" style="color: #f87171;">CONFIRM</strong> below:</p>
    <input type="text" id="confirmInput" oninput="validateConfirmInput()" placeholder="Type here..." style="width: 100%; padding: 10px; border-radius: 6px; border: 1px solid #334155; background: #1e293b; color: #fff; text-align: center; font-weight: bold; margin-bottom: 20px;">
    <div style="display: flex; gap: 10px; justify-content: center;">
      <button onclick="closeConfirmModal()" style="background: #334155; color: #fff; border: none; padding: 10px 18px; border-radius: 6px; cursor: pointer;">Cancel</button>
      <button id="finalConfirmBtn" onclick="executeDangerAction()" disabled style="background: #dc2626; color: white; border: none; padding: 10px 18px; border-radius: 6px; cursor: not-allowed; opacity: 0.5; font-weight: 600;">Proceed</button>
    </div>
  </div>
</div>
<script src="settings.js"></script>
</body>
</html>
)WEB_a698c015da";

static const char kAsset_settings_js[] = R"WEB_600a07472b(
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
)WEB_600a07472b";

static const char kAsset_style_css[] = R"WEB_b4ad21b4c1(
* {
  box-sizing: border-box;
  margin: 0;
  padding: 0;
  font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, sans-serif;
}

html,
body {
  height: 100vh;
  width: 100vw;
  overflow: hidden; /* Prevent scrolling */
  background-color: #070d19;
  color: #ffffff;
}

body {
  display: flex;
  padding: 10px;
  gap: 10px;
}

/* Left Sidebar */
.sidebar {
  width: 210px;
  height: calc(100vh - 20px);
  background-color: #0c172c;
  padding: 12px;
  border-radius: 14px;
  display: flex;
  flex-direction: column;
  gap: 6px;
  border: 1px solid #162646;
  flex-shrink: 0;
}

.brand-box {
  background-color: #ffffff;
  color: #0c172c;
  padding: 8px 12px;
  border-radius: 10px;
  margin-bottom: 6px;
}

.brand-box h3 {
  font-size: 13px;
  font-weight: 800;
}

.brand-box p {
  font-size: 10px;
  color: #64748b;
}

.nav-btn {
  display: flex;
  align-items: center;
  padding: 7px 12px;
  border-radius: 8px;
  color: #94a3b8;
  text-decoration: none;
  font-size: 12px;
  font-weight: 500;
  transition: 0.2s;
}

.nav-btn.active {
  background-color: #083c5a;
  color: #38bdf8;
  font-weight: 600;
}

.nav-btn:hover:not(.active) {
  background-color: #162646;
  color: #ffffff;
}

/* System Status Widget inside Sidebar bottom */
.system-status-box {
  margin-top: auto;
  background-color: #091222;
  border: 1px solid #162646;
  border-radius: 10px;
  padding: 10px;
  display: flex;
  flex-direction: column;
  gap: 6px;
}

.status-row {
  display: flex;
  justify-content: space-between;
  align-items: center;
  font-size: 11px;
}

.pill {
  padding: 2px 8px;
  border-radius: 12px;
  font-size: 10px;
  font-weight: 600;
}

.pill-green {
  background-color: rgba(56, 189, 248, 0.2);
  color: #38bdf8;
}

/* Main Workspace */
.main-workspace {
  flex: 1;
  height: calc(100vh - 20px);
  display: flex;
  flex-direction: column;
  gap: 8px;
}

/* Top Bar */
.top-bar {
  display: flex;
  justify-content: space-between;
  align-items: center;
  height: 38px;
}

.page-title h2 {
  font-size: 18px;
  color: #ffffff;
}

.page-title span {
  font-size: 10px;
  color: #38bdf8;
  text-transform: uppercase;
  font-weight: 700;
  letter-spacing: 0.5px;
}

.top-tags {
  display: flex;
  gap: 6px;
  align-items: center;
}

.top-tag {
  background-color: #0c172c;
  border: 1px solid #162646;
  padding: 4px 10px;
  border-radius: 14px;
  font-size: 11px;
  color: #cbd5e1;
}

/* Interactive Toggle Button */
.toggle-btn {
  cursor: pointer;
  user-select: none;
  transition: all 0.2s ease;
}

.toggle-btn:hover {
  opacity: 0.85;
  transform: scale(0.98);
}

/* Common Card Styling */
.card {
  background-color: #0c172c;
  border: 1px solid #162646;
  border-radius: 12px;
  padding: 10px 12px;
  display: flex;
  flex-direction: column;
  justify-content: space-between;
}

.card-title {
  font-size: 11px;
  color: #94a3b8;
  font-weight: 600;
  display: flex;
  justify-content: space-between;
  align-items: center;
}

.card-value {
  font-size: 20px;
  font-weight: 700;
  color: #ffffff;
  margin: 2px 0;
}

.card-subtext {
  font-size: 10px;
  color: #64748b;
}

.progress-bg {
  width: 100%;
  height: 4px;
  background-color: #162646;
  border-radius: 2px;
  margin-top: 4px;
  overflow: hidden;
}

.progress-fill {
  height: 100%;
  background-color: #38bdf8;
  transition: width 0.3s ease;
}

/* 6 Metrics Grid */
.metrics-row {
  display: grid;
  grid-template-columns: repeat(6, 1fr);
  gap: 8px;
  height: 80px;
}

/* Main Dashboard Grid (3 Columns) */
.dashboard-grid {
  flex: 1;
  display: grid;
  grid-template-columns: 2.1fr 1.2fr 1.2fr;
  gap: 8px;
  overflow: hidden;
}

.column {
  display: flex;
  flex-direction: column;
  gap: 8px;
  height: 100%;
}

/* List Item Row Styles */
.item-row {
  display: flex;
  justify-content: space-between;
  align-items: center;
  padding: 4px 8px;
  background-color: #081020;
  border-radius: 6px;
  font-size: 11px;
  border: 1px solid #13223f;
}

.ok-badge {
  background-color: rgba(56, 189, 248, 0.15);
  color: #38bdf8;
  padding: 2px 6px;
  border-radius: 4px;
  font-size: 10px;
  font-weight: 700;
}

/* SVG Chart Containers */
.chart-box {
  flex: 1;
  width: 100%;
  display: flex;
  align-items: center;
  justify-content: center;
}

svg {
  width: 100%;
  height: 100%;
}
)WEB_b4ad21b4c1";

static const char kAsset_telemetry_api_js[] = R"WEB_55e989e099(
async function fetchLiveTelemetry() {
  const response = await fetch('/api/telemetry', { cache: 'no-store' });
  if (!response.ok) {
    throw new Error('Telemetry unavailable');
  }
  return response.json();
}

function isSimulatedMode() {
  return localStorage.getItem('simulatedMode') === 'true';
}

function setSimulatedMode(enabled) {
  localStorage.setItem('simulatedMode', enabled ? 'true' : 'false');
}

function ensureSimulatedModeDefault() {
  if (localStorage.getItem('simulatedMode') === null) {
    localStorage.setItem('simulatedMode', 'false');
  }
}

function telemetryToDisplayValues(payload) {
  return {
    voltage: typeof payload.voltage_mv === 'number' ? payload.voltage_mv / 1000 : null,
    current: typeof payload.current_ma === 'number' ? payload.current_ma / 1000 : null,
    temperature: typeof payload.temperature_centi_c === 'number'
      ? payload.temperature_centi_c / 100
      : null,
    soc: typeof payload.soc_tenths_percent === 'number'
      ? payload.soc_tenths_percent / 10
      : null,
    faultFlags: typeof payload.fault_flags === 'number' ? payload.fault_flags : 0,
    valid: payload.valid === true
  };
}

)WEB_55e989e099";

static const char kAsset_telemetry_css[] = R"WEB_48b33b0fed(
* {
      box-sizing: border-box;
      margin: 0;
      padding: 0;
      font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, sans-serif;
    }

    html, body {
      height: 100vh;
      width: 100vw;
      overflow: hidden; 
      background-color: #0d131f;
      color: #ffffff;
    }

    body {
      display: flex;
    }
    .sidebar {
      width: 230px;
      height: 100vh;
      background-color: #121927;
      padding: 15px 12px;
      display: flex;
      flex-direction: column;
      gap: 6px;
      border-right: 1px solid #1e293b;
    }

    .brand-box {
      background-color: #ffffff;
      color: #0d131f;
      padding: 10px;
      border-radius: 10px;
      margin-bottom: 10px;
    }

    .brand-box h3 {
      font-size: 13px;
      font-weight: 700;
    }

    .brand-box p {
      font-size: 10px;
      color: #64748b;
    }

    .nav-btn {
      display: flex;
      align-items: center;
      gap: 10px;
      padding: 8px 12px;
      border-radius: 8px;
      color: #94a3b8;
      text-decoration: none;
      font-size: 13px;
      font-weight: 500;
    }

    .nav-btn.active {
      background-color: #154c52;
      color: #38bdf8;
    }

    .nav-btn:hover:not(.active) {
      background-color: #1e293b;
      color: #ffffff;
    }

    .main-content {
      flex: 1;
      height: 100vh;
      padding: 18px 24px;
      display: flex;
      flex-direction: column;
      gap: 15px;
    }

    .header-bar {
      display: flex;
      justify-content: space-between;
      align-items: center;
    }

    .header-bar h2 {
      font-size: 20px;
    }

    .status-tags {
      display: flex;
      gap: 8px;
    }

    .tag {
      background-color: #1e293b;
      padding: 4px 10px;
      border-radius: 15px;
      font-size: 11px;
      color: #cbd5e1;
    }

    /* Grid Layout Fit for Height */
    .dashboard-grid {
      display: grid;
      grid-template-columns: repeat(3, 1fr);
      gap: 12px;
      flex: 1;
    }

    .card {
      background-color: #131d31;
      border: 1px solid #1e293b;
      border-radius: 12px;
      padding: 14px;
      display: flex;
      flex-direction: column;
      justify-content: center;
    }

    .card-title {
      font-size: 12px;
      color: #94a3b8;
      margin-bottom: 6px;
    }

    .card-value {
      font-size: 24px;
      font-weight: 700;
      color: #ffffff;
      margin-bottom: 4px;
    }

    .card-subtitle {
      font-size: 11px;
      color: #64748b;
    }
    .cell-grid {
      display: grid;
      grid-template-columns: 1fr 1fr;
      gap: 8px;
      margin-top: 4px;
    }

    .cell-item {
      background-color: #1a263d;
      padding: 8px 12px;
      border-radius: 8px;
      display: flex;
      justify-content: space-between;
      font-size: 12px;
    }

    .cell-value {
      font-weight: 600;
      color: #38bdf8;
    }
)WEB_48b33b0fed";

static const char kAsset_telemetry_html[] = R"WEB_14662895b3(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Telemetry - BMS Control Centre</title>
  <link rel="stylesheet" href="telemetry.css">
</head>
<body>
  <div class="sidebar">
    <div class="brand-box">
      <h3>BMS Control Centre</h3>
      <p id="stm">ESP32 Control Hub</p>
    </div>

    <a href="diagnostics.html" class="nav-btn">Dashboard</a>
    <a href="telemetry.html" class="nav-btn active">Telemetry</a>
    <a href="charts.html" class="nav-btn">Charts</a>
    <a href="firmware.html" class="nav-btn">Firmware Update</a>
    <a href="config.html" class="nav-btn">Configuration</a>
    <a href="logs.html" class="nav-btn">Logs</a>
    <a href="diagnostics.html" class="nav-btn">Diagnostics</a>
    <a href="settings.html" class="nav-btn">Settings</a>
    <a href="about.html" class="nav-btn">About</a>
  </div>
  <div class="main-content">
    
    <div class="header-bar">
      <h2>Telemetry</h2>
      <div class="status-tags">
        <span class="tag" id="status-pill">🔴 Offline</span>
        <span class="tag">10:15 am</span>
      </div>
    </div>
    <div class="dashboard-grid">
      
      <div class="card">
        <div class="card-title">Battery Voltage</div>
        <div class="card-value" id="volt-val">12.53 V</div>
        <div class="card-subtitle">Range: 10.0 - 15.0 V</div>
      </div>

      <div class="card">
        <div class="card-title">Battery Current</div>
        <div class="card-value" id="curr-val">2.31 A</div>
        <div class="card-subtitle">Discharging</div>
      </div>

      <div class="card">
        <div class="card-title">Temperature</div>
        <div class="card-value" id="temp-val">31.2 °C</div>
        <div class="card-subtitle">Range: 0 - 60 °C</div>
      </div>

      <div class="card">
        <div class="card-title">State of Charge</div>
        <div class="card-value">87%</div>
      </div>

      <div class="card" style="grid-column: span 2;">
        <div class="card-title">Cell Voltages</div>
        <div class="cell-grid">
          <div class="cell-item">
            <span>Cell 1</span>
            <span class="cell-value">3.132 V</span>
          </div>
          <div class="cell-item">
            <span>Cell 2</span>
            <span class="cell-value">3.128 V</span>
          </div>
          <div class="cell-item">
            <span>Cell 3</span>
            <span class="cell-value">3.122 V</span>
          </div>
          <div class="cell-item">
            <span>Cell 4</span>
            <span class="cell-value">3.127 V</span>
          </div>
        </div>
      </div>

    </div>

  </div>
  <script src="telemetry-api.js"></script>
  <script src="telemetry.js"></script>
</body>
</html>
)WEB_14662895b3";

static const char kAsset_telemetry_js[] = R"WEB_a9f080542a(
let pageSimInterval = null;
let liveInterval = null;

function updatePageData() {
  if (typeof drawGraphs === 'function') drawGraphs();
}

function startSimulation() {
  if (!pageSimInterval) {
    pageSimInterval = setInterval(updatePageData, 500);
  }
}

function stopSimulation() {
  if (pageSimInterval) {
    clearInterval(pageSimInterval);
    pageSimInterval = null;
  }
}

function applyTelemetryToPage(values) {
  const voltage = document.querySelector('.voltage-value') || document.getElementById('volt-val');
  const current = document.querySelector('.current-value') || document.getElementById('curr-val');
  const temp = document.querySelector('.temp-value') || document.getElementById('temp-val');
  const statusTag = document.getElementById('status-pill');

  if (voltage && typeof values.voltage === 'number') {
    voltage.innerText = values.voltage.toFixed(2) + ' V';
  }
  if (current && typeof values.current === 'number') {
    current.innerText = values.current.toFixed(2) + ' A';
  }
  if (temp && typeof values.temperature === 'number') {
    temp.innerText = values.temperature.toFixed(1) + ' °C';
  }
  if (statusTag) {
    statusTag.innerHTML = values.valid ? '🟢 Online' : '🔴 Offline';
    statusTag.style.color = values.valid ? '#4ade80' : '#f87171';
    statusTag.style.backgroundColor = values.valid ? '#143823' : '#1e293b';
  }
}

async function refreshLiveTelemetry() {
  try {
    const payload = await fetchLiveTelemetry();
    applyTelemetryToPage(telemetryToDisplayValues(payload));
  } catch (error) {
    applyTelemetryToPage({ valid: false });
  }
}

function startLivePolling() {
  if (!liveInterval) {
    refreshLiveTelemetry();
    liveInterval = setInterval(refreshLiveTelemetry, 2000);
  }
}

function stopLivePolling() {
  if (liveInterval) {
    clearInterval(liveInterval);
    liveInterval = null;
  }
}

function syncSimMode() {
  const isSimOn = isSimulatedMode();
  const btn = document.getElementById('sim-mode-btn');

  if (btn) {
    btn.innerText = isSimOn ? 'Simulated Mode: On' : 'Simulated Mode: Off';
    btn.style.backgroundColor = isSimOn ? '#083c5a' : '#1e293b';
    btn.style.color = isSimOn ? '#38bdf8' : '#94a3b8';
    btn.style.borderColor = isSimOn ? '#162646' : '#334155';
  }

  if (isSimOn) {
    stopLivePolling();
    startSimulation();
  } else {
    stopSimulation();
    startLivePolling();
  }
}

function toggleSimulatedMode() {
  setSimulatedMode(!isSimulatedMode());
  syncSimMode();
}

window.addEventListener('DOMContentLoaded', () => {
  ensureSimulatedModeDefault();
  syncSimMode();

  const stmLabel = document.getElementById('stm');
  if (stmLabel) {
    stmLabel.innerHTML = 'ESP32 Control Hub';
  }
});

window.addEventListener('storage', (event) => {
  if (event.key === 'simulatedMode') {
    syncSimMode();
  }
});

)WEB_a9f080542a";

static const EmbeddedAsset kEmbeddedAssets[] = {
    {"/about.css", kAsset_about_css, sizeof(kAsset_about_css) - 1U, "text/css"},
    {"/about.html", kAsset_about_html, sizeof(kAsset_about_html) - 1U, "text/html"},
    {"/about.js", kAsset_about_js, sizeof(kAsset_about_js) - 1U, "application/javascript"},
    {"/chart.css", kAsset_chart_css, sizeof(kAsset_chart_css) - 1U, "text/css"},
    {"/chart.js", kAsset_chart_js, sizeof(kAsset_chart_js) - 1U, "application/javascript"},
    {"/charts.html", kAsset_charts_html, sizeof(kAsset_charts_html) - 1U, "text/html"},
    {"/config.css", kAsset_config_css, sizeof(kAsset_config_css) - 1U, "text/css"},
    {"/config.html", kAsset_config_html, sizeof(kAsset_config_html) - 1U, "text/html"},
    {"/config.js", kAsset_config_js, sizeof(kAsset_config_js) - 1U, "application/javascript"},
    {"/diagnostics.css", kAsset_diagnostics_css, sizeof(kAsset_diagnostics_css) - 1U, "text/css"},
    {"/diagnostics.html", kAsset_diagnostics_html, sizeof(kAsset_diagnostics_html) - 1U, "text/html"},
    {"/diagnostics.js", kAsset_diagnostics_js, sizeof(kAsset_diagnostics_js) - 1U, "application/javascript"},
    {"/firmware.css", kAsset_firmware_css, sizeof(kAsset_firmware_css) - 1U, "text/css"},
    {"/firmware.html", kAsset_firmware_html, sizeof(kAsset_firmware_html) - 1U, "text/html"},
    {"/firmware.js", kAsset_firmware_js, sizeof(kAsset_firmware_js) - 1U, "application/javascript"},
    {"/index.html", kAsset_index_html, sizeof(kAsset_index_html) - 1U, "text/html"},
    {"/logs.css", kAsset_logs_css, sizeof(kAsset_logs_css) - 1U, "text/css"},
    {"/logs.html", kAsset_logs_html, sizeof(kAsset_logs_html) - 1U, "text/html"},
    {"/logs.js", kAsset_logs_js, sizeof(kAsset_logs_js) - 1U, "application/javascript"},
    {"/main.js", kAsset_main_js, sizeof(kAsset_main_js) - 1U, "application/javascript"},
    {"/setting.css", kAsset_setting_css, sizeof(kAsset_setting_css) - 1U, "text/css"},
    {"/settings.html", kAsset_settings_html, sizeof(kAsset_settings_html) - 1U, "text/html"},
    {"/settings.js", kAsset_settings_js, sizeof(kAsset_settings_js) - 1U, "application/javascript"},
    {"/style.css", kAsset_style_css, sizeof(kAsset_style_css) - 1U, "text/css"},
    {"/telemetry-api.js", kAsset_telemetry_api_js, sizeof(kAsset_telemetry_api_js) - 1U, "application/javascript"},
    {"/telemetry.css", kAsset_telemetry_css, sizeof(kAsset_telemetry_css) - 1U, "text/css"},
    {"/telemetry.html", kAsset_telemetry_html, sizeof(kAsset_telemetry_html) - 1U, "text/html"},
    {"/telemetry.js", kAsset_telemetry_js, sizeof(kAsset_telemetry_js) - 1U, "application/javascript"},
};

static constexpr size_t kEmbeddedAssetCount = sizeof(kEmbeddedAssets) / sizeof(kEmbeddedAssets[0]);

} // namespace

const EmbeddedAsset* findEmbeddedAsset(const char* path) noexcept
{
    if (path == nullptr) {
        return nullptr;
    }

    for (size_t index = 0U; index < kEmbeddedAssetCount; ++index) {
        if (strcmp(kEmbeddedAssets[index].path, path) == 0) {
            return &kEmbeddedAssets[index];
        }
    }

    return nullptr;
}

} // namespace control_hub::dashboard
