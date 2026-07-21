declare module 'node-snap7' {
  import { EventEmitter } from 'events';

  export interface Snap7Error extends Error {
    code?: string; // Symbolic Snap7 code, e.g., "SNAP7_CLIENT_CODE_33"
    errno?: number; // Numeric Snap7/ISO/TCP error code
  }

  export interface Snap7RWError extends Error {
    expectedLength?: number;
    actualLength?: number;
  }

  export type Snap7Callback<T = void> = (err: Snap7Error | null, result?: T) => void;

  export interface S7Client {
    // Connection Methods
    Connect(): Promise<void>;
    Connect(callback: Snap7Callback): void;
    ConnectSync(): void;

    ConnectTo(ip: string, rack: number, slot: number): Promise<void>;
    ConnectTo(ip: string, rack: number, slot: number, callback: Snap7Callback): void;
    ConnectToSync(ip: string, rack: number, slot: number): void;
    SetConnectionParams(remoteAddress: string, localTSAP: number, remoteTSAP: number): void;
    SetConnectionType(type: ConnectionType): void;
    Disconnect(): void;

    // Parameter Methods
    GetParam(paramNumber: ClientParameter): number;
    SetParam(paramNumber: ClientParameter, value: number): void;

    // Data I/O Methods
    ReadArea(area: S7Area, dbNumber: number, start: number, amount: number, wordLen: S7WordLen): Promise<Buffer>;
    ReadArea(area: S7Area, dbNumber: number, start: number, amount: number, wordLen: S7WordLen, callback: Snap7Callback<Buffer>): void;
    ReadAreaSync(area: S7Area, dbNumber: number, start: number, amount: number, wordLen: S7WordLen): Buffer;

    WriteArea(area: S7Area, dbNumber: number, start: number, amount: number, wordLen: S7WordLen, buffer: Buffer): Promise<void>;
    WriteArea(area: S7Area, dbNumber: number, start: number, amount: number, wordLen: S7WordLen, buffer: Buffer, callback: Snap7Callback): void;
    WriteAreaSync(area: S7Area, dbNumber: number, start: number, amount: number, wordLen: S7WordLen, buffer: Buffer): void;

    ReadMultiVars(items: S7MultiVarRead[]): Promise<S7MultiVarReadResult[]>;
    ReadMultiVars(items: S7MultiVarRead[], callback: Snap7Callback<S7MultiVarReadResult[]>): void;
    ReadMultiVarsSync(items: S7MultiVarRead[]): S7MultiVarReadResult[];

    WriteMultiVars(items: S7MultiVarWrite[]): Promise<S7MultiVarWriteResult[]>;
    WriteMultiVars(items: S7MultiVarWrite[], callback: Snap7Callback<S7MultiVarWriteResult[]>): void;
    WriteMultiVarsSync(items: S7MultiVarWrite[]): S7MultiVarWriteResult[];

    // Convenience Methods
    DBRead(dbNumber: number, start: number, size: number): Promise<Buffer>;
    DBRead(dbNumber: number, start: number, size: number, callback: Snap7Callback<Buffer>): void;
    DBReadSync(dbNumber: number, start: number, size: number): Buffer;

    DBWrite(dbNumber: number, start: number, size: number, buffer: Buffer): Promise<void>;
    DBWrite(dbNumber: number, start: number, size: number, buffer: Buffer, callback: Snap7Callback): void;
    DBWriteSync(dbNumber: number, start: number, size: number, buffer: Buffer): void;

    MBRead(start: number, size: number): Promise<Buffer>;
    MBRead(start: number, size: number, callback: Snap7Callback<Buffer>): void;
    MBReadSync(start: number, size: number): Buffer;

    MBWrite(start: number, size: number, buffer: Buffer): Promise<void>;
    MBWrite(start: number, size: number, buffer: Buffer, callback: Snap7Callback): void;
    MBWriteSync(start: number, size: number, buffer: Buffer): void;

    EBRead(start: number, size: number): Promise<Buffer>;
    EBRead(start: number, size: number, callback: Snap7Callback<Buffer>): void;
    EBReadSync(start: number, size: number): Buffer;

    EBWrite(start: number, size: number, buffer: Buffer): Promise<void>;
    EBWrite(start: number, size: number, buffer: Buffer, callback: Snap7Callback): void;
    EBWriteSync(start: number, size: number, buffer: Buffer): void;

    ABRead(start: number, size: number): Promise<Buffer>;
    ABRead(start: number, size: number, callback: Snap7Callback<Buffer>): void;
    ABReadSync(start: number, size: number): Buffer;

    ABWrite(start: number, size: number, buffer: Buffer): Promise<void>;
    ABWrite(start: number, size: number, buffer: Buffer, callback: Snap7Callback): void;
    ABWriteSync(start: number, size: number, buffer: Buffer): void;

    TMRead(start: number, size: number): Promise<Buffer>;
    TMRead(start: number, size: number, callback: Snap7Callback<Buffer>): void;
    TMReadSync(start: number, size: number): Buffer;

    TMWrite(start: number, size: number, buffer: Buffer): Promise<void>;
    TMWrite(start: number, size: number, buffer: Buffer, callback: Snap7Callback): void;
    TMWriteSync(start: number, size: number, buffer: Buffer): void;

    CTRead(start: number, size: number): Promise<Buffer>;
    CTRead(start: number, size: number, callback: Snap7Callback<Buffer>): void;
    CTReadSync(start: number, size: number): Buffer;

    CTWrite(start: number, size: number, buffer: Buffer): Promise<void>;
    CTWrite(start: number, size: number, buffer: Buffer, callback: Snap7Callback): void;
    CTWriteSync(start: number, size: number, buffer: Buffer): void;

    // Block Operations
    ListBlocks(): Promise<BlocksList>;
    ListBlocks(callback: Snap7Callback<BlocksList>): void;
    ListBlocksSync(): BlocksList;

    ListBlocksOfType(blockType: BlockType): Promise<number[]>;
    ListBlocksOfType(blockType: BlockType, callback: Snap7Callback<number[]>): void;
    ListBlocksOfTypeSync(blockType: BlockType): number[];

    Upload(blockType: BlockType, blockNumber: number, size: number): Promise<Buffer>;
    Upload(blockType: BlockType, blockNumber: number, size: number, callback: Snap7Callback<Buffer>): void;
    UploadSync(blockType: BlockType, blockNumber: number, size: number): Buffer;

    FullUpload(blockType: BlockType, blockNumber: number, size: number): Promise<Buffer>;
    FullUpload(blockType: BlockType, blockNumber: number, size: number, callback: Snap7Callback<Buffer>): void;
    FullUploadSync(blockType: BlockType, blockNumber: number, size: number): Buffer;

    Download(blockNumber: number, buffer: Buffer): Promise<void>;
    Download(blockNumber: number, buffer: Buffer, callback: Snap7Callback): void;
    DownloadSync(blockNumber: number, buffer: Buffer): void;

    Delete(blockType: BlockType, blockNum: number): Promise<void>;
    Delete(blockType: BlockType, blockNum: number, callback: Snap7Callback): void;
    DeleteSync(blockType: BlockType, blockNum: number): void;

    DBFill(dbNumber: number, fillChar: number | string): Promise<void>;
    DBFill(dbNumber: number, fillChar: number | string, callback: Snap7Callback): void;
    DBFillSync(dbNumber: number, fillChar: number | string): void;

    DBGet(dbNumber: number): Promise<Buffer>;
    DBGet(dbNumber: number, callback: Snap7Callback<Buffer>): void;
    DBGetSync(dbNumber: number): Buffer;

    // PLC Control
    PlcHotStart(): Promise<void>;
    PlcHotStart(callback: Snap7Callback): void;
    PlcHotStartSync(): void;

    PlcColdStart(): Promise<void>;
    PlcColdStart(callback: Snap7Callback): void;
    PlcColdStartSync(): void;

    PlcStop(): Promise<void>;
    PlcStop(callback: Snap7Callback): void;
    PlcStopSync(): void;

    // Control Functions
    CopyRamToRom(timeout: number): Promise<void>;
    CopyRamToRom(timeout: number, callback: Snap7Callback): void;
    CopyRamToRomSync(timeout: number): void;

    Compress(timeout: number): Promise<void>;
    Compress(timeout: number, callback: Snap7Callback): void;
    CompressSync(timeout: number): void;

    PlcStatus(): Promise<PlcStatus>;
    PlcStatus(callback: Snap7Callback<PlcStatus>): void;
    PlcStatusSync(): PlcStatus;

    GetProtection(): Promise<Protection>;
    GetProtection(callback: Snap7Callback<Protection>): void;
    GetProtectionSync(): Protection;

    SetSessionPassword(password: string): Promise<void>;
    SetSessionPassword(password: string, callback: Snap7Callback): void;
    SetSessionPasswordSync(password: string): void;

    ClearSessionPassword(): Promise<void>;
    ClearSessionPassword(callback: Snap7Callback): void;
    ClearSessionPasswordSync(): void;

    // DateTime Functions
    GetPlcDateTime(): Promise<Date>;
    GetPlcDateTime(callback: Snap7Callback<Date>): void;
    GetPlcDateTimeSync(): Date;

    SetPlcDateTime(date: Date | DateTimeObject): Promise<void>;
    SetPlcDateTime(date: Date | DateTimeObject, callback: Snap7Callback): void;
    SetPlcDateTimeSync(date: Date | DateTimeObject): void;

    SetPlcSystemDateTime(): Promise<void>;
    SetPlcSystemDateTime(callback: Snap7Callback): void;
    SetPlcSystemDateTimeSync(): void;

    // SZL Operations
    ReadSZL(id: number, index: number): Promise<Buffer>;
    ReadSZL(id: number, index: number, callback: Snap7Callback<Buffer>): void;
    ReadSZLSync(id: number, index: number): Buffer;

    ReadSZLList(): Promise<number[]>;
    ReadSZLList(callback: Snap7Callback<number[]>): void;
    ReadSZLListSync(): number[];

    // Information
    GetCpuInfo(): Promise<CpuInfo>;
    GetCpuInfo(callback: Snap7Callback<CpuInfo>): void;
    GetCpuInfoSync(): CpuInfo;

    GetCpInfo(): Promise<CpInfo>;
    GetCpInfo(callback: Snap7Callback<CpInfo>): void;
    GetCpInfoSync(): CpInfo;

    GetOrderCode(): Promise<OrderCode>;
    GetOrderCode(callback: Snap7Callback<OrderCode>): void;
    GetOrderCodeSync(): OrderCode;

    // Block Info
    GetAgBlockInfo(blockType: BlockType, blockNumber: number): Promise<BlockInfo>;
    GetAgBlockInfo(blockType: BlockType, blockNumber: number, callback: Snap7Callback<BlockInfo>): void;
    GetAgBlockInfoSync(blockType: BlockType, blockNumber: number): BlockInfo;

    GetPgBlockInfo(buffer: Buffer): BlockInfo;

    // Properties and Info
    ExecTime(): number;
    PDURequested(): number;
    PDULength(): number;
    Connected(): boolean;
    ErrorText(code: number): string;
  }

  export interface S7MultiVarRead {
    Area: S7Area;
    WordLen: S7WordLen;
    DBNumber?: number | undefined;
    Start: number;
    Amount: number;
  }

  export interface S7MultiVarReadResult {
    Result: number;
    Data: Buffer | null;
  }

  export interface S7MultiVarWrite {
    Area: S7Area;
    WordLen: S7WordLen;
    DBNumber?: number | undefined;
    Start: number;
    Amount: number;
    Data: Buffer;
  }

  export interface S7MultiVarWriteResult {
    Result: number;
  }

  export interface BlocksList {
    OBCount: number;
    FBCount: number;
    FCCount: number;
    SFBCount: number;
    SFCCount: number;
    DBCount: number;
    SDBCount: number;
  }

  export interface Protection {
    sch_schal: number;
    sch_par: number;
    sch_rel: number;
    bart_sch: number;
    anl_sch: number;
  }

  export interface DateTimeObject {
    year: number;
    month: number;
    day: number;
    hours: number;
    minutes: number;
    seconds: number;
  }

  export interface BlockInfo {
    BlkType: number;
    BlkNumber: number;
    BlkLang: number;
    BlkFlags: number;
    MC7Size: number;
    LoadSize: number;
    LocalData: number;
    SBBLength: number;
    CheckSum: number;
    Version: number;
    CodeDate: string;
    IntfDate: string;
    Author: string;
    Family: string;
    Header: string;
  }

  export interface CpInfo {
    MaxPduLength: number;
    MaxConnections: number;
    MaxMpiRate: number;
    MaxBusRate: number;
  }

  export interface CpuInfo {
    ModuleTypeName: string;
    SerialNumber: string;
    ASName: string;
    Copyright: string;
    ModuleName: string;
  }

  export interface OrderCode {
    Code: string;
    V1: number;
    V2: number;
    V3: number;
  }

  export const ConnectionType: Readonly<{
    PG: 0x01;
    OP: 0x02;
    BASIC: 0x03;
  }>;
  export type ConnectionType = (typeof ConnectionType)[keyof typeof ConnectionType];

  export const PlcStatus: Readonly<{
    Unknown: 0x00;
    Run: 0x08;
    Stop: 0x04;
  }>;
  export type PlcStatus = (typeof PlcStatus)[keyof typeof PlcStatus];

  export const S7Area: Readonly<{
    PE: 0x81;
    PA: 0x82;
    MK: 0x83;
    DB: 0x84;
    CT: 0x1C;
    TM: 0x1D;
  }>;
  export type S7Area = (typeof S7Area)[keyof typeof S7Area];

  export const S7WordLen: Readonly<{
    Bit: 0x01;
    Byte: 0x02;
    Word: 0x04;
    DWord: 0x06;
    Real: 0x08;
    Counter: 0x1C;
    Timer: 0x1D;
  }>;
  export type S7WordLen = (typeof S7WordLen)[keyof typeof S7WordLen];

  export const BlockType: Readonly<{
    OB: 0x38;
    DB: 0x41;
    SDB: 0x42;
    FC: 0x43;
    SFC: 0x44;
    FB: 0x45;
    SFB: 0x46;
  }>;
  export type BlockType = (typeof BlockType)[keyof typeof BlockType];

  export const SubBlockType: Readonly<{
    OB: 0x08;
    DB: 0x0A;
    SDB: 0x0B;
    FC: 0x0C;
    SFC: 0x0D;
    FB: 0x0E;
    SFB: 0x0F;
  }>;
  export type SubBlockType = (typeof SubBlockType)[keyof typeof SubBlockType];

  export const BlockLang: Readonly<{
    AWL: 0x01;
    KOP: 0x02;
    FUP: 0x03;
    SCL: 0x04;
    DB: 0x05;
    GRAPH: 0x06;
  }>;
  export type BlockLang = (typeof BlockLang)[keyof typeof BlockLang];

  export const ClientParameter: Readonly<{
    RemotePort: 2;
    PingTimeout: 3;
    SendTimeout: 4;
    RecvTimeout: 5;
    SrcRef: 7;
    DstRef: 8;
    SrcTSap: 9;
    PDURequest: 10;
  }>;
  export type ClientParameter = (typeof ClientParameter)[keyof typeof ClientParameter];

  export const ServerParameter: Readonly<{
    LocalPort: 1;
    WorkInterval: 6;
    PDURequest: 10;
    MaxClients: 11;
  }>;
  export type ServerParameter = (typeof ServerParameter)[keyof typeof ServerParameter];

  export const PartnerParameter: Readonly<{
    PDURequest: 10;
    SrcTSap: 9;
    DstRef: 8;
    SrcRef: 7;
    WorkInterval: 6;
    RecvTimeout: 5;
    SendTimeout: 4;
    PingTimeout: 3;
    RemotePort: 2;
    BSendTimeout: 12;
    BRecvTimeout: 13;
    RecoveryTime: 14;
    KeepAliveTime: 15;
  }>;
  export type PartnerParameter = (typeof PartnerParameter)[keyof typeof PartnerParameter];

  export const ClientError: Readonly<{
    NegotiatingPDU: 0x00100000;
    InvalidParams: 0x00200000;
    JobPending: 0x00300000;
    TooManyItems: 0x00400000;
    InvalidWordLen: 0x00500000;
    PartialDataWritten: 0x00600000;
    SizeOverPDU: 0x00700000;
    InvalidPlcAnswer: 0x00800000;
    AddressOutOfRange: 0x00900000;
    InvalidTransportSize: 0x00A00000;
    WriteDataSizeMismatch: 0x00B00000;
    ItemNotAvailable: 0x00C00000;
    InvalidValue: 0x00D00000;
    CannotStartPLC: 0x00E00000;
    AlreadyRun: 0x00F00000;
    CannotStopPLC: 0x01000000;
    CannotCopyRamToRom: 0x01100000;
    CannotCompress: 0x01200000;
    AlreadyStop: 0x01300000;
    FunNotAvailable: 0x01400000;
    UploadSequenceFailed: 0x01500000;
    InvalidDataSizeRecvd: 0x01600000;
    InvalidBlockType: 0x01700000;
    InvalidBlockNumber: 0x01800000;
    InvalidBlockSize: 0x01900000;
    DownloadSequenceFailed: 0x01A00000;
    InsertRefused: 0x01B00000;
    DeleteRefused: 0x01C00000;
    NeedPassword: 0x01D00000;
    InvalidPassword: 0x01E00000;
    NoPasswordToSetOrClear: 0x01F00000;
    JobTimeout: 0x02000000;
    PartialDataRead: 0x02100000;
    BufferTooSmall: 0x02200000;
    FunctionRefused: 0x02300000;
    Destroying: 0x02400000;
    InvalidParamNumber: 0x02500000;
    CannotChangeParam: 0x02600000;
  }>;
  export type ClientError = (typeof ClientError)[keyof typeof ClientError];

  export const ServerError: Readonly<{
    CannotStart: 0x00100000;
    DBNullPointer: 0x00200000;
    AreaAlreadyExists: 0x00300000;
    UnknownArea: 0x00400000;
    InvalidParams: 0x00500000;
    TooManyDB: 0x00600000;
    InvalidParamNumber: 0x00700000;
    CannotChangeParam: 0x00800000;
  }>;
  export type ServerError = (typeof ServerError)[keyof typeof ServerError];

  export const PartnerError: Readonly<{
    BindError: 0x01000000;
    Destroying: 0x01100000;
    InvalidParamNumber: 0x01200000;
    CannotChangeParam: 0x01300000;
    BufferTooSmall: 0x01400000;
  }>;
  export type PartnerError = (typeof PartnerError)[keyof typeof PartnerError];

  export const ServerArea: Readonly<{
    PE: 0;
    PA: 1;
    MK: 2;
    CT: 3;
    TM: 4;
    DB: 5;
  }>;
  export type ServerArea = (typeof ServerArea)[keyof typeof ServerArea];

  export const ServerEventCode: Readonly<{
    ServerStarted: 0x00000001;
    ServerStopped: 0x00000002;
    ListenerCannotStart: 0x00000004;
    ClientAdded: 0x00000008;
    ClientRejected: 0x00000010;
    ClientNoRoom: 0x00000020;
    ClientException: 0x00000040;
    ClientDisconnected: 0x00000080;
    ClientTerminated: 0x00000100;
    ClientsDropped: 0x00000200;
    PDUincoming: 0x00010000;
    DataRead: 0x00020000;
    DataWrite: 0x00040000;
    NegotiatePDU: 0x00080000;
    ReadSZL: 0x00100000;
    Clock: 0x00200000;
    Upload: 0x00400000;
    Download: 0x00800000;
    Directory: 0x01000000;
    Security: 0x02000000;
    Control: 0x04000000;
    All: 0xFFFFFFFF;
    None: 0x00000000;
  }>;
  export type ServerEventCode = (typeof ServerEventCode)[keyof typeof ServerEventCode];

  export const ServerEventSubcode: Readonly<{
    Unknown: 0;
    StartUpload: 1;
    StartDownload: 1;
    GetBlockList: 1;
    StartListBoT: 2;
    ListBoT: 3;
    GetBlockInfo: 4;
    GetClock: 1;
    SetClock: 2;
    SetPassword: 1;
    ClrPassword: 2;
  }>;
  export type ServerEventSubcode = (typeof ServerEventSubcode)[keyof typeof ServerEventSubcode];

  export const ServerEventGroup: Readonly<{
    Programmer: 0x41;
    CyclicData: 0x42;
    BlocksInfo: 0x43;
    SZL: 0x44;
    Password: 0x45;
    BSend: 0x46;
    Clock: 0x47;
    Security: 0x45;
  }>;
  export type ServerEventGroup = (typeof ServerEventGroup)[keyof typeof ServerEventGroup];

  export const ServerEventControl: Readonly<{
    Unknown: 0;
    ColdStart: 1;
    WarmStart: 2;
    Stop: 3;
    Compress: 4;
    CpyRamRom: 5;
    InsDel: 6;
  }>;
  export type ServerEventControl = (typeof ServerEventControl)[keyof typeof ServerEventControl];

  export const ServerEventResult: Readonly<{
    NoError: 0;
    FragmentRejected: 1;
    MalformedPDU: 2;
    SparseBytes: 3;
    CannotHandlePDU: 4;
    NotImplemented: 5;
    ErrException: 6;
    ErrAreaNotFound: 7;
    ErrOutOfRange: 8;
    ErrOverPDU: 9;
    ErrTransportSize: 10;
    InvalidGroupUData: 11;
    InvalidSZL: 12;
    DataSizeMismatch: 13;
    CannotUpload: 14;
    CannotDownload: 15;
    UploadInvalidID: 16;
    ResNotFound: 17;
  }>;
  export type ServerEventResult = (typeof ServerEventResult)[keyof typeof ServerEventResult];

  export const ServerStatus: Readonly<{
    SrvStopped: 0;
    SrvRunning: 1;
    SrvError: 2;
  }>;
  export type ServerStatus = (typeof ServerStatus)[keyof typeof ServerStatus];

  export const PartnerStatus: Readonly<{
    Stopped: 0;
    Connecting: 1;
    Waiting: 2;
    Linked: 3;
    Sending: 4;
    Receiving: 5;
    BindError: 6;
  }>;
  export type PartnerStatus = (typeof PartnerStatus)[keyof typeof PartnerStatus];

  export const ServerOperation: Readonly<{
    Read: 0;
    Write: 1;
  }>;
  export type ServerOperation = (typeof ServerOperation)[keyof typeof ServerOperation];

  export class S7Client {
    constructor();
  }

  export interface S7Server extends EventEmitter {
    // Administrative Methods
    /** Resolves on success; rejects with a Snap7Error on failure. */
    Start(): Promise<void>;
    Start(callback: Snap7Callback): void;
    StartSync(): void;
    /** Resolves on success; rejects with a Snap7Error on failure. */
    StartTo(ip: string): Promise<void>;
    StartTo(ip: string, callback: Snap7Callback): void;
    StartToSync(ip: string): void;
    /** Resolves on success; rejects with a Snap7Error on failure. */
    Stop(): Promise<void>;
    Stop(callback: Snap7Callback): void;
    StopSync(): void;
    GetParam(paramNumber: ServerParameter): number;
    SetParam(paramNumber: ServerParameter, value: number): void;
    SetResourceless(value: boolean): void;

    // Memory Methods
    RegisterArea(areaCode: ServerArea, index: number, buffer: Buffer): void;
    UnregisterArea(areaCode: typeof ServerArea.DB, index: number): void;
    UnregisterArea(areaCode: Exclude<ServerArea, typeof ServerArea.DB>): void;
    GetArea(areaCode: typeof ServerArea.DB, index: number): Buffer;
    GetArea(areaCode: Exclude<ServerArea, typeof ServerArea.DB>): Buffer;
    /** `index` is only used when `areaCode === ServerArea.DB`; otherwise it is ignored. */
    SetArea(areaCode: ServerArea, index: number, buffer: Buffer): void;
    LockArea(areaCode: typeof ServerArea.DB, index: number): void;
    LockArea(areaCode: Exclude<ServerArea, typeof ServerArea.DB>): void;
    UnlockArea(areaCode: typeof ServerArea.DB, index: number): void;
    UnlockArea(areaCode: Exclude<ServerArea, typeof ServerArea.DB>): void;

    // Event Methods
    GetEventsMask(): number;
    SetEventsMask(mask: number): void;

    // Miscellaneous Methods
    EventText(evt: SrvEvent): string;
    ErrorText(errNum: number): string;
    ServerStatus(): ServerStatus;
    ClientsCount(): number;
    GetCpuStatus(): PlcStatus;
    SetCpuStatus(status: PlcStatus): boolean;

    // EventEmitter overloads
    on(event: 'event', listener: (evt: SrvEvent) => void): this;
    on(
      event: 'readWrite',
      listener: (
        sender: string,
        operation: ServerOperation,
        tag: S7Tag,
        buffer: Buffer,
        callback: (buf?: Buffer) => void
      ) => void
    ): this;
    on(event: 'error', listener: (err: Snap7Error | Snap7RWError) => void): this;

  }

  export interface SrvEvent {
    EvtTime: Date;
    EvtSender: string;
    EvtCode: number;
    EvtRetCode: number;
    EvtParam1: number;
    EvtParam2: number;
    EvtParam3: number;
    EvtParam4: number;
  }

  export interface S7Tag {
    Area: S7Area;
    DBNumber: number;
    Start: number;
    Size: number;
    WordLen: S7WordLen;
  }

  export class S7Server extends EventEmitter {
    constructor();
  }

  export interface PartnerRecvEvent {
    result: number;
    R_ID: number;
    data: Buffer;
  }

  export interface S7Partner extends EventEmitter {
    Start(): Promise<void>;
    Start(callback: Snap7Callback): void;
    StartSync(): void;

    StartTo(localAddress: string, remoteAddress: string, localTSAP: number, remoteTSAP: number): Promise<void>;
    StartTo(localAddress: string, remoteAddress: string, localTSAP: number, remoteTSAP: number, callback: Snap7Callback): void;
    StartToSync(localAddress: string, remoteAddress: string, localTSAP: number, remoteTSAP: number): void;

    Stop(): Promise<void>;
    Stop(callback: Snap7Callback): void;
    StopSync(): void;

    GetParam(paramNumber: PartnerParameter): number;
    SetParam(paramNumber: PartnerParameter, value: number): void;

    BSend(rId: number, buffer: Buffer): Promise<void>;
    BSend(rId: number, buffer: Buffer, callback: Snap7Callback): void;
    BSendSync(rId: number, buffer: Buffer): void;

    BRecv(timeout: number): Promise<{ R_ID: number; data: Buffer }>;
    BRecv(timeout: number, callback: Snap7Callback<{ R_ID: number; data: Buffer }>): void;
    BRecvSync(timeout: number): { R_ID: number; data: Buffer };
    Status(): PartnerStatus;
    GetTimes(): { SendTime: number; RecvTime: number };
    GetStats(): { BytesSent: number; BytesRecv: number; ErrSend: number; ErrRecv: number };
    ErrorText(errNum: number): string;
    Linked(): boolean;

    on(event: 'recv', listener: (evt: PartnerRecvEvent) => void): this;
  }

  export class S7Partner extends EventEmitter {
    constructor(active: boolean);
  }

  export interface NodeSnap7 {
    S7Client: typeof S7Client;
    S7Server: typeof S7Server;
    S7Partner: typeof S7Partner;
    ClientError: typeof ClientError;
    ClientParameter: typeof ClientParameter;
    ConnectionType: typeof ConnectionType;
    S7Area: typeof S7Area;
    S7WordLen: typeof S7WordLen;
    BlockType: typeof BlockType;
    SubBlockType: typeof SubBlockType;
    BlockLang: typeof BlockLang;
    PlcStatus: typeof PlcStatus;
    ServerError: typeof ServerError;
    ServerParameter: typeof ServerParameter;
    ServerArea: typeof ServerArea;
    ServerOperation: typeof ServerOperation;
    ServerEventCode: typeof ServerEventCode;
    ServerEventSubcode: typeof ServerEventSubcode;
    ServerEventGroup: typeof ServerEventGroup;
    ServerEventControl: typeof ServerEventControl;
    ServerEventResult: typeof ServerEventResult;
    ServerStatus: typeof ServerStatus;
    PartnerParameter: typeof PartnerParameter;
    PartnerError: typeof PartnerError;
    PartnerStatus: typeof PartnerStatus;
  }

  const nodeSnap7: NodeSnap7;
  export default nodeSnap7;
}
