//
//  UIBannerPurchase.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/10/07.
//

#include "UIBannerPurchase.h"

#include "GameUI/UI/UIReddot.h"

#include "GameUIPopup/Other/TimingPackage/PopupTimingPackage.h"
#include "GameUIPopup/Other/PurchaseCash/PopupPurchaseBanner.h"

#include "ManagerEvent/EventManager.h"
#include "ManagerGame/CashManager.h"

USING_NS_CC;

UIBannerPurchase* UIBannerPurchase::create()
{
    UIBannerPurchase* pRet = new(std::nothrow) UIBannerPurchase();
    if ( pRet && pRet->init() )
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}

UIBannerPurchase::UIBannerPurchase():
_callbackReload(nullptr),
_button(nullptr),
_lbTimer(nullptr)
{
}

UIBannerPurchase::~UIBannerPurchase()
{
}
#pragma mark -init
bool UIBannerPurchase::init()
{
    setContentSize(Size(90, 100));
    
    int nBannerIdx = CashManager::getInstance()->getBannerIdx();
    int nBannerBox = 7;
    if ( nBannerIdx == 1 )          nBannerBox = 5;
    else if ( nBannerIdx == 2 )     nBannerBox = 1;
    else if ( nBannerIdx == 3 )     nBannerBox = 3;
    
    _button = ui::Button::create(MafUtils::format("Assets/ui/bg_icon/mpackage_icon_%d_01.png", nBannerBox));
    _button->addClickEventListener(CC_CALLBACK_1(UIBannerPurchase::onClick, this));
    _button->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    _button->setZoomScale(0.05f);
    _button->setPosition(Vec2(getContentSize().width, getContentSize().height / 2));
    addChild(_button);
    
    _lbTimer = Label::createWithTTF(GAME_TEXT("t_ui_week_banner"), GAME_FONT, 16);
    _lbTimer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _lbTimer->setPosition(Vec2(getContentSize().width / 2, -10));
    _lbTimer->setDimensions(getContentSize().width + 20, 30);
    _lbTimer->setAlignment(TextHAlignment::CENTER, TextVAlignment::TOP);
    _lbTimer->setOverflow(Label::Overflow::SHRINK);
    _lbTimer->enableOutline(Color4B::BLACK, 1);
    _lbTimer->setLineSpacing(-10);
    addChild(_lbTimer);
    
    auto action1 = CallFuncN::create( [=]( Ref* sender ) {
        
        if ( EventManager::getInstance()->isActivatedEvent(21) == false && EventManager::getInstance()->isActivatedEvent(22) == false )
        {
            return;
        }
        
        int64_t nTimeDiff = CashManager::getInstance()->getBannerTimeRemaining();
        if ( nTimeDiff < 0 )
        {
            ((Label*)sender)->setString("00:00:00");
            ((Label*)sender)->stopAllActions();
            return;
        }
        
        if ( nTimeDiff / 86400 >= 1 )
        {
            ((Label*)sender)->setString(GAME_TEXT("t_ui_week_banner"));
            return;
        }

        std::string strSaleTime = UtilsDate::getInstance()->getTimeString(nTimeDiff, UtilsDate::eType::date_user_type_1);
        ((Label*)sender)->setString(strSaleTime);
    });
    auto action2 = DelayTime::create(1.0);
    auto seq = Sequence::create(action1, action2, nullptr);
    _lbTimer->runAction( RepeatForever::create(seq) );
    
    //
    
    // reddot
    auto uiReddot = UIReddot::create(E_REDDOT_TYPE::NOMAL, Vec2::ANCHOR_TOP_LEFT);
    uiReddot->setPosition(getContentSize().width - 35, getContentSize().height - 20);
    uiReddot->setScale(0.7);
    uiReddot->setVisible(false);
    addChild(uiReddot);
    RefreshReddotManager::getInstance()->addUI(E_REFRESH_REDDOT::PURCHASE_BANNER, uiReddot);
    
    return true;
}

#pragma mark -utils
void UIBannerPurchase::onClick(cocos2d::Ref* pSender)
{
    RefreshReddotManager::getInstance()->setValue(E_REFRESH_REDDOT::PURCHASE_BANNER, false);
    RefreshReddotManager::getInstance()->refreshUI(E_REFRESH_REDDOT::PURCHASE_BANNER);
    
    //
    auto popup = PopupPurchaseBanner::create();
    popup->show();
}

#pragma mark -callback
void UIBannerPurchase::callbackRedraw(bool bResult)
{
    if ( bResult == false )
    {
        if(_callbackReload != nullptr)
            _callbackReload();
        return;
    }
    
    {
        int nBannerIdx = CashManager::getInstance()->getBannerIdx();
        int nBannerBox = 7;
        if ( nBannerIdx == 1 )          nBannerBox = 5;
        else if ( nBannerIdx == 2 )     nBannerBox = 1;
        else if ( nBannerIdx == 3 )     nBannerBox = 3;

        _button->loadTextureNormal(MafUtils::format("Assets/ui/bg_icon/mpackage_icon_%d_01.png", nBannerBox));
    }
    
    {
        // label
        _lbTimer->setString("");
        
        auto action1 = DelayTime::create(1.0);
        auto action2 = CallFuncN::create( [=]( Ref* sender ) {
            
            if ( EventManager::getInstance()->isActivatedEvent(21) == false && EventManager::getInstance()->isActivatedEvent(22) == false )
            {
                return;
            }

            int nTimeDiff = CashManager::getInstance()->getBannerTimeRemaining();
            if ( nTimeDiff <= 0 )
            {
                CashManager::getInstance()->requestBannerInfo(CC_CALLBACK_1(UIBannerPurchase::callbackRedraw, this));
                
                ((Label*)sender)->setString("00:00:00");
                ((Label*)sender)->stopAllActions();
                return;
            }
            
            if ( nTimeDiff / 86400 >= 1 )
            {
                ((Label*)sender)->setString(GAME_TEXT("t_ui_week_banner"));
                return;
            }

            std::string strSaleTime = UtilsDate::getInstance()->getTimeString(nTimeDiff, UtilsDate::eType::date_user_type_1);
            ((Label*)sender)->setString(strSaleTime);
        });
        auto seq = Sequence::create(action1, action2, nullptr);
        if ( CashManager::getInstance()->getBannerTimeRemaining() > 0 )
        {
            _lbTimer->runAction(RepeatForever::create(seq));
        }
        
    }
}

void UIBannerPurchase::setCallbackReload(const std::function<void(void)>& pCallback)
{
    _callbackReload = pCallback;
}
