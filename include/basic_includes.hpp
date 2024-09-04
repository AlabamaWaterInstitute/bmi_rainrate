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
#include <fstream>
#include <sstream>
#include <cstring>

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

    // backtrace utility
    #include <execinfo.h>
    #include <cxxabi.h>
    #include <cstdlib>

    static std::string string_trace() {
      void *array[10];
      size_t size;
      char **strings;
      size_t i;

      size = backtrace(array, 10);
      strings = backtrace_symbols(array, size);

      std::string trace = "";

      for (i = 0; i < size; i++) {
        std::string line = strings[i];
        size_t start = line.find("(");
        size_t end = line.find("+", start);
        if (start != std::string::npos && end != std::string::npos) {
          std::string mangled = line.substr(start + 1, end - start - 1);
          int status;
          char* demangled = abi::__cxa_demangle(mangled.c_str(), NULL, NULL, &status);
          if (status == 0) {
            trace += std::string(demangled) + std::string("\n");
            free(demangled);
          } else {
            trace += mangled + std::string("\n");
          }
        }
      }

      free(strings);

      return trace;
    }
  #else
    #define _log(msg)
    #define _en()
    #define _ex()
    #define _ln()
    #define _lnf()
    #define _mk_setup_info() std::string("")
    #define _mk_setup_nofunc_info() std::string("")

    static std::string string_trace() {
      return std::string("");
    }
  #endif
#endif


#endif