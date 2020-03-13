// The MIT License (MIT)

// Copyright (c) 2020 nabijaczleweli

// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.


#pragma once


#include <cstdio>
#include <fmt/format.h>
#undef assert


#define assert(expr, message)                                                                      \
	do                                                                                               \
		if(!(expr)) {                                                                                  \
			fmt::print(stderr, "{}:{}: assertion failed: {}: {}\n", __FILE__, __LINE__, #expr, message); \
			return __LINE__;                                                                             \
		}                                                                                              \
	while(false)

#define assert_eq(lhs, rhs, message) assert_eq_print(lhs, rhs, message, [](const auto & what) { return what; })

#define assert_eq_print(lhs, rhs, message, print_fn)                                                         \
	do {                                                                                                       \
		const auto assert_eq_lhs = (lhs);                                                                        \
		const auto assert_eq_rhs = (rhs);                                                                        \
		if(!(assert_eq_lhs == assert_eq_rhs)) {                                                                  \
			fmt::print(stderr,                                                                                     \
			           "{}:{}: assertion failed: {} == {}: {}\n"                                                   \
			           "  lhs: {}\n"                                                                               \
			           "  rhs: {}\n",                                                                              \
			           __FILE__, __LINE__, #lhs, #rhs, message, print_fn(assert_eq_lhs), print_fn(assert_eq_rhs)); \
			return __LINE__;                                                                                       \
		}                                                                                                        \
	} while(false)


#define test_ok() fmt::print("{}: OK\n", __FILE__);
