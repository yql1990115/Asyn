/***************************************************************************
*   Copyright (C) 2019 by yql1990115                                      *
*   yql1990115@gmail.com                                                  *
*                                                                         *
*   www.yql1990115.com                                                    *
***************************************************************************/

#pragma once

#include "IJob.h"
#include "JobBase.h"
#include "JobStateBase.h"
#include "IJobScheduler.h"
#include "IJobSchedulerTimer.h"

namespace async{

class CAsynCall
{
protected:
	class CAsynCall0 : public CJobBase<CJobStateBase>
	{
	public:
		CAsynCall0(void(*func)(), const char* strName) : CJobBase(nullptr, strName)
		{
			m_func = func;
		}

		virtual void InternalExecute()
		{
			(*m_func)();
		}

	protected:
		void			(*m_func)();
	};
	
	template<class TObj>
	class CAsynCallMember0 : public CJobBase<CJobStateBase>
	{
	public:
		CAsynCallMember0(TObj* instance, void (TObj::*func)(), const char* strName) : CJobBase(nullptr, strName)
		{
			m_instance = instance;
			m_func = func;
		}

		virtual void InternalExecute()
		{
			(m_instance->*m_func)();
		}

	protected:
		TObj*			m_instance;
		void			(TObj::*m_func)();
	};

	template<class T1>
	class CAsynCall1 : public CJobBase<CJobStateBase>
	{
	public:
		CAsynCall1(void(*func)(T1), T1 parameter1, const char* strName) : CJobBase(nullptr, strName)
		{
			m_func = func;
			m_parameter1 = parameter1;
		}

		virtual void InternalExecute()
		{
			(*m_func)(m_parameter1);
		}

	protected:
		void			(*m_func)(T1);
		T1				m_parameter1;
	};

	template<class TObj, class T1>
	class CAsynCallMember1 : public CJobBase<CJobStateBase>
	{
	public:
		CAsynCallMember1(TObj* instance, void (TObj::*func)(T1), T1 parameter1, const char* strName) : CJobBase(nullptr, strName)
		{
			m_instance = instance;
			m_func = func;
			m_parameter1 = parameter1;
		}

		virtual void InternalExecute()
		{
			(m_instance->*m_func)(m_parameter1);
		}

	protected:
		TObj*			m_instance;
		void			(TObj::*m_func)(T1);
		T1				m_parameter1;
	};

	template<class T1, class T2>
	class CAsynCall2 : public CJobBase<CJobStateBase>
	{
	public:
		CAsynCall2(void(*func)(T1, T2), T1 parameter1, T2 parameter2, const char* strName) : CJobBase(nullptr, strName)
		{
			m_func = func;
			m_parameter1 = parameter1;
			m_parameter2 = parameter2;
		}

		virtual void InternalExecute()
		{
			(*m_func)(m_parameter1, m_parameter2);
		}

	protected:
		void(*m_func)(T1, T2);
		T1				m_parameter1;
		T2				m_parameter2;
	};

	template<class TObj, class T1, class T2>
	class CAsynCallMember2 : public CJobBase<CJobStateBase>
	{
	public:
		CAsynCallMember2(TObj* instance, void (TObj::*func)(T1, T2), T1 parameter1, T2 parameter2, const char* strName) : CJobBase(nullptr, strName)
		{
			m_instance = instance;
			m_func = func;
			m_parameter1 = parameter1;
			m_parameter2 = parameter2;
		}

		virtual void InternalExecute()
		{
			(m_instance->*m_func)(m_parameter1, m_parameter2);
		}

	protected:
		TObj*			m_instance;
		void			(TObj::*m_func)(T1, T2);
		T1				m_parameter1;
		T2				m_parameter2;
	};

	template<class T1, class T2, class T3>
	class CAsynCall3 : public CJobBase<CJobStateBase>
	{
	public:
		CAsynCall3(void(*func)(T1, T2, T3), T1 parameter1, T2 parameter2, T3 parameter3, const char* strName) : CJobBase(nullptr, strName)
		{
			m_func = func;
			m_parameter1 = parameter1;
			m_parameter2 = parameter2;
			m_parameter3 = parameter3;
		}

		virtual void InternalExecute()
		{
			(*m_func)(m_parameter1, m_parameter2, m_parameter3);
		}

	protected:
		void(*m_func)(T1, T2, T3);
		T1				m_parameter1;
		T2				m_parameter2;
		T3				m_parameter3;
	};

	template<class TObj, class T1, class T2, class T3>
	class CAsynCallMember3 : public CJobBase<CJobStateBase>
	{
	public:
		CAsynCallMember3(TObj* instance, void (TObj::*func)(T1, T2, T3), T1 parameter1, T2 parameter2, T3 parameter3, const char* strName) : CJobBase(nullptr, strName)
		{
			m_instance = instance;
			m_func = func;
			m_parameter1 = parameter1;
			m_parameter2 = parameter2;
			m_parameter3 = parameter3;
		}

		virtual void InternalExecute()
		{
			(m_instance->*m_func)(m_parameter1, m_parameter2, m_parameter3);
		}

	protected:
		TObj*			m_instance;
		void			(TObj::*m_func)(T1, T2, T3);
		T1				m_parameter1;
		T2				m_parameter2;
		T3				m_parameter3;
	};

	template<class T1, class T2, class T3, class T4>
	class CAsynCall4 : public CJobBase<CJobStateBase>
	{
	public:
		CAsynCall4(void(*func)(T1, T2, T3, T4), T1 parameter1, T2 parameter2, T3 parameter3, T4 parameter4, const char* strName) : CJobBase(nullptr, strName)
		{
			m_func = func;
			m_parameter1 = parameter1;
			m_parameter2 = parameter2;
			m_parameter3 = parameter3;
			m_parameter4 = parameter4;
		}

		virtual void InternalExecute()
		{
			(*m_func)(m_parameter1, m_parameter2, m_parameter3, m_parameter4);
		}

	protected:
		void(*m_func)(T1, T2, T3, T4);
		T1				m_parameter1;
		T2				m_parameter2;
		T3				m_parameter3;
		T4				m_parameter4;
	};

	template<class TObj, class T1, class T2, class T3, class T4>
	class CAsynCallMember4 : public CJobBase<CJobStateBase>
	{
	public:
		CAsynCallMember4(TObj* instance, void (TObj::*func)(T1, T2, T3, T4), T1 parameter1, T2 parameter2, T3 parameter3, T4 parameter4, const char* strName) : CJobBase(nullptr, strName)
		{
			m_instance = instance;
			m_func = func;
			m_parameter1 = parameter1;
			m_parameter2 = parameter2;
			m_parameter3 = parameter3;
			m_parameter4 = parameter4;
		}

		virtual void InternalExecute()
		{
			(m_instance->*m_func)(m_parameter1, m_parameter2, m_parameter3, m_parameter4);
		}

	protected:
		TObj*			m_instance;
		void			(TObj::*m_func)(T1, T2, T3, T4);
		T1				m_parameter1;
		T2				m_parameter2;
		T3				m_parameter3;
		T4				m_parameter4;
	};

	template<class T1, class T2, class T3, class T4, class T5>
	class CAsynCall5 : public CJobBase<CJobStateBase>
	{
	public:
		CAsynCall5(void(*func)(T1, T2, T3, T4, T5), T1 parameter1, T2 parameter2, T3 parameter3, T4 parameter4, T5 parameter5, const char* strName) : CJobBase(nullptr, strName)
		{
			m_func = func;
			m_parameter1 = parameter1;
			m_parameter2 = parameter2;
			m_parameter3 = parameter3;
			m_parameter4 = parameter4;
			m_parameter5 = parameter5;
		}

		virtual void InternalExecute()
		{
			(*m_func)(m_parameter1, m_parameter2, m_parameter3, m_parameter4, m_parameter5);
		}

	protected:
		void(*m_func)(T1, T2, T3, T4, T5);
		T1				m_parameter1;
		T2				m_parameter2;
		T3				m_parameter3;
		T4				m_parameter4;
		T5				m_parameter5;
	};

	template<class TObj, class T1, class T2, class T3, class T4, class T5>
	class CAsynCallMember5 : public CJobBase<CJobStateBase>
	{
	public:
		CAsynCallMember5(TObj* instance, void (TObj::*func)(T1, T2, T3, T4, T5), T1 parameter1, T2 parameter2, T3 parameter3, T4 parameter4, T5 parameter5, const char* strName) : CJobBase(nullptr, strName)
		{
			m_instance = instance;
			m_func = func;
			m_parameter1 = parameter1;
			m_parameter2 = parameter2;
			m_parameter3 = parameter3;
			m_parameter4 = parameter4;
			m_parameter5 = parameter5;
		}

		virtual void InternalExecute()
		{
			(m_instance->*m_func)(m_parameter1, m_parameter2, m_parameter3, m_parameter4, m_parameter5);
		}

	protected:
		TObj*			m_instance;
		void			(TObj::*m_func)(T1, T2, T3, T4, T5);
		T1				m_parameter1;
		T2				m_parameter2;
		T3				m_parameter3;
		T4				m_parameter4;
		T5				m_parameter5;
	};

public:
	CAsynCall(void(*func)(), IJobScheduler* pScheduler, const char* strName = nullptr)
	{
		assert(func);
		assert(pScheduler);
		IJob* l_pJob = new CAsynCall0(func, strName);
		assert(l_pJob);
		BOOL l_bRet = pScheduler->Schedule(l_pJob);
		assert(l_bRet );
	};

	template<class TObj>
	CAsynCall(TObj* instance, void (TObj::*func)(), IJobScheduler* pScheduler, const char* strName = nullptr)
	{
		assert(func);
		assert(instance);
		assert(pScheduler);
		IJob* l_pJob = new CAsynCallMember0<TObj>(instance, func, strName);
		assert(l_pJob);
		BOOL l_bRet = pScheduler->Schedule(l_pJob);
		assert(l_bRet);
	};

	template<class T1>
	CAsynCall(void(*func)(T1), T1 parameter1, IJobScheduler* pScheduler, const char* strName = nullptr)
	{
		assert(func);
		assert(pScheduler);
		IJob* l_pJob = new CAsynCall1<T1>(func, parameter1, strName);
		assert(l_pJob);
		BOOL l_bRet = pScheduler->Schedule(l_pJob);
		assert(l_bRet);
	};

	template<class TObj, class T1>
	CAsynCall(TObj* instance, void (TObj::*func)(T1), T1 parameter1, IJobScheduler* pScheduler, const char* strName = nullptr)
	{
		assert(func);
		assert(instance);
		assert(pScheduler);
		IJob* l_pJob = new CAsynCallMember1<TObj, T1>(instance, func, parameter1, strName);
		assert(l_pJob);
		BOOL l_bRet = pScheduler->Schedule(l_pJob);
		assert(l_bRet);
	};

	template<class T1, class T2>
	CAsynCall(void(*func)(T1, T2), T1 parameter1, T2 parameter2, IJobScheduler* pScheduler, const char* strName = nullptr)
	{
		assert(func);
		assert(pScheduler);
		IJob* l_pJob = new CAsynCall2<T1, T2>(func, parameter1, parameter2, strName);
		assert(l_pJob);
		BOOL l_bRet = pScheduler->Schedule(l_pJob);
		assert(l_bRet);
	};

	template<class TObj, class T1, class T2>
	CAsynCall(TObj* instance, void (TObj::*func)(T1, T2), T1 parameter1, T2 parameter2, IJobScheduler* pScheduler, const char* strName = nullptr)
	{
		assert(func);
		assert(instance);
		assert(pScheduler);
		IJob* l_pJob = new CAsynCallMember2<TObj, T1, T2>(instance, func, parameter1, parameter2, strName);
		assert(l_pJob);
		BOOL l_bRet = pScheduler->Schedule(l_pJob);
		assert(l_bRet);
	};

	template<class T1, class T2, class T3>
	CAsynCall(void(*func)(T1, T2, T3), T1 parameter1, T2 parameter2, T3 parameter3, IJobScheduler* pScheduler, const char* strName = nullptr)
	{
		assert(func);
		assert(pScheduler);
		IJob* l_pJob = new CAsynCall3<T1, T2, T3>(func, parameter1, parameter2, parameter3, strName);
		assert(l_pJob);
		BOOL l_bRet = pScheduler->Schedule(l_pJob);
		assert(l_bRet);
	};

	template<class TObj, class T1, class T2, class T3>
	CAsynCall(TObj* instance, void (TObj::*func)(T1, T2, T3), T1 parameter1, T2 parameter2, T3 parameter3, IJobScheduler* pScheduler, const char* strName = nullptr)
	{
		assert(func);
		assert(instance);
		assert(pScheduler);
		IJob* l_pJob = new CAsynCallMember3<TObj, T1, T2, T3>(instance, func, parameter1, parameter2, parameter3, strName);
		assert(l_pJob);
		BOOL l_bRet = pScheduler->Schedule(l_pJob);
		assert(l_bRet);
	};

	template<class T1, class T2, class T3, class T4>
	CAsynCall(void(*func)(T1, T2, T3, T4), T1 parameter1, T2 parameter2, T3 parameter3, T4 parameter4, IJobScheduler* pScheduler, const char* strName = nullptr)
	{
		assert(func);
		assert(pScheduler);
		IJob* l_pJob = new CAsynCall4<T1, T2, T3, T4>(func, parameter1, parameter2, parameter3, parameter4, strName);
		assert(l_pJob);
		BOOL l_bRet = pScheduler->Schedule(l_pJob);
		assert(l_bRet);
	};

	template<class TObj, class T1, class T2, class T3, class T4>
	CAsynCall(TObj* instance, void (TObj::*func)(T1, T2, T3, T4), T1 parameter1, T2 parameter2, T3 parameter3, T4 parameter4, IJobScheduler* pScheduler, const char* strName = nullptr)
	{
		assert(func);
		assert(instance);
		assert(pScheduler);
		IJob* l_pJob = new CAsynCallMember4<TObj, T1, T2, T3, T4>(instance, func, parameter1, parameter2, parameter3, parameter4, strName);
		assert(l_pJob);
		BOOL l_bRet = pScheduler->Schedule(l_pJob);
		assert(l_bRet);
	};

	template<class T1, class T2, class T3, class T4, class T5>
	CAsynCall(void(*func)(T1, T2, T3, T4, T5), T1 parameter1, T2 parameter2, T3 parameter3, T4 parameter4, T5 parameter5, IJobScheduler* pScheduler, const char* strName = nullptr)
	{
		assert(func);
		assert(pScheduler);
		IJob* l_pJob = new CAsynCall5<T1, T2, T3, T4, T5>(func, parameter1, parameter2, parameter3, parameter4, parameter5, strName);
		assert(l_pJob);
		BOOL l_bRet = pScheduler->Schedule(l_pJob);
		assert(l_bRet);
	};

	template<class TObj, class T1, class T2, class T3, class T4, class T5>
	CAsynCall(TObj* instance, void (TObj::*func)(T1, T2, T3, T4, T5), T1 parameter1, T2 parameter2, T3 parameter3, T4 parameter4, T5 parameter5, IJobScheduler* pScheduler, const char* strName = nullptr)
	{
		assert(func);
		assert(instance);
		assert(pScheduler);
		IJob* l_pJob = new CAsynCallMember5<TObj, T1, T2, T3, T4, T5>(instance, func, parameter1, parameter2, parameter3, parameter4, parameter5, strName);
		assert(l_pJob);
		BOOL l_bRet = pScheduler->Schedule(l_pJob);
		assert(l_bRet);
	};

	virtual ~CAsynCall(){};
};

}

