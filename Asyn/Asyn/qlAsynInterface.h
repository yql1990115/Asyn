/*
===========================================================================

Copyright (C) 2019 by yql1990115
	yql1990115@gmail.com
	www.yql1990115.com

===========================================================================
*/

#ifndef __ASYNINTERFACE_H__
#define __ASYNINTERFACE_H__

namespace asyn {

class IJob;
class IJobState;
class IScheduler;
class ISchedulerTimer;

class IJob
{
public:
	/// 析构函数
	virtual 					~IJob(void) {};

	/// 作业名称
	virtual const char*			Name() const = 0;

	/**
	调用此方法将作业放入调度程序

	@param pScheduler
	@return BOOL
	如果放入调度器失败将返回FALSE	(譬如调度器关闭)
	*/
	virtual BOOL				Schedule(IScheduler* pScheduler) = 0;

	/**
	调用此方法将作业放入定时调度程序

	@param dwSecondDelay
	延迟多少秒后执行此作业
	@param bRepeat
	值为FALSE, 此作业调度一次后, 被调度程序调用 \ref Release 函数删除
	值为TRUE, 调度程序每隔 \ref dwSecondDelay 秒调用一次此作业
	@param pScheduler
	@return BOOL
	如果放入调度器失败将返回FALSE	(譬如调度器关闭)
	*/
	virtual BOOL				ScheduleTimer(DWORD dwSecondDelay, BOOL bRepeat, ISchedulerTimer* pScheduler) = 0;

	/**
	调度程序负责调用该方法
	*/
	virtual void 				Execute() = 0;

	/**
	释放自己(谁创建谁释放原则)
	*/
	virtual void				Release() = 0;

public:
	/**
	如果已请求终止作业执行, 则返回TRUE
	注意: 虽然有终止请求, 但是作业有可能仍在执行中
	*/
	virtual BOOL 				HasBeenTerminated() const = 0;

	/**
	如果已请求暂停作业执行, 则返回TRUE
	注意: 虽然有暂停请求, 但是作业有可能仍在执行中
	*/
	virtual BOOL 				HasBeenSuspended() const = 0;

public:
	/**
	如果调度程序执行此作业发生异常, 将调用该方法通知此作业

	@param pScheduler
	*/
	virtual void				OnException() = 0;
};


class IJobState
{
public:
	virtual						~IJobState() {};

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

friend class CqlJob;
};


class IScheduler
{
public:
	/// 析构函数
	virtual 					~IScheduler() {};

public:
	/// 调度器名称
	virtual const char*			Name() const = 0;

	/**
	启动调度程序

	@return BOOL
	如果失败将返回FALSE, 错误信息可以通过GetLastError()函数获得
	*/
	virtual BOOL				Startup() = 0;

	/**
	关闭调度程序, 释放所有调度作业

	@param dwMilliSeconds
	等待运行中作业超时后, 将强制关闭所有作业
	*/
	virtual void				Shutdown(DWORD dwMilliSecondsTimeOut) = 0;

	/**
	如果已请求终止调度程序, 则返回TRUE
	注意: 虽然有终止请求, 但是调度程序可能仍在执行中, 直到等待超时
	*/
	virtual BOOL 				HasBeenTerminated() const = 0;

	/**
	请求暂停调度程序执行
	*/
	virtual void 				Stop() = 0;

	/**
	请求恢复调度程序执行
	*/
	virtual void 				Resume() = 0;

	/**
	如果已请求暂停调度程序执行, 则返回TRUE
	注意: 虽然有暂停请求, 但是作业有可能仍在执行中
	*/
	virtual BOOL 				HasBeenSuspended() const = 0;

	/**
	判断任务调度程序是否还活着
	*/
	virtual BOOL				HasAlive() const = 0;

	/// 通知线程退出 Shutdown之前调用 加快推出速度
	virtual void				NotifyExit() = 0;

public:
	/**
	将作业放入调度程序中

	@param pJob
	作业

	@return BOOL
	如果作业放入调度池中失败, 将调用作业的 \ref Release() 函数释放作业并返回FALSE
	*/
	virtual BOOL				Schedule(IJob* pJob) = 0;
};


class ISchedulerTimer : public IScheduler
{
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
	virtual BOOL				ScheduleTimer(DWORD dwSecondDelay, BOOL bRepeat, IJob* pJob) = 0;

	/**
	将作业从调度程序中移除, 作业将在定时器到来时删除

	@param pJob

	@return BOOL
	如果调度池中不存在此作业, 返回FALSE
	*/
	virtual BOOL				KillTimer(IJob* pJob) = 0;
};

}

#endif	/*!__ASYNINTERFACE_H__*/


