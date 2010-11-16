#include <iostream>
#include <string>
#include <vector>
#include <iterator>
#include <complex>
#include <algorithm>
#include <functional>
#include <typeinfo>
#include <cassert>

struct IStatistics
{
    //template<class T>
    //void append( const T &v );
    
    virtual ~IStatistics() {}
};

class PrintValue: public IStatistics
{
public:
    template<class T>
    void append( const T &v ) { std::cout << ">> " << v << std::endl; }
    
    template<class T>
    void append( const std::complex<T> &v ) { std::cout << ">> " << v.real() << " " << v.imag() << std::endl; }
};

class PrintType: public IStatistics
{
public:
    template<class T>
    void append( const T & ) { std::cout << "type " << typeid(T).name() << std::endl; }
};

template<class Functor>
typename Functor::result_type id2type_cast( const std::type_info &ti, IStatistics *pStat, const Functor &functor )
{
    if( ti == typeid(PrintValue) )
        return functor( static_cast<PrintValue *>(pStat) );
        
    if( ti == typeid(PrintType) )
        return functor( static_cast<PrintType *>(pStat) );
    
    assert(false);
}

template<class Result>
struct function
{
    typedef Result result_type;
};


template<class T>
class CalcAppendFunc: public function<void>
{
public:
    CalcAppendFunc( const T &arg ): m_arg(arg) {}
    
    template<class StatT>
    void operator()( StatT *pStat ) const
    {
        pStat->append(m_arg);
    }
    
private:
    const T &m_arg;
};

template<class T>
CalcAppendFunc<T> MakeCalcAppendFunc( const T &arg )
{
    return CalcAppendFunc<T>(arg);
} 

struct IData
{
    //...
    virtual void processStatistics( IStatistics *pStat ) = 0;
};


class RealData: public IData
{
public:
    RealData( double v ): val(v) {}
    void processStatistics( IStatistics *pStat ) { id2type_cast(typeid(*pStat), pStat, MakeCalcAppendFunc(val)); }
    //...
private:
    double val;
};

class ComplexData: public IData
{
public:
    ComplexData( double r, double im ): val(r, im), val2(r, im) {}
    void processStatistics( IStatistics *pStat ) 
    { 
        id2type_cast(typeid(*pStat), pStat, MakeCalcAppendFunc(val)); 
        id2type_cast(typeid(*pStat), pStat, MakeCalcAppendFunc(val2));
    }
    //...
private:
    std::complex<double> val;
    std::complex<int> val2;
};


typedef std::vector<IData *> TDataContainer;
typedef std::vector<IData *>::const_iterator TDataConstIterator;

void calcStat( const TDataContainer &vecData, IStatistics *pStat )
{
    //Цикл из предыдущего примера можно иначе
    std::for_each( vecData.begin(), vecData.end(), std::bind2nd(std::mem_fun(&IData::processStatistics), pStat ) ); 
    std::cout << "======= " << std::endl;
}

int main()
{
    TDataContainer vecData;
    
    vecData.push_back( new RealData(1) );
    vecData.push_back( new ComplexData(2.3, 3.7) );
    vecData.push_back( new RealData(4) );

    calcStat( vecData, new PrintType );
    calcStat( vecData, new PrintValue );
    
    //...
    return 0;
}
