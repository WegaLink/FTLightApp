//////////////////////////////////////////////////////////////////////////////
//
// CmParallel.ixx - Module for CmParallel classes
//
//////////////////////////////////////////////////////////////////////////////
//
// author: 	   Eckhard Kantz
// website:    http://wegalink.eu
//
//////////////////////////////////////////////////////////////////////////////
/*
This is FREE software

Permission is hereby granted, free of charge,  to any person obtaining  a copy
of this software and associated documentation files (the "Software"),  to deal
in the Software without restriction, including without limitation  the  rights
to use,  copy,  modify,  merge,  publish,  distribute, sublicense, and/or sell
copies  of  the  Software,   and  to  permit  persons  to  whom  the  Software
is furnished to do so, subject to the following conditions:

There are no conditions imposed on the use of this software.

THE SOFTWARE IS PROVIDED "AS IS",  WITHOUT  WARRANTY  OF ANY KIND,  EXPRESS OR
IMPLIED,  INCLUDING  BUT  NOT  LIMITED  TO  THE  WARRANTIES OF MERCHANTABILITY,
FITNESS  FOR  A  PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS  OR  COPYRIGHT  HOLDERS  BE  LIABLE  FOR  ANY CLAIM,  DAMAGES OR OTHER
LIABILITY,  WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,  ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN  THE
SOFTWARE.
*/

module;

#include "Services/Basic/Modules/CmTypes.h"

export module CmBasic:CmParallel;

export import :CmDateTime;

export namespace Cosmos
{
	// forward declaration
	class CmString;
	class CmStringFTL;

	/** CmLogLevel.
	*  The logging will be controlled by the following log levels
	*/
	enum CmLogLevel {
		CMLOG_None = 0,			// logging disabled
		CMLOG_Error = 1,		// errors (=default)
		CMLOG_TestMode = 2,		// communication test
		CMLOG_QuickView = 3,	// quick view
		CMLOG_Info = 4,			// info
		CMLOG_Control = 5,		// control/response
		CMLOG_Request = 6,		// request/dataset
		CMLOG_Process = 7,		// processing data
		CMLOG_Network = 8,		// network maintenance
		CMLOG_Connection = 9,	// connection details
		CMLOG_PlugNodes = 10,	// PlugNodes operation
		CMLOG_Resource = 11,	// resource management
	};

	class CmParallel
	{
	public:
		/** CmParallel.
		*  A member function will run a specified number of times in background.
		*/
		CmParallel();
		virtual ~CmParallel();

	public:
		/** startParallel/Timer.
		*  A thread will be started which calls a static thread function.
		*/
		void startParallel(int64 _LoopCount = 0);
		void startWatchdog(int64 _LoopCount = 0);

	protected:
		/** StaticThreadStart.
		*  A static function which will be started in a thread.
		*/
		static DWORD WINAPI StaticThreadParallel(void* _Param);
		static DWORD WINAPI StaticThreadWatchdog(void* _Param);

	protected:
		/** runParallel/Watchdog.
		*  Those functions will be executed in parallel to other program functionality.
		*/
		virtual bool runParallel();
		virtual bool runWatchdog();

	public:
		/** isRunningParallel/Watchdog.
		*  Returns true if a thread was started and is still running.
		*/
		bool isRunningParallel();
		bool isRunningWatchdog();

	public:
		/** stopParallel/Watchdog.
		*  Stop parallel execution of this class' program functionality gracefully.
		*  After the timeout has elapsed the thread will be unconditionally removed.
		*/
		bool stopParallel(int32 _Timeout_ms = 400, int32 _WaitStep = 20);
		bool stopWatchdog(int32 _Timeout_ms = 400, int32 _WaitStep = 20);

	public:
		/** enter/leave/trySerialize/isLocked.
		*  A critical section will be entered as soon as it is not used by others,
		*  respectively it will be left and thus be enabled again for access by others.
		*  The leave function will return the critical section's recursion counter,
		*  respectively it returns a success indicator.
		*/
		virtual int32 enterSerialize(int32 _LockID = 0);
		virtual int32 leaveSerialize();
		virtual bool leaveSerialize(bool _Success);
		bool trySerialize(int32 _LockID = 0);
		bool isLocked();
		// watchdog
		virtual int32 enterSerializeWatchdog(int32 _LockID = 0);
		virtual int32 leaveSerializeWatchdog();
		bool trySerializeWatchdog(int32 _LockID = 0);
		bool isLockedWatchdog();

		//--------logging-functionality---------------------------------------------

	public:
		/** setLogLevel.
		*  The global MaxLogLevel will be set.
		*/
		static void setLogLevel(CmLogLevel _LogLevel);

	public:
		/** log.
		*   A message will be added to a Logger (CmStringFTL) if the LogLevel is less
		*   or equal to the current maximal LogLevel .
		*   An  optional Context can be used for a context-sensitive logging.
		*/
		bool log(const CmString& _Message, CmLogLevel _LogLevel = CMLOG_Error, const CmString* _Context = NULL);

	public:
		/** writeLog.
		*  The content of the Logger (CmStringFTL) will be written to specified file.
		*  The IFTL is assigned as a top level to the FTLight-style log file.
		*/
		bool writeLog(CmString _LogFile, CmString _IFTL, CmString _FilePath = "log\\");

		//--------workspace---------------------------------------------------------

	protected:
		// Thread parallel
		DWORD ParallelThreadID;
		int64 ParallelLoopCount;
		bool isParallelThreadShutdown;
		bool isParallelThreadFinished;
		bool isParallelThreadPending;
	public:
		HANDLE ParallelThreadHandle;

	private:
		// serialize
		CRITICAL_SECTION	ParallelCriticalSection;
		int32 ParallelLockID;
		int32 ParallelLastLockID;
		CmTimestamp ParallelTimestamp;
		int32 ParallelLastLockTime_ms;

	protected:
		// Thread watchdog
		DWORD WatchdogThreadID;
		int64 WatchdogLoopCount;
		bool isWatchdogThreadShutdown;
		bool isWatchdogThreadFinished;
		bool isWatchdogThreadPending;
	public:
		HANDLE WatchdogThreadHandle;

	private:
		// serialize
		CRITICAL_SECTION	WatchdogCriticalSection;
		int32 WatchdogLockID;
		int32 WatchdogLastLockID;
		CmTimestamp WatchdogTimestamp;
		int32 WatchdogLastLockTime_ms;

	};

}  // namespace Cosmos

using namespace Cosmos;


// ----------------------------------------------------------------------------
// CmParallel class
//----------------------------------------------------------------------------
//

// static CmStringFTL class for logging purposes
static CmStringFTL * Logger = NULL;
static CmLogLevel MaxLogLevel = CMLOG_Error;
static CmParallel LogAccess;

CmParallel::CmParallel()
{
	// initialize workspace
	ParallelThreadHandle = 0;
	ParallelThreadID = 0;
	isParallelThreadShutdown = false;
	InitializeCriticalSection(&ParallelCriticalSection);

	// initialize workspace timer
	WatchdogThreadHandle = 0;
	WatchdogThreadID = 0;
	isWatchdogThreadShutdown = false;
	InitializeCriticalSection(&WatchdogCriticalSection);
}
CmParallel::~CmParallel()
{
	// stop thread functions should they still be running
	stopParallel();
	stopWatchdog();

	// release critical section resources
	DeleteCriticalSection(&ParallelCriticalSection);
	DeleteCriticalSection(&WatchdogCriticalSection);
}
void CmParallel::startParallel(int64 _LoopCount)
{
	// check if Parallel is already running
	if (isRunningParallel()) return;

	// store loop count
	ParallelLoopCount = _LoopCount;

	// start worker thread
	ParallelLockID = 0;
	isParallelThreadShutdown = false;
	LPSECURITY_ATTRIBUTES ThreadAtributes = NULL;
	SIZE_T StackSize = 0;
	LPVOID Context = this;
	DWORD CreationFlag = 0;
	ParallelThreadHandle = CreateThread(ThreadAtributes, StackSize, StaticThreadParallel, Context, CreationFlag, &ParallelThreadID);
	LOG3("ID=", CmString::UInt2Hex((uint64)ParallelThreadHandle), " ParallelThreadHandle opened", Msg1, CMLOG_Resource)
}
void CmParallel::startWatchdog(int64 _LoopCount)
{
	// check if Watchdog is already running
	if (isRunningWatchdog()) return;

	// store loop count
	WatchdogLoopCount = _LoopCount;

	// start worker thread
	WatchdogLockID = 0;
	isWatchdogThreadShutdown = false;
	LPSECURITY_ATTRIBUTES ThreadAtributes = NULL;
	SIZE_T StackSize = 0;
	LPVOID Context = this;
	DWORD CreationFlag = 0;
	WatchdogThreadHandle = CreateThread(ThreadAtributes, StackSize, StaticThreadWatchdog, Context, CreationFlag, &WatchdogThreadID);
	LOG3("ID=", CmString::UInt2Hex((uint64)WatchdogThreadHandle), " WatchdogThreadHandle opened", Msg1, CMLOG_Resource)
}
DWORD WINAPI CmParallel::StaticThreadParallel(void* _Param)
{
	// restore object pointer
	CmParallel* This = (CmParallel*)_Param;

	// run background function continuously or for a specified number of times
	This->isParallelThreadFinished = false;
	This->isParallelThreadPending = false;
	// exit with 0 if succeeded, otherwise with 1
	if (This->ParallelLoopCount <= 0) {
		for (;;) {
			if (This->isParallelThreadShutdown) break;
			if (false == This->isRunningParallel()) break;
			if (false == This->runParallel()) {
				This->isParallelThreadPending = true;
				return 1;
			}
		}
	}
	else {
		for (int i = 0; i < This->ParallelLoopCount; i++) {
			if (This->isParallelThreadShutdown) break;
			if (false == This->isRunningParallel())	break;
			if (false == This->runParallel()) {
				This->isParallelThreadPending = true;
				return 1;
			}
		}
	}

	// thread has finished successfully
	This->isParallelThreadFinished = true;
	return 0;
}
DWORD WINAPI CmParallel::StaticThreadWatchdog(void* _Param)
{
	// restore object pointer
	CmParallel* This = (CmParallel*)_Param;

	// run background function continuously or for a specified number of times
	This->isWatchdogThreadFinished = false;
	This->isWatchdogThreadPending = false;
	// exit with 0 if succeeded, otherwise with 1
	if (This->WatchdogLoopCount <= 0) {
		for (;;) {
			if (This->isWatchdogThreadShutdown) break;
			if (false == This->isRunningWatchdog()) break;
			if (false == This->runWatchdog()) {
				This->isWatchdogThreadPending = true;
				return 1;
			}
		}
	}
	else {
		for (int i = 0; i < This->WatchdogLoopCount; i++) {
			if (This->isWatchdogThreadShutdown) break;
			if (false == This->isRunningWatchdog())	break;
			if (false == This->runWatchdog()) {
				This->isWatchdogThreadPending = true;
				return 1;
			}
		}
	}

	// thread has finished successfully
	This->isWatchdogThreadFinished = true;
	return 0;
}
bool CmParallel::runParallel()
{
	Sleep(40);	// ~25fps
	return true;
}
bool CmParallel::runWatchdog()
{
	Sleep(100);	// ~10fps
	return true;
}
bool CmParallel::isRunningParallel()
{
	// thread is not running if its thread handle is 0
	if (0 == ParallelThreadHandle) return false;

	// thread is not running if the OS does not report 'STILL_ACTIVE'
	DWORD ExitCode = 0;
	GetExitCodeThread(ParallelThreadHandle, &ExitCode);
	if (ExitCode != STILL_ACTIVE)	return false;

	// deactivated for runtime reasons, activate only if needed
	//LOG3("ID=", CmString::UInt2Hex(uint64(ThreadHandle)), " ThreadHandle still running", Msg1, CMLOG_Resource)

	// thread is not running if its thread function has been left
	if (isParallelThreadFinished || isParallelThreadPending)
		return false;

	return true;
}
bool CmParallel::isRunningWatchdog()
{
	// thread is not running if its thread handle is 0
	if (0 == WatchdogThreadHandle) return false;

	// thread is not running if the OS does not report 'STILL_ACTIVE'
	DWORD ExitCode = 0;
	GetExitCodeThread(WatchdogThreadHandle, &ExitCode);
	if (ExitCode != STILL_ACTIVE)	return false;

	// deactivated for runtime reasons, activate only if needed
	//LOG3("ID=", CmString::UInt2Hex(uint64(WatchdogThreadHandle)), " WatchdogThreadHandle still running", Msg1, CMLOG_Resource)

	// thread is not running if its thread function has been left
	if (isWatchdogThreadFinished || isWatchdogThreadPending)
		return false;

	return true;
}
bool CmParallel::stopParallel(int32 _Timeout_ms, int32 _WaitStep)
{
	bool hasShutdownGracefully = true;

	// check if the thread is down
	if (0 == ParallelThreadHandle) return true;

	// wait until thread went down if it is not pending already
	isParallelThreadShutdown = true;
	if (false == isParallelThreadPending) {
		for (int32 t = 0; t < _Timeout_ms; t += _WaitStep) {
			// ToDo: fix blocking that occurs sporadically
			Sleep(_WaitStep);
			if (isParallelThreadFinished)
				break;
		}
	}

	// check if thread is still running
	if (isRunningParallel()) {
		hasShutdownGracefully = false;
		LOG3("ID=", CmString::UInt2Hex((uint64)ParallelThreadHandle), " ParallelThread did not go down", Msg1, CMLOG_Error)
	}
	else {
		// close thread handle
		CloseHandle(ParallelThreadHandle);
		LOG3("ID=", CmString::UInt2Hex((uint64)ParallelThreadHandle), " ParallelThreadHandle closed", Msg1, CMLOG_Resource)

			//clear thread resources
			ParallelThreadHandle = 0;
		ParallelThreadID = 0;
	}

	return hasShutdownGracefully;
}
bool CmParallel::stopWatchdog(int32 _Timeout_ms, int32 _WaitStep)
{
	bool hasShutdownGracefully = true;

	// check if the thread is down
	if (0 == WatchdogThreadHandle) return true;

	// wait until thread went down if it is not pending already
	isWatchdogThreadShutdown = true;
	if (false == isWatchdogThreadPending) {
		for (int32 t = 0; t < _Timeout_ms; t += _WaitStep) {
			// ToDo: fix blocking that occurs sporadically
			Sleep(_WaitStep);
			if (isWatchdogThreadFinished)
				break;
		}
	}

	// check if thread is still running
	if (isRunningWatchdog()) {
		hasShutdownGracefully = false;
		LOG3("ID=", CmString::UInt2Hex((uint64)WatchdogThreadHandle), " WatchdogThread did not go down", Msg1, CMLOG_Error)
	}
	else {
		// close thread handle
		CloseHandle(WatchdogThreadHandle);
		LOG3("ID=", CmString::UInt2Hex((uint64)WatchdogThreadHandle), " WatchdogThreadHandle closed", Msg1, CMLOG_Resource)

			//clear thread resources
			WatchdogThreadHandle = 0;
		WatchdogThreadID = 0;
	}

	return hasShutdownGracefully;
}
int32 CmParallel::enterSerialize(int32 _LockID)
{
	// wait for access to a critical section
	EnterCriticalSection(&ParallelCriticalSection);

	// remember first active lock
	ParallelCriticalSection.RecursionCount == 1 ? ParallelLockID = _LockID : 0;

	return ParallelCriticalSection.RecursionCount;
}
int32 CmParallel::enterSerializeWatchdog(int32 _LockID)
{
	// wait for access to a critical section
	EnterCriticalSection(&WatchdogCriticalSection);

	// remember first active lock
	WatchdogCriticalSection.RecursionCount == 1 ? WatchdogLockID = _LockID : 0;

	return WatchdogCriticalSection.RecursionCount;
}
int32 CmParallel::leaveSerialize()
{
	// clear for last lock 
	ParallelCriticalSection.RecursionCount == 1 ? ParallelLockID = 0 : 0;

	// leave a critical section
	LeaveCriticalSection(&ParallelCriticalSection);

	return ParallelCriticalSection.RecursionCount;
}
bool CmParallel::leaveSerialize(bool _Success)
{
	// clear for last lock 
	ParallelCriticalSection.RecursionCount == 1 ? ParallelLockID = 0 : 0;

	// leave a critical section
	LeaveCriticalSection(&ParallelCriticalSection);

	return _Success;
}
int32 CmParallel::leaveSerializeWatchdog()
{
	// clear for last lock
	WatchdogCriticalSection.RecursionCount == 1 ? WatchdogLockID = 0 : 0;

	// leave a critical section
	LeaveCriticalSection(&WatchdogCriticalSection);

	return WatchdogCriticalSection.RecursionCount;
}
bool CmParallel::trySerialize(int32 _LockID)
{
	// wait for access to a critical section
	if (false == TryEnterCriticalSection(&ParallelCriticalSection)) return false;

	// remember first active lock
	ParallelCriticalSection.RecursionCount == 1 ? ParallelLockID = _LockID : 0;

	return true;
}
bool CmParallel::trySerializeWatchdog(int32 _LockID)
{
	// wait for access to a critical section
	if (false == TryEnterCriticalSection(&WatchdogCriticalSection)) return false;

	// remember first active lock
	WatchdogCriticalSection.RecursionCount == 1 ? WatchdogLockID = _LockID : 0;

	return true;
}
bool CmParallel::isLocked() {
	return ParallelCriticalSection.RecursionCount == 0 ? false : true;
}
bool CmParallel::isLockedWatchdog() {
	return WatchdogCriticalSection.RecursionCount == 0 ? false : true;
}

// ToDo:	 
//--------logging-functionality-----------------------------------------------
void CmParallel::setLogLevel(CmLogLevel _LogLevel)
{
	MaxLogLevel = _LogLevel;
}
bool CmParallel::log(const CmString& _Message, CmLogLevel _LogLevel, const CmString* _Context)
{
	//// reject non-relevant logging
	//if (_LogLevel > MaxLogLevel) return false;

	//// create a logger if it does not exist yet
	//CmStringFTL* Location = NULL;
	//CmStringFTL* IFTL = NULL;
	//CmStringFTL* Created = NULL;
	//CmStringFTL* LogLevel = NULL;
	//CmString TimestampCreated;
	//if (NULL == Logger) {
	//	LogAccess.enterSerialize();

	//	// create a new logger
	//	Logger = CmString::allocateMemory<CmStringFTL>(1, isCmStringFTL);
	//	NULL != Logger ? Logger->addChild(&Location) : 0;
	//	NULL != Location ? Location->addChild(&IFTL) : 0;
	//	NULL != IFTL ? IFTL->addChild(&Created) : 0;
	//	NULL != Created ? Created->addChild(&LogLevel) : 0;
	//	// timestamp created
	//	TimestampCreated = CmDateTime::getTimeUTC(0, -1, true); // true= timestamp without colon
	//	if (NULL != Created) { Created->setText(TimestampCreated); }
	//	// log level
	//	CmString MsgLogLevel;
	//	switch (MaxLogLevel) {
	//	case CMLOG_None: MsgLogLevel = "CMLOG_None"; break;
	//	case CMLOG_Error: MsgLogLevel = "CMLOG_Error"; break;
	//	case CMLOG_Info: MsgLogLevel = "CMLOG_Info"; break;
	//	case CMLOG_Control: MsgLogLevel = "CMLOG_Control"; break;
	//	case CMLOG_Process: MsgLogLevel = "CMLOG_Process"; break;
	//	case CMLOG_Network: MsgLogLevel = "CMLOG_Network"; break;
	//	case CMLOG_Connection: MsgLogLevel = "CMLOG_Connection"; break;
	//	case CMLOG_PlugNodes: MsgLogLevel = "CMLOG_PlugNodes"; break;
	//	case CMLOG_Resource: MsgLogLevel = "CMLOG_Resource"; break;
	//	default: MsgLogLevel = "CMLOG?"; break;
	//	}
	//	if (NULL != LogLevel) { LogLevel->setText(MsgLogLevel); }

	//	LogAccess.leaveSerialize();
	//}
	//NULL != Logger ? Location = Logger->getChild() : 0;
	//NULL != Location ? IFTL = Location->getChild() : 0;
	//NULL != IFTL ? Created = IFTL->getChild() : 0;
	//NULL != Created ? LogLevel = IFTL->getChild() : 0;
	//if (NULL == LogLevel) return false;

	//// timestamp
	//int64 SysClockNanoSec = CmDateTime::getSysClockNanoSec(false); // get absolute time
	//CmString MsgTimestamp = CmDateTime::getTimestamp(SysClockNanoSec, 0, true, true);

	//LogAccess.enterSerialize();

	//// insert new message into logger
	//CmStringFTL* Timestamp;
	//CmStringFTL* Message;
	////CmStringFTL *LogLevel;
	//CmStringFTL* Context;
	//if (false == IFTL->addChild(&Timestamp)) return false;
	//if (false == Timestamp->addChild(&Message)) return false;
	////if (false == Message->addChild(&LogLevel)) return false;
	//if (false == Message->addChild(&Context)) return false;

	//// assign values
	//Timestamp->setText(MsgTimestamp);
	//Message->setText(_Message);
	////LogLevel->setText(MsgLogLevel);
	//if (NULL != _Context) { Context->setText(*_Context); }

	//LogAccess.leaveSerialize();

	return true;
}
bool CmParallel::writeLog(CmString _LogFile, CmString _IFTL, CmString _FilePath)
{
	//// check Logger availability
	//CmStringFTL* Location = NULL;
	//CmStringFTL* IFTL = NULL;
	//NULL != Logger ? Location = Logger->getChild() : 0;
	//NULL != Location ? IFTL = Location->getChild() : 0;
	//if (NULL == IFTL) return false;

	//// check file path
	//if (false == PathFileExistsA(_FilePath.getText())) return false;

	//// assign a IFTL
	//IFTL->setText(_IFTL);

	//// convert Logger to a string
	//CmStringFTL LogFTL;
	//Logger->serializeStringFTL(LogFTL);
	//// decode escaped characters
	//LogFTL.decodeTXL();

	//// release logger
	//CmString::releaseMemory<CmStringFTL>(Logger, 1, isCmStringFTL);
	//Logger = NULL;

	//// add a timestamp prefix to the logfile name
	//// ToDo: insert the logfile into a FTLight repository
	//CmString Timestamp = CmDateTime::getTimeUTC(0, -1, true); // true= timestamp for filename
	//Timestamp += "_";
	//Timestamp += _LogFile;
	//_LogFile = _FilePath;
	//if (_LogFile[((int32)_LogFile.getLength()) - 1] != '\\') { _LogFile += "\\"; }
	//_LogFile += Timestamp;

	//// write logfile to disk
	//return LogFTL.writeBinary(_LogFile.getText());

	return true;
}
