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


#include "file.hpp"


#define NOMODE static_cast<file_mode>(0)


file_mode operator&(file_mode lhs, file_mode rhs) noexcept {
	return static_cast<file_mode>(static_cast<std::uint8_t>(lhs) & static_cast<std::uint8_t>(rhs));
}

file_mode operator|(file_mode lhs, file_mode rhs) noexcept {
	return static_cast<file_mode>(static_cast<std::uint8_t>(lhs) | static_cast<std::uint8_t>(rhs));
}


file::file(const char * fname, file_mode mode, bool buffer) : handle{nullptr} {
	if(mode == NOMODE)
		return;

	bool rw = ((mode & file_mode::read) != NOMODE) && ((mode & (file_mode::write | file_mode::append)) != NOMODE);

	char mode_c{};
	if((mode & file_mode::read) != NOMODE)
		mode_c = 'r';

	if((mode & file_mode::append) != NOMODE)
		mode_c = 'a';
	else if((mode & file_mode::write) != NOMODE)
		mode_c = 'w';

	char mode_buf[4]{};
	mode_buf[0] = rw ? 'a' : mode_c;
	mode_buf[1] = 'b';
	mode_buf[2] = rw ? '+' : '\0';
	mode_buf[3] = '\0';  // Write this here explicitly as well to enable compiler warning
	// std::cout << mode_buf << '\n';

	handle = std::fopen(fname, mode_buf);
	if(!handle)
		return;

	if(!buffer)
		std::setbuf(handle, nullptr);
}

file::~file() {
	if(handle) {
		fclose(handle);
		handle = nullptr;
	}
}


file::operator const std::FILE *() const noexcept {
	return handle;
}

file::operator std::FILE *() noexcept {
	return handle;
}
