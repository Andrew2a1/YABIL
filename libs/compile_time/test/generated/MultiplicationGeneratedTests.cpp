#include <gtest/gtest.h>
#include <yabil/compile_time/ConstBigInt.h>
#include <yabil/compile_time/Literals.h>
#include <yabil/compile_time/detail/BigIntData.h>

namespace yabil::compile_time
{

using namespace yabil::compile_time::literals;

class ConstBigIntMultiplicationGeneratedTests : public ::testing::Test
{
};

TEST_F(ConstBigIntMultiplicationGeneratedTests, arithmetic)
{
    EXPECT_TRUE((92_bi * 33_bi) == 3036_bi);
    EXPECT_TRUE((198_bi * -408_bi) == -80784_bi);
    EXPECT_TRUE((-1357_bi * 4248_bi) == -5764536_bi);
    EXPECT_TRUE((41467_bi * 57311_bi) == 2376515237_bi);
    EXPECT_TRUE((-607849_bi * 440616_bi) == -267827994984_bi);
    EXPECT_TRUE((-9905693_bi * -8980823_bi) == 88961275525339_bi);
    EXPECT_TRUE((-96032484_bi * 93104076_bi) == -8941015688804784_bi);
    EXPECT_TRUE((-361633526_bi * 477406895_bi) == -172646338775561770_bi);
    EXPECT_TRUE((2233042561_bi * 3985936399_bi) == 8900765624406077839_bi);
    EXPECT_TRUE((-65380229231_bi * -28752094849_bi) == 1879818552099074331119_bi);
    EXPECT_TRUE((779165884772_bi * 148894733412_bi) == 116013696696852050402064_bi);
    EXPECT_TRUE((9193616795439_bi * -5764353386446_bi) == -52995256108475622097219794_bi);
    EXPECT_TRUE((-36300099259905_bi * 42719110148326_bi) == -1550707938679048807374669030_bi);
    EXPECT_TRUE((676812866726706_bi * -347498373909553_bi) == -235191370628593344019213622418_bi);
    EXPECT_TRUE((-8754286190612295_bi * 4326129101866735_bi) == -37872172255277928649317142506825_bi);
    EXPECT_TRUE((57693927712843997_bi * 28005202945869018_bi) == 1615730154342492880783131429584946_bi);
    EXPECT_TRUE((212783649627563550_bi * 556706910600826540_bi) == 118458128210529618723431796376617000_bi);
    EXPECT_TRUE((2147423520556496469_bi * 6267624435895379664_bi) == 13459244131656381417397192916430406416_bi);
}

}  // namespace yabil::compile_time
