#include <gtest/gtest.h>
#include <yabil/bigint/BigInt.h>

using namespace yabil::bigint;

class BigIntConstructorTest : public ::testing::Test
{
};

TEST_F(BigIntConstructorTest, canConstructBigIntFromRawData)
{
    BigInt big_int1(std::vector<bigint_base_t>({0xff, 0xff}));
    BigInt big_int2(std::vector<bigint_base_t>({0x02, 0x01}), Sign::Minus);
}

TEST_F(BigIntConstructorTest, canConstructBigIntFromUnsignedNumber)
{
    BigInt big_int1(0xffff0abcULL);
}

TEST_F(BigIntConstructorTest, canConstructBigIntFromSignedNumber)
{
    BigInt big_int1(-192931829LL);
}
