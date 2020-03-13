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


#include "lib/display/display.hpp"
#include "lib/file.hpp"
#include "lib/io.hpp"
#include "lib/mmap/mmap_view.hpp"
#include <fmt/format.h>
#include <pir-8-emu/port_handler.h>


struct bulk_io_state {
	io_mapping io_map;
	file log;
};

static file init_log(const char * func) {
	file log{"bulk_io.log", file_mode::append, false};
	fmt::print(log,
	           "\n"
	           "{}()\n",
	           func);
	return log;
}

static io_config read_io_config(const mmap_view & input_file, std::FILE * log) {
	if(!input_file) {
		fmt::print(log, "Couldn't map bulk.io.\n");
		return {};
	}
	fmt::print(log, "Mapped bulk.io ({}B)!\n\n", input_file.size());

	const auto config = load_configured_io(input_file, "bulk.io", log);
	fmt::print(log, "{} files configured: ", input_file.size());
	for(auto && spec : config)
		fmt::print(log, "{}; ", spec.second.second);
	fmt::print(log, "\n");

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
		fmt::print(log, "{} files opened – {}!\n\n", map.size(), (map.size() == config.size()) ? "all good" : "some are missing");
		return map;
	}();

	return new bulk_io_state{std::move(map), std::move(log)};
}

void pir_8_emu_uninit(void * state_v) {
	auto state = static_cast<bulk_io_state *>(state_v);
	fmt::print(state->log, "pir_8_emu_uninit()\n\n");

	delete state;
}

unsigned char pir_8_emu_handle_read(void * state_p, unsigned char port) {
	auto & state = *static_cast<bulk_io_state *>(state_p);
	fmt::print(state.log, "pir_8_emu_handle_read({:#02x}): ", port);

	if(auto stream = state.io_map.find(port); stream != state.io_map.end()) {
		const auto raw_data = fgetc(stream->second);
		const char data = raw_data == EOF ? 0x00 : raw_data;

		if(std::feof(stream->second))
			fmt::print(state.log, "EOF");
		if(std::ferror(stream->second))
			fmt::print(state.log, "error");
		else
			fmt::print(state.log, "got {}", maybe_printable_byte{data});
		fmt::print(state.log, "\n\n");

		return data;
	} else {
		fmt::print(state.log, "not found\n\n");
		return 0x00;
	}
}

void pir_8_emu_handle_write(void * state_p, unsigned char port, unsigned char byte) {
	auto & state = *static_cast<bulk_io_state *>(state_p);
	fmt::print(state.log, "pir_8_emu_handle_write({:#02x}, {}): ", port, maybe_printable_byte{static_cast<char>(byte)});

	if(auto stream = state.io_map.find(port); stream != state.io_map.end()) {
		std::fputc(byte, stream->second);

		if(std::ferror(stream->second))
			fmt::print(state.log, "error");
		else
			fmt::print(state.log, "ok");
		fmt::print(state.log, "\n\n");
	} else
		fmt::print(state.log, "not found\n\n");
}
