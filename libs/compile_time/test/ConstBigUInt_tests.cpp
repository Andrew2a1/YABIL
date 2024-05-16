#include <gtest/gtest.h>
#include <yabil/bigint/BigIntBase.h>
#include <yabil/compile_time/ConstBigUInt.h>

using namespace yabil::compile_time;
using digit_type = yabil::bigint::bigint_base_t;

class CompileTimeBigUInt_tests : public ::testing::Test
{
};

TEST_F(CompileTimeBigUInt_tests, canCreateBigUIntForValue0)
{
    constexpr ConstBigUInt<4> a;
    EXPECT_TRUE(a.is_zero());
}

TEST_F(CompileTimeBigUInt_tests, canCreateBigUIntForValue0_2)
{
    constexpr ConstBigUInt<0> a;
    EXPECT_TRUE(a.is_zero());
}

TEST_F(CompileTimeBigUInt_tests, canCreateBigUIntForAnyArray)
{
    constexpr std::array<digit_type, 3> data{0, 1, 0};
    constexpr ConstBigUInt a(data);
    EXPECT_FALSE(a.is_zero());
}

TEST_F(CompileTimeBigUInt_tests, canCheckIfTwoBigUIntsAreEqual)
{
    {
        constexpr ConstBigUInt a(std::array<digit_type, 3>{1, 1, 1});
        constexpr ConstBigUInt b(std::array<digit_type, 2>{1, 1});
        EXPECT_TRUE(a != b);
    }
    {
        constexpr ConstBigUInt a(std::array<digit_type, 3>{1, 1, 1});
        constexpr ConstBigUInt b(std::array<digit_type, 3>{1, 1, 2});
        EXPECT_FALSE(a == b);
    }
    {
        constexpr ConstBigUInt a(std::array<digit_type, 3>{1, 1, 1});
        constexpr ConstBigUInt b(std::array<digit_type, 3>{1, 1, 1});
        EXPECT_TRUE(a == b);
    }
    {
        constexpr ConstBigUInt a(std::array<digit_type, 3>{1, 0, 0});
        constexpr ConstBigUInt b(std::array<digit_type, 3>{1, 0});
        EXPECT_TRUE(a == b);
    }
}

TEST_F(CompileTimeBigUInt_tests, canAddTwoNumbers)
{
    constexpr ConstBigUInt a(std::array<digit_type, 3>{1, 1, 1});
    constexpr ConstBigUInt b(std::array<digit_type, 2>{1, 1});
    constexpr ConstBigUInt expected(std::array<digit_type, 3>{2, 2, 1});
    constexpr auto sum = a + b;
    static_assert(sum == expected);
    EXPECT_TRUE(sum == expected);
}

TEST_F(CompileTimeBigUInt_tests, canAddZeroAndNonZero)
{
    constexpr ConstBigUInt a(std::array<digit_type, 2>{1, 1});
    constexpr ConstBigUInt<0> b;
    constexpr ConstBigUInt expected(a);
    constexpr auto sum = a + b;
    static_assert(sum == expected);
    static_assert(a + b == b + a);
    EXPECT_TRUE(sum == expected);
}

TEST_F(CompileTimeBigUInt_tests, canAddLongerNumberToShorter)
{
    constexpr ConstBigUInt<0> a;
    constexpr ConstBigUInt b(std::array<digit_type, 2>{1, 1});
    constexpr ConstBigUInt expected(b);
    constexpr auto sum = a + b;
    static_assert(sum == expected);
    EXPECT_TRUE(sum == expected) << "sum: " << sum << ", expected: " << expected;
}