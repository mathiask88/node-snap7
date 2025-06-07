declare module 'node-snap7' {
  import { EventEmitter } from 'events';

  export interface S7Client {
    // Connection Methods
    Connect(): Promise<void>;
    Connect(callback: (err: Error | null, result: void) => void): void;
    ConnectSync(): boolean;

    ConnectTo(ip: string, rack: number, slot: number): Promise<void>;
    ConnectTo(ip: string, rack: number, slot: number, callback: (err: Error | null, result: void) => void): void;
    ConnectToSync(ip: string, rack: number, slot: number): boolean;

    SetConnectionParams(remoteAddress: string, localTSAP: number, remoteTSAP: number): boolean;
    SetConnectionType(type: ConnectionType): boolean;
    Disconnect(): boolean;

    // Parameter Methods
    GetParam(paramNumber: Parameter): number;
    SetParam(paramNumber: Parameter, value: number): boolean;

    // Data I/O Methods
    ReadArea(area: Area, dbNumber: number, start: number, amount: number, wordLen: WordLen): Promise<Buffer>;
    ReadArea(area: Area, dbNumber: number, start: number, amount: number, wordLen: WordLen, callback: (err: Error | null, data: Buffer) => void): void;
    ReadAreaSync(area: Area, dbNumber: number, start: number, amount: number, wordLen: WordLen): Buffer;

    WriteArea(area: Area, dbNumber: number, start: number, amount: number, wordLen: WordLen, buffer: Buffer): Promise<void>;
    WriteArea(area: Area, dbNumber: number, start: number, amount: number, wordLen: WordLen, buffer: Buffer, callback: (err: Error | null, result: void) => void): void;
    WriteAreaSync(area: Area, dbNumber: number, start: number, amount: number, wordLen: WordLen, buffer: Buffer): boolean;

    ReadMultiVars(items: S7MultiVarRead[]): Promise<S7MultiVarReadResult[]>;
    ReadMultiVars(items: S7MultiVarRead[], callback: (err: Error | null, data: S7MultiVarReadResult[]) => void): void;
    ReadMultiVarsSync(items: S7MultiVarRead[]): S7MultiVarReadResult[];

    WriteMultiVars(items: S7MultiVarWrite[]): Promise<S7MultiVarWriteResult[]>;
    WriteMultiVars(items: S7MultiVarWrite[], callback: (err: Error | null, result: S7MultiVarWriteResult[]) => void): void;
    WriteMultiVarsSync(items: S7MultiVarWrite[]): S7MultiVarWriteResult[];

    // Convenience Methods
    DBRead(dbNumber: number, start: number, size: number): Promise<Buffer>;
    DBRead(dbNumber: number, start: number, size: number, callback: (err: Error | null, data: Buffer) => void): void;
    DBReadSync(dbNumber: number, start: number, size: number): Buffer;

    DBWrite(dbNumber: number, start: number, size: number, buffer: Buffer): Promise<void>;
    DBWrite(dbNumber: number, start: number, size: number, buffer: Buffer, callback: (err: Error | null, result: void) => void): void;
    DBWriteSync(dbNumber: number, start: number, size: number, buffer: Buffer): boolean;

    MBRead(start: number, size: number): Promise<Buffer>;
    MBRead(start: number, size: number, callback: (err: Error | null, data: Buffer) => void): void;
    MBReadSync(start: number, size: number): Buffer;

    MBWrite(start: number, size: number, buffer: Buffer): Promise<void>;
    MBWrite(start: number, size: number, buffer: Buffer, callback: (err: Error | null, result: void) => void): void;
    MBWriteSync(start: number, size: number, buffer: Buffer): boolean;

    EBRead(start: number, size: number): Promise<Buffer>;
    EBRead(start: number, size: number, callback: (err: Error | null, data: Buffer) => void): void;
    EBReadSync(start: number, size: number): Buffer;

    EBWrite(start: number, size: number, buffer: Buffer): Promise<void>;
    EBWrite(start: number, size: number, buffer: Buffer, callback: (err: Error | null, result: void) => void): void;
    EBWriteSync(start: number, size: number, buffer: Buffer): boolean;

    ABRead(start: number, size: number): Promise<Buffer>;
    ABRead(start: number, size: number, callback: (err: Error | null, data: Buffer) => void): void;
    ABReadSync(start: number, size: number): Buffer;

    ABWrite(start: number, size: number, buffer: Buffer): Promise<void>;
    ABWrite(start: number, size: number, buffer: Buffer, callback: (err: Error | null, result: void) => void): void;
    ABWriteSync(start: number, size: number, buffer: Buffer): boolean;

    TMRead(start: number, size: number): Promise<Buffer>;
    TMRead(start: number, size: number, callback: (err: Error | null, data: Buffer) => void): void;
    TMReadSync(start: number, size: number): Buffer;

    TMWrite(start: number, size: number, buffer: Buffer): Promise<void>;
    TMWrite(start: number, size: number, buffer: Buffer, callback: (err: Error | null, result: void) => void): void;
    TMWriteSync(start: number, size: number, buffer: Buffer): boolean;

    CTRead(start: number, size: number): Promise<Buffer>;
    CTRead(start: number, size: number, callback: (err: Error | null, data: Buffer) => void): void;
    CTReadSync(start: number, size: number): Buffer;

    CTWrite(start: number, size: number, buffer: Buffer): Promise<void>;
    CTWrite(start: number, size: number, buffer: Buffer, callback: (err: Error | null, result: void) => void): void;
    CTWriteSync(start: number, size: number, buffer: Buffer): boolean;

    // Block Operations
    ListBlocks(): Promise<BlocksList>;
    ListBlocks(callback: (err: Error | null, data: BlocksList) => void): void;
    ListBlocksSync(): BlocksList;

    ListBlocksOfType(blockType: BlockType): Promise<number[]>;
    ListBlocksOfType(blockType: BlockType, callback: (err: Error | null, data: number[]) => void): void;
    ListBlocksOfTypeSync(blockType: BlockType): number[];

    Upload(blockType: BlockType, blockNumber: number, size: number): Promise<Buffer>;
    Upload(blockType: BlockType, blockNumber: number, size: number, callback: (err: Error | null, data: Buffer) => void): void;
    UploadSync(blockType: BlockType, blockNumber: number, size: number): Buffer;

    FullUpload(blockType: BlockType, blockNumber: number, size: number): Promise<Buffer>;
    FullUpload(blockType: BlockType, blockNumber: number, size: number, callback: (err: Error | null, data: Buffer) => void): void;
    FullUploadSync(blockType: BlockType, blockNumber: number, size: number): Buffer;

    Download(blockNumber: number, buffer: Buffer): Promise<void>;
    Download(blockNumber: number, buffer: Buffer, callback: (err: Error | null, result: void) => void): void;
    DownloadSync(blockNumber: number, buffer: Buffer): boolean;

    Delete(blockType: BlockType, blockNum: number): Promise<void>;
    Delete(blockType: BlockType, blockNum: number, callback: (err: Error | null, result: void) => void): void;
    DeleteSync(blockType: BlockType, blockNum: number): boolean;

    DBFill(dbNumber: number, fillChar: number | string): Promise<void>;
    DBFill(dbNumber: number, fillChar: number | string, callback: (err: Error | null, result: void) => void): void;
    DBFillSync(dbNumber: number, fillChar: number | string): boolean;

    DBGet(dbNumber: number): Promise<Buffer>;
    DBGet(dbNumber: number, callback: (err: Error | null, data: Buffer) => void): void;
    DBGetSync(dbNumber: number): Buffer;

    // PLC Control
    PlcHotStart(): Promise<void>;
    PlcHotStart(callback: (err: Error | null, result: void) => void): void;
    PlcHotStartSync(): boolean;

    PlcColdStart(): Promise<void>;
    PlcColdStart(callback: (err: Error | null, result: void) => void): void;
    PlcColdStartSync(): boolean;

    PlcStop(): Promise<void>;
    PlcStop(callback: (err: Error | null, result: void) => void): void;
    PlcStopSync(): boolean;

    // Control Functions
    CopyRamToRom(timeout: number): Promise<void>;
    CopyRamToRom(timeout: number, callback: (err: Error | null, result: void) => void): void;
    CopyRamToRomSync(timeout: number): boolean;

    Compress(timeout: number): Promise<void>;
    Compress(timeout: number, callback: (err: Error | null, result: void) => void): void;
    CompressSync(timeout: number): boolean;

    PlcStatus(): Promise<PlcStatus>;
    PlcStatus(callback: (err: Error | null, status: PlcStatus) => void): void;
    PlcStatusSync(): PlcStatus;

    GetProtection(): Promise<Protection>;
    GetProtection(callback: (err: Error | null, data: Protection) => void): void;
    GetProtectionSync(): Protection;

    SetSessionPassword(password: string): Promise<void>;
    SetSessionPassword(password: string, callback: (err: Error | null, result: void) => void): void;
    SetSessionPasswordSync(password: string): boolean;

    ClearSessionPassword(): Promise<void>;
    ClearSessionPassword(callback: (err: Error | null, result: void) => void): void;
    ClearSessionPasswordSync(): boolean;

    // DateTime Functions
    GetPlcDateTime(): Promise<Date>;
    GetPlcDateTime(callback: (err: Error | null, date: Date) => void): void;
    GetPlcDateTimeSync(): Date;

    SetPlcDateTime(date: Date | DateTimeObject): Promise<void>;
    SetPlcDateTime(date: Date | DateTimeObject, callback: (err: Error | null, result: void) => void): void;
    SetPlcDateTimeSync(date: Date | DateTimeObject): boolean;

    SetPlcSystemDateTime(): Promise<void>;
    SetPlcSystemDateTime(callback: (err: Error | null, result: void) => void): void;
    SetPlcSystemDateTimeSync(): boolean;

    // SZL Operations
    ReadSZL(id: number, index: number): Promise<Buffer>;
    ReadSZL(id: number, index: number, callback: (err: Error | null, data: Buffer) => void): void;
    ReadSZLSync(id: number, index: number): Buffer;

    ReadSZLList(): Promise<number[]>;
    ReadSZLList(callback: (err: Error | null, data: number[]) => void): void;
    ReadSZLListSync(): number[];

    // Information
    GetCpuInfo(): Promise<CpuInfo>;
    GetCpuInfo(callback: (err: Error | null, data: CpuInfo) => void): void;
    GetCpuInfoSync(): CpuInfo;

    GetCpInfo(): Promise<CpInfo>;
    GetCpInfo(callback: (err: Error | null, data: CpInfo) => void): void;
    GetCpInfoSync(): CpInfo;

    GetOrderCode(): Promise<OrderCode>;
    GetOrderCode(callback: (err: Error | null, data: OrderCode) => void): void;
    GetOrderCodeSync(): OrderCode;

    // Block Info
    GetAgBlockInfo(blockType: BlockType, blockNumber: number): Promise<BlockInfo>;
    GetAgBlockInfo(blockType: BlockType, blockNumber: number, callback: (err: Error | null, data: BlockInfo) => void): void;
    GetAgBlockInfoSync(blockType: BlockType, blockNumber: number): BlockInfo;

    GetPgBlockInfo(buffer: Buffer): Promise<BlockInfo>;
    GetPgBlockInfo(buffer: Buffer, callback: (err: Error | null, data: BlockInfo) => void): void;
    GetPgBlockInfoSync(buffer: Buffer): BlockInfo;

    // Properties and Info
    ExecTime(): number | boolean;
    LastError(): number | boolean;
    PDURequested(): number | boolean;
    PDULength(): number | boolean;
    Connected(): boolean;
    ErrorText(code: number): string;

    // Error codes
    errNegotiatingPDU: number;
    errCliInvalidParams: number;
    errCliJobPending: number;
    errCliTooManyItems: number;
    errCliInvalidWordLen: number;
    errCliPartialDataWritten: number;
    errCliSizeOverPDU: number;
    errCliInvalidPlcAnswer: number;
    errCliAddressOutOfRange: number;
    errCliInvalidTransportSize: number;
    errCliWriteDataSizeMismatch: number;
    errCliItemNotAvailable: number;
    errCliInvalidValue: number;
    errCliCannotStartPLC: number;
    errCliAlreadyRun: number;
    errCliCannotStopPLC: number;
    errCliCannotCopyRamToRom: number;
    errCliCannotCompress: number;
    errCliAlreadyStop: number;
    errCliFunNotAvailable: number;
    errCliUploadSequenceFailed: number;
    errCliInvalidDataSizeRecvd: number;
    errCliInvalidBlockType: number;
    errCliInvalidBlockNumber: number;
    errCliInvalidBlockSize: number;
    errCliDownloadSequenceFailed: number;
    errCliInsertRefused: number;
    errCliDeleteRefused: number;
    errCliNeedPassword: number;
    errCliInvalidPassword: number;
    errCliNoPasswordToSetOrClear: number;
    errCliJobTimeout: number;
    errCliPartialDataRead: number;
    errCliBufferTooSmall: number;
    errCliFunctionRefused: number;
    errCliDestroying: number;
    errCliInvalidParamNumber: number;
    errCliCannotChangeParam: number;

    // Connection Types
    CONNTYPE_PG: number;
    CONNTYPE_OP: number;
    CONNTYPE_BASIC: number;

    // CPU Status codes
    S7CpuStatusUnknown: number;
    S7CpuStatusRun: number;
    S7CpuStatusStop: number;

    // Area IDs
    S7AreaPE: number;
    S7AreaPA: number;
    S7AreaMK: number;
    S7AreaDB: number;
    S7AreaCT: number;
    S7AreaTM: number;

    // Word Length
    S7WLBit: number;
    S7WLByte: number;
    S7WLWord: number;
    S7WLDWord: number;
    S7WLReal: number;
    S7WLCounter: number;
    S7WLTimer: number;

    // Block Types
    Block_OB: number;
    Block_DB: number;
    Block_SDB: number;
    Block_FC: number;
    Block_SFC: number;
    Block_FB: number;
    Block_SFB: number;

    // Sub Block Types
    SubBlk_OB: number;
    SubBlk_DB: number;
    SubBlk_SDB: number;
    SubBlk_FC: number;
    SubBlk_SFC: number;
    SubBlk_FB: number;
    SubBlk_SFB: number;

    // Block Languages
    BlockLangAWL: number;
    BlockLangKOP: number;
    BlockLangFUP: number;
    BlockLangSCL: number;
    BlockLangDB: number;
    BlockLangGRAPH: number;

    // Parameters
    LocalPort: number;
    RemotePort: number;
    PingTimeout: number;
    SendTimeout: number;
    RecvTimeout: number;
    WorkInterval: number;
    SrcRef: number;
    DstRef: number;
    SrcTSap: number;
    PDURequest: number;
    MaxClients: number;
    BSendTimeout: number;
    BRecvTimeout: number;
    RecoveryTime: number;
    KeepAliveTime: number;
  }

  interface S7MultiVarRead {
    Area: Area;
    WordLen: WordLen;
    DBNumber?: number | undefined;
    Start: number;
    Amount: number;
  }

  interface S7MultiVarReadResult {
    Result: number;
    Data: Buffer | null;
  }

  interface S7MultiVarWrite {
    Area: Area;
    WordLen: WordLen;
    DBNumber?: number | undefined;
    Start: number;
    Amount: number;
    Data: Buffer;
  }

  interface S7MultiVarWriteResult {
    Result: number;
  }

  interface BlocksList {
    OBCount: number;
    FBCount: number;
    FCCount: number;
    SFBCount: number;
    SFCCount: number;
    DBCount: number;
    SDBCount: number;
  }

  interface Protection {
    sch_schal: number;
    sch_par: number;
    sch_rel: number;
    bart_sch: number;
    anl_sch: number;
  }

  interface DateTimeObject {
    year: number;
    month: number;
    day: number;
    hours: number;
    minutes: number;
    seconds: number;
  }

  interface BlockInfo {
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

  interface CpInfo {
    MaxPduLength: number;
    MaxConnections: number;
    MaxMpiRate: number;
    MaxBusRate: number;
  }

  interface CpuInfo {
    ModuleTypeName: string;
    SerialNumber: string;
    ASName: string;
    Copyright: string;
    ModuleName: string;
  }

  interface OrderCode {
    Code: string;
    V1: number;
    V2: number;
    V3: number;
  }

  export enum ConnectionType {
    PG = 0x01,
    OP = 0x02,
    BASIC = 0x03
  }

  export enum PlcStatus {
    Unknown = 0x00,
    Run = 0x08,
    Stop = 0x04
  }

  export enum Area {
    PE = 0x81,
    PA = 0x82,
    MK = 0x83,
    DB = 0x84,
    CT = 0x1C,
    TM = 0x1D
  }

  export enum WordLen {
    Bit = 0x01,
    Byte = 0x02,
    Word = 0x04,
    DWord = 0x06,
    Real = 0x08,
    Counter = 0x1C,
    Timer = 0x1D
  }

  export enum BlockType {
    OB = 0x38,
    DB = 0x41,
    SDB = 0x42,
    FC = 0x43,
    SFC = 0x44,
    FB = 0x45,
    SFB = 0x46
  }

  export enum SubBlockType {
    OB = 0x08,
    DB = 0x0A,
    SDB = 0x0B,
    FC = 0x0C,
    SFC = 0x0D,
    FB = 0x0E,
    SFB = 0x0F
  }

  export enum BlockLang {
    AWL = 0x01,
    KOP = 0x02,
    FUP = 0x03,
    SCL = 0x04,
    DB = 0x05,
    GRAPH = 0x06
  }

  export enum Parameter {
    LocalPort = 1,
    RemotePort = 2,
    PingTimeout = 3,
    SendTimeout = 4,
    RecvTimeout = 5,
    WorkInterval = 6,
    SrcRef = 7,
    DstRef = 8,
    SrcTSap = 9,
    PDURequest = 10,
    MaxClients = 11,
    BSendTimeout = 12,
    BRecvTimeout = 13,
    RecoveryTime = 14,
    KeepAliveTime = 15
  }

  export enum ClientError {
    NegotiatingPDU = 0x0001,
    InvalidParams = 0x0002,
    JobPending = 0x0003,
    TooManyItems = 0x0004,
    InvalidWordLen = 0x0005,
    PartialDataWritten = 0x0006,
    SizeOverPDU = 0x0007,
    InvalidPlcAnswer = 0x0008,
    AddressOutOfRange = 0x0009,
    InvalidTransportSize = 0x000A,
    WriteDataSizeMismatch = 0x000B,
    ItemNotAvailable = 0x000C,
    InvalidValue = 0x000D,
    CannotStartPLC = 0x000E,
    AlreadyRun = 0x000F,
    CannotStopPLC = 0x0010,
    CannotCopyRamToRom = 0x0011,
    CannotCompress = 0x0012,
    AlreadyStop = 0x0013,
    FunNotAvailable = 0x0014,
    UploadSequenceFailed = 0x0015,
    InvalidDataSizeRecvd = 0x0016,
    InvalidBlockType = 0x0017,
    InvalidBlockNumber = 0x0018,
    InvalidBlockSize = 0x0019,
    DownloadSequenceFailed = 0x001A,
    InsertRefused = 0x001B,
    DeleteRefused = 0x001C,
    NeedPassword = 0x001D,
    InvalidPassword = 0x001E,
    NoPasswordToSetOrClear = 0x001F,
    JobTimeout = 0x0020,
    PartialDataRead = 0x0021,
    BufferTooSmall = 0x0022,
    FunctionRefused = 0x0023,
    Destroying = 0x0024,
    InvalidParamNumber = 0x0025,
    CannotChangeParam = 0x0026
  }

  export class S7Client {
    constructor();
  }

  export interface S7Server extends EventEmitter {
    // Administrative Methods
    Start(): Promise<boolean>;
    StartTo(ip: string): Promise<boolean>;
    Stop(): Promise<boolean>;
    GetParam(paramNumber: number): number;
    SetParam(paramNumber: number, value: number): boolean;
    SetResourceless(value: boolean): boolean;

    // Memory Methods
    RegisterArea(areaCode: number, index: number, buffer: Buffer): boolean;
    UnregisterArea(areaCode: number, index: number): boolean;
    GetArea(areaCode: number, index: number): Buffer;
    SetArea(areaCode: number, index: number, buffer: Buffer): boolean;
    LockArea(areaCode: number, index: number): boolean;
    UnlockArea(areaCode: number, index: number): boolean;

    // Event Methods
    GetEventsMask(): number;
    SetEventsMask(mask: number): void;

    // Miscellaneous Methods
    LastError(): number;
    EventText(evt: SrvEvent): string;
    ErrorText(errNum: number): string;
    ServerStatus(): number;
    ClientsCount(): number;
    GetCpuStatus(): number;
    SetCpuStatus(status: number): boolean;

    // Constants
    srvAreaPE: number;
    srvAreaPA: number;
    srvAreaMK: number;
    srvAreaCT: number;
    srvAreaTM: number;
    srvAreaDB: number;

    // Event Constants  
    evcServerStarted: number;
    evcServerStopped: number;
    evcClientAdded: number;
    evcClientRejected: number;
    evcClientDisconnected: number;
    evcPDUincoming: number;
    evcDataRead: number;
    evcDataWrite: number;

    // Operation Constants
    operationRead: number;
    operationWrite: number;

    // Status Constants
    SrvStopped: number;
    SrvRunning: number;
    SrvError: number;

    // Events
    on(event: 'event', listener: (event: SrvEvent) => void): this;
    on(event: 'readWrite', listener: (sender: string, operation: number, tag: S7Tag, buffer: Buffer, callback: Function) => void): this;
  }

  interface SrvEvent {
    EvtTime: Date;
    EvtSender: string;
    EvtCode: number;
    EvtRetCode: number;
    EvtParam1?: number;
    EvtParam2?: number;
    EvtParam3?: number;
    EvtParam4?: number;
  }

  interface S7Tag {
    Area: number;
    DBNumber: number;
    Start: number;
    Size: number;
    WordLen: number;
  }

  export class S7Server extends EventEmitter {
    constructor();
  }
}