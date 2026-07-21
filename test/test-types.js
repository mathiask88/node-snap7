const { readFileSync } = require('fs');
const { join } = require('path');
const { test } = require('node:test');
const assert = require('assert');
const snap7 = require('../lib/node-snap7');

const declaration = readFileSync(join(__dirname, '..', 'lib', 'node-snap7.d.ts'), 'utf8');

function declaredConstant(name) {
  const block = declaration.match(
    new RegExp(`export const ${name}: Readonly<\\{([\\s\\S]*?)\\}>;`)
  );

  assert.ok(block, `Missing literal declaration for ${name}`);

  const result = {};
  const property = /^\s*([A-Za-z][A-Za-z0-9_]*):\s*(0x[0-9A-F]+|\d+);\s*$/gim;
  let match;

  while ((match = property.exec(block[1])) !== null) {
    result[match[1]] = Number(match[2]);
  }

  return result;
}

test('declaration constant literals match the compiled addon', () => {
  for (const [name, value] of Object.entries(snap7)) {
    if (value !== null && typeof value === 'object') {
      assert.deepStrictEqual(declaredConstant(name), value, `${name} differs`);
    }
  }
});
