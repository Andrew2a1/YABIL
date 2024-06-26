#include <gtest/gtest.h>
#include <yabil/bigint/BigInt.h>

#include <stdexcept>

using namespace yabil::bigint;

class BigIntDivOperator_tests : public ::testing::Test
{
};

TEST_F(BigIntDivOperator_tests, fastModuloZeroByAnyShouldGiveZero)
{
    const BigInt big_int;
    for (unsigned i = 1; i < 10; ++i)
    {
        EXPECT_EQ(0, big_int % i);
    }
}

TEST_F(BigIntDivOperator_tests, fastModuloByZeroShouldThrowException)
{
    ASSERT_THROW({ BigInt() % 0; }, std::invalid_argument);
}

TEST_F(BigIntDivOperator_tests, fastModuloDivTwoNonZeroGetRemainder)
{
    const BigInt big_int1(50);
    EXPECT_EQ(10, big_int1 % 20);
}

TEST_F(BigIntDivOperator_tests, fastModuloWithLargeInput)
{
    const BigInt big_int1(
        "12712642178621745214167236126412748678126782148251752175635217357125381236187236512678312857198272713872183");
    EXPECT_EQ(5215, big_int1 % 6134);
}

TEST_F(BigIntDivOperator_tests, zeroDivAnyShouldGiveZero)
{
    const BigInt big_int;
    for (int i = 1; i < 10; ++i)
    {
        EXPECT_EQ(0, (big_int / BigInt(i)).to_int());
        EXPECT_EQ(0, (big_int / BigInt(-i)).to_int());
    }
}

TEST_F(BigIntDivOperator_tests, divisionByZeroShouldThrowException)
{
    const BigInt big_int(1);
    ASSERT_THROW(big_int / BigInt(0), std::invalid_argument);
    ASSERT_THROW(big_int % BigInt(0), std::invalid_argument);
    ASSERT_THROW(big_int.divide(BigInt(0)), std::invalid_argument);
}

TEST_F(BigIntDivOperator_tests, divTwoNonZeroGetQuotient)
{
    const BigInt big_int1(50), big_int2(20);
    EXPECT_EQ(2, (big_int1 / big_int2).to_int());
}

TEST_F(BigIntDivOperator_tests, divTwoNonZeroGetRemainder)
{
    const BigInt big_int1(50), big_int2(20);
    EXPECT_EQ(10, (big_int1 % big_int2).to_int());
}

TEST_F(BigIntDivOperator_tests, divTwoNonZeroGetQuotientAndRemainder)
{
    const BigInt big_int1(50), big_int2(20);
    const auto [quotient, remainder] = big_int1.divide(big_int2);
    EXPECT_EQ(2, quotient.to_int());
    EXPECT_EQ(10, remainder.to_int());
}

TEST_F(BigIntDivOperator_tests, divTwoLongNonZeroWithOverflow)
{
    const BigInt big_int1("10928490128490812093812903");
    const BigInt big_int2("128371823");

    const BigInt expected_quotioent("85131533331039570");
    const BigInt expected_remainder(7776793);

    const auto [quotient, remainder] = big_int1.divide(big_int2);

    EXPECT_EQ(expected_quotioent, quotient);
    EXPECT_EQ(expected_remainder, remainder);
}

TEST_F(BigIntDivOperator_tests, divTwoLongNonZeroWithTheSameSign)
{
    const BigInt big_int1("192381902390120000");
    const BigInt big_int2("124109200000");

    const BigInt expected_quotioent("1550101");
    const BigInt expected_remainder("107360920000");

    const auto [quotient, remainder] = big_int1.divide(big_int2);
    EXPECT_EQ(expected_quotioent, quotient);
    EXPECT_EQ(expected_remainder, remainder);
}

TEST_F(BigIntDivOperator_tests, divTwoLongNonZeroMinusPlus)
{
    const BigInt big_int1("-1923819023901200000000");
    const BigInt big_int2("1241092000000000");

    const BigInt expected_quotioent("-1550101");
    const BigInt expected_remainder("-1073609200000000");

    const auto [quotient, remainder] = big_int1.divide(big_int2);
    EXPECT_EQ(expected_quotioent, quotient);
    EXPECT_EQ(expected_remainder, remainder);
}

TEST_F(BigIntDivOperator_tests, divTwoLongNonZeroPlusMinus)
{
    const BigInt big_int1("1923819023901200000000");
    const BigInt big_int2("-1241092000000000");

    const BigInt expected_quotioent("-1550101");
    const BigInt expected_remainder("1073609200000000");

    const auto [quotient, remainder] = big_int1.divide(big_int2);

    EXPECT_EQ(expected_quotioent.raw_data(), quotient.raw_data());
    EXPECT_EQ(expected_remainder.raw_data(), remainder.raw_data());
    EXPECT_EQ(expected_quotioent.get_sign(), quotient.get_sign());
    EXPECT_EQ(expected_remainder.get_sign(), remainder.get_sign());
}

TEST_F(BigIntDivOperator_tests, divTwoLongNonZeroWithMinusMinus)
{
    const BigInt big_int1("-1923819023901200000000");
    const BigInt big_int2("-1241092000000000");

    const BigInt expected_quotioent("1550101");
    const BigInt expected_remainder("-1073609200000000");

    const auto [quotient, remainder] = big_int1.divide(big_int2);
    EXPECT_EQ(expected_quotioent, quotient);
    EXPECT_EQ(expected_remainder, remainder);
}

TEST_F(BigIntDivOperator_tests, denominatorBiggerThanNominator)
{
    const BigInt big_int1("129319");
    const BigInt big_int2("12410920");

    const BigInt expected_remainder(129319);
    const auto [quotient, remainder] = big_int1.divide(big_int2);

    ASSERT_TRUE(quotient.is_zero());
    EXPECT_EQ(expected_remainder, remainder);

    EXPECT_EQ(Sign::Plus, quotient.get_sign());
    EXPECT_EQ(Sign::Plus, remainder.get_sign());
}

TEST_F(BigIntDivOperator_tests, noReminderTestPlusPlus)
{
    const BigInt big_int1(2048);
    const BigInt big_int2(2);

    const BigInt expected_quotioent(1024);
    const BigInt expected_remainder(0);

    const auto [quotient, remainder] = big_int1.divide(big_int2);
    EXPECT_EQ(expected_quotioent, quotient);
    EXPECT_EQ(expected_remainder, remainder);
}

TEST_F(BigIntDivOperator_tests, noReminderTestMinusPlus)
{
    const BigInt big_int1(-2048);
    const BigInt big_int2(2);

    const BigInt expected_quotioent(-1024);
    const BigInt expected_remainder(0);

    const auto [quotient, remainder] = big_int1.divide(big_int2);
    EXPECT_EQ(expected_quotioent, quotient);
    EXPECT_EQ(expected_remainder, remainder);
}

TEST_F(BigIntDivOperator_tests, noReminderTestPlusMinus)
{
    const BigInt big_int1(2048);
    const BigInt big_int2(-2);

    const BigInt expected_quotioent(-1024);
    const BigInt expected_remainder(0);

    const auto [quotient, remainder] = big_int1.divide(big_int2);
    EXPECT_EQ(expected_quotioent, quotient);
    EXPECT_EQ(expected_remainder, remainder);
}

TEST_F(BigIntDivOperator_tests, noReminderTestMinusMinus)
{
    const BigInt big_int1(-2048);
    const BigInt big_int2(-2);

    const BigInt expected_quotioent(1024);
    const BigInt expected_remainder(0);

    const auto [quotient, remainder] = big_int1.divide(big_int2);
    EXPECT_EQ(expected_quotioent, quotient);
    EXPECT_EQ(expected_remainder, remainder);
}

TEST_F(BigIntDivOperator_tests, inPlaceDivision)
{
    BigInt big_int1(-2048);
    BigInt big_int2(-2);

    big_int1 /= big_int2;

    EXPECT_EQ(1024, big_int1.to_int());
}

TEST_F(BigIntDivOperator_tests, inPlaceModulo)
{
    BigInt big_int1(2049);
    BigInt big_int2(2);

    big_int1 %= big_int2;

    EXPECT_EQ(1, big_int1.to_int());
}

TEST_F(BigIntDivOperator_tests, divTwoLongNumbers)
{
    const BigInt big_int1("19238190239012");
    const BigInt big_int2("-12410920");

    const BigInt expected_quotioent("-1550101");

    const auto quotient = big_int1 / big_int2;
    EXPECT_EQ(expected_quotioent, quotient);
}

TEST_F(BigIntDivOperator_tests, divHugeNumbers)
{
    const BigInt big_int1(
        "12712642178621745214167236126412748678126782148251752175635217357125381236187236512678312857198272713872183");
    const BigInt big_int2("123216874726781263781628736219678531949374081935798615619386573625");

    const BigInt expected_quotioent("103172899059568864307005240092688842441637");
    const BigInt expected_remainder("41901113651699220090698360505119628087373313170391250252247848058");

    const auto [quotient, remainder] = big_int1.divide(big_int2);
    EXPECT_EQ(expected_quotioent, quotient);
    EXPECT_EQ(expected_remainder, remainder);
}

TEST_F(BigIntDivOperator_tests, divHugeUnbalancedNumbers)
{
    const BigInt big_int1(
        "271264217862174521416723612641270004867812678214825175217563521735712538123618723651267831285719827271387218"
        "3");
    const BigInt big_int2("287362196785319493745798615619386573625");

    const BigInt expected_quotioent("9439801786622221017674810050846457664024907903528889626985672190493594");
    const BigInt expected_remainder("53244412639911237906247612787542013933");

    const auto [quotient, remainder] = big_int1.divide(big_int2);
    EXPECT_EQ(expected_quotioent, quotient);
    EXPECT_EQ(expected_remainder, remainder);
}

TEST_F(BigIntDivOperator_tests, divHugeUnbalancedNumbers_2)
{
    const BigInt big_int1("10000000000000000000000000000000000000000000000000000000000000000000000000000000000000001");
    const BigInt big_int2("100000000000010000000000000000000000000100");

    const BigInt expected_quotioent("99999999999990000000000000999999999999800000000");
    const BigInt expected_remainder("2999999999999900000000000020000000001");

    const auto [quotient, remainder] = big_int1.divide(big_int2);
    EXPECT_EQ(expected_quotioent, quotient);
    EXPECT_EQ(expected_remainder, remainder);
}

TEST_F(BigIntDivOperator_tests, divHugeUnbalancedNumbers_3)
{
    const BigInt big_int1(
        "10000000000000000000100000000000000000000000000000000000000000000010000000000000000000000000000000000000000000"
        "00000000000000000000000000000000000000000000000000000000000000000000001100000000000000000001000000000000000000"
        "00000000000000000000000000010000000000000000000000000000000000000000000000000000000000000000000000000000000000"
        "00000000000000000000000000000001");
    const BigInt big_int2(
        "10000000000001000000000000000001000000000000000000010000000000000000000000000000000000000000000001000000000000"
        "00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000100000000"
        "100");

    const BigInt expected_quotioent(
        "99999999999990000001000000999989899999900002009999809999699001039100039099694090025190060790891550990200941737"
        "89742169970792140965098393362");
    const BigInt expected_remainder(
        "42532679046770370171930941810283226695879400099872908449009800058262102478300291978600349015076378999010099100"
        "11009799010018799030118896120095986120587087511584918391838821890750727190163656239465689064109465672901606638"
        "01");

    const auto [quotient, remainder] = big_int1.divide(big_int2);
    EXPECT_EQ(expected_quotioent, quotient);
    EXPECT_EQ(expected_remainder, remainder);
}

TEST_F(BigIntDivOperator_tests, divHugeUnbalancedNumbers_4)
{
    const BigInt big_int1(
        "10384719875891347897198475000000008471987589134789719847500008471995465691347897198475000000084719875891347897"
        "19848913478971984750000000084719875891347898913478971984750000000084719875891347897198475000084719719847500008"
        "47198913478971984750000000084719875891347897891347897198475000000008471987589134789719847500008471919847500008"
        "47191557500000134141410000000000000000000000000000005135135535100000000000000000000000000110000000000000000000"
        "10000000000092108390280938219038901820391000001304819084091849083905372095739050239238943242342334253253525500"
        "00000000003924801904810941343525235235288546546454000000000000000000000000000000000000000000000000000000000000"
        "00000000000000000000000000000001");
    const BigInt big_int2(
        "100000000000010000000000198409128309128301283902821903812312000000000000000000001000000000000");

    const BigInt expected_quotioent(
        "10384719875890309425210865364825304286697794110135994208306137796678417169108848476948973806732918106727839247"
        "84400182653671634334428435282283762945532911167004536015090771366797553462778968356675151676416139837520968778"
        "17887871077715024863105309690839487989688804644247419160140128472358152868290122466326626769287006258377984458"
        "19525978937225266832360071997965473971753156274989698392748513717028448030775159822756180682746291700014992708"
        "27940651946125026257887888703984988573224541386606057881648516017611708634459399792043012257092042311562258232"
        "74786987490917094452260599583934610171171590701077");
    const BigInt expected_remainder(
        "26138604728014604595908096689384795581613643727072635140392065389828828409298923000000000001");

    const auto [quotient, remainder] = big_int1.divide(big_int2);
    EXPECT_EQ(expected_quotioent, quotient);
    EXPECT_EQ(expected_remainder, remainder);
}
