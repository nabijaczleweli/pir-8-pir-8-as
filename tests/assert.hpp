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


#include <iostream>


#define assert(expr, message)                                                                                 \
	do                                                                                                          \
		if(!(expr)) {                                                                                             \
			std::cerr << __FILE__ << ':' << __LINE__ << ": assertion failed: " << #expr << ": " << message << '\n'; \
			return __LINE__;                                                                                        \
		}                                                                                                         \
	while(false)

#define assert_eq(lhs, rhs, message)                                                                                                                  \
	do {                                                                                                                                                \
		const auto assert_eq_lhs = (lhs);                                                                                                                 \
		const auto assert_eq_rhs = (rhs);                                                                                                                 \
		if(!(assert_eq_lhs == assert_eq_rhs)) {                                                                                                           \
			std::cerr << __FILE__ << ':' << __LINE__ << ": assertion failed: " << #lhs << " == " << #rhs << ": " << message << "\n  lhs: " << assert_eq_lhs \
			          << "\n  rhs: " << assert_eq_rhs << '\n';                                                                                              \
			return __LINE__;                                                                                                                                \
		}                                                                                                                                                 \
	} while(false)

#define assert_eq_print(lhs, rhs, message, print)                                                                                     \
	do {                                                                                                                                \
		const auto assert_eq_lhs = (lhs);                                                                                                 \
		const auto assert_eq_rhs = (rhs);                                                                                                 \
		if(!(assert_eq_lhs == assert_eq_rhs)) {                                                                                           \
			std::cerr << __FILE__ << ':' << __LINE__ << ": assertion failed: " << #lhs << " == " << #rhs << ": " << message << "\n  lhs: "; \
			print(assert_eq_lhs, std::cerr);                                                                                                \
			std::cerr << "\n  rhs: ";                                                                                                       \
			print(assert_eq_rhs, std::cerr);                                                                                                \
			std::cerr << '\n';                                                                                                              \
			return __LINE__;                                                                                                                \
		}                                                                                                                                 \
	} while(false)


#define test_ok() std::cout << __FILE__ << ": OK\n";
