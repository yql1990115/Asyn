/***************************************************************************
*   Copyright (C) 2019 by yql1990115                                      *
*   yql1990115@gmail.com                                                  *
*                                                                         *
*   www.yql1990115.com                                                    *
***************************************************************************/

#pragma once

#include <assert.h>
#include <minwindef.h>
#include <windef.h>
#include <WinUser.h>
#include <string>
#include <sysinfoapi.h>
#include <vector>

namespace async{

template<class TObj>
class CWindowEvent
{
public:
	CWindowEvent(TObj* instance, void (TObj::*EventProc)(), void (TObj::*TimerProc)(), long(TObj::*OnException)(struct _EXCEPTION_POINTERS *) = nullptr);
	virtual ~CWindowEvent();

public:
	/**
	启动任务窗口

	@return BOOL
	如果失败将返回FALSE, 错误信息可以通过GetLastError()函数获得
	*/
	virtual BOOL				Startup();

	/**
	销毁窗口, 释放所有调度作业

	@param dwMilliSeconds
	等待运行中作业超时后, 将强制关闭所有作业
	*/
	virtual void				Shutdown(DWORD dwMilliSecondsTimeOut = 30000);

	/**
	判断任务窗口是否还活着
	*/
	virtual BOOL				HasAlive() const;

	/**
	获取窗口句柄
	*/
	virtual HWND				GetWindowHandle() const;

	/**
	发送处理Event的消息
	*/
	virtual void				PostEventMsg();

protected:
	/**
	窗口处理安全入口
	*/
	static LRESULT __stdcall	ISafeWindowProc(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam);

	/// 事件处理
	virtual void				EventProc();

	/// 定时器处理
	virtual void				TimerProc();

	///
	virtual long				OnException(struct _EXCEPTION_POINTERS *);

protected:
	///
	virtual std::string			Format(const char *strFmt, ...);

	///
	virtual HWND				CreateWnd();

protected:
	TObj*						m_Instance;

	///
	void						(TObj::*m_EventProc)();

	///
	void						(TObj::*m_TimerProc)();

	///
	long						(TObj::*m_OnException)(struct _EXCEPTION_POINTERS *);

protected:
	/// 窗口句柄
	HWND						m_hWnd;

protected:
	/// 使用 \ref m_strEventMsg 向系统注册的消息
	UINT						m_nEventMsg;

	/// 用来注册 \ref m_nEventMsg 消息的字符串
	std::string					m_strEventMsg;

	/// 定时器ID
	UINT						m_nTimerID;

	/// 窗口类名
	std::string					m_strWindowClassName;
};

template<class TObj>CWindowEvent<TObj>::CWindowEvent(TObj* instance, void (TObj::*EventProc)(), void (TObj::*TimerProc)(), long(TObj::*OnException)(struct _EXCEPTION_POINTERS *)) : m_Instance(instance), m_EventProc(EventProc), m_TimerProc(TimerProc), m_OnException(OnException)
{
	assert(m_Instance);
	m_hWnd = NULL;
	m_nEventMsg = 0;
	//
	DWORD l_dwValue = GetTickCount();
	m_strEventMsg = Format("CWindowEvent::Event::Disptacher::Message::%d", l_dwValue);
	m_strWindowClassName = Format("CWindowEvent::Event::Disptacher::ClassName::%d", l_dwValue);
	//
	m_nTimerID = 666666;
	m_nEventMsg = ::RegisterWindowMessageA(m_strEventMsg.c_str());
	assert(0 != m_nEventMsg);
	//
	::Sleep(10);
}

template<class TObj>
CWindowEvent<TObj>::~CWindowEvent()
{
	Shutdown();
	//
	assert(nullptr == m_hWnd);
}

template<class TObj>
BOOL CWindowEvent<TObj>::Startup()
{
	assert(m_Instance);
	//
	BOOL l_bRet = FALSE;
	//
	if (nullptr == m_hWnd)
	{
		m_hWnd = CreateWnd();
	}
	l_bRet = ::IsWindow(m_hWnd);
	if (l_bRet)
	{
		::SetWindowLongPtrA(m_hWnd, GWLP_USERDATA, (LONG_PTR)this);
		::SetTimer(m_hWnd, m_nTimerID, 1000, NULL);
	}
	//
	return l_bRet;
}

template<class TObj>
void CWindowEvent<TObj>::Shutdown(DWORD dwMilliSecondsTimeOut)
{
	if (nullptr != m_hWnd)
	{
		::KillTimer(m_hWnd, m_nTimerID);
		::DestroyWindow(m_hWnd);
		m_hWnd = nullptr;
	}
}

template<class TObj>
BOOL CWindowEvent<TObj>::HasAlive() const
{
	BOOL l_bRet = FALSE;
	if (nullptr != m_hWnd)
	{
		l_bRet = ::IsWindow(m_hWnd);
	}
	return l_bRet;
}

template<class TObj>
HWND CWindowEvent<TObj>::GetWindowHandle() const
{
	return m_hWnd;
}

/**
发送处理Event的消息
*/
template<class TObj>
void CWindowEvent<TObj>::PostEventMsg()
{
	::PostMessageA(m_hWnd, m_nEventMsg, 0, 0);
}

template<class TObj>
LRESULT __stdcall CWindowEvent<TObj>::ISafeWindowProc(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	DWORD l_dwEx = 0;
	CWindowEvent<TObj>* pThis = (CWindowEvent<TObj>*)::GetWindowLongPtr(hWnd, GWLP_USERDATA);;
	__try
	{
		if (pThis && (msg == pThis->m_nEventMsg))
		{
			{//从队列中移除所有的注册消息，防止多次唤醒
				MSG l_msg;
				while (::PeekMessage(&l_msg, pThis->GetWindowHandle(), pThis->m_nEventMsg, pThis->m_nEventMsg, PM_REMOVE))
				{
					//nothing
				}
			}
			pThis->EventProc();
		}
		if (pThis && (WM_TIMER == msg) && (wparam == pThis->m_nTimerID))
		{
			pThis->TimerProc();
		}
	}
	__except (l_dwEx = GetExceptionCode(), (pThis ? (pThis->OnException(GetExceptionInformation())) : EXCEPTION_EXECUTE_HANDLER))
	{
		assert(pThis);
	}
	//
	return ::DefWindowProcA(hWnd, msg, wparam, lparam);
}

template<class TObj>
void CWindowEvent<TObj>::EventProc()
{
	assert(m_Instance);
	assert(m_EventProc);
	//
	if (m_Instance)
	{
		(m_Instance->*m_EventProc)();
	}
}

template<class TObj>
void CWindowEvent<TObj>::TimerProc()
{
	assert(m_Instance);
	assert(m_TimerProc);
	//
	if (m_Instance)
	{
		(m_Instance->*m_TimerProc)();
	}
}

///
template<class TObj>
long CWindowEvent<TObj>::OnException(struct _EXCEPTION_POINTERS * pInfo)
{
	long l_nRet = EXCEPTION_EXECUTE_HANDLER;
	//assert(m_Instance);
	//
	if (m_Instance && m_OnException)
	{
		l_nRet = (m_Instance->*m_OnException)(pInfo);
	}
	return l_nRet;
}

template<class TObj>
std::string CWindowEvent<TObj>::Format(const char *strFmt, ...)
{
	std::string l_strRet = "";
	if (NULL != strFmt)
	{
		va_list l_marker = NULL;
		va_start(l_marker, strFmt);									//初始化变量参数 
		size_t l_nLength = _vscprintf(strFmt, l_marker) + 1;		//获取格式化字符串长度
		std::vector<char> l_vBuffer(l_nLength, '\0');				//创建用于存储格式化字符串的字符数组
		int l_nWritten = _vsnprintf_s(&l_vBuffer[0], l_vBuffer.size(), l_nLength, strFmt, l_marker);
		if (l_nWritten > 0)
		{
			l_strRet = &l_vBuffer[0];
		}
		va_end(l_marker);                                    //重置变量参数
	}
	return l_strRet;
}

template<class TObj>
HWND CWindowEvent<TObj>::CreateWnd()
{
	HWND l_hWnd = nullptr;
	WNDCLASSA l_wc;
	l_wc.style = CS_HREDRAW | CS_VREDRAW;
	l_wc.lpfnWndProc = ISafeWindowProc;
	l_wc.cbClsExtra = 0;
	l_wc.cbWndExtra = 0;
	l_wc.hInstance = ::GetModuleHandleA(NULL);
	l_wc.hIcon = 0;
	l_wc.hCursor = 0;
	l_wc.hbrBackground = 0;
	l_wc.lpszMenuName = 0;
	l_wc.lpszClassName = m_strWindowClassName.c_str();
	//
	if (::RegisterClassA(&l_wc))
	{
		l_hWnd = ::CreateWindowExA(0, m_strWindowClassName.c_str(), "", 0, 0, 0, 1, 1, HWND_MESSAGE, 0, 0, 0);
		DWORD l_dw = GetLastError();
	}
	//
	assert(l_hWnd);
	return l_hWnd;
}

}