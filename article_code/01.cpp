#include <iostream>
#include <string>
#include <vector>
#include <iterator>

struct A
{
    template<class T>
    /*virtual*/ void save(const T &t); // error: templates may not be 'virtual'
};

struct B: A
{
    template<class T>
    void save(const T &t) 
    { 
        std::cout << "single type: " << t << std::endl; 
    }

    template<class T>
    void save( const std::vector<T> &v ) 
    { 
        std::cout << "container: { "; 
        std::copy( v.begin(), v.end(), std::ostream_iterator<T>(std::cout, " ") ); 
        std::cout << "}" << std::endl;
    }
};

struct C: A
{
    template<class T>
    void save(const T &) 
    { 
        std::cout << "C::save() called" << std::endl; 
    }
};

template<class A>
void test( A *p ) //Нешаблонная функция
{
    p->save( 2 );
    p->save( 3.4 );
    p->save( std::string("test") );
    p->save( std::vector<int>(3, 1) );
}

int main()
{
    B b;
    C c;

    test(&b);
    test(&c);

    return 0;
}
