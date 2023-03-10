#include <gtest/gtest.h>
#include <yabil/bigint/BigInt.h>

#include <limits>

using namespace yabil::bigint;

class BigIntBitOperations_tests : public ::testing::Test
{
};

TEST_F(BigIntBitOperations_tests, getBitFromZeroShouldGiveZero)
{
    const BigInt big_int;
    for (std::size_t n = 0; n < 20; ++n)
    {
        ASSERT_EQ(0, big_int.get_bit(n));
    }
}

TEST_F(BigIntBitOperations_tests, getBitFromSingleItem)
{
    const BigInt big_int(1);
    ASSERT_EQ(1, big_int.get_bit(0));
    for (std::size_t n = 1; n < 100; ++n)
    {
        ASSERT_EQ(0, big_int.get_bit(n));
    }
}

TEST_F(BigIntBitOperations_tests, getBitFromLongNumber)
{
    const BigInt big_int(std::vector<bigint_base_t>{0b0101010101010101, 0b0101010101010101});
    ASSERT_EQ(1, big_int.get_bit(32));
    ASSERT_EQ(0, big_int.get_bit(60));
}

TEST_F(BigIntBitOperations_tests, getBitFromMiddleOfLongNumber)
{
    const BigInt big_int(std::vector<bigint_base_t>{0b0101010101010101, 0b0101010101010101, 0b0101010101010101});
    ASSERT_EQ(1, big_int.get_bit(38));
    ASSERT_EQ(0, big_int.get_bit(69));
}

TEST_F(BigIntBitOperations_tests, setBitForZero)
{
    BigInt big_int;
    big_int.set_bit(0, true);
    ASSERT_EQ(1, big_int.to_int());
}

TEST_F(BigIntBitOperations_tests, setBitForZeroLongNumber)
{
    BigInt big_int;
    const std::vector<bigint_base_t> expected = {0, 0, 0, 0b10};

    big_int.set_bit(97, true);
    ASSERT_EQ(expected, big_int.raw_data());
}

TEST_F(BigIntBitOperations_tests, setZeroForLongNumber)
{
    BigInt big_int(std::vector<bigint_base_t>{0, 0, 0, 0b10});
    big_int.set_bit(97, false);
    ASSERT_EQ(0, big_int.to_int());
}

TEST_F(BigIntBitOperations_tests, clearingZeroShouldNotAffectNumber)
{
    BigInt big_int;
    big_int.set_bit(12, false);
    ASSERT_EQ(0, big_int.to_int());
}

TEST_F(BigIntBitOperations_tests, clearingZeroInLongNumber)
{
    BigInt big_int(std::vector<bigint_base_t>{0b0101010101010101, 0b0101010101010101, 0b0101010101010101});
    const std::vector<bigint_base_t> expected = {0b0101010101010101, 0b0101010101010101, 0b0101010101010001};

    big_int.set_bit(66, false);
    ASSERT_EQ(expected, big_int.raw_data());
}
