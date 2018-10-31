/*
Given a year, report if it is a leap year.

The tricky thing here is that a leap year in the Gregorian calendar occurs:

on every year that is evenly divisible by 4
  except every year that is evenly divisible by 100
    unless the year is also evenly divisible by 400

For example, 1997 is not a leap year, but 1996 is. 1900 is not a leap year, but 2000 is.

If your language provides a method in the standard library that does this look-up, pretend it doesn't exist and implement it yourself.
*/

#include <gtest/gtest.h>

bool is_multiple(int number, int base)
{
    return number % base == 0;
}

bool is_leap_year(int year)
{
    if (is_multiple(year, 400))
    {
        return true;
    }

    if (is_multiple(year, 100))
    {
        return false;
    }

    if (is_multiple(year, 4))
    {
        return true;
    }

    return false;
}

TEST(is_leap_year, returns_true_for_4)
{
    EXPECT_TRUE(is_leap_year(4));
}

TEST(is_leap_year, returns_false_for_5)
{
    EXPECT_FALSE(is_leap_year(5));
}

TEST(is_leap_year, returns_false_for_100)
{
    EXPECT_FALSE(is_leap_year(100));
}

TEST(is_leap_year, returns_true_for_400)
{
    EXPECT_TRUE(is_leap_year(400));
}
