#include "./unit_mini/unit_mini.h"


namespace test {

int add(int a, int b)
{
  return a + b;
}

int sub(int a, int b)
{
  return a - b;
}

int mul(int a, int b)
{
  return a * b;
}

int div(int a, int b)
{
  return a / b;
}

int mod(int a, int b)
{
  return a % b;
}



UNITMINI_TEST(Function_add)
{
  EXPECT_EQ(3, add(1, 2));
  EXPECT_EQ(4, add(2, 2));
  //EXPECT_EQ(5, add(3, 3));
  ASSERT_TRUE(5 == add(2, 3));
}

UNITMINI_TEST(Function_sub)
{
  EXPECT_EQ(3, sub(34, 31));
  EXPECT_TRUE(3== sub(34, 31));
}

UNITMINI_TEST(Function_mul)
{
  EXPECT_EQ(45, mul(5, 9));
  ASSERT_TRUE(45 == mul(5, 9));
}

UNITMINI_TEST(Function_div)
{
  EXPECT_EQ(0, div(0, 45));
  EXPECT_EQ(0, div(0, 2));
  EXPECT_TRUE(0 == div(0, 45));
}

UNITMINI_TEST(Function_mod)
{
  EXPECT_EQ(1, mod(3, 2));
  EXPECT_EQ(0, mod(100, 5));
  ASSERT_TRUE(0 == mod(100, 25));
}

}
