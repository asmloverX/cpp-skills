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
#include "unit_mini.h"



namespace unit_mini {

void TestCase::result(bool success)
{
  result_ = success;
}

bool TestCase::result(void) const
{
  return result_;
}

const char* TestCase::name(void) const
{
  return name_;
}



TestUnit::TestUnit(void)
  : clr_(new Color)
{}

TestUnit::~TestUnit(void)
{
  int n = static_cast<int>(caseLists_.size());
  for (int i = 0; i < n; ++i)
  {
    if (NULL != caseLists_[i])
      delete caseLists_[i];
  }
  caseLists_.clear();

  delete clr_;
}

TestUnit& TestUnit::singleton(void)
{
  static TestUnit s;
  return s;
}

TestCase* TestUnit::regTestCase(TestCase* testCase)
{
  caseLists_.push_back(testCase);
  return testCase;
}

TestCase* TestUnit::getCurrent(void) const
{
  return current_;
}

Color* TestUnit::color(void) const
{
  return clr_;
}

int TestUnit::run(void)
{
  int ret = 0;
  std::vector<TestCase*>::iterator i;

  clr_->save();
  for (i = caseLists_.begin(); i != caseLists_.end(); ++i)
  {
    current_ = *i;

    clr_->green();
    fprintf(stdout, "=========================================================\n");
    fprintf(stdout, "Run TestCase : %s\n", (*i)->name());
    (*i)->run();
    clr_->green();
    fprintf(stdout, "End TestCase : %s\n", (*i)->name());

    if ((*i)->result())
      ++passed_;
    else
    {
      ++failed_;
      ret = -1;
    }
  }
  clr_->green();
  fprintf(stdout, "=========================================================\n");
  fprintf(stdout, "Total TestCase : %d\n", passed_ + failed_);
  fprintf(stdout, "Passed : %d\n", passed_);
  clr_->red();
  fprintf(stdout, "Failed : %d\n", failed_);
  clr_->reset();

  return ret;
}

}
