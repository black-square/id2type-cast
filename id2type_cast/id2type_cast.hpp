#ifndef ID2TYPE_CAST_ID2TYPE_CAST_HPP
#define ID2TYPE_CAST_ID2TYPE_CAST_HPP

#include "./type_list.hpp"
#include "./embedded_type_id.hpp"
#include "./detail/table_impl.hpp"
#include "./detail/switch_impl.hpp"

namespace i2tc
{
    template<class ReturnValue = void>
    struct function
    {
        typedef ReturnValue return_value;
    };
    
    ///////////////////////////////////////////////////////////////////////////////

    struct table_impl_tag 
    {
        template<class Functor, class TypeList, class Base>
        struct rebind { typedef detail::table_impl<Functor, TypeList, Base> type; };
    };

    struct switch_impl_tag 
    {
        template<class Functor, class TypeList, class Base>
        struct rebind { typedef detail::switch_impl<Functor, TypeList, Base> type; };
    };
    
    namespace type_list
    {
        template<class ImplTag = switch_impl_tag>
        struct base
        {
            typedef ImplTag id2type_cast_impl_tag;
        };
    }

    ///////////////////////////////////////////////////////////////////////////////

    template<class TypeList, class Functor, class Base>
    typename Functor::return_value id2type_cast( id_type n, Functor &functor, Base *type )
    {
        typedef typename TypeList::id2type_cast_impl_tag::template rebind<Functor, TypeList, Base>::type impl_type;
        return impl_type::cast( n, type, functor ); 
    }

    template<class TypeList, class Functor, class Base>
    typename Functor::return_value id2type_cast( id_type n, const Functor &functor, Base *type )
    {
        typedef typename TypeList::id2type_cast_impl_tag::template rebind<const Functor, TypeList, Base>::type impl_type;
        return impl_type::cast( n, type, functor ); 
    }

    template<class TypeList, class Functor>
    typename Functor::return_value id2type_cast( id_type n, Functor &functor )
    {
        return id2type_cast<TypeList>( n, functor, static_cast<const void *>(0) ); 
    }

    template<class TypeList, class Functor>
    typename Functor::return_value id2type_cast( id_type n, const Functor &functor )
    {
        return id2type_cast<TypeList>( n, functor, static_cast<const void *>(0) ); 
    }

    template<class TypeList, class Functor, class Base>
    typename Functor::return_value id2type_cast( id_type n, Base *type )
    {
        const Functor functor = Functor();
        return id2type_cast<TypeList>( n, functor, type ); 
    }

    template<class TypeList, class Functor>
    typename Functor::return_value id2type_cast( id_type n )
    {
        const Functor functor = Functor();
        return id2type_cast<TypeList>( n, functor, static_cast<const void *>(0) ); 
    }
}

#endif
