// MIT License

// Copyright (c) 2022-2026 kadirlua

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

//  Author: Kadir Altindag
//  Simple math parser in modern C++.
//  This library parse and evaluate given math expressions such as (a + b - c * d).
//  It's a cross-platform library which should compile on any operating system without any problem
//  (I have tested on Windows and Linux)
//  If you have any opinion or question, please do not hesitate to ask me :)

#ifndef ARITHMETIC_PARSER
#define ARITHMETIC_PARSER

#include <string>
#include <stack>
#include <algorithm>

#if __cplusplus >= 201703L
#define NODISCARD   [[nodiscard]]
#define INLINE      inline
#else
#define NODISCARD
#define INLINE
#endif

namespace Parser
{
    class ParserException
    {
    public:
        ParserException() noexcept = default;
        ParserException(const std::string& err_msg) noexcept : 
            m_error_msg{ err_msg } 
        {
        }

        ParserException(std::string&& err_msg) noexcept :
            m_error_msg{ std::move(err_msg) } 
        {
        }

        virtual ~ParserException() = default;

        /*
        *	Gets an error message for detecting which error occurs.
        *	returns: Error message.
        *	exception: This function never throws an exception.
        */
        NODISCARD virtual const std::string& getErrorMsg() const noexcept
		{
            return m_error_msg;
        }

    protected:
        std::string m_error_msg;
    };

	template<typename T>
	class ArithmeticParser
	{
        INLINE static constexpr auto const BRACE_LEFT = '(';
        INLINE static constexpr auto const BRACE_RIGHT = ')';
        INLINE static constexpr auto const OP_INC = '+';
        INLINE static constexpr auto const OP_MIN = '-';
        INLINE static constexpr auto const OP_MUL = '*';
        INLINE static constexpr auto const OP_DIV = '/';

	public:
        explicit ArithmeticParser() noexcept = default;	// default constructor
		explicit ArithmeticParser(std::string) noexcept;
		virtual ~ArithmeticParser() = default;	// destructor
		// non-copyable class
		ArithmeticParser(const ArithmeticParser&) = delete;
		ArithmeticParser& operator=(const ArithmeticParser&) = delete;

        // parse the given expression and evaluate the result.
        // this function throws an exception if an error occurs.
        NODISCARD T parseAndEvaluate();

        // setter and getter member functions
		void setExpression(std::string) noexcept;

        NODISCARD std::string getExpression() const noexcept
		{
			return m_strEpxr;
		}
	protected:
        // operator priorities
        int operatorPriority(char) noexcept;
        // to evaluate result from operands with an operator
        T callOperator(const T&, const T&, char);
        //Check an operator is valid or not
		NODISCARD bool isValidOperator(char op) const noexcept;
	private:
		std::stack<char> m_Ops;	// store operators into stack
		std::stack<T> m_Values;	// store values into stack
		std::string m_strEpxr;	// string expression for parsing
	};

    template<typename T>
    ArithmeticParser<T>::ArithmeticParser(std::string strExpr) noexcept :
        m_strEpxr{ std::move(strExpr) }
    {
    }

    template<typename T>
    T ArithmeticParser<T>::parseAndEvaluate()
    {
        // at first, we should trim the expression as well.
        m_strEpxr.erase(std::remove_if(m_strEpxr.begin(), m_strEpxr.end(),
            [](unsigned char ch) {
                return std::isspace(ch); }
        ), m_strEpxr.end());

        // check if the expression is empty
        if (m_strEpxr.empty()) {
			throw ParserException{ "Nothing to do parse!" };
		}

        // iterate each character into for loop
        for (auto iter = m_strEpxr.cbegin(); iter != m_strEpxr.cend(); iter++) 
        {
            const auto ch = *iter;

            switch (ch)
            {
            case BRACE_LEFT:
                // Current token is an opening
                // brace, push it to 'ops'
                m_Ops.push(ch);
                break;
            case BRACE_RIGHT:
                // Closing brace encountered, solve
                // entire brace.
                while (!m_Ops.empty() && m_Ops.top() != BRACE_LEFT) {
                    T&& val2 = std::move(m_Values.top());
                    m_Values.pop();

                    T&& val1 = std::move(m_Values.top());
                    m_Values.pop();

                    const auto op = m_Ops.top();
                    m_Values.push(callOperator(val1, val2, op));
                    m_Ops.pop();
                }

                // if we find a right parenthesis when the stack is empty
                if (m_Ops.empty()) {
					throw ParserException{ "unbalanced parentheses!" };
				}
				m_Ops.pop();    // pop opening brace.
                break;

            default:
                /*  
                * From cppreference.com:
                * isdigit and isxdigit are the only standard narrow character classification functions 
                * that are not affected by the currently installed C locale. although some implementations 
                * (e.g. Microsoft in 1252 codepage) may classify additional single-byte characters as digits.
                * Like all other functions from <cctype>, the behavior of std::isdigit is undefined 
                * if the argument's value is neither representable as unsigned char nor equal to EOF. 
                * To use these functions safely with plain chars (or signed chars), 
                * the argument should first be converted to unsigned char:
                */

                // Current token is a number, push
                // it to stack for numbers.
                if (std::isdigit(static_cast<unsigned char>(ch)) != 0) {
                    // get the current value of digit number
                    T val = ch - '0';

                    // If there may be more digit, throw an exception
                    for (auto next_iter = iter + 1; next_iter != m_strEpxr.cend() &&
                        std::isdigit(static_cast<unsigned char>(*next_iter)) != 0; next_iter++) {
                        throw ParserException("Literal is too large!");
                    }

                    m_Values.push(val);
                }
                // Current token is an operator.
                else
                {
                    // While top of 'ops' has same or greater
                    // precedence to current token, which
                    // is an operator. Apply operator on top
                    // of 'ops' to top two elements in values stack.
                    while (!m_Ops.empty() && operatorPriority(m_Ops.top())
                        >= operatorPriority(ch)) {

                        const auto op = m_Ops.top();

                        T&& val2 = std::move(m_Values.top());
                        m_Values.pop();

                        if (m_Values.empty()) {
                            switch (op) {
                            case OP_INC:
                                m_Values.push(val2);
                                break;
                            case OP_MIN:
                                throw ParserException{ "negative literal or unary minus" };
                                break;
                            case OP_MUL:
                            case OP_DIV:
                                throw ParserException{ "missing operand" };
                                break;
                            }
                        }
                        else {
                            T&& val1 = std::move(m_Values.top());
                            m_Values.pop();

                            m_Values.push(callOperator(val1, val2, op));
                        }

                        m_Ops.pop();
                    }

                    // Push current token to 'ops'.
                    if (isValidOperator(ch)) {
						m_Ops.push(ch);
					}
                    else {
						throw ParserException{ "Invalid token." };
					}
                }
                break;
            }
        }

        // Entire expression has been parsed at this
        // point, apply remaining ops to remaining
        // values.
        while (!m_Ops.empty()) {
            
            // get the operator and check it
            const auto op = m_Ops.top();

            // if there is still a left parenthesis at the top of the stack
            if (op == BRACE_LEFT) {
				throw ParserException{"unbalanced parentheses!"};
			}

            T&& val2 = std::move(m_Values.top());
            m_Values.pop();

            if (m_Values.empty()) {
                switch (op) {
                case OP_INC:
                    m_Values.push(val2);
                    break;
                case OP_MIN:
                    throw ParserException{ "negative literal or unary minus" };
                    break;
                case OP_MUL:
                case OP_DIV:
                    throw ParserException{ "missing operand" };
                    break;
                }
            }
            else {
                T&& val1 = std::move(m_Values.top());
                m_Values.pop();

                m_Values.push(callOperator(val1, val2, op));
            }

            m_Ops.pop();
        }

        // Top of 'values' contains result, return it.
        return m_Values.top();
    }

    template<typename T>
    void ArithmeticParser<T>::setExpression(std::string strExpr) noexcept
    {
        m_strEpxr = std::move(strExpr);
    }

    template<typename T>
    int ArithmeticParser<T>::operatorPriority(const char op) noexcept
    {
        switch (op) {
        case OP_INC:
        case OP_MIN:
            return 1;
        case OP_MUL:
        case OP_DIV:
            return 2;
        }

        return 0;
    }

    template<typename T>
    T ArithmeticParser<T>::callOperator(const T& val1, const T& val2, const char op)
    {
        switch (op) {
        case OP_INC:
            return val1 + val2;
        case OP_MIN:
            return val1 - val2;
        case OP_MUL:
            return val1 * val2;
        case OP_DIV:
            if (val2 == 0) {
                throw ParserException{ "cannot divide by zero" };
            }
            return val1 / val2;
        }
        return 0;
    }

    template<typename T>
    bool ArithmeticParser<T>::isValidOperator(const char op) const noexcept {
        switch (op) {
        case OP_INC:
        case OP_MIN:
        case OP_MUL:
        case OP_DIV:
            return true;
        }
        return false;
    }

    using ArithmeticParserInt = ArithmeticParser<int>;
    using ArithmeticParserDouble = ArithmeticParser<double>;
    using ArithmeticParserFloat = ArithmeticParser<float>;
}

#endif