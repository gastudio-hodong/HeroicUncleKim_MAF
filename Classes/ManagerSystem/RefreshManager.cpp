#include "RefreshManager.h"

#include "Common/Observer/IRefresh.h"

USING_NS_CC;

RefreshManager::RefreshManager(void)
{
    
}

RefreshManager::~RefreshManager(void)
{
    
}

bool RefreshManager::init()
{
    return true;
}
 
void RefreshManager::addUI(E_REFRESH eType, cocos2d::Node* node)
{
    auto target = dynamic_cast<IRefresh*>(node);
    if ( target )
    {
        auto listener = cocos2d::EventListenerCustom::create(enumToString(eType), CC_CALLBACK_1(IRefresh::callbackRefreshUI, target));
        cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, node);
    }
}

void RefreshManager::refreshUI(E_REFRESH eType, cocos2d::ValueMap listData)
{
    cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(enumToString(eType), &listData);
}

void RefreshManager::refreshCurrencyUI(E_ITEMS eType, std::string strCount, bool bPet)
{
    ValueMap listData;
    listData["type"] = Value((int)eType);
    listData["count"] = Value(strCount);
    listData["pet"] = Value(bPet);
    
    refreshUI(E_REFRESH::CURRENCY, listData);
}
