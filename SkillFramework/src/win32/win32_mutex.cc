// Copyright (c) 2012, ASMlover. All rights reserved.
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
#include "../../stdafx.h"
#include "../../inc/win32/win32_mutex.h"


win32_mutex::win32_mutex(void)
{
  int err = do_init();

  UNREFERENCED_PARAMETER(err);
  //!< TODO
}

win32_mutex::~win32_mutex(void)
{
  DeleteCriticalSection(&cs_);
}

void win32_mutex::lock(void)
{
  int err = do_lock();

  UNREFERENCED_PARAMETER(err);
  //!< TODO
}

void win32_mutex::unlock(void)
{
  LeaveCriticalSection(&cs_);
}


int win32_mutex::do_init(void)
{
  __try 
  {
    InitializeCriticalSection(&cs_);
  }
  __except (STATUS_NO_MEMORY == GetExceptionCode() 
    ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH)
  {
    return ERROR_OUTOFMEMORY;
  }
  return 0;
}

int win32_mutex::do_lock(void)
{
  __try 
  {
    EnterCriticalSection(&cs_);
  }
  __except (STATUS_INVALID_HANDLE == GetExceptionCode() 
    || STATUS_NO_MEMORY ==  GetExceptionCode()
    ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH)
  {
    return (STATUS_NO_MEMORY == GetExceptionCode() 
      ? ERROR_OUTOFMEMORY : ERROR_INVALID_HANDLE);
  }
  return 0;
}
