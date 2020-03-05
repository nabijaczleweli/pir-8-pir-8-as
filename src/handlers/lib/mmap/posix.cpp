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


#ifndef _WIN32


#include "mmap_view.hpp"
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>


mmap_view::mmap_view(const char * fname, std::ostream & log) {
	raw_file = open(fname, O_RDONLY);
	if(raw_file == -1) {
		log << "Couldn't open " << fname << ": " << errno << '\n';
		return;
	}

	struct stat file_info;
	if(fstat(raw_file, &file_info)) {
		log << "Couldn't get size of " << fname << ": " << errno << '\n';
		return;
	}
	file_size = file_info.st_size;

	if(file_size == 0) {
		log << "Size of " << fname << " is 0, not mapping further\n";
		return;
	}

	file_view = mmap(nullptr, file_size, PROT_READ, MAP_PRIVATE, raw_file, 0);
	if(file_view == MAP_FAILED) {
		file_view = nullptr;
		log << "Couldn't map " << fname << ": " << errno << '\n';
		return;
	}
}

mmap_view::~mmap_view() {
	if(file_view) {
		munmap(file_view, file_size);
		file_view = nullptr;
	}

	if(raw_file != -1) {
		close(raw_file);
		raw_file = -1;
	}
}


#endif
