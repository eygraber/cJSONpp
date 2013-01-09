#include<map>
#include "Json.h"

using namespace std;

void JsonType::throwInvalidCharacterException(int i) {
    string one = "Invalid character '";
    string two = "'";
    throw InvalidJsonException(one + jsonString->at(i) + two, i);
}

void JsonType::throwInvalidEscapeCodeException(int i) {
    string one = "Invalid escape character '";
    string two = "'";
    throw InvalidJsonException(one + jsonString->at(i) + two, i);
}

void JsonType::throwUnexpectedEndException() {
    string one = "Invalid escape character '";
    string two = "'";
    throw InvalidJsonException("Unexpected end of input");
}

bool JsonType::getNextToken(int& i) {
    if((i++) == jsonString->length()) {
        return false;
    }
    else {
        return true;
    }
}

bool JsonType::isEscapeCode(int& i){
    switch(jsonString->at(i)){
        case 92: case 47: case '"': case 'b': case 'f': case 'n': case 'r': case 't':
            return true;
        case 'u':
            return isUnicodeEscapeCode(i);
        default:
            return false;
    }
}

bool JsonType::isUnicodeEscapeCode(int& i){
    for(int j = 0; j < 4; j++) {
        char c = jsonString->at(i);
        if((c < 48 && c > 57) && (c < 65 && c > 70) && (c < 97 && c > 102)) { //if the char is not 0-9 or [a-fA-F]
            return false;
        }
        if(!getNextToken(i)) {
            throwUnexpectedEndException();
        }
    }
    i--;
    return true;
}

bool JsonType::isWhiteSpaceOrLine(char c) {
    if(c == 9 || c == 10 || c == 13 || c == 32) {
        return true;
    }
    else {
        return false;
    }
}

bool JsonType::consumeWhiteSpace(int& i) {
    while(isWhiteSpaceOrLine(jsonString->at(i))) {
        if(!getNextToken(i)) {
            return false;
        }
    }
    return true;
}

/*void JsonType::parseEscapeCode(int &i) {
    switch(jsonString->at(i){
        case 92: case 47: case '"': case 'b': case 'f': case 'n': case 'r': case 't':
            s += jsonString->at(i);
        case 'u':
            try {
                getNextToken(i);
                parseUnicodeEscapeCode(s, i);
            } catch(InvalidJsonException e) { throw e; }
        default:
            throw InvalidJsonException(string("Invalid escape character ") + this->jsonString->at(i) + string(" at position ") + CJSON::itoa(i));
    }
}

void JsonType::parseUnicodeEscapeCode(string&s, int& i) {
    for(int j = 0; j < 4; j++) {
        char c = jsonString->at(i);
        if((c < 48 && c > 57) && (c < 65 && c > 70) && (c < 97 && c > 102)) { //if the char is not 0-9 or [a-fA-F]
            throw InvalidJsonException(string("Invalid escape character ") + this->jsonString->at(i) + string(" at position ") + CJSON::itoa(i));
        }
        try {
            getNextToken(i);
        } catch(InvalidJsonException e) { throw e; }
    }
}*/

void JsonType::parseObject(int& i, bool isRoot) {
    if(jsonString->at(i) != '{') {
        throw InvalidJsonException("JSON Object must begin with '{'");
    }

    if(!getNextToken(i) || !consumeWhiteSpace(i)) {
        throwUnexpectedEndException();
    }

    if(jsonString->at(i) == '}') {
        return;
    }

    while(true) {
        string name = parseName(i);
        if(!getNextToken(i) || !consumeWhiteSpace(i)) {
            throwUnexpectedEndException();
        }
        if(jsonString->at(i) != ':') {
            throwInvalidCharacterException(i);
        }
        if(!getNextToken(i)) {
            throwUnexpectedEndException();
        }
        parseValue(i, &name, isRoot);
        if(!getNextToken(i) || !consumeWhiteSpace(i)) {
            throwUnexpectedEndException();
        }
        if(jsonString->at(i) == '}') {
            break;
        }
        else if(jsonString->at(i) != ',') {
            throwInvalidCharacterException(i);
        }
        else {
            if(!getNextToken(i)) {
                throwUnexpectedEndException();
            }
        }
    }
}

void JsonType::parseArray(int& i, bool isRoot) {
    if(jsonString->at(i) != '[') {
        throw InvalidJsonException("JSON Array must begin with '['");
    }

    if(!getNextToken(i) || !consumeWhiteSpace(i)) {
        throwUnexpectedEndException();
    }

    if(jsonString->at(i) == ']') {
        return;
    }

    while(true) {
        parseValue(i, isRoot);
        if(!getNextToken(i) || !consumeWhiteSpace(i)) {
            throwUnexpectedEndException();
        }
        if(jsonString->at(i) == ']') {
            break;
        }
        else if(jsonString->at(i) != ',') {
            throwInvalidCharacterException(i);
        }
        else {
            if(!getNextToken(i)) {
                throwUnexpectedEndException();
            }
        }
    }
}

string JsonType::parseNumber(int& i) {
    if(!consumeWhiteSpace(i)) {
        throwUnexpectedEndException();
    }

    int start = i;

    if(jsonString->at(i) == '-') {
        if(!getNextToken(i)) {
            throwUnexpectedEndException();
        }
    }

    if(jsonString->at(i) == '0') {
        if(!getNextToken(i)) {
            throwUnexpectedEndException();
        }
        if(detectNumberEnd(i)) {
            i--;
            return (*jsonString).substr(start, (i + 1) - start);
        }
    }
    else {
        if(isDigit(jsonString->at(i), 49)) {
            if(!getNextToken(i)) {
                throwUnexpectedEndException();
            }
        }
        else {
            throwInvalidCharacterException(i);
        }
        if(!parseDigit(i)) {
            throwUnexpectedEndException();
        }
        if(detectNumberEnd(i)) {
            i--;
            return (*jsonString).substr(start, (i + 1) - start);
        }
    }
    if(jsonString->at(i) != '.') {
        throwInvalidCharacterException(i);
    }
    if(!getNextToken(i) || !parseDigit(i)) {
        throwUnexpectedEndException();
    }
    if(detectNumberEnd(i)) {
        i--;
        return (*jsonString).substr(start, (i + 1) - start);
    }

    if(jsonString->at(i) == 'e' || jsonString->at(i) == 'E') {
        if(!getNextToken(i)) {
            throwUnexpectedEndException();
        }
    }
    else {
        throwInvalidCharacterException(i);
    }
    if(jsonString->at(i) == '-' || jsonString->at(i) == '+') {
        if(!getNextToken(i)) {
            throwUnexpectedEndException();
        }
    }
    else {
        throwInvalidCharacterException(i);
    }
    if(!parseDigit(i)) {
        throwUnexpectedEndException();
    }

    i--;

    return jsonString->substr(start, (i + 1) - start);
}

bool JsonType::isDigit(char c, int lowerBound, int upperBound) {
    if(c >= lowerBound && c <= upperBound) {
        return true;
    }
    else {
        return false;
    }
}

bool JsonType::parseDigit(int& i) {
    while(isDigit(jsonString->at(i))) {
        if(!getNextToken(i)) {
            return false;
        }
    }
    return true;
}

void* JsonType::parseNull(int& i) {
    if(jsonString->at(i) == 'n') {
        string nul = "ull";
        for(int j = 0; j < nul.length(); j++) {
            if(!getNextToken(i)) {
                throwUnexpectedEndException();
            }
            if(jsonString->at(i) != nul[j]) {
                throwInvalidCharacterException(i);
            }
        }
        return 0;
    }

    throwInvalidCharacterException(i);
}

bool JsonType::parseBoolean(int& i) {
    if(jsonString->at(i) == 'f') {
        string fals = "alse";
        for(int j = 0; j < fals.length(); j++) {
            if(!getNextToken(i)) {
                throwUnexpectedEndException();
            }
            if(jsonString->at(i) != fals[j]) {
                throwInvalidCharacterException(i);
            }
        }
        return false;
    }
    else if(jsonString->at(i) == 't') {
        string tru = "rue";
        for(int j = 0; j < tru.length(); j++) {
            if(!getNextToken(i)) {
                throwUnexpectedEndException();
            }
            if(jsonString->at(i) != tru[j]) {
                throwInvalidCharacterException(i);
            }
        }
        return true;
    }

    throwInvalidCharacterException(i);
}

string JsonType::parseName(int& i) {
    if(!consumeWhiteSpace(i)) {
        throwUnexpectedEndException();
    }
    return parseString(i);
}

string JsonType::parseString(int& i) {
    if(jsonString->at(i) != '"') {
        throwInvalidCharacterException(i);
    }
    int start = i + 1;
    bool escape = false;

    while(true) {
        if(!getNextToken(i)) {
            throwUnexpectedEndException();
        }

        if(escape){
            if(!isEscapeCode(i)) {
                    throwInvalidEscapeCodeException(i);
            }
            if(!getNextToken(i)) {
                throwUnexpectedEndException();
            }
            escape = false;
            continue;
        }

        if(jsonString->at(i) == 92) {
            escape = true;
        }
        else if(!escape && jsonString->at(i) == '"') {
            break;
        }
    }

    return jsonString->substr(start, (i + 1) - start);
}

