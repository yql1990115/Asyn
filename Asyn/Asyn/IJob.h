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
	/// ��������
	virtual 					~IJob(void){};

	/// ��ҵ����
	virtual const char*			Name() const = 0;
	
	/**
	���ô˷�������ҵ������ȳ���
	
	@param pScheduler	
	*/
	virtual void				Schedule(IJobScheduler* pScheduler) = 0;
	
	/**
	���ô˷�������ҵ���붨ʱ���ȳ���
	
	@param dwSecondDelay
		�ӳٶ������ִ�д���ҵ
	@param bRepeat
		ֵΪFALSE, ����ҵ����һ�κ�, �����ȳ������ \ref Release ����ɾ��
		ֵΪTRUE, ���ȳ���ÿ�� \ref dwSecondDelay �����һ�δ���ҵ
	@param pScheduler
	*/
	virtual void				ScheduleTimer(DWORD dwSecondDelay, BOOL bRepeat, IJobSchedulerTimer* pScheduler) = 0;
	
	/**
	���ȳ�������ø÷���
	*/
	virtual void 				Execute() = 0;
	
	/**
	�ͷ��Լ�(˭����˭�ͷ�ԭ��)
	*/	
	virtual void				Release() = 0;

public:
	/**
	�����������ֹ��ҵִ��, �򷵻�TRUE
	ע��: ��Ȼ����ֹ����, ������ҵ�п�������ִ����
	*/
	virtual BOOL 				HasBeenTerminated() const = 0;

	/**
	�����������ͣ��ҵִ��, �򷵻�TRUE
	ע��: ��Ȼ����ͣ����, ������ҵ�п�������ִ����
	*/
	virtual BOOL 				HasBeenSuspended() const = 0;
	
public:
	/**
	������ȳ���ִ�д���ҵ�����쳣, �����ø÷���֪ͨ����ҵ
	
	@param pScheduler
	*/
	virtual void				OnException() = 0;
};

}