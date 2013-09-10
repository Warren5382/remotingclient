#ifndef COUNTDOWN_H
#define COUNTDOWN_H

#include "version.h"

/*
 * CondStruct������������������ν�����.
 */
typedef struct{
	public:
		boost::condition_variable_any cond;
		boost::mutex cond_mutex;
		bool pflag;
}CondStruct;

/*
 * Phone��Ϊ�߳�ͬ���� �źŷ����ߵ���set�������źŽ����߾��ܴ�wait�н�����
 */
class CountDown : public LWPR::Object {
	public:
		CountDown() {
			mHandles.pflag = true;
		}

		~CountDown() {
		}

		/*
		 * �����¼����ź�֪ͨ��صȴ��߳�
		 */
		void Set() {
			{
				boost::mutex::scoped_lock lock(mHandles.cond_mutex);
				mHandles.pflag = false;
			}
			mHandles.cond.notify_all();
		}

		/*
		 * ���ø�λ����Ҫ�Ƕ�ν�ʸ�λ
		 */
		void Reset() {
			{
				boost::mutex::scoped_lock lock(mHandles.cond_mutex);
				mHandles.pflag = true;
			}
		}

		/*
		 * �����ȴ�
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
		 * ������ʱ�ȴ�
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
