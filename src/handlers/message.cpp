// The MIT License (MIT)

// Copyright (c) 2019 Lounge<C++>

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


#include <pir-8-emu/port_handler.h>
#include "lib/message_box.hpp"
#include <string>


//! Read in bytes from a port until a byte is 0,
//! then show the bytes in a message box as string and clear it


unsigned char pir_8_emu_port_count() {
	return 1;
}

void * pir_8_emu_init(const unsigned char *, unsigned char) {
	return new std::string;
}

void pir_8_emu_uninit(void * state) {
	delete static_cast<std::string *>(state);
}

unsigned char pir_8_emu_handle_read(void *, unsigned char) {
	return 0;
}

void pir_8_emu_handle_write(void * state, unsigned char, unsigned char byte) {
	auto & val = *static_cast<std::string *>(state);

	if(byte == 0) {
		message_box(val.c_str(), val.c_str());
		val.clear();
	} else
		val.push_back(byte);
}
