#include "./../id2type_cast/id2type_cast.hpp"
#include "./../id2type_cast/embedded_type_id.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <numeric>

using i2tc::id2type_cast;

namespace
{

struct DataTypes;

struct Base: i2tc::embedded_type_id_base<DataTypes, i2tc::member_tag>
{

};

struct AData: i2tc::embedded_type_id_derived<AData, Base>
{
    int a;

    AData(): a(1) {}
};

struct BData: i2tc::embedded_type_id_derived<BData, Base>
{
    BData() { s = "some test string"; } 

    std::string s;
};

struct CData: i2tc::embedded_type_id_derived<CData, Base>
{
    CData() { l.push_back(10); l.push_back(20); l.push_back(30); }

    std::vector<int> l;
};

struct DataTypes: i2tc::type_list::base<>
{
    template <int N> struct at;
};

template <int N> struct DataTypes::at{ typedef i2tc::type_list::undefined_tag type; };
template <> struct DataTypes::at<0>{ typedef AData type; };
template <> struct DataTypes::at<1>{ typedef BData type; };
template <> struct DataTypes::at<2>{ typedef CData type; };

struct TestFunctor: i2tc::function<int>
{
    int operator()( const AData *p ) const
    {
        return p->a;
    }

    int operator()( const BData *p ) const
    {
        return (int)p->s.length();
    }

    int operator()( const CData *p ) const
    {
        return std::accumulate( p->l.begin(), p->l.end(), 0 );
    }
};

}

int testOptimizationsImpl( Base *p )
{
    return id2type_cast<TestFunctor>( p );
}

void testOptimizations()
{
    Base * const data[] = { new AData, new BData, new CData };
    int rez = 0;

    for( int i = 0; i < 3; ++i )
        rez += testOptimizationsImpl( data[i] );

    std::cout << rez << std::endl;
}

#ifdef MAIN_DEFINITION_REQUIRED
int main()
{
    testOptimizations();
    return 0; 
}
#endif
