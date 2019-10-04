//
// Trick with constexpr on template parameter
//

#include <iostream>
using namespace std;

template <int N>
constexpr auto fib() {
    if constexpr (N>1) {
        return fib<N-1>() + fib<N-2>();
    } else {
        return N;
    }
}

int main() {
    cout << "Fib(30)=" << fib<30>() << endl;
}


