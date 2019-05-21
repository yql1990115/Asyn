/***************************************************************************
*   Copyright (C) 2019 by yql1990115                                      *
*   yql1990115@gmail.com                                                  *
*                                                                         *
*   www.yql1990115.com                                                    *
***************************************************************************/

#include "stdafx.h"
#include "JobStateBase.h"
#include <WinBase.h>
#include <winnt.h>

namespace async{

CJobStateBase::CJobStateBase()
{
	m_nWaiting = TRUE;
	m_nTerminated = FALSE;
}

/**
���˴���JOB_DONE״̬������״̬�ں������غ��п��ܷ������
 
@return eJobStatus
	\ref eJobStatus
*/
long CJobStateBase::GetStatus() const
{
	long l_nRet = JOB_WAITING;
	if (m_nWaiting != TRUE)
	{
		l_nRet = m_executionDone.Test() ? JOB_DONE : JOB_RUNNING;
		if (JOB_RUNNING == l_nRet)
		{
			l_nRet = m_pause.Test() ? JOB_SUSPENDED : JOB_RUNNING;
		}
	}
	return l_nRet;
}

/**
�ȴ�Jobִ��״̬Ϊ \ref JOB_DONE \ref eJobStatus

@param dwMilliSeconds
	��ʱʱ��
@return BOOL
	�����ʱ������FALSE
*/
BOOL CJobStateBase::WaitUntilDoneOrTimeout(DWORD dwMilliSeconds)
{
	BOOL l_bRet = m_executionDone.WaitForEndOrTimeout(dwMilliSeconds);
	return l_bRet;
}

/**
������ǰ��ֹ��ҵִ��; �����ҵ��δ����ִ�У�����ֹ��ҵ����; Jobִ��״̬Ϊ \ref JOB_DONE
*/	
void CJobStateBase::Terminate()
{
	InterlockedExchange(&m_nTerminated, TRUE);
}

/**
�����������ֹ��ҵִ��, �򷵻�TRUE
ע��: ��Ȼ����ֹ����, ������ҵ�п�������ִ����
*/
BOOL CJobStateBase::HasBeenTerminated() const
{
	BOOL l_bRet = (TRUE == m_nTerminated);
	return l_bRet;
}

/**
������ͣ��ҵִ��
*/	
void CJobStateBase::Stop()
{
	m_pause.Set();
}

/**
����ָ���ҵִ��
*/	
void CJobStateBase::Resume()
{
	m_pause.Reset();
}

/**
�����������ͣ��ҵִ��, �򷵻�TRUE
ע��: ��Ȼ����ͣ����, ������ҵ�п�������ִ����
*/
BOOL CJobStateBase::HasBeenSuspended() const
{
	BOOL l_bRet = m_pause.Test();
	return l_bRet;
}

/**
��ҵִ�����������ҵ״̬Ϊdone
*/
void CJobStateBase::Done()
{
	m_executionDone.Set();
}

/**
��ҵִ�����������ҵ״̬Ϊrunning
*/
void CJobStateBase::Executing()
{
	InterlockedExchange(&m_nWaiting, FALSE);
}

}