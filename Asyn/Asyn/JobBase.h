/***************************************************************************
*   Copyright (C) 2019 by yql1990115                                      *
*   yql1990115@gmail.com                                                  *
*                                                                         *
*   www.yql1990115.com                                                    *
***************************************************************************/

#pragma once

#include "IJob.h"
#include "IJobState.h"
#include "IJobScheduler.h"
#include "IJobSchedulerTimer.h"
#include "WaitableEvent.h"
#include "JobSchedulerBase.h"
#include "JobSchedulerTimerBase.h"
#include <processthreadsapi.h>
#include <memory>
#include <string>

namespace async{

template<class TState>
class CJobBase : public IJob
{
public:
	/// ��ҵ����
	virtual const char*			Name() const;

	/**
	���ô˷�������ҵ������ȳ���
	
	@param pScheduler	
	*/
	virtual void				Schedule(IJobScheduler* pScheduler) override;
	
	/**
	���ô˷�������ҵ���붨ʱ���ȳ���
	
	@param dwSecondDelay
		�ӳٶ������ִ�д���ҵ
	@param bRepeat
		ֵΪFALSE, ����ҵ����һ�κ�, �����ȳ������ \ref Release ����ɾ��
		ֵΪTRUE, ���ȳ���ÿ�� \ref dwSecondDelay �����һ�δ���ҵ
	@param pScheduler
	*/
	virtual void				ScheduleTimer(DWORD dwSecondDelay, BOOL bRepeat, IJobSchedulerTimer* pScheduler) override;
	
	/**
	���ȳ�������ø÷���
	*/
	virtual void 				Execute() override;
		
	/**
	�ͷ��Լ�(˭����˭�ͷ�ԭ��)
	*/	
	virtual void				Release() override;

public:
	/**
	�����������ֹ��ҵִ��, �򷵻�TRUE
	ע��: ��Ȼ����ֹ����, ������ҵ�п�������ִ����
	*/
	virtual BOOL 				HasBeenTerminated() const override;

	/**
	�����������ͣ��ҵִ��, �򷵻�TRUE
	ע��: ��Ȼ����ͣ����, ������ҵ�п�������ִ����
	*/
	virtual BOOL 				HasBeenSuspended() const override;
	
protected:
    /// ����
	explicit CJobBase(std::shared_ptr<TState>* spState, const char* strName = "NameLess");

    /// implement this in your job class
	virtual void InternalExecute() = 0;

    /// ����
    virtual ~CJobBase(void);
	
protected:
	/**
	������ȳ���ִ�д���ҵ�����쳣, �����ø÷���֪ͨ����ҵ
	
	@param pScheduler
	*/
	virtual void				OnException() override;

#ifdef _DEBUG
	virtual BOOL				IsLocalRunSingleErr()
	{
		BOOL l_bErr = FALSE;
		if (m_nCreateJobThreadID == GetCurrentThreadId())
		{
			if (m_spState.get())
			{
				l_bErr = m_spState->HasBeenSuspended();
			}
		}
		return l_bErr;
	}
#endif

protected:
	/// ������ҵ�������߳�ID 
	volatile long				m_nCreateJobThreadID;
	//
	std::shared_ptr<TState>		m_spState;
	//
	const std::string			m_strName;
};


/// ��ҵ����
template<class TState>
const char* CJobBase<TState>::Name() const
{
	return m_strName.c_str();
}

/**
���ô˷�������ҵ������ȳ���

@param pScheduler
*/
template<class TState>
void CJobBase<TState>::Schedule(IJobScheduler* pScheduler)
{
	assert(pScheduler);
	if (pScheduler)
	{
		BOOL l_bRet = pScheduler->Schedule(this);
		//assert(l_bRet);
	}
}

/**
���ô˷�������ҵ���붨ʱ���ȳ���

@param dwSecondDelay
�ӳٶ������ִ�д���ҵ
@param bRepeat
ֵΪFALSE, ����ҵ����һ�κ�, �����ȳ������ \ref Release ����ɾ��
ֵΪTRUE, ���ȳ���ÿ�� \ref dwSecondDelay �����һ�δ���ҵ
@param pScheduler
*/
template<class TState>
void CJobBase<TState>::ScheduleTimer(DWORD dwSecondDelay, BOOL bRepeat, IJobSchedulerTimer* pScheduler)
{
	assert(pScheduler);
	if (pScheduler)
	{
		BOOL l_bRet = pScheduler->ScheduleTimer(dwSecondDelay, bRepeat, this);
		//assert(l_bRet);
	}
}

/**
���ȳ�������ø÷���
*/
template<class TState>
void CJobBase<TState>::Execute()
{
	TState	l_State;
	TState* l_pState = m_spState.get();
	if (!l_pState)
	{
		l_pState = &l_State;
	}
	//�����ҵ���ڴ����߳������У���Ӧ������ͣ�ź�
	assert(!IsLocalRunSingleErr());
	//
	l_pState->Executing();
	//
	if (FALSE == l_pState->HasBeenTerminated())
	{
		InternalExecute();
	}
	l_pState->Done();
}

/**
�ͷ��Լ�(˭����˭�ͷ�ԭ��)
*/
template<class TState>
void CJobBase<TState>::Release()
{
	TState* l_pState = m_spState.get();
	if (l_pState)
	{
		l_pState->Terminate();
	}
	//
	assert(this);
	delete this;
}

/**
�����������ֹ��ҵִ��, �򷵻�TRUE
ע��: ��Ȼ����ֹ����, ������ҵ�п�������ִ����
*/
template<class TState>
BOOL CJobBase<TState>::HasBeenTerminated() const
{
	BOOL l_bRet = FALSE;
	TState* l_pState = m_spState.get();
	if (l_pState)
	{
		l_bRet = l_pState->HasBeenTerminated();
	}
	return l_bRet;
}

/**
�����������ͣ��ҵִ��, �򷵻�TRUE
ע��: ��Ȼ����ͣ����, ������ҵ�п�������ִ����
*/
template<class TState>
BOOL CJobBase<TState>::HasBeenSuspended() const
{
	BOOL l_bRet = FALSE;
	TState* l_pState = m_spState.get();
	if (l_pState)
	{
		l_bRet = l_pState->HasBeenSuspended();
	}
	return l_bRet;
}

/// base class is not intended for creation
template<class TState>
CJobBase<TState>::CJobBase(std::shared_ptr<TState>* spState, const char* strName)
	: m_nCreateJobThreadID(GetCurrentThreadId()), m_strName(strName ? strName : "")
{
	if (spState)
	{
		m_spState = *spState;
	}
}

/// asserts that the job is deletable
template<class TState>
CJobBase<TState>::~CJobBase(void)
{
	//nothing
}

/**
������ȳ���ִ�д���ҵ�����쳣, �����ø÷���֪ͨ����ҵ

@param pScheduler
*/
template<class TState>
void CJobBase<TState>::OnException()
{
	//nothing
}

}