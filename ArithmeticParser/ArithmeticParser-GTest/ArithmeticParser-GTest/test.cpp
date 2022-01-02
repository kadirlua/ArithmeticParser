#include "pch.h"
#include "../../ArithmeticParser.h"

using namespace Parser;

// parse and evulate the given expression
int EvulateExpression(std::string strExpression)
{
	ArithmeticParserInt p{ strExpression };
	return p.parseAndEvulate();
}

TEST(SimpleTestCase, ArithmeticParserTest) {
	ArithmeticParserInt parser1;
	parser1.setExpression("(4 + 5 * (7 - 3)) - 2");
	EXPECT_EQ(parser1.parseAndEvulate(), 22);
	EXPECT_EQ(ArithmeticParserInt{ "4+5+7/2" }.parseAndEvulate(), 12);
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