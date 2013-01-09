#include "Utils.h"
using namespace std;

string cJSONpp::itoa(int value) {
    return convert<int>(value);
}

string cJSONpp::ltoa(long value) {
    return convert<long>(value);
}

string cJSONpp::ftoa(float value) {
    return convert<float>(value);
}

string cJSONpp::dtoa(double value) {
    return convert<double>(value);
}

/*
string cJSONpp::itoa(int value) {
    string dig = "0123456789";
    int neg = 0;
    unsigned int v;

    if(value < 0){
        value = -value;
        neg = 1;
    }

    v = value;

    string s = "";

    do{
        s += dig[v%10];
        v /= 10;
    } while (v);

    if(neg)
        s += "-";

    for(int i = 0; i < s.length() / 2; i++){
        int j = (s.length() - 1) - i;
        char t = s[i];
        s[i] = s[j];
        s[j] = t;
    }

    return s;
}
*/
