#ifndef MYEXCEPTION_H
#define MYEXCEPTION_H

#include <boost/exception/all.hpp>

/*
   typedef error_info<struct errinfo_api_function_, char const *> errinfo_api_function;
   typedef error_info<struct _, int> errinfo_errno;
   typedef error_info<struct errinfo_errno_, int> errinfo_errno;
   typedef error_info<struct errinfo_file_handle_, weak_ptr<FILE>> errinfo_file_handle;
   typedef error_info<struct errinfo_file_name_, std::string> errinfo_file_name;
   typedef error_info<struct errinfo_file_open_mode, std::string> errinfo_file_open_mode;
   typedef error_info<struct errinfo_type_info_name_, std::string> errinfo_type_info_name;
 */
class myException : virtual std::exception, virtual boost::exception
{

};

#endif
