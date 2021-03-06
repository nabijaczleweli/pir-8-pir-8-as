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


#include <cstdint>
#include <cstdio>
#include <fmt/format.h>


enum class file_mode : std::uint8_t {
	read   = 0b001,
	write  = 0b010,
	append = 0b100,
};

file_mode operator&(file_mode lhs, file_mode rhs) noexcept;
file_mode operator|(file_mode lhs, file_mode rhs) noexcept;

namespace detail {
	struct file_mode_buf {
		char buf[4];
		file_mode_buf(file_mode mode) noexcept;
	};
}

template <>
struct fmt::formatter<file_mode> {
	constexpr auto parse(format_parse_context & ctx) {
		auto it = ctx.begin();
		if(it != ctx.end() && *it != '}')
			throw fmt::format_error("invalid format");
		return it;
	}

	template <typename FormatContext>
	auto format(file_mode mode, FormatContext & ctx) {
		const detail::file_mode_buf mode_buf{mode};
		return format_to(ctx.out(), "{}", mode_buf.buf);
	}
};


class file {
private:
	std::FILE * handle;

public:
	file(const char * fname, file_mode mode, bool buffer = true);
	file(const file &) = delete;
	file(file &&);
	~file();

	operator const std::FILE *() const noexcept;
	operator std::FILE *() noexcept;
};
