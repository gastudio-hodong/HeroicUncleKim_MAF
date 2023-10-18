#ifndef ITimingPackage_h
#define ITimingPackage_h

#include "Common/ConfigDefault.h"

class ITimingPackage
{
public:
    virtual void onNotifyTimingPackge(cocos2d::ValueMap listData) = 0;
};

#endif /* ITimingPackage_hpp */
