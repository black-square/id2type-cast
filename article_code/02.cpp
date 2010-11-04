#include <iostream>
#include <string>
#include <vector>
#include <iterator>
#include <complex>
#include <algorithm>
#include <functional>

struct IStatistics
{
    //...
    virtual void append( double v ) = 0;
};

struct IData
{
    //...
    virtual void processStatistics( IStatistics *pStat ) = 0;
};

class RealData: public IData
{
public:
    RealData( double v ): val(v) {}
    void processStatistics( IStatistics *pStat ) { pStat->append(val); }
    //...
private:
    double val;
};

class ComplexData: public IData
{
public:
    ComplexData( double r, double im ): val(r, im) {}
    void processStatistics( IStatistics *pStat ) { pStat->append(val.real()); }
    //...
private:
    std::complex<double> val;
};

class ConcreteStat: public IStatistics
{
public:
    void append( double v ) { std::cout << ">> " << v << std::endl; }
    //...
};

typedef std::vector<IData *> TDataContainer;
typedef std::vector<IData *>::const_iterator TDataConstIterator;

void calcStat( const TDataContainer &vecData, IStatistics *pStat )
{
    std::for_each( vecData.begin(), vecData.end(), std::bind2nd(std::mem_fun(&IData::processStatistics), pStat ) );
    
    std::cout << "======= " << std::endl;
    
    for( 
        TDataConstIterator iter = vecData.begin(); 
        iter != vecData.end(); 
        ++iter 
    )
        (*iter)->processStatistics(pStat);
}

int main()
{
    TDataContainer vecData;
    IStatistics *pStat = new ConcreteStat;
    
    vecData.push_back( new RealData(1) );
    vecData.push_back( new ComplexData(2, 3) );
    vecData.push_back( new RealData(4) );

    calcStat( vecData, pStat );
    
    //...
    return 0;
}
