#ifndef __ASYNHELPER_H__
#define __ASYNHELPER_H__

#include <memory>
#include <string>
#include <assert.h>
#include <windows.h>
#include "qlAsynInterface.h"

namespace asyn {

/***************************************************************************
*   Copyright (C) 2009 by Stefan Fuhrmann                                 *
*   stefanfuhrmann@alice-dsl.de
***************************************************************************/
class CCriticalSection
{
private:
	/// OS-specific critical section object
	CRITICAL_SECTION section;

public:

	/// construction
	inline CCriticalSection(void)
	{ 
		InitializeCriticalSectionAndSpinCount(&section, 5000); 
	};

	/// destruction
	inline ~CCriticalSection(void)
	{
		DeleteCriticalSection(&section);
	};

	/// Acquire the mutex, i.e. enter the critical section
	inline void Acquire()
	{
		EnterCriticalSection(&section);
	};

	/// Release the mutex, i.e. leave the critical section
	inline void Release()
	{
		LeaveCriticalSection(&section);
	};
};

/**
* RAII lock class for \ref CCriticalSection mutexes.
*/
class CCriticalSectionLock
{
private:

	CCriticalSection& section;

	// dummy assignment operator to silence the C4512 compiler warning
	// not implemented assignment operator (even is dummy and private) to
	// get rid of cppcheck error as well as accidental use
	CCriticalSectionLock & operator=(const CCriticalSectionLock &);
public:

	inline CCriticalSectionLock(CCriticalSection& section)
		: section(section)
	{
		section.Acquire();
	}

	inline ~CCriticalSectionLock()
	{
		section.Release();
	}
};


/**
* A waitable event that can be signaled only once.
* Also, only one waiting thread is allowed.
*
* This is a more lightweight implementation than
* \ref CWaitableEvent as the \ref event handle is
* only allocated if \ref WaitFor() requires it.
*/

class COneShotEvent
{
private:

	/// OS-specific event object

	HANDLE event;
	volatile unsigned long state;

public:

	/// construction / destruction: manage event handle

	COneShotEvent();
	~COneShotEvent();

	/// eventing interface

	void Set();
	bool Test() const;
	void WaitFor();

	/// returns false in case of a timeout

	bool WaitForEndOrTimeout(DWORD milliSeconds);
};

/**
* A waitable event that must be reset manually.
* This implementation uses a global pool of event
* handles to minimize OS handle creation and destruction
* overhead.
*/

class CWaitableEvent
{
private:

	/// OS-specific event object

	HANDLE event;

public:

	/// construction / destruction: manage event handle

	CWaitableEvent();
	~CWaitableEvent();

	/// eventing interface

	void Set();
	void Reset();
	bool Test() const;
	void WaitFor();

	/// returns false in case of a timeout

	bool WaitForEndOrTimeout(DWORD milliSeconds);
};


/*
//////////////////////////////////////////////////////////////////////////
//  作业状态实现类
//	(如果把JOB比喻为一辆车, 那么JobState即为这辆车的方向盘/刹车/油门, 即通过JobState对JOB进行控制)
//////////////////////////////////////////////////////////////////////////
*/
class CqlJobState : public IJobState
{
public:
	CqlJobState();

public:
	virtual long				GetStatus() const override;

	virtual BOOL 				WaitUntilDoneOrTimeout(DWORD dwMilliSeconds) override;

public:
	virtual void 				Terminate() override;

	virtual BOOL 				HasBeenTerminated() const override;

	virtual void 				Stop() override;

	virtual void 				Resume() override;

	virtual BOOL 				HasBeenSuspended() const override;

public:
	virtual void				Done() override;

	virtual void				Executing() override;

protected:
	COneShotEvent 				m_executionDone;
	CWaitableEvent				m_pause;
	volatile unsigned long 		m_nWaiting;
	volatile unsigned long 		m_nTerminated;
};

inline CqlJobState::CqlJobState()
{
	m_nWaiting = TRUE;
	m_nTerminated = FALSE;
}

inline long CqlJobState::GetStatus() const
{
	long l_nRet = JOB_WAITING;
	if (m_nWaiting != TRUE)
	{
		l_nRet = m_executionDone.Test() ? JOB_DONE : JOB_RUNNING;
		if (JOB_RUNNING == l_nRet)
		{
			l_nRet = m_pause.Test() ? JOB_SUSPENDED : JOB_RUNNING;
		}
	}
	return l_nRet;
}

inline BOOL CqlJobState::WaitUntilDoneOrTimeout(DWORD dwMilliSeconds)
{
	BOOL l_bRet = m_executionDone.WaitForEndOrTimeout(dwMilliSeconds);
	return l_bRet;
}

inline void CqlJobState::Terminate()
{
	InterlockedExchange(&m_nTerminated, TRUE);
}

inline BOOL CqlJobState::HasBeenTerminated() const
{
	BOOL l_bRet = (TRUE == m_nTerminated);
	return l_bRet;
}

inline void CqlJobState::Stop()
{
	m_pause.Set();
}

inline void CqlJobState::Resume()
{
	m_pause.Reset();
}

inline BOOL CqlJobState::HasBeenSuspended() const
{
	BOOL l_bRet = m_pause.Test();
	return l_bRet;
}

inline void CqlJobState::Done()
{
	m_executionDone.Set();
}

inline void CqlJobState::Executing()
{
	InterlockedExchange(&m_nWaiting, FALSE);
}

/*
//////////////////////////////////////////////////////////////////////////
// 作业实现类
//////////////////////////////////////////////////////////////////////////
*/
class CqlJob : public IJob
{
public:
	virtual const char*			Name() const;

	virtual BOOL				Schedule(IScheduler* pScheduler) override;

	virtual BOOL				ScheduleTimer(DWORD dwSecondDelay, BOOL bRepeat, ISchedulerTimer* pScheduler) override;

	virtual void 				Execute() override;

	virtual void				Release() override;

public:
	virtual BOOL 				HasBeenTerminated() const override;

	virtual BOOL 				HasBeenSuspended() const override;

protected:
	explicit CqlJob(std::shared_ptr<IJobState>* pState, const char* strName = "NameLess", BOOL bMustInternalExecute = FALSE);

	virtual void InternalExecute() = 0;

	virtual void HandleException() {};

	virtual ~CqlJob(void);

protected:
	virtual void				OnException() override;

#ifdef _DEBUG
	virtual BOOL				IsLocalRunSingleErr()
	{
		BOOL l_bErr = FALSE;
		if (m_nCreateJobThreadID == GetCurrentThreadId())
		{
			if (m_spState)
			{
				l_bErr = m_spState->HasBeenSuspended();
			}
		}
		return l_bErr;
	}
#endif

protected:
	/// 创建作业的所在线程ID 
	volatile long				m_nCreateJobThreadID;
	//
	std::shared_ptr<IJobState>	m_spState;
	//
	const std::string			m_strName;
	//
	BOOL						m_bMustInternalExecute;
};

inline const char* CqlJob::Name() const
{
	return m_strName.c_str();
}

inline BOOL CqlJob::Schedule(IScheduler* pScheduler)
{
	BOOL l_bRet = FALSE;
	assert(pScheduler);
	if (pScheduler)
	{
		l_bRet = pScheduler->Schedule(this);
	}
	return l_bRet;
}

inline BOOL CqlJob::ScheduleTimer(DWORD dwSecondDelay, BOOL bRepeat, ISchedulerTimer* pScheduler)
{
	BOOL l_bRet = FALSE;
	assert(pScheduler);
	if (pScheduler)
	{
		l_bRet = pScheduler->ScheduleTimer(dwSecondDelay, bRepeat, this);
	}
	return l_bRet;
}

inline void CqlJob::Execute()
{
	//如果作业是在创建线程中运行，则不应该置暂停信号
	assert(!IsLocalRunSingleErr());
	//
	if (m_spState)
	{
		m_spState->Executing();
	}
	//
	BOOL l_bTerminated = FALSE;
	if (m_spState)
	{
		l_bTerminated = m_spState->HasBeenTerminated();
	}
	//
	if (m_bMustInternalExecute || (FALSE == l_bTerminated))
	{
		InternalExecute();
	}
	//
	if (m_spState)
	{
		m_spState->Done();
	}
}

inline void CqlJob::Release()
{
	if (m_spState)
	{
		m_spState->Terminate();
	}
	//
	assert(this);
	delete this;
}

inline BOOL CqlJob::HasBeenTerminated() const
{
	BOOL l_bRet = FALSE;
	if (m_spState)
	{
		l_bRet = m_spState->HasBeenTerminated();
	}
	return l_bRet;
}

inline BOOL CqlJob::HasBeenSuspended() const
{
	BOOL l_bRet = FALSE;
	if (m_spState)
	{
		l_bRet = m_spState->HasBeenSuspended();
	}
	return l_bRet;
}

inline CqlJob::CqlJob(std::shared_ptr<IJobState>* pState, const char* strName /* = "NameLess" */, BOOL bMustInternalExecute /* = FALSE */)
	: m_nCreateJobThreadID(GetCurrentThreadId()), m_strName(strName ? strName : ""), m_bMustInternalExecute(bMustInternalExecute)
{
	if (pState)
	{
		m_spState = *pState;
	}
}

inline CqlJob::~CqlJob(void)
{
	//nothing
}

inline void CqlJob::OnException()
{
	HandleException();
	//
	if (m_spState)
	{
		m_spState->Done();
	}
}

}

/**
生成dump文件
\ref hFile 和 \ref strDumpName不存在, 则使用默认名生成dump文件
如果检测到有调试器, 则返回值置为EXCEPTION_CONTINUE_SEARCH, 交给调试器

@param pExceptionInfo
通过GetExceptionInformation()获取的异常信息
@param hFile
如果句柄存在, 则dump信息写入这个句柄指向的文件; 句柄存在则忽略strDumpName参数
@param strDumpName
如果 \ref hFile 不存在, 则函数内部将使用strDumpName创建一个文件句柄并使用

@return long
#define EXCEPTION_EXECUTE_HANDLER       1
#define EXCEPTION_CONTINUE_SEARCH       0
#define EXCEPTION_CONTINUE_EXECUTION    -1
*/
extern long WINAPI qlExceptionFilter(PEXCEPTION_POINTERS pExceptionInfo, HANDLE hFile, const char* strDumpName);

/**
生成dump文件
\ref hFile 和 \ref strDumpName不存在, 则使用默认名生成dump文件

@param pExceptionInfo
通过GetExceptionInformation()获取的异常信息
@param hFile
如果句柄存在, 则dump信息写入这个句柄指向的文件; 句柄存在则忽略strDumpName参数
@param strDumpName
如果 \ref hFile 不存在, 则函数内部将使用strDumpName创建一个文件句柄并使用

@return long
#define EXCEPTION_EXECUTE_HANDLER       1
#define EXCEPTION_CONTINUE_SEARCH       0
#define EXCEPTION_CONTINUE_EXECUTION    -1
*/
extern long WINAPI qlGenerateMiniDump(PEXCEPTION_POINTERS pExceptionInfo, HANDLE hFile, const char* strDumpName);


#endif /* !__ASYNHELPER_H__ */
