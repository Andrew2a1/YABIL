#include <gtest/gtest.h>
#include <yabil/bigint/BigInt.h>

#include <limits>

using namespace yabil::bigint;

class BigIntMulOperator_tests : public ::testing::Test
{
};

TEST_F(BigIntMulOperator_tests, mulTwoZeros)
{
    const BigInt big_int1, big_int2;
    ASSERT_EQ(0, (big_int1 * big_int2).to_int());
}

TEST_F(BigIntMulOperator_tests, mulZeroAndNonZeroShouldAlwaysGiveZero)
{
    const BigInt zero;
    for (int i = -10; i < 10; ++i)
    {
        ASSERT_EQ(0, (zero * BigInt(i)).to_int());
    }
}

TEST_F(BigIntMulOperator_tests, mulTwoNonZero)
{
    const BigInt big_int1(50), big_int2(20);
    ASSERT_EQ(1000, (big_int1 * big_int2).to_int());
}

TEST_F(BigIntMulOperator_tests, mulTwoNonZeroWithOverflow)
{
    const BigInt big_int1(std::numeric_limits<bigint_base_t>::max());
    const BigInt big_int2(20);
    const std::vector<bigint_base_t> expected = {0b11111111111111111111111111101100, 0b10011};
    ASSERT_EQ(expected, (big_int1 * big_int2).raw_data());
}

TEST_F(BigIntMulOperator_tests, mulTwoLongNonZeroWithOverflow)
{
    const BigInt big_int1(std::vector<bigint_base_t>{std::numeric_limits<bigint_base_t>::max(),
                                                     std::numeric_limits<bigint_base_t>::max()});
    const BigInt big_int2(10);
    const std::vector<bigint_base_t> expected = {0b11111111111111111111111111110110, 0b11111111111111111111111111111111,
                                                 0b1001};
    ASSERT_EQ(expected, (big_int1 * big_int2).raw_data());
}

TEST_F(BigIntMulOperator_tests, mulTwoNegativeWithOverflow)
{
    const BigInt big_int1(std::vector<bigint_base_t>{std::numeric_limits<bigint_base_t>::max(),
                                                     std::numeric_limits<bigint_base_t>::max()},
                          Sign::Minus);
    const BigInt big_int2(-10);

    const std::vector<bigint_base_t> expected = {0b11111111111111111111111111110110, 0b11111111111111111111111111111111,
                                                 0b1001};
    const auto result = big_int1 * big_int2;

    ASSERT_EQ(expected, result.raw_data());
    ASSERT_EQ(Sign::Plus, result.get_sign());
}

TEST_F(BigIntMulOperator_tests, mulTwoWithDifferentSigns)
{
    for (int i = 1; i < 11; ++i)
    {
        const BigInt big_int1(i);
        const BigInt big_int2(-i);
        const auto result = big_int1 * big_int2;
        ASSERT_LT(result.to_int(), 0);
    }
}

TEST_F(BigIntMulOperator_tests, mulTwoWithDifferentSignsWithOverflow)
{
    const BigInt big_int1(std::vector<bigint_base_t>{0, std::numeric_limits<bigint_base_t>::max()}, Sign::Minus);
    const BigInt big_int2(std::numeric_limits<bigint_base_t>::max());

    const std::vector<bigint_base_t> expected = {0, 0b00000000000000000000000000000001,
                                                 0b11111111111111111111111111111110};
    const auto result = big_int1 * big_int2;

    ASSERT_EQ(expected, result.raw_data());
    ASSERT_EQ(Sign::Minus, result.get_sign());
}

TEST_F(BigIntMulOperator_tests, mulTwoWithDifferentSignsWithOverflow_2)
{
    const BigInt big_int1(std::vector<bigint_base_t>{0, std::numeric_limits<bigint_base_t>::max()});
    const BigInt big_int2(std::numeric_limits<bigint_base_t>::max(), Sign::Minus);

    const std::vector<bigint_base_t> expected = {0, 0b00000000000000000000000000000001,
                                                 0b11111111111111111111111111111110};
    const auto result = big_int1 * big_int2;

    ASSERT_EQ(expected, result.raw_data());
    ASSERT_EQ(Sign::Minus, result.get_sign());
}

TEST_F(BigIntMulOperator_tests, mulTwoTheSameLong)
{
    const BigInt big_int1(std::vector<bigint_base_t>{0, std::numeric_limits<bigint_base_t>::max()});
    const BigInt big_int2(std::vector<bigint_base_t>{0, std::numeric_limits<bigint_base_t>::max()});

    const std::vector<bigint_base_t> expected = {0, 0, 1, 0b11111111111111111111111111111110};
    const auto result = big_int1 * big_int2;

    ASSERT_EQ(expected, result.raw_data());
    ASSERT_EQ(Sign::Plus, result.get_sign());
}
