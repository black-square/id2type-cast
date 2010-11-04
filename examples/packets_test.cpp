#include "./../id2type_cast/id2type_cast.hpp"

#include <iostream>
#include <string>
#include <ctime>
#include <typeinfo>

using i2tc::id2type_cast;

enum Packets
{
    HELLO,
    MOVE_UNIT,
    CREATE_BUILDING,
    COUNT
};


template<int N>
class Packet;

template<int N>
int GetNumber( Packet<N> * ) { return N; }

template<int N>
int GetNumber( const Packet<N> * ) { return N; }

template<> struct Packet<HELLO>
{
    int a;

    void print() const
    {
        std::cout << "Packet " << GetNumber(this) << ": " << typeid(*this).name() << std::endl;
    }

    int check() const
    {
        return GetNumber(this);
    }

    Packet()
    {
        std::cout << "Packet<" << GetNumber(this) << ">()" << std::endl;
    }

    ~Packet()
    {
        std::cout << "~Packet<" << GetNumber(this) << ">()" << std::endl;
    }
};

template<> struct Packet<MOVE_UNIT>
{
    int a;
    float b;
    std::string c;

    void print() const
    {
        std::cout << "Packet " << GetNumber(this) << ": " << typeid(*this).name() << std::endl;
    }

    int check() const
    {
        return GetNumber(this);
    }

    Packet()
    {
        std::cout << "Packet<" << GetNumber(this) << ">()" << std::endl;
    }

    ~Packet()
    {
        std::cout << "~Packet<" << GetNumber(this) << ">()" << std::endl;
    }
};

template<> struct Packet<CREATE_BUILDING>
{
    float a;

    void print() const
    {
        std::cout << "Packet " << GetNumber(this) << ": " << typeid(*this).name() << std::endl;
    }

    int check() const
    {
        return GetNumber(this);
    }

    Packet()
    {
        std::cout << "Packet<" << GetNumber(this) << ">()" << std::endl;
    }

    ~Packet()
    {
        std::cout << "~Packet<" << GetNumber(this) << ">()" << std::endl;
    }
};

template <int N> struct at_impl { typedef Packet<N> type; };
template <> struct at_impl<COUNT> { typedef i2tc::type_list::finish_tag type; };

template<class ImplTag>
struct PacketTypes: i2tc::type_list::base<ImplTag>
{
    template <int N> struct at { typedef typename at_impl<N>::type type; };
};

struct CallVisitor: i2tc::visitor_base<int>
{
    template<int N>
    int exec( const Packet<N> *t ) const
    {
        t->print();
        return t->check();
    }
};

struct CreateVisitor: i2tc::visitor_base<void *>
{
    template<int N>
    void *exec( const Packet<N> * ) const
    {
        return new Packet<N>;
    }
};

struct DestroyVisitor: i2tc::visitor_base<void>
{
    template<int N>
    void exec( Packet<N> *pPacket ) const
    {
        delete pPacket;
    }
};

template< class ImplTag >
void test()
{
    typedef PacketTypes<ImplTag> PT;

    const int PacketNum = static_cast<int>(std::time(NULL) % COUNT); 
    
    void * const ptr = id2type_cast<PT, CreateVisitor>( PacketNum );
    const int rez = id2type_cast<PT>( PacketNum, CallVisitor(), static_cast<const void *>(ptr) );
    id2type_cast<PT>( PacketNum,  DestroyVisitor(), ptr );

    std::cout << "Result " << rez << std::endl;
}

void packetsTest()
{
    test<i2tc::table_impl_tag>();
    test<i2tc::switch_impl_tag>();
}


#ifdef MAIN_DEFINITION_REQUIRED
int main()
{
    packetsTest();
    return 0; 
}
#endif