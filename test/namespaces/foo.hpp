
#include <string>

using std::string;

class Foo {
    public:
    int x;
};

namespace jackie {
    class Foo {
        public:
            string my_string;
            int * my_ptr;
    };
}
