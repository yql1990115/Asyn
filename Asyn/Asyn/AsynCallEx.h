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
#include <memory>

namespace async{

class AsynCallEx
{
protected:
	class CAsynCallState : public CJobStateBase
	{
	public:
		CAsynCallState(IJobScheduler* pScheduler) : m_pScheduler(*pScheduler){};

	public:
		///
		virtual BOOL 	HasBeenTerminated() const override
		{
			BOOL l_bRet1 = CJobStateBase::HasBeenTerminated();
			BOOL l_bRet2 = m_pScheduler.HasBeenTerminated();
			BOOL l_bRet = l_bRet1 || l_bRet2;
			return l_bRet;
		}

		///
		virtual BOOL 	HasBeenSuspended() const override
		{
			BOOL l_bRet1 = CJobStateBase::HasBeenSuspended();
			BOOL l_bRet2 = m_pScheduler.HasBeenSuspended();
			BOOL l_bRet = l_bRet1 || l_bRet2;
			return l_bRet;
		}

	public:
		IJobScheduler& m_pScheduler;
	};

protected:
	class CAsynCall0 : public CJobBase<CJobStateBase>
	{
	public:
		CAsynCall0(void(*func)(IJobState*), const char* strName, std::shared_ptr<CJobStateBase>* spState) : CJobBase(spState, strName)
		{
			m_func = func;
		}

		virtual void InternalExecute()
		{
			(*m_func)(m_spState.get());
		}

	protected:
		void(*m_func)(IJobState*);
	};

	template<class TObj>
	class CAsynCallMember0 : public CJobBase<CJobStateBase>
	{
	public:
		CAsynCallMember0(TObj* instance, void(TObj::*func)(IJobState*), const char* strName, std::shared_ptr<CJobStateBase>* spState) : CJobBase(spState, strName)
		{
			m_instance = instance;
			m_func = func;
		}

		virtual void InternalExecute()
		{
			(m_instance->*m_func)(m_spState.get());
		}

	protected:
		TObj*			m_instance;
		void(TObj::*m_func)(IJobState*);
	};

	template<class T1>
	class CAsynCall1 : public CJobBase<CJobStateBase>
	{
	public:
		CAsynCall1(void(*func)(T1, IJobState*), T1 parameter1, const char* strName, std::shared_ptr<CJobStateBase>* spState) : CJobBase(spState, strName)
		{
			m_func = func;
			m_parameter1 = parameter1;
		}

		virtual void InternalExecute()
		{
			(*m_func)(m_parameter1, m_spState.get());
		}

	protected:
		void(*m_func)(T1, IJobState*);
		T1				m_parameter1;
	};

	template<class TObj, class T1>
	class CAsynCallMember1 : public CJobBase<CJobStateBase>
	{
	public:
		CAsynCallMember1(TObj* instance, void(TObj::*func)(T1, IJobState*), T1 parameter1, const char* strName, std::shared_ptr<CJobStateBase>* spState) : CJobBase(spState, strName)
		{
			m_instance = instance;
			m_func = func;
			m_parameter1 = parameter1;
		}

		virtual void InternalExecute()
		{
			(m_instance->*m_func)(m_parameter1, m_spState.get());
		}

	protected:
		TObj*			m_instance;
		void(TObj::*m_func)(T1, IJobState*);
		T1				m_parameter1;
	};

	template<class T1, class T2>
	class CAsynCall2 : public CJobBase<CJobStateBase>
	{
	public:
		CAsynCall2(void(*func)(T1, T2, IJobState*), T1 parameter1, T2 parameter2, const char* strName, std::shared_ptr<CJobStateBase>* spState) : CJobBase(spState, strName)
		{
			m_func = func;
			m_parameter1 = parameter1;
			m_parameter2 = parameter2;
		}

		virtual void InternalExecute()
		{
			(*m_func)(m_parameter1, m_parameter2, m_spState.get());
		}

	protected:
		void(*m_func)(T1, T2, IJobState*);
		T1				m_parameter1;
		T2				m_parameter2;
	};

	template<class TObj, class T1, class T2>
	class CAsynCallMember2 : public CJobBase<CJobStateBase>
	{
	public:
		CAsynCallMember2(TObj* instance, void(TObj::*func)(T1, T2, IJobState*), T1 parameter1, T2 parameter2, const char* strName, std::shared_ptr<CJobStateBase>* spState) : CJobBase(spState, strName)
		{
			m_instance = instance;
			m_func = func;
			m_parameter1 = parameter1;
			m_parameter2 = parameter2;
		}

		virtual void InternalExecute()
		{
			(m_instance->*m_func)(m_parameter1, m_parameter2, m_spState.get());
		}

	protected:
		TObj*			m_instance;
		void(TObj::*m_func)(T1, T2, IJobState*);
		T1				m_parameter1;
		T2				m_parameter2;
	};

	template<class T1, class T2, class T3>
	class CAsynCall3 : public CJobBase<CJobStateBase>
	{
	public:
		CAsynCall3(void(*func)(T1, T2, T3, IJobState*), T1 parameter1, T2 parameter2, T3 parameter3, const char* strName, std::shared_ptr<CJobStateBase>* spState) : CJobBase(spState, strName)
		{
			m_func = func;
			m_parameter1 = parameter1;
			m_parameter2 = parameter2;
			m_parameter3 = parameter3;
		}

		virtual void InternalExecute()
		{
			(*m_func)(m_parameter1, m_parameter2, m_parameter3, m_spState.get());
		}

	protected:
		void(*m_func)(T1, T2, T3, IJobState*);
		T1				m_parameter1;
		T2				m_parameter2;
		T3				m_parameter3;
	};

	template<class TObj, class T1, class T2, class T3>
	class CAsynCallMember3 : public CJobBase<CJobStateBase>
	{
	public:
		CAsynCallMember3(TObj* instance, void(TObj::*func)(T1, T2, T3, IJobState*), T1 parameter1, T2 parameter2, T3 parameter3, const char* strName, std::shared_ptr<CJobStateBase>* spState) : CJobBase(spState, strName)
		{
			m_instance = instance;
			m_func = func;
			m_parameter1 = parameter1;
			m_parameter2 = parameter2;
			m_parameter3 = parameter3;
		}

		virtual void InternalExecute()
		{
			(m_instance->*m_func)(m_parameter1, m_parameter2, m_parameter3, m_spState.get());
		}

	protected:
		TObj*			m_instance;
		void(TObj::*m_func)(T1, T2, T3, IJobState*);
		T1				m_parameter1;
		T2				m_parameter2;
		T3				m_parameter3;
	};

	template<class T1, class T2, class T3, class T4>
	class CAsynCall4 : public CJobBase<CJobStateBase>
	{
	public:
		CAsynCall4(void(*func)(T1, T2, T3, T4, IJobState*), T1 parameter1, T2 parameter2, T3 parameter3, T4 parameter4, const char* strName, std::shared_ptr<CJobStateBase>* spState) : CJobBase(spState, strName)
		{
			m_func = func;
			m_parameter1 = parameter1;
			m_parameter2 = parameter2;
			m_parameter3 = parameter3;
			m_parameter4 = parameter4;
		}

		virtual void InternalExecute()
		{
			(*m_func)(m_parameter1, m_parameter2, m_parameter3, m_parameter4, m_spState.get());
		}

	protected:
		void(*m_func)(T1, T2, T3, T4, IJobState*);
		T1				m_parameter1;
		T2				m_parameter2;
		T3				m_parameter3;
		T4				m_parameter4;
	};

	template<class TObj, class T1, class T2, class T3, class T4>
	class CAsynCallMember4 : public CJobBase<CJobStateBase>
	{
	public:
		CAsynCallMember4(TObj* instance, void(TObj::*func)(T1, T2, T3, T4, IJobState*), T1 parameter1, T2 parameter2, T3 parameter3, T4 parameter4, const char* strName, std::shared_ptr<CJobStateBase>* spState) : CJobBase(spState, strName)
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
			(m_instance->*m_func)(m_parameter1, m_parameter2, m_parameter3, m_parameter4, m_spState.get());
		}

	protected:
		TObj*			m_instance;
		void(TObj::*m_func)(T1, T2, T3, T4, IJobState*);
		T1				m_parameter1;
		T2				m_parameter2;
		T3				m_parameter3;
		T4				m_parameter4;
	};

	template<class T1, class T2, class T3, class T4, class T5>
	class CAsynCall5 : public CJobBase<CJobStateBase>
	{
	public:
		CAsynCall5(void(*func)(T1, T2, T3, T4, T5, IJobState*), T1 parameter1, T2 parameter2, T3 parameter3, T4 parameter4, T5 parameter5, const char* strName, std::shared_ptr<CJobStateBase>* spState) : CJobBase(spState, strName)
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
			(*m_func)(m_parameter1, m_parameter2, m_parameter3, m_parameter4, m_parameter5, m_spState.get());
		}

	protected:
		void(*m_func)(T1, T2, T3, T4, T5, IJobState*);
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
		CAsynCallMember5(TObj* instance, void(TObj::*func)(T1, T2, T3, T4, T5, IJobState*), T1 parameter1, T2 parameter2, T3 parameter3, T4 parameter4, T5 parameter5, const char* strName, std::shared_ptr<CJobStateBase>* spState) : CJobBase(spState, strName)
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
			(m_instance->*m_func)(m_parameter1, m_parameter2, m_parameter3, m_parameter4, m_parameter5, m_spState.get());
		}

	protected:
		TObj*			m_instance;
		void(TObj::*m_func)(T1, T2, T3, T4, T5, IJobState*);
		T1				m_parameter1;
		T2				m_parameter2;
		T3				m_parameter3;
		T4				m_parameter4;
		T5				m_parameter5;
	};

public:
	BOOL AsynCall(void(*func)(IJobState*), IJobScheduler* pScheduler, const char* strName = nullptr)
	{
		assert(func);
		assert(pScheduler);
		std::shared_ptr<CAsynCallState>& l_spState = ResetState(pScheduler);
		std::shared_ptr<CJobStateBase> l_spStateBase = std::dynamic_pointer_cast<CJobStateBase>(l_spState);
		IJob* l_pJob = new CAsynCall0(func, strName, &l_spStateBase);
		assert(l_pJob);
		BOOL l_bRet = pScheduler->Schedule(l_pJob);
		assert(l_bRet);
		return l_bRet;
	};

	template<class TObj>
	BOOL AsynCall(TObj* instance, void(TObj::*func)(IJobState*), IJobScheduler* pScheduler, const char* strName = nullptr)
	{
		assert(func);
		assert(instance);
		assert(pScheduler);
		std::shared_ptr<CAsynCallState>& l_spState = ResetState(pScheduler);
		std::shared_ptr<CJobStateBase> l_spStateBase = std::dynamic_pointer_cast<CJobStateBase>(l_spState);
		IJob* l_pJob = new CAsynCallMember0<TObj>(instance, func, strName, &l_spStateBase);
		assert(l_pJob);
		BOOL l_bRet = pScheduler->Schedule(l_pJob);
		assert(l_bRet);
		return l_bRet;
	};

	template<class T1>
	BOOL AsynCall(void(*func)(T1, IJobState*), T1 parameter1, IJobScheduler* pScheduler, const char* strName = nullptr)
	{
		assert(func);
		assert(pScheduler);
		std::shared_ptr<CAsynCallState>& l_spState = ResetState(pScheduler);
		std::shared_ptr<CJobStateBase> l_spStateBase = std::dynamic_pointer_cast<CJobStateBase>(l_spState);
		IJob* l_pJob = new CAsynCall1<T1>(func, parameter1, strName, &l_spStateBase);
		assert(l_pJob);
		BOOL l_bRet = pScheduler->Schedule(l_pJob);
		assert(l_bRet);
		return l_bRet;
	};

	template<class TObj, class T1>
	BOOL AsynCall(TObj* instance, void(TObj::*func)(T1, IJobState*), T1 parameter1, IJobScheduler* pScheduler, const char* strName = nullptr)
	{
		assert(func);
		assert(instance);
		assert(pScheduler);
		std::shared_ptr<CAsynCallState>& l_spState = ResetState(pScheduler);
		std::shared_ptr<CJobStateBase> l_spStateBase = std::dynamic_pointer_cast<CJobStateBase>(l_spState);
		IJob* l_pJob = new CAsynCallMember1<TObj, T1>(instance, func, parameter1, strName, &l_spStateBase);
		assert(l_pJob);
		BOOL l_bRet = pScheduler->Schedule(l_pJob);
		assert(l_bRet);
		return l_bRet;
	};

	template<class T1, class T2>
	BOOL AsynCall(void(*func)(T1, T2, IJobState*), T1 parameter1, T2 parameter2, IJobScheduler* pScheduler, const char* strName = nullptr)
	{
		assert(func);
		assert(pScheduler);
		std::shared_ptr<CAsynCallState>& l_spState = ResetState(pScheduler);
		std::shared_ptr<CJobStateBase> l_spStateBase = std::dynamic_pointer_cast<CJobStateBase>(l_spState);
		IJob* l_pJob = new CAsynCall2<T1, T2>(func, parameter1, parameter2, strName, &l_spStateBase);
		assert(l_pJob);
		BOOL l_bRet = pScheduler->Schedule(l_pJob);
		assert(l_bRet);
		return l_bRet;
	};

	template<class TObj, class T1, class T2>
	BOOL AsynCall(TObj* instance, void(TObj::*func)(T1, T2, IJobState*), T1 parameter1, T2 parameter2, IJobScheduler* pScheduler, const char* strName = nullptr)
	{
		assert(func);
		assert(instance);
		assert(pScheduler);
		std::shared_ptr<CAsynCallState>& l_spState = ResetState(pScheduler);
		std::shared_ptr<CJobStateBase> l_spStateBase = std::dynamic_pointer_cast<CJobStateBase>(l_spState);
		IJob* l_pJob = new CAsynCallMember2<TObj, T1, T2>(instance, func, parameter1, parameter2, strName, &l_spStateBase);
		assert(l_pJob);
		BOOL l_bRet = pScheduler->Schedule(l_pJob);
		assert(l_bRet);
		return l_bRet;
	};

	template<class T1, class T2, class T3>
	BOOL AsynCall(void(*func)(T1, T2, T3, IJobState*), T1 parameter1, T2 parameter2, T3 parameter3, IJobScheduler* pScheduler, const char* strName = nullptr)
	{
		assert(func);
		assert(pScheduler);
		std::shared_ptr<CAsynCallState>& l_spState = ResetState(pScheduler);
		std::shared_ptr<CJobStateBase> l_spStateBase = std::dynamic_pointer_cast<CJobStateBase>(l_spState);
		IJob* l_pJob = new CAsynCall3<T1, T2, T3>(func, parameter1, parameter2, parameter3, strName, &l_spStateBase);
		assert(l_pJob);
		BOOL l_bRet = pScheduler->Schedule(l_pJob);
		assert(l_bRet);
		return l_bRet;
	};

	template<class TObj, class T1, class T2, class T3>
	BOOL AsynCall(TObj* instance, void(TObj::*func)(T1, T2, T3, IJobState*), T1 parameter1, T2 parameter2, T3 parameter3, IJobScheduler* pScheduler, const char* strName = nullptr)
	{
		assert(func);
		assert(instance);
		assert(pScheduler);
		std::shared_ptr<CAsynCallState>& l_spState = ResetState(pScheduler);
		std::shared_ptr<CJobStateBase> l_spStateBase = std::dynamic_pointer_cast<CJobStateBase>(l_spState);
		IJob* l_pJob = new CAsynCallMember3<TObj, T1, T2, T3>(instance, func, parameter1, parameter2, parameter3, strName, &l_spStateBase);
		assert(l_pJob);
		BOOL l_bRet = pScheduler->Schedule(l_pJob);
		assert(l_bRet);
		return l_bRet;
	};

	template<class T1, class T2, class T3, class T4>
	BOOL AsynCall(void(*func)(T1, T2, T3, T4, IJobState*), T1 parameter1, T2 parameter2, T3 parameter3, T4 parameter4, IJobScheduler* pScheduler, const char* strName = nullptr)
	{
		assert(func);
		assert(pScheduler);
		std::shared_ptr<CAsynCallState>& l_spState = ResetState(pScheduler);
		std::shared_ptr<CJobStateBase> l_spStateBase = std::dynamic_pointer_cast<CJobStateBase>(l_spState);
		IJob* l_pJob = new CAsynCall4<T1, T2, T3, T4>(func, parameter1, parameter2, parameter3, parameter4, strName, &l_spStateBase);
		assert(l_pJob);
		BOOL l_bRet = pScheduler->Schedule(l_pJob);
		assert(l_bRet);
		return l_bRet;
	};

	template<class TObj, class T1, class T2, class T3, class T4>
	BOOL AsynCall(TObj* instance, void(TObj::*func)(T1, T2, T3, T4, IJobState*), T1 parameter1, T2 parameter2, T3 parameter3, T4 parameter4, IJobScheduler* pScheduler, const char* strName = nullptr)
	{
		assert(func);
		assert(instance);
		assert(pScheduler);
		std::shared_ptr<CAsynCallState>& l_spState = ResetState(pScheduler);
		std::shared_ptr<CJobStateBase> l_spStateBase = std::dynamic_pointer_cast<CJobStateBase>(l_spState);
		IJob* l_pJob = new CAsynCallMember4<TObj, T1, T2, T3, T4>(instance, func, parameter1, parameter2, parameter3, parameter4, strName, &l_spStateBase);
		assert(l_pJob);
		BOOL l_bRet = pScheduler->Schedule(l_pJob);
		assert(l_bRet);
		return l_bRet;
	};

	template<class T1, class T2, class T3, class T4, class T5>
	BOOL AsynCall(void(*func)(T1, T2, T3, T4, T5, IJobState*), T1 parameter1, T2 parameter2, T3 parameter3, T4 parameter4, T5 parameter5, IJobScheduler* pScheduler, const char* strName = nullptr)
	{
		assert(func);
		assert(pScheduler);
		std::shared_ptr<CAsynCallState>& l_spState = ResetState(pScheduler);
		std::shared_ptr<CJobStateBase> l_spStateBase = std::dynamic_pointer_cast<CJobStateBase>(l_spState);
		IJob* l_pJob = new CAsynCall5<T1, T2, T3, T4, T5>(func, parameter1, parameter2, parameter3, parameter4, parameter5, strName, &l_spStateBase);
		assert(l_pJob);
		BOOL l_bRet = pScheduler->Schedule(l_pJob);
		assert(l_bRet);
		return l_bRet;
	};

	template<class TObj, class T1, class T2, class T3, class T4, class T5>
	BOOL AsynCall(TObj* instance, void(TObj::*func)(T1, T2, T3, T4, T5, IJobState*), T1 parameter1, T2 parameter2, T3 parameter3, T4 parameter4, T5 parameter5, IJobScheduler* pScheduler, const char* strName = nullptr)
	{
		assert(func);
		assert(instance);
		assert(pScheduler);
		std::shared_ptr<CAsynCallState>& l_spState = ResetState(pScheduler);
		std::shared_ptr<CJobStateBase> l_spStateBase = std::dynamic_pointer_cast<CJobStateBase>(l_spState);
		IJob* l_pJob = new CAsynCallMember5<TObj, T1, T2, T3, T4, T5>(instance, func, parameter1, parameter2, parameter3, parameter4, parameter5, strName, &l_spStateBase);
		assert(l_pJob);
		BOOL l_bRet = pScheduler->Schedule(l_pJob);
		assert(l_bRet);
		return l_bRet;
	};

public:
	///
	void WaitFor()
	{
		assert(m_spState.get());
		if (m_spState.get())
		{
			m_spState->WaitUntilDoneOrTimeout(INFINITE);
		}
	}
	///
	BOOL WaitForEndOrTimeout(DWORD milliSeconds)
	{
		assert(m_spState.get());
		BOOL l_bRet = FALSE;
		if (m_spState.get())
		{
			l_bRet = m_spState->WaitUntilDoneOrTimeout(milliSeconds);
		}
		return l_bRet;
	}
	///
	BOOL IsDone() const
	{
		//assert(m_spState.get());
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

public:
	AsynCallEx(){};
	virtual ~AsynCallEx()
	{
		if (m_spState.get())
		{
			m_spState->Terminate();
			m_spState = nullptr;
		}
	};

protected:
	std::shared_ptr<CAsynCallState>& ResetState(IJobScheduler* pScheduler)
	{
		std::shared_ptr<CAsynCallState> l_spState(new CAsynCallState(pScheduler));
		//
		if (m_spState.get())
		{
			m_spState->Terminate();
			m_spState = nullptr;
		}
		//
		m_spState.swap(l_spState);
		//
		return m_spState;
	}

protected:
	std::shared_ptr<CAsynCallState> m_spState;
};

}
