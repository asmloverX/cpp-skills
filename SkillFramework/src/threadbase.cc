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
#include "../stdafx.h"
#include "../inc/threadbase.h"


ThreadBase::ThreadBase(void)
  : hThread_(NULL)
  , hStartEv_(NULL)
  , hStopEv_(NULL)
  , alived_(false)
  , loopStep_(0u)
  , callback_(NULL)
{}

ThreadBase::~ThreadBase(void)
{}

bool ThreadBase::startThread(pTHREADCALLBACK cb, void* arg, DWORD step)
{
  loopStep_ = step;
  callback_ = cb;
  assert(NULL != callback_);
  hStartEv_ = CreateEvent(NULL, TRUE, FALSE, NULL);
  assert(NULL != hStartEv_);
  hStopEv_ = CreateEvent(NULL, TRUE, FALSE, NULL);
  assert(NULL != hStopEv_);

  Arguments args;
  args.self = this;
  args.args = arg;
  hThread_ = (HANDLE)_beginthreadex(NULL, 0u, &ThreadBase::sThread, &args, 0u, NULL);
  if (NULL != hThread_)
    WaitForSingleObject(hStartEv_, INFINITE);
  CloseHandle(hStartEv_);

  return (NULL != hThread_);
}

void ThreadBase::stopThread(void)
{
  if (NULL != hThread_)
  {
    SetEvent(hStopEv_);
    if (WAIT_TIMEOUT == WaitForSingleObject(hThread_, INFINITE))
      TerminateThread(hThread_, ~0u);
    CloseHandle(hThread_);
  }
  CloseHandle(hStopEv_);
}

bool ThreadBase::alived(void) const 
{ return alived_; }


UINT WINAPI ThreadBase::sThread(void* arg)
{
  ThreadBase* self = static_cast<Arguments*>(arg)->self;
  void*       args = static_cast<Arguments*>(arg)->args;

  if (NULL == self)
    return ~0u;
  SetEvent(self->hStartEv_);

  self->alived_ = true;
  while (WAIT_OBJECT_0 != WaitForSingleObject(self->hStopEv_, self->loopStep_))
  {
    if (!self->callback_(args))
      break;
  }
  self->alived_ = false;

  return 0u;
}
