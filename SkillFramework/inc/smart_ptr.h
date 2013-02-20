// Copyright (c) 2011, ASMlove. All rights reserved.
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
#ifndef __SMART_POINTER_HEADER_H__
#define __SMART_POINTER_HEADER_H__

#ifndef __cplusplus
  #error requires C++ compilation (use a .cpp or .cc suffix)
#endif

template <typename Tp>
class SmartPointer 
{
  struct __InternalData {
    int count_;
    Tp* pointer_;
  };
  __InternalData* pData_;

  void inc(void)
  {
    ++pData_->count_;
  }

  void dec(void)
  {
    if (0 != pData_) 
    {
      if (0 == --pData_->count_) 
      {
        delete pData_->pointer_;
        delete pData_;
        pData_ = 0;
      }
    }
  }
public:
  SmartPointer(void) : pData_(0)
  {}

  SmartPointer(Tp* p)
  {
    pData_ = new __InternalData;
    pData_->pointer_ = p;
    pData_->count_   = 1;
  }

  SmartPointer(const SmartPointer<Tp>& other)
  {
    pData_ = other.pData_;
    inc();
  }

  ~SmartPointer(void)
  {
    dec();
  }

  operator bool(void) const 
  {
    return (0 != pData_);
  }

  SmartPointer<Tp>& operator =(Tp* p)
  {
    dec();
    pData_ = new __InternalData;
    pData_->pointer_ = p;
    pData_->count_   = 1;
    return *this;
  }

  SmartPointer<Tp>& operator =(const SmartPointer<Tp>& other)
  {
    dec();
    pData_ = other.pData_;
    inc();
    return *this;
  }

  bool operator ==(Tp* p) const 
  {
    return (0 != pData_ ? (pData_->pointer_ == p) : (0 == p ? true : false));
  }

  bool operator ==(const SmartPointer<Tp>& other) const
  {
    return (pData_ == other.pData_);
  }
};

#endif  // __SMART_POINTER_HEADER_H__
