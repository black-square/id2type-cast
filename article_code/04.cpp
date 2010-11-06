#include <typeinfo>
#define rev_typeid(arg) C

using namespace std;

struct A {};
struct B: A {};
struct C: A {};
struct D: A {};

template<class T>
void f( T * );

void call_f( const type_info &ti, A *pA )
{
    f( static_cast<rev_typeid(ti) *>(pA) );
}

template<class T>
void f2( T * );

void call_f2( const type_info &ti, A *pA )
{
    f2( static_cast<rev_typeid(ti) *>(pA) );
}

template<class Result>
struct function
{
    typedef Result result_type;
};

struct F: function<void>
{ 
    template<class T> void operator()( T * );
};

template<class T, class Func>
typename Func::result_type call( const type_info &ti, T *p, Func f )
{
    return f( static_cast<rev_typeid(ti) *>(p) );
}

int main()
{
    A *pA = 0;
    const type_info &ti = typeid(A);
    call_f( ti, pA );
    call_f2( ti, pA );
    
    call( ti, pA, F() );
}