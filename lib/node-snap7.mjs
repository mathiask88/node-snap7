import { createRequire } from 'module';

const require = createRequire(import.meta.url);
const snap7 = require('./node-snap7.js');

export default snap7;
export const {
  S7Client,
  S7Server,
  S7Partner,
  ClientError,
  ClientParameter,
  ConnectionType,
  S7Area,
  S7WordLen,
  BlockType,
  SubBlockType,
  BlockLang,
  PlcStatus,
  ServerError,
  ServerParameter,
  ServerArea,
  ServerOperation,
  ServerEventCode,
  ServerEventSubcode,
  ServerEventGroup,
  ServerEventControl,
  ServerEventResult,
  ServerStatus,
  PartnerParameter,
  PartnerError,
  PartnerStatus
} = snap7;
