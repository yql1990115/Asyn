// Asyn.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "IJob.h"
#include "IJobState.h"
#include "IJobScheduler.h"
#include "IJobSchedulerTimer.h"
#include "CriticalSection.h"
#include "WaitableEvent.h"
#include "ThreadSimple.h"
#include "JobBase.h"
#include "JobStateBase.h"
#include "JobSchedulerBase.h"
#include "JobSchedulerTimerBase.h"
#include "JobSchedulerPool.h"
#include "AsynCall.h"
#include "AsynCallEx.h"
#include "AsynCall4W.h"


class CTest
{
public:
	CTest(){ m_c = 0; };
	virtual ~CTest(){};

public:
	void Add0()
	{
		m_c = 1;
	};
	//
	void Add1(int a)
	{
		m_c = a;
	};
	//
	void Add2(int a, int b)
	{
		m_c = a + b;
	};
	//
	void Add3(int a, int b, int c)
	{
		m_c = a + b + c;
	};
	//
	void Add4(int a, int b, int c, int d)
	{
		m_c = a + b + c + d;
	};
	//
	void Add5(int a, int b, int c, int d, int e)
	{
		m_c = a + b + c + d + e;
	};

protected:
	int m_c;
};

class CTestEx
{
public:
	CTestEx(){ m_c = 0; };
	virtual ~CTestEx(){};

public:
	void Add0(async::IJobState* pState)
	{
		m_c = 1;
	};
	//
	void Add1(int a, async::IJobState* pState)
	{
		m_c = a;
	};
	//
	void Add2(int a, int b, async::IJobState* pState)
	{
		m_c = a + b;
	};
	//
	void Add3(int a, int b, int c, async::IJobState* pState)
	{
		m_c = a + b + c;
	};
	//
	void Add4(int a, int b, int c, int d, async::IJobState* pState)
	{
		m_c = a + b + c + d;
	};
	//
	void Add5(int a, int b, int c, int d, int e, async::IJobState* pState)
	{
		Sleep(10000);
		m_c = a + b + c + d + e;
	};

protected:
	int m_c;
};

class CTestRet
{
public:
	CTestRet(){ };
	virtual ~CTestRet(){};

public:
	int Add0(async::IJobState* pState)
	{
		return 1;
	};
	//
	int Add1(int a, async::IJobState* spState)
	{
		return a;
	};
	//
	int Add2(int a, int b, async::IJobState* spState)
	{
		return (a + b);
	};
	//
	int Add3(int a, int b, int c, async::IJobState* spState)
	{
		return (a + b + c);
	};
	//
	int Add4(int a, int b, int c, int d, async::IJobState* spState)
	{
		return (a + b + c + d);
	};
	//
	int Add5(int a, int b, int c, int d, int e, async::IJobState* spState)
	{
		::Sleep(10000);
		return (a + b + c + d + e);
	};
};

class CTestPoolRet
{
public:
	CTestPoolRet(){ };
	virtual ~CTestPoolRet(){};

public:
	int Add0(async::IJobState* pState)
	{
		::Sleep(10000);
		OutputDebugStringA("CTestPoolRet add0\r\n");
		return 1;
	};
	//
	int Add1(int a, async::IJobState* spState)
	{
		::Sleep(10000);
		OutputDebugStringA("CTestPoolRet add1\r\n");
		return a;
	};
	//
	int Add2(int a, int b, async::IJobState* spState)
	{
		::Sleep(10000);
		OutputDebugStringA("CTestPoolRet add2\r\n");
		return (a + b);
	};
	//
	int Add3(int a, int b, int c, async::IJobState* spState)
	{
		::Sleep(10000);
		OutputDebugStringA("CTestPoolRet add3\r\n");
		return (a + b + c);
	};
	//
	int Add4(int a, int b, int c, int d, async::IJobState* spState)
	{
		::Sleep(10000);
		OutputDebugStringA("CTestPoolRet add4\r\n");
		return (a + b + c + d);
	};
	//
	int Add5(int a, int b, int c, int d, int e, async::IJobState* spState)
	{
		::Sleep(10000);
		OutputDebugStringA("CTestPoolRet add5\r\n");
		return (a + b + c + d + e);
	};
};

void ThreadRun(async::CJobSchedulerTimerBase* pTimer)
{
	CTestRet l_testRet;
	int l_nRet = 0;
	async::AsynCall4W<int> l_Asyn;
	l_Asyn.AsynCall(&l_testRet, &CTestRet::Add0, pTimer);
	if (l_Asyn.WaitForEndOrTimeout(30 * 1000))
	{
		if (l_Asyn.IsDone())
		{
			l_nRet = l_Asyn.GetResult();
		}
	}
	//l_nRet = l_Asyn.WaitFor();
	l_Asyn.AsynCall(&l_testRet, &CTestRet::Add1, 1, pTimer);
	l_nRet = l_Asyn.WaitFor();
	//
	l_Asyn.AsynCall(&l_testRet, &CTestRet::Add2, 1, 2, pTimer);
	l_nRet = l_Asyn.WaitFor();
	//
	l_Asyn.AsynCall(&l_testRet, &CTestRet::Add3, 1, 2, 3, pTimer);
	l_nRet = l_Asyn.WaitFor();
	//
	l_Asyn.AsynCall(&l_testRet, &CTestRet::Add4, 1, 2, 3, 4, pTimer);
	l_nRet = l_Asyn.WaitFor();
	//
	l_Asyn.AsynCall(&l_testRet, &CTestRet::Add5, 1, 2, 3, 4, 5, pTimer);
	l_nRet = l_Asyn.WaitFor();

	l_nRet = 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
	CTest l_test;
	CTestEx l_testEx;
	CTestRet l_testRet;
	
/*	async::CJobSchedulerTimerBase l_JobSchedulerTimer("JobSchedulerTimer");
	l_JobSchedulerTimer.Startup();
	//
	async::CJobSchedulerBase l_JobScheduler("JobScheduler");
	BOOL l_bRet = l_JobScheduler.Startup();

//	{
//		async::CAsynCall l_Call0(&l_test, &CTest::Add0, &l_JobSchedulerTimer);
//		async::CAsynCall l_Call1(&l_test, &CTest::Add1, 1, &l_JobSchedulerTimer);
//		async::CAsynCall l_Call2(&l_test, &CTest::Add2, 1, 2, &l_JobSchedulerTimer);
//		async::CAsynCall l_Call3(&l_test, &CTest::Add3, 1, 2, 3, &l_JobSchedulerTimer);
//		async::CAsynCall l_Call4(&l_test, &CTest::Add4, 1, 2, 3, 4, &l_JobSchedulerTimer);
//		async::CAsynCall l_Call5(&l_test, &CTest::Add5, 1, 2, 3, 4, 5, &l_JobSchedulerTimer);
//	}
	
	{
		async::CAsynCall l_Call0(ThreadRun, &l_JobSchedulerTimer, &l_JobScheduler);
	}

	{
		MSG l_msg = { 0 };
		while (::GetMessage(&l_msg, NULL, 0, 0))
		{
			if (l_msg.message == WM_QUIT)
			{
				break;
			}
			::TranslateMessage(&l_msg);
			::DispatchMessage(&l_msg);
		}
		long l = 0;
	}
	l_JobSchedulerTimer.Shutdown();
*/

	async::CJobSchedulerPool<> l_JobSchedulerPool("CJobSchedulerPool");
	{
		CTestPoolRet l_testRet;
		BOOL l_bRetPool = l_JobSchedulerPool.Startup();
		async::AsynCall4W<int> l_Asyn0;
		async::AsynCall4W<int> l_Asyn1;
		async::AsynCall4W<int> l_Asyn2;
		async::AsynCall4W<int> l_Asyn3;
		async::AsynCall4W<int> l_Asyn4;
		async::AsynCall4W<int> l_Asyn5;
		async::AsynCall4W<int> l_Asyn6;
		async::AsynCall4W<int> l_Asyn7;
		async::AsynCall4W<int> l_Asyn8;
		async::AsynCall4W<int> l_Asyn9;
		async::AsynCall4W<int> l_Asyn10;
		async::AsynCall4W<int> l_Asyn11;
		async::AsynCall4W<int> l_Asyn12;
		async::AsynCall4W<int> l_Asyn13;
		async::AsynCall4W<int> l_Asyn14;
		async::AsynCall4W<int> l_Asyn15;
		async::AsynCall4W<int> l_Asyn16;
		async::AsynCall4W<int> l_Asyn17;
		async::AsynCall4W<int> l_Asyn18;
		async::AsynCall4W<int> l_Asyn19;
		async::AsynCall4W<int> l_Asyn20;
		async::AsynCall4W<int> l_Asyn21;
		async::AsynCall4W<int> l_Asyn22;
		async::AsynCall4W<int> l_Asyn23;
		async::AsynCall4W<int> l_Asyn24;
		async::AsynCall4W<int> l_Asyn25;
		async::AsynCall4W<int> l_Asyn26;
		async::AsynCall4W<int> l_Asyn27;
		async::AsynCall4W<int> l_Asyn28;
		async::AsynCall4W<int> l_Asyn29;
		l_Asyn0.AsynCall(&l_testRet, &CTestPoolRet::Add0, &l_JobSchedulerPool);
		l_Asyn1.AsynCall(&l_testRet, &CTestPoolRet::Add1, 1, &l_JobSchedulerPool);
		l_Asyn2.AsynCall(&l_testRet, &CTestPoolRet::Add2, 1, 2, &l_JobSchedulerPool);
		l_Asyn3.AsynCall(&l_testRet, &CTestPoolRet::Add3, 1, 2, 3, &l_JobSchedulerPool);
		l_Asyn4.AsynCall(&l_testRet, &CTestPoolRet::Add4, 1, 2, 3, 4, &l_JobSchedulerPool);
		l_Asyn5.AsynCall(&l_testRet, &CTestPoolRet::Add5, 1, 2, 3, 4, 5, &l_JobSchedulerPool);
		l_Asyn6.AsynCall(&l_testRet, &CTestPoolRet::Add0, &l_JobSchedulerPool);
		l_Asyn7.AsynCall(&l_testRet, &CTestPoolRet::Add1, 1, &l_JobSchedulerPool);
		l_Asyn8.AsynCall(&l_testRet, &CTestPoolRet::Add2, 1, 2, &l_JobSchedulerPool);
		l_Asyn9.AsynCall(&l_testRet, &CTestPoolRet::Add3, 1, 2, 3, &l_JobSchedulerPool);
		l_Asyn10.AsynCall(&l_testRet, &CTestPoolRet::Add0, &l_JobSchedulerPool);
		l_Asyn11.AsynCall(&l_testRet, &CTestPoolRet::Add1, 1, &l_JobSchedulerPool);
		l_Asyn12.AsynCall(&l_testRet, &CTestPoolRet::Add2, 1, 2, &l_JobSchedulerPool);
		l_Asyn13.AsynCall(&l_testRet, &CTestPoolRet::Add3, 1, 2, 3, &l_JobSchedulerPool);
		l_Asyn14.AsynCall(&l_testRet, &CTestPoolRet::Add4, 1, 2, 3, 4, &l_JobSchedulerPool);
		l_Asyn15.AsynCall(&l_testRet, &CTestPoolRet::Add5, 1, 2, 3, 4, 5, &l_JobSchedulerPool);
		l_Asyn16.AsynCall(&l_testRet, &CTestPoolRet::Add0, &l_JobSchedulerPool);
		l_Asyn17.AsynCall(&l_testRet, &CTestPoolRet::Add1, 1, &l_JobSchedulerPool);
		l_Asyn18.AsynCall(&l_testRet, &CTestPoolRet::Add2, 1, 2, &l_JobSchedulerPool);
		l_Asyn19.AsynCall(&l_testRet, &CTestPoolRet::Add3, 1, 2, 3, &l_JobSchedulerPool);
		l_Asyn20.AsynCall(&l_testRet, &CTestPoolRet::Add0, &l_JobSchedulerPool);
		l_Asyn21.AsynCall(&l_testRet, &CTestPoolRet::Add1, 1, &l_JobSchedulerPool);
		l_Asyn22.AsynCall(&l_testRet, &CTestPoolRet::Add2, 1, 2, &l_JobSchedulerPool);
		l_Asyn23.AsynCall(&l_testRet, &CTestPoolRet::Add3, 1, 2, 3, &l_JobSchedulerPool);
		l_Asyn24.AsynCall(&l_testRet, &CTestPoolRet::Add4, 1, 2, 3, 4, &l_JobSchedulerPool);
		l_Asyn25.AsynCall(&l_testRet, &CTestPoolRet::Add5, 1, 2, 3, 4, 5, &l_JobSchedulerPool);
		l_Asyn26.AsynCall(&l_testRet, &CTestPoolRet::Add0, &l_JobSchedulerPool);
		l_Asyn27.AsynCall(&l_testRet, &CTestPoolRet::Add1, 1, &l_JobSchedulerPool);
		l_Asyn28.AsynCall(&l_testRet, &CTestPoolRet::Add2, 1, 2, &l_JobSchedulerPool);
		l_Asyn29.AsynCall(&l_testRet, &CTestPoolRet::Add3, 1, 2, 3, &l_JobSchedulerPool);
		l_Asyn0.WaitFor();
		l_Asyn1.WaitFor();
		l_Asyn2.WaitFor();
		l_Asyn3.WaitFor();
		l_Asyn4.WaitFor();
		l_Asyn5.WaitFor();
		l_Asyn6.WaitFor();
		l_Asyn7.WaitFor();
		l_Asyn8.WaitFor();
		l_Asyn9.WaitFor();
		l_Asyn10.WaitFor();
		l_Asyn11.WaitFor();
		l_Asyn12.WaitFor();
		l_Asyn13.WaitFor();
		l_Asyn14.WaitFor();
		l_Asyn15.WaitFor();
		l_Asyn16.WaitFor();
		l_Asyn17.WaitFor();
		l_Asyn18.WaitFor();
		l_Asyn19.WaitFor();
		l_Asyn20.WaitFor();
		l_Asyn21.WaitFor();
		l_Asyn22.WaitFor();
		l_Asyn23.WaitFor();
		l_Asyn24.WaitFor();
		l_Asyn25.WaitFor();
		l_Asyn26.WaitFor();
		l_Asyn27.WaitFor();
		l_Asyn28.WaitFor();
		l_Asyn29.WaitFor();
		int l_nIndex = 0;
	}
	/*{
		CTestPoolRet l_testRet;
		BOOL l_bRetPool = l_JobSchedulerPool.Startup();
		async::AsynCall4W<int> l_Asyn0;
		async::AsynCall4W<int> l_Asyn1;
		async::AsynCall4W<int> l_Asyn2;
		async::AsynCall4W<int> l_Asyn3;
		async::AsynCall4W<int> l_Asyn4;
		async::AsynCall4W<int> l_Asyn5;
		l_Asyn0.AsynCall(&l_testRet, &CTestPoolRet::Add0, &l_JobSchedulerPool);
		l_Asyn1.AsynCall(&l_testRet, &CTestPoolRet::Add1, 1, &l_JobSchedulerPool);
		l_Asyn2.AsynCall(&l_testRet, &CTestPoolRet::Add2, 1, 2, &l_JobSchedulerPool);
		l_Asyn3.AsynCall(&l_testRet, &CTestPoolRet::Add3, 1, 2, 3, &l_JobSchedulerPool);
		l_Asyn4.AsynCall(&l_testRet, &CTestPoolRet::Add4, 1, 2, 3, 4, &l_JobSchedulerPool);
		l_Asyn5.AsynCall(&l_testRet, &CTestPoolRet::Add5, 1, 2, 3, 4, 5, &l_JobSchedulerPool);
		l_Asyn0.WaitFor();
		l_Asyn1.WaitFor();
		l_Asyn2.WaitFor();
		l_Asyn3.WaitFor();
		l_Asyn4.WaitFor();
		l_Asyn5.WaitFor();
		int l_nIndex = 0;
	}*/
	Sleep(1000);
	int i = 0;
	async::CJobSchedulerBase l_JobScheduler("JobScheduler");
	{
		BOOL l_bRet = l_JobScheduler.Startup();
	}
	{
		async::AsynCallEx l_Asyn;
		l_Asyn.AsynCall(&l_testEx, &CTestEx::Add0, &l_JobScheduler);
		l_Asyn.WaitFor();
		l_Asyn.AsynCall(&l_testEx, &CTestEx::Add1, 1, &l_JobScheduler);
		l_Asyn.WaitFor();
		l_Asyn.AsynCall(&l_testEx, &CTestEx::Add2, 1, 2, &l_JobScheduler);
		l_Asyn.WaitFor();
		l_Asyn.AsynCall(&l_testEx, &CTestEx::Add3, 1, 2, 3, &l_JobScheduler);
		l_Asyn.WaitFor();
		l_Asyn.AsynCall(&l_testEx, &CTestEx::Add4, 1, 2, 3, 4, &l_JobScheduler);
		l_Asyn.WaitFor();
		l_Asyn.AsynCall(&l_testEx, &CTestEx::Add5, 1, 2, 3, 4, 5, &l_JobScheduler);
		l_Asyn.WaitFor();
	}
	{
		async::CAsynCall l_Call0(&l_test, &CTest::Add0, &l_JobScheduler);
		async::CAsynCall l_Call1(&l_test, &CTest::Add1, 1, &l_JobScheduler);
		async::CAsynCall l_Call2(&l_test, &CTest::Add2, 1, 2, &l_JobScheduler);
		async::CAsynCall l_Call3(&l_test, &CTest::Add3, 1, 2, 3, &l_JobScheduler);
		async::CAsynCall l_Call4(&l_test, &CTest::Add4, 1, 2, 3, 4, &l_JobScheduler);
		async::CAsynCall l_Call5(&l_test, &CTest::Add5, 1, 2, 3, 4, 5, &l_JobScheduler);
	}
	//
	{
		async::AsynCall4W<int> l_Asyn;
		l_Asyn.AsynCall(&l_testRet, &CTestRet::Add0, &l_JobScheduler);
		l_Asyn.AsynCall(&l_testRet, &CTestRet::Add1, 1, &l_JobScheduler);
		l_Asyn.AsynCall(&l_testRet, &CTestRet::Add2, 1, 2, &l_JobScheduler);
		l_Asyn.AsynCall(&l_testRet, &CTestRet::Add3, 1, 2, 3, &l_JobScheduler);
		l_Asyn.AsynCall(&l_testRet, &CTestRet::Add4, 1, 2, 3, 4, &l_JobScheduler);
		l_Asyn.AsynCall(&l_testRet, &CTestRet::Add5, 1, 2, 3, 4, 5, &l_JobScheduler);
	}
	//
	{
		int l_nRet = 0;
		async::AsynCall4W<int> l_Asyn;
		l_Asyn.AsynCall(&l_testRet, &CTestRet::Add0, &l_JobScheduler);
		if (l_Asyn.WaitForEndOrTimeout(30 * 1000))
		{
			if (l_Asyn.IsDone())
			{
				l_nRet = l_Asyn.GetResult();
			}
		}
		//l_nRet = l_Asyn.WaitFor();
		l_Asyn.AsynCall(&l_testRet, &CTestRet::Add1, 1, &l_JobScheduler);
		l_nRet = l_Asyn.WaitFor();
		//if (l_Asyn.WaitForEndOrTimeout(30 * 1000))
		//{
		//	l_nRet = l_Asyn.GetResult();
		//}
		l_Asyn.AsynCall(&l_testRet, &CTestRet::Add2, 1, 2, &l_JobScheduler);
		l_nRet = l_Asyn.WaitFor();
		//if (l_Asyn.WaitForEndOrTimeout(30 * 1000))
		//{
		//	l_nRet = l_Asyn.GetResult();
		//}
		l_Asyn.AsynCall(&l_testRet, &CTestRet::Add3, 1, 2, 3, &l_JobScheduler);
		l_nRet = l_Asyn.WaitFor();
		//if (l_Asyn.WaitForEndOrTimeout(30 * 1000))
		//{
		//	l_nRet = l_Asyn.GetResult();
		//}
		l_Asyn.AsynCall(&l_testRet, &CTestRet::Add4, 1, 2, 3, 4, &l_JobScheduler);
		l_nRet = l_Asyn.WaitFor();
		//if (l_Asyn.WaitForEndOrTimeout(30 * 1000))
		//{
		//	l_nRet = l_Asyn.GetResult();
		//}
		l_Asyn.AsynCall(&l_testRet, &CTestRet::Add5, 1, 2, 3, 4, 5, &l_JobScheduler);
		l_nRet = l_Asyn.WaitFor();
		//if (l_Asyn.WaitForEndOrTimeout(30 * 1000))
		//{
		//	l_nRet = l_Asyn.GetResult();
		//}
	}
	Sleep(1000);
	//async::CJobBase<async::CJobStateBase> g_job();
	system("pause");
	l_JobScheduler.NotifyExit();
	l_JobScheduler.Shutdown();
	//
	return 0;
}

