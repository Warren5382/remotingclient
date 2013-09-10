#ifndef COUNTDOWN_H
#define COUNTDOWN_H

#include "version.h"

/*
 * CondStruct由条件量，互斥锁，谓词组成.
 */
typedef struct{
	public:
		boost::condition_variable_any cond;
		boost::mutex cond_mutex;
		bool pflag;
}CondStruct;

/*
 * Phone类为线程同步， 信号发起者调用set方法，信号接受者就能从wait中解阻塞
 */
class CountDown : public LWPR::Object {
	public:
		CountDown() {
			mHandles.pflag = true;
		}

		~CountDown() {
		}

		/*
		 * 置起事件。信号通知相关等待线程
		 */
		void Set() {
			{
				boost::mutex::scoped_lock lock(mHandles.cond_mutex);
				mHandles.pflag = false;
			}
			mHandles.cond.notify_all();
		}

		/*
		 * 重置复位，主要是对谓词复位
		 */
		void Reset() {
			{
				boost::mutex::scoped_lock lock(mHandles.cond_mutex);
				mHandles.pflag = true;
			}
		}

		/*
		 * 阻塞等待
		 */
		bool Wait() {
			{
				boost::mutex::scoped_lock lock(mHandles.cond_mutex);
				//while(mHandles.pflag) {
				mHandles.cond.wait(mHandles.cond_mutex);
				//}
			}
			return true;
		}

		/*
		 * 阻塞超时等待
		 */
		bool Wait(int timethreshold) {
			bool IsOK;
			{
				boost::mutex::scoped_lock lock(mHandles.cond_mutex);
				while(mHandles.pflag) {
					IsOK = mHandles.cond.timed_wait(mHandles.cond_mutex, boost::get_system_time() + boost::posix_time::seconds(timethreshold));
				}
			}
			return IsOK;
		}
	private:
		CondStruct mHandles;
};

DECLAREVAR(CountDown);

#endif
