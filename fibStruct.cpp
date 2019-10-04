#include <iostream>
using namespace std;

template <int n>
struct Fib{
    static constexpr int value = Fib<n-1>::value + Fib<n-2>::value;
};

///This is unfortunately required for C++11/14
template<>
struct Fib<1>{
    static constexpr int value = 1;
};

template <>
struct Fib<0>{
    static constexpr  int value = 0;
};

///Another Implementation using lambda that relies on C++17 tricks on constexpr if
template <int n>
struct FibConstexprIf{
    static constexpr int value = [](){
        if constexpr (n > 1) {
            return FibConstexprIf<n-1>::value + FibConstexprIf<n-2>::value;
        } else {
            return n;
        }
    }();
};

int main() {
    cout << "Fib(30) = " << Fib<30>::value << endl;
    cout << "FibConstexprIf(30) = " << FibConstexprIf<30>::value << endl;
}
