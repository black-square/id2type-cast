#ifndef ID2TYPE_CAST_EMBEDDED_TYPE_ID_HPP
#define ID2TYPE_CAST_EMBEDDED_TYPE_ID_HPP

#include <cassert>
#include <limits>

namespace i2tc
{

struct virt_func_tag {};
struct member_tag {};

template<class TypeList, class ImplTag = virt_func_tag>
class embedded_type_id_base;

template<class Derived, class Base, class ImplTag = virt_func_tag>
class embedded_type_id_derived;

///////////////////////////////////////////////////////////////////////////////

template<class TypeList>
class embedded_type_id_base <TypeList, member_tag>
{
public:
    typedef TypeList i2tc_type_list;
    id_type i2tc_get_id() const { return m_typeId; }

protected:
    embedded_type_id_base(): m_typeId( std::numeric_limits<id_type>::min() ) {}
    ~embedded_type_id_base() { assert( m_typeId != std::numeric_limits<id_type>::min() ); }

private:
    template<class Derived, class Base, class ImplTag>
    friend class embedded_type_id_derived;

    id_type m_typeId;
};
///////////////////////////////////////////////////////////////////////////////

template< class Derived, class Base >
class embedded_type_id_derived<Derived, Base, member_tag>: public Base
{
public:
    embedded_type_id_derived() 
    { 
        static_cast<Derived *>(this)->m_typeId = i2tc::type_list::index_of<typename Derived::i2tc_type_list, Derived>::value; 
    }
};
///////////////////////////////////////////////////////////////////////////////

template<class TypeList>
class embedded_type_id_base <TypeList, virt_func_tag>
{
public: 
    typedef TypeList i2tc_type_list;
    virtual id_type i2tc_get_id() const = 0;

protected:
    embedded_type_id_base() {}
    ~embedded_type_id_base() {} 
};
///////////////////////////////////////////////////////////////////////////////

template< class Derived, class Base >
class embedded_type_id_derived<Derived, Base, virt_func_tag>: public Base
{
public:
    id_type i2tc_get_id() const 
    { 
        return i2tc::type_list::index_of<typename Derived::i2tc_type_list, Derived>::value; 
    }
};
///////////////////////////////////////////////////////////////////////////////

namespace detail
{
    ///////////////////////////////////////////////////////////////////////////////
    //  Loki::Conversion
    ///////////////////////////////////////////////////////////////////////////////
    template< class T>
    struct is_embedded_type_id
    {
        struct helper
        {
            struct yes { char a; };
            struct no { yes a[2]; };
            static no test(...);
            template<class TypeList, class ImplTag> static yes test( const embedded_type_id_base<TypeList, ImplTag> * );
            static T *make_t();
        };

        enum{ value = sizeof(typename helper::yes) == sizeof((helper::test(helper::make_t()))) };
    };
}
///////////////////////////////////////////////////////////////////////////////

template<class Functor, class TypeList, class ImplTag>
typename Functor::return_value id2type_cast( embedded_type_id_base<TypeList, ImplTag> *type, Functor &functor )
{
    assert( type != 0 );
    return id2type_cast<TypeList>( type->i2tc_get_id(), functor, type ); 
}
///////////////////////////////////////////////////////////////////////////////

template<class Functor, class TypeList, class ImplTag>
typename Functor::return_value id2type_cast( const embedded_type_id_base<TypeList, ImplTag> *type, const Functor &functor )
{           
    assert( type != 0 );
    return id2type_cast<TypeList>( type->i2tc_get_id(), functor, type ); 
}
///////////////////////////////////////////////////////////////////////////////

template<class Functor, class TypeList, class ImplTag>
typename Functor::return_value id2type_cast( embedded_type_id_base<TypeList, ImplTag> *type, const Functor &functor )
{           
    assert( type != 0 );
    return id2type_cast<TypeList>( type->i2tc_get_id(), functor, type ); 
}
///////////////////////////////////////////////////////////////////////////////

template<class Functor, class TypeList, class ImplTag>
typename Functor::return_value id2type_cast( const embedded_type_id_base<TypeList, ImplTag> *type, Functor &functor )
{           
    assert( type != 0 );
    return id2type_cast<TypeList>( type->i2tc_get_id(), functor, type ); 
}
///////////////////////////////////////////////////////////////////////////////

template<class Functor, class TypeList, class ImplTag>
typename Functor::return_value id2type_cast( embedded_type_id_base<TypeList, ImplTag> *type )
{
    assert( type != 0 );
    return id2type_cast<TypeList, Functor>( type->i2tc_get_id(), type ); 
}
///////////////////////////////////////////////////////////////////////////////

template<class Functor, class TypeList, class ImplTag>
typename Functor::return_value id2type_cast( const embedded_type_id_base<TypeList, ImplTag> *type )
{
    assert( type != 0 );
    return id2type_cast<TypeList, Functor>( type->i2tc_get_id(), type ); 
}

}

#endif
