// Copyright (c) 2012, Steinwurf ApS
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of Steinwurf ApS nor the
//       names of its contributors may be used to endorse or promote products
//       derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL Steinwurf ApS BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/random/bernoulli_distribution.hpp>
#include <boost/random/binomial_distribution.hpp>

#include <cstdlib>
#include <limits>
#include <vector>

#include <gtest/gtest.h>

using namespace boost::random;

TEST(TestBoostRandom, mt19937_uniform_distributions)
{
    mt19937 random_generator;

    uint32_t count = 100;

    uniform_int_distribution<uint8_t> uint8_distribution;

    for (uint32_t i = 0; i < count; ++i)
    {
        auto value = uint8_distribution(random_generator);
        EXPECT_GE(value, (uint8_t)0);
        EXPECT_LE(value, std::numeric_limits<uint8_t>::max());
    }

    uniform_int_distribution<uint16_t> uint16_distribution;

    for (uint32_t i = 0; i < count; ++i)
    {
        auto value = uint16_distribution(random_generator);
        EXPECT_GE(value, (uint16_t)0);
        EXPECT_LE(value, std::numeric_limits<uint16_t>::max());
    }

    uniform_int_distribution<uint32_t> uint32_distribution;

    for (uint32_t i = 0; i < count; ++i)
    {
        auto value = uint32_distribution(random_generator);
        EXPECT_GE(value, 0U);
        EXPECT_LE(value, std::numeric_limits<uint32_t>::max());
    }
}

TEST(TestBoostRandom, mt19937_bernoulli_distribution)
{
    mt19937 random_generator;

    uint32_t count = 100;

    bernoulli_distribution<> bernoulli(0.5);

    uint32_t heads = 0;
    for (uint32_t i = 0; i < count; ++i)
    {
        if (bernoulli(random_generator)) heads++;
    }
    EXPECT_GE(heads, 0U);
    EXPECT_LE(heads, count);
}

namespace
{
    void test_binomial(uint32_t seed, uint32_t n, uint32_t count,
                       std::vector<uint32_t> expected_results)
    {
        mt19937 random_generator;
        random_generator.seed(seed);

        binomial_distribution<> binomial(n, 0.5);

        uint32_t heads = 0;
        std::vector<uint32_t> results;
        for (uint32_t i = 0; i < count; ++i)
        {
            heads = binomial(random_generator);
            results.push_back(heads);
            EXPECT_GE(heads, 0U);
            EXPECT_LE(heads, n);
        }
        EXPECT_EQ(expected_results, results);
    }
}

TEST(TestBoostRandom, mt19937_binomial_distribution_10)
{
    // binomial_distribution uses the "inversion" algorithm if (n+1) * p < 11
    std::vector<uint32_t> expected_results =
        { 4, 5, 3, 4, 4, 5, 5, 2, 4, 4, 5, 5, 4, 4, 8, 4, 6, 4, 3, 6 };
    test_binomial(1337, 10, 20, expected_results);
}

TEST(TestBoostRandom, mt19937_binomial_distribution_100)
{
    // binomial_distribution uses the "BTRD" algorithm if (n+1) * p >= 11
    std::vector<uint32_t> expected_results =
        { 50, 49, 50, 39, 43, 51, 49, 58, 64, 52,
          47, 46, 48, 46, 48, 46, 47, 55, 41, 52 };
    test_binomial(1337, 100, 20, expected_results);
}
