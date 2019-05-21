/***************************************************************************
*   Copyright (C) 2019 by yql1990115                                      *
*   yql1990115@gmail.com                                                  *
*                                                                         *
*   www.yql1990115.com                                                    *
***************************************************************************/

#pragma once

#include "IJob.h"

namespace async{
	
class IJobScheduler
{
public:
	/// 析构函数
	virtual 					~IJobScheduler(){};

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

}