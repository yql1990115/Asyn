/***************************************************************************
*   Copyright (C) 2019 by yql1990115                                      *
*   yql1990115@gmail.com                                                  *
*                                                                         *
*   www.yql1990115.com                                                    *
***************************************************************************/

#pragma once

namespace async{

class IJobScheduler;
class IJobSchedulerTimer;

class IJob
{
public:
	/// 析构函数
	virtual 					~IJob(void){};

	/// 作业名称
	virtual const char*			Name() const = 0;
	
	/**
	调用此方法将作业放入调度程序
	
	@param pScheduler	
	*/
	virtual void				Schedule(IJobScheduler* pScheduler) = 0;
	
	/**
	调用此方法将作业放入定时调度程序
	
	@param dwSecondDelay
		延迟多少秒后执行此作业
	@param bRepeat
		值为FALSE, 此作业调度一次后, 被调度程序调用 \ref Release 函数删除
		值为TRUE, 调度程序每隔 \ref dwSecondDelay 秒调用一次此作业
	@param pScheduler
	*/
	virtual void				ScheduleTimer(DWORD dwSecondDelay, BOOL bRepeat, IJobSchedulerTimer* pScheduler) = 0;
	
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

}