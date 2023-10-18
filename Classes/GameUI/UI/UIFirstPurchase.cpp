//
//  UIFirstPurchase.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/10/07.
//

#include "UIFirstPurchase.h"

#include "Common/ConfigKey.h"

#include "GameUI/UI/UIReddot.h"

#include "GameUIPopup/Other/TimingPackage/PopupTimingPackage.h"
#include "GameUIPopup/Other/PurchaseCash/PopupPurchaseFirst.h"
#include "GameUIPopup/Other/PurchaseCash/PopupCash.h"

#include "ManagerGame/CashManager.h"

USING_NS_CC;

UIFirstPurchase* UIFirstPurchase::create()
{
    UIFirstPurchase* pRet = new(std::nothrow) UIFirstPurchase();
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

UIFirstPurchase::UIFirstPurchase():
_callbackReload(nullptr),
_button(nullptr),
_lbText(nullptr)
{
}

UIFirstPurchase::~UIFirstPurchase()
{
}
#pragma mark -init
bool UIFirstPurchase::init()
{
    setContentSize(Size(72, 100));
    
    int nBannerIdx = CashManager::getInstance()->getBannerIdx();
    int nBannerBox = 7;
    if ( nBannerIdx == 1 )          nBannerBox = 5;
    else if ( nBannerIdx == 2 )     nBannerBox = 1;
    else if ( nBannerIdx == 3 )     nBannerBox = 3;
    
    _button = ui::Button::create("Assets/ui/bg_icon/icon_bonus1_1.png");
    _button->addClickEventListener(CC_CALLBACK_1(UIFirstPurchase::onClick, this));
    _button->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _button->setZoomScale(0.05f);
    _button->setPosition(getContentSize()/2);
    addChild(_button);
    
    _lbText = Label::createWithTTF(GAME_TEXT("t_ui_purchase_bonus_msg_10"), GAME_FONT, 16);
    _lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _lbText->setPosition(_button->getPosition() + Vec2::DOWN * 15);
    _lbText->setDimensions(getContentSize().width + 20, _lbText->getContentSize().height);
    _lbText->setAlignment(TextHAlignment::CENTER, TextVAlignment::TOP);
    _lbText->setOverflow(Label::Overflow::SHRINK);
    _lbText->enableOutline(Color4B::BLACK, 1);
    _lbText->setLineSpacing(-10);
    addChild(_lbText);
    
    // reddot
    auto action1 = CallFuncN::create([=](Ref* sender){
        
        auto item = (Node*)sender;
        
        int nResult = CashManager::getInstance()->getFirstBonusResult();
        if ( nResult != 0 && nResult != 1 )
        {
            item->stopAllActions();
            if(_callbackReload != nullptr)
                _callbackReload();
            return;
        }
        
        bool bShow = UserDefault::getInstance()->getBoolForKey(KEY_REDDOT_PURCHASE_FIRST, false);
        if ( nResult == 0 )
        {
            bShow = false;
        }
        
        auto oldRedDot = item->getChildByName("UIReddot");
        
        if ( bShow == false  )
        {
            if (oldRedDot == nullptr )
            {
                auto uiReddot = UIReddot::create(E_REDDOT_TYPE::NOMAL, Vec2::ANCHOR_TOP_LEFT);
                uiReddot->setPosition(getContentSize().width - 10, getContentSize().height - 20);
                uiReddot->setScale(0.7);
                
               item->addChild(uiReddot);
                
            }
        }
        else if ( oldRedDot != nullptr )
        {
            oldRedDot->removeFromParent();
        }
    });
    auto action2 = DelayTime::create(2);
    auto seq = Sequence::create(action1, action2, nullptr);
    _button->runAction(RepeatForever::create(seq));
    
    return true;
}

#pragma mark -utils
void UIFirstPurchase::onClick(cocos2d::Ref* pSender)
{
    auto popup = PopupPurchaseFirst::create();
    popup->setCallbackResult(CC_CALLBACK_1(UIFirstPurchase::callbackPurchaseFirstBonus, this));
    popup->show();
}
void UIFirstPurchase::callbackPurchaseFirstBonus(int nType)
{
    if ( nType != 5 )
    {
        auto popup = PopupCash::create();
        popup->show();
    }
}


void UIFirstPurchase::setCallbackReload(const std::function<void(void)>& pCallback)
{
    _callbackReload = pCallback;
}
