//
//

#include <iostream>
#include <string>
using namespace std;

template <typename T>
string add1(T a, T b){
    return a + b;
}


template <typename T>
auto add2(T a, T b) {
    return a + b;
}

template <typename T>
T process(T a) {
    cout << "processing " << a << endl;
    return a;
}

template <typename T>
typename std::enable_if_t<!std::is_same<T, void>::value, T> processIf(T a) {
    cout << "processingif " << a << endl;
    return a;
}

template <typename T>
auto processIf1(T a) {
    static_assert(!std::is_same<T, void>::value);
    cout << "processWithAssert..." << a << endl;
    return a;
}


int main(){
    /// signature of below funciton which is instantiated will be
    ///  std::string add1(std::string, std::string)
    cout << add1(string{"hello"}, string{"world\n"});
    /// signature of below function which is instaintiated will be
    ///    auto add2<std::string>(std::string, std::string)
    cout << add2(string{"hello"}, string{"world\n"});

    auto test = string{"test"};
    /// signature of below function instaintaited would be
    ///  std::string process(std::string)
    process(test);

    /// signature of below function instantiated will be
    ///  std::enable_if<!std::is_same<std::string, void>::value, std::string>::type processIf(std::string)
    processIf(test);
    /// signature: auto processIf<std::string>(std::string)
    processIf1(test);


    /// check the summary by nm xxx | c++filter | grep <name>
    //// 1. inline functions aren't necessarily inlined
    //// 2. constexpr functions are not necessarily inlined usually not memorized
    //// 3. template functions are guaranteed to be memorized but their functions are not guaranteed to be inlined
    //// 4. constexpr if and auto go well with template functions
    //// results may vary...
}

