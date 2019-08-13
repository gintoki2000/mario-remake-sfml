#ifndef BASE_H
#define BASE_H
#include <string>
typedef char            TInt8;
typedef short           TInt16;
typedef int             TInt32;
typedef long            TInt64;
typedef int             TInt;
typedef unsigned int    TUInt;
typedef unsigned int    TUInt32;
typedef unsigned long   TUInt64;
typedef float           TFloat;
typedef double          TDouble;
typedef bool            TBoolean;
typedef std::string     TString;

class CObject
{
public:
    CObject();

    virtual ~CObject();
};

#endif // BASE_H
