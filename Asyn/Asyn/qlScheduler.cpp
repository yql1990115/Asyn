#include "stdafx.h"
#include "qlAsynHelper.h"
#include "qlScheduler.h"
#include <vector>

namespace asyn {

CqlScheduler::CqlScheduler(const char* strName) : m_thread(this, &CqlScheduler::OnProcess, &CqlScheduler::OnException)
{
	m_strName = strName ? strName : "";
	m_pause.Reset();
	m_HasNewJob.Reset();
	m_terminated.Reset();
}

CqlScheduler::~CqlScheduler()
{
	Shutdown();
	//m_thread.Shutdown();
}

/// ����������
const char* CqlScheduler::Name() const
{
	const char* l_strRet = m_strName.c_str();
	return l_strRet;
}

/**
�������ȳ���

@return BOOL
���ʧ�ܽ�����FALSE, ������Ϣ����ͨ��GetLastError()�������
*/
BOOL CqlScheduler::Startup()
{
	BOOL l_bRet = FALSE;
	//
	assert(FALSE == m_thread.HasAlive());
	assert(0 == m_ListJob.size());
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
void CqlScheduler::Shutdown(DWORD dwMilliSecondsTimeOut)
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
		for (; ; )
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
BOOL CqlScheduler::HasBeenTerminated() const
{
	BOOL l_bRet = m_terminated.Test();
	return l_bRet;
}

/**
������ͣ���ȳ���ִ��
*/
void CqlScheduler::Stop()
{
	m_pause.Set();
}

/**
����ָ����ȳ���ִ��
*/
void CqlScheduler::Resume()
{
	m_pause.Reset();
}

/**
�����������ͣ���ȳ���ִ��, �򷵻�TRUE
ע��: ��Ȼ����ͣ����, ������ҵ�п�������ִ����
*/
BOOL CqlScheduler::HasBeenSuspended() const
{
	BOOL l_bRet = m_pause.Test();
	return l_bRet;
}

/**
�ж�������ȳ����Ƿ񻹻���
*/
BOOL CqlScheduler::HasAlive() const
{
	BOOL l_bRet = m_thread.HasAlive();
	return l_bRet;
}

void CqlScheduler::NotifyExit()
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
BOOL CqlScheduler::Schedule(IJob* pJob)
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
unsigned int CqlScheduler::OnProcess()
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

long CqlScheduler::OnException(struct _EXCEPTION_POINTERS * pInfo)
{
	//assert(0);	//�쳣�ж��в�ʹ�ö���
	long l_nRet = qlExceptionFilter(pInfo, nullptr, m_strName.c_str());
	return l_nRet;	//EXCEPTION_EXECUTE_HANDLER
}

/// �Ƴ���ҵ����
void CqlScheduler::RemoveListJob()
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
void CqlScheduler::RemoveListJob(LIST_JOB& listJob)
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
void CqlScheduler::OnProcessJob(IJob* pJob)
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

BOOL CqlScheduler::SignalHandle()
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

std::string CqlScheduler::Format(const char *strFmt, ...)
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


CqlSchedulerPool::CqlSchedulerPool(unsigned long nPoolSize, const char* strName) : CqlScheduler(strName), m_nPoolSize(nPoolSize)
{
	assert(sizeof(long) >= 4);
	m_ptSlave = new SlaveInfo_t[(m_nPoolSize / s_SlavePageSize) + 1];
}

CqlSchedulerPool::~CqlSchedulerPool()
{
	Shutdown();
	if (m_ptSlave)
	{
		delete[] m_ptSlave;
		m_ptSlave = NULL;
	}
}

BOOL CqlSchedulerPool::Startup()
{
	BOOL l_bRet = FALSE;
	//
	assert(FALSE == m_thread.HasAlive());
	assert(0 == m_ListJob.size());
	//
	Shutdown();
	//
	if ( NULL == m_ptSlave )
	{
		m_ptSlave = new SlaveInfo_t[(m_nPoolSize / s_SlavePageSize) + 1];
	}
	//
	if ( m_ptSlave )
	{
		m_pause.Reset();
		m_HasNewJob.Reset();
		m_terminated.Reset();
		//
		for (unsigned long i = 0; i < (m_nPoolSize / s_SlavePageSize) + 1; i++)
		{
			m_ptSlave[i].SlaveInfo = 0;
		}
		//
		int l_nIndex0 = 0;
		int l_nIndex1 = 0;
		for (unsigned long i = 0; i < m_nPoolSize; i++)
		{
			l_nIndex0 = i / s_SlavePageSize;
			l_nIndex1 = i % s_SlavePageSize;
			m_ptSlave[l_nIndex0].acScheduler[l_nIndex1].Startup(&m_ptSlave[l_nIndex0].SlaveInfo, l_nIndex1, &m_HasNewThread, Format("CJobSchedulerSlave_%d", i).c_str());
		}
		//
		l_bRet = m_thread.Startup();
	}
	return l_bRet;
}

void CqlSchedulerPool::Shutdown(DWORD dwMilliSecondsTimeOut)
{
	NotifyExit();
	//
	CqlScheduler::Shutdown(dwMilliSecondsTimeOut);
	//
	int l_nIndex0 = 0;
	int l_nIndex1 = 0;
	for (unsigned long i = 0; i < m_nPoolSize; i++)
	{
		l_nIndex0 = i / s_SlavePageSize;
		l_nIndex1 = i % s_SlavePageSize;
		m_ptSlave[l_nIndex0].acScheduler[l_nIndex1].Shutdown(dwMilliSecondsTimeOut);
	}
}

/**
������ͣ���ȳ���ִ��
*/
void CqlSchedulerPool::Stop()
{
	int l_nIndex0 = 0;
	int l_nIndex1 = 0;
	for (unsigned long i = 0; i < m_nPoolSize; i++)
	{
		l_nIndex0 = i / s_SlavePageSize;
		l_nIndex1 = i % s_SlavePageSize;
		m_ptSlave[l_nIndex0].acScheduler[l_nIndex1].Stop();
	}
	//
	CqlScheduler::Stop();
}

/**
����ָ����ȳ���ִ��
*/
void CqlSchedulerPool::Resume()
{
	int l_nIndex0 = 0;
	int l_nIndex1 = 0;
	for (unsigned long i = 0; i < m_nPoolSize; i++)
	{
		l_nIndex0 = i / s_SlavePageSize;
		l_nIndex1 = i % s_SlavePageSize;
		m_ptSlave[l_nIndex0].acScheduler[l_nIndex1].Resume();
	}
	//
	CqlScheduler::Resume();
}

/// �������˳��źţ�����������ҵ�¼�
void CqlSchedulerPool::NotifyExit()
{
	int l_nIndex0 = 0;
	int l_nIndex1 = 0;
	for (unsigned long i = 0; i < m_nPoolSize; i++)
	{
		l_nIndex0 = i / s_SlavePageSize;
		l_nIndex1 = i % s_SlavePageSize;
		m_ptSlave[l_nIndex0].acScheduler[l_nIndex1].NotifyExit();
	}
	//
	CqlScheduler::NotifyExit();
	m_HasNewThread.Set();
}

unsigned int CqlSchedulerPool::GetSlaveSize() const
{
	return m_nPoolSize;
}
///
IScheduler* CqlSchedulerPool::GetSlaveScheduler(unsigned int nIndex) const
{
	IScheduler* l_pRet = nullptr;
	if (nIndex >= 0 && nIndex < m_nPoolSize)
	{
		int l_nIndex0 = nIndex / s_SlavePageSize;
		int l_nIndex1 = nIndex % s_SlavePageSize;
		//
		l_pRet = &m_ptSlave[l_nIndex0].acScheduler[l_nIndex1];
	}
	return l_pRet;
}

/// ������ Ĭ�Ϸ���0 ���ִ���1 ����2
unsigned int CqlSchedulerPool::OnProcess()
{
	unsigned int l_nRet = 0;
	//
	BOOL l_bExit = FALSE;
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
			if (!m_HasNewThread.WaitForEndOrTimeout(INFINITE))
			{
				l_nRet = 1;
				goto tagExit;
			}
			//
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
					(*l_it) = nullptr;
					//
					if (!HasNewThread())
					{
						if (!m_HasNewThread.WaitForEndOrTimeout(INFINITE))
						{
							l_nRet = 1;
							goto tagExit;
						}
					}
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

/// �Ƴ���ҵ����
void CqlSchedulerPool::RemoveListJob(LIST_JOB& listJob)
{
	//�����ҵת����Slave, ��l_pJob����Ϊ��ָ��; ���������ָ�����
	for (auto it = listJob.begin(); it != listJob.end(); it++)
	{
		IJob* l_pJob = (*it);
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
void CqlSchedulerPool::OnProcessJob(IJob* pJob)
{
	int l_nIndex0 = -1;
	int l_nIndex1 = -1;
	for (unsigned long i = 0; i < m_nPoolSize; i++)
	{
		l_nIndex0 = i / s_SlavePageSize;
		l_nIndex1 = i % s_SlavePageSize;
		//
		long l_nMask = 1 << l_nIndex1;
		if (m_ptSlave[l_nIndex0].SlaveInfo & l_nMask)
		{
			unsigned long l_nRet = InterlockedExchangeAdd(&m_ptSlave[l_nIndex0].SlaveInfo, -l_nMask);
			assert(l_nRet & l_nMask);
			assert(0 == (m_ptSlave[l_nIndex0].SlaveInfo & l_nMask));
			break;
		}
	}
	//
	//OutputDebugStringA(Format("slave %d process job %d\r\n", l_nIndex1, GetTickCount()).c_str());
	assert(-1 != l_nIndex0);
	assert(-1 != l_nIndex1);
	if ((l_nIndex0 != -1) && (l_nIndex1 != -1))
	{
		m_ptSlave[l_nIndex0].acScheduler[l_nIndex1].Schedule(pJob);
	}
	else
	{
		if (pJob)
		{
			pJob->Release();
			pJob = NULL;
		}
	}
}

/// ȷ���Ƿ���δʹ���߳�
BOOL CqlSchedulerPool::HasNewThread()
{
	BOOL l_bHasSlaveThread = FALSE;
	for (unsigned long i = 0; i < (m_nPoolSize / s_SlavePageSize) + 1; i++)
	{
		if (m_ptSlave[i].SlaveInfo)
		{
			l_bHasSlaveThread = TRUE;
			break;
		}
	}
	//
	if (!l_bHasSlaveThread)
	{
		m_HasNewThread.Reset();
		//
		for (unsigned long i = 0; i < (m_nPoolSize / s_SlavePageSize) + 1; i++)
		{
			if (m_ptSlave[i].SlaveInfo)
			{
				l_bHasSlaveThread = TRUE;
				break;
			}
		}
		//
		if (l_bHasSlaveThread)
		{
			m_HasNewThread.Set();
		}
	}
	return l_bHasSlaveThread;
}

CqlSchedulerPool::CqlSchedulerSlave::CqlSchedulerSlave(const char* strName)
	: CqlScheduler(strName)
{
	m_pnSlaveInfo = nullptr;
	m_nSlaveIndex = 0;
	m_pNotifyEvent = nullptr;
}

/**
�������ȳ���   ���̵߳���

@return BOOL
���ʧ�ܽ�����FALSE, ������Ϣ����ͨ��GetLastError()�������
*/
BOOL CqlSchedulerPool::CqlSchedulerSlave::Startup(unsigned long volatile* pnSlaveInfo, unsigned char nSlaveIndex, CWaitableEvent* pNotifyEvent, const char* strName)
{
	BOOL l_bRet = FALSE;
	//
	assert(pnSlaveInfo);
	assert(pNotifyEvent);
	assert(strName);
	//
	m_pnSlaveInfo = pnSlaveInfo;
	m_nSlaveIndex = nSlaveIndex;
	m_pNotifyEvent = pNotifyEvent;
	m_strName = strName ? strName : "";
	//
	l_bRet = CqlScheduler::Startup();
	return l_bRet;
}

/// ������ Ĭ�Ϸ���0 ���ִ���1 ����2
unsigned int CqlSchedulerPool::CqlSchedulerSlave::OnProcess()
{
	assert(m_pnSlaveInfo);
	assert(m_pNotifyEvent);
	//
	unsigned int l_nRet = 0;
	LIST_JOB l_listJob;
	//
	if ((nullptr == m_pnSlaveInfo) || (nullptr == m_pNotifyEvent))
	{
		l_nRet = 1;
		goto tagExit;
	}
	//
	//notify master thread
	long l_nMask = 1 << m_nSlaveIndex;
	unsigned long l_nSlaveInfo = InterlockedExchangeAdd(m_pnSlaveInfo, l_nMask);
	m_pNotifyEvent->Set();
	//
	for (;;)
	{
		if (m_HasNewJob.WaitForEndOrTimeout(INFINITE))
		{
			//OutputDebugStringA(Format("Enter slave %d %d\r\n", m_nSlaveIndex, GetTickCount()).c_str());
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
			//
			//notify master thread
			l_nSlaveInfo = InterlockedExchangeAdd(m_pnSlaveInfo, l_nMask);
			m_pNotifyEvent->Set();
			//OutputDebugStringA(Format("Leave slave %d %d\r\n", m_nSlaveIndex, GetTickCount()).c_str());
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

}