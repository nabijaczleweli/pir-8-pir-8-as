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


#include "io.hpp"
#include "mmap/mmap_view.hpp"
#include <algorithm>
#include <charconv>
#include <ostream>
#include <string_view>
#include <system_error>


#define WHITESPACE "\x20\x0c\x0a\x0d\x09\x0b"  // https://en.cppreference.com/w/c/string/byte/isspace
#define FILE_OFFSET(for) (for.data() - input_start)


static const char * from_chars_reason(std::errc err) {
	switch(err) {
		case std::errc::invalid_argument:
			return "not a number";
		case std::errc::result_out_of_range:
			return "out of range";
		default:
			__builtin_unreachable();
	}
}


io_config load_configured_io(std::string_view input, const char * input_name, std::ostream & log) {
	const auto input_start = input.data();
	io_config ret{};

	while(!input.empty()) {
		if(const auto off = input.find_first_not_of(WHITESPACE); off != std::string_view::npos)
			input.remove_prefix(off);
		else
			break;

		if(input[0] == '#') {
			if(const auto off = input.find('\n'); off != std::string_view::npos) {
				input.remove_prefix(off);
				continue;
			} else
				break;
		}

		std::uint8_t io_num{};
		auto parse_result = std::from_chars(input.data(), input.data() + input.size(), io_num);
		if(parse_result.ec != std::errc{}) {
			log << "Couldn't parse " << input_name << ':' << FILE_OFFSET(input) << ": " << from_chars_reason(parse_result.ec) << ".\nData left: " << input << '\n';
			return ret;
		}
		if(ret.find(io_num) != ret.end()) {
			log << input_name << ':' << FILE_OFFSET(input) << ": file #" << static_cast<unsigned int>(io_num) << " specified twice."
			    << "\nData left: " << input << '\n';
			return ret;
		}
		input.remove_prefix(parse_result.ptr - input.data());

		if(const auto off = input.find_first_not_of(WHITESPACE); off != std::string_view::npos)
			input.remove_prefix(off);
		else
			break;

		std::ios::openmode mode{};
		if(const auto off = input.find_first_of(WHITESPACE); off != std::string_view::npos) {
			std::string_view raw_mode(input.data(), off);
			if(raw_mode == "r")
				mode = std::ios::in;
			else if(raw_mode == "w")
				mode = std::ios::out;
			else {
				log << input_name << ':' << FILE_OFFSET(raw_mode) << ": invalid open-mode for file #" << static_cast<unsigned int>(io_num) << ": " << raw_mode << '\n';
				return ret;
			}

			input.remove_prefix(off);
		} else
			break;

		if(const auto off = input.find_first_not_of(WHITESPACE); off != std::string_view::npos)
			input.remove_prefix(off);
		else
			break;

		if(const auto off = input.find_first_of(WHITESPACE); off != std::string_view::npos) {
			std::string_view input_name(input.data(), off);
			ret.insert(std::make_pair(io_num, std::make_pair(mode, input_name)));

			input.remove_prefix(off);
		} else
			break;
	}

	return ret;
}
