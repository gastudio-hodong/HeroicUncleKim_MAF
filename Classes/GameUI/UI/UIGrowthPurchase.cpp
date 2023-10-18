//
//  UIGrowthPurchase.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/10/07.
//

#include "UIGrowthPurchase.h"

#include "Common/ConfigGameNormal.h"

#include "GameUI/UI/UIReddot.h"

#include "GameUIPopup/Other/TimingPackage/PopupTimingPackage.h"
#include "GameUIPopup/Other/PurchaseCash/PopupPurchaseGrowth.h"

#include "ManagerGame/AccountManager.h"

USING_NS_CC;

UIGrowthPurchase* UIGrowthPurchase::create()
{
    UIGrowthPurchase* pRet = new(std::nothrow) UIGrowthPurchase();
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

UIGrowthPurchase::UIGrowthPurchase():
_callbackReload(nullptr),
_button(nullptr),
_lbText(nullptr)
{
}

UIGrowthPurchase::~UIGrowthPurchase()
{
}
#pragma mark -init
bool UIGrowthPurchase::init()
{
    setContentSize(Size(72, 100));
    
    _button = ui::Button::create("Assets/ui/bg_icon/growpackage_icon_prison.png");
    _button->addClickEventListener(CC_CALLBACK_1(UIGrowthPurchase::onClick, this));
    _button->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _button->setZoomScale(0.05f);
    _button->setPosition(getContentSize()/2);
    addChild(_button);
    
    _lbText = Label::createWithTTF(GAME_TEXT("t_cash_title_simple_402"), GAME_FONT, 16);
    _lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _lbText->setPosition(Vec2(getContentSize().width / 2, -5));
    _lbText->setDimensions(getContentSize().width + 20, _lbText->getContentSize().height);
    _lbText->setAlignment(TextHAlignment::CENTER, TextVAlignment::TOP);
    _lbText->setOverflow(Label::Overflow::SHRINK);
    _lbText->enableOutline(Color4B::BLACK, 1);
    _lbText->setLineSpacing(-10);
    addChild(_lbText);
    
    return true;
}

#pragma mark -utils
void UIGrowthPurchase::onClick(cocos2d::Ref* pSender)
{
    if ( AccountManager::getInstance()->getUserIdx() == 0 )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_1"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    // 던전 선장 패키지
    auto popup = PopupPurchaseGrowth::create("FLOOR");
    popup->show();
}


void UIGrowthPurchase::setCallbackReload(const std::function<void(void)>& pCallback)
{
    _callbackReload = pCallback;
}
