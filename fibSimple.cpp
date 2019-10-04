//
// Created by Yan Fei on 19-10-3.
//

#include <iostream>
using namespace std;

constexpr int fib(int n) {
    return n <= 1 ? n : fib(n - 1) + fib(n - 2);
}

int main() {
    cout << "For n = " << 30 << ", fib(n) = " << fib(30) << endl;
    return 0;
}
