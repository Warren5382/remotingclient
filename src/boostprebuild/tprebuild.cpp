#define BOOST_DATE_TIME_SOURCE
#define BOOST_THREAD_NO_LIB
#include <boost/thread.hpp>

extern "C" void tss_cleanup_implemented(void) {}
#include <libs/thread/src/win32/thread.cpp>
#include <libs/thread/src/win32/tss_dll.cpp>
#include <libs/thread/src/win32/tss_pe.cpp>