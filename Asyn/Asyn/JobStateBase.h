/***************************************************************************
*   Copyright (C) 2019 by yql1990115                                      *
*   yql1990115@gmail.com                                                  *
*                                                                         *
*   www.yql1990115.com                                                    *
***************************************************************************/

#pragma once

#include "WaitableEvent.h"
#include "IJobState.h"

namespace async{

class CJobStateBase : public IJobState
{
public:
	CJobStateBase();

public:
	/**
	除了处于JOB_DONE状态，其他状态在函数返回后都有可能发生变更
	 
	@return eJobStatus
		\ref eJobStatus
	*/
	virtual long				GetStatus() const override;	
		
	/**
	等待Job执行状态为 \ref JOB_DONE \ref eJobStatus
	
	@param dwMilliSeconds
		超时时间
	@return BOOL
		如果超时将返回FALSE
	*/
	virtual BOOL 				WaitUntilDoneOrTimeout(DWORD dwMilliSeconds) override;
	
public:
	/**
	请求提前终止作业执行; 如果作业暂未调度执行，则阻止作业调度; Job执行状态为 \ref JOB_DONE
	*/	
	virtual void 				Terminate() override;
	
	/**
	如果已请求终止作业执行, 则返回TRUE
	注意: 虽然有终止请求, 但是作业有可能仍在执行中
	*/
	virtual BOOL 				HasBeenTerminated() const override;
	
	/**
	请求暂停作业执行
	*/	
	virtual void 				Stop() override;
	
	/**
	请求恢复作业执行
	*/	
	virtual void 				Resume() override;
	
	/**
	如果已请求暂停作业执行, 则返回TRUE
	注意: 虽然有暂停请求, 但是作业有可能仍在执行中
	*/
	virtual BOOL 				HasBeenSuspended() const override;

public:
	/**
	作业执行完后设置作业状态为done
	*/
	virtual void				Done() override;

	/**
	作业执行完后设置作业状态为running
	*/
	virtual void				Executing() override;
	
protected:
	/// waitable event. Will be set after \ref Execute() finished.
    COneShotEvent 				m_executionDone;
	
	/// 如果事件被设置, 作业应尝试暂停运行, 直到事件复位
	CWaitableEvent				m_pause;

    /// 值为TRUE直到 Execute() 函数被调用
	volatile unsigned long 		m_nWaiting;

    /// 如果设置, 作业不应该运行或尝试尽快终止
	volatile unsigned long 		m_nTerminated;
};

}