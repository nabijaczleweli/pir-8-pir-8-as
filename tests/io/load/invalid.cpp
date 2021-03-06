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


#include "assert.hpp"
#include "lib/file.hpp"
#include "lib/io.hpp"
#include "lib/mmap/mmap_view.hpp"
#include <cstdio>


int main() {
	mmap_view readme("README.md", stderr);
	assert(readme, "Couldn't map file");

	assert_eq_print(load_configured_io(readme, "README.md", stderr), io_config{}, "Somehow not empty", [](const auto & what) {
		std::string ret;
		for(auto && kv : what)
			ret += fmt::format("{{{}, {{{}, {}}}}}; ", kv.first, kv.second.first, kv.second.second);
		return ret;
	});

	test_ok();
}
