// Copyright (c) 2012, ASMlove. All rights reserved.
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
#ifndef __THREADBASE_HEDER_H__
#define __THREADBASE_HEDER_H__

#ifndef __cplusplus
  #error requires C++ compilation (use a .cpp or .cc suffix)
#endif

#include "noncopyable.h"

typedef bool (*pTHREADCALLBACK)(void* arg);
class ThreadBase : private noncopyable {
  HANDLE hThread_;
  HANDLE hStartEv_;
  HANDLE hStopEv_;
  bool   alived_;
  DWORD  loopStep_;
  pTHREADCALLBACK callback_;

  struct Arguments {
    ThreadBase* self;
    void*       args;
  };
public:
  ThreadBase(void);
  virtual ~ThreadBase(void);

  bool startThread(pTHREADCALLBACK cb, void* arg, DWORD step = 5u);
  void stopThread(void);

  bool alived(void) const;
private:
  static UINT WINAPI sThread(void* arg);
};

#endif  // __THREADBASE_HEDER_H__