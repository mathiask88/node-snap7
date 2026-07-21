import snap7, {
  PlcStatus,
  S7Area,
  S7Client,
  S7Partner,
  S7Server,
  S7WordLen,
  ServerArea,
  ServerOperation,
  ServerStatus,
  type BlocksList,
  type DateTimeObject,
  type PartnerStatus,
  type S7Tag,
  type SrvEvent,
} from 'node-snap7';

const client = new S7Client();
const server = new S7Server();
const partner = new S7Partner(true);

new snap7.S7Client();
client.ReadArea(S7Area.DB, 1, 0, 1, S7WordLen.Byte);
server.GetArea(ServerArea.PE);
server.GetArea(ServerArea.DB, 1);

const serverStatus: ServerStatus = server.ServerStatus();
const cpuStatus: PlcStatus = server.GetCpuStatus();
const partnerStatus: PartnerStatus = partner.Status();
void [serverStatus, cpuStatus, partnerStatus];

server.on('readWrite', (_sender, operation, tag, _buffer, done) => {
  const typedOperation: ServerOperation = operation;
  const typedTag: S7Tag = tag;
  void [typedOperation, typedTag];
  done();
});

client.ReadArea(S7Area.DB, 1, 0, 1, S7WordLen.Byte, (err, data) => {
  if (err || !data) return;
  data.readUInt8(0);
});

// @ts-expect-error arbitrary numbers are not valid server areas
const invalidArea: ServerArea = 123456;
void invalidArea;

declare const blocks: BlocksList;
declare const dateTime: DateTimeObject;
declare const event: SrvEvent;
void [blocks, dateTime, event];
