// MIT License

// Copyright (c) 2022-2025 kadirlua

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

// main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <chrono>

#include "ArithmeticParser.h"

#define BENCHMARK_TEST  1

namespace {
	constexpr auto const MAX_ITER = 100000;
}

#define TEST_PARSER(str)                                                        \
try {                                                                           \
     std::cout << Parser::ArithmeticParserInt{str}.parseAndEvaluate() << "\n"; \
}                                                                               \
catch (const Parser::ParserException& ex) {                                     \
     std::cout << "Exception thrown!: " << ex.getErrorMsg() << "\n";            \
}                                                                               \

int main()
{
    TEST_PARSER("(4 + 5 * (7 - 3)) - 2");
    TEST_PARSER("4+5+7/2");
    TEST_PARSER("10 + 1");
    TEST_PARSER("-10");
    TEST_PARSER("((((7 (* 4)))")
    TEST_PARSER("(5 + 2) + (5 - 2");
    TEST_PARSER("-1");
    TEST_PARSER("))) 5 + 2 (((");
    TEST_PARSER("+5");
    TEST_PARSER("*2");
    TEST_PARSER("5 /2 + 4 / 0");
	TEST_PARSER("     ");   //nothing to parse

#if BENCHMARK_TEST

    auto sysClockNow = std::chrono::steady_clock::now();
    
    for (int i = 0; i < MAX_ITER; i++) {
        try {
            Parser::ArithmeticParserInt parser{ "5 + 4 * 3 / 2" };
            (void)parser.parseAndEvaluate();
        }
        catch (...) {
            // do not print anything, just a dummy test :)
        }
    }

    auto sysClockEnd = std::chrono::steady_clock::now();

    /* Getting number of milliseconds as a double. */
    const std::chrono::duration<double, std::milli> ms_double = sysClockEnd - sysClockNow;

    std::cout << "Benchmark test result: " << ms_double.count() << " ms\n";
#endif

    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
