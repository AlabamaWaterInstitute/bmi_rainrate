#pragma once

#ifndef BASIC_INCLUDES_HPP
#define BASIC_INCLUDES_HPP

#include <memory>
#include <string>
#include <vector>
#include <map>
#include "bmi.hxx"
#include <numeric>
#include <iostream>

#define DEBUG_
#ifndef DEBUG_UTILS
  #define DEBUG_UTILS
  #ifdef DEBUG_
    #define _log(msg) fprintf(stderr, "[%s] %s\n", __func__, msg)
    #define _en() fprintf(stderr, "[%s] %s\n", __func__, "ENTER");
    #define _ex() fprintf(stderr, "[%s] %s\n", __func__, "EXIT");
    #define _ln() fprintf(stderr, "%d\n", __LINE__);
    #define _lnf() fprintf(stderr, "%d|%s\n", __LINE__, __FILE__);
    #define _mk_setup_info() std::string(__func__) + std::string(" in ") + std::string(__FILE__) + std::string(" at line ") + std::to_string(__LINE__)
    #define _mk_setup_nofunc_info() std::string(" in ") + std::string(__FILE__) + std::string(" at line ") + std::to_string(__LINE__)
  #else
    #define _log(msg)
    #define _en()
    #define _ex()
    #define _ln()
    #define _lnf()
    #define _mk_setup_info() std::string("")
    #define _mk_setup_nofunc_info() std::string("")
  #endif
#endif


#endif