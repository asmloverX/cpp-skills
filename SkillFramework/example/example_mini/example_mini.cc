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
#include "example_mini.h"




namespace example_mini {

//! Example
Example::~Example(void)
{}

void Example::release(void)
{
  delete this;
}



//! ExampleMgr
ExampleMgr::ExampleMgr(void)
{}

ExampleMgr::~ExampleMgr(void)
{
  std::map<std::string, MgrData>::iterator i;
  for (i = mgrMap_.begin(); i != mgrMap_.end(); ++i)
    i->second.example_->release();
  mgrMap_.clear();
}

ExampleMgr& ExampleMgr::singleton(void)
{
  static ExampleMgr s;
  return s;
}

bool ExampleMgr::regExample(const char* cmd, const char* descriptor, Example* ex)
{
  if (NULL != cmd && NULL != descriptor) 
  {
    MgrData dat;
    dat.descriptor_ = descriptor;
    dat.example_    = ex;

    std::map<std::string, MgrData>::iterator it = mgrMap_.find(cmd);
    if (mgrMap_.end() == it) 
    {
      mgrMap_[cmd] = dat;
      return true;
    }
  }
  return false;
}

Example* ExampleMgr::getExample(const char* cmd)
{
  std::map<std::string, MgrData>::iterator it = mgrMap_.find(NULL != cmd ? cmd : "");
  return (mgrMap_.end() != it ? it->second.example_ : NULL);
}

void ExampleMgr::runHelp(void)
{
  std::map<std::string, MgrData>::iterator i;

  fprintf(stdout, "**************************[ Help ]****************************\n");
  fprintf(stdout, "  [exit]\t: exit\n");
  for (i = mgrMap_.begin(); mgrMap_.end() != i; ++i) 
    fprintf(stdout, "  [%s]\t: %s\n", i->first.c_str(), i->second.descriptor_.c_str());
  fprintf(stdout, "**************************************************************\n");
}

}




//! ExampleHelp
class ExampleHelp : public example_mini::Example 
{
public:
  virtual void do_Example(void)
  {
    example_mini::ExampleMgr::singleton().runHelp();
  }
};
REG_EXAMPLE("help", "Help for Example", ExampleHelp);