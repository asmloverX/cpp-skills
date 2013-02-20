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
#include "../stdafx.h"
#include "../inc/win32/win32_mutex.h"
#include "../inc/threadbase.h"
#include "../inc/philosophereat.h"


class Philosopher;
class PhilosopherEatImpl : private noncopyable {
  enum { PHILOSOPHER_COUNT = 5, };

  win32_mutex mutex_;
  win32_mutex mutexs_[PHILOSOPHER_COUNT];
  std::vector<Philosopher*> philosophers_;
public:
  void show(int index, const char* msg)
  {
    mutex_.lock();
    fprintf(stdout, "Philosopher[%d] ===> %s\n", index, msg);
    mutex_.unlock();
  }

  void do_PhilosopherEat(void);
};

//! ’‹—ßº“¿‡
class Philosopher : private noncopyable {
  enum { CHOPSTICKS_COUNT = 5, PHILOSOPHER_SLEEP = 2000, };

  PhilosopherEatImpl* impl_;
  win32_mutex*        chopsticksMutexs_;
  int                 id_;
  ThreadBase          tb_;
public:
  Philosopher(PhilosopherEatImpl* impl, win32_mutex* chopsticks, int id)
    : impl_(impl)
    , chopsticksMutexs_(chopsticks)
    , id_(id)
  {
    assert(NULL != impl_);
    assert(NULL != chopsticksMutexs_);
  }

  ~Philosopher(void) 
  {}

  bool start(void)
  {
    return tb_.startThread(&Philosopher::sEatThread, this, 0u);
  }

  void stop(void)
  {
    tb_.stopThread();
  }
private:
  static bool sEatThread(void* arg)
  {
    Philosopher* self = static_cast<Philosopher*>(arg);

    srand((UINT)GetTickCount());
    if (NULL != self)
    {
      self->think();
      self->wait();
      self->eat();

      return true;
    }
    return false;
  }

  void think(void)
  {
    impl_->show(id_, "is thinking ...");
    Sleep((DWORD)rand() % PHILOSOPHER_SLEEP);
  }

  void wait(void)
  {
    impl_->show(id_, "is waiting for eating ...");
    if (0 == id_ % 2)
    {
      chopsticksMutexs_[id_].lock();
      chopsticksMutexs_[(id_ + 1) % CHOPSTICKS_COUNT].lock();
    }
    else 
    {
      chopsticksMutexs_[(id_ + 1) % CHOPSTICKS_COUNT].lock();
      chopsticksMutexs_[id_].lock();
    }
  }

  void eat(void)
  {
    impl_->show(id_, "is eating ...");
    Sleep((DWORD)rand() % PHILOSOPHER_SLEEP);
    chopsticksMutexs_[(id_ + 1) % CHOPSTICKS_COUNT].unlock();
    chopsticksMutexs_[id_].unlock();
  }
};



void PhilosopherEatImpl::do_PhilosopherEat(void)
{
  for (int i = 0; i < PHILOSOPHER_COUNT; ++i)
  {
    Philosopher* p = new Philosopher(this, mutexs_, i);
    p->start();

    philosophers_.push_back(p);
  }

  int count = 0;
  while (count++ < 500)
    Sleep(100);
  
  for (int i = 0; i < (int)philosophers_.size(); ++i)
  {
    if (NULL != philosophers_[i])
    {
      philosophers_[i]->stop();
      delete philosophers_[i];
    }
  }
}




PhilosopherEat::PhilosopherEat(void)
{
  impl_ = new PhilosopherEatImpl();
  assert(NULL != impl_);
}

PhilosopherEat::~PhilosopherEat(void)
{
  delete impl_;
}

void PhilosopherEat::do_PhilosopherEat(void)
{
  impl_->do_PhilosopherEat();
}