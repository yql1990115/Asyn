/***************************************************************************
*   Copyright (C) 2019 by yql1990115                                      *
*   yql1990115@gmail.com                                                  *
*                                                                         *
*   www.yql1990115.com                                                    *
***************************************************************************/

#pragma once

#include "IJob.h"

namespace async{
	
class IJobScheduler
{
public:
	/// ��������
	virtual 					~IJobScheduler(){};

public:
	/// ����������
	virtual const char*			Name() const = 0;

	/**
	�������ȳ���
	
	@return BOOL
		���ʧ�ܽ�����FALSE, ������Ϣ����ͨ��GetLastError()�������
	*/
	virtual BOOL				Startup() = 0;
	
	/**
	�رյ��ȳ���, �ͷ����е�����ҵ
	
	@param dwMilliSeconds
		�ȴ���������ҵ��ʱ��, ��ǿ�ƹر�������ҵ
	*/
	virtual void				Shutdown(DWORD dwMilliSecondsTimeOut) = 0;
	
	/**
	�����������ֹ���ȳ���, �򷵻�TRUE
	ע��: ��Ȼ����ֹ����, ���ǵ��ȳ����������ִ����, ֱ���ȴ���ʱ
	*/
	virtual BOOL 				HasBeenTerminated() const = 0;
	
	/**
	������ͣ���ȳ���ִ��
	*/	
	virtual void 				Stop() = 0;
	
	/**
	����ָ����ȳ���ִ��
	*/	
	virtual void 				Resume() = 0;
	
	/**
	�����������ͣ���ȳ���ִ��, �򷵻�TRUE
	ע��: ��Ȼ����ͣ����, ������ҵ�п�������ִ����
	*/
	virtual BOOL 				HasBeenSuspended() const = 0;

	/**
	�ж�������ȳ����Ƿ񻹻���
	*/
	virtual BOOL				HasAlive() const = 0;

	/// ֪ͨ�߳��˳� Shutdown֮ǰ���� �ӿ��Ƴ��ٶ�
	virtual void				NotifyExit() = 0;

public:
	/**
	����ҵ������ȳ�����
	
	@param pJob
		��ҵ
		
	@return BOOL	
		�����ҵ������ȳ���ʧ��, ��������ҵ�� \ref Release() �����ͷ���ҵ������FALSE
	*/
	virtual BOOL				Schedule(IJob* pJob) = 0;
};

}