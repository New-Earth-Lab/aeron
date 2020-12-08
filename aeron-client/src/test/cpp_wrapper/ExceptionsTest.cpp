
/*
 * Copyright 2014-2020 Real Logic Limited.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * https://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <functional>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "util/Exceptions.h"
extern "C"
{
#include "util/aeron_error.h"
}

using namespace aeron::util;
using testing::MockFunction;
using testing::_;

class ExceptionsTest : public testing::Test
{
public:
    ExceptionsTest()
    {
    }

    ~ExceptionsTest() override
    {
    }
};

TEST_F(ExceptionsTest, shouldThrowAppropriateType)
{
    aeron_set_err(EINVAL, "Invalid argument");
    ASSERT_THROW(
        {
            AERON_MAP_ERRNO_TO_SOURCED_EXCEPTION_AND_THROW;
        },
        IllegalArgumentException);

    aeron_set_err(EPERM, "Invalid argument");
    ASSERT_THROW(
        {
            AERON_MAP_ERRNO_TO_SOURCED_EXCEPTION_AND_THROW;
        },
        IllegalStateException);
}
