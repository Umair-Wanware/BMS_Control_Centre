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
