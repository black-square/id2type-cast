#pragma once
#include "./../id2type_cast/id2type_cast.hpp"
#include <iosfwd>
#include <limits>

namespace poly_serialization
{

namespace detail
{
    ///////////////////////////////////////////////////////////////////////////////
    template<class DerivedT>
    struct serializer_base
    {
        template< class T >
        DerivedT &operator&( T &t )
        {
            DerivedT *const pThis = static_cast<DerivedT *>(this);
            pThis->process(t);
            return *pThis;
        }

        template< class T >
        DerivedT &operator&( const T &t )
        {
            DerivedT *const pThis = static_cast<DerivedT *>(this);
            pThis->process(t);
            return *pThis;
        }

        template< class T>
        DerivedT &operator&( T *&t )
        {
            DerivedT *const pThis = static_cast<DerivedT *>(this);
            pointer_helper( t, i2tc::detail::int2type<i2tc::detail::is_embedded_type_id<T>::value>() );
            return *pThis;
        }

    private:
        template<class T>
        void pointer_helper( T *&t, i2tc::detail::int2type<false> )
        {
            static_cast<DerivedT *>(this)->process_pointer_regular(t);
        }

        template<class T>
        void pointer_helper( T *&t, i2tc::detail::int2type<true> )
        {
            static_cast<DerivedT *>(this)->process_pointer_poly(t);
        }
    };
    ///////////////////////////////////////////////////////////////////////////////

    template<class DerivedT>
    struct serializer_write_base: serializer_base<DerivedT>
    {
        template< class T>
        void process_pointer_regular( T *&pT )
        {
            DerivedT &archive = static_cast<DerivedT &>(*this);

            if( pT != 0 )
            {
                char nullTag = 'G';
                archive & nullTag;
                pT->Serialize( archive );
            }
            else
            {
                char nullTag = 'N';
                archive & nullTag;
            }
        }

        template< class T>
        void process_pointer_poly( T *&pT )
        {
            DerivedT &archive = static_cast<DerivedT &>(*this);

            if( pT != 0 )
            {
                archive & pT->i2tc_get_id();
                i2tc::id2type_cast( pT, serializer_write_visitor(archive) );
            }
            else
            {
                int typeTag = -1;
                archive & typeTag;
            } 
        }

    private:
        struct serializer_write_visitor: i2tc::visitor_base<>
        {
            DerivedT &m_archive;
            serializer_write_visitor( DerivedT &archive ): m_archive(archive) {}

            template<class T>
            void exec( T *t ) const
            {
                t->Serialize( m_archive );
            }

        private:
            serializer_write_base &operator=( const serializer_write_visitor & );
        };
    };
    ///////////////////////////////////////////////////////////////////////////////
    
    template<class DerivedT>
    struct serializer_read_base: serializer_base<DerivedT>
    {
        template< class T>
        void process_pointer_regular( T *&pT )
        {
            DerivedT &archive = static_cast<DerivedT &>(*this);
            char nullTag;

            archive & nullTag;

            if( nullTag == 'G' )
            {
                if( pT == 0 ) pT = new T;
                pT->Serialize(archive);
            }
            else
            {
                delete pT;
                pT = 0;
            }
        }

        template< class T>
        void process_pointer_poly( T *&pT )
        {
            DerivedT &archive = static_cast<DerivedT &>(*this);
            int typeTag = 0;

            archive & typeTag;

            if( typeTag >= 0 )
            {
                i2tc::id2type_cast<typename T::i2tc_type_list>( typeTag, serialization_visitor_read<T>(archive, pT), pT ); 
            }
            else
            {
                delete pT;
                pT = 0;
            }
        }

    private:
        template < class DstT >
        struct creation_visitor: i2tc::visitor_base<DstT *>
        {
            template<class RealT>
            static DstT *createImpl( RealT *pCur, i2tc::detail::int2type<true> )
            {
                return pCur;
            }

            template<class RealT>
            static DstT *createImpl( RealT *pCur, i2tc::detail::int2type<false> )
            {
                delete pCur;    
                return new DstT;
            }

            template<class RealT>
            DstT *exec( RealT *pCur ) const
            {
                return createImpl( pCur, i2tc::detail::int2type<i2tc::detail::is_same_type<DstT, RealT>::value>() );
            }
        };

        template< class TCurr >
        struct serialization_visitor_read: i2tc::visitor_base<>
        {
            DerivedT &m_archive;
            TCurr *&m_pCur;
            serialization_visitor_read( DerivedT &archive, TCurr *&pCur ): m_archive(archive), m_pCur(pCur) {}

            template<class T>
            void exec( T * ) const
            {
                T * const pNew = m_pCur != 0 ?
                    i2tc::id2type_cast< creation_visitor<T> >( m_pCur ):
                    new T;

                pNew->Serialize( m_archive );
                m_pCur = pNew; 
            }

        private:
            serialization_visitor_read &operator=( const serialization_visitor_read & );
        };
        
        protected:
            ~serializer_read_base() {}
    };
    ///////////////////////////////////////////////////////////////////////////////
}//namespace detail


class reader: public detail::serializer_read_base<reader>
{                           
public:
    reader( std::istream &is ): m_is(is) {}

    template< class T>
    void process( T &t )
    {
        m_is >> t;
    }

private:
    std::istream &m_is;
    reader &operator=( const reader & );
};

class writer: public detail::serializer_write_base<writer>
{
public:
    writer( std::ostream &is ): is(is) {}

    template< class T>
    void process( T &t )
    {
        is << t << ' ';
    }

private:
    std::ostream &is;
    writer &operator=( const writer & );
};

} //namespace PolySerialization

