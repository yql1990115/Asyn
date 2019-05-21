/***************************************************************************
*   Copyright (C) 2019 by yql1990115                                      *
*   yql1990115@gmail.com                                                  *
*                                                                         *
*   www.yql1990115.com                                                    *
***************************************************************************/

#pragma once

#include "WaitableEvent.h"
#include "IJobState.h"

namespace async{

class CJobStateBase : public IJobState
{
public:
	CJobStateBase();

public:
	/**
	���˴���JOB_DONE״̬������״̬�ں������غ��п��ܷ������
	 
	@return eJobStatus
		\ref eJobStatus
	*/
	virtual long				GetStatus() const override;	
		
	/**
	�ȴ�Jobִ��״̬Ϊ \ref JOB_DONE \ref eJobStatus
	
	@param dwMilliSeconds
		��ʱʱ��
	@return BOOL
		�����ʱ������FALSE
	*/
	virtual BOOL 				WaitUntilDoneOrTimeout(DWORD dwMilliSeconds) override;
	
public:
	/**
	������ǰ��ֹ��ҵִ��; �����ҵ��δ����ִ�У�����ֹ��ҵ����; Jobִ��״̬Ϊ \ref JOB_DONE
	*/	
	virtual void 				Terminate() override;
	
	/**
	�����������ֹ��ҵִ��, �򷵻�TRUE
	ע��: ��Ȼ����ֹ����, ������ҵ�п�������ִ����
	*/
	virtual BOOL 				HasBeenTerminated() const override;
	
	/**
	������ͣ��ҵִ��
	*/	
	virtual void 				Stop() override;
	
	/**
	����ָ���ҵִ��
	*/	
	virtual void 				Resume() override;
	
	/**
	�����������ͣ��ҵִ��, �򷵻�TRUE
	ע��: ��Ȼ����ͣ����, ������ҵ�п�������ִ����
	*/
	virtual BOOL 				HasBeenSuspended() const override;

public:
	/**
	��ҵִ�����������ҵ״̬Ϊdone
	*/
	virtual void				Done() override;

	/**
	��ҵִ�����������ҵ״̬Ϊrunning
	*/
	virtual void				Executing() override;
	
protected:
	/// waitable event. Will be set after \ref Execute() finished.
    COneShotEvent 				m_executionDone;
	
	/// ����¼�������, ��ҵӦ������ͣ����, ֱ���¼���λ
	CWaitableEvent				m_pause;

    /// ֵΪTRUEֱ�� Execute() ����������
	volatile unsigned long 		m_nWaiting;

    /// �������, ��ҵ��Ӧ�����л��Ծ�����ֹ
	volatile unsigned long 		m_nTerminated;
};

}