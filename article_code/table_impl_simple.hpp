#ifndef ID2TYPE_CAST_TYPE_TABLE_IMPL_HPP
#define ID2TYPE_CAST_TYPE_TABLE_IMPL_HPP

#include <cassert>

namespace i2tc { namespace detail {

template<class Functor, class TypeList, class Base>
class table_impl
{
public:
    typedef Functor &functor_ref;
    typedef typename Functor::return_value return_value;
    typedef Base *base_type_ptr;

public:
    static return_value cast( id_type n, base_type_ptr type, functor_ref functor )
    {
        return (*m_map_data.get_func(n))( functor, type ); 
    }

private:
    class map_data;
    static const map_data m_map_data;
};
///////////////////////////////////////////////////////////////////////////////

template<class Functor, class TypeList, class Base >
class table_impl<Functor, TypeList, Base>::map_data
{
public:
    typedef return_value (*mapped_function)( functor_ref functor, base_type_ptr type );
    enum { types_count = type_list::size<TypeList>::value };
public:
    map_data()
    {
        fill_map_impl( detail::int2type<0>() );
    }

    mapped_function get_func( id_type n ) const
    {
        assert( n >= 0 && n <= types_count );

        return *(m_map + n); //Not a m_map[n] for suppress warning C6385: Invalid data: accessing 'm_map', 
                             //the readable size is '12' bytes, but '-8' bytes might be read
    }

private:
    template<class RealTypeT>
    static return_value convert_impl( functor_ref functor, base_type_ptr type )
    {
        typedef typename detail::select<detail::is_const<Base>::value, const RealTypeT, RealTypeT>::result cast_type;
        return functor( static_cast<cast_type *>(type) );
    }

    template<id_type N>
    void fill_map_impl( detail::int2type<N> )
    {
        typedef typename type_list::at<TypeList, N>::type TypeN;
        m_map[N] = &convert_impl<TypeN>; 
        fill_map_impl( detail::int2type<N + 1>() );
    }
    
    void fill_map_impl( detail::int2type<types_count> )
    {
        //Nothing   
    }

private:
    mapped_function m_map[types_count];
};
///////////////////////////////////////////////////////////////////////////////

template<class Functor, class TypeList, class Base >
const typename table_impl<Functor, TypeList, Base>::map_data 
    table_impl<Functor, TypeList, Base>::m_map_data;

}}

#endif
