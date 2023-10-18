//
//  UILayoutCashHotDeal.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/12/03.
//

#include "UILayoutCashHotDeal.h"

#include "GameObject/InfoSetting.h"

#include "GameUIPopup/Other/PurchaseCash/PopupPurchaseGrowth.h"
#include "GameUIPopup/Other/PurchaseCash/PopupPurchaseBanner.h"
#include "GameUIPopup/Other/PurchaseCash/PopupPurchaseFirst.h"

#include "ManagerEvent/EventManager.h"
#include "ManagerGame/ItemsManager.h"
#include "ManagerGame/CashManager.h"

USING_NS_CC;

UILayoutCashHotDeal* UILayoutCashHotDeal::create()
{
    UILayoutCashHotDeal* pRet = new(std::nothrow) UILayoutCashHotDeal();
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

UILayoutCashHotDeal::UILayoutCashHotDeal():
_bInitUI(false),

//

//
_uiContainerInfo(nullptr)
{
    
}

bool UILayoutCashHotDeal::init()
{
    if ( !ui::Layout::init() )
    {
        return false;
    }
        
    //
    initVar();
    initUI();
    
    return true;
}


void UILayoutCashHotDeal::onEnter()
{
    ui::Layout::onEnter();
    
    if ( _bInitUI == false )
    {
        //
        uiContainer();
        uiInfo();
        
        _bInitUI = true;
    }
    
    // draw
    drawInfo();
}

#pragma mark - init
void UILayoutCashHotDeal::initVar()
{
    
}

void UILayoutCashHotDeal::initUI()
{
    
}

#pragma mark - ui
void UILayoutCashHotDeal::uiContainer()
{
    Size size = getContentSize();
    
    //
    auto layout = ui::Layout::create();
    layout->setContentSize(Size(size.width, size.height));
    layout->setIgnoreAnchorPointForPosition(false);
    layout->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    layout->setPosition(Vec2(getContentSize().width / 2, getContentSize().height));
    addChild(layout);

    _uiContainerInfo = ui::Layout::create();
    _uiContainerInfo->setContentSize(Size(size.width, size.height));
    _uiContainerInfo->setIgnoreAnchorPointForPosition(false);
    _uiContainerInfo->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _uiContainerInfo->setPosition(Vec2(size.width / 2, size.height));
    _uiContainerInfo->setLayoutType(ui::Layout::Type::VERTICAL);
    layout->addChild(_uiContainerInfo);
}

void UILayoutCashHotDeal::uiInfo()
{
    auto layout = _uiContainerInfo;
    auto size = layout->getContentSize();

    int nResult = CashManager::getInstance()->getFirstBonusResult();
    if ( nResult == 0 || nResult == 1 )
    {
        layout->addChild(getUiFirstBonus());
    }
    
    layout->addChild(getUiGrowthPrison());
    layout->addChild(getUiGrowthNormal());
    layout->addChild(getUiWeekBanner());
    layout->requestDoLayout();
}

cocos2d::ui::Layout* UILayoutCashHotDeal::getUiGrowthNormal()
{
    Size size = Size(696, 184);
    
    auto layout = ui::Layout::create();
    layout->setContentSize(size);
    layout->setIgnoreAnchorPointForPosition(false);
    layout->setTouchEnabled(true);
    layout->addClickEventListener([=](Ref*){

        auto popup = PopupPurchaseGrowth::create("FLOOR");
        popup->show();
    });
    
    auto uiBG = ui::ImageView::create("Assets/ui/purchase/hot_deal_banner_02.png");
    uiBG->setPosition(Vec2(size.width / 2, size.height / 2));
    ((ui::Scale9Sprite*)uiBG->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
    uiBG->setScale(4);
    layout->addChild(uiBG);
    
    auto uiName = ui::CText::create(GAME_TEXT("t_cash_title_402"), GAME_FONT, 28);
    uiName->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    uiName->setPosition(Vec2(20, size.height - 32));
    uiName->setTextColor(Color4B(255, 200, 20, 255));
    layout->addChild(uiName);
    
    auto uiDesc = ui::CText::create(GAME_TEXT("t_ui_shop_growth_402"), GAME_FONT, 24);
    uiDesc->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    uiDesc->setPosition(Vec2(25, 28));
    uiDesc->enableOutline(Color4B::BLACK, 2);
    layout->addChild(uiDesc);
    
    // reward
    std::vector<int> listReward;
    auto listGrowth = CashManager::getInstance()->getGrowthGroup("FLOOR");
    for ( int i = 0; i < listGrowth.size(); i++ )
    {
        auto objGrowth = listGrowth.at(i);
        
        Vector<InfoItems*> listFree = objGrowth->getItemsFree();
        for( auto& item : listFree )
        {
            listReward.push_back(item->getIdx());
        }
        
        Vector<InfoItems*> listCash = objGrowth->getItemsCash();
        for( auto& item : listCash )
        {
            listReward.push_back(item->getIdx());
        }
    }
    std::sort(listReward.begin(), listReward.end());
    listReward.erase(unique(listReward.begin(), listReward.end()), listReward.end());
    
    for ( int i = 0; i < listReward.size(); i++ )
    {
        int idx = listReward.at(i);
        
        auto objItem = ItemsManager::getInstance()->getInfoItems(idx);
        
        auto uiItemBG = ui::ImageView::create("Assets/ui_common/box_item.png");
        uiItemBG->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        uiItemBG->setPosition(Vec2(25 + i * 50 + i * 5, 95));
        layout->addChild(uiItemBG);
        
        auto uiItemIcon = ui::ImageView::create(objItem->getPath());
        uiItemIcon->setPosition(Vec2(uiItemBG->getContentSize().width / 2, uiItemBG->getContentSize().height / 2));
        uiItemBG->addChild(uiItemIcon);
    }
    
    // margin
    auto params = ui::LinearLayoutParameter::create();
    params->setMargin(ui::Margin(0, 0, 0, 5));
    params->setGravity(ui::LinearLayoutParameter::LinearGravity::CENTER_HORIZONTAL);
    layout->setLayoutParameter(params);
    
    return layout;
}

cocos2d::ui::Layout* UILayoutCashHotDeal::getUiGrowthPrison()
{
    Size size = Size(696, 184);
    
    auto layout = ui::Layout::create();
    layout->setContentSize(size);
    layout->setIgnoreAnchorPointForPosition(false);
    layout->setTouchEnabled(true);
    layout->addClickEventListener([=](Ref*){

        auto popup = PopupPurchaseGrowth::create("PRISON_FLOOR");
        popup->show();
    });
    
    auto uiBG = ui::ImageView::create("Assets/ui/purchase/hot_deal_banner_01.png");
    uiBG->setPosition(Vec2(size.width / 2, size.height / 2));
    ((ui::Scale9Sprite*)uiBG->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
    uiBG->setScale(4);
    layout->addChild(uiBG);
    
    auto uiName = ui::CText::create(GAME_TEXT("t_cash_title_403"), GAME_FONT, 28);
    uiName->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    uiName->setPosition(Vec2(20, size.height - 32));
    uiName->setTextColor(Color4B(255, 200, 20, 255));
    layout->addChild(uiName);
    
    auto uiDesc = ui::CText::create(GAME_TEXT("t_ui_shop_growth_403"), GAME_FONT, 24);
    uiDesc->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    uiDesc->setPosition(Vec2(25, 28));
    uiDesc->enableOutline(Color4B::BLACK, 2);
    layout->addChild(uiDesc);
    
    // reward
    std::vector<int> listReward;
    auto listGrowth = CashManager::getInstance()->getGrowthGroup("PRISON_FLOOR");
    for ( int i = 0; i < listGrowth.size(); i++ )
    {
        auto objGrowth = listGrowth.at(i);
        
        Vector<InfoItems*> listFree = objGrowth->getItemsFree();
        for( auto& item : listFree )
        {
            listReward.push_back(item->getIdx());
        }
        
        Vector<InfoItems*> listCash = objGrowth->getItemsCash();
        for( auto& item : listCash )
        {
            listReward.push_back(item->getIdx());
        }
    }
    std::sort(listReward.begin(), listReward.end());
    listReward.erase(unique(listReward.begin(), listReward.end()), listReward.end());
    
    for ( int i = 0; i < listReward.size(); i++ )
    {
        int idx = listReward.at(i);
        
        auto objItem = ItemsManager::getInstance()->getInfoItems(idx);
        
        auto uiItemBG = ui::ImageView::create("Assets/ui_common/box_item.png");
        uiItemBG->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        uiItemBG->setPosition(Vec2(25 + i * 50 + i * 5, 95));
        layout->addChild(uiItemBG);
        
        auto uiItemIcon = ui::ImageView::create(objItem->getPath());
        uiItemIcon->setPosition(Vec2(uiItemBG->getContentSize().width / 2, uiItemBG->getContentSize().height / 2));
        uiItemBG->addChild(uiItemIcon);
    }
    
    // margin
    auto params = ui::LinearLayoutParameter::create();
    params->setMargin(ui::Margin(0, 0, 0, 5));
    params->setGravity(ui::LinearLayoutParameter::LinearGravity::CENTER_HORIZONTAL);
    layout->setLayoutParameter(params);
    
    return layout;
}

cocos2d::ui::Layout* UILayoutCashHotDeal::getUiWeekBanner()
{
    Size size = Size(696, 184);
    
    auto layout = ui::Layout::create();
    layout->setContentSize(size);
    layout->setIgnoreAnchorPointForPosition(false);
    layout->setTouchEnabled(true);
    layout->addClickEventListener([=](Ref*){

        auto popup = PopupPurchaseBanner::create();
        popup->show();
    });
    
    auto uiBG = ui::ImageView::create("Assets/ui/purchase/hot_deal_banner_03.png");
    uiBG->setPosition(Vec2(size.width / 2, size.height / 2));
    ((ui::Scale9Sprite*)uiBG->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
    uiBG->setScale(4);
    layout->addChild(uiBG);
    
    auto uiName = ui::CText::create(GAME_TEXT("t_ui_week_banner"), GAME_FONT, 28);
    uiName->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    uiName->setPosition(Vec2(20, size.height - 32));
    uiName->setTextColor(Color4B(255, 200, 20, 255));
    layout->addChild(uiName);
    
    auto uiDesc = ui::CText::create(GAME_TEXT("t_ui_shop_growth_403"), GAME_FONT, 24);
    uiDesc->setPosition(Vec2(size.width / 2, 22));
    uiDesc->setTextColor(Color4B(255, 240, 70, 255));
    uiDesc->enableOutline(Color4B::BLACK, 2);
    layout->addChild(uiDesc);
    {
        auto action1 = CallFuncN::create([=](Ref* sender){
        
            if ( EventManager::getInstance()->isActivatedEvent(21) == false && EventManager::getInstance()->isActivatedEvent(22) == false )
            {
                ((ui::CText*)sender)->setString(GAME_TEXTFORMAT("t_ui_popshop_time", "00:00:00"));
                ((ui::CText*)sender)->stopAllActions();
                return;
            }

            int64_t timeDiff = CashManager::getInstance()->getBannerTimeRemaining();
            if ( timeDiff < 0 )
            {
                ((ui::CText*)sender)->setString(GAME_TEXTFORMAT("t_ui_popshop_time", "00:00:00"));
                ((ui::CText*)sender)->stopAllActions();
                return;
            }

            std::string timeText = UtilsDate::getInstance()->getTimeString(timeDiff, UtilsDate::eType::date_user_type_1);
            ((ui::CText*)sender)->setString(GAME_TEXTFORMAT("t_ui_popshop_time", timeText.c_str()));
        });
        auto action2 = DelayTime::create(1.0);
        uiDesc->runAction(RepeatForever::create(Sequence::create(action1, action2, nullptr)));
    }
    
    // margin
    auto params = ui::LinearLayoutParameter::create();
    params->setMargin(ui::Margin(0, 0, 0, 5));
    params->setGravity(ui::LinearLayoutParameter::LinearGravity::CENTER_HORIZONTAL);
    layout->setLayoutParameter(params);
    
    return layout;
}

cocos2d::ui::Layout* UILayoutCashHotDeal::getUiFirstBonus()
{
    Size size = Size(696, 184);
    
    auto layout = ui::Layout::create();
    layout->setContentSize(size);
    layout->setIgnoreAnchorPointForPosition(false);
    layout->setTouchEnabled(true);
    layout->setName("UI_FRISTBONUS");
    layout->addClickEventListener([=](Ref*){

        auto popup = PopupPurchaseFirst::create();
        popup->setCallbackResult([=](int nType){
           
            if ( nType == 5 )
            {
                _uiContainerInfo->removeChildByName("UI_FRISTBONUS");
                _uiContainerInfo->requestDoLayout();
            }
        });
        popup->show();
    });
    
    auto uiBG = ui::ImageView::create("Assets/ui/purchase/hot_deal_banner_04.png");
    uiBG->setPosition(Vec2(size.width / 2, size.height / 2));
    ((ui::Scale9Sprite*)uiBG->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
    uiBG->setScale(4);
    layout->addChild(uiBG);
    
    auto uiName = ui::CText::create(GAME_TEXT("t_ui_purchase_bonus_msg_1"), GAME_FONT, 28);
    uiName->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    uiName->setPosition(Vec2(20, size.height - 32));
    uiName->setTextColor(Color4B(255, 200, 20, 255));
    layout->addChild(uiName);
    
    auto uiDesc = ui::CText::create(GAME_TEXT("t_ui_purchase_bonus_msg_3"), GAME_FONT, 24);
    uiDesc->setPosition(Vec2(size.width / 2, 22));
    uiDesc->setTextColor(Color4B(85, 250, 255, 255));
    uiDesc->enableOutline(Color4B::BLACK, 2);
    layout->addChild(uiDesc);
 
    
    // margin
    auto params = ui::LinearLayoutParameter::create();
    params->setMargin(ui::Margin(0, 0, 0, 5));
    params->setGravity(ui::LinearLayoutParameter::LinearGravity::CENTER_HORIZONTAL);
    layout->setLayoutParameter(params);
    
    return layout;
}

#pragma mark - draw
void UILayoutCashHotDeal::drawInfo()
{
    auto layout = _uiContainerInfo;
}

#pragma mark - set, get

#pragma mark - callback

#pragma mark - click
