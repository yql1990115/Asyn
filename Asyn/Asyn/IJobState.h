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
	/// �������صĿ���ֵ \ref GetStatus().	
	const static long	JOB_WAITING = 0;
	const static long	JOB_RUNNING = 1;
	const static long	JOB_SUSPENDED = 2;
	const static long	JOB_DONE = 3;

	/**
	���˴���JOB_DONE״̬������״̬�ں������غ��п��ܷ������
	 
	@return long
		\ref eJobStatus
	*/
	virtual long				GetStatus() const = 0;	
		
	/**
	�ȴ�Jobִ��״̬Ϊ \ref JOB_DONE \ref eJobStatus
	
	@param dwMilliSeconds
		��ʱʱ��, ֵΪINFINITE, ���һֱ�ȴ��źŵ����ŷ���
	@return BOOL
		�����ʱ������FALSE
	*/
	virtual BOOL 				WaitUntilDoneOrTimeout(DWORD dwMilliSeconds) = 0;
	
public:
	/**
	������ǰ��ֹ��ҵִ��; �����ҵ��δ����ִ�У�����ֹ��ҵ����; Jobִ��״̬Ϊ \ref JOB_DONE
	*/	
	virtual void 				Terminate() = 0;
	
	/**
	�����������ֹ��ҵִ��, �򷵻�TRUE
	ע��: ��Ȼ����ֹ����, ������ҵ�п�������ִ����
	*/
	virtual BOOL 				HasBeenTerminated() const = 0;
	
	/**
	������ͣ��ҵִ��
	*/	
	virtual void 				Stop() = 0;
	
	/**
	����ָ���ҵִ��
	*/	
	virtual void 				Resume() = 0;
	
	/**
	�����������ͣ��ҵִ��, �򷵻�TRUE
	ע��: ��Ȼ����ͣ����, ������ҵ�п�������ִ����
	*/
	virtual BOOL 				HasBeenSuspended() const = 0;

protected:
	/**
	��ҵִ�����������ҵ״̬Ϊdone
	*/
	virtual void				Done() = 0;

	/**
	��ҵִ�����������ҵ״̬Ϊrunning
	*/
	virtual void				Executing() = 0;

	friend class IJob;
};

}