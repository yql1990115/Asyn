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
除了处于JOB_DONE状态，其他状态在函数返回后都有可能发生变更
 
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
等待Job执行状态为 \ref JOB_DONE \ref eJobStatus

@param dwMilliSeconds
	超时时间
@return BOOL
	如果超时将返回FALSE
*/
BOOL CJobStateBase::WaitUntilDoneOrTimeout(DWORD dwMilliSeconds)
{
	BOOL l_bRet = m_executionDone.WaitForEndOrTimeout(dwMilliSeconds);
	return l_bRet;
}

/**
请求提前终止作业执行; 如果作业暂未调度执行，则阻止作业调度; Job执行状态为 \ref JOB_DONE
*/	
void CJobStateBase::Terminate()
{
	InterlockedExchange(&m_nTerminated, TRUE);
}

/**
如果已请求终止作业执行, 则返回TRUE
注意: 虽然有终止请求, 但是作业有可能仍在执行中
*/
BOOL CJobStateBase::HasBeenTerminated() const
{
	BOOL l_bRet = (TRUE == m_nTerminated);
	return l_bRet;
}

/**
请求暂停作业执行
*/	
void CJobStateBase::Stop()
{
	m_pause.Set();
}

/**
请求恢复作业执行
*/	
void CJobStateBase::Resume()
{
	m_pause.Reset();
}

/**
如果已请求暂停作业执行, 则返回TRUE
注意: 虽然有暂停请求, 但是作业有可能仍在执行中
*/
BOOL CJobStateBase::HasBeenSuspended() const
{
	BOOL l_bRet = m_pause.Test();
	return l_bRet;
}

/**
作业执行完后设置作业状态为done
*/
void CJobStateBase::Done()
{
	m_executionDone.Set();
}

/**
作业执行完后设置作业状态为running
*/
void CJobStateBase::Executing()
{
	InterlockedExchange(&m_nWaiting, FALSE);
}

}