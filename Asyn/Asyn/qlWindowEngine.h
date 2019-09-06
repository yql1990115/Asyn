#ifndef __QLWINDOWENGINE_H__
#define __QLWINDOWENGINE_H__

#include <assert.h>
#include <windef.h>
#include <WinUser.h>
#include <string>
#include <vector>

namespace asyn {

template<class TObj>
class CqlWindowEngine
{
public:
	CqlWindowEngine(TObj* instance, void (TObj::*EventProc)(), void (TObj::*TimerProc)(), long(TObj::*OnException)(struct _EXCEPTION_POINTERS *) = nullptr);
	virtual ~CqlWindowEngine();

public:
	/**
	�������񴰿�

	@return BOOL
	���ʧ�ܽ�����FALSE, ������Ϣ����ͨ��GetLastError()�������
	*/
	virtual BOOL				Startup();

	/**
	���ٴ���, �ͷ����е�����ҵ

	@param dwMilliSeconds
	�ȴ���������ҵ��ʱ��, ��ǿ�ƹر�������ҵ
	*/
	virtual void				Shutdown(DWORD dwMilliSecondsTimeOut = 30000);

	/**
	�ж����񴰿��Ƿ񻹻���
	*/
	virtual BOOL				HasAlive() const;

	/**
	��ȡ���ھ��
	*/
	virtual HWND				GetWindowHandle() const;

	/**
	���ʹ���Event����Ϣ
	*/
	virtual void				PostEventMsg();

protected:
	CqlWindowEngine(const CqlWindowEngine&); //= delete;
	CqlWindowEngine& operator=(const CqlWindowEngine&); //= delete;

protected:
	/**
	���ڴ���ȫ���
	*/
	static LRESULT __stdcall	ISafeWindowProc(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam);

	/// �¼�����
	virtual void				EventProc();

	/// ��ʱ������
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
	/// ���ھ��
	HWND						m_hWnd;

protected:
	/// ʹ�� \ref m_strEventMsg ��ϵͳע�����Ϣ
	UINT						m_nEventMsg;

	/// ����ע�� \ref m_nEventMsg ��Ϣ���ַ���
	std::string					m_strEventMsg;

	/// ��ʱ��ID
	UINT						m_nTimerID;

	/// ��������
	std::string					m_strWindowClassName;
};

template<class TObj>CqlWindowEngine<TObj>::CqlWindowEngine(TObj* instance, void (TObj::*EventProc)(), void (TObj::*TimerProc)(), long(TObj::*OnException)(struct _EXCEPTION_POINTERS *)) : m_Instance(instance), m_EventProc(EventProc), m_TimerProc(TimerProc), m_OnException(OnException)
{
	assert(m_Instance);
	m_hWnd = NULL;
	m_nEventMsg = 0;
	//
	DWORD l_dwValue = GetTickCount();
	m_strEventMsg = Format("CqlWindowEngine::Event::Disptacher::Message::%d", l_dwValue);
	m_strWindowClassName = Format("CqlWindowEngine::Event::Disptacher::ClassName::%d", l_dwValue);
	//
	m_nTimerID = 666666;
	m_nEventMsg = ::RegisterWindowMessageA(m_strEventMsg.c_str());
	assert(0 != m_nEventMsg);
	//
	::Sleep(10);
}

template<class TObj>
CqlWindowEngine<TObj>::~CqlWindowEngine()
{
	Shutdown();
	//
	assert(nullptr == m_hWnd);
}

template<class TObj>
BOOL CqlWindowEngine<TObj>::Startup()
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
void CqlWindowEngine<TObj>::Shutdown(DWORD dwMilliSecondsTimeOut)
{
	if (nullptr != m_hWnd)
	{
		::KillTimer(m_hWnd, m_nTimerID);
		::DestroyWindow(m_hWnd);
		m_hWnd = nullptr;
	}
}

template<class TObj>
BOOL CqlWindowEngine<TObj>::HasAlive() const
{
	BOOL l_bRet = FALSE;
	if (nullptr != m_hWnd)
	{
		l_bRet = ::IsWindow(m_hWnd);
	}
	return l_bRet;
}

template<class TObj>
HWND CqlWindowEngine<TObj>::GetWindowHandle() const
{
	return m_hWnd;
}

/**
���ʹ���Event����Ϣ
*/
template<class TObj>
void CqlWindowEngine<TObj>::PostEventMsg()
{
	::PostMessageA(m_hWnd, m_nEventMsg, 0, 0);
}

template<class TObj>
LRESULT __stdcall CqlWindowEngine<TObj>::ISafeWindowProc(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	DWORD l_dwEx = 0;
	CqlWindowEngine<TObj>* pThis = (CqlWindowEngine<TObj>*)::GetWindowLongPtr(hWnd, GWLP_USERDATA);;
	__try
	{
		if (pThis && (msg == pThis->m_nEventMsg))
		{
			{//�Ӷ������Ƴ����е�ע����Ϣ����ֹ��λ���
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
void CqlWindowEngine<TObj>::EventProc()
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
void CqlWindowEngine<TObj>::TimerProc()
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
long CqlWindowEngine<TObj>::OnException(struct _EXCEPTION_POINTERS * pInfo)
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
std::string CqlWindowEngine<TObj>::Format(const char *strFmt, ...)
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

template<class TObj>
HWND CqlWindowEngine<TObj>::CreateWnd()
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

#endif /*!__QLWINDOWENGINE_H__*/