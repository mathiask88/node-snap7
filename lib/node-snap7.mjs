import { createRequire } from 'module';

const require = createRequire(import.meta.url);
const snap7 = require('./node-snap7.js');

export default snap7;
export const { S7Client, S7Server } = snap7;
