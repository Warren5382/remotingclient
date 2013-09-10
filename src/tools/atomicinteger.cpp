#include "atomicinteger.h"

namespace LWPR
{

	AtomicInteger::AtomicInteger(int initialValue)
		: m_nValue(initialValue)
	{
	}

	AtomicInteger::~AtomicInteger()
	{
	}

	int AtomicInteger::Get() const
	{
		return m_nValue;
	}

	void AtomicInteger::Set(int newValue)
	{
		m_nValue = newValue;
	}

	int AtomicInteger::GetAndIncrement()
	{
		return GetAndIncrement(&m_nValue);
	}

	int AtomicInteger::GetAndDecrement()
	{
		return GetAndDecrement(&m_nValue);
	}

	int AtomicInteger::IncrementAndGet()
	{
		return IncrementAndGet(&m_nValue);
	}

	int AtomicInteger::DecrementAndGet()
	{
		return DecrementAndGet(&m_nValue);
	}

	int AtomicInteger::operator = (int newValue)
	{
		this->Set(newValue);
		return newValue;
	}

	int AtomicInteger::operator++()
	{
		return this->IncrementAndGet();
	}

	int AtomicInteger::operator++(int)
	{
		return this->GetAndIncrement();
	}

	int AtomicInteger::operator--()
	{
		return this->DecrementAndGet();
	}

	int AtomicInteger::operator--(int)
	{
		return this->GetAndDecrement();
	}

	AtomicInteger::operator int () const
	{
		return this->Get();
	}

	int AtomicInteger::GetAndIncrement(volatile boost::uint32_t *ptr)
	{
		return boost::interprocess::detail::atomic_inc32(ptr);
	}

	int AtomicInteger::GetAndDecrement(volatile boost::uint32_t *ptr)
	{
		return boost::interprocess::detail::atomic_dec32(ptr);
	}

	int AtomicInteger::IncrementAndGet(volatile boost::uint32_t *ptr)
	{
		return boost::interprocess::detail::atomic_inc32(ptr)+1;
	}

	int AtomicInteger::DecrementAndGet(volatile boost::uint32_t *ptr)
	{
		return boost::interprocess::detail::atomic_dec32(ptr)-1;
	}
};
