#ifndef ID2TYPE_CAST_TYPE_TABLE_IMPL_HPP
#define ID2TYPE_CAST_TYPE_TABLE_IMPL_HPP

#include <cassert>

namespace i2tc { namespace detail {

template<class Visitor, class TypeList, class Base>
class table_impl
{
public:
    typedef Visitor &visitor_ref;
    typedef typename Visitor::return_value return_value;
    typedef Base *base_type_ptr;

public:
    static return_value cast( id_type n, base_type_ptr type, visitor_ref visitor )
    {
        return (*m_map_data.get_func(n))( visitor, type ); 
    }

private:
    class map_data;
    static const map_data m_map_data;
};
///////////////////////////////////////////////////////////////////////////////

template<class Visitor, class TypeList, class Base >
class table_impl<Visitor, TypeList, Base>::map_data
{
public:
    typedef return_value (*mapped_function)( visitor_ref visitor, base_type_ptr type );
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
    static return_value convert_impl( visitor_ref visitor, base_type_ptr type )
    {
        typedef typename detail::select<detail::is_const<Base>::value, const RealTypeT, RealTypeT>::result cast_type;
        return visitor.exec( static_cast<cast_type *>(type) );
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

template<class Visitor, class TypeList, class Base >
const typename table_impl<Visitor, TypeList, Base>::map_data 
    table_impl<Visitor, TypeList, Base>::m_map_data;

}}

#endif
