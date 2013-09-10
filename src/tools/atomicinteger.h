#ifndef LWPR_ATOMIC_INTEGER_H__
#define LWPR_ATOMIC_INTEGER_H__
//#include "LWPRType.h"
#include <boost/interprocess/detail/atomic.hpp>

namespace LWPR
{
	class AtomicIntegerImpl;

	class AtomicInteger
	{
		public:

			/**
			 * 构造函数
			 */
			AtomicInteger(int initialValue = 0);

			/**
			 * 析构函数
			 */
			~AtomicInteger();

			/**
			 * 获取当前值
			 */
			int Get() const;

			/**
			 * 设置为给定值
			 */
			void Set(int newValue);

			/**
			 * 以原子方式将当前值加 1
			 * @return 以前的值
			 */
			int GetAndIncrement();

			/**
			 * 以原子方式将当前值减 1
			 * @return 以前的值
			 */
			int GetAndDecrement();

			/**
			 * 以原子方式将当前值加 1
			 * @return 更新的值
			 */
			int IncrementAndGet();

			/**
			 * 以原子方式将当前值减 1
			 * @return 更新的值
			 */
			int DecrementAndGet();

			/**
			 * i = 2
			 */
			int operator=(int newValue);

			/**
			 * ++i
			 */
			int operator++();

			/**
			 * i++
			 */
			int operator++(int);

			/**
			 * --i
			 */
			int operator--();

			/**
			 * i--
			 */
			int operator--(int);

			/**
			 * 类型转化操作符
			 */
			operator int () const;

			//////////////////////////////////////////////////////////////////////////

			/**
			 * 以原子方式将当前值加 1
			 * @return 以前的值
			 */
			static int GetAndIncrement(volatile boost::uint32_t *ptr);

			/**
			 * 以原子方式将当前值减 1
			 * @return 以前的值
			 */
			static int GetAndDecrement(volatile boost::uint32_t *ptr);
			/**
			 * 以原子方式将当前值加 1
			 * @return 更新的值
			 */
			static int IncrementAndGet(volatile boost::uint32_t *ptr);

			/**
			 * 以原子方式将当前值减 1
			 * @return 更新的值
			 */
			static int DecrementAndGet(volatile boost::uint32_t *ptr);

		private:
			volatile boost::uint32_t m_nValue;
	};
};

#endif //
