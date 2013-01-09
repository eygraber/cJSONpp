#ifndef UTILS_H
#define UTILS_H
#include<sstream>
using namespace std;

namespace cJSONpp {
    string itoa(int value);
    string ltoa(long value);
    string ftoa(float value);
    string dtoa(double value);

    template <class in_value>
    string convert(const in_value & t) {
        string s;
        { ostringstream ss;
        ss << t;
        s = ss.str();
        }
        return s;
    }
}
#endif
