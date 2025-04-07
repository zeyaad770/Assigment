#include <iostream>
#include <cctype>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;
char nextChar;
int charIndex = 0, lineNumber = 1, charPosition = 0;
string inputBuffer;

/* getChar - a function to get the next character of input and determine its character class */
void getChar()
{
    int len = inputBuffer.length();
    if (charIndex < len)
    {
        nextChar = inputBuffer[charIndex++];
    }
    else
    {
        nextChar = '\0';
    }
}

/* getNonBlank - a function to call getChar until it returns a non-whitespace character */
void getNonBlank()
{
    while (isspace(nextChar))
    {
        getChar();
    }
}

enum class TokenCode
{
    START,
    END,
    SEMICOLON,
    RESERVED_WORD,
    IDENTIFIER,
    UNKNOWN,
    INT_LIT,
    FLOAT_LIT,
    LETTER,
    ASSIGN_OP,
    ADD_OP,
    SUB_OP,
    DIV_OP,
    MULTI_OP,
    LEFT_PAREN,
    RIGHT_PAREN,
    EQUAL_OP,
    NOT_EQUAL,
    LESS_EQ,
    GREATER_EQ,
};

struct Token
{
    TokenCode type;
    string value;
    int line;
    int position;
};

// reserved words
vector<string> reservedWords = { "if", "else", "while", "return", "int", "float", "void" };

// Func to check if string is reserved word
bool isReservedWord(const string token)
{
    return find(reservedWords.begin(), reservedWords.end(), token) != reservedWords.end();
}

// Func to Identify Token
TokenCode lookup(const string& token)
{
    if (token == ";")
        return TokenCode::SEMICOLON;
    if (token == "+")
        return TokenCode::ADD_OP;
    if (token == "-")
        return TokenCode::SUB_OP;
    if (token == "*")
        return TokenCode::MULTI_OP;
    if (token == "/")
        return TokenCode::DIV_OP;
    if (token == "=")
        return TokenCode::ASSIGN_OP;
    if (token == "==")
        return TokenCode::EQUAL_OP;
    if (token == "!=")
        return TokenCode::NOT_EQUAL;
    if (token == "<=")
        return TokenCode::LESS_EQ;
    if (token == ">=")
        return TokenCode::GREATER_EQ;
    if (token == "(")
        return TokenCode::LEFT_PAREN;
    if (token == ")")
        return TokenCode::RIGHT_PAREN;
    if (isReservedWord(token))
        return TokenCode::RESERVED_WORD;
    if (isdigit(token[0]))
        return token.find('.') != string::npos ? TokenCode::FLOAT_LIT : TokenCode::INT_LIT;
    if (isalpha(token[0]))
        return TokenCode::IDENTIFIER;
    return TokenCode::UNKNOWN;
}

vector<Token> tokenize(const string input)
{
    vector<Token> tokens;
    string current;
    inputBuffer = input;
    charIndex = 0;
    lineNumber = 1;
    charPosition = 0;
    getChar();

    while (nextChar != '\0')
    {
        getNonBlank();
        current.clear();
        int startPos = charPosition;

        if (isalpha(nextChar)) // Identifiers or Reserved Words
        {
            while (isalpha(nextChar) || isdigit(nextChar))
            {
                current += nextChar;
                getChar();
                charPosition++;
            }
            tokens.push_back({ lookup(current), current, lineNumber, startPos });
        }
        else if (isdigit(nextChar)) // Numbers
        {
            while (isdigit(nextChar))
            {
                current += nextChar;
                getChar();
                charPosition++;
            }
            if (nextChar == '.')
            {
                current += nextChar;
                getChar();
                while (isdigit(nextChar))
                {
                    current += nextChar;
                    getChar();
                    charPosition++;
                }
                tokens.push_back({ TokenCode::FLOAT_LIT, current, lineNumber, startPos });
            }
            else
            {
                tokens.push_back({ TokenCode::INT_LIT, current, lineNumber, startPos });
            }
        }
        else if (ispunct(nextChar))
        {
            current += nextChar;
            char tempNext = inputBuffer[charIndex];

            if ((current == "=" && tempNext == '=') ||
                (current == "!" && tempNext == '=') ||
                (current == "<" && tempNext == '=') ||
                (current == ">" && tempNext == '='))
            {
                getChar();
                current += nextChar;
            }

            tokens.push_back({ lookup(current), current, lineNumber, startPos });
            getChar();
            charPosition++;
        }
    }

    return tokens;
}

int main()
{
    string input;
    cout << "Enter Line of code: ";
    getline(cin, input);
    auto tokens = tokenize(input);

    for (auto token : tokens)
    {
        cout << "Lexeme: " << token.value
            << " | Token: " << static_cast<int>(token.type)
            << " | Line: " << token.line
            << " | Position: " << token.position
            << endl;
    }

    cin >> input;
}