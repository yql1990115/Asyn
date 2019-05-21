/***************************************************************************
*   Copyright (C) 2019 by yql1990115                                      *
*   yql1990115@gmail.com                                                  *
*                                                                         *
*   www.yql1990115.com                                                    *
***************************************************************************/

#include "stdafx.h"
#include "MiniDump.h"
#include "JobSchedulerBase.h"
#include <WinBase.h>
#include <winnt.h>
#include <assert.h>
#include <vector>

namespace async{

CJobSchedulerBase::CJobSchedulerBase(const char* strName) : m_thread(this, &CJobSchedulerBase::OnProcess, &CJobSchedulerBase::OnException)
{
	m_strName = strName ? strName : "";
	m_pause.Reset();
	m_HasNewJob.Reset();
	m_terminated.Reset();
}

CJobSchedulerBase::~CJobSchedulerBase()
{
	Shutdown();
	//m_thread.Shutdown();
}

/// ����������
const char* CJobSchedulerBase::Name() const
{
	const char* l_strRet = m_strName.c_str();
	return l_strRet;
}

/**
�������ȳ���

@return BOOL
	���ʧ�ܽ�����FALSE, ������Ϣ����ͨ��GetLastError()�������
*/
BOOL CJobSchedulerBase::Startup()
{
	BOOL l_bRet = FALSE;
	//
	assert( FALSE == m_thread.HasAlive() );
	assert( 0 == m_ListJob.size() );
	//
	Shutdown();
	//
	m_pause.Reset();
	m_HasNewJob.Reset();
	m_terminated.Reset();
	//
	l_bRet = m_thread.Startup();
	return l_bRet;
}

/**
�رյ��ȳ���, �ͷ����е�����ҵ

@param dwMilliSeconds
	�ȴ���������ҵ��ʱ��, ��ǿ�ƹر�������ҵ
*/
void CJobSchedulerBase::Shutdown(DWORD dwMilliSecondsTimeOut)
{
	if (dwMilliSecondsTimeOut < 200)
	{
		dwMilliSecondsTimeOut = 200;
	}
	//
	if (m_thread.HasAlive())
	{
		NotifyExit();
		::Sleep(1);
		//
		BOOL l_bExitFlag = FALSE;
		DWORD l_dwStart = GetTickCount();
		HANDLE l_handle = m_thread.GetThreadHandle();
		for ( ; ; )
		{
			l_bExitFlag = !m_thread.HasAlive();
			DWORD l_dwRet = MsgWaitForMultipleObjects(1, &l_handle, TRUE, 100, QS_ALLINPUT);
			switch (l_dwRet)
			{
			case WAIT_OBJECT_0:
				l_bExitFlag = TRUE;
				break;
			case WAIT_OBJECT_0 + 1:
			{
				MSG l_msg = { 0 };
				if (::PeekMessage(&l_msg, NULL, 0, 0, PM_REMOVE))
				{
					::TranslateMessage(&l_msg);
					::DispatchMessage(&l_msg);
				}
			}
				break;
			case WAIT_TIMEOUT:
				break;
			default:
				l_bExitFlag = TRUE;
				break;
			}
			//
			if (l_bExitFlag)
			{
				break;
			}
			//
			DWORD l_dwEnd = GetTickCount();
			if ((l_dwEnd - l_dwStart) > dwMilliSecondsTimeOut)
			{//��ʱ�˳��ǲ�Ӧ�õ�
				assert(0);
				l_bExitFlag = TRUE;
			}
		}//end for
		//
		m_thread.Shutdown();
	}
	//
	RemoveListJob();
}

/**
�����������ֹ���ȳ���, �򷵻�TRUE
ע��: ��Ȼ����ֹ����, ���ǵ��ȳ����������ִ����, ֱ���ȴ���ʱ
*/
BOOL CJobSchedulerBase::HasBeenTerminated() const
{
	BOOL l_bRet = m_terminated.Test();
	return l_bRet;
}

/**
������ͣ���ȳ���ִ��
*/	
void CJobSchedulerBase::Stop()
{
	m_pause.Set();
}

/**
����ָ����ȳ���ִ��
*/	
void CJobSchedulerBase::Resume()
{
	m_pause.Reset();
}

/**
�����������ͣ���ȳ���ִ��, �򷵻�TRUE
ע��: ��Ȼ����ͣ����, ������ҵ�п�������ִ����
*/
BOOL CJobSchedulerBase::HasBeenSuspended() const
{
	BOOL l_bRet = m_pause.Test();
	return l_bRet;
}

/**
�ж�������ȳ����Ƿ񻹻���
*/
BOOL CJobSchedulerBase::HasAlive() const
{
	BOOL l_bRet = m_thread.HasAlive();
	return l_bRet;
}

void CJobSchedulerBase::NotifyExit()
{
	m_terminated.Set();
	m_HasNewJob.Set();
	m_pause.Reset();
}

/**
����ҵ������ȳ�����

@param pJob
	��ҵ
	
@return BOOL	
	�����ҵ������ȳ���ʧ��, ��������ҵ�� \ref Release() �����ͷ���ҵ������FALSE
*/
BOOL CJobSchedulerBase::Schedule(IJob* pJob)
{
	assert(pJob);
	//
	BOOL l_bSchedule = FALSE;
	if (pJob)
	{
		l_bSchedule = !m_terminated.Test();
		if (l_bSchedule)
		{
			CCriticalSectionLock l_lock(m_ListJobLock);
			l_bSchedule = !m_terminated.Test();
			if (l_bSchedule)
			{
				m_ListJob.push_back(pJob);
				m_HasNewJob.Set();
			}
		}
		//
		if (!l_bSchedule)
		{
			assert(pJob);
			pJob->Release();
			pJob = NULL;
		}
	}
	//
	return l_bSchedule;
}

/// ������ 
unsigned int CJobSchedulerBase::OnProcess()
{
	unsigned int l_nRet = 0;
	//
	LIST_JOB l_listJob;
	for (;;)
	{
		if (m_HasNewJob.WaitForEndOrTimeout(INFINITE))
		{
			if (SignalHandle())
			{
				goto tagExit;
			}
			//
			{
				CCriticalSectionLock l_lock(m_ListJobLock);
				l_listJob.swap(m_ListJob);
				m_HasNewJob.Reset();
			}
			//
			BOOL l_bExit = FALSE;
			for (LIST_JOB::iterator l_it = l_listJob.begin(); l_it != l_listJob.end(); l_it++)
			{
				if (l_bExit = SignalHandle())
				{
					goto tagExit;
				}
				IJob* l_pJob = NULL;
				l_pJob = (*l_it);
				assert(l_pJob);
				if (l_pJob)
				{
					OnProcessJob(l_pJob);
				}
			}//end for
			//
			RemoveListJob(l_listJob);
		}
		else
		{
			l_nRet = 1;
			break;
		}//end if (m_HasNewJob.WaitForEndOrTimeout(INFINITE))
	}//end for

tagExit:
	RemoveListJob(l_listJob);
	return l_nRet;
}

long CJobSchedulerBase::OnException(struct _EXCEPTION_POINTERS * pInfo)
{
	//assert(0);	//�쳣�ж��в�ʹ�ö���
	long l_nRet = ExceptionFilter(pInfo, nullptr, m_strName.c_str());
	return l_nRet;	//EXCEPTION_EXECUTE_HANDLER
}

/// �Ƴ���ҵ����
void CJobSchedulerBase::RemoveListJob()
{
	LIST_JOB l_listJob;
	//
	{
		CCriticalSectionLock l_lock(m_ListJobLock);
		l_listJob.swap(m_ListJob);
	}
	//
	RemoveListJob(l_listJob);
}

/// �Ƴ���ҵ����
void CJobSchedulerBase::RemoveListJob(LIST_JOB& listJob)
{
	for (auto it = listJob.begin(); it != listJob.end(); it++)
	{
		IJob* l_pJob = (*it);
		assert(l_pJob);
		if (l_pJob)
		{
			l_pJob->Release();
			l_pJob = nullptr;
		}
	}//end for
	//
	listJob.clear();
}

/// ������ҵ
void CJobSchedulerBase::OnProcessJob(IJob* pJob)
{
	assert(pJob);
	DWORD l_dwEx = 0;
	__try
	{
		pJob->Execute();
	}
	__except (l_dwEx = GetExceptionCode(), OnException(GetExceptionInformation()))
	{
		pJob->OnException();
	}
}

BOOL CJobSchedulerBase::SignalHandle()
{
	BOOL l_bExitRet = FALSE;
	//
	BOOL l_bExit = FALSE;
	BOOL l_bPause = FALSE;
	BOOL l_bDoPause = FALSE;
	//
	for (;;)
	{
		l_bExit = m_terminated.Test();
		l_bPause = m_pause.Test();
		//
		if (l_bExit)
		{
			OnTerminated();
			l_bExitRet = TRUE;
			break;
		}
		//
		if (l_bPause != l_bDoPause)
		{
			if (l_bPause)
			{
				OnPause();
			}
			else
			{
				OnResume();
			}
			l_bDoPause = l_bPause;
		}
		//
		if (FALSE == l_bDoPause && FALSE == l_bPause)
		{
			break;
		}
		//
		::Sleep(100);
	}
	//
	return l_bExitRet;
}

std::string CJobSchedulerBase::Format(const char *strFmt, ...)
{
	std::string l_strRet = "";
	if (NULL != strFmt)
	{
		va_list l_marker = NULL;
		va_start(l_marker, strFmt);									//��ʼ���������� 
		size_t l_nLength = _vscprintf(strFmt, l_marker) + 1;		//��ȡ��ʽ���ַ�������
		std::vector<char> l_vBuffer(l_nLength, '\0');				//�������ڴ洢��ʽ���ַ������ַ�����
		int l_nWritten = _vsnprintf_s(&l_vBuffer[0], l_vBuffer.size(), l_nLength, strFmt, l_marker);
		if (l_nWritten > 0)
		{
			l_strRet = &l_vBuffer[0];
		}
		va_end(l_marker);                                    //���ñ�������
	}
	return l_strRet;
}

//CJobSchedulerBase g_Base;
//CThreadSimple<CJobSchedulerBase>	m_thread(&g_Base, &CJobSchedulerBase::OnProcess);

}