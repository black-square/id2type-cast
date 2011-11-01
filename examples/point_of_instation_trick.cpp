// Иллюстрация трюка с тем, что определение списка DataTypes находится после функции
// pointOfInstationTrick, в которой он используется (см. optimization_test.cpp). Этот код успешно компилирует 
// MSVC и GCC (комментируем/определяем STANDARD_COMPILANT), но Comeau online ругается 
// error: incomplete type is not allowed typedef typename TypeList::template at<T> Tmp;
// Поэтому использовать такой подход не желательно

#include <cassert>
#include <limits>
#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <numeric>

//#define STANDARD_COMPILANT

template<class TypeList>
class embedded_type_id_base;

namespace detail
{
    template<class Derived, class Base>
    class embedded_type_id_derived_impl;
}

///////////////////////////////////////////////////////////////////////////////

template<class TypeList>
class embedded_type_id_base
{
public:
    typedef TypeList i2tc_type_list;
    int i2tc_get_id() const { return m_typeId; }

protected:
    embedded_type_id_base(): m_typeId( std::numeric_limits<int>::min() ) {}
    ~embedded_type_id_base() { assert( m_typeId != std::numeric_limits<int>::min() ); }

private:
    template<class Derived, class Base>
    friend class detail::embedded_type_id_derived_impl;

    int m_typeId;
};
///////////////////////////////////////////////////////////////////////////////

template<class TypeList, class T>
struct at
{
    typedef typename TypeList::template at<T> Tmp;
    enum{ value = Tmp::value };
};

namespace detail
{
    template< class Derived, class Base >
    class embedded_type_id_derived_impl: public Base
    {
    public:
        embedded_type_id_derived_impl() 
        { 
            static_cast<Derived *>(this)->m_typeId = at< typename Derived::i2tc_type_list, Derived >::value; 
        }
    };
}
///////////////////////////////////////////////////////////////////////////////

template<class Derived, class Base>
class embedded_type_id_derived: 
    public detail::embedded_type_id_derived_impl<Derived, Base>
{};
///////////////////////////////////////////////////////////////////////////////

#ifndef STANDARD_COMPILANT
  struct DataTypes;
#else
  struct AData;
  struct BData;
  struct CData;

  struct DataTypes
  {
    template <class T> struct at;
  };

  template <class T> struct DataTypes::at{ enum{ value = -1 }; };
  template <> struct DataTypes::at<AData>{ enum{ value = 10 }; };
  template <> struct DataTypes::at<BData>{ enum{ value = 20 }; };
  template <> struct DataTypes::at<CData>{ enum{ value = 30 }; };
#endif

struct Base: embedded_type_id_base<DataTypes>
{

};

struct AData: embedded_type_id_derived<AData, Base>
{
    int a;

    AData(): a(1) {}
};

struct BData: embedded_type_id_derived<BData, Base>
{
    BData() { s = "some test string"; } 

    std::string s;
};

struct CData: embedded_type_id_derived<CData, Base>
{
    CData() { l.push_back(10); l.push_back(20); l.push_back(30); }

    std::vector<int> l;
};

void test()
{
  AData data1;
  BData data2;
  CData data3;
  
  std::cout << data1.i2tc_get_id() << std::endl;
  std::cout << data2.i2tc_get_id() << std::endl;
  std::cout << data3.i2tc_get_id() << std::endl;
}

int main()
{
  test();
  return 0;
}

#ifndef STANDARD_COMPILANT
  struct DataTypes
  {
    template <class T> struct at;
  };

  template <class T> struct DataTypes::at{ enum{ value = -1 }; };
  template <> struct DataTypes::at<AData>{ enum{ value = 10 }; };
  template <> struct DataTypes::at<BData>{ enum{ value = 20 }; };
  template <> struct DataTypes::at<CData>{ enum{ value = 30 }; };
#endif
