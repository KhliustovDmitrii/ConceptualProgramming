#include "lexer_tests.h"

TEST_F(LexerTest, Should_return_empty_token_list_when_put_empty_expression)
{
    // Act
    Interpreter::Tokens tokens = Interpreter::Lexer::Tokenize(L"");

    // Assert
    ASSERT_TRUE(tokens.empty());
}

TEST_F(LexerTest, Should_tokenize_single_plus_operator)
{
    // Act
    Interpreter::Tokens tokens = Interpreter::Lexer::Tokenize(L"+");

    // Assert
    ASSERT_THAT(tokens, ::testing::ElementsAre(Interpreter::Operator::Plus));
}

TEST_F(LexerTest, Should_tokenize_single_digit)
{
    // Act
    Interpreter::Tokens tokens = Interpreter::Lexer::Tokenize(L"1");

    // Assert
    ASSERT_THAT(tokens, ::testing::ElementsAre(1.0));
}

TEST_F(LexerTest, Should_tokenize_floating_point_number)
{
    // Act
    Interpreter::Tokens tokens = Interpreter::Lexer::Tokenize(L"12.34");

    // Assert
    ASSERT_THAT(tokens, ::testing::Pointwise(::testing::DoubleNear(0.001), {12.34}));
}

TEST_F(LexerTest, Should_get_type_for_operator_token)
{
    // Act
    Interpreter::Token opToken(Interpreter::Operator::Plus);

    // Assert
    ASSERT_EQ(opToken.Type(), Interpreter::TokenType::Operator);
}

TEST_F(LexerTest, Should_get_type_for_number_token)
{
    // Act
    Interpreter::Token numToken(1.2);

    // Assert
    ASSERT_EQ(numToken.Type(), Interpreter::TokenType::Number);
}

TEST_F(LexerTest, Should_get_operator_code_from_operator_token)
{
    // Act
    Interpreter::Token token(Interpreter::Operator::Plus);

    // Assert
    ASSERT_EQ(token, Interpreter::Operator::Plus);
}


TEST_F(LexerTest, Should_get_number_value_from_number_token)
{
    // Act
    Interpreter::Token token(1.23);

    // Assert
    ASSERT_EQ(token, 1.23);
}
