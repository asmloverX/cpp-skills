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
#ifndef __SCOPED_LOCK_HEADER_H__
#define __SCOPED_LOCK_HEADER_H__

#ifndef __cplusplus
  #error requires C++ compilation (use a .cpp or .cc suffix)
#endif

template <typename _Mutex>
class scoped_lock : private noncopyable {
  _Mutex& mutex_;
  bool    locked_;
public:
  scoped_lock(_Mutex& mutex)
    : mutex_(mutex)
  {
    mutex_.lock();
    locked_ = true;
  }

  ~scoped_lock(void)
  {
    if (locked_)
      mutex_.unlock();
  }

  void lock(void)
  {
    if (!locked_)
    {
      mutex_.lock();
      locked_ = true;
    }
  }

  void unlock(void)
  {
    if (locked_)
    {
      mutex_.unlock();
      locked_ = false;
    }
  }

  bool locked(void) const 
  { return locked_; }

  _Mutex& mutex(void) const 
  { return muetx_; }
};

#endif  // __SCOPED_LOCK_HEADER_H__