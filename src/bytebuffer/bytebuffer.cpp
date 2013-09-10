/**
 */
#include "bytebuffer.h"
#include <cassert>
#include <cstring>
#include <cstdlib>


ByteBuffer::ByteBuffer(int capacity) : m_capacity(capacity), m_size(0), m_readpos(0), m_pbuf(NULL), m_ptop(NULL)
{
	assert(capacity > 0);

	void *p = malloc(capacity);

	memset(p, 0, capacity);
	m_pbuf = (char *)p;
	m_ptop = m_pbuf;
}

ByteBuffer::ByteBuffer(const ByteBuffer& rhs) : m_capacity(0), m_size(0), m_readpos(0), m_pbuf(NULL), m_ptop(NULL)
{
	*this = rhs;
}

ByteBuffer& ByteBuffer::operator=(const ByteBuffer& rhs)
{
	/*
	if(this == &rhs)
	{
		return *this;
	}

	m_size = rhs.m_size;
	Realloc(rhs.m_capacity);
	m_ptop = rhs.m_ptop;
	memcpy(m_pbuf, rhs.m_pbuf, rhs.m_capacity);
	return *this;
	*/
	*this = rhs;
	return *this;
}

char *ByteBuffer::Inout() const
{
	return m_pbuf;
}

int ByteBuffer::Capacity() const
{
	return m_capacity;
}

int ByteBuffer::Capacity(int capacity)
{
	this->Realloc(capacity);
	return capacity;
}

int ByteBuffer::Size(int sz)
{
	m_size = sz;

	if(sz > 0)
	{
		Capacity(sz);
	}

	return m_size;
}

int ByteBuffer::Size() const
{
	return m_size;
}

ByteBuffer::~ByteBuffer()
{
	if(m_pbuf != NULL)
	{
		free(m_pbuf);
	}
}

void ByteBuffer::Realloc(int size)
{
	assert(size > 0);

	if(size <= m_capacity) return;

	void *p = NULL;

	if(m_pbuf == NULL)
	{
		p = malloc(size);
	}
	else
	{
		p = realloc(m_pbuf, size);
	}

	m_pbuf = (char *)p;

	// 初始化新分配的内存
	memset(m_pbuf + m_capacity, 0, size - m_capacity);

	m_capacity = size;
}

ByteBuffer* ByteBuffer::CopyValueOf(int offset, int count)
{
	assert(offset >= 0 && offset < m_capacity && count <= (m_capacity - offset));

	ByteBuffer_var vBuf = new ByteBuffer(count + 1);
	memcpy(vBuf->Inout(), m_pbuf + offset, count);
	vBuf->Size(count);
	return vBuf.retn();
}

int ByteBuffer::getInt() {
	char temp[4];
	for(int i = 0; i < 4; i++) {
		temp[i] = m_ptop[3-i];
	}
	int *pt = (int*)temp;
	m_ptop += 4;
	m_readpos += 4;
	return *pt;
}

void ByteBuffer::putInt(int value) {
	char *pt = (char*)(&value);
	char temp;
	for(int i = 0; i < 2; i++) {
		temp = pt[i];
		pt[i] = pt[3-i];
		pt[3-i] = temp;
	}
	memcpy(m_ptop, pt, 4*sizeof(char));
	m_ptop += 4;
	m_size += 4;
}

char* ByteBuffer::getBytes(int length) {
	char* temp = m_ptop;
	m_ptop += length;
	m_readpos += length;
	return temp;
}

void ByteBuffer::putBytes(char* addr, int length) {
	memcpy(m_ptop, addr, length*sizeof(char));
	m_size += length;
	m_ptop += length;
}

void ByteBuffer::Reset()
{
	memset(m_pbuf, 0, m_capacity);
	m_size = 0;
	m_readpos = 0;
	m_ptop = m_pbuf;
}

bool ByteBuffer::isFullPackage( int packetsize )
{
	return (m_size-m_readpos) >= packetsize;
}

void ByteBuffer::adjustReadpos( int offset )
{
	m_readpos += offset;
	m_ptop += offset;

}

Byte::Byte() : m_buf(0), m_size(0)
{

}

Byte::Byte( int size )
{
	m_buf = new char[size];
	m_size = size;
	memset(m_buf, 0, sizeof(char)*size);
}

Byte::~Byte()
{
	if(m_buf != NULL) {
		delete[] m_buf;
	}

}

int Byte::size()
{
	return m_size;
}

char* Byte::getBuffer() {
	return m_buf;
}

int Byte::setBuffer(const char* buf, int size )
{
	memcpy(m_buf, buf, sizeof(char)*size);
	return size;
}