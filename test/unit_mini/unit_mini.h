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
#ifndef __UNITMINI_TEST_HEADER_H__
#define __UNITMINI_TEST_HEADER_H__

#ifndef  __cplusplus
  #error requires C++ compilation (use a .cpp or .cc suffix)
#endif

#include <vector>
#include "unit_color.h"

namespace unit_mini {

//! @class TestCase
//! @brief base class of TestCase
class TestCase 
{
  bool result_;     //!< result of the test case
  const char* name_;//!< name of the test case
public:
  TestCase(const char* name)
    : result_(true), name_(name)
  {}
  virtual ~TestCase(void)
  {}

  void result(bool success);    //!< set result of the test case
  bool result(void) const;      //!< get result of the test case
  const char* name(void) const; //!< get name of the test case

  virtual void run(void) = 0;   //!< run this test case
};

//! @class TestCase
//! @brief unit of the test
class TestUnit 
{
  int passed_;        //!< number of passed test cases
  int failed_;        //!< number if failed test cases

  Color*    clr_;     //!< message color object
  TestCase* current_; //!< current test case
  std::vector<TestCase*> caseLists_; //!< test case list
public:
  TestUnit(void);
  ~TestUnit(void);

  static TestUnit& singleton(void);
  TestCase* regTestCase(TestCase* testCase);  //!< register a test case into the test unit
  TestCase* getCurrent(void) const;           //!< get current test case
  Color* color(void) const;                   //!< get the color object

  int run(void);                              //!< run all tests of this test unit
};

}

#define TCASE_NAME(_TCName) _TCName##_TestCase

#define TCASE_(_TCName)\
class TCASE_NAME(_TCName) : public unit_mini::TestCase\
{\
  static unit_mini::TestCase* const case_;\
public:\
  TCASE_NAME(_TCName)(const char* name)\
    : unit_mini::TestCase(name)\
  {}\
  virtual void run(void);\
};\
unit_mini::TestCase* const TCASE_NAME(_TCName)\
  ::case_ = unit_mini::TestUnit::singleton().regTestCase(new TCASE_NAME(_TCName)(#_TCName));\
void TCASE_NAME(_TCName)::run(void)

#define UNITMINI_TEST_BOOLEAN_(condition)\
  try\
  {\
    if (!(condition))\
    {\
      unit_mini::TestUnit::singleton().getCurrent()->result(false);\
      unit_mini::TestUnit::singleton().color()->red();\
      fprintf(stdout, "Failed : %s (%d)\n", __FILE__, __LINE__);\
      fprintf(stdout, "Expect : " #condition "\n");\
    }\
  }\
  catch (...)\
  {\
    unit_mini::TestUnit::singleton().getCurrent()->result(false);\
    unit_mini::TestUnit::singleton().color()->red();\
    fprintf(stdout, "Exception : %s (%d)\n", __FILE__, __LINE__);\
    exit(1);\
  }



#define UNITMINI_TEST(testCaseName) TCASE_(testCaseName)

#define RUN_ALL_TESTS()             unit_mini::TestUnit::singleton().run()

#define EXPECT_EQ(expected, actual)\
  try\
  {\
    if ((expected) != (actual))\
    {\
      unit_mini::TestUnit::singleton().getCurrent()->result(false);\
      unit_mini::TestUnit::singleton().color()->red();\
      fprintf(stdout, "Failed : %s (%d)\n", __FILE__, __LINE__);\
      fprintf(stdout, "Expect : " #expected "\n");\
      fprintf(stdout, "Actual : " #actual "\n");\
    }\
  }\
  catch (...)\
  {\
    unit_mini::TestUnit::singleton().getCurrent()->result(false);\
    unit_mini::TestUnit::singleton().color()->red();\
    fprintf(stdout, "Exception : %s (%d)\n", __FILE__, __LINE__);\
    exit(1);\
  }

#define EXPECT_TRUE(condition)    UNITMINI_TEST_BOOLEAN_(condition)
#define ASSERT_TRUE(condition)    UNITMINI_TEST_BOOLEAN_(condition)


#endif  // __UNITMINI_TEST_HEADER_H__
