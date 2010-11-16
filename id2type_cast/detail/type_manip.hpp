#ifndef ID2TYPE_CAST_TYPE_MANIP_HPP
#define ID2TYPE_CAST_TYPE_MANIP_HPP

namespace i2tc { namespace detail {
    ////////////////////////////////////////////////////////////////////////////////
    // Loki::Int2Type
    // boost::mpl::int_
    ////////////////////////////////////////////////////////////////////////////////
    template <int v>
    struct int2type
    {
        enum { value = v };
    };

    ////////////////////////////////////////////////////////////////////////////////
    // Loki::Select
    // boost::mpl::if_c
    ////////////////////////////////////////////////////////////////////////////////
    template <bool flag, typename T, typename U>
    struct select
    {
        typedef T result;
    };
    
    template <typename T, typename U>
    struct select<false, T, U>
    {
        typedef U result;
    };

    ////////////////////////////////////////////////////////////////////////////////
    // Loki::IsSameType
    // boost::is_same
    ////////////////////////////////////////////////////////////////////////////////
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

    ////////////////////////////////////////////////////////////////////////////////
    // Loki::UnConst
    // boost::is_const
    ////////////////////////////////////////////////////////////////////////////////
    template <class U> struct is_const
    {
        enum { value = false };
    };

    template <class U> struct is_const<const U>
    {
        enum { value = true };
    };

    template <class U> struct is_const<const U&>
    {
        enum { value = true };
    };
}}

#endif
