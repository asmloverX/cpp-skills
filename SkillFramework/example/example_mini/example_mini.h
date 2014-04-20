// Copyright (c) 2011, ASMlover. All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 
//  * Redistributions of source code must retain the above copyright
//    notice, this list ofconditions and the following disclaimer.
// 
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in
//    the documentation and/or other materialsprovided with the
//    distribution.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
// FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
// COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
// BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
// ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
#ifndef __EXAMPLE_MINI_HEADER_H__
#define __EXAMPLE_MINI_HEADER_H__

#ifndef __cplusplus
  #error requires C++ compilation (use a .cpp or .cc suffix)
#endif

#if defined(_MSC_VER) && (_MSC_VER >= 1400)
  #pragma warning(disable:4996)
#endif

#include <map>
#include <string>


namespace example_mini {

struct Example 
{
  virtual ~Example(void);
  virtual void release(void);

  virtual void do_Example(void) = 0;
};

class ExampleMgr 
{
  struct MgrData {
    std::string descriptor_;
    Example*    example_;
  };
  std::map<std::string, MgrData> mgrMap_;
public:
  ExampleMgr(void);
  ~ExampleMgr(void);

  static ExampleMgr& singleton(void);
  bool regExample(const char* cmd, const char* descriptor, Example* ex);
  Example* getExample(const char* cmd);
  void runHelp(void);
};

}


#define REG_EXAMPLE(_Cmd, _Descriptor, _ExampleClass)\
  static bool g_sLocal = example_mini::ExampleMgr::singleton().regExample(_Cmd, _Descriptor, new _ExampleClass())


#endif  // __EXAMPLE_MINI_HEADER_H__
