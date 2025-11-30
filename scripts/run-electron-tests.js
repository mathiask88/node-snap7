const { spawn, spawnSync } = require('child_process');
const electronBinary = require('electron');
const os = require('os');

function getElectronProcessVersions() {
  try {
    const result = spawnSync(
      electronBinary,
      ['-e', 'console.log(JSON.stringify(process.versions))'],
      {
        env: { ...process.env, ELECTRON_RUN_AS_NODE: '1' },
        encoding: 'utf8'
      }
    );
    if (result.status === 0 && result.stdout) {
      return JSON.parse(result.stdout.trim());
    }
  } catch {
    // ignore and fall back to empty object
  }
  return {};
}

const electronVersions = getElectronProcessVersions();
const electronNode = electronVersions.node || 'unknown';
const electronChrome = electronVersions.chrome || 'unknown';
const electronV8 = electronVersions.v8 || 'unknown';
const electronNapi = electronVersions.napi || 'unknown';
const electronVersion = electronVersions.electron || 'unknown';

console.log(
  `[electron-tests] electron ${electronVersion} (node ${electronNode}, chrome ${electronChrome}, v8 ${electronV8}, napi ${electronNapi}); runner node ${process.version} (v8 ${process.versions.v8}, napi ${process.versions.napi})${os.EOL}`
);

// Use Electron's embedded Node to run the Node.js test runner on the collected files.
// Keep concurrency at 1 to avoid port collisions in integration tests.
const child = spawn(
  electronBinary,
  ['--test', '--test-concurrency=1'],
  {
    stdio: 'inherit',
    env: {
      ...process.env,
      ELECTRON_RUN_AS_NODE: '1'
    }
  }
);

child.on('exit', code => {
  process.exit(code);
});

child.on('error', err => {
  console.error('Failed to start Electron for tests:', err);
  process.exit(1);
});
