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
	/// ��������
	virtual 					~IJob(void) {};

	/// ��ҵ����
	virtual const char*			Name() const = 0;

	/**
	���ô˷�������ҵ������ȳ���

	@param pScheduler
	@return BOOL
	������������ʧ�ܽ�����FALSE	(Ʃ��������ر�)
	*/
	virtual BOOL				Schedule(IScheduler* pScheduler) = 0;

	/**
	���ô˷�������ҵ���붨ʱ���ȳ���

	@param dwSecondDelay
	�ӳٶ������ִ�д���ҵ
	@param bRepeat
	ֵΪFALSE, ����ҵ����һ�κ�, �����ȳ������ \ref Release ����ɾ��
	ֵΪTRUE, ���ȳ���ÿ�� \ref dwSecondDelay �����һ�δ���ҵ
	@param pScheduler
	@return BOOL
	������������ʧ�ܽ�����FALSE	(Ʃ��������ر�)
	*/
	virtual BOOL				ScheduleTimer(DWORD dwSecondDelay, BOOL bRepeat, ISchedulerTimer* pScheduler) = 0;

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


class IJobState
{
public:
	virtual						~IJobState() {};

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

friend class CqlJob;
};


class IScheduler
{
public:
	/// ��������
	virtual 					~IScheduler() {};

public:
	/// ����������
	virtual const char*			Name() const = 0;

	/**
	�������ȳ���

	@return BOOL
	���ʧ�ܽ�����FALSE, ������Ϣ����ͨ��GetLastError()�������
	*/
	virtual BOOL				Startup() = 0;

	/**
	�رյ��ȳ���, �ͷ����е�����ҵ

	@param dwMilliSeconds
	�ȴ���������ҵ��ʱ��, ��ǿ�ƹر�������ҵ
	*/
	virtual void				Shutdown(DWORD dwMilliSecondsTimeOut) = 0;

	/**
	�����������ֹ���ȳ���, �򷵻�TRUE
	ע��: ��Ȼ����ֹ����, ���ǵ��ȳ����������ִ����, ֱ���ȴ���ʱ
	*/
	virtual BOOL 				HasBeenTerminated() const = 0;

	/**
	������ͣ���ȳ���ִ��
	*/
	virtual void 				Stop() = 0;

	/**
	����ָ����ȳ���ִ��
	*/
	virtual void 				Resume() = 0;

	/**
	�����������ͣ���ȳ���ִ��, �򷵻�TRUE
	ע��: ��Ȼ����ͣ����, ������ҵ�п�������ִ����
	*/
	virtual BOOL 				HasBeenSuspended() const = 0;

	/**
	�ж�������ȳ����Ƿ񻹻���
	*/
	virtual BOOL				HasAlive() const = 0;

	/// ֪ͨ�߳��˳� Shutdown֮ǰ���� �ӿ��Ƴ��ٶ�
	virtual void				NotifyExit() = 0;

public:
	/**
	����ҵ������ȳ�����

	@param pJob
	��ҵ

	@return BOOL
	�����ҵ������ȳ���ʧ��, ��������ҵ�� \ref Release() �����ͷ���ҵ������FALSE
	*/
	virtual BOOL				Schedule(IJob* pJob) = 0;
};


class ISchedulerTimer : public IScheduler
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

#endif	/*!__ASYNINTERFACE_H__*/


