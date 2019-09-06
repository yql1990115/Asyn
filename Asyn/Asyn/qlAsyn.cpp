#include "stdafx.h"
#include "qlAsyn.h"
#include "qlAsynHelper.h"
#include "qlScheduler.h"
#include "qlSchedulerTimer.h"

ASYN_API asyn::IScheduler* WINAPI QL_Create_Scheduler(const char* strName)
{
	asyn::IScheduler* l_pScheduler = new asyn::CqlScheduler(strName);
	return l_pScheduler;
}

ASYN_API asyn::IScheduler* WINAPI QL_Create_SchedulerPool(unsigned long nSize, const char* strName)
{
	asyn::IScheduler* l_pScheduler = NULL;
	l_pScheduler = (nSize <= 1) ? (new asyn::CqlScheduler(strName)) : (new asyn::CqlSchedulerPool(nSize, strName));
	return l_pScheduler;
}

ASYN_API asyn::ISchedulerTimer* WINAPI QL_Create_SchedulerTimerWindow(const char* strName)
{
	asyn::ISchedulerTimer* l_pScheduler = new asyn::CqlSchedulerTimer(strName);
	return l_pScheduler;
}

ASYN_API void WINAPI QL_Destroy_Scheduler(asyn::IScheduler* pScheduler, DWORD dwMilliSecondsTimeOut)
{
	if (pScheduler)
	{
		pScheduler->Shutdown(dwMilliSecondsTimeOut);
		delete pScheduler;
		pScheduler = NULL;
	}
}

ASYN_API void WINAPI QL_Destroy_SchedulerPool(asyn::IScheduler* pScheduler, DWORD dwMilliSecondsTimeOut)
{
	if (pScheduler)
	{
		pScheduler->Shutdown(dwMilliSecondsTimeOut);
		delete pScheduler;
		pScheduler = NULL;
	}
}

ASYN_API void WINAPI QL_Destroy_SchedulerTimerWindow(asyn::ISchedulerTimer* pScheduler, DWORD dwMilliSecondsTimeOut)
{
	if (pScheduler)
	{
		pScheduler->Shutdown(dwMilliSecondsTimeOut);
		delete pScheduler;
		pScheduler = NULL;
	}
}

