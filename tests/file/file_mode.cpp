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


#include "assert.hpp"
#include "lib/file.hpp"


// enum class file_mode : std::uint8_t {
	// read   = 0b001,
	// write  = 0b010,
	// append = 0b100,
// };
	// buf[0] = rw ? 'a' : mode_c;
	// buf[1] = 'b';
	// buf[2] = rw ? '+' : '\0';
	// buf[3] = '\0';

int main() {
	assert_eq(fmt::format("{}", file_mode::read), "rb", "Mismatch");
	assert_eq(fmt::format("{}", file_mode::write), "wb", "Mismatch");
	assert_eq(fmt::format("{}", file_mode::append), "ab", "Mismatch");
	assert_eq(fmt::format("{}", file_mode::read | file_mode::write), "ab+", "Mismatch");
	assert_eq(fmt::format("{}", file_mode::read | file_mode::append), "ab+", "Mismatch");
	assert_eq(fmt::format("{}", file_mode::read | file_mode::write | file_mode::append), "ab+", "Mismatch");

	test_ok();
}