#include "./poly_serialization.hpp"

#include <sstream>
#include <iostream>
#include <ctime>

using i2tc::id2type_cast;

#if 0
    typedef i2tc::virt_func_tag emb_type_id_impl;
#else
    typedef i2tc::member_tag emb_type_id_impl;
#endif

struct DataTypes;

struct BaseData: i2tc::embedded_type_id_base<DataTypes, emb_type_id_impl>
{
    virtual ~BaseData() {}
};

struct AData: i2tc::embedded_type_id_derived<AData, BaseData, emb_type_id_impl>
{
    short a;

    AData(): a(static_cast<short>(rand())) { std::cout << "AData()" << std::endl; }
    ~AData() { std::cout << "~AData()" << std::endl; }

    template<class Archive>
    void Serialize(Archive & archive)
    {
        archive & a;
    }
};

struct BData: i2tc::embedded_type_id_derived<BData, BaseData, emb_type_id_impl>
{
    long d;
    double e;

    BData(): d(static_cast<long>(rand())), e(static_cast<double>(rand())) { std::cout << "BData()" << std::endl; }
    ~BData() { std::cout << "~BData()" << std::endl; }                     

    template<class Archive>
    void Serialize(Archive & archive)
    {
        archive & d & e;
    }
};

struct CData: i2tc::embedded_type_id_derived<CData, BaseData, emb_type_id_impl>
{
    float g;
    unsigned short h;
    unsigned int i;

    CData(): g(static_cast<float>(rand())), h(static_cast<unsigned short>(rand())), i(static_cast<unsigned int>(rand())) { std::cout << "CData()" << std::endl; }
    ~CData() { std::cout << "~CData()" << std::endl; }   

    template<class Archive>
    void Serialize(Archive & archive)
    {
        archive & g & h & i;
    }
};

struct DataTypes: i2tc::type_list::base<>
{
    template <int N> struct at;
};

template <int N> struct DataTypes::at{ typedef i2tc::type_list::finish_tag type; };
template <> struct DataTypes::at<0>{ typedef AData type; };
template <> struct DataTypes::at<1>{ typedef BData type; };
template <> struct DataTypes::at<2>{ typedef CData type; };


struct RegularData
{
    double e;

    RegularData(): e(static_cast<double>(rand())) { std::cout << "RegularData()" << std::endl; }
    ~RegularData() { std::cout << "~RegularData()" << std::endl; }   

    template<class Archive>
    void Serialize(Archive & archive)
    {
        archive & e;
    }
};

struct Data
{
    int a;
    BaseData *pBase;
    RegularData *pRegData;

    Data(): a( rand() ), pBase(0), pRegData(0) {}
    ~Data() { delete pBase; delete pRegData; }

    template<class Archive>
    void Serialize(Archive & archive)
    {
        archive & a & pBase & pRegData;
    }
};

struct CreateFunctor: i2tc::function<BaseData *>
{
    template<class T>
    BaseData *operator()( const T * ) const
    {
        return new T;
    }
};

template<class T>
void print( T & t)
{
    std::stringstream stream;
    poly_serialization::writer outSlr(stream);
    t.Serialize(outSlr);
    std::cout << stream.str() << std::endl;   
}

void initBaseImpl( Data &data, int curType )
{
    if( curType != i2tc::type_list::size<DataTypes>::value )
    {
        data.pBase = id2type_cast<DataTypes, CreateFunctor>( curType );
        data.pRegData = new RegularData;
    }
}

void SerializationTestImpl( int curType )
{
    std::string rawData;

    std::cout << "Write to rawData" << std::endl;

    {
        Data data;
        initBaseImpl(data, curType);

        std::stringstream stream;
        poly_serialization::writer outSlr(stream);

        data.Serialize(outSlr);
        rawData = stream.str();
        std::cout << rawData << std::endl;
    }

    std::cout << std::endl << "Read from rawData clean object" << std::endl;

    {
        Data data;
        print(data);

        std::stringstream stream( rawData );
        poly_serialization::reader inSlr(stream);
        data.Serialize(inSlr);
        print(data);
    }


    std::cout << std::endl << "Read from rawData exists object" << std::endl;
    {
        Data data;
        initBaseImpl(data, 2); 
        print(data);

        std::stringstream stream( rawData );
        poly_serialization::reader inSlr(stream);
        data.Serialize(inSlr);
        print(data);
    }
}

void serializationTest()
{
    for(int i = 0; i < i2tc::type_list::size<DataTypes>::value + 1; ++ i )
    {
        std::cout << ">>>> Type # " << i  << std::endl;
        SerializationTestImpl(i);
        std::cout << std::endl;
    }
}

#ifdef MAIN_DEFINITION_REQUIRED
int main()
{
    serializationTest();
    return 0; 
}
#endif
