#include <string>
#include <vector>

template <typename T, typename U>
class Bar {
    public:
    std::vector<T> vec;
    U a;
};

class Foo {
    public:
    Bar<double, int> b1;
    Bar<int, double> b2;

};

