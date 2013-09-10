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
		 * 构造函数
		 */
		ByteBuffer(int capacity = ByteBuffer_DEFAULT_CAPACITY);

		/**
		 * 拷贝构造函数
		 */
		ByteBuffer(const ByteBuffer& rhs);

		/**
		 * 赋值操作符重载
		 */
		ByteBuffer& operator= (const ByteBuffer& rhs);

		/**
		 * 析构函数
		 */
		virtual ~ByteBuffer();

		/**
		 * 将所有内容初始化为0
		 */
		void Reset();

		/**
		 * 返回内存块首地址，供调用者读写
		 */
		virtual char *Inout() const;

		/**
		 * 返回分配的内存大小
		 */
		virtual int Capacity() const;

		/**
		 * 重新设置分配的内存大小
		 */
		virtual int Capacity(int capacity);

		/**
		 * 设置内存的有效大小
		 */
		int Size(int sz);

		/**
		 * 返回内存的有效大小
		 */
		int Size() const;

		/**
		 * 复制一份缓冲区
		 */
		ByteBuffer* CopyValueOf(int offset, int count);

		/**
		 * 从ByteBuffer中取出一个int数据，大小端转换
		 */
		int getInt();

		/**
		 * 向ByteBuffer中放入一个int数据，大小端转换
		 */
		void putInt(int value);

		/**
		 * 从ByteBuffer中取出一段内存，为了节省内存，此处返回ByteBuffer的地址
		 */
		char* getBytes(int length);

		/**
		 * 向ByteBuffer中复制一份内存
		 */
		void putBytes(char* addr, int length);

		/**
		 * 表示buffer中是否还需要继续收集包
		 */
		bool isFullPackage(int packetsize);

		/**
		 * 调整readpos
		 */
		void adjustReadpos(int offset);


	protected:
		void Realloc(int size);

	protected:
		int     m_capacity;	///< 内存块的大小
		int     m_size;		///< 内存块的有效大小，默认为0
		int		m_readpos;
		char    *m_pbuf;	///< 内存块的首地址
		char    *m_ptop;     ///< 内存快的读写地址，随着读写移动
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
