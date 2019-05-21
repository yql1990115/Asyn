#pragma once
#include "JobSchedulerBase.h"

namespace async{

template<unsigned long POOLSIZE = 8>
class CJobSchedulerPool : public CJobSchedulerBase
{
public:
	CJobSchedulerPool(const char* strName = "CJobSchedulerPool");
	virtual ~CJobSchedulerPool();

public:
	/**
	启动调度程序   主线程调用

	@return BOOL
	如果失败将返回FALSE, 错误信息可以通过GetLastError()函数获得
	*/
	virtual BOOL				Startup() override;

	/**
	关闭调度程序, 释放所有调度作业	主线程调用

	@param dwMilliSeconds
	等待运行中作业超时后, 将强制关闭所有作业
	*/
	virtual void				Shutdown(DWORD dwMilliSecondsTimeOut = 30000) override;

	/**
	请求暂停调度程序执行
	*/
	virtual void 				Stop() override;

	/**
	请求恢复调度程序执行
	*/
	virtual void 				Resume() override;

	/// 先设置退出信号，在设置有作业事件
	virtual void				NotifyExit() override;

public:
	///
	virtual unsigned int		GetSlaveSize() const;
	///
	IJobScheduler*				GetSlaveScheduler(unsigned int nIndex) const;

protected:
	/// 处理函数 默认返回0 出现错误1 崩溃2
	virtual unsigned int		OnProcess() override;

	/// 移除作业链表
	virtual void				RemoveListJob(LIST_JOB& listJob);

	/// 处理作业
	virtual void				OnProcessJob(IJob* pJob);

	/// 确认是否还有未使用线程
	virtual BOOL				HasNewThread();

protected:
	class CJobSchedulerSlave : public CJobSchedulerBase
	{
	public:
		CJobSchedulerSlave(const char* strName = "CJobSchedulerSlave_NameLess");

	public:
		/**
		启动调度程序   主线程调用

		@return BOOL
		如果失败将返回FALSE, 错误信息可以通过GetLastError()函数获得
		*/
		virtual BOOL			Startup(unsigned long volatile*	pnSlaveInfo, unsigned char nSlaveIndex, CWaitableEvent* pNotifyEvent, const char* strName);

	public:
		/// 处理函数 默认返回0 出现错误1 崩溃2
		virtual unsigned int	OnProcess() override;

	protected:
		///
		unsigned long volatile*	m_pnSlaveInfo;
		///
		unsigned char			m_nSlaveIndex;
		//
		CWaitableEvent*			m_pNotifyEvent;
	};

protected:
	const static long			s_SlavePageSize = 31;			//long型是4个字节，高位不用
	//
	typedef struct SlaveInfo_s
	{
		unsigned long volatile	SlaveInfo;
		CJobSchedulerSlave		acScheduler[s_SlavePageSize];
	}SlaveInfo_t;
	//
	SlaveInfo_t					m_atSlave[(POOLSIZE / s_SlavePageSize) + 1];
	//
	/// 如果事件被设置, 表明有新的线程可以使用
	CWaitableEvent				m_HasNewThread;
};

template<unsigned long POOLSIZE>
CJobSchedulerPool<POOLSIZE>::CJobSchedulerPool(const char* strName) : CJobSchedulerBase(strName)
{
	assert(sizeof(long) >= 4);	
}

template<unsigned long POOLSIZE>
CJobSchedulerPool<POOLSIZE>::~CJobSchedulerPool()
{
	Shutdown();
}

template<unsigned long POOLSIZE>
BOOL CJobSchedulerPool<POOLSIZE>::Startup()
{
	BOOL l_bRet = FALSE;
	//
	assert(FALSE == m_thread.HasAlive());
	assert(0 == m_ListJob.size());
	//
	Shutdown();
	//
	m_pause.Reset();
	m_HasNewJob.Reset();
	m_terminated.Reset();
	//
	for (int i = 0; i < (POOLSIZE / s_SlavePageSize) + 1; i++)
	{
		m_atSlave[i].SlaveInfo = 0;
	}
	//
	int l_nIndex0 = 0;
	int l_nIndex1 = 0;
	for (unsigned long i = 0; i < POOLSIZE; i++)
	{
		l_nIndex0 = i / s_SlavePageSize;
		l_nIndex1 = i % s_SlavePageSize;
		m_atSlave[l_nIndex0].acScheduler[l_nIndex1].Startup(&m_atSlave[l_nIndex0].SlaveInfo, l_nIndex1, &m_HasNewThread, Format("CJobSchedulerSlave_%d", i).c_str());
	}
	//
	l_bRet = m_thread.Startup();
	return l_bRet;
}

template<unsigned long POOLSIZE>
void CJobSchedulerPool<POOLSIZE>::Shutdown(DWORD dwMilliSecondsTimeOut)
{
	NotifyExit();
	//
	CJobSchedulerBase::Shutdown(dwMilliSecondsTimeOut);
	//
	int l_nIndex0 = 0;
	int l_nIndex1 = 0;
	for (unsigned long i = 0; i < POOLSIZE; i++)
	{
		l_nIndex0 = i / s_SlavePageSize;
		l_nIndex1 = i % s_SlavePageSize;
		m_atSlave[l_nIndex0].acScheduler[l_nIndex1].Shutdown(dwMilliSecondsTimeOut);
	}
}

/**
请求暂停调度程序执行
*/
template<unsigned long POOLSIZE>
void CJobSchedulerPool<POOLSIZE>::Stop()
{
	int l_nIndex0 = 0;
	int l_nIndex1 = 0;
	for (unsigned long i = 0; i < POOLSIZE; i++)
	{
		l_nIndex0 = i / s_SlavePageSize;
		l_nIndex1 = i % s_SlavePageSize;
		m_atSlave[l_nIndex0].acScheduler[l_nIndex1].Stop();
	}
	//
	CJobSchedulerBase::Stop();
}

/**
请求恢复调度程序执行
*/
template<unsigned long POOLSIZE>
void CJobSchedulerPool<POOLSIZE>::Resume()
{
	int l_nIndex0 = 0;
	int l_nIndex1 = 0;
	for (unsigned long i = 0; i < POOLSIZE; i++)
	{
		l_nIndex0 = i / s_SlavePageSize;
		l_nIndex1 = i % s_SlavePageSize;
		m_atSlave[l_nIndex0].acScheduler[l_nIndex1].Resume();
	}
	//
	CJobSchedulerBase::Resume();
}

/// 先设置退出信号，在设置有作业事件
template<unsigned long POOLSIZE>
void CJobSchedulerPool<POOLSIZE>::NotifyExit()
{
	int l_nIndex0 = 0;
	int l_nIndex1 = 0;
	for (unsigned long i = 0; i < POOLSIZE; i++)
	{
		l_nIndex0 = i / s_SlavePageSize;
		l_nIndex1 = i % s_SlavePageSize;
		m_atSlave[l_nIndex0].acScheduler[l_nIndex1].NotifyExit();
	}
	//
	CJobSchedulerBase::NotifyExit();
	m_HasNewThread.Set();
}

template<unsigned long POOLSIZE>
unsigned int CJobSchedulerPool<POOLSIZE>::GetSlaveSize() const
{
	return POOLSIZE;
}
///
template<unsigned long POOLSIZE>
IJobScheduler* CJobSchedulerPool<POOLSIZE>::GetSlaveScheduler(unsigned int nIndex) const
{
	IJobScheduler* l_pRet = nullptr;
	if (nIndex >= 0 && nIndex < POOLSIZE)
	{
		int l_nIndex0 = nIndex / s_SlavePageSize;
		int l_nIndex1 = nIndex % s_SlavePageSize;
		//
		l_pRet = &m_atSlave[l_nIndex0].acScheduler[l_nIndex1];
	}
	return l_pRet;
}

/// 处理函数 默认返回0 出现错误1 崩溃2
template<unsigned long POOLSIZE>
unsigned int CJobSchedulerPool<POOLSIZE>::OnProcess()
{
	unsigned int l_nRet = 0;
	//
	BOOL l_bExit = FALSE;
	LIST_JOB l_listJob;
	for (;;)
	{
		if (m_HasNewJob.WaitForEndOrTimeout(INFINITE))
		{
			if (SignalHandle())
			{
				goto tagExit;
			}
			//
			{
				CCriticalSectionLock l_lock(m_ListJobLock);
				l_listJob.swap(m_ListJob);
				m_HasNewJob.Reset();
			}
			//
			if (!m_HasNewThread.WaitForEndOrTimeout(INFINITE))
			{
				l_nRet = 1;
				goto tagExit;
			}
			//
			for (LIST_JOB::iterator l_it = l_listJob.begin(); l_it != l_listJob.end(); l_it++)
			{
				if (l_bExit = SignalHandle())
				{
					goto tagExit;
				}
				IJob* l_pJob = NULL;
				l_pJob = (*l_it);
				assert(l_pJob);
				if (l_pJob)
				{
					OnProcessJob(l_pJob);
					(*l_it) = nullptr;
					//
					if ( !HasNewThread() )
					{
						if (!m_HasNewThread.WaitForEndOrTimeout(INFINITE))
						{
							l_nRet = 1;
							goto tagExit;
						}
					}
				}
			}//end for
			//
			RemoveListJob(l_listJob);
		}
		else
		{
			l_nRet = 1;
			break;
		}//end if (m_HasNewJob.WaitForEndOrTimeout(INFINITE))
	}//end for

tagExit:
	RemoveListJob(l_listJob);
	return l_nRet;
}

/// 移除作业链表
template<unsigned long POOLSIZE>
void CJobSchedulerPool<POOLSIZE>::RemoveListJob(LIST_JOB& listJob)
{
	//如果作业转交给Slave, 则l_pJob被置为空指针; 所以允许空指针存在
	for (auto it = listJob.begin(); it != listJob.end(); it++)
	{
		IJob* l_pJob = (*it);
		if (l_pJob)
		{
			l_pJob->Release();
			l_pJob = nullptr;
		}
	}//end for
	//
	listJob.clear();
}

/// 处理作业
template<unsigned long POOLSIZE>
void CJobSchedulerPool<POOLSIZE>::OnProcessJob(IJob* pJob)
{
	int l_nIndex0 = -1;
	int l_nIndex1 = -1;
	for (int i = 0; i < POOLSIZE; i++)
	{
		l_nIndex0 = i / s_SlavePageSize;
		l_nIndex1 = i % s_SlavePageSize;
		//
		long l_nMask = 1 << l_nIndex1;
		if (m_atSlave[l_nIndex0].SlaveInfo & l_nMask)
		{
			unsigned long l_nRet = InterlockedExchangeAdd(&m_atSlave[l_nIndex0].SlaveInfo, -l_nMask);
			assert(l_nRet & l_nMask);
			assert(0 == (m_atSlave[l_nIndex0].SlaveInfo & l_nMask));
			break;
		}
	}
	//
	//OutputDebugStringA(Format("slave %d process job %d\r\n", l_nIndex1, GetTickCount()).c_str());
	assert(-1 != l_nIndex0);
	assert(-1 != l_nIndex1);
	if ((l_nIndex0 != -1) && (l_nIndex1 != -1))
	{
		m_atSlave[l_nIndex0].acScheduler[l_nIndex1].Schedule(pJob);
	}
	else
	{
		if (pJob)
		{
			pJob->Release();
			pJob = NULL;
		}
	}
}

/// 确认是否还有未使用线程
template<unsigned long POOLSIZE>
BOOL CJobSchedulerPool<POOLSIZE>::HasNewThread()
{
	BOOL l_bHasSlaveThread = FALSE;
	for (int i = 0; i < (POOLSIZE / s_SlavePageSize) + 1; i++)
	{
		if (m_atSlave[i].SlaveInfo)
		{
			l_bHasSlaveThread = TRUE;
			break;
		}
	}
	//
	if (!l_bHasSlaveThread)
	{
		m_HasNewThread.Reset();
		//
		for (int i = 0; i < (POOLSIZE / s_SlavePageSize) + 1; i++)
		{
			if (m_atSlave[i].SlaveInfo)
			{
				l_bHasSlaveThread = TRUE;
				break;
			}
		}
		//
		if (l_bHasSlaveThread)
		{
			m_HasNewThread.Set();
		}
	}
	return l_bHasSlaveThread;
}

template<unsigned long POOLSIZE>
CJobSchedulerPool<POOLSIZE>::CJobSchedulerSlave::CJobSchedulerSlave(const char* strName)
	: CJobSchedulerBase(strName)
{
	m_pnSlaveInfo = nullptr;
	m_nSlaveIndex = 0;
	m_pNotifyEvent = nullptr;
}

/**
启动调度程序   主线程调用

@return BOOL
如果失败将返回FALSE, 错误信息可以通过GetLastError()函数获得
*/
template<unsigned long POOLSIZE>
BOOL CJobSchedulerPool<POOLSIZE>::CJobSchedulerSlave::Startup(unsigned long volatile* pnSlaveInfo, unsigned char nSlaveIndex, CWaitableEvent* pNotifyEvent, const char* strName)
{
	BOOL l_bRet = FALSE;
	//
	assert(pnSlaveInfo);
	assert(pNotifyEvent);
	assert(strName);
	//
	m_pnSlaveInfo = pnSlaveInfo;
	m_nSlaveIndex = nSlaveIndex;
	m_pNotifyEvent = pNotifyEvent;
	m_strName = strName ? strName : "";
	//
	l_bRet = CJobSchedulerBase::Startup();
	return l_bRet;
}

/// 处理函数 默认返回0 出现错误1 崩溃2
template<unsigned long POOLSIZE>
unsigned int CJobSchedulerPool<POOLSIZE>::CJobSchedulerSlave::OnProcess()
{
	assert(m_pnSlaveInfo);
	assert(m_pNotifyEvent);
	//
	unsigned int l_nRet = 0;
	LIST_JOB l_listJob;
	//
	if ((nullptr == m_pnSlaveInfo) || (nullptr == m_pNotifyEvent))
	{
		l_nRet = 1;
		goto tagExit;
	}
	//
	//notify master thread
	long l_nMask = 1 << m_nSlaveIndex;
	unsigned long l_nSlaveInfo = InterlockedExchangeAdd(m_pnSlaveInfo, l_nMask);
	m_pNotifyEvent->Set();
	//
	for (;;)
	{
		if (m_HasNewJob.WaitForEndOrTimeout(INFINITE))
		{
			//OutputDebugStringA(Format("Enter slave %d %d\r\n", m_nSlaveIndex, GetTickCount()).c_str());
			if (SignalHandle())
			{
				goto tagExit;
			}
			//
			{
				CCriticalSectionLock l_lock(m_ListJobLock);
				l_listJob.swap(m_ListJob);
				m_HasNewJob.Reset();
			}
			//
			BOOL l_bExit = FALSE;
			for (LIST_JOB::iterator l_it = l_listJob.begin(); l_it != l_listJob.end(); l_it++)
			{
				if (l_bExit = SignalHandle())
				{
					goto tagExit;
				}
				IJob* l_pJob = NULL;
				l_pJob = (*l_it);
				assert(l_pJob);
				if (l_pJob)
				{
					OnProcessJob(l_pJob);
				}
			}//end for
			//
			RemoveListJob(l_listJob);
			//
			//notify master thread
			l_nSlaveInfo = InterlockedExchangeAdd(m_pnSlaveInfo, l_nMask);
			m_pNotifyEvent->Set();
			//OutputDebugStringA(Format("Leave slave %d %d\r\n", m_nSlaveIndex, GetTickCount()).c_str());
		}
		else
		{
			l_nRet = 1;
			break;
		}//end if (m_HasNewJob.WaitForEndOrTimeout(INFINITE))
	}//end for

tagExit:
	RemoveListJob(l_listJob);
	return l_nRet;
}

}
