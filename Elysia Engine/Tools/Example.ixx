// Example.ixx
#include <stdafx.h>
export module Example;

#define ANSWER 42

namespace Example_NS
{
    int f_internal() {
        return ANSWER;
    }

    export int f() {
        return f_internal();
    }
}