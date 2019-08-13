#include "property.h"


CProperties::CProperties()
{

}

CProperties::~CProperties()
{
    for(const auto& pair : mProperties)
    {
        delete pair.second;
    }
    mProperties.clear();
}

void CProperties::put(const TString& aName, CObject *aValue)
{
    if(contains(aName)) return;
    mProperties.insert(std::make_pair(aName, aValue));
}

void CProperties::set(const TString &aName, CObject *aValue)
{
    auto oldVal = mProperties[aName];
    if(oldVal) delete oldVal;
    mProperties[aName] = aValue;
}

void CProperties::remove(const TString &aName)
{
    auto pos = mProperties.find(aName);
    if(pos != mProperties.end())
    {
        delete pos->second;
        mProperties.erase(pos);
    }
}

TBoolean CProperties::contains(const std::string &aName) const
{
    return mProperties.find(aName) != mProperties.end();
}
