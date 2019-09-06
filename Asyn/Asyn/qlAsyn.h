/*
===========================================================================

Copyright (C) 2019 by yql1990115
yql1990115@gmail.com
www.yql1990115.com

===========================================================================
*/

#ifndef __ASYN_H__
#define __ASYN_H__

#include "qlAsynInterface.h"

#ifdef __cplusplus
extern "C" {
#endif /*!__cplusplus*/

#ifdef ASYN_EXPORTS
#define ASYN_API __declspec(dllexport)
#else
#define ASYN_API __declspec(dllimport)
#endif

/*
//////////////////////////////////////////////////////////////////////////
// ��������
//////////////////////////////////////////////////////////////////////////
*/
extern ASYN_API asyn::IScheduler* WINAPI QL_Create_Scheduler(const char* strName);

extern ASYN_API asyn::IScheduler* WINAPI QL_Create_SchedulerPool(unsigned long nSize, const char* strName);

extern ASYN_API asyn::ISchedulerTimer* WINAPI QL_Create_SchedulerTimerWindow(const char* strName);

extern ASYN_API void WINAPI QL_Destroy_Scheduler(asyn::IScheduler* pScheduler, DWORD dwMilliSecondsTimeOut = 30000);

extern ASYN_API void WINAPI QL_Destroy_SchedulerPool(asyn::IScheduler* pScheduler, DWORD dwMilliSecondsTimeOut = 30000);

extern ASYN_API void WINAPI QL_Destroy_SchedulerTimerWindow(asyn::ISchedulerTimer* pScheduler, DWORD dwMilliSecondsTimeOut = 30000);

/**
����dump�ļ�
\ref hFile �� \ref strDumpName������, ��ʹ��Ĭ��������dump�ļ�
�����⵽�е�����, �򷵻�ֵ��ΪEXCEPTION_CONTINUE_SEARCH, ����������

@param pExceptionInfo
ͨ��GetExceptionInformation()��ȡ���쳣��Ϣ
@param hFile
����������, ��dump��Ϣд��������ָ����ļ�; ������������strDumpName����
@param strDumpName
��� \ref hFile ������, �����ڲ���ʹ��strDumpName����һ���ļ������ʹ��

@return long
#define EXCEPTION_EXECUTE_HANDLER       1
#define EXCEPTION_CONTINUE_SEARCH       0
#define EXCEPTION_CONTINUE_EXECUTION    -1
*/
extern ASYN_API long WINAPI qlExceptionFilter(PEXCEPTION_POINTERS pExceptionInfo, HANDLE hFile = nullptr, const char* strDumpName = nullptr);

/**
����dump�ļ�
\ref hFile �� \ref strDumpName������, ��ʹ��Ĭ��������dump�ļ�

@param pExceptionInfo
ͨ��GetExceptionInformation()��ȡ���쳣��Ϣ
@param hFile
����������, ��dump��Ϣд��������ָ����ļ�; ������������strDumpName����
@param strDumpName
��� \ref hFile ������, �����ڲ���ʹ��strDumpName����һ���ļ������ʹ��

@return long
#define EXCEPTION_EXECUTE_HANDLER       1
#define EXCEPTION_CONTINUE_SEARCH       0
#define EXCEPTION_CONTINUE_EXECUTION    -1
*/
extern ASYN_API long WINAPI qlGenerateMiniDump(PEXCEPTION_POINTERS pExceptionInfo, HANDLE hFile = nullptr, const char* strDumpName = nullptr);

#ifdef __cplusplus
}
#endif /*!__cplusplus*/

namespace asyn {

/*
//////////////////////////////////////////////////////////////////////////
// ʹ�õ���������װ����, �����ṩ������/��������/��ʱ������
//////////////////////////////////////////////////////////////////////////
*/
class CScheduler
{
public:
	BOOL IsValid()				const { return m_pObj != nullptr; }
	IScheduler* Get()			const { return m_pObj; }
	IScheduler* operator ->	()	const { return m_pObj; }
	operator IScheduler*	()	const { return m_pObj; }

public:
	explicit CScheduler(const char* strName, DWORD dwMilliSecondsTimeOut = 30000)
	{
		m_pObj = QL_Create_Scheduler(strName);
		m_dwMilliSecondsTimeOut = dwMilliSecondsTimeOut;
	}

	virtual ~CScheduler()
	{
		QL_Destroy_Scheduler(m_pObj, m_dwMilliSecondsTimeOut);
	}

private:
	CScheduler(const CScheduler&);
	CScheduler& operator = (const CScheduler&);
	CScheduler& operator = (IScheduler* pObj);

protected:
	IScheduler*	m_pObj;
	DWORD		m_dwMilliSecondsTimeOut;
};

class CSchedulerPool
{
public:
	BOOL IsValid()				const { return m_pObj != nullptr; }
	IScheduler* Get()			const { return m_pObj; }
	IScheduler* operator ->	()	const { return m_pObj; }
	operator IScheduler*	()	const { return m_pObj; }

public:
	CSchedulerPool(unsigned long nSize, const char* strName, DWORD dwMilliSecondsTimeOut = 30000)
	{
		m_pObj = QL_Create_SchedulerPool(nSize, strName);
		m_dwMilliSecondsTimeOut = dwMilliSecondsTimeOut;
	}

	virtual ~CSchedulerPool()
	{
		QL_Destroy_SchedulerPool(m_pObj, m_dwMilliSecondsTimeOut);
	}

private:
	CSchedulerPool(const CSchedulerPool&);
	CSchedulerPool& operator = (const CSchedulerPool&);
	CSchedulerPool& operator = (IScheduler* pObj);

protected:
	IScheduler*	m_pObj;
	DWORD		m_dwMilliSecondsTimeOut;
};

class CSchedulerTimer
{
public:
	BOOL IsValid()					const { return m_pObj != nullptr; }
	ISchedulerTimer* Get()			const { return m_pObj; }
	ISchedulerTimer* operator ->()	const { return m_pObj; }
	operator ISchedulerTimer*	()	const { return m_pObj; }

public:
	explicit CSchedulerTimer(const char* strName, DWORD dwMilliSecondsTimeOut = 30000)
	{
		m_pObj = QL_Create_SchedulerTimerWindow(strName);
		m_dwMilliSecondsTimeOut = dwMilliSecondsTimeOut;
	}

	virtual ~CSchedulerTimer()
	{
		QL_Destroy_SchedulerTimerWindow(m_pObj, m_dwMilliSecondsTimeOut);
	}

private:
	CSchedulerTimer(const CSchedulerPool&);
	CSchedulerTimer& operator = (const CSchedulerTimer&);
	CSchedulerTimer& operator = (ISchedulerTimer* pObj);

protected:
	ISchedulerTimer*	m_pObj;
	DWORD		m_dwMilliSecondsTimeOut;
};

}

#endif /*!__ASYN_H__*/