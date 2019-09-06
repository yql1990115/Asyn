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
// 导出函数
//////////////////////////////////////////////////////////////////////////
*/
extern ASYN_API asyn::IScheduler* WINAPI QL_Create_Scheduler(const char* strName);

extern ASYN_API asyn::IScheduler* WINAPI QL_Create_SchedulerPool(unsigned long nSize, const char* strName);

extern ASYN_API asyn::ISchedulerTimer* WINAPI QL_Create_SchedulerTimerWindow(const char* strName);

extern ASYN_API void WINAPI QL_Destroy_Scheduler(asyn::IScheduler* pScheduler, DWORD dwMilliSecondsTimeOut = 30000);

extern ASYN_API void WINAPI QL_Destroy_SchedulerPool(asyn::IScheduler* pScheduler, DWORD dwMilliSecondsTimeOut = 30000);

extern ASYN_API void WINAPI QL_Destroy_SchedulerTimerWindow(asyn::ISchedulerTimer* pScheduler, DWORD dwMilliSecondsTimeOut = 30000);

/**
生成dump文件
\ref hFile 和 \ref strDumpName不存在, 则使用默认名生成dump文件
如果检测到有调试器, 则返回值置为EXCEPTION_CONTINUE_SEARCH, 交给调试器

@param pExceptionInfo
通过GetExceptionInformation()获取的异常信息
@param hFile
如果句柄存在, 则dump信息写入这个句柄指向的文件; 句柄存在则忽略strDumpName参数
@param strDumpName
如果 \ref hFile 不存在, 则函数内部将使用strDumpName创建一个文件句柄并使用

@return long
#define EXCEPTION_EXECUTE_HANDLER       1
#define EXCEPTION_CONTINUE_SEARCH       0
#define EXCEPTION_CONTINUE_EXECUTION    -1
*/
extern ASYN_API long WINAPI qlExceptionFilter(PEXCEPTION_POINTERS pExceptionInfo, HANDLE hFile = nullptr, const char* strDumpName = nullptr);

/**
生成dump文件
\ref hFile 和 \ref strDumpName不存在, 则使用默认名生成dump文件

@param pExceptionInfo
通过GetExceptionInformation()获取的异常信息
@param hFile
如果句柄存在, 则dump信息写入这个句柄指向的文件; 句柄存在则忽略strDumpName参数
@param strDumpName
如果 \ref hFile 不存在, 则函数内部将使用strDumpName创建一个文件句柄并使用

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
// 使用导出函数封装的类, 向外提供调度器/调度器池/定时调度器
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