#include <iostream>
#include <vector>
#include <regex>
#include <string>



// Enum для категоризації токенів
enum TokenType {
    KEYWORD, IDENTIFIER, STRING_CONSTANT, NUMERIC_CONSTANT, HEXADECIMAL_NUMBER,
    DECIMAL_NUMBER, OPERATOR, DELIMITER, PREPROCESSOR_DIRECTIVE, COMMENT, UNKNOWN
};


// Структура токену
struct Token {
    std::string value;
    TokenType type;
};

// Оголошення функцій
bool isKeyword(const std::string& token);
bool isIdentifier(const std::string& token);
bool isStringConstant(const std::string& token);
bool isNumericConstant(const std::string& token);
bool isOperator(const std::string& token);
bool isDelimiter(const std::string& token);
bool isPreprocessorDirective(const std::string& token);
std::vector<Token> tokenize(const std::string& code);
void analyze(const std::vector<Token>& tokens);
void displayTokens(const std::vector<Token>& tokens);

// Основна функція
int main() {
    setlocale(LC_ALL, "uk_UA.UTF-8");

    std::string code = R"(
        // Це коментар
import Foundation

// Директива препроцесора
#if DEBUG
print("Debug режим")
#else
print("Release режим")
#endif

// Змінні та ідентифікатори
var age: Int = 30
var name: String = "Іван"
let pi: Double = 3.14
var isStudent: Bool = true

// Шістнадцяткові числа
let hexNumber: Int = 0x1A3
let anotherHexNumber: Int = 0xFF

// Рядкові та символьні константи
let message: String = "Привіт, світе!"
let grade: Character = "A"

// Оператори
let sum = age + 5
let isAdult = age >= 18
let result = (pi * 2) / Double(age)

// Розділові знаки
let coordinates = (x: 10.0, y: 5.0)
let greeting = "Hello, " + name

// Розгалуження
if isStudent {
    print("Студент")
} else {
    print("Не студент")
}

// Цикли
for i in 1...5 {
    print("Число: \(i) ") }

// Функції
func sayHello(name: String) {
    print("Привіт, \(name)!")
}

sayHello(name: "Hello")

// Кінець програми
  )";
    std::vector<Token> tokens = tokenize(code);
    analyze(tokens);
    displayTokens(tokens);
    return 0;
}

// Реалізація функцій
bool isKeyword(const std::string& token) {
    // Список зарезервованих слів Swift
    std::vector<std::string> keywords = { "associatedtype", "class", "deinit", "enum", "extension", "fileprivate", "func", "import", "init", "inout", "internal", "let", "open", "operator", "private", "protocol", "public", "static", "struct", "subscript", "typealias", "var", "break", "case", "continue", "default", "defer", "do", "else", "fallthrough", "for", "guard", "if", "in", "repeat", "return", "switch", "where", "while"};

    return std::find(keywords.begin(), keywords.end(), token) != keywords.end();
}

bool isIdentifier(const std::string& token) {
    // Регулярний вираз для ідентифікаторів Swift: [_a-zA-Z][_a-zA-Z0-9]*
    std::regex identifier_regex("[_a-zA-Z][_a-zA-Z0-9]*");
    return std::regex_match(token, identifier_regex);
}

bool isStringConstant(const std::string& token) {
    // Регулярний вираз для рядкової константи: "([^"\]|\.)*"
    std::regex string_regex("\"([^\"\\\\]|\\\\.)*\"");
    return std::regex_match(token, string_regex);
}

bool isHexadecimalNumber(const std::string& token) {
    // Регулярний вираз для шістнадцяткових чисел 
    std::regex hexPattern("^0[xX][0-9a-fA-F]+$");
    return std::regex_match(token, hexPattern);
}

bool isDecimalNumber(const std::string& token) {
    // Регулярний вираз для десяткових чисел
    std::regex decimalPattern("^\\d*\\.\\d+|\\d+\\.$");
    return std::regex_match(token, decimalPattern);
}

bool isNumericConstant(const std::string& token) {
    // Регулярний вираз для числової константи
    std::regex numeric_regex("([0-9]*\\.[0-9]+|[0-9]+)");
    return std::regex_match(token, numeric_regex);
}

bool isOperator(const std::string& token) {

    // Список операторів мови Swift
    std::vector<std::string> operators = { "+", "-", "*", "/", "%", "&", "|", "^", "!", "~", "++", "--", "<<", ">>", "==", "!=", ">", "<", ">=", "<=", "&&", "||", "??", "?:", "=>", "=", "+=", "-=", "*=", "/=", "%=", "&=", "|=", "^=", "<<=", ">>=", "[]", "()", ".", "->" };

    return std::find(operators.begin(), operators.end(), token) != operators.end();
}

bool isDelimiter(const std::string& token) {

    // Список розділових знаків мови Swift
    std::vector<std::string> delimiters = { ",", ";", "{", "}", "(", ")", "[", "]", ":", "?" };

    return std::find(delimiters.begin(), delimiters.end(), token) != delimiters.end();
}

bool isPreprocessorDirective(const std::string& token) {
    // Регулярний вираз для директив препроцесора: #<directive>
    std::regex preprocessor_regex("#[a-z]+");
    return std::regex_match(token, preprocessor_regex);
}

std::vector<Token> tokenize(const std::string& code) {
    std::vector<Token> tokens;
    std::string token;
    bool isString = false;
    bool isSingleLineComment = false;
    bool isMultiLineComment = false;

    for (size_t i = 0; i < code.length(); ++i) {
        char c = code[i];

 
        if (c == '/' && !isString && !isMultiLineComment) {
            if (i + 1 < code.length()) {
                if (code[i + 1] == '/') isSingleLineComment = true;
                else if (code[i + 1] == '*') isMultiLineComment = true;
            }
        }
        // Обробка директив препроцесора
        if (c == '#' && !isString && !isSingleLineComment && !isMultiLineComment) {
            token += c;
            while (i + 1 < code.length() && !std::isspace(code[i + 1])) {
                token += code[++i];
            }
            tokens.push_back({ token, PREPROCESSOR_DIRECTIVE });
            token.clear();
            continue;
        }

        // Обробка закінчення коментарів
        if (isSingleLineComment && c == '\n') {
            isSingleLineComment = false;
            tokens.push_back({ token, COMMENT });
            token.clear();
        }
        if (isMultiLineComment && c == '*' && i + 1 < code.length() && code[i + 1] == '/') {
            isMultiLineComment = false;
            token += "*/";
            tokens.push_back({ token, COMMENT });
            token.clear();
            ++i;  
            continue;
        }


        // Збір символів всередині коментарів
        if (isSingleLineComment || isMultiLineComment) {
            token += c;
            continue;
        }

        // Обробка рядків
        if (c == '\"') {
            if (isString) {
                token += c;
                tokens.push_back({ token, STRING_CONSTANT });
                token.clear();
            }
            else {
                if (!token.empty()) {
                    tokens.push_back({ token, UNKNOWN });  // Додати попередній токен як "UNKNOWN"
                    token.clear();
                }
            }
            isString = !isString;
        }
        else if (isString) {
            token += c;
        }

        // Обробка інших токенів
        else if (std::isspace(c)) {
            if (!token.empty()) {
                tokens.push_back({ token, UNKNOWN });  // Додати попередній токен як "UNKNOWN" 
                token.clear();
            }
        }
        else if (std::ispunct(c)) {
            if (!token.empty()) {
                tokens.push_back({ token, UNKNOWN });  // Додати попередній токен як "UNKNOWN" 
                token.clear();
            }
            tokens.push_back({ std::string(1, c), UNKNOWN });  // Додати попередній токен як "UNKNOWN" 
        }
        else {
            token += c;
        }
    }

    // Перевірка наявності будь-якого залишкового токену
    if (!token.empty()) {
        tokens.push_back({ token, UNKNOWN });  // Додати попередній токен як "UNKNOWN" 
    }


    // Розбиття на категорії токенів
    for (Token& t : tokens) {
        if (isKeyword(t.value)) t.type = KEYWORD;
        else if (isIdentifier(t.value)) t.type = IDENTIFIER;
        else if (isHexadecimalNumber(t.value)) t.type = HEXADECIMAL_NUMBER;
        else if (isDecimalNumber(t.value)) t.type = DECIMAL_NUMBER;
        else if (isNumericConstant(t.value)) t.type = NUMERIC_CONSTANT;
        else if (isOperator(t.value)) t.type = OPERATOR;
        else if (isDelimiter(t.value)) t.type = DELIMITER;
        else if (isPreprocessorDirective(t.value)) t.type = PREPROCESSOR_DIRECTIVE;
        
    }

    return tokens;
}



void analyze(const std::vector<Token>& tokens) {
    int keywordCount = 0;
    int identifierCount = 0;
    int stringConstantCount = 0;
    int numericConstantCount = 0;
    int operatorCount = 0;
    int delimiterCount = 0;
    int preprocessorDirectiveCount = 0;
    int unknownCount = 0;

    // Підрахунок кількості токенів кожного типу
    for (const Token& token : tokens) {
        switch (token.type) {
        case KEYWORD: ++keywordCount; break;
        case IDENTIFIER: ++identifierCount; break;
        case STRING_CONSTANT: ++stringConstantCount; break;
        case NUMERIC_CONSTANT: ++numericConstantCount; break;
        case OPERATOR: ++operatorCount; break;
        case DELIMITER: ++delimiterCount; break;
        case PREPROCESSOR_DIRECTIVE: ++preprocessorDirectiveCount; break;
        case UNKNOWN: ++unknownCount; break;
        }
    }

    // Вивід
    std::cout << "Analysis Results:" << std::endl;
    std::cout << "Keywords: " << keywordCount << std::endl;
    std::cout << "Identifiers: " << identifierCount << std::endl;
    std::cout << "String Constants: " << stringConstantCount << std::endl;
    std::cout << "Numeric Constants: " << numericConstantCount << std::endl;
    std::cout << "Operators: " << operatorCount << std::endl;
    std::cout << "Delimiters: " << delimiterCount << std::endl;
    std::cout << "Preprocessor Directives: " << preprocessorDirectiveCount << std::endl;
    std::cout << "Unknown: " << unknownCount << std::endl;
}

void displayTokens(const std::vector<Token>& tokens) {
    for (const Token& token : tokens) {
        std::cout << "Lexeme: " << token.value << " - Type: ";
        switch (token.type) {
        case KEYWORD: std::cout << "Keyword"; break;
        case IDENTIFIER: std::cout << "Identifier"; break;
        case STRING_CONSTANT: std::cout << "String Constant"; break;
        case NUMERIC_CONSTANT: std::cout << "Numeric Constant"; break;
        case HEXADECIMAL_NUMBER: std::cout << "Hexadecimal Number"; break;
        case DECIMAL_NUMBER: std::cout << "Decimal Number"; break;
        case OPERATOR: std::cout << "Operator"; break;
        case DELIMITER: std::cout << "Delimiter"; break;
        case PREPROCESSOR_DIRECTIVE: std::cout << "Preprocessor Directive"; break;
        case COMMENT: std::cout << "Comment"; break;
        case UNKNOWN: std::cout << "Unknown"; break;
        }
        std::cout << std::endl;
    }
}