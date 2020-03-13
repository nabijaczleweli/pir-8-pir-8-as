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
#include <cstdio>
#include <string_view>


class mmap_view {
private:
	MMAP_VIEW_PLATFORM_DATA
	std::size_t file_size{};
	void * file_view{};

public:
	mmap_view(const char * fname, FILE * log);
	mmap_view(const mmap_view &) = delete;
	~mmap_view();

	const void * data() const noexcept;
	std::size_t size() const noexcept;

	operator bool() const noexcept;

	template <class T>
	operator std::basic_string_view<T>() const noexcept {
		return {static_cast<const T *>(data()), size()};
	}
};
