#include <benchmark/benchmark.h>
#include <yabil/bigint/BigInt.h>
#include <yabil/bigint/BigIntGlobalConfig.h>
#include <yabil/bigint/Parallel.h>

// Boost
#include <boost/multiprecision/cpp_int.hpp>

// GMP
#include <gmp.h>

// OpenSSL
#include <openssl/bn.h>

// CPython
#include <Python.h>

// FLINT
#include <fmpz.h>

// Utils
#include <thread>

#include "benchmark_utils.h"  // NOLINT

namespace
{

class Multiplication : public BaseBigIntBenchmark
{
};

BENCHMARK_DEFINE_F(Multiplication, YABIL)(benchmark::State& state)
{
    const int size = static_cast<int>(state.range(0));
    const auto [a_data, b_data] = generate_test_numbers(size);

    yabil::bigint::BigInt a;
    yabil::bigint::BigInt b;

    convertTo_(&a, a_data);
    convertTo_(&b, b_data);
    yabil::bigint::BigIntGlobalConfig::instance().set_parallel_algorithms_enabled(false);

    for (auto _ : state)
    {
        auto c = a * b;
        benchmark::DoNotOptimize(c);
        benchmark::ClobberMemory();
    }

    yabil::bigint::BigIntGlobalConfig::instance().set_parallel_algorithms_enabled(true);
}

BENCHMARK_DEFINE_F(Multiplication, YABIL_parallel_thread)(benchmark::State& state)
{
    const int size = static_cast<int>(state.range(0));
    const int thread_count = static_cast<int>(state.range(1));
    const auto [a_data, b_data] = generate_test_numbers(size);

    yabil::bigint::BigInt a;
    yabil::bigint::BigInt b;

    yabil::bigint::BigIntGlobalConfig::instance().set_thread_count(thread_count);

    convertTo_(&a, a_data);
    convertTo_(&b, b_data);

    for (auto _ : state)
    {
        auto c = yabil::bigint::parallel::multiply(a, b);
        benchmark::DoNotOptimize(c);
        benchmark::ClobberMemory();
    }

    yabil::bigint::BigIntGlobalConfig::instance().set_thread_count(11);
}

BENCHMARK_DEFINE_F(Multiplication, YABIL_parallel)(benchmark::State& state)
{
    const int size = static_cast<int>(state.range(0));
    const auto [a_data, b_data] = generate_test_numbers(size);

    yabil::bigint::BigInt a;
    yabil::bigint::BigInt b;

    convertTo_(&a, a_data);
    convertTo_(&b, b_data);

    for (auto _ : state)
    {
        auto c = yabil::bigint::parallel::multiply(a, b);
        benchmark::DoNotOptimize(c);
        benchmark::ClobberMemory();
    }
}

BENCHMARK_DEFINE_F(Multiplication, GMP)(benchmark::State& state)
{
    const int size = static_cast<int>(state.range(0));
    const auto [a_data, b_data] = generate_test_numbers(size);

    mpz_t a, b, c;
    convertTo_(a, a_data);
    convertTo_(b, b_data);

    for (auto _ : state)
    {
        mpz_init(c);
        mpz_mul(c, a, b);
        benchmark::DoNotOptimize(c);
        benchmark::ClobberMemory();
        mpz_clear(c);
    }
}

BENCHMARK_DEFINE_F(Multiplication, boost)(benchmark::State& state)
{
    const int size = static_cast<int>(state.range(0));
    const auto [a_data, b_data] = generate_test_numbers(size);

    boost::multiprecision::cpp_int a;
    boost::multiprecision::cpp_int b;
    convertTo_(&a, a_data);
    convertTo_(&b, b_data);

    boost::multiprecision::cpp_int c;

    for (auto _ : state)
    {
        c = a * b;
        benchmark::DoNotOptimize(c);
        benchmark::ClobberMemory();
    }

    static_cast<void>(c);
}

BENCHMARK_DEFINE_F(Multiplication, openssl)(benchmark::State& state)
{
    const int size = static_cast<int>(state.range(0));
    const auto [a_data, b_data] = generate_test_numbers(size);

    BN_CTX* ctx = BN_CTX_new();
    BIGNUM* a = BN_new();
    BIGNUM* b = BN_new();

    convertTo_(a, a_data);
    convertTo_(b, b_data);

    for (auto _ : state)
    {
        BIGNUM* c = BN_new();
        BN_mul(c, a, b, ctx);
        benchmark::DoNotOptimize(c);
        benchmark::ClobberMemory();
        BN_free(c);
    }

    BN_free(a);
    BN_free(b);
    BN_CTX_free(ctx);
}

BENCHMARK_DEFINE_F(Multiplication, python)(benchmark::State& state)
{
    const int size = static_cast<int>(state.range(0));
    const auto [a_data, b_data] = generate_test_numbers(size);

    Py_Initialize();
    PyObject* a;
    PyObject* b;

    convertTo_(&a, a_data);
    convertTo_(&b, b_data);
    PyObject* c = nullptr;

    for (auto _ : state)
    {
        c = PyNumber_Multiply(a, b);
        benchmark::DoNotOptimize(c);
        benchmark::ClobberMemory();
        Py_DECREF(c);
    }

    Py_DECREF(a);
    Py_DECREF(b);
    Py_Finalize();
}

BENCHMARK_DEFINE_F(Multiplication, FLINT)(benchmark::State& state)
{
    const int size = static_cast<int>(state.range(0));
    const auto [a_data, b_data] = generate_test_numbers(size);

    fmpz_t a, b;
    convertTo_(a, a_data);
    convertTo_(b, b_data);

    PyObject* c = nullptr;

    for (auto _ : state)
    {
        fmpz_t c;
        fmpz_init(c);
        fmpz_mul(c, a, b);
        benchmark::DoNotOptimize(c);
        benchmark::ClobberMemory();
        fmpz_clear(c);
    }
}

REGISTER_F(Multiplication, YABIL);
REGISTER_F(Multiplication, YABIL_parallel)->UseRealTime();
BENCHMARK_REGISTER_F(Multiplication, YABIL_parallel_thread)
    ->UseRealTime()
    ->ArgsProduct({benchmark::CreateDenseRange(64, BaseBigIntBenchmark::number_max_size_digits,
                                               BaseBigIntBenchmark::step_size),
                   {1, 2, 3, 5, 7, 9}});
REGISTER_F(Multiplication, GMP);
REGISTER_F(Multiplication, boost);
REGISTER_F(Multiplication, openssl);
REGISTER_F(Multiplication, python);
REGISTER_F(Multiplication, FLINT);  // flint_set_num_threads(n)

// ----------
// Perform multiplication for large inputs

constexpr int extended_range_start = 1520064;
constexpr int extended_range_stop = 4'000'000;
constexpr int extended_range_step = extended_range_stop / BaseBigIntBenchmark::number_of_probes;

BENCHMARK_REGISTER_F(Multiplication, YABIL)
    ->Name("Multiplication/YABIL_big")
    ->DenseRange(extended_range_start, extended_range_stop, extended_range_step);

BENCHMARK_REGISTER_F(Multiplication, YABIL_parallel_thread)
    ->Name("Multiplication/YABIL_parallel_thread_big")
    ->UseRealTime()
    ->ArgsProduct({benchmark::CreateDenseRange(extended_range_start, extended_range_stop, extended_range_step), {11}});

BENCHMARK_REGISTER_F(Multiplication, GMP)
    ->Name("Multiplication/GMP_big")
    ->DenseRange(extended_range_start, extended_range_stop, extended_range_step);

BENCHMARK_REGISTER_F(Multiplication, boost)
    ->Name("Multiplication/boost_big")
    ->DenseRange(extended_range_start, extended_range_stop, extended_range_step);

}  // namespace
