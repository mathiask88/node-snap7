const { spawn } = require('child_process');
const os = require('os');

console.log(
  `[node-tests] node ${process.version} (v8 ${process.versions.v8}, napi ${process.versions.napi})${os.EOL}`
);

const child = spawn(
  process.execPath,
  ['--test', '--test-concurrency=1'],
  {
    stdio: 'inherit',
    env: process.env
  }
);

child.on('exit', code => {
  process.exit(code);
});

child.on('error', err => {
  console.error('Failed to start Node tests:', err);
  process.exit(1);
});
