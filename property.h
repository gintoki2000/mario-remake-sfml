#ifndef PROPERTY_H
#define PROPERTY_H
#include "base.h"
#include <map>
#include <string>
template<class __PropertyType>
class CProperty : public CObject
{
public:
    CProperty(const __PropertyType& aValue);

    const __PropertyType& value() const;

    void set(const __PropertyType& aValue);
private:
    __PropertyType mValue;
};

template<class __PropertyType>
CProperty<__PropertyType>::CProperty(const __PropertyType &aValue):
    mValue(aValue)
{

}

template<class __PropertyType>
const __PropertyType &CProperty<__PropertyType>::value() const
{
    return mValue;
}

template<class __PropertyType>
void CProperty<__PropertyType>::set(const __PropertyType &aValue)
{
    mValue = aValue;
}

class CProperties : public CObject
{
public:
    CProperties(const CProperties&) = delete;
    CProperties&operator=(const CProperties&) = delete;
    CProperties();
    ~CProperties();

    void put(const TString& aName, CObject* aValue);

    void set(const TString& aName, CObject* aValue);

    template<class __PropertyType>
    const __PropertyType* get(const TString& aName) const
    {
        auto pos = mProperties.find(aName);
        if(pos == mProperties.end()) return nullptr;
        return dynamic_cast<__PropertyType*>(pos->second);
    }

    void remove(const TString& aName);

    TBoolean contains(const TString& aName) const;
private:
    std::map<TString, CObject*> mProperties;
};

typedef CProperty<TInt> CInt;
typedef CProperty<TFloat> CFloat;
typedef CProperty<TBoolean> CBoolean;
typedef CProperty<TString> CString;
#endif // PROPERTY_H
