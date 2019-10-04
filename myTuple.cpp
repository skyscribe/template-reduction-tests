//
//

#include <string>
#include <iostream>
#include <tuple>
using namespace std;

/// Simple yet naive tuple implemenation with O(N^3) code generation!
template <typename... T> class MyTuple;

template <typename T>
class MyTuple<T> {
    T first;
    template <size_t  N, typename TN> friend class Getter;
};

template <typename T, typename... More>
class MyTuple<T, More...>{
    T first;
    MyTuple<More...> more;

    template<size_t  N, typename TN> friend class Getter;
};

template<size_t N, typename TN>
struct Getter{
    template<typename T0, typename... MoreN>
    static TN& get(MyTuple<T0, MoreN...>&tup) {
        return Getter<N - 1, TN>::get(tup.more);
    }
};

template<typename T0>
struct Getter<0, T0>{
    template<typename... Ts>
    static T0& get(MyTuple<Ts...>& tup) {
        return tup.first;
    }
};

///For now we have to rely on std::tuple_element to deduct the type!
template <size_t N, typename... More>
auto get(MyTuple<More...>& tup) -> typename std::tuple_element<N, std::tuple<More...>>::type& {
    return Getter<N, typename std::tuple_element<N, std::tuple<More...>>::type>::get(tup);
}

void testMyTuple() {
    using T = MyTuple<bool, string, int, char, double>;
    T tup;
    get<0>(tup) = true;
    get<1>(tup) = "somthing";
    get<2>(tup) = 1;
    get<3>(tup) = 'c';
    get<4>(tup) = 12.34;
    ///check generated symbols, now it is O(N^3) for those
    /// Getter, Getter::get, std::tuple_element, MyTuple...
    /// Example of Getter::get
    /// 1. bool& Getter<0ul, bool>::get<string, int, char, double>(T&)
    /// 2. string& Getter<0ul, string>::get<string, int ,char, double>(MyTuple<string, int, char, double>&)
    /// 3. int& Getter<0ul, int>::get<int, char, double>(MyTuple<int, char, double>&)
    /// 4. char& Getter<0ul, char>::get<char, double>(MyTuple<char, double>&)
    /// 5. double& Getter<0ul, double>::get<double>(MyTuple<double>&)
    /// 6. string& Getter<1ul, string, int, char, double>::get<bool, string, int, char, double>(MyTuple<bool, string, int, char, double>&)
    /// 7. int& Getter<1ul, int>::get<string, int, char, double>(MyTuple<string, int, char, double>&)
    ///...
}

/// Using new tricks
template <typename... T>
class NewTuple {
    //serves as a base type only
    public:
        void get(...) = delete;
};

template <typename T, typename... More>
class NewTuple<T, More...> : NewTuple<More...> {
    T first;

public:
    using NewTuple<More...>::get;
    auto& get(std::integral_constant<size_t, sizeof...(More)>) {return first;}
};

template<size_t N, typename... More>
auto& get(NewTuple<More...>& tup) {
    return tup.get(std::integral_constant<size_t, sizeof...(More) - 1 - N>{});
}

void testNewTuple() {
    using T = NewTuple<bool, string, int, char, double>;
    T tup;
    get<0>(tup) = true;
    get<1>(tup) = "somthing";
    get<2>(tup) = 1;
    get<3>(tup) = 'c';
    get<4>(tup) = 12.34;

    /// This is O(N^2) now due to the inheritance
}

/// Using leaf
template <std::size_t Index, typename T>
struct MyTupleLeaf{
    T value_;
    //    template <typename... More>
    //    constexpr MyTupleLeaf(More&&... arg) : value_(std::forward<More>(arg)...) {}

    T& get(std::integral_constant<size_t, Index>) {return value_;}
};

template <typename... Ts> class NNTupleImpl;

template <std::size_t... Indexes, typename... Ts>
class NNTupleImpl<std::index_sequence<Indexes...>, Ts...>: public MyTupleLeaf<Indexes, Ts>...  {
    // This is C++17 only
    using MyTupleLeaf<Indexes, Ts>::get...;

public:
    template <size_t I>
    auto& get() {
        return get(std::integral_constant<size_t, I>{});
    };

    //C++14 version of get
    template <size_t I, typename FType>
    auto& get14() {
        MyTupleLeaf<I, FType>& leaf = *this;
        return leaf.get(std::integral_constant<size_t, I>{});
    }

    //We can even deduce its type if we pass in the parameter
    template <size_t I, typename FType>
    auto& getLeaf(MyTupleLeaf<I, FType>* leaf) {return *leaf;}

    template <size_t I>
    auto& get14_1() {
        auto& leaf = getLeaf<I>(this);
        return leaf.get(std::integral_constant<size_t, I>{});
    }
};

/// Rename it to make it more eaiser to use
template <typename... Ts>
using NNTuple = NNTupleImpl<std::make_index_sequence<sizeof...(Ts)>, Ts...>;

void testNNTuple(){
    using T = NNTuple<bool, string, int, char, double>;
    T t;
    t.get<0>() = true;
    t.get<1>() = "something";
    t.get<2>() = 1;
    t.get14<3, char>() = 'c';
    t.get14_1<4>() = 12.34;
}

////////////////////////////////////////////////////////////////////////////////////////////
/// The main

int main(){
    testMyTuple();
}