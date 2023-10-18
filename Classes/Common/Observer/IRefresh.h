#ifndef IRefresh_h
#define IRefresh_h

#include "Common/ConfigDefault.h"

class IRefresh
{
public:
    virtual void refresh(E_REFRESH eType, cocos2d::ValueMap listData) = 0;
    
public:
    void callbackRefreshUI(cocos2d::EventCustom *event)
    {
        cocos2d::ValueMap listData;
        
        auto userData = static_cast<cocos2d::ValueMap*>(event->getUserData());
        if ( userData != nullptr )
        {
            listData = *userData;
        }
        
        //
        refresh(enumFromString<E_REFRESH>(event->getEventName()), listData);
    }
};

#endif /* IRefresh_h */
