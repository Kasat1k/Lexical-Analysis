#include <iostream>
#include <vector>
#include <regex>
#include <string>
using namespace std;


// Enum для категоризації лексем
enum LexemeType {
    RESERVEDWORD, IDENTIFIER, STRING_CONSTANT, NUMERIC_CONSTANT, HEXADECIMAL_NUMBER,
    DECIMAL_NUMBER, OPERATOR, DELIMITER, PREPROCESSOR_DIRECTIVE, COMMENT, UNKNOWN
};
// Структура лексеми
struct Lexeme {
    string value;
    LexemeType type;
};

// Оголошення функцій
bool isReservedWord(const string& lexeme);
bool isIdentifier(const string& lexeme);
bool isStringConstant(const string& lexeme);
bool isNumericConstant(const string& lexeme);
bool isOperator(const string& lexeme);
bool isDelimiter(const string& lexeme);
bool isPreprocessorDirective(const string& lexeme);
vector<Lexeme> lexemeSize(const string& code);
void analyze(const vector<Lexeme>& lexemes);
void display(const vector<Lexeme>& lexemes);

// Основна функція
int main() {
    setlocale(LC_ALL, "uk_UA.UTF-8");
    string code = R"(
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
    vector<Lexeme> lexemes = lexemeSize(code);
    analyze(lexemes);
    display(lexemes);
    return 0;
}

// Реалізація функцій

bool isReservedWord(const string& Lexeme) {
    // Список зарезервованих слів Swift
    vector<string> reservedwords = { "associatedtype", "class", "deinit", "enum", "extension", "fileprivate", "func", "import", "init", "inout", "internal", "let", "open", "operator", "private", "protocol", "public", "static", "struct", "subscript", "typealias", "var", "break", "case", "continue", "default", "defer", "do", "else", "fallthrough", "for", "guard", "if", "in", "repeat", "return", "switch", "where", "while"};

    return find(reservedwords.begin(), reservedwords.end(), Lexeme) != reservedwords.end();
}

bool isIdentifier(const string& lexeme) {
    // Регулярний вираз для ідентифікаторів Swift: [_a-zA-Z][_a-zA-Z0-9]*
    regex identifier_regex("[_a-zA-Z][_a-zA-Z0-9]*");
    return regex_match(lexeme, identifier_regex);
}

bool isStringConstant(const string& lexeme) {
    // Регулярний вираз для рядкової константи: "([^"\]|\.)*"
    regex string_regex("\"([^\"\\\\]|\\\\.)*\"");
    return regex_match(lexeme, string_regex);
}

bool isHexadecimalNumber(const string& lexeme) {
    // Регулярний вираз для шістнадцяткових чисел 
    regex hexPattern("^0[xX][0-9a-fA-F]+$");
    return regex_match(lexeme, hexPattern);
}

bool isDecimalNumber(const string& lexeme) {
    // Регулярний вираз для десяткових чисел
    regex decimalPattern("^\\d*\\.\\d+|\\d+\\.$");
    return regex_match(lexeme, decimalPattern);
}

bool isNumericConstant(const string& lexeme) {
    // Регулярний вираз для числової константи
    regex numeric_regex("([0-9]*\\.[0-9]+|[0-9]+)");
    return regex_match(lexeme, numeric_regex);
}

bool isOperator(const string& lexeme) {

    // Список операторів мови Swift
    vector<string> operators = { "+", "-", "*", "/", "%", "&", "|", "^", "!", "~", "++", "--", "<<", ">>", "==", "!=", ">", "<", ">=", "<=", "&&", "||", "??", "?:", "=>", "=", "+=", "-=", "*=", "/=", "%=", "&=", "|=", "^=", "<<=", ">>=", "[]", "()", ".", "->" };

    return find(operators.begin(), operators.end(), lexeme) != operators.end();
}

bool isDelimiter(const string& lexeme) {

    // Список розділових знаків мови Swift
    vector<string> delimiters = { ",", ";", "{", "}", "(", ")", "[", "]", ":", "?" };

    return find(delimiters.begin(), delimiters.end(), lexeme) != delimiters.end();
}

bool isPreprocessorDirective(const string& lexeme) {
    // Регулярний вираз для директив препроцесора: #<directive>
    regex preprocessor_regex("#[a-z]+");
    return regex_match(lexeme, preprocessor_regex);
}

void display(const vector<Lexeme>& lexemes) {
    for (const Lexeme& lexeme : lexemes) {
        cout << "Lexeme: " << lexeme.value << " - Type: ";
        switch (lexeme.type) {
        case RESERVEDWORD: cout << "Reservedword"; break;
        case IDENTIFIER: cout << "Identifier"; break;
        case STRING_CONSTANT: cout << "String Constant"; break;
        case NUMERIC_CONSTANT: cout << "Numeric Constant"; break;
        case HEXADECIMAL_NUMBER: cout << "Hexadecimal Number"; break;
        case DECIMAL_NUMBER: cout << "Decimal Number"; break;
        case OPERATOR: cout << "Operator"; break;
        case DELIMITER: cout << "Delimiter"; break;
        case PREPROCESSOR_DIRECTIVE: cout << "Preprocessor Directive"; break;
        case COMMENT: cout << "Comment"; break;
        case UNKNOWN: cout << "Unknown"; break;
        }
        cout << endl;
    }
}

 vector<Lexeme> lexemeSize(const string& code) {
    vector<Lexeme> lexemes;
    string lexeme;
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
            lexeme += c;
            while (i + 1 < code.length() && !isspace(code[i + 1])) {
                lexeme += code[++i];
            }
            lexemes.push_back({ lexeme, PREPROCESSOR_DIRECTIVE });
            lexeme.clear();
            continue;
        }

        // Обробка закінчення коментарів
        if (isSingleLineComment && c == '\n') {
            isSingleLineComment = false;
            lexemes.push_back({ lexeme, COMMENT });
            lexeme.clear();
        }
        if (isMultiLineComment && c == '*' && i + 1 < code.length() && code[i + 1] == '/') {
            isMultiLineComment = false;
            lexeme += "*/";
            lexemes.push_back({ lexeme, COMMENT });
            lexeme.clear();
            ++i;  
            continue;
        }


        // Збір символів всередині коментарів
        if (isSingleLineComment || isMultiLineComment) {
            lexeme += c;
            continue;
        }

        // Обробка рядків
        if (c == '\"') {
            if (isString) {
                lexeme += c;
                lexemes.push_back({ lexeme, STRING_CONSTANT });
                lexeme.clear();
            }
            else {
                if (!lexeme.empty()) {
                    lexemes.push_back({ lexeme, UNKNOWN });  // Додати попередню лексему як "UNKNOWN"
                    lexeme.clear();
                }
            }
            isString = !isString;
        }
        else if (isString) {
            lexeme += c;
        }

        // Обробка інших лексем
        else if (isspace(c)) {
            if (!lexeme.empty()) {
                lexemes.push_back({ lexeme, UNKNOWN });  // Додати попередню лексему як "UNKNOWN"
                lexeme.clear();
            }
        }
        else if (ispunct(c)) {
            if (!lexeme.empty()) {
                lexemes.push_back({ lexeme, UNKNOWN });  // Додати попередню лексему як "UNKNOWN"
                lexeme.clear();
            }
            lexemes.push_back({ string(1, c), UNKNOWN });  // Додати попередню лексему як "UNKNOWN"
        }
        else {
            lexeme += c;
        }
    }

    // Перевірка наявності будь-якої залишкової лексеми
    if (!lexeme.empty()) {
        lexemes.push_back({ lexeme, UNKNOWN });  // Додати попередню лексему як "UNKNOWN"
    }


    // Розбиття на категорії лексем
    for (Lexeme& t : lexemes) {
        if (isReservedWord(t.value)) t.type = RESERVEDWORD;
        else if (isIdentifier(t.value)) t.type = IDENTIFIER;
        else if (isHexadecimalNumber(t.value)) t.type = HEXADECIMAL_NUMBER;
        else if (isDecimalNumber(t.value)) t.type = DECIMAL_NUMBER;
        else if (isNumericConstant(t.value)) t.type = NUMERIC_CONSTANT;
        else if (isOperator(t.value)) t.type = OPERATOR;
        else if (isDelimiter(t.value)) t.type = DELIMITER;
        else if (isPreprocessorDirective(t.value)) t.type = PREPROCESSOR_DIRECTIVE;
        
    }

    return lexemes;
}



void analyze(const vector<Lexeme>& lexemes) {
    int reservedwordCount = 0;
    int identifierCount = 0;
    int stringConstantCount = 0;
    int numericConstantCount = 0;
    int operatorCount = 0;
    int delimiterCount = 0;
    int preprocessorDirectiveCount = 0;
    int unknownCount = 0;

    // Підрахунок кількості лексем кожного типу
    for (const Lexeme& lexeme : lexemes) {
        switch (lexeme.type) {
        case RESERVEDWORD: ++reservedwordCount; break;
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
    cout << "Analysis Results:" << endl;
    cout << "Reservedwords: " << reservedwordCount << endl;
    cout << "Identifiers: " << identifierCount << endl;
    cout << "String Constants: " << stringConstantCount << endl;
    cout << "Numeric Constants: " << numericConstantCount << endl;
    cout << "Operators: " << operatorCount << endl;
    cout << "Delimiters: " << delimiterCount << endl;
    cout << "Preprocessor Directives: " << preprocessorDirectiveCount << endl;
    cout << "Unknown: " << unknownCount << endl;
}

