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

/// ��ʱ����������
const char* CqlSchedulerTimer::Name() const
{
	return m_strName.c_str();
}

/**
�������ȳ���

@return BOOL
���ʧ�ܽ�����FALSE, ������Ϣ����ͨ��GetLastError()�������
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
�رյ��ȳ���, �ͷ����е�����ҵ

@param dwMilliSeconds
�ȴ���������ҵ��ʱ��, ��ǿ�ƹر�������ҵ
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
�����������ֹ���ȳ���, �򷵻�TRUE
ע��: ��Ȼ����ֹ����, ���ǵ��ȳ����������ִ����, ֱ���ȴ���ʱ
*/
BOOL CqlSchedulerTimer::HasBeenTerminated() const
{
	BOOL l_bRet = m_terminated.Test();
	return l_bRet;
}

/**
������ͣ���ȳ���ִ��
*/
void CqlSchedulerTimer::Stop()
{
	//nothing
}

/**
����ָ����ȳ���ִ��
*/
void CqlSchedulerTimer::Resume()
{
	//nothing
}

/**
�����������ͣ���ȳ���ִ��, �򷵻�TRUE
ע��: ��Ȼ����ͣ����, ������ҵ�п�������ִ����
*/
BOOL CqlSchedulerTimer::HasBeenSuspended() const
{
	//nothing
	return FALSE;
}

/**
�ж�������ȳ����Ƿ񻹻���
*/
BOOL CqlSchedulerTimer::HasAlive() const
{
	BOOL l_bRet = m_window.HasAlive();
	return l_bRet;
}

/// ֪ͨ�߳��˳� Shutdown֮ǰ���� �ӿ��˳��ٶ�
void CqlSchedulerTimer::NotifyExit()
{
	m_terminated.Set();
}

/**
����ҵ������ȳ�����

@param pJob
��ҵ

@return BOOL
�����ҵ������ȳ���ʧ��, ��������ҵ�� \ref Release() �����ͷ���ҵ������FALSE
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
���ô˷�������ҵ���붨ʱ���ȳ���

@param dwSecondDelay
�ӳٶ������ִ�д���ҵ
@param bRepeat
ֵΪFALSE, ����ҵ����һ�κ�, ��������ҵ�� \ref Release ����ɾ������ҵ
ֵΪTRUE, ÿ�� \ref dwSecondDelay �����һ�δ���ҵ
@param pJob

@return BOOL
�����ҵ������ȳ���ʧ��, ��������ҵ�� \ref Release() �����ͷ���ҵ������FALSE
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
						assert(0); //�����ǲ��ܽ�ͬһ��Job�����ν�ScheduleTimer
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
����ҵ�ӵ��ȳ������Ƴ�, ��ҵ���ڶ�ʱ������ʱɾ��

@param pJob

@return BOOL
������ȳ��в����ڴ���ҵ, ����FALSE
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
		if (m_HasNewJob.WaitForEndOrTimeout(0)) //�¼������ȼ��ȶ�ʱ�����
		{
			m_HasNewJob.Reset();
			EventProc();
		}
		if (!(*it)->m_bKill)	//�� \ref KillTimer ����ɾ����TimerJob������
		{
			OnProcessTimerJob(*it);
		}
	}//end for
		//
	RemoveListTimerJobIfCanKill(l_listTimerJob);
	//
	if (!l_listTimerJob.empty()) //��ʱ��������Ϊ����ϲ���ȥ
	{
		CCriticalSectionLock l_lock(m_ListJobLock);
		m_ListTimerJob.splice(m_ListTimerJob.begin(), l_listTimerJob);
	}
	//
	RemoveCacheTimerJobIfCanKill(); //ɾ�������е���ҵ
}

long CqlSchedulerTimer::OnException(struct _EXCEPTION_POINTERS * pInfo)
{
	//assert(0);	//�쳣�ж��в�ʹ�ö���
	long l_nRet = qlExceptionFilter(pInfo, nullptr, m_strName.c_str());
	return l_nRet;	//EXCEPTION_EXECUTE_HANDLER
}

/// �Ƴ���ʱ��ҵ����
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

/// �Ƴ���ʱ��ҵ����
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

/// �Ƴ���ʱ��ҵ�����п��Ա��Ƴ�����ҵ
void CqlSchedulerTimer::RemoveListTimerJobIfCanKill(LIST_TIMER_JOB& listTimerJob)
{
	for (auto it = listTimerJob.begin(); it != listTimerJob.end(); )//it++)		//erase����������һ��Ԫ�أ�����һ���������
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

/// �Ƴ���ʱ��ҵ���� 
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

/// �Ƴ���ʱ��ҵ�����п��Ա��Ƴ�����ҵ 
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

/// ����ʱ��ҵ
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

/// �Ƴ���ҵ����
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

/// �Ƴ���ҵ����
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

/// ������ҵ
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
�¼��źŴ���
�� \ref m_terminated �¼��źŽ��д���
1. ��⵽m_terminated�źŶ�����, ������������FALSE
2. m_terminated�ź�����, ��������һ��OnTerminated(), ��������TRUE

@return BOOL
���m_terminated�źű�����, ��������TRUE; �����������FALSE
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
