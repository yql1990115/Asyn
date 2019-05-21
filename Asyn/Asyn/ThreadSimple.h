/***************************************************************************
*   Copyright (C) 2019 by yql1990115                                      *
*   yql1990115@gmail.com                                                  *
*                                                                         *
*   www.yql1990115.com                                                    *
***************************************************************************/

#pragma once

#include <assert.h>
#include <process.h>

namespace async{

template<class TObj>
class CThreadSimple
{
public:
	CThreadSimple(TObj* instance, unsigned int (TObj::*ThreadProc)(), long(TObj::*OnException)(struct _EXCEPTION_POINTERS *) = nullptr);
	virtual ~CThreadSimple();

public:
	/**
	启动线程

	@return BOOL
	如果失败将返回FALSE, 错误信息可以通过GetLastError()函数获得
	*/
	virtual BOOL				Startup();

	/**
	关闭线程, 释放所有调度作业

	@param dwMilliSeconds
	等待运行中作业超时后, 将强制关闭所有作业
	*/
	virtual void				Shutdown(DWORD dwMilliSecondsTimeOut = 30000);

	/**
	判断任务线程是否还活着
	*/
	virtual BOOL				HasAlive() const;

	/**
	获取线程ID
	*/
	virtual unsigned int		GetThreadID() const;

	/**
	获取线程句柄
	*/
	virtual HANDLE				GetThreadHandle() const;

protected:
	/**
	线程安全入口
	*/
	static unsigned int __stdcall ISafeThreadProc(void *lpParam);

	/// 线程处理
	virtual unsigned int		ThreadProc();

	///
	virtual long				OnException(struct _EXCEPTION_POINTERS *);

protected:
	TObj*						m_Instance;

	///
	unsigned int				(TObj::*m_ThreadProc)();

	///
	long						(TObj::*m_OnException)(struct _EXCEPTION_POINTERS *);

protected:
	/// 线程句柄
	HANDLE						m_hThread;

	/// 线程ID
	unsigned int				m_nThreadID;
};

template<class TObj>
CThreadSimple<TObj>::CThreadSimple(TObj* instance, unsigned int (TObj::*ThreadProc)(), long(TObj::*OnException)(struct _EXCEPTION_POINTERS *)) : m_Instance(instance), m_ThreadProc(ThreadProc), m_OnException(OnException)
{
	assert(m_Instance);
	//
	m_hThread = INVALID_HANDLE_VALUE;
	m_nThreadID = 0;
}

template<class TObj>
CThreadSimple<TObj>::~CThreadSimple()
{
	Shutdown();
	//
	if (INVALID_HANDLE_VALUE != m_hThread)
	{
		::CloseHandle(m_hThread);
	}
	m_hThread = INVALID_HANDLE_VALUE;
}

template<class TObj>
BOOL CThreadSimple<TObj>::Startup()
{
	assert(m_Instance);
	//
	BOOL l_bRet = TRUE;
	m_hThread = (HANDLE)::_beginthreadex(0, 0, ISafeThreadProc, this, 0, &m_nThreadID);
	//
	if (((HANDLE)-1 == m_hThread) || ((HANDLE)0 == m_hThread))
	{
		m_hThread = INVALID_HANDLE_VALUE;
		m_nThreadID = 0;
		l_bRet = FALSE;
	}
	//
	return l_bRet;
}

template<class TObj>
void CThreadSimple<TObj>::Shutdown(DWORD dwMilliSecondsTimeOut)
{
	if (INVALID_HANDLE_VALUE != m_hThread)
	{
		::TerminateThread(m_hThread, 0);
		::WaitForSingleObject(m_hThread, 500);
		::CloseHandle(m_hThread);
		//
		m_hThread = INVALID_HANDLE_VALUE;
		m_nThreadID = 0;
	}
}

template<class TObj>
BOOL CThreadSimple<TObj>::HasAlive() const
{
	BOOL l_bRet = FALSE;
	if (INVALID_HANDLE_VALUE != m_hThread)
	{
		l_bRet = (WAIT_TIMEOUT == ::WaitForSingleObject(m_hThread, 0));
	}
	return l_bRet;
}

template<class TObj>
unsigned int CThreadSimple<TObj>::GetThreadID() const
{
	return m_nThreadID;
}

template<class TObj>
HANDLE CThreadSimple<TObj>::GetThreadHandle() const
{
	return m_hThread;
}

template<class TObj>
unsigned int __stdcall CThreadSimple<TObj>::ISafeThreadProc(void *lpParam)
{
	unsigned int l_nRet = 0;
	//
	DWORD l_dwEx = 0;
	CThreadSimple* pThis = (CThreadSimple*)lpParam;
	__try
	{
		if (pThis)
		{
			l_nRet = pThis->ThreadProc();
		}
		else
		{
			l_nRet = 1;
			assert(0);
		}
	}
	__except (l_dwEx = GetExceptionCode(), (pThis ? (pThis->OnException(GetExceptionInformation())) : EXCEPTION_EXECUTE_HANDLER))
	{
		assert(pThis);
		l_nRet = 2;
	}
	//
	return l_nRet;
}

template<class TObj>
unsigned int CThreadSimple<TObj>::ThreadProc()
{
	assert(m_Instance);
	assert(m_ThreadProc);
	//
	unsigned int l_nRet = 1;
	if ( m_Instance )
	{
		l_nRet = (m_Instance->*m_ThreadProc)();
	}
	return l_nRet;
}

///
template<class TObj>
long CThreadSimple<TObj>::OnException(struct _EXCEPTION_POINTERS * pInfo)
{
	long l_nRet = EXCEPTION_EXECUTE_HANDLER;
	//assert(m_Instance);
	//
	if (m_Instance && m_OnException)
	{
		l_nRet = (m_Instance->*m_OnException)(pInfo);
	}
	return l_nRet;
}

}