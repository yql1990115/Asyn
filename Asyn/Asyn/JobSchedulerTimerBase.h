/***************************************************************************
*   Copyright (C) 2019 by yql1990115                                      *
*   yql1990115@gmail.com                                                  *
*                                                                         *
*   www.yql1990115.com                                                    *
***************************************************************************/

#pragma once

#include <list>
#include <map>
#include "IJobSchedulerTimer.h"
#include "CriticalSection.h"
#include "WaitableEvent.h"
#include "WindowEvent.h"
#include "IJob.h"

namespace async{
	
class CJobSchedulerTimerBase : public IJobSchedulerTimer
{
public:
	CJobSchedulerTimerBase(const char* strName = "CJobSchedulerTimerBase");
	virtual ~CJobSchedulerTimerBase();

public:
	/// ��ʱ����������
	virtual const char*			Name() const override;

	/**
	������ʱ���ȳ���

	@return BOOL
	���ʧ�ܽ�����FALSE, ������Ϣ����ͨ��GetLastError()�������
	*/
	virtual BOOL				Startup() override;

	/**
	�رն�ʱ���ȳ���, �ͷ����е�����ҵ

	@param dwMilliSeconds
	�ȴ���������ҵ��ʱ��, ��ǿ�ƹر�������ҵ
	*/
	virtual void				Shutdown(DWORD dwMilliSecondsTimeOut = 30000) override;

	/**
	�����������ֹ���ȳ���, �򷵻�TRUE
	ע��: ��Ȼ����ֹ����, ���ǵ��ȳ����������ִ����, ֱ���ȴ���ʱ
	*/
	virtual BOOL 				HasBeenTerminated() const override;

	/**
	������ͣ���ȳ���ִ��
	*/
	virtual void 				Stop() override;

	/**
	����ָ����ȳ���ִ��
	*/
	virtual void 				Resume() override;

	/**
	�����������ͣ���ȳ���ִ��, �򷵻�TRUE
	ע��: ��Ȼ����ͣ����, ������ҵ�п�������ִ����
	*/
	virtual BOOL 				HasBeenSuspended() const override;

	/**
	�ж�������ȳ����Ƿ񻹻���
	*/
	virtual BOOL				HasAlive() const override;

	/// ֪ͨ�߳��˳� Shutdown֮ǰ���� �ӿ��Ƴ��ٶ�
	virtual void				NotifyExit() override;

public:
	/**
	����ҵ������ȳ�����

	@param pJob
	��ҵ

	@return BOOL
	�����ҵ������ȳ���ʧ��, ��������ҵ�� \ref Release() �����ͷ���ҵ������FALSE
	*/
	virtual BOOL				Schedule(IJob* pJob) override;
	
public:
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
	virtual BOOL				ScheduleTimer(DWORD dwSecondDelay, BOOL bRepeat, IJob* pJob) override;

	/**
	����ҵ�ӵ��ȳ������Ƴ�, ��ҵ���ڶ�ʱ������ʱɾ��
	
	@param pJob
	
	@return BOOL
		������ȳ��в����ڴ���ҵ, ����FALSE
	*/
	virtual BOOL				KillTimer(IJob* pJob) override;

public:
	///
	virtual void				EventProc();
	
	///
	virtual void				TimerProc();
	
	/// Legal values for expression in except().
	virtual long				OnException(struct _EXCEPTION_POINTERS *);

protected:
	class CTimerJobHelper
	{
	public:
		CTimerJobHelper(BOOL bRepeat, DWORD dwSecondDelay, IJob* pTimerJob)
		{
			assert(m_pTimerJob);
			m_bKill = FALSE;
			m_bRelease = FALSE;
			m_bRepeat = bRepeat;
			m_bExecute = FALSE;
			m_dwSecondDelay = dwSecondDelay;
			m_dwElapse = dwSecondDelay;
			m_pTimerJob = pTimerJob;
		};
		virtual ~CTimerJobHelper()
		{
			assert(nullptr == m_pTimerJob);
		};

	public:
		virtual void			Execute()
		{
			m_dwElapse--;
			if (0 == m_dwElapse)
			{
				m_pTimerJob->Execute();
				m_dwElapse = m_dwSecondDelay;
				m_bExecute = TRUE;
			}
		}

		virtual void			OnException()
		{
			m_pTimerJob->OnException();
		}

		virtual void			Release()
		{
			InterlockedExchange(&m_bRelease, TRUE);
			if (m_bRelease && m_bKill)
			{
				m_pTimerJob->Release();
				m_pTimerJob = nullptr;
				delete this;
			}
		}

		virtual void			KillJob()
		{
			InterlockedExchange(&m_bKill, TRUE);
			if (m_bRelease && m_bKill)
			{
				m_pTimerJob->Release();
				m_pTimerJob = nullptr;
				delete this;
			}
		}

	public:
		unsigned long volatile	m_bKill;
		unsigned long volatile	m_bRelease;
		BOOL					m_bRepeat;
		BOOL					m_bExecute;
		DWORD					m_dwSecondDelay;
		DWORD					m_dwElapse;
		IJob*					m_pTimerJob;
	};

protected:
	typedef std::list<CTimerJobHelper*>	LIST_TIMER_JOB;

	typedef std::map<IJob*, CTimerJobHelper*> CACHE_TIMER_JOB;

	/// �Ƴ���ʱ��ҵ����
	virtual void				RemoveListTimerJob();

	/// �Ƴ���ʱ��ҵ����
	virtual void				RemoveListTimerJob(LIST_TIMER_JOB& listTimerJob);

	/// �Ƴ���ʱ��ҵ�����п��Ա��Ƴ�����ҵ
	virtual void				RemoveListTimerJobIfCanKill(LIST_TIMER_JOB& listTimerJob);

	/// �Ƴ���ʱ��ҵ����
	virtual void				RemoveCacheTimerJob();

	/// �Ƴ���ʱ��ҵ�����п��Ա��Ƴ�����ҵ
	virtual void				RemoveCacheTimerJobIfCanKill();

	/// ����ʱ��ҵ
	virtual void				OnProcessTimerJob(CTimerJobHelper* pTimerJob);

protected:
	///
	typedef std::list<IJob*>	LIST_JOB;

	/// �Ƴ���ҵ����
	virtual void				RemoveListJob();

	/// �Ƴ���ҵ����
	virtual void				RemoveListJob(LIST_JOB& listJob);

	/// ������ҵ
	virtual void				OnProcessJob(IJob* pJob);

protected:
	/**
	�¼��źŴ���
	�� \ref m_terminated �¼��źŽ��д���
	1. ��⵽m_terminated�źŶ�����, ������������FALSE
	2. m_terminated�ź�����, ��������һ��OnTerminated(), ��������TRUE

	@return BOOL
	���m_terminated�źű�����, ��������TRUE; �����������FALSE

	*/
	virtual BOOL				SignalHandle();

	/// 
	virtual void				OnTerminated(){};

protected:
	/// ����������е��ȵ���ҵ
	LIST_JOB					m_ListJob;

	/// ��ҵ��
	CCriticalSection			m_ListJobLock;

	///
	LIST_TIMER_JOB				m_ListTimerJob;

	/// ��ʱ��ҵ��
	CCriticalSection			m_ListTimerJobLock;

	///
	CACHE_TIMER_JOB				m_CacheTimerJob;

	/// ��ʱ��ҵ������
	CCriticalSection			m_CacheTimerJobLock;

	/// ����¼�������, ���ȳ���ֱ��ɾ��ʣ��δ������ҵ, ���� \ref Schedule() �����޷�������ҵ 
	CWaitableEvent 				m_terminated;

	/// ����¼�������, �������µ���ҵ�Ž�����
	CWaitableEvent				m_HasNewJob;

protected:
	/// ��ʱ���ȳ��򴰿�����
	CWindowEvent<CJobSchedulerTimerBase> m_window;

	///
	std::string					m_strName;
};

}