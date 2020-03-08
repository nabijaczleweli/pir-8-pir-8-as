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


#include "lib/io.hpp"
#include "lib/mmap/mmap_view.hpp"
#include <fstream>
#include <pir-8-emu/port_handler.h>


struct bulk_io_state {
	io_config io_cfg;
	std::ofstream log;
};


unsigned char pir_8_emu_port_count() {
	return 1;
}

void * pir_8_emu_init(const unsigned char *, unsigned char) {
	std::ofstream log("bulk_io.log", std::ios::app);
	log.rdbuf()->pubsetbuf(nullptr, 0);
	log << '\n';

	mmap_view input_file{"bulk.io", log};
	if(!input_file) {
		log << "Couldn't map bulk.io.\n";
		return {};
	}
	log << "Mapped bulk.io (" << input_file.size() << "B)!\n";

	return new bulk_io_state{load_configured_io(input_file, "bulk.io", log), std::move(log)};
}

void pir_8_emu_uninit(void * state) {
	delete static_cast<bulk_io_state *>(state);
}

unsigned char pir_8_emu_handle_read(void *, unsigned char) {
	return 0;
}

void pir_8_emu_handle_write(void *, unsigned char, unsigned char) {}
