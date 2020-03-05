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
#include <ios>
#include <string>


using namespace std::literals;


static const char * const TEST_BUF = R"(
# comment
12 r twelfth.in
13 w thirteenth.out  # Hewwo!
2  w second.out
)";

static const io_config TEST_EXPECTED = {{12, {std::ios::in, "twelfth.in"s}},       //
                                        {13, {std::ios::out, "thirteenth.out"s}},  //
                                        {2, {std::ios::out, "second.out"s}}};


#include "correct.hpp"
