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
////////////////////////////////////////////////////


template<class TypeList, int N>
struct at
{
    typedef typename TypeList::template at<N>::type type;
};
////////////////////////////////////////////////////

template<class TypeList, int N>
struct is_valid
{
    typedef typename TypeList::template at<N>::type cur_type;
    enum { value = !is_same_type<cur_type, undefined_tag>::value };
};
////////////////////////////////////////////////////

template<class TypeList, int N, bool StopRecursion>
struct size_impl
{
    enum { is_last_type = !is_valid<TypeList, N>::value };
    enum { value = is_last_type ? N : 
           size_impl<TypeList, N + 1, is_last_type>::value }; 
};

template<class TypeList, int N>
struct size_impl< TypeList, N, true >
{
    enum { value = -1 };
};

template<class TypeList>
struct size
{
    enum { value = size_impl<TypeList, 0, false>::value };
};
////////////////////////////////////////////////////

template<class TypeList, class T, int N, bool StopRecursion>
struct index_of_impl
{
    typedef typename TypeList::template at<N>::type type;

    enum { is_last_type = !is_valid<TypeList, N>::value };
    enum { is_type_found = is_same_type<type, T>::value };
    enum { need_stop_recursion = is_last_type || is_type_found };

    enum { value = is_type_found ? N : index_of_impl<TypeList, T, N + 1, need_stop_recursion>::value };
};

template<class TypeList, class T, int N>
struct index_of_impl<TypeList, T, N, true>
{
    enum { value = -1 };
};

template<class TypeList, class T>
struct index_of
{
    enum { value = index_of_impl<TypeList, T, 0, false>::value };
};
////////////////////////////////////////////////////

int main()
{
    typedef at<DataTypes, 1>::type SecondType;
    const bool exist = is_valid<DataTypes, 3>::value;
    
    std::cout << exist << std::endl;
    std::cout << size<DataTypes>::value << std::endl;
    std::cout << index_of<DataTypes, BData>::value << std::endl;

    return 0;
}