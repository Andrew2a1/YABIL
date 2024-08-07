#include <gtest/gtest.h>
#include <yabil/bigint/BigIntBase.h>
#include <yabil/compile_time/ConstBigInt.h>
#include <yabil/compile_time/Literals.h>

namespace yabil::compile_time
{

using namespace yabil::compile_time::literals;

class ConstBigIntMultiplication_tests : public ::testing::Test
{
};

TEST_F(ConstBigIntMultiplication_tests, canMulZeroAndZero)
{
    EXPECT_TRUE(0_bi * 0_bi == 0_bi);
}

TEST_F(ConstBigIntMultiplication_tests, canMulZeroAndNonZero)
{
    EXPECT_TRUE(125_bi * 0_bi == 0_bi);
}

TEST_F(ConstBigIntMultiplication_tests, canMulTwoSmallNumbers)
{
    EXPECT_TRUE(91289123_bi * 2174981279412490_bi == 198552133538984167346270_bi);
}

TEST_F(ConstBigIntMultiplication_tests, canMulTwoBigNumbers)
{
    {
        constexpr auto val = 198552133531985521335389841673462708984167346270_bi * 2174981279412490_bi;
        EXPECT_TRUE(val == 431847173419477426164136693502190378937075718369239667992912300_bi);
    }
    {
        constexpr auto val = 2174981279412490_bi * 198552133531985521335389841673462708984167346270_bi;
        EXPECT_TRUE(val == 431847173419477426164136693502190378937075718369239667992912300_bi);
    }
}

TEST_F(ConstBigIntMultiplication_tests, canMulNegativeAndPositive)
{
    EXPECT_TRUE((-125_bi) * 100_bi == -12500_bi);
}

TEST_F(ConstBigIntMultiplication_tests, canMulTwoNegativeNumbers)
{
    EXPECT_TRUE((-91289123_bi) * (-2174981279412490_bi) == 198552133538984167346270_bi);
}

TEST_F(ConstBigIntMultiplication_tests, canMulNegativeAndZero)
{
    EXPECT_TRUE((-125_bi) * 0_bi == 0_bi);
}

TEST_F(ConstBigIntMultiplication_tests, canMulZeroAndNegative)
{
    EXPECT_TRUE(0_bi * (-125_bi) == 0_bi);
}

}  // namespace yabil::compile_time
