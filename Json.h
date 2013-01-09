#ifndef JSON_H
#define JSON_H

#include<stdexcept>
#include<map>

#include "Utils.h"

using namespace std;

class InvalidJsonException : public runtime_error{
    public:
        InvalidJsonException() : runtime_error("InvalidJsonException::The JSON was malformed!") {}
        InvalidJsonException(string msg) : runtime_error("InvalidJsonException::" + msg + "!") {}
        InvalidJsonException(string msg, int position) : runtime_error("InvalidJsonException::" + msg + " at position " + cJSONpp::itoa(position) + "!"){}
};

class JsonKeyNotFoundException : public runtime_error{
    public:
        JsonKeyNotFoundException(string key) : runtime_error(string("The key ") + key + string(" was not found!")) {}
        JsonKeyNotFoundException(int key) : runtime_error(string("The index ") + cJSONpp::itoa(key) + string(" was not found!")) {}
};

//forward declarations
class JsonArray;
class JsonObject;

class JsonType{
    protected:
        string* jsonString;
        int i;

        void throwInvalidCharacterException(int i);
        void throwInvalidEscapeCodeException(int i);
        void throwUnexpectedEndException();
        bool getNextToken(int& i);
        bool isEscapeCode(int& i);
        bool isUnicodeEscapeCode(int& i);
        bool isWhiteSpaceOrLine(char c);
        bool consumeWhiteSpace(int& i);
        //void parseEscapeCode(string& s, int &i);
        //void parseUnicodeEscapeCode(string& s, int &i);
        void parseObject(int& i, bool root);
        void parseArray(int& i, bool root);
        string parseNumber(int& i);
        bool isDigit(char c, int lowerBound = 48, int upperBound = 57);
        bool parseDigit(int& i);
        virtual bool detectNumberEnd(int& i) = 0;
        virtual void parseValue(int& i, string* name, bool isRoot) = 0;
        virtual void parseValue(int& i, bool isRoot) = 0;
        void* parseNull(int& i);
        bool parseBoolean(int& i);
        string parseName(int& i);
        string parseString(int& i);

        JsonType(){}

    public:
        string getJsonString(){return *jsonString;}
};

class JsonObject : public JsonType {
    friend class JsonArray;
    public:
        JsonObject(string* json);

        string getString(string key);
        int getInt(string key);
        long getLong(string key);
        float getFloat(string key);
        double getDouble(string key);
        bool getBoolean(string key);
        void* getNull(string key);
        JsonObject* getJsonObject(string key);
        JsonArray* getJsonArray(string key);

        void addString(string name, string value);
        void addObject(string name, JsonObject* value);
        void addArray(string name, JsonArray* value);
        void addNumber(string name, string value);
        void addInt(string name, int value);
        void addLong(string name, long value);
        void addFloat(string name, float value);
        void addDouble(string name, double value);
        void addBoolean(string name, bool value);
        void addNull(string name, void* value);

        bool isEmpty(){return strings->empty() && objects->empty() && arrays->empty();}

    private:
        JsonObject(string* json, int& i, bool isRoot);

        void init(string* json, int& i, bool isRoot);

        bool detectNumberEnd(int& i);

        void parseValue(int& i, string* name, bool isRoot);
        void parseValue(int& i, bool isRoot){}

        map<string, string>* strings;
        map<string, JsonObject*>* objects;
        map<string, JsonArray*>* arrays;
        map<string, string>* nums;
        map<string, bool>* bools;
        map<string, void*>* nulls;

        void addString(string* name, string value);
        void addObject(string* name, JsonObject* value);
        void addArray(string* name, JsonArray* value);
        void addNumber(string* name, string value);
        void addBoolean(string* name, bool value);
        void addNull(string* name, void* value);
};

class JsonArray : public JsonType {
    friend class JsonObject;
    public:
        JsonArray(string* jsonString);

        string getString(int key);
        int getInt(int key);
        long getLong(int key);
        float getFloat(int key);
        double getDouble(int key);
        bool getBoolean(int key);
        void* getNull(int key);
        JsonObject* getJsonObject(int key);
        JsonArray* getJsonArray(int key);

        void addString(string value);
        void addObject(JsonObject* value);
        void addArray(JsonArray* value);
        void addNumber(string value);
        void addInt(int value);
        void addLong(long value);
        void addFloat(float value);
        void addDouble(double value);
        void addBoolean(bool value);
        void addNull(void* value);

        bool isEmpty(){return strings->empty() && objects->empty() && arrays->empty();}

    private:
        JsonArray(string* json, int& i, bool isRoot);

        int index;

        void init(string* json, int& i, bool isRoot);

        bool detectNumberEnd(int& i);

        void parseValue(int& i, string* name, bool isRoot){}
        void parseValue(int& i, bool isRoot);

        map<int, string>* strings;
        map<int, JsonObject*>* objects;
        map<int, JsonArray*>* arrays;
        map<int, string>* nums;
        map<int, bool>* bools;
        map<int, void*>* nulls;
};
#endif
