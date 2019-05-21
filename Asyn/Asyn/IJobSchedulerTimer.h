/***************************************************************************
*   Copyright (C) 2019 by yql1990115                                      *
*   yql1990115@gmail.com                                                  *
*                                                                         *
*   www.yql1990115.com                                                    *
***************************************************************************/

#pragma once

#include "IJobScheduler.h"

namespace async{
	
class IJobSchedulerTimer : public IJobScheduler
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