#ifndef __QLTHREADENGINE_H__
#define __QLTHREADENGINE_H__

#include <assert.h>
#include <process.h>

namespace asyn {

template<class TObj>
class CqlThreadEngine
{
public:
	CqlThreadEngine(TObj* instance, unsigned int (TObj::*ThreadProc)(), long(TObj::*OnException)(struct _EXCEPTION_POINTERS *) = nullptr);
	virtual ~CqlThreadEngine();

public:
	/**
	�����߳�

	@return BOOL
	���ʧ�ܽ�����FALSE, ������Ϣ����ͨ��GetLastError()�������
	*/
	virtual BOOL				Startup();

	/**
	�ر��߳�, �ͷ����е�����ҵ

	@param dwMilliSeconds
	�ȴ���������ҵ��ʱ��, ��ǿ�ƹر�������ҵ
	*/
	virtual void				Shutdown(DWORD dwMilliSecondsTimeOut = 30000);

	/**
	�ж������߳��Ƿ񻹻���
	*/
	virtual BOOL				HasAlive() const;

	/**
	��ȡ�߳�ID
	*/
	virtual unsigned int		GetThreadID() const;

	/**
	��ȡ�߳̾��
	*/
	virtual HANDLE				GetThreadHandle() const;

protected:
	CqlThreadEngine(const CqlThreadEngine&); //= delete;
	CqlThreadEngine& operator=(const CqlThreadEngine&); //= delete;

protected:
	/**
	�̰߳�ȫ���
	*/
	static unsigned int __stdcall ISafeThreadProc(void *lpParam);

	/// �̴߳���
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
	/// �߳̾��
	HANDLE						m_hThread;

	/// �߳�ID
	unsigned int				m_nThreadID;
};

template<class TObj>
CqlThreadEngine<TObj>::CqlThreadEngine(TObj* instance, unsigned int (TObj::*ThreadProc)(), long(TObj::*OnException)(struct _EXCEPTION_POINTERS *)) : m_Instance(instance), m_ThreadProc(ThreadProc), m_OnException(OnException)
{
	assert(m_Instance);
	//
	m_hThread = INVALID_HANDLE_VALUE;
	m_nThreadID = 0;
}

template<class TObj>
CqlThreadEngine<TObj>::~CqlThreadEngine()
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
BOOL CqlThreadEngine<TObj>::Startup()
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
void CqlThreadEngine<TObj>::Shutdown(DWORD dwMilliSecondsTimeOut)
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
BOOL CqlThreadEngine<TObj>::HasAlive() const
{
	BOOL l_bRet = FALSE;
	if (INVALID_HANDLE_VALUE != m_hThread)
	{
		l_bRet = (WAIT_TIMEOUT == ::WaitForSingleObject(m_hThread, 0));
	}
	return l_bRet;
}

template<class TObj>
unsigned int CqlThreadEngine<TObj>::GetThreadID() const
{
	return m_nThreadID;
}

template<class TObj>
HANDLE CqlThreadEngine<TObj>::GetThreadHandle() const
{
	return m_hThread;
}

template<class TObj>
unsigned int __stdcall CqlThreadEngine<TObj>::ISafeThreadProc(void *lpParam)
{
	unsigned int l_nRet = 0;
	//
	DWORD l_dwEx = 0;
	CqlThreadEngine* pThis = (CqlThreadEngine*)lpParam;
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
unsigned int CqlThreadEngine<TObj>::ThreadProc()
{
	assert(m_Instance);
	assert(m_ThreadProc);
	//
	unsigned int l_nRet = 1;
	if (m_Instance)
	{
		l_nRet = (m_Instance->*m_ThreadProc)();
	}
	return l_nRet;
}

///
template<class TObj>
long CqlThreadEngine<TObj>::OnException(struct _EXCEPTION_POINTERS * pInfo)
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

#endif /*!__QLTHREADENGINE_H__*/
