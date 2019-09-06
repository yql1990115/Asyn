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
	/// 调度器名称
	virtual const char*			Name() const override;

	/**
	启动调度程序   主线程调用

	@return BOOL
	如果失败将返回FALSE, 错误信息可以通过GetLastError()函数获得
	*/
	virtual BOOL				Startup() override;

	/**
	关闭调度程序, 释放所有调度作业	主线程调用

	@param dwMilliSeconds
	等待运行中作业超时后, 将强制关闭所有作业
	*/
	virtual void				Shutdown(DWORD dwMilliSecondsTimeOut = 30000) override;

	/**
	如果已请求终止调度程序, 则返回TRUE
	注意: 虽然有终止请求, 但是调度程序可能仍在执行中, 直到等待超时
	*/
	virtual BOOL 				HasBeenTerminated() const override;

	/**
	请求暂停调度程序执行
	*/
	virtual void 				Stop() override;

	/**
	请求恢复调度程序执行
	*/
	virtual void 				Resume() override;

	/**
	如果已请求暂停调度程序执行, 则返回TRUE
	注意: 虽然有暂停请求, 但是作业有可能仍在执行中
	*/
	virtual BOOL 				HasBeenSuspended() const override;

	/**
	判断任务调度程序是否还活着
	*/
	virtual BOOL				HasAlive() const override;

	/// 先设置退出信号，在设置有作业事件
	virtual void				NotifyExit() override;

public:
	/**
	将作业放入调度程序中

	@param pJob
	作业

	@return BOOL
	如果作业放入调度池中失败, 将调用作业的 \ref Release() 函数释放作业并返回FALSE
	*/
	virtual BOOL				Schedule(IJob* pJob) override;

public:
	/// 处理函数 默认返回0 出现错误1 崩溃2
	virtual unsigned int		OnProcess();

	/// Legal values for expression in except().
	virtual long				OnException(struct _EXCEPTION_POINTERS *);

protected:
	typedef std::list<IJob*>	LIST_JOB;

	/// 移除作业链表
	virtual void				RemoveListJob();

	/// 移除作业链表
	virtual void				RemoveListJob(LIST_JOB& listJob);

	/// 处理作业
	virtual void				OnProcessJob(IJob* pJob);

	/**
	事件信号处理
	对 \ref m_pause 和 \ref m_terminated 事件信号进行处理
	1. 检测到m_pause和m_terminated信号都未设置, 函数立即返回FALSE
	2. 检测到m_pause信号设置, m_terminated信号未设置, 函数会调用一次OnPause并等待
	1). m_pause信号被重置, m_terminated信号未设置, 函数调用一次OnResume, 函数返回FALSE
	2). m_terminated信号设置, 函数调用一次OnTerminated(), 函数返回TRUE
	3. m_terminated信号设置, 函数调用一次OnTerminated(), 函数返回TRUE

	@return BOOL
	如果m_terminated信号被设置, 函数返回TRUE; 其他情况返回FALSE
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
	/// 保存请求进行调度的作业
	LIST_JOB					m_ListJob;

	/// 作业锁
	CCriticalSection			m_ListJobLock;

	/// 如果事件被设置, 调度程序应尝试暂停运行, 直到事件复位
	CWaitableEvent				m_pause;

	/// 如果事件被设置, 调度程序将直接删除剩余未运行作业, 并且 \ref Schedule() 函数无法放入作业 
	CWaitableEvent 				m_terminated;

	/// 如果事件被设置, 表明有新的作业放进来了
	CWaitableEvent				m_HasNewJob;

protected:
	/// 调度程序线程引擎
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
	启动调度程序   主线程调用

	@return BOOL
	如果失败将返回FALSE, 错误信息可以通过GetLastError()函数获得
	*/
	virtual BOOL				Startup() override;

	/**
	关闭调度程序, 释放所有调度作业	主线程调用

	@param dwMilliSeconds
	等待运行中作业超时后, 将强制关闭所有作业
	*/
	virtual void				Shutdown(DWORD dwMilliSecondsTimeOut = 30000) override;

	/**
	请求暂停调度程序执行
	*/
	virtual void 				Stop() override;

	/**
	请求恢复调度程序执行
	*/
	virtual void 				Resume() override;

	/// 先设置退出信号，在设置有作业事件
	virtual void				NotifyExit() override;

public:
	///
	virtual unsigned int		GetSlaveSize() const;
	///
	IScheduler*					GetSlaveScheduler(unsigned int nIndex) const;

protected:
	/// 处理函数 默认返回0 出现错误1 崩溃2
	virtual unsigned int		OnProcess() override;

	/// 移除作业链表
	virtual void				RemoveListJob(LIST_JOB& listJob);

	/// 处理作业
	virtual void				OnProcessJob(IJob* pJob);

	/// 确认是否还有未使用线程
	virtual BOOL				HasNewThread();

protected:
	class CqlSchedulerSlave : public CqlScheduler
	{
	public:
		CqlSchedulerSlave(const char* strName = "CqlSchedulerSlave_NameLess");

	public:
		/**
		启动调度程序   主线程调用

		@return BOOL
		如果失败将返回FALSE, 错误信息可以通过GetLastError()函数获得
		*/
		virtual BOOL			Startup(unsigned long volatile*	pnSlaveInfo, unsigned char nSlaveIndex, CWaitableEvent* pNotifyEvent, const char* strName);

	public:
		/// 处理函数 默认返回0 出现错误1 崩溃2
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
	const static long			s_SlavePageSize = 31;			//long型是4个字节，高位不用
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
	/// 如果事件被设置, 表明有新的线程可以使用
	CWaitableEvent				m_HasNewThread;
};

}

#endif /*!__QLSCHEDULER_H__*/
