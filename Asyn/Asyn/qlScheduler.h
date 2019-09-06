#ifndef __QLSCHEDULER_H__
#define __QLSCHEDULER_H__

#include "qlAsynHelper.h"
#include "qlThreadEngine.h"
#include <list>
#include <string>

namespace asyn {

class CqlScheduler : public IScheduler
{
public:
	CqlScheduler(const char* strName = "CqlScheduler");
	virtual ~CqlScheduler();

public:
	/// ����������
	virtual const char*			Name() const override;

	/**
	�������ȳ���   ���̵߳���

	@return BOOL
	���ʧ�ܽ�����FALSE, ������Ϣ����ͨ��GetLastError()�������
	*/
	virtual BOOL				Startup() override;

	/**
	�رյ��ȳ���, �ͷ����е�����ҵ	���̵߳���

	@param dwMilliSeconds
	�ȴ���������ҵ��ʱ��, ��ǿ�ƹر�������ҵ
	*/
	virtual void				Shutdown(DWORD dwMilliSecondsTimeOut = 30000) override;

	/**
	�����������ֹ���ȳ���, �򷵻�TRUE
	ע��: ��Ȼ����ֹ����, ���ǵ��ȳ����������ִ����, ֱ���ȴ���ʱ
	*/
	virtual BOOL 				HasBeenTerminated() const override;

	/**
	������ͣ���ȳ���ִ��
	*/
	virtual void 				Stop() override;

	/**
	����ָ����ȳ���ִ��
	*/
	virtual void 				Resume() override;

	/**
	�����������ͣ���ȳ���ִ��, �򷵻�TRUE
	ע��: ��Ȼ����ͣ����, ������ҵ�п�������ִ����
	*/
	virtual BOOL 				HasBeenSuspended() const override;

	/**
	�ж�������ȳ����Ƿ񻹻���
	*/
	virtual BOOL				HasAlive() const override;

	/// �������˳��źţ�����������ҵ�¼�
	virtual void				NotifyExit() override;

public:
	/**
	����ҵ������ȳ�����

	@param pJob
	��ҵ

	@return BOOL
	�����ҵ������ȳ���ʧ��, ��������ҵ�� \ref Release() �����ͷ���ҵ������FALSE
	*/
	virtual BOOL				Schedule(IJob* pJob) override;

public:
	/// ������ Ĭ�Ϸ���0 ���ִ���1 ����2
	virtual unsigned int		OnProcess();

	/// Legal values for expression in except().
	virtual long				OnException(struct _EXCEPTION_POINTERS *);

protected:
	typedef std::list<IJob*>	LIST_JOB;

	/// �Ƴ���ҵ����
	virtual void				RemoveListJob();

	/// �Ƴ���ҵ����
	virtual void				RemoveListJob(LIST_JOB& listJob);

	/// ������ҵ
	virtual void				OnProcessJob(IJob* pJob);

	/**
	�¼��źŴ���
	�� \ref m_pause �� \ref m_terminated �¼��źŽ��д���
	1. ��⵽m_pause��m_terminated�źŶ�δ����, ������������FALSE
	2. ��⵽m_pause�ź�����, m_terminated�ź�δ����, ���������һ��OnPause���ȴ�
	1). m_pause�źű�����, m_terminated�ź�δ����, ��������һ��OnResume, ��������FALSE
	2). m_terminated�ź�����, ��������һ��OnTerminated(), ��������TRUE
	3. m_terminated�ź�����, ��������һ��OnTerminated(), ��������TRUE

	@return BOOL
	���m_terminated�źű�����, ��������TRUE; �����������FALSE
	*/
	virtual BOOL				SignalHandle();

	/// 
	virtual void				OnTerminated() {};

	///
	virtual void				OnPause() {};

	///
	virtual void				OnResume() {};

	///
	virtual std::string			Format(const char *strFmt, ...);

protected:
	/// ����������е��ȵ���ҵ
	LIST_JOB					m_ListJob;

	/// ��ҵ��
	CCriticalSection			m_ListJobLock;

	/// ����¼�������, ���ȳ���Ӧ������ͣ����, ֱ���¼���λ
	CWaitableEvent				m_pause;

	/// ����¼�������, ���ȳ���ֱ��ɾ��ʣ��δ������ҵ, ���� \ref Schedule() �����޷�������ҵ 
	CWaitableEvent 				m_terminated;

	/// ����¼�������, �������µ���ҵ�Ž�����
	CWaitableEvent				m_HasNewJob;

protected:
	/// ���ȳ����߳�����
	CqlThreadEngine<CqlScheduler>	m_thread;

	///
	std::string					m_strName;
};


class CqlSchedulerPool : public CqlScheduler
{
public:
	CqlSchedulerPool(unsigned long nPoolSize = 8, const char* strName = "CqlSchedulerPool");
	virtual ~CqlSchedulerPool();

public:
	/**
	�������ȳ���   ���̵߳���

	@return BOOL
	���ʧ�ܽ�����FALSE, ������Ϣ����ͨ��GetLastError()�������
	*/
	virtual BOOL				Startup() override;

	/**
	�رյ��ȳ���, �ͷ����е�����ҵ	���̵߳���

	@param dwMilliSeconds
	�ȴ���������ҵ��ʱ��, ��ǿ�ƹر�������ҵ
	*/
	virtual void				Shutdown(DWORD dwMilliSecondsTimeOut = 30000) override;

	/**
	������ͣ���ȳ���ִ��
	*/
	virtual void 				Stop() override;

	/**
	����ָ����ȳ���ִ��
	*/
	virtual void 				Resume() override;

	/// �������˳��źţ�����������ҵ�¼�
	virtual void				NotifyExit() override;

public:
	///
	virtual unsigned int		GetSlaveSize() const;
	///
	IScheduler*					GetSlaveScheduler(unsigned int nIndex) const;

protected:
	/// ������ Ĭ�Ϸ���0 ���ִ���1 ����2
	virtual unsigned int		OnProcess() override;

	/// �Ƴ���ҵ����
	virtual void				RemoveListJob(LIST_JOB& listJob);

	/// ������ҵ
	virtual void				OnProcessJob(IJob* pJob);

	/// ȷ���Ƿ���δʹ���߳�
	virtual BOOL				HasNewThread();

protected:
	class CqlSchedulerSlave : public CqlScheduler
	{
	public:
		CqlSchedulerSlave(const char* strName = "CqlSchedulerSlave_NameLess");

	public:
		/**
		�������ȳ���   ���̵߳���

		@return BOOL
		���ʧ�ܽ�����FALSE, ������Ϣ����ͨ��GetLastError()�������
		*/
		virtual BOOL			Startup(unsigned long volatile*	pnSlaveInfo, unsigned char nSlaveIndex, CWaitableEvent* pNotifyEvent, const char* strName);

	public:
		/// ������ Ĭ�Ϸ���0 ���ִ���1 ����2
		virtual unsigned int	OnProcess() override;

	protected:
		///
		unsigned long volatile*	m_pnSlaveInfo;
		///
		unsigned char			m_nSlaveIndex;
		//
		CWaitableEvent*			m_pNotifyEvent;
	};

protected:
	const static long			s_SlavePageSize = 31;			//long����4���ֽڣ���λ����
	//
	typedef struct SlaveInfo_s
	{
		unsigned long volatile	SlaveInfo;
		CqlSchedulerSlave		acScheduler[s_SlavePageSize];
		SlaveInfo_s() { SlaveInfo = 0; }
	}SlaveInfo_t;
	//
	//SlaveInfo_t				m_atSlave[(POOLSIZE / s_SlavePageSize) + 1];
	SlaveInfo_t*				m_ptSlave;
	//
	const unsigned long 		m_nPoolSize;
	//
	/// ����¼�������, �������µ��߳̿���ʹ��
	CWaitableEvent				m_HasNewThread;
};

}

#endif /*!__QLSCHEDULER_H__*/
