#ifndef ID2TYPE_CAST_TYPE_TABLE_IMPL_HPP
#define ID2TYPE_CAST_TYPE_TABLE_IMPL_HPP

#include <cassert>
#include "./pp_repeat.hpp"

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
        fill_map_impl( detail::int2type<0>(), detail::int2type<types_count>() );
    }

    mapped_function get_func( id_type n ) const
    {
        assert( n >= 0 && n <= types_count );
        assert( *(m_map + n) != 0 );
        
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
         
    static return_value convert_impl_error( functor_ref functor, base_type_ptr type )
    {
        (void)functor;
        (void)type;
        assert(false);
        return return_value();
    }

    template<id_type N>
    void set_item( detail::int2type<true>)
    {
        m_map[N] = &convert_impl< typename type_list::at<TypeList, N>::type >;
    }

    template<id_type N>
    void set_item( detail::int2type<false>)
    {
        m_map[N] = &convert_impl_error;
    }

    template<id_type N, id_type M>
    void fill_map_impl( detail::int2type<N>, detail::int2type<M> )
    {
        fill_map_impl( detail::int2type<N>(), detail::int2type<10>() );
        fill_map_impl( detail::int2type<N + 10>(), detail::int2type<M - 10>() );
    }
    
    #define INT2TYPE_CAST_PP_DEF( arg ) set_item<N + arg>( detail::int2type<type_list::is_valid<TypeList, N + arg>::value>() );
    
    template<id_type N>
    void fill_map_impl( detail::int2type<N>, detail::int2type<0> )
    {
        //Nothing
    }
    
    template<id_type N>
    void fill_map_impl( detail::int2type<N>, detail::int2type<1> )
    {
        ID2TYPE_CAST_PP_REPEAT( 1, INT2TYPE_CAST_PP_DEF )
    }
    
    template<id_type N>
    void fill_map_impl( detail::int2type<N>, detail::int2type<2> )
    {
        ID2TYPE_CAST_PP_REPEAT( 2, INT2TYPE_CAST_PP_DEF )
    }
    
    template<id_type N>
    void fill_map_impl( detail::int2type<N>, detail::int2type<3> )
    {
        ID2TYPE_CAST_PP_REPEAT( 3, INT2TYPE_CAST_PP_DEF )
    }
    
    template<id_type N>
    void fill_map_impl( detail::int2type<N>, detail::int2type<4> )
    {
        ID2TYPE_CAST_PP_REPEAT( 4, INT2TYPE_CAST_PP_DEF )
    }
    
    template<id_type N>
    void fill_map_impl( detail::int2type<N>, detail::int2type<5> )
    {
        ID2TYPE_CAST_PP_REPEAT( 5, INT2TYPE_CAST_PP_DEF )
    }
    
    template<id_type N>
    void fill_map_impl( detail::int2type<N>, detail::int2type<6> )
    {
        ID2TYPE_CAST_PP_REPEAT( 6, INT2TYPE_CAST_PP_DEF )
    }    
    
    template<id_type N>
    void fill_map_impl( detail::int2type<N>, detail::int2type<7> )
    {
        ID2TYPE_CAST_PP_REPEAT( 7, INT2TYPE_CAST_PP_DEF )
    }    
    
    template<id_type N>
    void fill_map_impl( detail::int2type<N>, detail::int2type<8> )
    {
        ID2TYPE_CAST_PP_REPEAT( 8, INT2TYPE_CAST_PP_DEF )
    }    
    
    template<id_type N>
    void fill_map_impl( detail::int2type<N>, detail::int2type<9> )
    {
        ID2TYPE_CAST_PP_REPEAT( 9, INT2TYPE_CAST_PP_DEF )
    }    
    
    template<id_type N>
    void fill_map_impl( detail::int2type<N>, detail::int2type<10> )
    {
        ID2TYPE_CAST_PP_REPEAT( 10, INT2TYPE_CAST_PP_DEF )
    }
    
    #undef INT2TYPE_CAST_PP_DEF

private:
    mapped_function m_map[types_count];
};
///////////////////////////////////////////////////////////////////////////////

template<class Functor, class TypeList, class Base >
const typename table_impl<Functor, TypeList, Base>::map_data 
    table_impl<Functor, TypeList, Base>::m_map_data;

}}

#endif
