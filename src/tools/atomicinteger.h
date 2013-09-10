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
			 * ���캯��
			 */
			AtomicInteger(int initialValue = 0);

			/**
			 * ��������
			 */
			~AtomicInteger();

			/**
			 * ��ȡ��ǰֵ
			 */
			int Get() const;

			/**
			 * ����Ϊ����ֵ
			 */
			void Set(int newValue);

			/**
			 * ��ԭ�ӷ�ʽ����ǰֵ�� 1
			 * @return ��ǰ��ֵ
			 */
			int GetAndIncrement();

			/**
			 * ��ԭ�ӷ�ʽ����ǰֵ�� 1
			 * @return ��ǰ��ֵ
			 */
			int GetAndDecrement();

			/**
			 * ��ԭ�ӷ�ʽ����ǰֵ�� 1
			 * @return ���µ�ֵ
			 */
			int IncrementAndGet();

			/**
			 * ��ԭ�ӷ�ʽ����ǰֵ�� 1
			 * @return ���µ�ֵ
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
			 * ����ת��������
			 */
			operator int () const;

			//////////////////////////////////////////////////////////////////////////

			/**
			 * ��ԭ�ӷ�ʽ����ǰֵ�� 1
			 * @return ��ǰ��ֵ
			 */
			static int GetAndIncrement(volatile boost::uint32_t *ptr);

			/**
			 * ��ԭ�ӷ�ʽ����ǰֵ�� 1
			 * @return ��ǰ��ֵ
			 */
			static int GetAndDecrement(volatile boost::uint32_t *ptr);
			/**
			 * ��ԭ�ӷ�ʽ����ǰֵ�� 1
			 * @return ���µ�ֵ
			 */
			static int IncrementAndGet(volatile boost::uint32_t *ptr);

			/**
			 * ��ԭ�ӷ�ʽ����ǰֵ�� 1
			 * @return ���µ�ֵ
			 */
			static int DecrementAndGet(volatile boost::uint32_t *ptr);

		private:
			volatile boost::uint32_t m_nValue;
	};
};

#endif //
