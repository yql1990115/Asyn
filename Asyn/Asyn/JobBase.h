/***************************************************************************
*   Copyright (C) 2019 by yql1990115                                      *
*   yql1990115@gmail.com                                                  *
*                                                                         *
*   www.yql1990115.com                                                    *
***************************************************************************/

#pragma once

#include "IJob.h"
#include "IJobState.h"
#include "IJobScheduler.h"
#include "IJobSchedulerTimer.h"
#include "WaitableEvent.h"
#include "JobSchedulerBase.h"
#include "JobSchedulerTimerBase.h"
#include <processthreadsapi.h>
#include <memory>
#include <string>

namespace async{

template<class TState>
class CJobBase : public IJob
{
public:
	/// 作业名称
	virtual const char*			Name() const;

	/**
	调用此方法将作业放入调度程序
	
	@param pScheduler	
	*/
	virtual void				Schedule(IJobScheduler* pScheduler) override;
	
	/**
	调用此方法将作业放入定时调度程序
	
	@param dwSecondDelay
		延迟多少秒后执行此作业
	@param bRepeat
		值为FALSE, 此作业调度一次后, 被调度程序调用 \ref Release 函数删除
		值为TRUE, 调度程序每隔 \ref dwSecondDelay 秒调用一次此作业
	@param pScheduler
	*/
	virtual void				ScheduleTimer(DWORD dwSecondDelay, BOOL bRepeat, IJobSchedulerTimer* pScheduler) override;
	
	/**
	调度程序负责调用该方法
	*/
	virtual void 				Execute() override;
		
	/**
	释放自己(谁创建谁释放原则)
	*/	
	virtual void				Release() override;

public:
	/**
	如果已请求终止作业执行, 则返回TRUE
	注意: 虽然有终止请求, 但是作业有可能仍在执行中
	*/
	virtual BOOL 				HasBeenTerminated() const override;

	/**
	如果已请求暂停作业执行, 则返回TRUE
	注意: 虽然有暂停请求, 但是作业有可能仍在执行中
	*/
	virtual BOOL 				HasBeenSuspended() const override;
	
protected:
    /// 构造
	explicit CJobBase(std::shared_ptr<TState>* spState, const char* strName = "NameLess");

    /// implement this in your job class
	virtual void InternalExecute() = 0;

    /// 析构
    virtual ~CJobBase(void);
	
protected:
	/**
	如果调度程序执行此作业发生异常, 将调用该方法通知此作业
	
	@param pScheduler
	*/
	virtual void				OnException() override;

#ifdef _DEBUG
	virtual BOOL				IsLocalRunSingleErr()
	{
		BOOL l_bErr = FALSE;
		if (m_nCreateJobThreadID == GetCurrentThreadId())
		{
			if (m_spState.get())
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
	std::shared_ptr<TState>		m_spState;
	//
	const std::string			m_strName;
};


/// 作业名称
template<class TState>
const char* CJobBase<TState>::Name() const
{
	return m_strName.c_str();
}

/**
调用此方法将作业放入调度程序

@param pScheduler
*/
template<class TState>
void CJobBase<TState>::Schedule(IJobScheduler* pScheduler)
{
	assert(pScheduler);
	if (pScheduler)
	{
		BOOL l_bRet = pScheduler->Schedule(this);
		//assert(l_bRet);
	}
}

/**
调用此方法将作业放入定时调度程序

@param dwSecondDelay
延迟多少秒后执行此作业
@param bRepeat
值为FALSE, 此作业调度一次后, 被调度程序调用 \ref Release 函数删除
值为TRUE, 调度程序每隔 \ref dwSecondDelay 秒调用一次此作业
@param pScheduler
*/
template<class TState>
void CJobBase<TState>::ScheduleTimer(DWORD dwSecondDelay, BOOL bRepeat, IJobSchedulerTimer* pScheduler)
{
	assert(pScheduler);
	if (pScheduler)
	{
		BOOL l_bRet = pScheduler->ScheduleTimer(dwSecondDelay, bRepeat, this);
		//assert(l_bRet);
	}
}

/**
调度程序负责调用该方法
*/
template<class TState>
void CJobBase<TState>::Execute()
{
	TState	l_State;
	TState* l_pState = m_spState.get();
	if (!l_pState)
	{
		l_pState = &l_State;
	}
	//如果作业是在创建线程中运行，则不应该置暂停信号
	assert(!IsLocalRunSingleErr());
	//
	l_pState->Executing();
	//
	if (FALSE == l_pState->HasBeenTerminated())
	{
		InternalExecute();
	}
	l_pState->Done();
}

/**
释放自己(谁创建谁释放原则)
*/
template<class TState>
void CJobBase<TState>::Release()
{
	TState* l_pState = m_spState.get();
	if (l_pState)
	{
		l_pState->Terminate();
	}
	//
	assert(this);
	delete this;
}

/**
如果已请求终止作业执行, 则返回TRUE
注意: 虽然有终止请求, 但是作业有可能仍在执行中
*/
template<class TState>
BOOL CJobBase<TState>::HasBeenTerminated() const
{
	BOOL l_bRet = FALSE;
	TState* l_pState = m_spState.get();
	if (l_pState)
	{
		l_bRet = l_pState->HasBeenTerminated();
	}
	return l_bRet;
}

/**
如果已请求暂停作业执行, 则返回TRUE
注意: 虽然有暂停请求, 但是作业有可能仍在执行中
*/
template<class TState>
BOOL CJobBase<TState>::HasBeenSuspended() const
{
	BOOL l_bRet = FALSE;
	TState* l_pState = m_spState.get();
	if (l_pState)
	{
		l_bRet = l_pState->HasBeenSuspended();
	}
	return l_bRet;
}

/// base class is not intended for creation
template<class TState>
CJobBase<TState>::CJobBase(std::shared_ptr<TState>* spState, const char* strName)
	: m_nCreateJobThreadID(GetCurrentThreadId()), m_strName(strName ? strName : "")
{
	if (spState)
	{
		m_spState = *spState;
	}
}

/// asserts that the job is deletable
template<class TState>
CJobBase<TState>::~CJobBase(void)
{
	//nothing
}

/**
如果调度程序执行此作业发生异常, 将调用该方法通知此作业

@param pScheduler
*/
template<class TState>
void CJobBase<TState>::OnException()
{
	//nothing
}

}