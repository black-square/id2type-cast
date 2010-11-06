#ifndef ID2TYPE_CAST_TYPE_LIST_HPP
#define ID2TYPE_CAST_TYPE_LIST_HPP

#include "./detail/type_manip.hpp"

///////////////////////////////////////////////////////////////////////////////
//  boost::mpl::vector
//      define BOOST_MPL_LIMIT_VECTOR_SIZE 20
//
//  Loki::Typelist
//      max define LOKI_TYPELIST_50
///////////////////////////////////////////////////////////////////////////////

namespace i2tc { 
    
typedef int id_type;

namespace type_list {

struct finish_tag {};

namespace detail
{
    struct base
    {
        typedef finish_tag size;
    };
}

///////////////////////////////////////////////////////////////////////////////
// boost:mpl::at
///////////////////////////////////////////////////////////////////////////////
template<class TypeList, id_type N>
struct at
{
    typedef typename TypeList::template at<N>::type type;
};
///////////////////////////////////////////////////////////////////////////////

template<class TypeList, id_type N>
struct is_valid
{
    typedef typename TypeList::template at<N>::type cur_type;
    enum { value = !i2tc::detail::is_same_type<cur_type, finish_tag>::value };
};

///////////////////////////////////////////////////////////////////////////////
// boost:mpl::size
///////////////////////////////////////////////////////////////////////////////
namespace detail
{
    template<class TypeList, id_type N, bool StopRecursion>
    struct size_impl
    {
        typedef typename TypeList::template at<N>::type cur_type;
        enum { is_last_type = i2tc::detail::is_same_type<cur_type, finish_tag>::value };
        enum { value = is_last_type ? N : size_impl<TypeList, N + 1, is_last_type>::value }; 
    };

    template<class TypeList, id_type N>
    struct size_impl< TypeList, N, true >
    {
        enum { value = 0 };
    };
    
    template<class TypeList, bool HasEmbededSize>
    struct size
    {
        enum { value = detail::size_impl<TypeList, 0, false>::value };
    };
    
    template<class TypeList>
    struct size< TypeList, true >
    {
        enum { value = TypeList::size::value };
    };    
}

template<class TypeList>
struct size
{
    enum { has_embeded_size = !i2tc::detail::is_same_type<typename TypeList::size, finish_tag>::value };    
    enum { value = detail::size<TypeList, has_embeded_size>::value };
};

///////////////////////////////////////////////////////////////////////////////
// boost::mpl::find
///////////////////////////////////////////////////////////////////////////////
namespace detail
{
    template<class TypeList, class T, id_type N, bool StopRecursion>
    struct index_of_impl
    {
        typedef typename TypeList::template at<N>::type type;

        enum { is_last_type = i2tc::detail::is_same_type<type, finish_tag>::value };
        enum { is_type_finished = i2tc::detail::is_same_type<type, T>::value };
        enum { need_stop_recursion = is_last_type || is_type_finished };

        enum { value = is_type_finished ? N : index_of_impl<TypeList, T, N + 1, need_stop_recursion>::value };
    };

    template<class TypeList, class T, id_type N>
    struct index_of_impl<TypeList, T, N, true>
    {
        enum { value = -1 };
    };
}

template<class TypeList, class T>
struct index_of
{
    enum { value = detail::index_of_impl<TypeList, T, 0, false>::value };
};

}}

#endif
