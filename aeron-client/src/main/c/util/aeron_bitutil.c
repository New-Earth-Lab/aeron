/*
 * Copyright 2014-2025 Real Logic Limited.
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

#if defined(__linux__)
#define _BSD_SOURCE
#define _GNU_SOURCE
#ifdef HAVE_BSDSTDLIB_H
#include <bsd/stdlib.h>
#endif
#endif

#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#if defined(__linux__) || defined(Darwin)
#include <unistd.h>
#endif

#include "util/aeron_bitutil.h"
#include "util/aeron_platform.h"

#if defined(AERON_COMPILER_MSVC)
#define _CRT_RAND_S
#endif

#ifndef HAVE_ARC4RANDOM
#if defined(__linux__) || defined(__CYGWIN__)
static int aeron_dev_random_fd = -1;
#endif
#endif

int32_t aeron_randomised_int32(void)
{
    int32_t result;

#ifdef HAVE_ARC4RANDOM
    uint32_t value = arc4random();

    memcpy(&result, &value, sizeof(int32_t));
#elif defined(__linux__) || defined(__CYGWIN__)
    if (-1 == aeron_dev_random_fd)
    {
        if ((aeron_dev_random_fd = open("/dev/urandom", O_RDONLY)) < 0)
        {
            fprintf(stderr, "could not open /dev/urandom (%d): %s\n", errno, strerror(errno));
            exit(EXIT_FAILURE);
        }
    }

    if (sizeof(result) != read(aeron_dev_random_fd, &result, sizeof(result)))
    {
        fprintf(stderr, "Failed to read from aeron_dev_random (%d): %s\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
#elif defined(_WIN32)
    uint32_t value;
    if (0 == rand_s(&value))
    {
        memcpy(&result, &value, sizeof(int32_t));
    }
    else
    {
        result = (int32_t)(rand() / (double)RAND_MAX * INT_MAX);
    }
#else
    result = (int32_t)(rand() / (double)RAND_MAX * INT_MAX);
#endif
    return result;
}

extern uint8_t *aeron_cache_line_align_buffer(uint8_t *buffer);
extern int aeron_number_of_trailing_zeroes(int32_t value);
extern int aeron_number_of_trailing_zeroes_u64(uint64_t value);
extern int aeron_number_of_leading_zeroes(int32_t value);
extern int32_t aeron_find_next_power_of_two(int32_t value);
