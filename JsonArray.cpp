#include<map>
#include<cstdlib>
#include "Json.h"

using namespace std;

string JsonArray::getString(int key){
    map<int, string>::iterator it;

    it = this->strings->find(key);

    if(it == this->strings->end())
        throw JsonKeyNotFoundException(key);

    return it->second;
}

int JsonArray::getInt(int key){
    map<int, string>::iterator it;

    it = this->nums->find(key);

    if(it == this->nums->end())
        throw JsonKeyNotFoundException(key);

    return atoi(it->second.c_str());
}

long JsonArray::getLong(int key){
    map<int, string>::iterator it;

    it = this->nums->find(key);

    if(it == this->nums->end())
        throw JsonKeyNotFoundException(key);

    return atol(it->second.c_str());
}

float JsonArray::getFloat(int key){
    map<int, string>::iterator it;

    it = this->nums->find(key);

    if(it == this->nums->end())
        throw JsonKeyNotFoundException(key);

    return atof(it->second.c_str());
}

double JsonArray::getDouble(int key){
    map<int, string>::iterator it;

    it = this->nums->find(key);

    if(it == this->nums->end())
        throw JsonKeyNotFoundException(key);

    return strtod(it->second.c_str(), 0);
}

bool JsonArray::getBoolean(int key) {
    map<int, bool>::iterator it;

    it = this->bools->find(key);

    if(it == this->bools->end())
        throw JsonKeyNotFoundException(key);

    return it->second;
}

void* JsonArray::getNull(int key) {
    map<int, void*>::iterator it;

    it = this->nulls->find(key);

    if(it == this->nulls->end())
        throw JsonKeyNotFoundException(key);

    return it->second;
}

JsonObject* JsonArray::getJsonObject(int key){
    map<int, JsonObject*>::iterator it;

    it = this->objects->find(key);

    if(it == this->objects->end())
        throw JsonKeyNotFoundException(key);

    return it->second;
}

JsonArray* JsonArray::getJsonArray(int key){
    map<int, JsonArray*>::iterator it;

    it = this->arrays->find(key);

    if(it == this->arrays->end())
        throw JsonKeyNotFoundException(key);

    return it->second;
}

void JsonArray::addString(string value) {
    this->strings->insert( pair<int, string>( index++, value ) );
}

void JsonArray::addObject(JsonObject* value) {
    this->objects->insert( pair<int, JsonObject*>( index++, value ) );
}

void JsonArray::addArray(JsonArray* value) {
    this->arrays->insert( pair<int, JsonArray*>( index++, value ) );
}

void JsonArray::addNumber(string value) {
    this->nums->insert( pair<int, string>( index++, value ) );
}

void JsonArray::addInt(int value) {
    this->nums->insert( pair<int, string>( index++, cJSONpp::itoa(value) ) );
}

void JsonArray::addLong(long value) {
    this->nums->insert( pair<int, string>( index++, cJSONpp::ltoa(value) ) );
}

void JsonArray::addFloat(float value) {
    this->nums->insert( pair<int, string>( index++, cJSONpp::ftoa(value) ) );
}

void JsonArray::addDouble(double value) {
    this->nums->insert( pair<int, string>( index++, cJSONpp::dtoa(value) ) );
}

void JsonArray::addBoolean(bool value) {
    this->bools->insert( pair<int, bool>( index++, value ) );
}

void JsonArray::addNull(void* value) {
    this->nulls->insert( pair<int, void*>( index++, value ) );
}

void JsonArray::parseValue(int& i, bool isRoot) {
    if(!consumeWhiteSpace(i)) {
        throwUnexpectedEndException();
    }

    bool defaulted = false;
    switch((*jsonString)[i]) {
        case '"':
            addString(parseString(i));
            break;
        case '{':
            addObject(new JsonObject(jsonString, i, false));
            break;
        case '[':
            addArray(new JsonArray(jsonString, i, false));
            break;
        case '-': case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
            addNumber(parseNumber(i));
            break;
        case 't': case 'f':
            addBoolean(parseBoolean(i));
            break;
        case 'n':
            addNull(parseNull(i));
            break;
        default:
            defaulted = true;
    }

    if(defaulted) {
        throwInvalidCharacterException(i);
    }
}

bool JsonArray::detectNumberEnd(int& i) {
    return jsonString->at(i) == ',' || jsonString->at(i) == ']' || isWhiteSpaceOrLine(jsonString->at(i));
}

void JsonArray::init(string* json, int& i, bool isRoot) {
    this->strings = new map<int, string>();
    this->objects = new map<int, JsonObject*>();
    this->arrays = new map<int, JsonArray*>();
    this->nums = new map<int, string>();
    this->bools = new map<int, bool>();
    this->nulls = new map<int, void*>();

    this->jsonString = json;
    this->index = 0;

    if(this->jsonString->length() < 1) {
        throw InvalidJsonException("JSON Array cannot be empty");
    }
    if(!consumeWhiteSpace(i)) {
        throwUnexpectedEndException();
    }
    parseArray(i, isRoot);
}

JsonArray::JsonArray(string* json, int& i, bool isRoot) {
    this->i = i;
    init(json, i, isRoot);
}

JsonArray::JsonArray(string* json) {
    this->i = 0;
    init(json, i, true);
}
