/*
===========================================================================

Copyright (C) 2019 by yql1990115
yql1990115@gmail.com
www.yql1990115.com

===========================================================================
*/
#ifndef __ASYNCALLEX_H__
#define __ASYNCALLEX_H__

#include "qlAsynHelper.h"

namespace asyn {

/*
//////////////////////////////////////////////////////////////////////////
// 作业状态重写
//////////////////////////////////////////////////////////////////////////
*/
template<class R> class CqlJobStateR : public CqlJobState
{
public:
	CqlJobStateR(IScheduler* pScheduler) : m_pScheduler(*pScheduler) {}

public:
	virtual BOOL 	HasBeenTerminated() const override
	{
		BOOL l_bRet1 = CqlJobState::HasBeenTerminated();
		BOOL l_bRet2 = m_pScheduler.HasBeenTerminated();
		BOOL l_bRet = l_bRet1 || l_bRet2;
		return l_bRet;
	}

	virtual BOOL 	HasBeenSuspended() const override
	{
		BOOL l_bRet1 = CqlJobState::HasBeenSuspended();
		BOOL l_bRet2 = m_pScheduler.HasBeenSuspended();
		BOOL l_bRet = l_bRet1 || l_bRet2;
		return l_bRet;
	}	

public:
	IScheduler& m_pScheduler;
	R			m_Result;
};


/*
//////////////////////////////////////////////////////////////////////////
// 作业辅助类重写
//////////////////////////////////////////////////////////////////////////
*/
template<class R> class CqlAsynCallEx
{
protected:
	typedef std::shared_ptr<IJobState> TState;

protected:
	//////////////////////////////////////////////////////////////////////////
	class CAsynCall0 : public CqlJob
	{
	public:
		CAsynCall0(R(*func)(), R& result, const char* strName, TState* spState, void(*ecb)(void* func, const char* strName) = NULL) : CqlJob(spState, strName, TRUE), m_Result(result)
		{
			m_Result = result;
			m_func = func;
			m_ecb = ecb;
		}

		virtual void InternalExecute() override
		{
			m_Result = (*m_func)();
		}

		virtual void HandleException() override
		{
			if (m_ecb)
			{
				m_ecb((void*)m_func, m_strName.c_str());
			}
		}
	protected:
		R&				m_Result;
		R(*m_func)();
		void(*m_ecb)(void* func, const char* strName);
	};

	template<class TObj>
	class CAsynCallMember0 : public CqlJob
	{
	public:
		CAsynCallMember0(TObj* instance, R(TObj::*func)(), R& result, const char* strName, TState* spState, void(TObj::*ecb)(void* func, const char* strName) = NULL) : CqlJob(spState, strName, TRUE), m_Result(result)
		{
			m_instance = instance;
			m_func = func;
			m_ecb = ecb;
		}

		virtual void InternalExecute() override
		{
			m_Result = (m_instance->*m_func)();
		}

		virtual void HandleException() override
		{
			if (m_ecb)
			{
				(m_instance->*m_ecb)(*((void**)&m_func), m_strName.c_str());
			}
		}
	protected:
		R&				m_Result;
		TObj*			m_instance;
		R(TObj::*m_func)();
		void(TObj::*m_ecb)(void* func, const char* strName);
	};

	class CAsynCallEx0 : public CqlJob
	{
	public:
		CAsynCallEx0(R(*func)(IJobState*), R& result, const char* strName, TState* spState, void(*ecb)(void* func, const char* strName) = NULL) : CqlJob(spState, strName, TRUE), m_Result(result)
		{
			m_Result = result;
			m_func = func;
			m_ecb = ecb;
		}

		virtual void InternalExecute() override
		{
			m_Result = (*m_func)(m_spState.get());
		}

		virtual void HandleException() override
		{
			if (m_ecb)
			{
				m_ecb((void*)m_func, m_strName.c_str());
			}
		}

	protected:
		R&				m_Result;
		R(*m_func)(IJobState*);
		void(*m_ecb)(void* func, const char* strName);
	};

	template<class TObj>
	class CAsynCallMemberEx0 : public CqlJob
	{
	public:
		CAsynCallMemberEx0(TObj* instance, R(TObj::*func)(IJobState*), R& result, const char* strName, TState* spState, void(TObj::*ecb)(void* func, const char* strName) = NULL) : CqlJob(spState, strName, TRUE), m_Result(result)
		{
			m_instance = instance;
			m_func = func;
			m_ecb = ecb;
		}

		virtual void InternalExecute() override
		{
			m_Result = (m_instance->*m_func)(m_spState.get());
		}

		virtual void HandleException() override
		{
			if (m_ecb)
			{
				(m_instance->*m_ecb)(*((void**)&m_func), m_strName.c_str());
			}
		}

	protected:
		R&				m_Result;
		TObj*			m_instance;
		R(TObj::*m_func)(IJobState*);
		void(TObj::*m_ecb)(void* func, const char* strName);
	};

	//////////////////////////////////////////////////////////////////////////
	template<class T1>
	class CAsynCall1 : public CqlJob
	{
	public:
		CAsynCall1(R(*func)(T1), T1 p1, R& result, const char* strName, TState* spState, void(*ecb)(void* func, const char* strName) = NULL) : CqlJob(spState, strName, TRUE), m_Result(result)
		{
			m_func = func;
			m_p1 = p1;
			m_ecb = ecb;
		}

		virtual void InternalExecute() override
		{
			m_Result = (*m_func)(m_p1);
		}

		virtual void HandleException() override
		{
			if (m_ecb)
			{
				m_ecb((void*)m_func, m_strName.c_str());
			}
		}

	protected:
		R&				m_Result;
		R(*m_func)(T1);
		void(*m_ecb)(void* func, const char* strName);
		T1				m_p1;
	};

	template<class TObj, class T1>
	class CAsynCallMember1 : public CqlJob
	{
	public:
		CAsynCallMember1(TObj* instance, R(TObj::*func)(T1), T1 p1, R& result, const char* strName, TState* spState, void(TObj::*ecb)(void* func, const char* strName) = NULL) : CqlJob(spState, strName, TRUE), m_Result(result)
		{
			m_instance = instance;
			m_func = func;
			m_p1 = p1;
			m_ecb = ecb;
		}

		virtual void InternalExecute() override
		{
			m_Result = (m_instance->*m_func)(m_p1);
		}

		virtual void HandleException() override
		{
			if (m_ecb)
			{
				(m_instance->*m_ecb)(*((void**)&m_func), m_strName.c_str());
			}
		}

	protected:
		R&				m_Result;
		TObj*			m_instance;
		R(TObj::*m_func)(T1);
		void(TObj::*m_ecb)(void* func, const char* strName);
		T1				m_p1;
	};

	template<class T1>
	class CAsynCallEx1 : public CqlJob
	{
	public:
		CAsynCallEx1(R(*func)(T1, IJobState*), T1 p1, R& result, const char* strName, TState* spState, void(*ecb)(void* func, const char* strName) = NULL) : CqlJob(spState, strName, TRUE), m_Result(result)
		{
			m_func = func;
			m_p1 = p1;
			m_ecb = ecb;
		}

		virtual void InternalExecute() override
		{
			m_Result = (*m_func)(m_p1, m_spState.get());
		}

		virtual void HandleException() override
		{
			if (m_ecb)
			{
				m_ecb((void*)m_func, m_strName.c_str());
			}
		}

	protected:
		R&				m_Result;
		R(*m_func)(T1, IJobState*);
		void(*m_ecb)(void* func, const char* strName);
		T1				m_p1;
	};

	template<class TObj, class T1>
	class CAsynCallMemberEx1 : public CqlJob
	{
	public:
		CAsynCallMemberEx1(TObj* instance, R(TObj::*func)(T1, IJobState*), T1 p1, R& result, const char* strName, TState* spState, void(TObj::*ecb)(void* func, const char* strName) = NULL) : CqlJob(spState, strName, TRUE), m_Result(result)
		{
			m_instance = instance;
			m_func = func;
			m_p1 = p1;
			m_ecb = ecb;
		}

		virtual void InternalExecute() override
		{
			m_Result = (m_instance->*m_func)(m_p1, m_spState.get());
		}

		virtual void HandleException() override
		{
			if (m_ecb)
			{
				(m_instance->*m_ecb)(*((void**)&m_func), m_strName.c_str());
			}
		}

	protected:
		R&				m_Result;
		TObj*			m_instance;
		R(TObj::*m_func)(T1, IJobState*);
		void(TObj::*m_ecb)(void* func, const char* strName);
		T1				m_p1;
	};

	//////////////////////////////////////////////////////////////////////////
	template<class T1, class T2>
	class CAsynCall2 : public CqlJob
	{
	public:
		CAsynCall2(R(*func)(T1, T2), T1 p1, T2 p2, R& result, const char* strName, TState* spState, void(*ecb)(void* func, const char* strName) = NULL) : CqlJob(spState, strName, TRUE), m_Result(result)
		{
			m_func = func;
			m_p1 = p1;
			m_p2 = p2;
			m_ecb = ecb;
		}

		virtual void InternalExecute() override
		{
			m_Result = (*m_func)(m_p1, m_p2);
		}

		virtual void HandleException() override
		{
			if (m_ecb)
			{
				m_ecb((void*)m_func, m_strName.c_str());
			}
		}

	protected:
		R&				m_Result;
		R(*m_func)(T1, T2);
		void(*m_ecb)(void* func, const char* strName);
		T1				m_p1;
		T2				m_p2;
	};

	template<class TObj, class T1, class T2>
	class CAsynCallMember2 : public CqlJob
	{
	public:
		CAsynCallMember2(TObj* instance, R(TObj::*func)(T1, T2), T1 p1, T2 p2, R& result, const char* strName, TState* spState, void(TObj::*ecb)(void* func, const char* strName) = NULL) : CqlJob(spState, strName, TRUE), m_Result(result)
		{
			m_instance = instance;
			m_func = func;
			m_p1 = p1;
			m_p2 = p2;
			m_ecb = ecb;
		}

		virtual void InternalExecute() override
		{
			m_Result = (m_instance->*m_func)(m_p1, m_p2);
		}

		virtual void HandleException() override
		{
			if (m_ecb)
			{
				(m_instance->*m_ecb)(*((void**)&m_func), m_strName.c_str());
			}
		}

	protected:
		R&				m_Result;
		TObj*			m_instance;
		R(TObj::*m_func)(T1, T2);
		void(TObj::*m_ecb)(void* func, const char* strName);
		T1				m_p1;
		T2				m_p2;
	};

	template<class T1, class T2>
	class CAsynCallEx2 : public CqlJob
	{
	public:
		CAsynCallEx2(R(*func)(T1, T2, IJobState*), T1 p1, T2 p2, R& result, const char* strName, TState* spState, void(*ecb)(void* func, const char* strName) = NULL) : CqlJob(spState, strName, TRUE), m_Result(result)
		{
			m_func = func;
			m_p1 = p1;
			m_p2 = p2;
			m_ecb = ecb;
		}

		virtual void InternalExecute() override
		{
			m_Result = (*m_func)(m_p1, m_p2, m_spState.get());
		}

		virtual void HandleException() override
		{
			if (m_ecb)
			{
				m_ecb((void*)m_func, m_strName.c_str());
			}
		}
		
	protected:
		R&				m_Result;
		R(*m_func)(T1, T2, IJobState*);
		void(*m_ecb)(void* func, const char* strName);
		T1				m_p1;
		T2				m_p2;
	};

	template<class TObj, class T1, class T2>
	class CAsynCallMemberEx2 : public CqlJob
	{
	public:
		CAsynCallMemberEx2(TObj* instance, R(TObj::*func)(T1, T2, IJobState*), T1 p1, T2 p2, R& result, const char* strName, TState* spState, void(TObj::*ecb)(void* func, const char* strName) = NULL) : CqlJob(spState, strName, TRUE), m_Result(result)
		{
			m_instance = instance;
			m_func = func;
			m_p1 = p1;
			m_p2 = p2;
			m_ecb = ecb;
		}

		virtual void InternalExecute() override
		{
			m_Result = (m_instance->*m_func)(m_p1, m_p2, m_spState.get());
		}

		virtual void HandleException() override
		{
			if (m_ecb)
			{
				(m_instance->*m_ecb)(*((void**)&m_func), m_strName.c_str());
			}
		}

	protected:
		R&				m_Result;
		TObj*			m_instance;
		R(TObj::*m_func)(T1, T2, IJobState*);
		void(TObj::*m_ecb)(void* func, const char* strName);
		T1				m_p1;
		T2				m_p2;
	};

	//////////////////////////////////////////////////////////////////////////
	template<class T1, class T2, class T3>
	class CAsynCall3 : public CqlJob
	{
	public:
		CAsynCall3(R(*func)(T1, T2, T3), T1 p1, T2 p2, T3 p3, R& result, const char* strName, TState* spState, void(*ecb)(void* func, const char* strName) = NULL) : CqlJob(spState, strName, TRUE), m_Result(result)
		{
			m_func = func;
			m_p1 = p1;
			m_p2 = p2;
			m_p3 = p3;
			m_ecb = ecb;
		}

		virtual void InternalExecute() override
		{
			m_Result = (*m_func)(m_p1, m_p2, m_p3);
		}

		virtual void HandleException() override
		{
			if (m_ecb)
			{
				m_ecb((void*)m_func, m_strName.c_str());
			}
		}
		
	protected:
		R&				m_Result;
		R(*m_func)(T1, T2, T3);
		void(*m_ecb)(void* func, const char* strName);
		T1				m_p1;
		T2				m_p2;
		T3				m_p3;
	};

	template<class TObj, class T1, class T2, class T3>
	class CAsynCallMember3 : public CqlJob
	{
	public:
		CAsynCallMember3(TObj* instance, R(TObj::*func)(T1, T2, T3), T1 p1, T2 p2, T3 p3, R& result, const char* strName, TState* spState, void(TObj::*ecb)(void* func, const char* strName) = NULL) : CqlJob(spState, strName, TRUE), m_Result(result)
		{
			m_instance = instance;
			m_func = func;
			m_p1 = p1;
			m_p2 = p2;
			m_p3 = p3;
			m_ecb = ecb;
		}

		virtual void InternalExecute() override
		{
			m_Result = (m_instance->*m_func)(m_p1, m_p2, m_p3);
		}

		virtual void HandleException() override
		{
			if (m_ecb)
			{
				(m_instance->*m_ecb)(*((void**)&m_func), m_strName.c_str());
			}
		}

	protected:
		R&				m_Result;
		TObj*			m_instance;
		R(TObj::*m_func)(T1, T2, T3);
		void(TObj::*m_ecb)(void* func, const char* strName);
		T1				m_p1;
		T2				m_p2;
		T3				m_p3;
	};

	template<class T1, class T2, class T3>
	class CAsynCallEx3 : public CqlJob
	{
	public:
		CAsynCallEx3(R(*func)(T1, T2, T3, IJobState*), T1 p1, T2 p2, T3 p3, R& result, const char* strName, TState* spState, void(*ecb)(void* func, const char* strName) = NULL) : CqlJob(spState, strName, TRUE), m_Result(result)
		{
			m_func = func;
			m_p1 = p1;
			m_p2 = p2;
			m_p3 = p3;
			m_ecb = ecb;
		}

		virtual void InternalExecute() override
		{
			m_Result = (*m_func)(m_p1, m_p2, m_p3, m_spState.get());
		}

		virtual void HandleException() override
		{
			if (m_ecb)
			{
				m_ecb((void*)m_func, m_strName.c_str());
			}
		}

	protected:
		R&				m_Result;
		R(*m_func)(T1, T2, T3, IJobState*);
		void(*m_ecb)(void* func, const char* strName);
		T1				m_p1;
		T2				m_p2;
		T3				m_p3;
	};

	template<class TObj, class T1, class T2, class T3>
	class CAsynCallMemberEx3 : public CqlJob
	{
	public:
		CAsynCallMemberEx3(TObj* instance, R(TObj::*func)(T1, T2, T3, IJobState*), T1 p1, T2 p2, T3 p3, R& result, const char* strName, TState* spState, void(TObj::*ecb)(void* func, const char* strName) = NULL) : CqlJob(spState, strName, TRUE), m_Result(result)
		{
			m_instance = instance;
			m_func = func;
			m_p1 = p1;
			m_p2 = p2;
			m_p3 = p3;
			m_ecb = ecb;
		}

		virtual void InternalExecute() override
		{
			m_Result = (m_instance->*m_func)(m_p1, m_p2, m_p3, m_spState.get());
		}

		virtual void HandleException() override
		{
			if (m_ecb)
			{
				(m_instance->*m_ecb)(*((void**)&m_func), m_strName.c_str());
			}
		}

	protected:
		R&				m_Result;
		TObj*			m_instance;
		R(TObj::*m_func)(T1, T2, T3, IJobState*);
		void(TObj::*m_ecb)(void* func, const char* strName);
		T1				m_p1;
		T2				m_p2;
		T3				m_p3;
	};

	//////////////////////////////////////////////////////////////////////////
	template<class T1, class T2, class T3, class T4>
	class CAsynCall4 : public CqlJob
	{
	public:
		CAsynCall4(R(*func)(T1, T2, T3, T4), T1 p1, T2 p2, T3 p3, T4 p4, R& result, const char* strName, TState* spState, void(*ecb)(void* func, const char* strName) = NULL) : CqlJob(spState, strName, TRUE), m_Result(result)
		{
			m_func = func;
			m_p1 = p1;
			m_p2 = p2;
			m_p3 = p3;
			m_p4 = p4;
			m_ecb = ecb;
		}

		virtual void InternalExecute() override
		{
			m_Result = (*m_func)(m_p1, m_p2, m_p3, m_p4);
		}

		virtual void HandleException() override
		{
			if (m_ecb)
			{
				m_ecb((void*)m_func, m_strName.c_str());
			}
		}

	protected:
		R&				m_Result;
		R(*m_func)(T1, T2, T3, T4);
		void(*m_ecb)(void* func, const char* strName);
		T1				m_p1;
		T2				m_p2;
		T3				m_p3;
		T4				m_p4;
	};

	template<class TObj, class T1, class T2, class T3, class T4>
	class CAsynCallMember4 : public CqlJob
	{
	public:
		CAsynCallMember4(TObj* instance, R(TObj::*func)(T1, T2, T3, T4), T1 p1, T2 p2, T3 p3, T4 p4, R& result, const char* strName, TState* spState, void(TObj::*ecb)(void* func, const char* strName) = NULL) : CqlJob(spState, strName, TRUE), m_Result(result)
		{
			m_instance = instance;
			m_func = func;
			m_p1 = p1;
			m_p2 = p2;
			m_p3 = p3;
			m_p4 = p4;
			m_ecb = ecb;
		}

		virtual void InternalExecute() override
		{
			m_Result = (m_instance->*m_func)(m_p1, m_p2, m_p3, m_p4);
		}

		virtual void HandleException() override
		{
			if (m_ecb)
			{
				(m_instance->*m_ecb)(*((void**)&m_func), m_strName.c_str());
			}
		}

	protected:
		R&				m_Result;
		TObj*			m_instance;
		R(TObj::*m_func)(T1, T2, T3, T4);
		void(TObj::*m_ecb)(void* func, const char* strName);
		T1				m_p1;
		T2				m_p2;
		T3				m_p3;
		T4				m_p4;
	};

	template<class T1, class T2, class T3, class T4>
	class CAsynCallEx4 : public CqlJob
	{
	public:
		CAsynCallEx4(R(*func)(T1, T2, T3, T4, IJobState*), T1 p1, T2 p2, T3 p3, T4 p4, R& result, const char* strName, TState* spState, void(*ecb)(void* func, const char* strName) = NULL) : CqlJob(spState, strName, TRUE), m_Result(result)
		{
			m_func = func;
			m_p1 = p1;
			m_p2 = p2;
			m_p3 = p3;
			m_p4 = p4;
			m_ecb = ecb;
		}

		virtual void InternalExecute() override
		{
			m_Result = (*m_func)(m_p1, m_p2, m_p3, m_p4, m_spState.get());
		}

		virtual void HandleException() override
		{
			if (m_ecb)
			{
				m_ecb((void*)m_func, m_strName.c_str());
			}
		}

	protected:
		R&				m_Result;
		R(*m_func)(T1, T2, T3, T4, IJobState*);
		void(*m_ecb)(void* func, const char* strName);
		T1				m_p1;
		T2				m_p2;
		T3				m_p3;
		T4				m_p4;
	};

	template<class TObj, class T1, class T2, class T3, class T4>
	class CAsynCallMemberEx4 : public CqlJob
	{
	public:
		CAsynCallMemberEx4(TObj* instance, R(TObj::*func)(T1, T2, T3, T4, IJobState*), T1 p1, T2 p2, T3 p3, T4 p4, R& result, const char* strName, TState* spState, void(TObj::*ecb)(void* func, const char* strName) = NULL) : CqlJob(spState, strName, TRUE), m_Result(result)
		{
			m_instance = instance;
			m_func = func;
			m_p1 = p1;
			m_p2 = p2;
			m_p3 = p3;
			m_p4 = p4;
			m_ecb = ecb;
		}

		virtual void InternalExecute() override
		{
			m_Result = (m_instance->*m_func)(m_p1, m_p2, m_p3, m_p4, m_spState.get());
		}

		virtual void HandleException() override
		{
			if (m_ecb)
			{
				(m_instance->*m_ecb)(*((void**)&m_func), m_strName.c_str());
			}
		}

	protected:
		R&				m_Result;
		TObj*			m_instance;
		R(TObj::*m_func)(T1, T2, T3, T4, IJobState*);
		void(TObj::*m_ecb)(void* func, const char* strName);
		T1				m_p1;
		T2				m_p2;
		T3				m_p3;
		T4				m_p4;
	};

	//////////////////////////////////////////////////////////////////////////
	template<class T1, class T2, class T3, class T4, class T5>
	class CAsynCall5 : public CqlJob
	{
	public:
		CAsynCall5(R(*func)(T1, T2, T3, T4, T5), T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, R& result, const char* strName, TState* spState, void(*ecb)(void* func, const char* strName) = NULL) : CqlJob(spState, strName, TRUE), m_Result(result)
		{
			m_func = func;
			m_p1 = p1;
			m_p2 = p2;
			m_p3 = p3;
			m_p4 = p4;
			m_p5 = p5;
			m_ecb = ecb;
		}

		virtual void InternalExecute() override
		{
			m_Result = (*m_func)(m_p1, m_p2, m_p3, m_p4, m_p5);
		}

		virtual void HandleException() override
		{
			if (m_ecb)
			{
				m_ecb((void*)m_func, m_strName.c_str());
			}
		}

	protected:
		R&				m_Result;
		R(*m_func)(T1, T2, T3, T4, T5);
		void(*m_ecb)(void* func, const char* strName);
		T1				m_p1;
		T2				m_p2;
		T3				m_p3;
		T4				m_p4;
		T5				m_p5;
	};

	template<class TObj, class T1, class T2, class T3, class T4, class T5>
	class CAsynCallMember5 : public CqlJob
	{
	public:
		CAsynCallMember5(TObj* instance, R(TObj::*func)(T1, T2, T3, T4, T5), T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, R& result, const char* strName, TState* spState, void(TObj::*ecb)(void* func, const char* strName) = NULL) : CqlJob(spState, strName, TRUE), m_Result(result)
		{
			m_instance = instance;
			m_func = func;
			m_p1 = p1;
			m_p2 = p2;
			m_p3 = p3;
			m_p4 = p4;
			m_p5 = p5;
			m_ecb = ecb;
		}

		virtual void InternalExecute() override
		{
			m_Result = (m_instance->*m_func)(m_p1, m_p2, m_p3, m_p4, m_p5);
		}

		virtual void HandleException() override
		{
			if (m_ecb)
			{
				(m_instance->*m_ecb)(*((void**)&m_func), m_strName.c_str());
			}
		}

	protected:
		R&				m_Result;
		TObj*			m_instance;
		R(TObj::*m_func)(T1, T2, T3, T4, T5);
		void(TObj::*m_ecb)(void* func, const char* strName);
		T1				m_p1;
		T2				m_p2;
		T3				m_p3;
		T4				m_p4;
		T5				m_p5;
	};

	template<class T1, class T2, class T3, class T4, class T5>
	class CAsynCallEx5 : public CqlJob
	{
	public:
		CAsynCallEx5(R(*func)(T1, T2, T3, T4, T5, IJobState*), T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, R& result, const char* strName, TState* spState, void(*ecb)(void* func, const char* strName) = NULL) : CqlJob(spState, strName, TRUE), m_Result(result)
		{
			m_func = func;
			m_p1 = p1;
			m_p2 = p2;
			m_p3 = p3;
			m_p4 = p4;
			m_p5 = p5;
			m_ecb = ecb;
		}

		virtual void InternalExecute() override
		{
			m_Result = (*m_func)(m_p1, m_p2, m_p3, m_p4, m_p5, m_spState.get());
		}

		virtual void HandleException() override
		{
			if (m_ecb)
			{
				m_ecb((void*)m_func, m_strName.c_str());
			}
		}

	protected:
		R&				m_Result;
		R(*m_func)(T1, T2, T3, T4, T5, IJobState*);
		void(*m_ecb)(void* func, const char* strName);
		T1				m_p1;
		T2				m_p2;
		T3				m_p3;
		T4				m_p4;
		T5				m_p5;
	};

	template<class TObj, class T1, class T2, class T3, class T4, class T5>
	class CAsynCallMemberEx5 : public CqlJob
	{
	public:
		CAsynCallMemberEx5(TObj* instance, R(TObj::*func)(T1, T2, T3, T4, T5, IJobState*), T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, R& result, const char* strName, TState* spState, void(TObj::*ecb)(void* func, const char* strName) = NULL) : CqlJob(spState, strName, TRUE), m_Result(result)
		{
			m_instance = instance;
			m_func = func;
			m_p1 = p1;
			m_p2 = p2;
			m_p3 = p3;
			m_p4 = p4;
			m_p5 = p5;
			m_ecb = ecb;
		}

		virtual void InternalExecute() override
		{
			m_Result = (m_instance->*m_func)(m_p1, m_p2, m_p3, m_p4, m_p5, m_spState.get());
		}

		virtual void HandleException() override
		{
			if (m_ecb)
			{
				(m_instance->*m_ecb)(*((void**)&m_func), m_strName.c_str());
			}
		}

	protected:
		R&				m_Result;
		TObj*			m_instance;
		R(TObj::*m_func)(T1, T2, T3, T4, T5, IJobState*);
		void(TObj::*m_ecb)(void* func, const char* strName);
		T1				m_p1;
		T2				m_p2;
		T3				m_p3;
		T4				m_p4;
		T5				m_p5;
	};

	//////////////////////////////////////////////////////////////////////////
	template<class T1, class T2, class T3, class T4, class T5, class T6>
	class CAsynCall6 : public CqlJob
	{
	public:
		CAsynCall6(R(*func)(T1, T2, T3, T4, T5, T6), T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, R& result, const char* strName, TState* spState, void(*ecb)(void* func, const char* strName) = NULL) : CqlJob(spState, strName, TRUE), m_Result(result)
		{
			m_func = func;
			m_p1 = p1;
			m_p2 = p2;
			m_p3 = p3;
			m_p4 = p4;
			m_p5 = p5;
			m_p6 = p6;
			m_ecb = ecb;
		}

		virtual void InternalExecute() override
		{
			m_Result = (*m_func)(m_p1, m_p2, m_p3, m_p4, m_p5, m_p6);
		}

		virtual void HandleException() override
		{
			if (m_ecb)
			{
				m_ecb((void*)m_func, m_strName.c_str());
			}
		}

	protected:
		R&				m_Result;
		R(*m_func)(T1, T2, T3, T4, T5, T6);
		void(*m_ecb)(void* func, const char* strName);
		T1				m_p1;
		T2				m_p2;
		T3				m_p3;
		T4				m_p4;
		T5				m_p5;
		T6				m_p6;
	};

	template<class TObj, class T1, class T2, class T3, class T4, class T5, class T6>
	class CAsynCallMember6 : public CqlJob
	{
	public:
		CAsynCallMember6(TObj* instance, R(TObj::*func)(T1, T2, T3, T4, T5, T6), T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, R& result, const char* strName, TState* spState, void(TObj::*ecb)(void* func, const char* strName) = NULL) : CqlJob(spState, strName, TRUE), m_Result(result)
		{
			m_instance = instance;
			m_func = func;
			m_p1 = p1;
			m_p2 = p2;
			m_p3 = p3;
			m_p4 = p4;
			m_p5 = p5;
			m_p6 = p6;
			m_ecb = ecb;
		}

		virtual void InternalExecute() override
		{
			m_Result = (m_instance->*m_func)(m_p1, m_p2, m_p3, m_p4, m_p5, m_p6);
		}

		virtual void HandleException() override
		{
			if (m_ecb)
			{
				(m_instance->*m_ecb)(*((void**)&m_func), m_strName.c_str());
			}
		}

	protected:
		R&				m_Result;
		TObj*			m_instance;
		R(TObj::*m_func)(T1, T2, T3, T4, T5, T6);
		void(TObj::*m_ecb)(void* func, const char* strName);
		T1				m_p1;
		T2				m_p2;
		T3				m_p3;
		T4				m_p4;
		T5				m_p5;
		T6				m_p6;
	};

	template<class T1, class T2, class T3, class T4, class T5, class T6>
	class CAsynCallEx6 : public CqlJob
	{
	public:
		CAsynCallEx6(R(*func)(T1, T2, T3, T4, T5, T6, IJobState*), T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, R& result, const char* strName, TState* spState, void(*ecb)(void* func, const char* strName) = NULL) : CqlJob(spState, strName, TRUE), m_Result(result)
		{
			m_func = func;
			m_p1 = p1;
			m_p2 = p2;
			m_p3 = p3;
			m_p4 = p4;
			m_p5 = p5;
			m_p6 = p6;
			m_ecb = ecb;
		}

		virtual void InternalExecute() override
		{
			m_Result = (*m_func)(m_p1, m_p2, m_p3, m_p4, m_p5, m_p6, m_spState.get());
		}

		virtual void HandleException() override
		{
			if (m_ecb)
			{
				m_ecb((void*)m_func, m_strName.c_str());
			}
		}

	protected:
		R&				m_Result;
		R(*m_func)(T1, T2, T3, T4, T5, T6, IJobState*);
		void(*m_ecb)(void* func, const char* strName);
		T1				m_p1;
		T2				m_p2;
		T3				m_p3;
		T4				m_p4;
		T5				m_p5;
		T6				m_p6;
	};

	template<class TObj, class T1, class T2, class T3, class T4, class T5, class T6>
	class CAsynCallMemberEx6 : public CqlJob
	{
	public:
		CAsynCallMemberEx6(TObj* instance, R(TObj::*func)(T1, T2, T3, T4, T5, T6, IJobState*), T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, R& result, const char* strName, TState* spState, void(TObj::*ecb)(void* func, const char* strName) = NULL) : CqlJob(spState, strName, TRUE), m_Result(result)
		{
			m_instance = instance;
			m_func = func;
			m_p1 = p1;
			m_p2 = p2;
			m_p3 = p3;
			m_p4 = p4;
			m_p5 = p5;
			m_p6 = p6;
			m_ecb = ecb;
		}

		virtual void InternalExecute() override
		{
			m_Result = (m_instance->*m_func)(m_p1, m_p2, m_p3, m_p4, m_p5, m_p6, m_spState.get());
		}

		virtual void HandleException() override
		{
			if (m_ecb)
			{
				(m_instance->*m_ecb)(*((void**)&m_func), m_strName.c_str());
			}
		}

	protected:
		R&				m_Result;
		TObj*			m_instance;
		R(TObj::*m_func)(T1, T2, T3, T4, T5, T6, IJobState*);
		void(TObj::*m_ecb)(void* func, const char* strName);
		T1				m_p1;
		T2				m_p2;
		T3				m_p3;
		T4				m_p4;
		T5				m_p5;
		T6				m_p6;
	};

	//////////////////////////////////////////////////////////////////////////
	template<class T1, class T2, class T3, class T4, class T5, class T6, class T7>
	class CAsynCall7 : public CqlJob
	{
	public:
		CAsynCall7(R(*func)(T1, T2, T3, T4, T5, T6, T7), T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, R& result, const char* strName, TState* spState, void(*ecb)(void* func, const char* strName) = NULL) : CqlJob(spState, strName, TRUE), m_Result(result)
		{
			m_func = func;
			m_p1 = p1;
			m_p2 = p2;
			m_p3 = p3;
			m_p4 = p4;
			m_p5 = p5;
			m_p6 = p6;
			m_p7 = p7;
			m_ecb = ecb;
		}

		virtual void InternalExecute() override
		{
			m_Result = (*m_func)(m_p1, m_p2, m_p3, m_p4, m_p5, m_p6, m_p7);
		}

		virtual void HandleException() override
		{
			if (m_ecb)
			{
				m_ecb((void*)m_func, m_strName.c_str());
			}
		}

	protected:
		R&				m_Result;
		R(*m_func)(T1, T2, T3, T4, T5, T6, T7);
		void(*m_ecb)(void* func, const char* strName);
		T1				m_p1;
		T2				m_p2;
		T3				m_p3;
		T4				m_p4;
		T5				m_p5;
		T6				m_p6;
		T7				m_p7;
	};

	template<class TObj, class T1, class T2, class T3, class T4, class T5, class T6, class T7>
	class CAsynCallMember7 : public CqlJob
	{
	public:
		CAsynCallMember7(TObj* instance, R(TObj::*func)(T1, T2, T3, T4, T5, T6, T7), T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, R& result, const char* strName, TState* spState, void(TObj::*ecb)(void* func, const char* strName) = NULL) : CqlJob(spState, strName, TRUE), m_Result(result)
		{
			m_instance = instance;
			m_func = func;
			m_p1 = p1;
			m_p2 = p2;
			m_p3 = p3;
			m_p4 = p4;
			m_p5 = p5;
			m_p6 = p6;
			m_p7 = p7;
			m_ecb = ecb;
		}

		virtual void InternalExecute() override
		{
			m_Result = (m_instance->*m_func)(m_p1, m_p2, m_p3, m_p4, m_p5, m_p6, m_p7);
		}

		virtual void HandleException() override
		{
			if (m_ecb)
			{
				(m_instance->*m_ecb)(*((void**)&m_func), m_strName.c_str());
			}
		}

	protected:
		R&				m_Result;
		TObj*			m_instance;
		R(TObj::*m_func)(T1, T2, T3, T4, T5, T6, T7);
		void(TObj::*m_ecb)(void* func, const char* strName);
		T1				m_p1;
		T2				m_p2;
		T3				m_p3;
		T4				m_p4;
		T5				m_p5;
		T6				m_p6;
		T7				m_p7;
	};

	template<class T1, class T2, class T3, class T4, class T5, class T6, class T7>
	class CAsynCallEx7 : public CqlJob
	{
	public:
		CAsynCallEx7(R(*func)(T1, T2, T3, T4, T5, T6, T7, IJobState*), T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, R& result, const char* strName, TState* spState, void(*ecb)(void* func, const char* strName) = NULL) : CqlJob(spState, strName, TRUE), m_Result(result)
		{
			m_func = func;
			m_p1 = p1;
			m_p2 = p2;
			m_p3 = p3;
			m_p4 = p4;
			m_p5 = p5;
			m_p6 = p6;
			m_p7 = p7;
			m_ecb = ecb;
		}

		virtual void InternalExecute() override
		{
			m_Result = (*m_func)(m_p1, m_p2, m_p3, m_p4, m_p5, m_p6, m_p7, m_spState.get());
		}

		virtual void HandleException() override
		{
			if (m_ecb)
			{
				m_ecb((void*)m_func, m_strName.c_str());
			}
		}
		
	protected:
		R&				m_Result;
		R(*m_func)(T1, T2, T3, T4, T5, T6, T7, IJobState*);
		void(*m_ecb)(void* func, const char* strName);
		T1				m_p1;
		T2				m_p2;
		T3				m_p3;
		T4				m_p4;
		T5				m_p5;
		T6				m_p6;
		T7				m_p7;
	};

	template<class TObj, class T1, class T2, class T3, class T4, class T5, class T6, class T7>
	class CAsynCallMemberEx7 : public CqlJob
	{
	public:
		CAsynCallMemberEx7(TObj* instance, R(TObj::*func)(T1, T2, T3, T4, T5, T6, T7, IJobState*), T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, R& result, const char* strName, TState* spState, void(TObj::*ecb)(void* func, const char* strName) = NULL) : CqlJob(spState, strName, TRUE), m_Result(result)
		{
			m_instance = instance;
			m_func = func;
			m_p1 = p1;
			m_p2 = p2;
			m_p3 = p3;
			m_p4 = p4;
			m_p5 = p5;
			m_p6 = p6;
			m_p7 = p7;
			m_ecb = ecb;
		}

		virtual void InternalExecute() override
		{
			m_Result = (m_instance->*m_func)(m_p1, m_p2, m_p3, m_p4, m_p5, m_p6, m_p7, m_spState.get());
		}

		virtual void HandleException() override
		{
			if (m_ecb)
			{
				(m_instance->*m_ecb)(*((void**)&m_func), m_strName.c_str());
			}
		}

	protected:
		R&				m_Result;
		TObj*			m_instance;
		R(TObj::*m_func)(T1, T2, T3, T4, T5, T6, T7, IJobState*);
		void(TObj::*m_ecb)(void* func, const char* strName);
		T1				m_p1;
		T2				m_p2;
		T3				m_p3;
		T4				m_p4;
		T5				m_p5;
		T6				m_p6;
		T7				m_p7;
	};

	//////////////////////////////////////////////////////////////////////////
	template<class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8>
	class CAsynCall8 : public CqlJob
	{
	public:
		CAsynCall8(R(*func)(T1, T2, T3, T4, T5, T6, T7, T8), T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, R& result, const char* strName, TState* spState, void(*ecb)(void* func, const char* strName) = NULL) : CqlJob(spState, strName, TRUE), m_Result(result)
		{
			m_func = func;
			m_p1 = p1;
			m_p2 = p2;
			m_p3 = p3;
			m_p4 = p4;
			m_p5 = p5;
			m_p6 = p6;
			m_p7 = p7;
			m_p8 = p8;
			m_ecb = ecb;
		}

		virtual void InternalExecute() override
		{
			m_Result = (*m_func)(m_p1, m_p2, m_p3, m_p4, m_p5, m_p6, m_p7, m_p8);
		}

		virtual void HandleException() override
		{
			if (m_ecb)
			{
				m_ecb((void*)m_func, m_strName.c_str());
			}
		}

	protected:
		R&				m_Result;
		R(*m_func)(T1, T2, T3, T4, T5, T6, T7, T8);
		void(*m_ecb)(void* func, const char* strName);
		T1				m_p1;
		T2				m_p2;
		T3				m_p3;
		T4				m_p4;
		T5				m_p5;
		T6				m_p6;
		T7				m_p7;
		T8				m_p8;
	};

	template<class TObj, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8>
	class CAsynCallMember8 : public CqlJob
	{
	public:
		CAsynCallMember8(TObj* instance, R(TObj::*func)(T1, T2, T3, T4, T5, T6, T7, T8), T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, R& result, const char* strName, TState* spState, void(TObj::*ecb)(void* func, const char* strName) = NULL) : CqlJob(spState, strName, TRUE), m_Result(result)
		{
			m_instance = instance;
			m_func = func;
			m_p1 = p1;
			m_p2 = p2;
			m_p3 = p3;
			m_p4 = p4;
			m_p5 = p5;
			m_p6 = p6;
			m_p7 = p7;
			m_p8 = p8;
			m_ecb = ecb;
		}

		virtual void InternalExecute() override
		{
			m_Result = (m_instance->*m_func)(m_p1, m_p2, m_p3, m_p4, m_p5, m_p6, m_p7, m_p8);
		}

		virtual void HandleException() override
		{
			if (m_ecb)
			{
				(m_instance->*m_ecb)(*((void**)&m_func), m_strName.c_str());
			}
		}

	protected:
		R&				m_Result;
		TObj*			m_instance;
		R(TObj::*m_func)(T1, T2, T3, T4, T5, T6, T7, T8);
		void(TObj::*m_ecb)(void* func, const char* strName);
		T1				m_p1;
		T2				m_p2;
		T3				m_p3;
		T4				m_p4;
		T5				m_p5;
		T6				m_p6;
		T7				m_p7;
		T8				m_p8;
	};

	template<class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8>
	class CAsynCallEx8 : public CqlJob
	{
	public:
		CAsynCallEx8(R(*func)(T1, T2, T3, T4, T5, T6, T7, T8, IJobState*), T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, R& result, const char* strName, TState* spState, void(*ecb)(void* func, const char* strName) = NULL) : CqlJob(spState, strName, TRUE), m_Result(result)
		{
			m_func = func;
			m_p1 = p1;
			m_p2 = p2;
			m_p3 = p3;
			m_p4 = p4;
			m_p5 = p5;
			m_p6 = p6;
			m_p7 = p7;
			m_p8 = p8;
			m_ecb = ecb;
		}

		virtual void InternalExecute() override
		{
			m_Result = (*m_func)(m_p1, m_p2, m_p3, m_p4, m_p5, m_p6, m_p7, m_p8, m_spState.get());
		}

		virtual void HandleException() override
		{
			if (m_ecb)
			{
				m_ecb((void*)m_func, m_strName.c_str());
			}
		}

	protected:
		R&				m_Result;
		R(*m_func)(T1, T2, T3, T4, T5, T6, T7, T8, IJobState*);
		void(*m_ecb)(void* func, const char* strName);
		T1				m_p1;
		T2				m_p2;
		T3				m_p3;
		T4				m_p4;
		T5				m_p5;
		T6				m_p6;
		T7				m_p7;
		T8				m_p8;
	};

	template<class TObj, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8>
	class CAsynCallMemberEx8 : public CqlJob
	{
	public:
		CAsynCallMemberEx8(TObj* instance, R(TObj::*func)(T1, T2, T3, T4, T5, T6, T7, T8, IJobState*), T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, R& result, const char* strName, TState* spState, void(TObj::*ecb)(void* func, const char* strName) = NULL) : CqlJob(spState, strName, TRUE), m_Result(result)
		{
			m_instance = instance;
			m_func = func;
			m_p1 = p1;
			m_p2 = p2;
			m_p3 = p3;
			m_p4 = p4;
			m_p5 = p5;
			m_p6 = p6;
			m_p7 = p7;
			m_p8 = p8;
			m_ecb = ecb;
		}

		virtual void InternalExecute() override
		{
			m_Result = (m_instance->*m_func)(m_p1, m_p2, m_p3, m_p4, m_p5, m_p6, m_p7, m_p8, m_spState.get());
		}

		virtual void HandleException() override
		{
			if (m_ecb)
			{
				(m_instance->*m_ecb)(*((void**)&m_func), m_strName.c_str());
			}
		}

	protected:
		R&				m_Result;
		TObj*			m_instance;
		R(TObj::*m_func)(T1, T2, T3, T4, T5, T6, T7, T8, IJobState*);
		void(TObj::*m_ecb)(void* func, const char* strName);
		T1				m_p1;
		T2				m_p2;
		T3				m_p3;
		T4				m_p4;
		T5				m_p5;
		T6				m_p6;
		T7				m_p7;
		T8				m_p8;
	};

	//////////////////////////////////////////////////////////////////////////
	template<class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9>
	class CAsynCall9 : public CqlJob
	{
	public:
		CAsynCall9(R(*func)(T1, T2, T3, T4, T5, T6, T7, T8, T9), T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, R& result, const char* strName, TState* spState, void(*ecb)(void* func, const char* strName) = NULL) : CqlJob(spState, strName, TRUE), m_Result(result)
		{
			m_func = func;
			m_p1 = p1;
			m_p2 = p2;
			m_p3 = p3;
			m_p4 = p4;
			m_p5 = p5;
			m_p6 = p6;
			m_p7 = p7;
			m_p8 = p8;
			m_p9 = p9;
			m_ecb = ecb;
		}

		virtual void InternalExecute() override
		{
			m_Result = (*m_func)(m_p1, m_p2, m_p3, m_p4, m_p5, m_p6, m_p7, m_p8, m_p9);
		}

		virtual void HandleException() override
		{
			if (m_ecb)
			{
				m_ecb((void*)m_func, m_strName.c_str());
			}
		}
		
	protected:
		R&				m_Result;
		R(*m_func)(T1, T2, T3, T4, T5, T6, T7, T8, T9);
		void(*m_ecb)(void* func, const char* strName);
		T1				m_p1;
		T2				m_p2;
		T3				m_p3;
		T4				m_p4;
		T5				m_p5;
		T6				m_p6;
		T7				m_p7;
		T8				m_p8;
		T9				m_p9;
	};

	template<class TObj, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9>
	class CAsynCallMember9 : public CqlJob
	{
	public:
		CAsynCallMember9(TObj* instance, R(TObj::*func)(T1, T2, T3, T4, T5, T6, T7, T8, T9), T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, R& result, const char* strName, TState* spState, void(TObj::*ecb)(void* func, const char* strName) = NULL) : CqlJob(spState, strName, TRUE), m_Result(result)
		{
			m_instance = instance;
			m_func = func;
			m_p1 = p1;
			m_p2 = p2;
			m_p3 = p3;
			m_p4 = p4;
			m_p5 = p5;
			m_p6 = p6;
			m_p7 = p7;
			m_p8 = p8;
			m_p9 = p9;
			m_ecb = ecb;
		}

		virtual void InternalExecute() override
		{
			m_Result = (m_instance->*m_func)(m_p1, m_p2, m_p3, m_p4, m_p5, m_p6, m_p7, m_p8, m_p9);
		}

		virtual void HandleException() override
		{
			if (m_ecb)
			{
				(m_instance->*m_ecb)(*((void**)&m_func), m_strName.c_str());
			}
		}

	protected:
		R&				m_Result;
		TObj*			m_instance;
		R(TObj::*m_func)(T1, T2, T3, T4, T5, T6, T7, T8, T9);
		void(TObj::*m_ecb)(void* func, const char* strName);
		T1				m_p1;
		T2				m_p2;
		T3				m_p3;
		T4				m_p4;
		T5				m_p5;
		T6				m_p6;
		T7				m_p7;
		T8				m_p8;
		T9				m_p9;
	};

	template<class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9>
	class CAsynCallEx9 : public CqlJob
	{
	public:
		CAsynCallEx9(R(*func)(T1, T2, T3, T4, T5, T6, T7, T8, T9, IJobState*), T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, R& result, const char* strName, TState* spState, void(*ecb)(void* func, const char* strName) = NULL) : CqlJob(spState, strName, TRUE), m_Result(result)
		{
			m_func = func;
			m_p1 = p1;
			m_p2 = p2;
			m_p3 = p3;
			m_p4 = p4;
			m_p5 = p5;
			m_p6 = p6;
			m_p7 = p7;
			m_p8 = p8;
			m_p9 = p9;
			m_ecb = ecb;
		}

		virtual void InternalExecute() override
		{
			m_Result = (*m_func)(m_p1, m_p2, m_p3, m_p4, m_p5, m_p6, m_p7, m_p8, m_p9, m_spState.get());
		}

		virtual void HandleException() override
		{
			if (m_ecb)
			{
				m_ecb((void*)m_func, m_strName.c_str());
			}
		}

	protected:
		R&				m_Result;
		R(*m_func)(T1, T2, T3, T4, T5, T6, T7, T8, T9, IJobState*);
		void(*m_ecb)(void* func, const char* strName);
		T1				m_p1;
		T2				m_p2;
		T3				m_p3;
		T4				m_p4;
		T5				m_p5;
		T6				m_p6;
		T7				m_p7;
		T8				m_p8;
		T9				m_p9;
	};

	template<class TObj, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9>
	class CAsynCallMemberEx9 : public CqlJob
	{
	public:
		CAsynCallMemberEx9(TObj* instance, R(TObj::*func)(T1, T2, T3, T4, T5, T6, T7, T8, T9, IJobState*), T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, R& result, const char* strName, TState* spState, void(TObj::*ecb)(void* func, const char* strName) = NULL) : CqlJob(spState, strName, TRUE), m_Result(result)
		{
			m_instance = instance;
			m_func = func;
			m_p1 = p1;
			m_p2 = p2;
			m_p3 = p3;
			m_p4 = p4;
			m_p5 = p5;
			m_p6 = p6;
			m_p7 = p7;
			m_p8 = p8;
			m_p9 = p9;
			m_ecb = ecb;
		}

		virtual void InternalExecute() override
		{
			m_Result = (m_instance->*m_func)(m_p1, m_p2, m_p3, m_p4, m_p5, m_p6, m_p7, m_p8, m_p9, m_spState.get());
		}

		virtual void HandleException() override
		{
			if (m_ecb)
			{
				(m_instance->*m_ecb)(*((void**)&m_func), m_strName.c_str());
			}
		}
		
	protected:
		R&				m_Result;
		TObj*			m_instance;
		R(TObj::*m_func)(T1, T2, T3, T4, T5, T6, T7, T8, T9, IJobState*);
		void(TObj::*m_ecb)(void* func, const char* strName);
		T1				m_p1;
		T2				m_p2;
		T3				m_p3;
		T4				m_p4;
		T5				m_p5;
		T6				m_p6;
		T7				m_p7;
		T8				m_p8;
		T9				m_p9;
	};

public:
	//////////////////////////////////////////////////////////////////////////
	BOOL AsynCall(R(*func)(), IScheduler* pScheduler, const char* strName = nullptr, void(*ecb)(void* func, const char* strName) = NULL)
	{
		assert(func);
		assert(pScheduler);
		std::shared_ptr<CqlJobStateR<R>>& l_spState = ResetState(pScheduler);
		TState l_spStateBase = std::dynamic_pointer_cast<IJobState>(l_spState);
		IJob* l_pJob = new CAsynCall0(func, l_spState->m_Result, strName, &l_spStateBase, ecb);
		assert(l_pJob);
		BOOL l_bRet = pScheduler->Schedule(l_pJob);
		assert(l_bRet);
		return l_bRet;
	};

	template<class TObj>
	BOOL AsynCall(TObj* instance, R(TObj::*func)(), IScheduler* pScheduler, const char* strName = nullptr, void(TObj::*ecb)(void* func, const char* strName) = NULL)
	{
		assert(func);
		assert(instance);
		assert(pScheduler);
		std::shared_ptr<CqlJobStateR<R>>& l_spState = ResetState(pScheduler);
		TState l_spStateBase = std::dynamic_pointer_cast<IJobState>(l_spState);
		IJob* l_pJob = new CAsynCallMember0<TObj>(instance, func, l_spState->m_Result, strName, &l_spStateBase, ecb);
		assert(l_pJob);
		BOOL l_bRet = pScheduler->Schedule(l_pJob);
		assert(l_bRet);
		return l_bRet;
	};

	BOOL AsynCallEx(R(*func)(IJobState*), IScheduler* pScheduler, const char* strName = nullptr, void(*ecb)(void* func, const char* strName) = NULL)
	{
		assert(func);
		assert(pScheduler);
		std::shared_ptr<CqlJobStateR<R>>& l_spState = ResetState(pScheduler);
		TState l_spStateBase = std::dynamic_pointer_cast<IJobState>(l_spState);
		IJob* l_pJob = new CAsynCallEx0(func, l_spState->m_Result, strName, &l_spStateBase, ecb);
		assert(l_pJob);
		BOOL l_bRet = pScheduler->Schedule(l_pJob);
		assert(l_bRet);
		return l_bRet;
	};

	template<class TObj>
	BOOL AsynCallEx(TObj* instance, R(TObj::*func)(IJobState*), IScheduler* pScheduler, const char* strName = nullptr, void(TObj::*ecb)(void* func, const char* strName) = NULL)
	{
		assert(func);
		assert(instance);
		assert(pScheduler);
		std::shared_ptr<CqlJobStateR<R>>& l_spState = ResetState(pScheduler);
		TState l_spStateBase = std::dynamic_pointer_cast<IJobState>(l_spState);
		IJob* l_pJob = new CAsynCallMemberEx0<TObj>(instance, func, l_spState->m_Result, strName, &l_spStateBase, ecb);
		assert(l_pJob);
		BOOL l_bRet = pScheduler->Schedule(l_pJob);
		assert(l_bRet);
		return l_bRet;
	};

	//////////////////////////////////////////////////////////////////////////
	template<class T1>
	BOOL AsynCall(R(*func)(T1), T1 p1, IScheduler* pScheduler, const char* strName = nullptr, void(*ecb)(void* func, const char* strName) = NULL)
	{
		assert(func);
		assert(pScheduler);
		std::shared_ptr<CqlJobStateR<R>>& l_spState = ResetState(pScheduler);
		TState l_spStateBase = std::dynamic_pointer_cast<IJobState>(l_spState);
		IJob* l_pJob = new CAsynCall1<T1>(func, p1, l_spState->m_Result, strName, &l_spStateBase, ecb);
		assert(l_pJob);
		BOOL l_bRet = pScheduler->Schedule(l_pJob);
		assert(l_bRet);
		return l_bRet;
	};

	template<class TObj, class T1>
	BOOL AsynCall(TObj* instance, R(TObj::*func)(T1), T1 p1, IScheduler* pScheduler, const char* strName = nullptr, void(TObj::*ecb)(void* func, const char* strName) = NULL)
	{
		assert(func);
		assert(instance);
		assert(pScheduler);
		std::shared_ptr<CqlJobStateR<R>>& l_spState = ResetState(pScheduler);
		TState l_spStateBase = std::dynamic_pointer_cast<IJobState>(l_spState);
		IJob* l_pJob = new CAsynCallMember1<TObj, T1>(instance, func, p1, l_spState->m_Result, strName, &l_spStateBase, ecb);
		assert(l_pJob);
		BOOL l_bRet = pScheduler->Schedule(l_pJob);
		assert(l_bRet);
		return l_bRet;
	};

	template<class T1>
	BOOL AsynCallEx(R(*func)(T1, IJobState*), T1 p1, IScheduler* pScheduler, const char* strName = nullptr, void(*ecb)(void* func, const char* strName) = NULL)
	{
		assert(func);
		assert(pScheduler);
		std::shared_ptr<CqlJobStateR<R>>& l_spState = ResetState(pScheduler);
		TState l_spStateBase = std::dynamic_pointer_cast<IJobState>(l_spState);
		IJob* l_pJob = new CAsynCallEx1<T1>(func, p1, l_spState->m_Result, strName, &l_spStateBase, ecb);
		assert(l_pJob);
		BOOL l_bRet = pScheduler->Schedule(l_pJob);
		assert(l_bRet);
		return l_bRet;
	};

	template<class TObj, class T1>
	BOOL AsynCallEx(TObj* instance, R(TObj::*func)(T1, IJobState*), T1 p1, IScheduler* pScheduler, const char* strName = nullptr, void(TObj::*ecb)(void* func, const char* strName) = NULL)
	{
		assert(func);
		assert(instance);
		assert(pScheduler);
		std::shared_ptr<CqlJobStateR<R>>& l_spState = ResetState(pScheduler);
		TState l_spStateBase = std::dynamic_pointer_cast<IJobState>(l_spState);
		IJob* l_pJob = new CAsynCallMemberEx1<TObj, T1>(instance, func, p1, l_spState->m_Result, strName, &l_spStateBase, ecb);
		assert(l_pJob);
		BOOL l_bRet = pScheduler->Schedule(l_pJob);
		assert(l_bRet);
		return l_bRet;
	};

	//////////////////////////////////////////////////////////////////////////
	template<class T1, class T2>
	BOOL AsynCall(R(*func)(T1, T2), T1 p1, T2 p2, IScheduler* pScheduler, const char* strName = nullptr, void(*ecb)(void* func, const char* strName) = NULL)
	{
		assert(func);
		assert(pScheduler);
		std::shared_ptr<CqlJobStateR<R>>& l_spState = ResetState(pScheduler);
		TState l_spStateBase = std::dynamic_pointer_cast<IJobState>(l_spState);
		IJob* l_pJob = new CAsynCall2<T1, T2>(func, p1, p2, l_spState->m_Result, strName, &l_spStateBase, ecb);
		assert(l_pJob);
		BOOL l_bRet = pScheduler->Schedule(l_pJob);
		assert(l_bRet);
		return l_bRet;
	};

	template<class TObj, class T1, class T2>
	BOOL AsynCall(TObj* instance, R(TObj::*func)(T1, T2), T1 p1, T2 p2, IScheduler* pScheduler, const char* strName = nullptr, void(TObj::*ecb)(void* func, const char* strName) = NULL)
	{
		assert(func);
		assert(instance);
		assert(pScheduler);
		std::shared_ptr<CqlJobStateR<R>>& l_spState = ResetState(pScheduler);
		TState l_spStateBase = std::dynamic_pointer_cast<IJobState>(l_spState);
		IJob* l_pJob = new CAsynCallMember2<TObj, T1, T2>(instance, func, p1, p2, l_spState->m_Result, strName, &l_spStateBase, ecb);
		assert(l_pJob);
		BOOL l_bRet = pScheduler->Schedule(l_pJob);
		assert(l_bRet);
		return l_bRet;
	};

	template<class T1, class T2>
	BOOL AsynCallEx(R(*func)(T1, T2, IJobState*), T1 p1, T2 p2, IScheduler* pScheduler, const char* strName = nullptr, void(*ecb)(void* func, const char* strName) = NULL)
	{
		assert(func);
		assert(pScheduler);
		std::shared_ptr<CqlJobStateR<R>>& l_spState = ResetState(pScheduler);
		TState l_spStateBase = std::dynamic_pointer_cast<IJobState>(l_spState);
		IJob* l_pJob = new CAsynCallEx2<T1, T2>(func, p1, p2, l_spState->m_Result, strName, &l_spStateBase, ecb);
		assert(l_pJob);
		BOOL l_bRet = pScheduler->Schedule(l_pJob);
		assert(l_bRet);
		return l_bRet;
	};

	template<class TObj, class T1, class T2>
	BOOL AsynCallEx(TObj* instance, R(TObj::*func)(T1, T2, IJobState*), T1 p1, T2 p2, IScheduler* pScheduler, const char* strName = nullptr, void(TObj::*ecb)(void* func, const char* strName) = NULL)
	{
		assert(func);
		assert(instance);
		assert(pScheduler);
		std::shared_ptr<CqlJobStateR<R>>& l_spState = ResetState(pScheduler);
		TState l_spStateBase = std::dynamic_pointer_cast<IJobState>(l_spState);
		IJob* l_pJob = new CAsynCallMemberEx2<TObj, T1, T2>(instance, func, p1, p2, l_spState->m_Result, strName, &l_spStateBase, ecb);
		assert(l_pJob);
		BOOL l_bRet = pScheduler->Schedule(l_pJob);
		assert(l_bRet);
		return l_bRet;
	};

	//////////////////////////////////////////////////////////////////////////
	template<class T1, class T2, class T3>
	BOOL AsynCall(R(*func)(T1, T2, T3), T1 p1, T2 p2, T3 p3, IScheduler* pScheduler, const char* strName = nullptr, void(*ecb)(void* func, const char* strName) = NULL)
	{
		assert(func);
		assert(pScheduler);
		std::shared_ptr<CqlJobStateR<R>>& l_spState = ResetState(pScheduler);
		TState l_spStateBase = std::dynamic_pointer_cast<IJobState>(l_spState);
		IJob* l_pJob = new CAsynCall3<T1, T2, T3>(func, p1, p2, p3, l_spState->m_Result, strName, &l_spStateBase, ecb);
		assert(l_pJob);
		BOOL l_bRet = pScheduler->Schedule(l_pJob);
		assert(l_bRet);
		return l_bRet;
	};

	template<class TObj, class T1, class T2, class T3>
	BOOL AsynCall(TObj* instance, R(TObj::*func)(T1, T2, T3), T1 p1, T2 p2, T3 p3, IScheduler* pScheduler, const char* strName = nullptr, void(TObj::*ecb)(void* func, const char* strName) = NULL)
	{
		assert(func);
		assert(instance);
		assert(pScheduler);
		std::shared_ptr<CqlJobStateR<R>>& l_spState = ResetState(pScheduler);
		TState l_spStateBase = std::dynamic_pointer_cast<IJobState>(l_spState);
		IJob* l_pJob = new CAsynCallMember3<TObj, T1, T2, T3>(instance, func, p1, p2, p3, l_spState->m_Result, strName, &l_spStateBase, ecb);
		assert(l_pJob);
		BOOL l_bRet = pScheduler->Schedule(l_pJob);
		assert(l_bRet);
		return l_bRet;
	};

	template<class T1, class T2, class T3>
	BOOL AsynCallEx(R(*func)(T1, T2, T3, IJobState*), T1 p1, T2 p2, T3 p3, IScheduler* pScheduler, const char* strName = nullptr, void(*ecb)(void* func, const char* strName) = NULL)
	{
		assert(func);
		assert(pScheduler);
		std::shared_ptr<CqlJobStateR<R>>& l_spState = ResetState(pScheduler);
		TState l_spStateBase = std::dynamic_pointer_cast<IJobState>(l_spState);
		IJob* l_pJob = new CAsynCallEx3<T1, T2, T3>(func, p1, p2, p3, l_spState->m_Result, strName, &l_spStateBase, ecb);
		assert(l_pJob);
		BOOL l_bRet = pScheduler->Schedule(l_pJob);
		assert(l_bRet);
		return l_bRet;
	};

	template<class TObj, class T1, class T2, class T3>
	BOOL AsynCallEx(TObj* instance, R(TObj::*func)(T1, T2, T3, IJobState*), T1 p1, T2 p2, T3 p3, IScheduler* pScheduler, const char* strName = nullptr, void(TObj::*ecb)(void* func, const char* strName) = NULL)
	{
		assert(func);
		assert(instance);
		assert(pScheduler);
		std::shared_ptr<CqlJobStateR<R>>& l_spState = ResetState(pScheduler);
		TState l_spStateBase = std::dynamic_pointer_cast<IJobState>(l_spState);
		IJob* l_pJob = new CAsynCallMemberEx3<TObj, T1, T2, T3>(instance, func, p1, p2, p3, l_spState->m_Result, strName, &l_spStateBase, ecb);
		assert(l_pJob);
		BOOL l_bRet = pScheduler->Schedule(l_pJob);
		assert(l_bRet);
		return l_bRet;
	};

	//////////////////////////////////////////////////////////////////////////
	template<class T1, class T2, class T3, class T4>
	BOOL AsynCall(R(*func)(T1, T2, T3, T4), T1 p1, T2 p2, T3 p3, T4 p4, IScheduler* pScheduler, const char* strName = nullptr, void(*ecb)(void* func, const char* strName) = NULL)
	{
		assert(func);
		assert(pScheduler);
		std::shared_ptr<CqlJobStateR<R>>& l_spState = ResetState(pScheduler);
		TState l_spStateBase = std::dynamic_pointer_cast<IJobState>(l_spState);
		IJob* l_pJob = new CAsynCall4<T1, T2, T3, T4>(func, p1, p2, p3, p4, l_spState->m_Result, strName, &l_spStateBase, ecb);
		assert(l_pJob);
		BOOL l_bRet = pScheduler->Schedule(l_pJob);
		assert(l_bRet);
		return l_bRet;
	};

	template<class TObj, class T1, class T2, class T3, class T4>
	BOOL AsynCall(TObj* instance, R(TObj::*func)(T1, T2, T3, T4), T1 p1, T2 p2, T3 p3, T4 p4, IScheduler* pScheduler, const char* strName = nullptr, void(TObj::*ecb)(void* func, const char* strName) = NULL)
	{
		assert(func);
		assert(instance);
		assert(pScheduler);
		std::shared_ptr<CqlJobStateR<R>>& l_spState = ResetState(pScheduler);
		TState l_spStateBase = std::dynamic_pointer_cast<IJobState>(l_spState);
		IJob* l_pJob = new CAsynCallMember4<TObj, T1, T2, T3, T4>(instance, func, p1, p2, p3, p4, l_spState->m_Result, strName, &l_spStateBase, ecb);
		assert(l_pJob);
		BOOL l_bRet = pScheduler->Schedule(l_pJob);
		assert(l_bRet);
		return l_bRet;
	};

	template<class T1, class T2, class T3, class T4>
	BOOL AsynCallEx(R(*func)(T1, T2, T3, T4, IJobState*), T1 p1, T2 p2, T3 p3, T4 p4, IScheduler* pScheduler, const char* strName = nullptr, void(*ecb)(void* func, const char* strName) = NULL)
	{
		assert(func);
		assert(pScheduler);
		std::shared_ptr<CqlJobStateR<R>>& l_spState = ResetState(pScheduler);
		TState l_spStateBase = std::dynamic_pointer_cast<IJobState>(l_spState);
		IJob* l_pJob = new CAsynCallEx4<T1, T2, T3, T4>(func, p1, p2, p3, p4, l_spState->m_Result, strName, &l_spStateBase, ecb);
		assert(l_pJob);
		BOOL l_bRet = pScheduler->Schedule(l_pJob);
		assert(l_bRet);
		return l_bRet;
	};

	template<class TObj, class T1, class T2, class T3, class T4>
	BOOL AsynCallEx(TObj* instance, R(TObj::*func)(T1, T2, T3, T4, IJobState*), T1 p1, T2 p2, T3 p3, T4 p4, IScheduler* pScheduler, const char* strName = nullptr, void(TObj::*ecb)(void* func, const char* strName) = NULL)
	{
		assert(func);
		assert(instance);
		assert(pScheduler);
		std::shared_ptr<CqlJobStateR<R>>& l_spState = ResetState(pScheduler);
		TState l_spStateBase = std::dynamic_pointer_cast<IJobState>(l_spState);
		IJob* l_pJob = new CAsynCallMemberEx4<TObj, T1, T2, T3, T4>(instance, func, p1, p2, p3, p4, l_spState->m_Result, strName, &l_spStateBase, ecb);
		assert(l_pJob);
		BOOL l_bRet = pScheduler->Schedule(l_pJob);
		assert(l_bRet);
		return l_bRet;
	};

	//////////////////////////////////////////////////////////////////////////
	template<class T1, class T2, class T3, class T4, class T5>
	BOOL AsynCall(R(*func)(T1, T2, T3, T4, T5), T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, IScheduler* pScheduler, const char* strName = nullptr, void(*ecb)(void* func, const char* strName) = NULL)
	{
		assert(func);
		assert(pScheduler);
		std::shared_ptr<CqlJobStateR<R>>& l_spState = ResetState(pScheduler);
		TState l_spStateBase = std::dynamic_pointer_cast<IJobState>(l_spState);
		IJob* l_pJob = new CAsynCall5<T1, T2, T3, T4, T5>(func, p1, p2, p3, p4, p5, l_spState->m_Result, strName, &l_spStateBase, ecb);
		assert(l_pJob);
		BOOL l_bRet = pScheduler->Schedule(l_pJob);
		assert(l_bRet);
		return l_bRet;
	};

	template<class TObj, class T1, class T2, class T3, class T4, class T5>
	BOOL AsynCall(TObj* instance, R(TObj::*func)(T1, T2, T3, T4, T5), T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, IScheduler* pScheduler, const char* strName = nullptr, void(TObj::*ecb)(void* func, const char* strName) = NULL)
	{
		assert(func);
		assert(instance);
		assert(pScheduler);
		std::shared_ptr<CqlJobStateR<R>>& l_spState = ResetState(pScheduler);
		TState l_spStateBase = std::dynamic_pointer_cast<IJobState>(l_spState);
		IJob* l_pJob = new CAsynCallMember5<TObj, T1, T2, T3, T4, T5>(instance, func, p1, p2, p3, p4, p5, l_spState->m_Result, strName, &l_spStateBase, ecb);
		assert(l_pJob);
		BOOL l_bRet = pScheduler->Schedule(l_pJob);
		assert(l_bRet);
		return l_bRet;
	};

	template<class T1, class T2, class T3, class T4, class T5>
	BOOL AsynCallEx(R(*func)(T1, T2, T3, T4, T5, IJobState*), T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, IScheduler* pScheduler, const char* strName = nullptr, void(*ecb)(void* func, const char* strName) = NULL)
	{
		assert(func);
		assert(pScheduler);
		std::shared_ptr<CqlJobStateR<R>>& l_spState = ResetState(pScheduler);
		TState l_spStateBase = std::dynamic_pointer_cast<IJobState>(l_spState);
		IJob* l_pJob = new CAsynCallEx5<T1, T2, T3, T4, T5>(func, p1, p2, p3, p4, p5, l_spState->m_Result, strName, &l_spStateBase, ecb);
		assert(l_pJob);
		BOOL l_bRet = pScheduler->Schedule(l_pJob);
		assert(l_bRet);
		return l_bRet;
	};

	template<class TObj, class T1, class T2, class T3, class T4, class T5>
	BOOL AsynCallEx(TObj* instance, R(TObj::*func)(T1, T2, T3, T4, T5, IJobState*), T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, IScheduler* pScheduler, const char* strName = nullptr, void(TObj::*ecb)(void* func, const char* strName) = NULL)
	{
		assert(func);
		assert(instance);
		assert(pScheduler);
		std::shared_ptr<CqlJobStateR<R>>& l_spState = ResetState(pScheduler);
		TState l_spStateBase = std::dynamic_pointer_cast<IJobState>(l_spState);
		IJob* l_pJob = new CAsynCallMemberEx5<TObj, T1, T2, T3, T4, T5>(instance, func, p1, p2, p3, p4, p5, l_spState->m_Result, strName, &l_spStateBase, ecb);
		assert(l_pJob);
		BOOL l_bRet = pScheduler->Schedule(l_pJob);
		assert(l_bRet);
		return l_bRet;
	};

	//////////////////////////////////////////////////////////////////////////
	template<class T1, class T2, class T3, class T4, class T5, class T6>
	BOOL AsynCall(R(*func)(T1, T2, T3, T4, T5, T6), T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, IScheduler* pScheduler, const char* strName = nullptr, void(*ecb)(void* func, const char* strName) = NULL)
	{
		assert(func);
		assert(pScheduler);
		std::shared_ptr<CqlJobStateR<R>>& l_spState = ResetState(pScheduler);
		TState l_spStateBase = std::dynamic_pointer_cast<IJobState>(l_spState);
		IJob* l_pJob = new CAsynCall6<T1, T2, T3, T4, T5, T6>(func, p1, p2, p3, p4, p5, p6, l_spState->m_Result, strName, &l_spStateBase, ecb);
		assert(l_pJob);
		BOOL l_bRet = pScheduler->Schedule(l_pJob);
		assert(l_bRet);
		return l_bRet;
	};

	template<class TObj, class T1, class T2, class T3, class T4, class T5, class T6>
	BOOL AsynCall(TObj* instance, R(TObj::*func)(T1, T2, T3, T4, T5, T6), T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, IScheduler* pScheduler, const char* strName = nullptr, void(TObj::*ecb)(void* func, const char* strName) = NULL)
	{
		assert(func);
		assert(instance);
		assert(pScheduler);
		std::shared_ptr<CqlJobStateR<R>>& l_spState = ResetState(pScheduler);
		TState l_spStateBase = std::dynamic_pointer_cast<IJobState>(l_spState);
		IJob* l_pJob = new CAsynCallMember6<TObj, T1, T2, T3, T4, T5, T6>(instance, func, p1, p2, p3, p4, p5, p6, l_spState->m_Result, strName, &l_spStateBase, ecb);
		assert(l_pJob);
		BOOL l_bRet = pScheduler->Schedule(l_pJob);
		assert(l_bRet);
		return l_bRet;
	};

	template<class T1, class T2, class T3, class T4, class T5, class T6>
	BOOL AsynCallEx(R(*func)(T1, T2, T3, T4, T5, T6, IJobState*), T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, IScheduler* pScheduler, const char* strName = nullptr, void(*ecb)(void* func, const char* strName) = NULL)
	{
		assert(func);
		assert(pScheduler);
		std::shared_ptr<CqlJobStateR<R>>& l_spState = ResetState(pScheduler);
		TState l_spStateBase = std::dynamic_pointer_cast<IJobState>(l_spState);
		IJob* l_pJob = new CAsynCallEx6<T1, T2, T3, T4, T5, T6>(func, p1, p2, p3, p4, p5, p6, l_spState->m_Result, strName, &l_spStateBase, ecb);
		assert(l_pJob);
		BOOL l_bRet = pScheduler->Schedule(l_pJob);
		assert(l_bRet);
		return l_bRet;
	};

	template<class TObj, class T1, class T2, class T3, class T4, class T5, class T6>
	BOOL AsynCallEx(TObj* instance, R(TObj::*func)(T1, T2, T3, T4, T5, T6, IJobState*), T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, IScheduler* pScheduler, const char* strName = nullptr, void(TObj::*ecb)(void* func, const char* strName) = NULL)
	{
		assert(func);
		assert(instance);
		assert(pScheduler);
		std::shared_ptr<CqlJobStateR<R>>& l_spState = ResetState(pScheduler);
		TState l_spStateBase = std::dynamic_pointer_cast<IJobState>(l_spState);
		IJob* l_pJob = new CAsynCallMemberEx6<TObj, T1, T2, T3, T4, T5, T6>(instance, func, p1, p2, p3, p4, p5, p6, l_spState->m_Result, strName, &l_spStateBase, ecb);
		assert(l_pJob);
		BOOL l_bRet = pScheduler->Schedule(l_pJob);
		assert(l_bRet);
		return l_bRet;
	};

	//////////////////////////////////////////////////////////////////////////
	template<class T1, class T2, class T3, class T4, class T5, class T6, class T7>
	BOOL AsynCall(R(*func)(T1, T2, T3, T4, T5, T6, T7), T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, IScheduler* pScheduler, const char* strName = nullptr, void(*ecb)(void* func, const char* strName) = NULL)
	{
		assert(func);
		assert(pScheduler);
		std::shared_ptr<CqlJobStateR<R>>& l_spState = ResetState(pScheduler);
		TState l_spStateBase = std::dynamic_pointer_cast<IJobState>(l_spState);
		IJob* l_pJob = new CAsynCall7<T1, T2, T3, T4, T5, T6, T7>(func, p1, p2, p3, p4, p5, p6, p7, l_spState->m_Result, strName, &l_spStateBase, ecb);
		assert(l_pJob);
		BOOL l_bRet = pScheduler->Schedule(l_pJob);
		assert(l_bRet);
		return l_bRet;
	};

	template<class TObj, class T1, class T2, class T3, class T4, class T5, class T6, class T7>
	BOOL AsynCall(TObj* instance, R(TObj::*func)(T1, T2, T3, T4, T5, T6, T7), T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, IScheduler* pScheduler, const char* strName = nullptr, void(TObj::*ecb)(void* func, const char* strName) = NULL)
	{
		assert(func);
		assert(instance);
		assert(pScheduler);
		std::shared_ptr<CqlJobStateR<R>>& l_spState = ResetState(pScheduler);
		TState l_spStateBase = std::dynamic_pointer_cast<IJobState>(l_spState);
		IJob* l_pJob = new CAsynCallMember7<TObj, T1, T2, T3, T4, T5, T6, T7>(instance, func, p1, p2, p3, p4, p5, p6, p7, l_spState->m_Result, strName, &l_spStateBase, ecb);
		assert(l_pJob);
		BOOL l_bRet = pScheduler->Schedule(l_pJob);
		assert(l_bRet);
		return l_bRet;
	};

	template<class T1, class T2, class T3, class T4, class T5, class T6, class T7>
	BOOL AsynCallEx(R(*func)(T1, T2, T3, T4, T5, T6, T7, IJobState*), T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, IScheduler* pScheduler, const char* strName = nullptr, void(*ecb)(void* func, const char* strName) = NULL)
	{
		assert(func);
		assert(pScheduler);
		std::shared_ptr<CqlJobStateR<R>>& l_spState = ResetState(pScheduler);
		TState l_spStateBase = std::dynamic_pointer_cast<IJobState>(l_spState);
		IJob* l_pJob = new CAsynCallEx7<T1, T2, T3, T4, T5, T6, T7>(func, p1, p2, p3, p4, p5, p6, p7, l_spState->m_Result, strName, &l_spStateBase, ecb);
		assert(l_pJob);
		BOOL l_bRet = pScheduler->Schedule(l_pJob);
		assert(l_bRet);
		return l_bRet;
	};

	template<class TObj, class T1, class T2, class T3, class T4, class T5, class T6, class T7>
	BOOL AsynCallEx(TObj* instance, R(TObj::*func)(T1, T2, T3, T4, T5, T6, T7, IJobState*), T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, IScheduler* pScheduler, const char* strName = nullptr, void(TObj::*ecb)(void* func, const char* strName) = NULL)
	{
		assert(func);
		assert(instance);
		assert(pScheduler);
		std::shared_ptr<CqlJobStateR<R>>& l_spState = ResetState(pScheduler);
		TState l_spStateBase = std::dynamic_pointer_cast<IJobState>(l_spState);
		IJob* l_pJob = new CAsynCallMemberEx7<TObj, T1, T2, T3, T4, T5, T6, T7>(instance, func, p1, p2, p3, p4, p5, p6, p7, l_spState->m_Result, strName, &l_spStateBase, ecb);
		assert(l_pJob);
		BOOL l_bRet = pScheduler->Schedule(l_pJob);
		assert(l_bRet);
		return l_bRet;
	};

	//////////////////////////////////////////////////////////////////////////
	template<class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8>
	BOOL AsynCall(R(*func)(T1, T2, T3, T4, T5, T6, T7, T8), T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, IScheduler* pScheduler, const char* strName = nullptr, void(*ecb)(void* func, const char* strName) = NULL)
	{
		assert(func);
		assert(pScheduler);
		std::shared_ptr<CqlJobStateR<R>>& l_spState = ResetState(pScheduler);
		TState l_spStateBase = std::dynamic_pointer_cast<IJobState>(l_spState);
		IJob* l_pJob = new CAsynCall8<T1, T2, T3, T4, T5, T6, T7, T8>(func, p1, p2, p3, p4, p5, p6, p7, p8, l_spState->m_Result, strName, &l_spStateBase, ecb);
		assert(l_pJob);
		BOOL l_bRet = pScheduler->Schedule(l_pJob);
		assert(l_bRet);
		return l_bRet;
	};

	template<class TObj, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8>
	BOOL AsynCall(TObj* instance, R(TObj::*func)(T1, T2, T3, T4, T5, T6, T7, T8), T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, IScheduler* pScheduler, const char* strName = nullptr, void(TObj::*ecb)(void* func, const char* strName) = NULL)
	{
		assert(func);
		assert(instance);
		assert(pScheduler);
		std::shared_ptr<CqlJobStateR<R>>& l_spState = ResetState(pScheduler);
		TState l_spStateBase = std::dynamic_pointer_cast<IJobState>(l_spState);
		IJob* l_pJob = new CAsynCallMember8<TObj, T1, T2, T3, T4, T5, T6, T7, T8>(instance, func, p1, p2, p3, p4, p5, p6, p7, p8, l_spState->m_Result, strName, &l_spStateBase, ecb);
		assert(l_pJob);
		BOOL l_bRet = pScheduler->Schedule(l_pJob);
		assert(l_bRet);
		return l_bRet;
	};

	template<class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8>
	BOOL AsynCallEx(R(*func)(T1, T2, T3, T4, T5, T6, T7, T8, IJobState*), T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, IScheduler* pScheduler, const char* strName = nullptr, void(*ecb)(void* func, const char* strName) = NULL)
	{
		assert(func);
		assert(pScheduler);
		std::shared_ptr<CqlJobStateR<R>>& l_spState = ResetState(pScheduler);
		TState l_spStateBase = std::dynamic_pointer_cast<IJobState>(l_spState);
		IJob* l_pJob = new CAsynCallEx8<T1, T2, T3, T4, T5, T6, T7, T8>(func, p1, p2, p3, p4, p5, p6, p7, p8, l_spState->m_Result, strName, &l_spStateBase, ecb);
		assert(l_pJob);
		BOOL l_bRet = pScheduler->Schedule(l_pJob);
		assert(l_bRet);
		return l_bRet;
	};

	template<class TObj, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8>
	BOOL AsynCallEx(TObj* instance, R(TObj::*func)(T1, T2, T3, T4, T5, T6, T7, T8, IJobState*), T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, IScheduler* pScheduler, const char* strName = nullptr, void(TObj::*ecb)(void* func, const char* strName) = NULL)
	{
		assert(func);
		assert(instance);
		assert(pScheduler);
		std::shared_ptr<CqlJobStateR<R>>& l_spState = ResetState(pScheduler);
		TState l_spStateBase = std::dynamic_pointer_cast<IJobState>(l_spState);
		IJob* l_pJob = new CAsynCallMemberEx8<TObj, T1, T2, T3, T4, T5, T6, T7, T8>(instance, func, p1, p2, p3, p4, p5, p6, p7, p8, l_spState->m_Result, strName, &l_spStateBase, ecb);
		assert(l_pJob);
		BOOL l_bRet = pScheduler->Schedule(l_pJob);
		assert(l_bRet);
		return l_bRet;
	};

	//////////////////////////////////////////////////////////////////////////
	template<class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9>
	BOOL AsynCall(R(*func)(T1, T2, T3, T4, T5, T6, T7, T8, T9), T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, IScheduler* pScheduler, const char* strName = nullptr, void(*ecb)(void* func, const char* strName) = NULL)
	{
		assert(func);
		assert(pScheduler);
		std::shared_ptr<CqlJobStateR<R>>& l_spState = ResetState(pScheduler);
		TState l_spStateBase = std::dynamic_pointer_cast<IJobState>(l_spState);
		IJob* l_pJob = new CAsynCall9<T1, T2, T3, T4, T5, T6, T7, T8, T9>(func, p1, p2, p3, p4, p5, p6, p7, p8, p9, l_spState->m_Result, strName, &l_spStateBase, ecb);
		assert(l_pJob);
		BOOL l_bRet = pScheduler->Schedule(l_pJob);
		assert(l_bRet);
		return l_bRet;
	};

	template<class TObj, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9>
	BOOL AsynCall(TObj* instance, R(TObj::*func)(T1, T2, T3, T4, T5, T6, T7, T8, T9), T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, IScheduler* pScheduler, const char* strName = nullptr, void(TObj::*ecb)(void* func, const char* strName) = NULL)
	{
		assert(func);
		assert(instance);
		assert(pScheduler);
		std::shared_ptr<CqlJobStateR<R>>& l_spState = ResetState(pScheduler);
		TState l_spStateBase = std::dynamic_pointer_cast<IJobState>(l_spState);
		IJob* l_pJob = new CAsynCallMember9<TObj, T1, T2, T3, T4, T5, T6, T7, T8, T9>(instance, func, p1, p2, p3, p4, p5, p6, p7, p8, p9, l_spState->m_Result, strName, &l_spStateBase, ecb);
		assert(l_pJob);
		BOOL l_bRet = pScheduler->Schedule(l_pJob);
		assert(l_bRet);
		return l_bRet;
	};

	template<class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9>
	BOOL AsynCallEx(R(*func)(T1, T2, T3, T4, T5, T6, T7, T8, T9, IJobState*), T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, IScheduler* pScheduler, const char* strName = nullptr, void(*ecb)(void* func, const char* strName) = NULL)
	{
		assert(func);
		assert(pScheduler);
		std::shared_ptr<CqlJobStateR<R>>& l_spState = ResetState(pScheduler);
		TState l_spStateBase = std::dynamic_pointer_cast<IJobState>(l_spState);
		IJob* l_pJob = new CAsynCallEx9<T1, T2, T3, T4, T5, T6, T7, T8, T9>(func, p1, p2, p3, p4, p5, p6, p7, p8, p9, l_spState->m_Result, strName, &l_spStateBase, ecb);
		assert(l_pJob);
		BOOL l_bRet = pScheduler->Schedule(l_pJob);
		assert(l_bRet);
		return l_bRet;
	};

	template<class TObj, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9>
	BOOL AsynCallEx(TObj* instance, R(TObj::*func)(T1, T2, T3, T4, T5, T6, T7, T8, T9, IJobState*), T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, IScheduler* pScheduler, const char* strName = nullptr, void(TObj::*ecb)(void* func, const char* strName) = NULL)
	{
		assert(func);
		assert(instance);
		assert(pScheduler);
		std::shared_ptr<CqlJobStateR<R>>& l_spState = ResetState(pScheduler);
		TState l_spStateBase = std::dynamic_pointer_cast<IJobState>(l_spState);
		IJob* l_pJob = new CAsynCallMemberEx9<TObj, T1, T2, T3, T4, T5, T6, T7, T8, T9>(instance, func, p1, p2, p3, p4, p5, p6, p7, p8, p9, l_spState->m_Result, strName, &l_spStateBase, ecb);
		assert(l_pJob);
		BOOL l_bRet = pScheduler->Schedule(l_pJob);
		assert(l_bRet);
		return l_bRet;
	};


public:
	class CAsynState
	{
	public:
		CAsynState() {};
		explicit CAsynState(const std::shared_ptr<CqlJobStateR<R>>& spState) { m_spState = spState; };
		CAsynState& operator=(const CAsynState& that) { m_spState = that.m_spState; return *this; };

	public:
		BOOL IsValid()				const { return BOOL(m_spState.get() != NULL); }

	public:
		const R& WaitFor()
		{
			if (m_spState.get())
			{
				m_spState->WaitUntilDoneOrTimeout(INFINITE);
				return m_spState->m_Result;
			}
			//
			static R l_r;
			return l_r;
		}
		///
		BOOL WaitForEndOrTimeout(DWORD milliSeconds)
		{
			BOOL l_bRet = FALSE;
			if (m_spState.get())
			{
				l_bRet = m_spState->WaitUntilDoneOrTimeout(milliSeconds);
			}
			return l_bRet;
		}
		/// 
		const R& GetResult() const
		{
			if (m_spState.get())
			{
				assert(m_spState->GetStatus() == IJobState::JOB_DONE);
				if (m_spState->GetStatus() == IJobState::JOB_DONE)
				{
					return m_spState->m_Result;
				}
			}
			//
			static R l_r;
			return l_r;
		}
		///
		BOOL IsDone() const
		{
			BOOL l_bRet = TRUE;
			if (m_spState.get())
			{
				l_bRet = (m_spState->GetStatus() == IJobState::JOB_DONE);
			}
			return l_bRet;
		};
		///
		void Terminate()
		{
			if (m_spState.get())
			{
				m_spState->Terminate();
			}
		}

	protected:
		std::shared_ptr<CqlJobStateR<R>> m_spState;
	};

public:
	const R& WaitFor()
	{
		std::shared_ptr<CqlJobStateR<R>> l_spState;
		{
			CCriticalSectionLock l_lock(m_csLock);
			l_spState = m_spState;
		}
		//
		if (l_spState.get())
		{
			l_spState->WaitUntilDoneOrTimeout(INFINITE);
			return l_spState->m_Result;
		}
		//
		static R l_r;
		return l_r;
	}
	///
	BOOL WaitForEndOrTimeout(DWORD milliSeconds)
	{
		std::shared_ptr<CqlJobStateR<R>> l_spState;
		{
			CCriticalSectionLock l_lock(m_csLock);
			l_spState = m_spState;
		}
		//
		BOOL l_bRet = FALSE;
		if (l_spState.get())
		{
			l_bRet = l_spState->WaitUntilDoneOrTimeout(milliSeconds);
		}
		return l_bRet;
	}
	/// 
	const R& GetResult() const
	{
		std::shared_ptr<CqlJobStateR<R>> l_spState;
		{
			CCriticalSectionLock l_lock(m_csLock);
			l_spState = m_spState;
		}
		//
		if (l_spState.get())
		{
			assert(l_spState->GetStatus() == IJobState::JOB_DONE);
			if (l_spState->GetStatus() == IJobState::JOB_DONE)
			{
				return l_spState->m_Result;
			}
		}
		//
		static R l_r;
		return l_r;;
	}
	///
	BOOL IsDone() const
	{
		std::shared_ptr<CqlJobStateR<R>> l_spState;
		{
			CCriticalSectionLock l_lock(m_csLock);
			l_spState = m_spState;
		}
		//
		BOOL l_bRet = TRUE;
		if (l_spState.get())
		{
			l_bRet = (l_spState->GetStatus() == IJobState::JOB_DONE);
		}
		return l_bRet;
	};
	///
	void Terminate()
	{
		std::shared_ptr<CqlJobStateR<R>> l_spState;
		{
			CCriticalSectionLock l_lock(m_csLock);
			l_spState = m_spState;
		}
		//
		if (l_spState.get())
		{
			l_spState->Terminate();
		}
	}
	///
	CAsynState GetCurState()
	{
		CCriticalSectionLock l_lock(m_csLock);
		return CAsynState(m_spState);
	}

public:
	virtual ~CqlAsynCallEx()
	{
		CCriticalSectionLock l_lock(m_csLock);
		if (m_spState.get())
		{
			m_spState->Terminate();
			m_spState = nullptr;
		}
	};

protected:
	std::shared_ptr<CqlJobStateR<R>>& ResetState(IScheduler* pScheduler)
	{
		std::shared_ptr<CqlJobStateR<R>> l_spState(new CqlJobStateR<R>(pScheduler));
		//
		CCriticalSectionLock l_lock(m_csLock);
		if (m_spState.get())
		{
			m_spState->Terminate();
			m_spState = nullptr;
		}
		//
		m_spState.swap(l_spState);
		return m_spState;
	}

protected:
	std::shared_ptr<CqlJobStateR<R>> m_spState;
	//
	mutable CCriticalSection		m_csLock;
};

}

#endif /*!__ASYNCALLEX_H__*/