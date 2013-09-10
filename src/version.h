#ifndef VERSION_H
#define VERSION_H

//#define WIN32 

//boost
#define BOOST_DATE_TIME_SOURCE
#define BOOST_THREAD_NO_LIB

#include <boost/cstdint.hpp>
#include <boost/cstdlib.hpp>
#include <boost/lexical_cast.hpp>

#include <set>

#include <boost/unordered_map.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/interprocess/sync/interprocess_semaphore.hpp>


//libevent
#include <event.h>
#include <event2/thread.h>

//Object
#include "tools/Object.h"
#include "tools/Var.h"

//Exception
#include "myException/myException.h"

//���л�
#include <JasonSerialize/JsonObejectBase.h>

//ԭ�Ӳ���
#include "tools/AtomicInteger.h"

#include <queue>

#endif
