#include <iostream>
#include "./../id2type_cast/id2type_cast.hpp"
#include <typeinfo>

struct DataTypes: i2tc::type_list::base<i2tc::table_impl_tag> 
{ 
    struct size;
    template <int N> struct at; 
};
template <int N> struct DataTypes::at{ typedef i2tc::type_list::undefined_tag type; };

////////////////////////////////////
// Maple:
//         N := 500:
//         fd := fopen("c:/temp/typelist_max_size_test_impl.h", WRITE);
//         for i from 0 to N do fprintf(fd, "struct A_%d {};\n", i); end do:
//         fprintf(fd, "\n\n"):
//         for i from 0 to N do fprintf(fd, "template <> struct DataTypes::at<%d>{ typedef A_%d type; };\n", i, i); end do:
//         fprintf(fd, "struct DataTypes::size{ enum{ value = %d }; };", N + 1);
//         fclose(fd);
////////////////////////////////////

#include "./typelist_max_size_test_impl.h"

namespace tl = i2tc::type_list;

struct Tst: i2tc::function<>
{
    template<class T>
    void operator()( T *t ) const
    {
        (void)t;
        std::cout << "call: " << typeid(*t).name() << std::endl;
    }
};


void maxTypeListSize()
{
    std::cout << "at: " << typeid(tl::at<DataTypes, 99>::type).name() << std::endl;
    std::cout << "size: " << tl::size<DataTypes>::value << std::endl;
    typedef tl::at<DataTypes, tl::size<DataTypes>::value - 1>::type last_type;
    std::cout << "last_type: " << typeid(last_type).name() << std::endl;
    //std::cout << "last_index: " << tl::index_of<DataTypes, last_type>::value << std::endl;

    for( int i = 0; i < 40; ++i )
        i2tc::id2type_cast<DataTypes, Tst>(i);
             
    for( int i = tl::size<DataTypes>::value - 40; i < tl::size<DataTypes>::value; ++i )
         i2tc::id2type_cast<DataTypes, Tst>(i);  
}

#ifdef MAIN_DEFINITION_REQUIRED
int main()
{
    maxTypeListSize();
    return 0;
}
#endif
