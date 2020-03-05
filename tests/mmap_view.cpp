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


#include "mmap/mmap_view.hpp"
#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>


int main() {
	mmap_view make_mapped("Makefile", std::cerr);
	assert(((void)"Couldn't map file", make_mapped));

	std::string make_read;
	assert(((void)"Couldn't read file", std::getline(std::ifstream("Makefile"), make_read, '\0')));

	assert(make_mapped.size() == make_read.size());
	assert(std::string_view(static_cast<const char *>(make_mapped.data()), make_mapped.size()) == make_read);

	std::cout << __FILE__ << ": OK\n";
}
