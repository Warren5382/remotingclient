/**
 */
#ifndef LWPR_ByteBuffer_H__
#define LWPR_ByteBuffer_H__

#include "version.h"

const int ByteBuffer_DEFAULT_CAPACITY = 1024;

class ByteBuffer : public LWPR::Object
{
	public:

		/**
		 * ���캯��
		 */
		ByteBuffer(int capacity = ByteBuffer_DEFAULT_CAPACITY);

		/**
		 * �������캯��
		 */
		ByteBuffer(const ByteBuffer& rhs);

		/**
		 * ��ֵ����������
		 */
		ByteBuffer& operator= (const ByteBuffer& rhs);

		/**
		 * ��������
		 */
		virtual ~ByteBuffer();

		/**
		 * ���������ݳ�ʼ��Ϊ0
		 */
		void Reset();

		/**
		 * �����ڴ���׵�ַ���������߶�д
		 */
		virtual char *Inout() const;

		/**
		 * ���ط�����ڴ��С
		 */
		virtual int Capacity() const;

		/**
		 * �������÷�����ڴ��С
		 */
		virtual int Capacity(int capacity);

		/**
		 * �����ڴ����Ч��С
		 */
		int Size(int sz);

		/**
		 * �����ڴ����Ч��С
		 */
		int Size() const;

		/**
		 * ����һ�ݻ�����
		 */
		ByteBuffer* CopyValueOf(int offset, int count);

		/**
		 * ��ByteBuffer��ȡ��һ��int���ݣ���С��ת��
		 */
		int getInt();

		/**
		 * ��ByteBuffer�з���һ��int���ݣ���С��ת��
		 */
		void putInt(int value);

		/**
		 * ��ByteBuffer��ȡ��һ���ڴ棬Ϊ�˽�ʡ�ڴ棬�˴�����ByteBuffer�ĵ�ַ
		 */
		char* getBytes(int length);

		/**
		 * ��ByteBuffer�и���һ���ڴ�
		 */
		void putBytes(char* addr, int length);

		/**
		 * ��ʾbuffer���Ƿ���Ҫ�����ռ���
		 */
		bool isFullPackage(int packetsize);

		/**
		 * ����readpos
		 */
		void adjustReadpos(int offset);


	protected:
		void Realloc(int size);

	protected:
		int     m_capacity;	///< �ڴ��Ĵ�С
		int     m_size;		///< �ڴ�����Ч��С��Ĭ��Ϊ0
		int		m_readpos;
		char    *m_pbuf;	///< �ڴ����׵�ַ
		char    *m_ptop;     ///< �ڴ��Ķ�д��ַ�����Ŷ�д�ƶ�
};

DECLAREVAR(ByteBuffer);

class Byte : public LWPR::Object {
private:
	char* m_buf;
	int m_size;
public:
	Byte();
	Byte(int size);
	~Byte();
	int size();
	char* getBuffer();
	int setBuffer(const char* buf, int size);
};
DECLAREVAR(Byte);

#endif // end of LWPR_ByteBuffer_H__
