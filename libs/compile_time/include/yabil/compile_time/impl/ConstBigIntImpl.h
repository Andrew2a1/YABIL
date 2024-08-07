#pragma once

#include <yabil/bigint/BigInt.h>
#include <yabil/compile_time/Math.h>
#include <yabil/compile_time/detail/BigIntData.h>
#include <yabil/compile_time/detail/ConstBigInt.h>
#include <yabil/compile_time/detail/MakeConstBigInt.h>
#include <yabil/compile_time/impl/Utils.h>

#include <array>
#include <bit>
#include <concepts>
#include <cstddef>
#include <type_traits>

namespace yabil::compile_time
{

namespace impl
{

constexpr char get_digit_char(int digit)
{
    if (digit >= 10 && digit <= 16) return static_cast<char>(digit + 'a' - 10);
    return static_cast<char>(digit + '0');
}

template <std::size_t Base, std::size_t InternalSize, BigIntData<InternalSize> InternalData,
          std::size_t StrSizeEstimate, int idx>
constexpr auto digits_to_string(std::array<char, StrSizeEstimate> &output_string)
{
    constexpr auto number = make_bigint<InternalSize, InternalData>();
    if constexpr (number.is_zero())
    {
        return idx;
    }
    else
    {
        constexpr auto quotient = number / bigint_v<Base>;
        constexpr auto remainder = number % bigint_v<Base>;
        output_string[idx] = get_digit_char(remainder.template to<int>());
        return digits_to_string<Base, quotient.data.size(), quotient.data, StrSizeEstimate, idx + 1>(output_string);
    }
}

template <std::size_t Base, Sign NumberSign, std::size_t InternalSize, BigIntData<InternalSize> InternalData>
constexpr auto to_oversized_reversed_string()
{
    // Estimate can be much larger than actual size - do not care
    constexpr auto str_size_estimate = bigint_base_t_size_bits * InternalSize / math::log2_int(bigint_v<Base>) +
                                       3;  // 3 for minus sign, null character and possible rounding

    std::array<char, str_size_estimate> number_characters{};

    constexpr int start_index = 1;  // First char is NULL - will be transformed to end of number string later
    int i = digits_to_string<Base, InternalSize, InternalData, str_size_estimate, start_index>(number_characters);

    if constexpr (NumberSign == Sign::Minus)
    {
        number_characters[i] = '-';
    }

    return number_characters;
}

template <std::size_t Base, Sign NumberSign, std::size_t InternalSize, BigIntData<InternalSize> InternalData>
constexpr auto to_string()
{
    constexpr auto oversized_str = to_oversized_reversed_string<Base, NumberSign, InternalSize, InternalData>();
    auto trimmed_str = impl::normalize<char, oversized_str.size(), oversized_str>();
    std::reverse(trimmed_str.begin(), trimmed_str.end());
    return trimmed_str;
}

}  // namespace impl

template <Sign NumberSign, std::size_t InternalSize, BigIntData<InternalSize> InternalData>
consteval std::size_t ConstBigInt<NumberSign, InternalSize, InternalData>::real_size()
{
    return impl::normalized_size<InternalSize, InternalData>();
}

template <Sign NumberSign, std::size_t InternalSize, BigIntData<InternalSize> InternalData>
consteval std::size_t ConstBigInt<NumberSign, InternalSize, InternalData>::byte_size()
{
    return real_size() * sizeof(bigint_base_t);
}

template <Sign NumberSign, std::size_t InternalSize, BigIntData<InternalSize> InternalData>
template <std::size_t n>
consteval bool ConstBigInt<NumberSign, InternalSize, InternalData>::get_bit()
{
    constexpr auto item_index = n / bigint_base_t_size_bits;
    constexpr auto bit_index = n % bigint_base_t_size_bits;

    if constexpr (item_index >= data.size())
    {
        return 0;
    }
    else
    {
        return (data[item_index] >> bit_index) & 0x01;
    }
}

template <Sign NumberSign, std::size_t InternalSize, BigIntData<InternalSize> InternalData>
consteval bool ConstBigInt<NumberSign, InternalSize, InternalData>::is_zero()
{
    return std::all_of(InternalData.cbegin(), InternalData.cend(), [](const auto &digit) { return digit == 0; });
}

template <Sign NumberSign, std::size_t InternalSize, BigIntData<InternalSize> InternalData>
consteval bool ConstBigInt<NumberSign, InternalSize, InternalData>::is_negative()
{
    return !is_zero() && (sign == Sign::Minus);
}

template <Sign NumberSign, std::size_t InternalSize, BigIntData<InternalSize> InternalData>
template <std::signed_integral OutType>
consteval OutType ConstBigInt<NumberSign, InternalSize, InternalData>::to()
{
    const OutType result = static_cast<OutType>(to<std::make_unsigned_t<OutType>>());
    return is_negative() ? -result : result;
}

template <Sign NumberSign, std::size_t InternalSize, BigIntData<InternalSize> InternalData>
template <std::unsigned_integral OutType>
consteval OutType ConstBigInt<NumberSign, InternalSize, InternalData>::to()
{
    if constexpr (sizeof(OutType) < sizeof(bigint_base_t))
    {
        return static_cast<OutType>(InternalData[0]);
    }
    else
    {
        OutType result = 0;
        for (std::size_t i = 0; (i < InternalData.size()) && (i < sizeof(OutType) / sizeof(bigint_base_t)); ++i)
        {
            result |= static_cast<OutType>(InternalData[i]) << (i * bigint_base_t_size_bits);
        }
        return result;
    }
}

template <Sign NumberSign, std::size_t InternalSize, BigIntData<InternalSize> InternalData>
template <std::signed_integral OutType>
consteval OutType ConstBigInt<NumberSign, InternalSize, InternalData>::is()
{
    return is<std::make_unsigned_t<OutType>>() && !get_bit<sizeof(OutType) * 8 - 1>();
}

template <Sign NumberSign, std::size_t InternalSize, BigIntData<InternalSize> InternalData>
template <std::unsigned_integral OutType>
consteval OutType ConstBigInt<NumberSign, InternalSize, InternalData>::is()
{
    constexpr auto leading_zeroes = std::countl_zero(impl::get_digit(data.size() - 1, data));
    return static_cast<int>(byte_size() * 8) - leading_zeroes <= static_cast<int>(sizeof(OutType) * 8);
}

template <Sign NumberSign, std::size_t InternalSize, BigIntData<InternalSize> InternalData>
bigint::BigInt ConstBigInt<NumberSign, InternalSize, InternalData>::to_bigint()
{
    return bigint::BigInt(data, sign);
}

template <Sign NumberSign, std::size_t InternalSize, BigIntData<InternalSize> InternalData>
ConstBigInt<NumberSign, InternalSize, InternalData>::operator bigint::BigInt() const
{
    return to_bigint();
}

template <Sign NumberSign, std::size_t InternalSize, BigIntData<InternalSize> InternalData>
template <std::size_t Base>
consteval auto ConstBigInt<NumberSign, InternalSize, InternalData>::to_str()
{
    if constexpr (is_zero())
    {
        return std::array<char, 2>{'0', '\0'};
    }
    else
    {
        return impl::to_string<Base, NumberSign, InternalSize, InternalData>();
    }
}

}  // namespace yabil::compile_time
