/***************************************************************************
*   Copyright (C) 2019 by yql1990115                                      *
*   yql1990115@gmail.com                                                  *
*                                                                         *
*   www.yql1990115.com                                                    *
***************************************************************************/

#pragma once

namespace async{

class IJobState
{
public:
	virtual						~IJobState(){};

public:
	/// 函数返回的可能值 \ref GetStatus().	
	const static long	JOB_WAITING = 0;
	const static long	JOB_RUNNING = 1;
	const static long	JOB_SUSPENDED = 2;
	const static long	JOB_DONE = 3;

	/**
	除了处于JOB_DONE状态，其他状态在函数返回后都有可能发生变更
	 
	@return long
		\ref eJobStatus
	*/
	virtual long				GetStatus() const = 0;	
		
	/**
	等待Job执行状态为 \ref JOB_DONE \ref eJobStatus
	
	@param dwMilliSeconds
		超时时间, 值为INFINITE, 则会一直等待信号到来才返回
	@return BOOL
		如果超时将返回FALSE
	*/
	virtual BOOL 				WaitUntilDoneOrTimeout(DWORD dwMilliSeconds) = 0;
	
public:
	/**
	请求提前终止作业执行; 如果作业暂未调度执行，则阻止作业调度; Job执行状态为 \ref JOB_DONE
	*/	
	virtual void 				Terminate() = 0;
	
	/**
	如果已请求终止作业执行, 则返回TRUE
	注意: 虽然有终止请求, 但是作业有可能仍在执行中
	*/
	virtual BOOL 				HasBeenTerminated() const = 0;
	
	/**
	请求暂停作业执行
	*/	
	virtual void 				Stop() = 0;
	
	/**
	请求恢复作业执行
	*/	
	virtual void 				Resume() = 0;
	
	/**
	如果已请求暂停作业执行, 则返回TRUE
	注意: 虽然有暂停请求, 但是作业有可能仍在执行中
	*/
	virtual BOOL 				HasBeenSuspended() const = 0;

protected:
	/**
	作业执行完后设置作业状态为done
	*/
	virtual void				Done() = 0;

	/**
	作业执行完后设置作业状态为running
	*/
	virtual void				Executing() = 0;

	friend class IJob;
};

}