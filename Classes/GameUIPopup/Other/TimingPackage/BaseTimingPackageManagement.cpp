//
//  BaseTimingPackageManagement.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/06/21.
//

#include "BaseTimingPackageManagement.h"

#include "ManagerGame/TimingPackageManager.h"
#include "ManagerGame/AccountManager.h"

USING_NS_CC;

BaseTimingPackageManagement::BaseTimingPackageManagement(void)
{
    _listInfoTimingPackage.clear();
    _listUITimingbtn.clear();
}

BaseTimingPackageManagement::~BaseTimingPackageManagement(void)
{
    _listInfoTimingPackage.clear();
    _listUITimingbtn.clear();
    
    TimingPackageManager::getInstance()->unSubEventTimingPackage(this);
}
#pragma mark -init
bool BaseTimingPackageManagement::init()
{
    if ( !LayerColor::initWithColor(Color4B(0, 0, 0, 0)) )
    {
        return false;
    }
    
    TimingPackageManager::getInstance()->subEventTimingPackage(new MafFunction<void(cocos2d::ValueMap )>(CC_CALLBACK_1(BaseTimingPackageManagement::onNotifyTimingPackge, this), this));
    
    return true;
}
void BaseTimingPackageManagement::initTimingPackage()
{
    TimingPackageManager::getInstance()->setArrangementInfo();

    checkOfflineTimingPackageReward();
    checkOfflineTimingPackageStart();
    
    checkTimingPackage();
}

#pragma mark -TimingPackage
void BaseTimingPackageManagement::onNotifyTimingPackge(cocos2d::ValueMap listData)
{
    std::string strKind = "NONE";
    
    auto iter = listData.find("KIND");
    if(iter != listData.end())
    {
        strKind = iter->second.asString();
    }
    
    if(strKind.compare("NONE") == 0)
        checkTimingPackage();
}

void BaseTimingPackageManagement::checkTimingPackage()
{
    //
    auto listTimingReward = TimingPackageManager::getInstance()->getListInfoTimingPackageRewardAll();
    if ( listTimingReward.size() > 0 )
    {
        ValueMap listData;
        listData["KIND"] = Value("REWARD");
        
        TimingPackageManager::getInstance()->invokeEventTimingPackage(listData);
    }
    
    //
    reloadTimingPackage();
}

void BaseTimingPackageManagement::checkOfflineTimingPackageReward()
{
    //
    auto listTimingPackageReward = TimingPackageManager::getInstance()->getOfflineRewardPackageInfo();
    
    for(auto obj : listTimingPackageReward)
    {
        TimingPackageManager::getInstance()->requestReward(obj, nullptr, false);
    }
    
    TimingPackageManager::getInstance()->clearOfflineRewardList();
}

void BaseTimingPackageManagement::checkOfflineTimingPackageStart()
{
    //
    auto listTimingPackageStart = TimingPackageManager::getInstance()->getOfflineStartPackageInfo();
    
    if ( AccountManager::getInstance()->getUserIdx() == 0 )
    {
        TimingPackageManager::getInstance()->clearOfflineStartList();
        return;
    }
    
    for(auto obj : listTimingPackageStart)
    {
        TimingPackageManager::getInstance()->requestTimingPackageActivate(CC_CALLBACK_2(BaseTimingPackageManagement::callbackOfflineTimingPackageStart, this), obj);
    }
    
    TimingPackageManager::getInstance()->clearOfflineStartList();
}

void BaseTimingPackageManagement::callbackOfflineTimingPackageStart(const bool bResult, const int nResult)
{
    if(bResult == false)
        return;
    
    checkTimingPackage();
}

#pragma mark - setting
void BaseTimingPackageManagement::makeupTimingPackageBtnList()
{
    resetTimingPackageBtnList();
    
    _listInfoTimingPackage = TimingPackageManager::getInstance()->getListInfoTimingPackageStartAll();
    
    for(auto obj : _listInfoTimingPackage)
    {
        auto uiItem = UITimingPackageBtn::create();
        uiItem->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        uiItem->setCallbackReload(CC_CALLBACK_0(BaseTimingPackageManagement::reloadTimingPackage, this));
        uiItem->setCallbackShowPopup(CC_CALLBACK_1(BaseTimingPackageManagement::callbackShowTimingPackagePopup, this));
        uiItem->setInfoTimingPackage(obj);
        uiItem->setVisibleTimingPackageBtn(false);
        addChild(uiItem);
        
        _listUITimingbtn.pushBack(uiItem);
    }
}
void BaseTimingPackageManagement::resetTimingPackageBtnList()
{
    for(auto obj : _listUITimingbtn)
    {
        obj->deActiveUITimingPackageBtn();
        obj->removeFromParent();
    }
    _listUITimingbtn.clear();
}

void BaseTimingPackageManagement::reloadTimingPackage()
{
    makeupTimingPackageBtnList();
    setMenu();
    drawContainer();
}

void BaseTimingPackageManagement::callbackShowTimingPackagePopup(InfoTimingPackage* info)
{
    TimingPackageManager::getInstance()->showTimingPackagePopup(info);
    onExpand();
}

void BaseTimingPackageManagement::setMenu()
{
    //override
}

void BaseTimingPackageManagement::drawExpand()
{
    //override
}

void BaseTimingPackageManagement::drawContainer()
{
    //override
}

void BaseTimingPackageManagement::onExpand()
{
    //override
}
