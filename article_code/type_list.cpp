#include <iostream>

template <typename T, typename U>
struct is_same_type
{
    enum { value = false };
};

template <typename T>
struct is_same_type<T,T>
{
    enum { value = true };
};


struct AData {};
struct BData {};
struct CData {};

struct DataTypes
{
    template <int N> struct at;
};

struct undefined_tag {};
template <int N> struct DataTypes::at{ typedef undefined_tag type; };

template <> struct DataTypes::at<0>{ typedef AData type; };
template <> struct DataTypes::at<1>{ typedef BData type; };
template <> struct DataTypes::at<2>{ typedef CData type; };

template<class TypeList, int N>
struct at
{
    typedef typename TypeList::template at<N>::type type;
};

template<class TypeList, int N>
struct is_valid
{
    typedef typename TypeList::template at<N>::type cur_type;
    enum { value = !is_same_type<cur_type, undefined_tag>::value };
};


int main()
{
    typedef at<DataTypes, 1>::type SecondType;
    const bool exist = is_valid<DataTypes, 3>::value;

    return 0;
}