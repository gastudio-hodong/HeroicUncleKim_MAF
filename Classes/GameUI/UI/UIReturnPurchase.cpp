//
//  UIReturnPurchase.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/10/07.
//

#include "UIReturnPurchase.h"

#include "GameUI/UI/UIReddot.h"

#include "GameUIPopup/Other/TimingPackage/PopupTimingPackage.h"
#include "GameUIPopup/Other/PurchaseCash/PopupPurchaseReturn.h"

#include "ManagerEvent/EventManager.h"
#include "ManagerGame/CashManager.h"

USING_NS_CC;

UIReturnPurchase* UIReturnPurchase::create()
{
    UIReturnPurchase* pRet = new(std::nothrow) UIReturnPurchase();
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

UIReturnPurchase::UIReturnPurchase():
_callbackReload(nullptr),
_button(nullptr),
_lbText(nullptr)
{
}

UIReturnPurchase::~UIReturnPurchase()
{
}
#pragma mark -init
bool UIReturnPurchase::init()
{
    setContentSize(Size(72, 100));
    
    int nBannerIdx = CashManager::getInstance()->getBannerIdx();
    int nBannerBox = 7;
    if ( nBannerIdx == 1 )          nBannerBox = 5;
    else if ( nBannerIdx == 2 )     nBannerBox = 1;
    else if ( nBannerIdx == 3 )     nBannerBox = 3;
    
    _button = ui::Button::create("Assets/ui/bg_icon/icon_return.png");
    _button->addClickEventListener(CC_CALLBACK_1(UIReturnPurchase::onClick, this));
    _button->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _button->setZoomScale(0.05f);
    _button->setPosition(getContentSize()/2);
    addChild(_button);
    
    _lbText = Label::createWithTTF(GAME_TEXT("t_ui_purchase_return_msg_12"), GAME_FONT, 16);
    _lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _lbText->setPosition(_button->getPosition() + Vec2::DOWN * 15);
    _lbText->setDimensions(getContentSize().width + 20, _lbText->getContentSize().height);
    _lbText->setAlignment(TextHAlignment::CENTER, TextVAlignment::TOP);
    _lbText->setOverflow(Label::Overflow::SHRINK);
    _lbText->enableOutline(Color4B::BLACK, 1);
    _lbText->setLineSpacing(-10);
    addChild(_lbText);
    
    return true;
}

#pragma mark -utils
void UIReturnPurchase::onClick(cocos2d::Ref* pSender)
{
    if ( EventManager::getInstance()->isActivatedEvent(14) == false )
    {
        if(_callbackReload != nullptr)
            _callbackReload();
        return;
    }
    auto popup = PopupPurchaseReturn::create();
    popup->show();
}


void UIReturnPurchase::setCallbackReload(const std::function<void(void)>& pCallback)
{
    _callbackReload = pCallback;
}
