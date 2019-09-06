#include "stdafx.h"
#include "qlAsynHelper.h"
#include "qlSchedulerTimer.h"

namespace asyn {

CqlSchedulerTimer::CqlSchedulerTimer(const char* strName)
	: m_window(this, &CqlSchedulerTimer::EventProc, &CqlSchedulerTimer::TimerProc, &CqlSchedulerTimer::OnException)
{
	m_strName = strName ? strName : "";
}

CqlSchedulerTimer::~CqlSchedulerTimer()
{
	Shutdown();
}

/// 定时调度器名称
const char* CqlSchedulerTimer::Name() const
{
	return m_strName.c_str();
}

/**
启动调度程序

@return BOOL
如果失败将返回FALSE, 错误信息可以通过GetLastError()函数获得
*/
BOOL CqlSchedulerTimer::Startup()
{
	BOOL l_bRet = FALSE;
	//
	assert(FALSE == m_window.HasAlive());
	assert(m_ListJob.empty());
	assert(m_ListTimerJob.empty());
	assert(m_CacheTimerJob.empty());
	//
	Shutdown();
	//
	m_HasNewJob.Reset();
	m_terminated.Reset();
	//
	l_bRet = m_window.Startup();
	return l_bRet;
}


/**
关闭调度程序, 释放所有调度作业

@param dwMilliSeconds
等待运行中作业超时后, 将强制关闭所有作业
*/
void CqlSchedulerTimer::Shutdown(DWORD dwMilliSecondsTimeOut)
{
	if (m_window.HasAlive())
	{
		m_window.Shutdown();
	}
	//
	RemoveListJob();
	RemoveListTimerJob();
	RemoveCacheTimerJob();
}

/**
如果已请求终止调度程序, 则返回TRUE
注意: 虽然有终止请求, 但是调度程序可能仍在执行中, 直到等待超时
*/
BOOL CqlSchedulerTimer::HasBeenTerminated() const
{
	BOOL l_bRet = m_terminated.Test();
	return l_bRet;
}

/**
请求暂停调度程序执行
*/
void CqlSchedulerTimer::Stop()
{
	//nothing
}

/**
请求恢复调度程序执行
*/
void CqlSchedulerTimer::Resume()
{
	//nothing
}

/**
如果已请求暂停调度程序执行, 则返回TRUE
注意: 虽然有暂停请求, 但是作业有可能仍在执行中
*/
BOOL CqlSchedulerTimer::HasBeenSuspended() const
{
	//nothing
	return FALSE;
}

/**
判断任务调度程序是否还活着
*/
BOOL CqlSchedulerTimer::HasAlive() const
{
	BOOL l_bRet = m_window.HasAlive();
	return l_bRet;
}

/// 通知线程退出 Shutdown之前调用 加快退出速度
void CqlSchedulerTimer::NotifyExit()
{
	m_terminated.Set();
}

/**
将作业放入调度程序中

@param pJob
作业

@return BOOL
如果作业放入调度池中失败, 将调用作业的 \ref Release() 函数释放作业并返回FALSE
*/
BOOL CqlSchedulerTimer::Schedule(IJob* pJob)
{
	assert(pJob);
	//
	BOOL l_bSchedule = FALSE;
	if (pJob)
	{
		l_bSchedule = !HasBeenTerminated();
		if (l_bSchedule)
		{
			CCriticalSectionLock l_lock(m_ListJobLock);
			l_bSchedule = !HasBeenTerminated();
			if (l_bSchedule)
			{
				m_ListJob.push_back(pJob);
				m_HasNewJob.Set();
			}
		}
		//
		if (l_bSchedule)
		{
			m_window.PostEventMsg();
		}
		else
		{
			assert(pJob);
			pJob->Release();
			pJob = NULL;
		}
	}
	//
	return l_bSchedule;
}

/**
调用此方法将作业放入定时调度程序

@param dwSecondDelay
延迟多少秒后执行此作业
@param bRepeat
值为FALSE, 此作业调度一次后, 将调用作业的 \ref Release 函数删除该作业
值为TRUE, 每隔 \ref dwSecondDelay 秒调用一次此作业
@param pJob

@return BOOL
如果作业放入调度池中失败, 将调用作业的 \ref Release() 函数释放作业并返回FALSE
*/
BOOL CqlSchedulerTimer::ScheduleTimer(DWORD dwSecondDelay, BOOL bRepeat, IJob* pJob)
{
	assert(pJob);
	//
	BOOL l_bSchedule = FALSE;
	if (pJob)
	{
		CTimerJobHelper* l_pTimerJobHelper = nullptr;
		l_bSchedule = !HasBeenTerminated();
		if (l_bSchedule)
		{
			l_pTimerJobHelper = new CTimerJobHelper(bRepeat, dwSecondDelay, pJob);
			if (l_pTimerJobHelper)
			{
				{
					CCriticalSectionLock l_lock(m_CacheTimerJobLock);
					auto l_it = m_CacheTimerJob.find(pJob);
					if (l_it != m_CacheTimerJob.end())
					{
						assert(0); //正常是不能将同一个Job放两次进ScheduleTimer
						l_it->second->KillJob();
					}
					m_CacheTimerJob.insert(CACHE_TIMER_JOB::value_type(pJob, l_pTimerJobHelper));
				}
				//
				{
					CCriticalSectionLock l_lock(m_ListJobLock);
					l_bSchedule = !HasBeenTerminated();
					if (l_bSchedule)
					{
						m_ListJob.push_back(pJob);
					}
				}
			}//end if (l_pTimerHobHelper)
		}
		//
		if (!l_bSchedule)
		{
			if (l_pTimerJobHelper)
			{
				l_pTimerJobHelper->Release();
			}
			else
			{
				assert(pJob);
				pJob->Release();
				pJob = NULL;
			}
		}
	}
	//
	return l_bSchedule;
}

/**
将作业从调度程序中移除, 作业将在定时器到来时删除

@param pJob

@return BOOL
如果调度池中不存在此作业, 返回FALSE
*/
BOOL CqlSchedulerTimer::KillTimer(IJob* pJob)
{
	BOOL l_bKill = FALSE;
	CCriticalSectionLock l_lock(m_CacheTimerJobLock);
	auto l_it = m_CacheTimerJob.find(pJob);
	if (l_it != m_CacheTimerJob.end())
	{
		assert(l_it->second);
		l_it->second->KillJob();
		m_CacheTimerJob.erase(l_it);
		l_bKill = TRUE;
	}
	return l_bKill;
}

void CqlSchedulerTimer::EventProc()
{
	LIST_JOB l_listJob;
	//
	{
		CCriticalSectionLock l_lock(m_ListJobLock);
		l_listJob.swap(m_ListJob);
	}
	//
	for (auto it = l_listJob.begin(); it != l_listJob.end(); it++)
	{
		if (TRUE == SignalHandle())
		{
			break;
		}
		OnProcessJob(*it);
	}
	//
	RemoveListJob(l_listJob);
}

void CqlSchedulerTimer::TimerProc()
{
	LIST_TIMER_JOB l_listTimerJob;
	//
	{
		CCriticalSectionLock l_lock(m_ListJobLock);
		l_listTimerJob.swap(m_ListTimerJob);
	}
	//
	for (auto it = l_listTimerJob.begin(); it != l_listTimerJob.end(); it++)
	{
		if (TRUE == SignalHandle())
		{
			break;
		}
		if (m_HasNewJob.WaitForEndOrTimeout(0)) //事件的优先级比定时任务高
		{
			m_HasNewJob.Reset();
			EventProc();
		}
		if (!(*it)->m_bKill)	//被 \ref KillTimer 函数删除的TimerJob不调用
		{
			OnProcessTimerJob(*it);
		}
	}//end for
		//
	RemoveListTimerJobIfCanKill(l_listTimerJob);
	//
	if (!l_listTimerJob.empty()) //定时任务链表不为空则合并回去
	{
		CCriticalSectionLock l_lock(m_ListJobLock);
		m_ListTimerJob.splice(m_ListTimerJob.begin(), l_listTimerJob);
	}
	//
	RemoveCacheTimerJobIfCanKill(); //删除缓存中的作业
}

long CqlSchedulerTimer::OnException(struct _EXCEPTION_POINTERS * pInfo)
{
	//assert(0);	//异常中断中不使用断言
	long l_nRet = qlExceptionFilter(pInfo, nullptr, m_strName.c_str());
	return l_nRet;	//EXCEPTION_EXECUTE_HANDLER
}

/// 移除定时作业链表
void CqlSchedulerTimer::RemoveListTimerJob()
{
	LIST_TIMER_JOB l_listTimerJob;
	//
	{
		CCriticalSectionLock l_lock(m_ListTimerJobLock);
		l_listTimerJob.swap(m_ListTimerJob);
	}
	//
	RemoveListTimerJob(l_listTimerJob);
}

/// 移除定时作业链表
void CqlSchedulerTimer::RemoveListTimerJob(LIST_TIMER_JOB& listTimerJob)
{
	for (auto it = listTimerJob.begin(); it != listTimerJob.end(); it++)
	{
		CTimerJobHelper* l_pTimerJob = (*it);
		if (l_pTimerJob)
		{
			l_pTimerJob->Release();
		}
	}//end for
		//
	listTimerJob.clear();
}

/// 移除定时作业链表中可以被移除的作业
void CqlSchedulerTimer::RemoveListTimerJobIfCanKill(LIST_TIMER_JOB& listTimerJob)
{
	for (auto it = listTimerJob.begin(); it != listTimerJob.end(); )//it++)		//erase函数返回下一个元素，真是一个糟糕的设计
	{
		CTimerJobHelper* l_pTimerJob = (*it);
		if ((!l_pTimerJob->m_bRepeat && l_pTimerJob->m_bExecute) || l_pTimerJob->m_bKill)
		{
			l_pTimerJob->Release();
			it = listTimerJob.erase(it);
		}
		else
		{
			it++;
		}
	}//end for
}

/// 移除定时作业缓存 
void CqlSchedulerTimer::RemoveCacheTimerJob()
{
	CACHE_TIMER_JOB l_casheTimerJob;
	//
	{
		CCriticalSectionLock l_lock(m_CacheTimerJobLock);
		l_casheTimerJob.swap(m_CacheTimerJob);
	}
	//
	for (auto it = l_casheTimerJob.begin(); it != l_casheTimerJob.end(); it++)
	{
		it->second->KillJob();
	}
}

/// 移除定时作业缓存中可以被移除的作业 
void CqlSchedulerTimer::RemoveCacheTimerJobIfCanKill()
{
	CCriticalSectionLock l_lock(m_CacheTimerJobLock);
	for (auto it = m_CacheTimerJob.begin(); it != m_CacheTimerJob.end(); )	//it++)
	{
		if (it->second->m_bRelease)
		{
			it->second->KillJob();
			it = m_CacheTimerJob.erase(it);
		}
		else
		{
			it++;
		}
	}
}

/// 处理定时作业
void CqlSchedulerTimer::OnProcessTimerJob(CTimerJobHelper* pTimerJob)
{
	assert(pTimerJob);
	DWORD l_dwEx = 0;
	__try
	{
		pTimerJob->Execute();
	}
	__except (l_dwEx = GetExceptionCode(), EXCEPTION_EXECUTE_HANDLER)
	{
		pTimerJob->OnException();
	}
}

/// 移除作业链表
void CqlSchedulerTimer::RemoveListJob()
{
	LIST_JOB l_listJob;
	//
	{
		CCriticalSectionLock l_lock(m_ListJobLock);
		l_listJob.swap(m_ListJob);
	}
	//
	RemoveListJob(l_listJob);
}

/// 移除作业链表
void CqlSchedulerTimer::RemoveListJob(LIST_JOB& listJob)
{
	for (auto it = listJob.begin(); it != listJob.end(); it++)
	{
		IJob* l_pJob = (*it);
		assert(l_pJob);
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
void CqlSchedulerTimer::OnProcessJob(IJob* pJob)
{
	assert(pJob);
	DWORD l_dwEx = 0;
	__try
	{
		pJob->Execute();
	}
	__except (l_dwEx = GetExceptionCode(), EXCEPTION_EXECUTE_HANDLER)
	{
		pJob->OnException();
	}
}

/**
事件信号处理
对 \ref m_terminated 事件信号进行处理
1. 检测到m_terminated信号都设置, 函数立即返回FALSE
2. m_terminated信号设置, 函数调用一次OnTerminated(), 函数返回TRUE

@return BOOL
如果m_terminated信号被设置, 函数返回TRUE; 其他情况返回FALSE
*/
BOOL CqlSchedulerTimer::SignalHandle()
{
	BOOL l_bExitRet = FALSE;
	//
	l_bExitRet = m_terminated.Test();
	if (l_bExitRet)
	{
		OnTerminated();
	}
	return l_bExitRet;
}

}
