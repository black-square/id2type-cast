#ifndef ID2TYPE_CAST_ID2TYPE_CAST_HPP
#define ID2TYPE_CAST_ID2TYPE_CAST_HPP

#include "./type_list.hpp"
#include "./embedded_type_id.hpp"
#include "./detail/table_impl.hpp"
#include "./detail/switch_impl.hpp"

namespace i2tc
{
    template<class ReturnValue = void>
    struct visitor_base
    {
        typedef ReturnValue return_value;
    };
    
    struct table_impl_tag 
    {
        template<class Visitor, class TypeList, class Base>
        struct rebind { typedef detail::table_impl<Visitor, TypeList, Base> type; };
    };

    struct switch_impl_tag 
    {
        template<class Visitor, class TypeList, class Base>
        struct rebind { typedef detail::switch_impl<Visitor, TypeList, Base> type; };
    };
    
    namespace type_list
    {
        template<class ImplTag = switch_impl_tag>
        struct base
        {
            typedef ImplTag id2type_cast_impl_tag;
        };
    }

    template<class TypeList, class Visitor, class Base>
    typename Visitor::return_value id2type_cast( id_type n, Visitor &visitor, Base *type )
    {
        typedef typename TypeList::id2type_cast_impl_tag::template rebind<Visitor, TypeList, Base>::type impl_type;
        return impl_type::cast( n, type, visitor ); 
    }

    template<class TypeList, class Visitor, class Base>
    typename Visitor::return_value id2type_cast( id_type n, const Visitor &visitor, Base *type )
    {
        typedef typename TypeList::id2type_cast_impl_tag::template rebind<const Visitor, TypeList, Base>::type impl_type;
        return impl_type::cast( n, type, visitor ); 
    }

    template<class TypeList, class Visitor>
    typename Visitor::return_value id2type_cast( id_type n, const Visitor &visitor )
    {
        return id2type_cast<TypeList>( n, visitor, static_cast<const void *>(0) ); 
    }

    template<class TypeList, class Visitor, class Base>
    typename Visitor::return_value id2type_cast( id_type n, Base *type )
    {
        const Visitor visitor = Visitor();
        return id2type_cast<TypeList>( n, visitor, type ); 
    }

    template<class TypeList, class Visitor>
    typename Visitor::return_value id2type_cast( id_type n )
    {
        const Visitor visitor = Visitor();
        return id2type_cast<TypeList>( n, visitor, static_cast<const void *>(0) ); 
    }
}

#endif
