// MIT License

// Copyright (c) 2022-2023 kadirlua

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

#include "pch.h"
#include "../../ArithmeticParser.h"

using namespace Parser;

// parse and evulate the given expression
int EvulateExpression(std::string strExpression)
{
	ArithmeticParserInt p{ strExpression };
	return p.parseAndEvaluate();
}

TEST(SimpleTestCase, ArithmeticParserTest) {
	ArithmeticParserInt parser1;
	parser1.setExpression("(4 + 5 * (7 - 3)) - 2");
	EXPECT_EQ(parser1.parseAndEvaluate(), 22);
	EXPECT_EQ(ArithmeticParserInt{ "4+5+7/2" }.parseAndEvaluate(), 12);
	EXPECT_THROW(EvulateExpression("10 + 1"), ParserException);
	EXPECT_THROW(EvulateExpression("-10"), ParserException);
}

TEST(SimpleTestCase2, ArithmeticParserTest) {
	EXPECT_THROW(EvulateExpression("    "), ParserException);	//nothing to parse
	EXPECT_EQ(EvulateExpression("4 / 2 + 3 - 5 + 2"), 2);
	EXPECT_THROW(EvulateExpression("(4 - 1) - 7 / 0"), ParserException);	//cannot divide by zero
	EXPECT_THROW(EvulateExpression("4 / 2 + 30 - 5 + 2"), ParserException);	//literal is too long
	EXPECT_EQ(EvulateExpression("(((5 + 7)))"), 12);
	EXPECT_EQ(EvulateExpression("(4 / 2) * (4 * 2)"), 16);
	EXPECT_EQ(EvulateExpression("(( 5 / 1) * (4 - 3))"), 5);
	EXPECT_THROW(EvulateExpression("( 15 % 3 + ( 9 - 2 )"), ParserException);	// literal is too long or unbalanced parantheses
	EXPECT_THROW(EvulateExpression("(( 6 + 2 ) - 5 "), ParserException);	// unbalanced parantheses
	EXPECT_THROW(EvulateExpression("a + b - c * d"), ParserException);	// invalid token
	EXPECT_THROW(EvulateExpression("/9"), ParserException);	// missing operand

}