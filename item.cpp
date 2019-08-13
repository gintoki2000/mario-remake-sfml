#include "item.h"


CItem::CItem()
{

}

TBoolean CItem::init(CProperties *properties)
{
    return CGameObject::init(properties);
}
