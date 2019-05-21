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
	/// 定时调度器名称
	virtual const char*			Name() const override;

	/**
	启动定时调度程序

	@return BOOL
	如果失败将返回FALSE, 错误信息可以通过GetLastError()函数获得
	*/
	virtual BOOL				Startup() override;

	/**
	关闭定时调度程序, 释放所有调度作业

	@param dwMilliSeconds
	等待运行中作业超时后, 将强制关闭所有作业
	*/
	virtual void				Shutdown(DWORD dwMilliSecondsTimeOut = 30000) override;

	/**
	如果已请求终止调度程序, 则返回TRUE
	注意: 虽然有终止请求, 但是调度程序可能仍在执行中, 直到等待超时
	*/
	virtual BOOL 				HasBeenTerminated() const override;

	/**
	请求暂停调度程序执行
	*/
	virtual void 				Stop() override;

	/**
	请求恢复调度程序执行
	*/
	virtual void 				Resume() override;

	/**
	如果已请求暂停调度程序执行, 则返回TRUE
	注意: 虽然有暂停请求, 但是作业有可能仍在执行中
	*/
	virtual BOOL 				HasBeenSuspended() const override;

	/**
	判断任务调度程序是否还活着
	*/
	virtual BOOL				HasAlive() const override;

	/// 通知线程退出 Shutdown之前调用 加快推出速度
	virtual void				NotifyExit() override;

public:
	/**
	将作业放入调度程序中

	@param pJob
	作业

	@return BOOL
	如果作业放入调度池中失败, 将调用作业的 \ref Release() 函数释放作业并返回FALSE
	*/
	virtual BOOL				Schedule(IJob* pJob) override;
	
public:
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
	virtual BOOL				ScheduleTimer(DWORD dwSecondDelay, BOOL bRepeat, IJob* pJob) override;

	/**
	将作业从调度程序中移除, 作业将在定时器到来时删除
	
	@param pJob
	
	@return BOOL
		如果调度池中不存在此作业, 返回FALSE
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

	/// 移除定时作业链表
	virtual void				RemoveListTimerJob();

	/// 移除定时作业链表
	virtual void				RemoveListTimerJob(LIST_TIMER_JOB& listTimerJob);

	/// 移除定时作业链表中可以被移除的作业
	virtual void				RemoveListTimerJobIfCanKill(LIST_TIMER_JOB& listTimerJob);

	/// 移除定时作业缓存
	virtual void				RemoveCacheTimerJob();

	/// 移除定时作业缓存中可以被移除的作业
	virtual void				RemoveCacheTimerJobIfCanKill();

	/// 处理定时作业
	virtual void				OnProcessTimerJob(CTimerJobHelper* pTimerJob);

protected:
	///
	typedef std::list<IJob*>	LIST_JOB;

	/// 移除作业链表
	virtual void				RemoveListJob();

	/// 移除作业链表
	virtual void				RemoveListJob(LIST_JOB& listJob);

	/// 处理作业
	virtual void				OnProcessJob(IJob* pJob);

protected:
	/**
	事件信号处理
	对 \ref m_terminated 事件信号进行处理
	1. 检测到m_terminated信号都设置, 函数立即返回FALSE
	2. m_terminated信号设置, 函数调用一次OnTerminated(), 函数返回TRUE

	@return BOOL
	如果m_terminated信号被设置, 函数返回TRUE; 其他情况返回FALSE

	*/
	virtual BOOL				SignalHandle();

	/// 
	virtual void				OnTerminated(){};

protected:
	/// 保存请求进行调度的作业
	LIST_JOB					m_ListJob;

	/// 作业锁
	CCriticalSection			m_ListJobLock;

	///
	LIST_TIMER_JOB				m_ListTimerJob;

	/// 定时作业锁
	CCriticalSection			m_ListTimerJobLock;

	///
	CACHE_TIMER_JOB				m_CacheTimerJob;

	/// 定时作业缓存锁
	CCriticalSection			m_CacheTimerJobLock;

	/// 如果事件被设置, 调度程序将直接删除剩余未运行作业, 并且 \ref Schedule() 函数无法放入作业 
	CWaitableEvent 				m_terminated;

	/// 如果事件被设置, 表明有新的作业放进来了
	CWaitableEvent				m_HasNewJob;

protected:
	/// 定时调度程序窗口引擎
	CWindowEvent<CJobSchedulerTimerBase> m_window;

	///
	std::string					m_strName;
};

}