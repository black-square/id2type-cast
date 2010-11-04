#ifndef ID2TYPE_CAST_TYPE_SWITCH_IMPL_HPP
#define ID2TYPE_CAST_TYPE_SWITCH_IMPL_HPP

#include <cassert>
#include "./pp_repeat.hpp"


#ifndef ID2TYPE_CAST_SWICTH_IMPL_MAX_TYPES
    #define ID2TYPE_CAST_SWICTH_IMPL_MAX_TYPES 100
#endif

namespace i2tc { namespace detail {

template<class Visitor, class TypeList, class Base>
class switch_impl
{
public:
    typedef Visitor &visitor_ref;
    typedef typename Visitor::return_value return_value;
    typedef Base *base_type_ptr;
    enum { types_count = type_list::size<TypeList>::value };

    static return_value cast( id_type n, base_type_ptr type, visitor_ref visitor )
    {
        assert( n >= 0 && n <= types_count );

        #define INT2TYPE_CAST_PP_DEF( n ) case (n): return cast_impl<(n)>( visitor, type );                                     
                                          
        switch(n)
        {
            ID2TYPE_CAST_PP_REPEAT( ID2TYPE_CAST_SWICTH_IMPL_MAX_TYPES, INT2TYPE_CAST_PP_DEF )

        default:
            return error_func(); 
        }

        #undef INT2TYPE_CAST_PP_DEF
    }

private:
    static return_value error_func()
    {
        return return_value();
    }

    template<id_type N>
    static return_value cast_impl_impl( visitor_ref visitor, base_type_ptr type, detail::int2type<true> )
    {
        typedef typename type_list::at<TypeList, N>::type list_type;
        typedef typename detail::select<detail::is_const<Base>::value, const list_type, list_type>::result cast_type;
        return visitor.exec( static_cast<cast_type *>(type) );
    }

    template<id_type N>
    static return_value cast_impl_impl( visitor_ref visitor, base_type_ptr type, detail::int2type<false> )
    {
        (void)visitor;
        (void)type;
        return error_func();
    }

    template<id_type N>
    static return_value cast_impl( visitor_ref visitor, base_type_ptr type )
    {
        return cast_impl_impl<N>( visitor, type, 
            typename detail::select<
                (N < types_count), 
                detail::int2type<true>, 
                detail::int2type<false> 
            >::result() 
       );
    }                                      
};

}}

#endif
