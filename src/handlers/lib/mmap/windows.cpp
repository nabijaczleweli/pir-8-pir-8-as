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


#ifdef _WIN32


#include "mmap_view.hpp"
#include <windows.h>


mmap_view::mmap_view(const char * fname, std::ostream & log) {
	raw_file = CreateFileA(fname, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	if(raw_file == INVALID_HANDLE_VALUE) {
		log << "Couldn't open " << fname << ": 0x" << std::hex << GetLastError() << std::dec << '\n';
		return;
	}

	LARGE_INTEGER file_size_temp;
	if(!GetFileSizeEx(raw_file, &file_size_temp)) {
		log << "Couldn't get size of " << fname << ": 0x" << std::hex << GetLastError() << std::dec << '\n';
		return;
	}
	file_size = file_size_temp.QuadPart;

	if(file_size == 0) {
		log << "Size of " << fname << " is 0, not mapping further\n";
		return;
	}

	file_mapping = CreateFileMappingA(raw_file, nullptr, PAGE_READONLY, 0, 0, nullptr);
	if(file_mapping == nullptr) {
		log << "Couldn't create mapping for " << fname << ": 0x" << std::hex << GetLastError() << std::dec << '\n';
		return;
	}

	file_view = MapViewOfFile(file_mapping, FILE_MAP_READ, 0, 0, 0);
	if(file_view == nullptr) {
		log << "Couldn't map view of " << fname << ": 0x" << std::hex << GetLastError() << std::dec << '\n';
		return;
	}
}

mmap_view::~mmap_view() {
	if(file_view != nullptr) {
		UnmapViewOfFile(file_view);
		file_view = nullptr;
	}

	if(file_mapping != nullptr) {
		CloseHandle(file_mapping);
		file_mapping = nullptr;
	}

	if(raw_file != INVALID_HANDLE_VALUE) {
		CloseHandle(raw_file);
		raw_file = INVALID_HANDLE_VALUE;
	}
}


#endif
