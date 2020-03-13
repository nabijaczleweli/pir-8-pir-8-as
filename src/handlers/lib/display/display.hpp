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


#include "posix.hpp"
#include "windows.hpp"
#include <cctype>
#include <cstdint>
#include <fmt/format.h>


struct maybe_printable_byte {
	char data;
};

template <>
struct fmt::formatter<maybe_printable_byte> {
	constexpr auto parse(format_parse_context & ctx) {
		auto it = ctx.begin();
		if(it != ctx.end() && *it != '}')
			throw fmt::format_error("invalid format");
		return it;
	}

	template <typename FormatContext>
	auto format(maybe_printable_byte self, FormatContext & ctx) {
		if(std::isprint(self.data))
			return format_to(ctx.out(), "'{}'", self.data);
		else
			return format_to(ctx.out(), "{:#02x}", static_cast<std::uint8_t>(self.data));
	}
};


struct error_write {
	SYSTEM_ERROR_TYPE error;
};

namespace detail {
	struct error_write_data {
		SYSTEM_ERROR_DATA_FIELDS
		error_write_data(SYSTEM_ERROR_TYPE error);
	};
}

template <>
struct fmt::formatter<error_write> {
	constexpr auto parse(format_parse_context & ctx) {
		auto it = ctx.begin();
		if(it != ctx.end() && *it != '}')
			throw fmt::format_error("invalid format");
		return it;
	}

	template <typename FormatContext>
	auto format(error_write error, FormatContext & ctx) {
		detail::error_write_data data{error.error};
		return format_to(ctx.out(), "{}", data.buf);
	}
};
