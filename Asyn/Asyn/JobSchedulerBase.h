/***************************************************************************
*   Copyright (C) 2019 by yql1990115                                      *
*   yql1990115@gmail.com                                                  *
*                                                                         *
*   www.yql1990115.com                                                    *
***************************************************************************/

#pragma once

#include <list>
#include <string>
#include "IJobScheduler.h"
#include "CriticalSection.h"
#include "WaitableEvent.h"
#include "ThreadSimple.h"

namespace async{

class CJobSchedulerBase : public IJobScheduler
{
public:
	CJobSchedulerBase(const char* strName = "CJobSchedulerBase");
	virtual ~CJobSchedulerBase();

public:
	/// ����������
	virtual const char*			Name() const override;

	/**
	�������ȳ���   ���̵߳���
	
	@return BOOL
		���ʧ�ܽ�����FALSE, ������Ϣ����ͨ��GetLastError()�������
	*/
	virtual BOOL				Startup() override;
	
	/**
	�رյ��ȳ���, �ͷ����е�����ҵ	���̵߳���
	
	@param dwMilliSeconds
		�ȴ���������ҵ��ʱ��, ��ǿ�ƹر�������ҵ
	*/
	virtual void				Shutdown(DWORD dwMilliSecondsTimeOut = 30000) override;
	
	/**
	�����������ֹ���ȳ���, �򷵻�TRUE
	ע��: ��Ȼ����ֹ����, ���ǵ��ȳ����������ִ����, ֱ���ȴ���ʱ
	*/
	virtual BOOL 				HasBeenTerminated() const override;
	
	/**
	������ͣ���ȳ���ִ��
	*/	
	virtual void 				Stop() override;
	
	/**
	����ָ����ȳ���ִ��
	*/	
	virtual void 				Resume() override;
	
	/**
	�����������ͣ���ȳ���ִ��, �򷵻�TRUE
	ע��: ��Ȼ����ͣ����, ������ҵ�п�������ִ����
	*/
	virtual BOOL 				HasBeenSuspended() const override;

	/**
	�ж�������ȳ����Ƿ񻹻���
	*/
	virtual BOOL				HasAlive() const override;

	/// �������˳��źţ�����������ҵ�¼�
	virtual void				NotifyExit() override;

public:
	/**
	����ҵ������ȳ�����
	
	@param pJob
		��ҵ
		
	@return BOOL	
		�����ҵ������ȳ���ʧ��, ��������ҵ�� \ref Release() �����ͷ���ҵ������FALSE
	*/
	virtual BOOL				Schedule(IJob* pJob) override;

public:
	/// ������ Ĭ�Ϸ���0 ���ִ���1 ����2
	virtual unsigned int		OnProcess();

	/// Legal values for expression in except().
	virtual long				OnException(struct _EXCEPTION_POINTERS *);

protected:
	typedef std::list<IJob*>	LIST_JOB;

	/// �Ƴ���ҵ����
	virtual void				RemoveListJob();
	
	/// �Ƴ���ҵ����
	virtual void				RemoveListJob(LIST_JOB& listJob);
	
	/// ������ҵ
	virtual void				OnProcessJob(IJob* pJob);

	/**
	�¼��źŴ���
		�� \ref m_pause �� \ref m_terminated �¼��źŽ��д��� 
		1. ��⵽m_pause��m_terminated�źŶ�δ����, ������������FALSE
		2. ��⵽m_pause�ź�����, m_terminated�ź�δ����, ���������һ��OnPause���ȴ�
			1). m_pause�źű�����, m_terminated�ź�δ����, ��������һ��OnResume, ��������FALSE
			2). m_terminated�ź�����, ��������һ��OnTerminated(), ��������TRUE
		3. m_terminated�ź�����, ��������һ��OnTerminated(), ��������TRUE

	@return BOOL
	���m_terminated�źű�����, ��������TRUE; �����������FALSE
	*/
	virtual BOOL				SignalHandle();
	
	/// 
	virtual void				OnTerminated(){};
	
	///
	virtual void				OnPause(){};

	///
	virtual void				OnResume(){};

	///
	virtual std::string			Format(const char *strFmt, ...);

protected:
	/// ����������е��ȵ���ҵ
	LIST_JOB					m_ListJob;

	/// ��ҵ��
	CCriticalSection			m_ListJobLock;

	/// ����¼�������, ���ȳ���Ӧ������ͣ����, ֱ���¼���λ
	CWaitableEvent				m_pause;

	/// ����¼�������, ���ȳ���ֱ��ɾ��ʣ��δ������ҵ, ���� \ref Schedule() �����޷�������ҵ 
	CWaitableEvent 				m_terminated;
	
	/// ����¼�������, �������µ���ҵ�Ž�����
	CWaitableEvent				m_HasNewJob;

protected:
	/// ���ȳ����߳�����
	CThreadSimple<CJobSchedulerBase>	m_thread;
	
	///
	std::string					m_strName;
};



}