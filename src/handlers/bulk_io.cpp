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


#include "lib/display.hpp"
#include "lib/io.hpp"
#include "lib/mmap/mmap_view.hpp"
#include <fstream>
#include <pir-8-emu/port_handler.h>


struct bulk_io_state {
	io_mapping io_map;
	std::ofstream log;
};

static std::ofstream init_log(const char * func) {
	std::ofstream log;
	log.rdbuf()->pubsetbuf(nullptr, 0);
	log.open("bulk_io.log", std::ios::app);
	log << '\n' << func << "()\n";
	return log;
}

static io_config read_io_config(const mmap_view & input_file, std::ostream & log) {
	if(!input_file) {
		log << "Couldn't map bulk.io.\n";
		return {};
	}
	log << "Mapped bulk.io (" << input_file.size() << "B)!\n\n";

	const auto config = load_configured_io(input_file, "bulk.io", log);
	log << config.size() << " files configured: ";
	for(auto && spec : config)
		log << spec.second.second << "; ";
	log << "\n";

	return config;
}


unsigned char pir_8_emu_port_count() {
	auto log = init_log(__func__);
	return read_io_config(mmap_view{"bulk.io", log}, log).size();
}

void * pir_8_emu_init(const unsigned char *, unsigned char) {
	auto log = init_log(__func__);

	auto map = [&] {
		const mmap_view input_file{"bulk.io", log};
		const auto config = read_io_config(input_file, log);

		auto map = open_configured_io(config, log);
		log << map.size() << " files opened – " << ((map.size() == config.size()) ? "all good" : "some are missing") << "!\n\n";
		return map;
	}();

	return new bulk_io_state{std::move(map), std::move(log)};
}

void pir_8_emu_uninit(void * state_v) {
	auto state = static_cast<bulk_io_state *>(state_v);
	state->log << "pir_8_emu_uninit()\n\n";

	delete state;
}

unsigned char pir_8_emu_handle_read(void * state_p, unsigned char port) {
	auto & state = *static_cast<bulk_io_state *>(state_p);
	state.log << "pir_8_emu_handle_read(" << byte_write{port} << "): ";

	if(auto stream = state.io_map.find(port); stream != state.io_map.end()) {
		char data{};
		stream->second.get(data);

		if(stream->second.eof())
			state.log << "EOF";
		else if(stream->second.fail())
			state.log << "failed";
		else
			state.log << "got " << maybe_printable_write{data};
		state.log << "\n\n";

		return data;
	} else {
		state.log << "not found\n\n";
		return 0x00;
	}
}

void pir_8_emu_handle_write(void * state_p, unsigned char port, unsigned char byte) {
	auto & state = *static_cast<bulk_io_state *>(state_p);
	state.log << "pir_8_emu_handle_write(" << byte_write{port} << ", " << maybe_printable_write{static_cast<char>(byte)} << "): ";

	if(auto stream = state.io_map.find(port); stream != state.io_map.end()) {
		stream->second.put(byte);

		if(stream->second.bad())
			state.log << "failed";
		else
			state.log << "ok";
		state.log << "\n\n";
	} else
		state.log << "not found\n\n";
}
