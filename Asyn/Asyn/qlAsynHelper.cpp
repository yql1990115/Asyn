#include "stdafx.h"
#include "qlAsynHelper.h"
#include <vector>
#include <string>
#include <WinBase.h>

namespace asyn {

namespace
{
	class CWaitableEventPool
	{
	private:

		// multi-threading sync.

		CCriticalSection mutex;

		// allocated, currently usused handles

		std::vector<HANDLE> handles;

		// flag indicating that this instance has not been destroyed, yet

		volatile unsigned long alive;

		// construction / destruction:
		// free all handles upon destruction at latest

		CWaitableEventPool();
		~CWaitableEventPool();

		// prevent cloning
		CWaitableEventPool(const CWaitableEventPool&) = delete;
		CWaitableEventPool& operator=(const CWaitableEventPool&) = delete;

	public:

		// Meyer's singleton

		static CWaitableEventPool* GetInstance();

		// recycling interface:
		// alloc at request

		HANDLE Alloc();
		void AutoAlloc(HANDLE& handle);
		void Release(HANDLE event);
		void Clear();
	};

	// construction / destruction:

	CWaitableEventPool::CWaitableEventPool()
		: alive(TRUE)
	{
	}

	CWaitableEventPool::~CWaitableEventPool()
	{
		Clear();
		InterlockedExchange(&alive, FALSE);
	}

	// Meyer's singleton

	CWaitableEventPool* CWaitableEventPool::GetInstance()
	{
		static CWaitableEventPool instance;
		return &instance;
	}

	// recycling interface:
	// alloc at request

	HANDLE CWaitableEventPool::Alloc()
	{
		if (InterlockedCompareExchange(&alive, TRUE, TRUE))
		{
			CCriticalSectionLock lock(mutex);
			if (!handles.empty())
			{
				HANDLE result = handles.back();
				handles.pop_back();
				return result;
			}
		}

		return CreateEvent(NULL, TRUE, FALSE, NULL);
	}

	void CWaitableEventPool::AutoAlloc(HANDLE& handle)
	{
		if (handle != NULL)
			return;

		if (InterlockedCompareExchange(&alive, TRUE, TRUE))
		{
			CCriticalSectionLock lock(mutex);
			if (!handles.empty())
			{
				handle = handles.back();
				handles.pop_back();
				return;
			}
		}

		handle = CreateEvent(NULL, TRUE, FALSE, NULL);
	}

	void CWaitableEventPool::Release(HANDLE event)
	{
		ResetEvent(event);
		if (InterlockedCompareExchange(&alive, TRUE, TRUE))
		{
			CCriticalSectionLock lock(mutex);
			handles.push_back(event);
		}
		else
		{
			CloseHandle(event);
		}
	}

	void CWaitableEventPool::Clear()
	{
		CCriticalSectionLock lock(mutex);

		while (!handles.empty())
			CloseHandle(Alloc());
	}
}

// construction / destruction: manage event handle

COneShotEvent::COneShotEvent()
	: event(NULL)
	, state(FALSE)
{
}

COneShotEvent::~COneShotEvent()
{
	if (event != NULL)
		CWaitableEventPool::GetInstance()->Release(event);
}

// eventing interface

void COneShotEvent::Set()
{
	if (InterlockedExchange(&state, TRUE) == FALSE)
		if (event != NULL)
			SetEvent(event);
}

bool COneShotEvent::Test() const
{
	return state == TRUE;
}

void COneShotEvent::WaitFor()
{
	if (state == FALSE)
	{
		CWaitableEventPool::GetInstance()->AutoAlloc(event);
		if (state == FALSE)
			WaitForSingleObject(event, INFINITE);
	}
}

bool COneShotEvent::WaitForEndOrTimeout(DWORD milliSeconds)
{
	if (state == FALSE)
	{
		CWaitableEventPool::GetInstance()->AutoAlloc(event);
		return WaitForSingleObject(event, milliSeconds) == WAIT_OBJECT_0;
	}

	return true;
}

// construction / destruction: manage event handle

CWaitableEvent::CWaitableEvent()
	: event(CWaitableEventPool::GetInstance()->Alloc())
{
}

CWaitableEvent::~CWaitableEvent()
{
	CWaitableEventPool::GetInstance()->Release(event);
}

// eventing interface

void CWaitableEvent::Set()
{
	SetEvent(event);
}

void CWaitableEvent::Reset()
{
	ResetEvent(event);
}

bool CWaitableEvent::Test() const
{
	return WaitForSingleObject(event, 0) == WAIT_OBJECT_0;
}

void CWaitableEvent::WaitFor()
{
	WaitForSingleObject(event, INFINITE);
}

bool CWaitableEvent::WaitForEndOrTimeout(DWORD milliSeconds)
{
	return WaitForSingleObject(event, milliSeconds) == WAIT_OBJECT_0;
}

}


long WINAPI qlGenerateMiniDump(PEXCEPTION_POINTERS pExceptionInfo, HANDLE hFile, const char* strDumpName)
{
	typedef enum _MINIDUMP_TYPE {
		MiniDumpNormal = 0x00000000,
		MiniDumpWithDataSegs = 0x00000001,
		MiniDumpWithFullMemory = 0x00000002,
		MiniDumpWithHandleData = 0x00000004,
		MiniDumpFilterMemory = 0x00000008,
		MiniDumpScanMemory = 0x00000010,
		MiniDumpWithUnloadedModules = 0x00000020,
		MiniDumpWithIndirectlyReferencedMemory = 0x00000040,
		MiniDumpFilterModulePaths = 0x00000080,
		MiniDumpWithProcessThreadData = 0x00000100,
		MiniDumpWithPrivateReadWriteMemory = 0x00000200,
		MiniDumpWithoutOptionalData = 0x00000400,
		MiniDumpWithFullMemoryInfo = 0x00000800,
		MiniDumpWithThreadInfo = 0x00001000,
		MiniDumpWithCodeSegs = 0x00002000,
		MiniDumpWithoutAuxiliaryState = 0x00004000,
		MiniDumpWithFullAuxiliaryState = 0x00008000,
		MiniDumpWithPrivateWriteCopyMemory = 0x00010000,
		MiniDumpIgnoreInaccessibleMemory = 0x00020000,
		MiniDumpWithTokenInformation = 0x00040000,
		MiniDumpValidTypeFlags = 0x0007ffff,
	} MINIDUMP_TYPE;
	//
	typedef struct _MINIDUMP_EXCEPTION_INFORMATION {
		DWORD ThreadId;
		PEXCEPTION_POINTERS ExceptionPointers;
		BOOL ClientPointers;
	} MINIDUMP_EXCEPTION_INFORMATION, *PMINIDUMP_EXCEPTION_INFORMATION;
	//
	typedef struct _MINIDUMP_USER_STREAM {
		ULONG32 Type;
		ULONG BufferSize;
		PVOID Buffer;

	} MINIDUMP_USER_STREAM, *PMINIDUMP_USER_STREAM;
	//
	typedef struct _MINIDUMP_USER_STREAM_INFORMATION {
		ULONG UserStreamCount;
		PMINIDUMP_USER_STREAM UserStreamArray;
	} MINIDUMP_USER_STREAM_INFORMATION, *PMINIDUMP_USER_STREAM_INFORMATION;
	//
	typedef struct _MINIDUMP_THREAD_CALLBACK {
		ULONG ThreadId;
		HANDLE ThreadHandle;
		CONTEXT Context;
		ULONG SizeOfContext;
		ULONG64 StackBase;
		ULONG64 StackEnd;
	} MINIDUMP_THREAD_CALLBACK, *PMINIDUMP_THREAD_CALLBACK;
	//	
	typedef struct _MINIDUMP_THREAD_EX_CALLBACK {
		ULONG ThreadId;
		HANDLE ThreadHandle;
		CONTEXT Context;
		ULONG SizeOfContext;
		ULONG64 StackBase;
		ULONG64 StackEnd;
		ULONG64 BackingStoreBase;
		ULONG64 BackingStoreEnd;
	} MINIDUMP_THREAD_EX_CALLBACK, *PMINIDUMP_THREAD_EX_CALLBACK;
	//
	typedef struct _MINIDUMP_MODULE_CALLBACK {
		PWCHAR FullPath;
		ULONG64 BaseOfImage;
		ULONG SizeOfImage;
		ULONG CheckSum;
		ULONG TimeDateStamp;
		VS_FIXEDFILEINFO VersionInfo;
		PVOID CvRecord;
		ULONG SizeOfCvRecord;
		PVOID MiscRecord;
		ULONG SizeOfMiscRecord;
	} MINIDUMP_MODULE_CALLBACK, *PMINIDUMP_MODULE_CALLBACK;
	//
	typedef struct _MINIDUMP_INCLUDE_THREAD_CALLBACK {
		ULONG ThreadId;
	} MINIDUMP_INCLUDE_THREAD_CALLBACK, *PMINIDUMP_INCLUDE_THREAD_CALLBACK;
	//
	typedef struct _MINIDUMP_INCLUDE_MODULE_CALLBACK {
		ULONG64 BaseOfImage;
	} MINIDUMP_INCLUDE_MODULE_CALLBACK, *PMINIDUMP_INCLUDE_MODULE_CALLBACK;
	//
	typedef struct _MINIDUMP_IO_CALLBACK {
		HANDLE Handle;
		ULONG64 Offset;
		PVOID Buffer;
		ULONG BufferBytes;
	} MINIDUMP_IO_CALLBACK, *PMINIDUMP_IO_CALLBACK;
	//
	typedef struct _MINIDUMP_READ_MEMORY_FAILURE_CALLBACK
	{
		ULONG64 Offset;
		ULONG Bytes;
		HRESULT FailureStatus;
	} MINIDUMP_READ_MEMORY_FAILURE_CALLBACK, *PMINIDUMP_READ_MEMORY_FAILURE_CALLBACK;
	//
	typedef struct _MINIDUMP_CALLBACK_INPUT {
		ULONG ProcessId;
		HANDLE ProcessHandle;
		ULONG CallbackType;
		union {
			HRESULT Status;
			MINIDUMP_THREAD_CALLBACK Thread;
			MINIDUMP_THREAD_EX_CALLBACK ThreadEx;
			MINIDUMP_MODULE_CALLBACK Module;
			MINIDUMP_INCLUDE_THREAD_CALLBACK IncludeThread;
			MINIDUMP_INCLUDE_MODULE_CALLBACK IncludeModule;
			MINIDUMP_IO_CALLBACK Io;
			MINIDUMP_READ_MEMORY_FAILURE_CALLBACK ReadMemoryFailure;
			ULONG SecondaryFlags;
		};
	} MINIDUMP_CALLBACK_INPUT, *PMINIDUMP_CALLBACK_INPUT;
	//
	typedef struct _MINIDUMP_MEMORY_INFO {
		ULONG64 BaseAddress;
		ULONG64 AllocationBase;
		ULONG32 AllocationProtect;
		ULONG32 __alignment1;
		ULONG64 RegionSize;
		ULONG32 State;
		ULONG32 Protect;
		ULONG32 Type;
		ULONG32 __alignment2;
	} MINIDUMP_MEMORY_INFO, *PMINIDUMP_MEMORY_INFO;
	//
	typedef struct _MINIDUMP_CALLBACK_OUTPUT {
		union {
			ULONG ModuleWriteFlags;
			ULONG ThreadWriteFlags;
			ULONG SecondaryFlags;
			struct {
				ULONG64 MemoryBase;
				ULONG MemorySize;
			};
			struct {
				BOOL CheckCancel;
				BOOL Cancel;
			};
			HANDLE Handle;
			struct {
				MINIDUMP_MEMORY_INFO VmRegion;
				BOOL Continue;
			};
			HRESULT Status;
		};
	} MINIDUMP_CALLBACK_OUTPUT, *PMINIDUMP_CALLBACK_OUTPUT;
	//
	typedef
		BOOL
		(WINAPI * MINIDUMP_CALLBACK_ROUTINE) (
			/*__inout*/ PVOID CallbackParam,
			/*__in*/    PMINIDUMP_CALLBACK_INPUT CallbackInput,
			/*__inout*/ PMINIDUMP_CALLBACK_OUTPUT CallbackOutput
			);
	//
	typedef struct _MINIDUMP_CALLBACK_INFORMATION {
		MINIDUMP_CALLBACK_ROUTINE CallbackRoutine;
		PVOID CallbackParam;
	} MINIDUMP_CALLBACK_INFORMATION, *PMINIDUMP_CALLBACK_INFORMATION;
	//
	typedef BOOL(WINAPI * MiniDumpWriteDumpT)(HANDLE, DWORD, HANDLE, MINIDUMP_TYPE
		, PMINIDUMP_EXCEPTION_INFORMATION
		, PMINIDUMP_USER_STREAM_INFORMATION
		, PMINIDUMP_CALLBACK_INFORMATION);
	//
	BOOL	l_bOwnDumpFile = FALSE;
	HANDLE	l_hDumpFile = hFile;
	MINIDUMP_EXCEPTION_INFORMATION	l_ExpParam;
	MiniDumpWriteDumpT l_pfMiniDumpWriteDump = NULL;
	//
	HMODULE	l_hDbgHelp = LoadLibraryA("DbgHelp.dll");
	if (l_hDbgHelp)
	{
		l_pfMiniDumpWriteDump = (MiniDumpWriteDumpT)GetProcAddress(l_hDbgHelp, "MiniDumpWriteDump");
	}
	//
	if (l_pfMiniDumpWriteDump)
	{
		if ((NULL == l_hDumpFile) || (INVALID_HANDLE_VALUE == l_hDumpFile))
		{
			char l_acDumpPath[4096] = { 0 };
			char* l_pAppName = strDumpName ? strDumpName : "MiniDump";
			SYSTEMTIME t = { 0 };
			//
			std::string strTemp;
			char szBuf[2048] = { 0 };
			if (0 != GetModuleFileNameA(NULL, szBuf, sizeof(szBuf) / sizeof(szBuf[0]) - 1))
			{
				strTemp = szBuf;
				size_t i = strTemp.rfind('\\');
				strTemp = strTemp.substr(0, i);
			}
			//
			GetLocalTime(&t);
			sprintf_s(l_acDumpPath, "%s\\%04d%02d%02d_%02d%02d%02d_P%ld_T%ld_%s.dmp"
				, strTemp.c_str()
				, t.wYear, t.wMonth, t.wDay
				, t.wHour, t.wMinute, t.wSecond
				, GetCurrentProcessId()
				, GetCurrentThreadId()
				, l_pAppName);
			//
			l_hDumpFile = CreateFileA(l_acDumpPath
				, GENERIC_READ | GENERIC_WRITE
				, FILE_SHARE_WRITE | FILE_SHARE_READ
				, 0
				, CREATE_ALWAYS
				, 0
				, 0);
			//
			//MessageBox(NULL, l_acDumpPath, NULL, MB_OK);
			l_bOwnDumpFile = TRUE;
		}//end if ( (NULL == l_hDumpFile) || (INVALID_HANDLE_VALUE == l_hDumpFile) )
		 //
		if (INVALID_HANDLE_VALUE != l_hDumpFile)
		{
			l_ExpParam.ThreadId = GetCurrentThreadId();
			l_ExpParam.ExceptionPointers = pExceptionInfo;
			l_ExpParam.ClientPointers = FALSE;
			//
			l_pfMiniDumpWriteDump(GetCurrentProcess()
				, GetCurrentProcessId()
				, l_hDumpFile
				, MiniDumpWithDataSegs
				, (pExceptionInfo ? &l_ExpParam : NULL)
				, NULL
				, NULL);
			//
			if (l_bOwnDumpFile)
			{
				CloseHandle(l_hDumpFile);
			}
		}//end if ( INVALID_HANDLE_VALUE != l_hDumpFile )
	}
	//
	if (NULL != l_hDbgHelp)
	{
		FreeLibrary(l_hDbgHelp);
	}
	//
	return EXCEPTION_EXECUTE_HANDLER;
}

long WINAPI qlExceptionFilter(PEXCEPTION_POINTERS pExceptionInfo, HANDLE hFile, const char* strDumpName)
{
	if (IsDebuggerPresent())
	{	//如果是调试模式，记录dump文件并交给调试器
		qlGenerateMiniDump(pExceptionInfo, hFile, strDumpName);
		return EXCEPTION_CONTINUE_SEARCH;
	}
	//
	return qlGenerateMiniDump(pExceptionInfo, hFile, strDumpName);
}