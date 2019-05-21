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
	virtual BOOL				ScheduleTimer(DWORD dwSecondDelay, BOOL bRepeat, IJob* pJob) = 0;

	/**
	����ҵ�ӵ��ȳ������Ƴ�, ��ҵ���ڶ�ʱ������ʱɾ��
	
	@param pJob
	
	@return BOOL
		������ȳ��в����ڴ���ҵ, ����FALSE
	*/
	virtual BOOL				KillTimer(IJob* pJob) = 0;
};

}