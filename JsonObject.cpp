#include<map>
#include<cstdlib>
#include "Json.h"

using namespace std;

string JsonObject::getString(string key){
    map<string, string>::iterator it;

    it = this->strings->find(key);

    if(it == this->strings->end())
        throw JsonKeyNotFoundException(key);

    return it->second;
}

int JsonObject::getInt(string key){
    map<string, string>::iterator it;

    it = this->nums->find(key);

    if(it == this->nums->end())
        throw JsonKeyNotFoundException(key);

    return atoi(it->second.c_str());
}

long JsonObject::getLong(string key){
    map<string, string>::iterator it;

    it = this->nums->find(key);

    if(it == this->nums->end())
        throw JsonKeyNotFoundException(key);

    return atol(it->second.c_str());
}

float JsonObject::getFloat(string key){
    map<string, string>::iterator it;

    it = this->nums->find(key);

    if(it == this->nums->end())
        throw JsonKeyNotFoundException(key);

    return atof(it->second.c_str());
}

double JsonObject::getDouble(string key){
    map<string, string>::iterator it;

    it = this->nums->find(key);

    if(it == this->nums->end())
        throw JsonKeyNotFoundException(key);

    return strtod(it->second.c_str(), 0);
}

bool JsonObject::getBoolean(string key) {
    map<string, bool>::iterator it;

    it = this->bools->find(key);

    if(it == this->bools->end())
        throw JsonKeyNotFoundException(key);

    return it->second;
}

void* JsonObject::getNull(string key) {
    map<string, void*>::iterator it;

    it = this->nulls->find(key);

    if(it == this->nulls->end())
        throw JsonKeyNotFoundException(key);

    return it->second;
}

JsonObject* JsonObject::getJsonObject(string key){
    map<string, JsonObject*>::iterator it;

    it = this->objects->find(key);

    if(it == this->objects->end())
        throw JsonKeyNotFoundException(key);

    return it->second;
}

JsonArray* JsonObject::getJsonArray(string key){
    map<string, JsonArray*>::iterator it;

    it = this->arrays->find(key);

    if(it == this->arrays->end())
        throw JsonKeyNotFoundException(key);

    return it->second;
}

void JsonObject::addString(string name, string value) {
    this->strings->insert( pair<string, string>( name, value ) );
}

void JsonObject::addObject(string name, JsonObject* value) {
    this->objects->insert( pair<string, JsonObject*>( name, value ) );
}

void JsonObject::addArray(string name, JsonArray* value) {
    this->arrays->insert( pair<string, JsonArray*>( name, value ) );
}

void JsonObject::addNumber(string name, string value) {
    this->nums->insert( pair<string, string>( name, value ) );
}

void JsonObject::addInt(string name, int value) {
    this->nums->insert( pair<string, string>( name, cJSONpp::itoa(value) ) );
}

void JsonObject::addLong(string name, long value) {
    this->nums->insert( pair<string, string>( name, cJSONpp::ltoa(value) ) );
}

void JsonObject::addFloat(string name, float value) {
    this->nums->insert( pair<string, string>( name, cJSONpp::ftoa(value) ) );
}

void JsonObject::addDouble(string name, double value) {
    this->nums->insert( pair<string, string>( name, cJSONpp::dtoa(value) ) );
}

void JsonObject::addBoolean(string name, bool value) {
    this->bools->insert( pair<string, bool>( name, value ) );
}

void JsonObject::addNull(string name, void* value) {
    this->nulls->insert( pair<string, void*>( name, value ) );
}

void JsonObject::addString(string* name, string value) {
    this->strings->insert( pair<string, string>( *name, value ) );
}

void JsonObject::addObject(string* name, JsonObject* value) {
    this->objects->insert( pair<string, JsonObject*>( *name, value ) );
}

void JsonObject::addArray(string* name, JsonArray* value) {
    this->arrays->insert( pair<string, JsonArray*>( *name, value ) );
}

void JsonObject::addNumber(string* name, string value) {
    this->nums->insert( pair<string, string>( *name, value ) );
}

void JsonObject::addBoolean(string* name, bool value) {
    this->bools->insert( pair<string, bool>( *name, value ) );
}

void JsonObject::addNull(string* name, void* value) {
    this->nulls->insert( pair<string, void*>( *name, value ) );
}

void JsonObject::parseValue(int& i, string* name, bool isRoot) {
    if(!consumeWhiteSpace(i)) {
        throwUnexpectedEndException();
    }

    bool defaulted = false;
    switch((*jsonString)[i]) {
        case '"':
            addString(name, parseString(i));
            break;
        case '{':
            addObject(name, new JsonObject(jsonString, i, false));
            break;
        case '[':
            addArray(name, new JsonArray(jsonString, i, false));
            break;
        case '-': case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
            addNumber(name, parseNumber(i));
            break;
        case 't': case 'f':
            addBoolean(name, parseBoolean(i));
            break;
        case 'n':
            addNull(name, parseNull(i));
            break;
        default:
            defaulted = true;
    }

    if(defaulted) {
        throwInvalidCharacterException(i);
    }
}

bool JsonObject::detectNumberEnd(int& i) {
    return jsonString->at(i) == ',' || jsonString->at(i) == '}' || isWhiteSpaceOrLine(jsonString->at(i));
}

void JsonObject::init(string* json, int& i, bool isRoot) {
    this->strings = new map<string, string>();
    this->objects = new map<string, JsonObject*>();
    this->arrays = new map<string, JsonArray*>();
    this->nums = new map<string, string>();
    this->bools = new map<string, bool>();
    this->nulls = new map<string, void*>();

    this->jsonString = json;

    if(this->jsonString->length() < 1) {
        throw InvalidJsonException("JSON Object cannot be empty");
    }
    if(!consumeWhiteSpace(i)) {
        throwUnexpectedEndException();
    }
    parseObject(i, isRoot);
}

JsonObject::JsonObject(string* json, int& i, bool isRoot) {
    this->i = i;
    init(json, i, isRoot);
}

JsonObject::JsonObject(string* json) {
    this->i = 0;
    init(json, i, true);
}
