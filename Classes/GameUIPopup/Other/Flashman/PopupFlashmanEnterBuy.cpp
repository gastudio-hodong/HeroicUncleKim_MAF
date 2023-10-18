//
//  PopupFlashmanEnterBuy.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/12/03.
//

#include "PopupFlashmanEnterBuy.h"

#include "Common/ConfigGameNormal.h"

#include "GameUI/UI/UICurrency.h"
#include "GameUI/UI/UIBuff.h"
#include "GameUI/UI/UIReddot.h"

#include "GameUIPopup/Other/Ads/PopupAdsMafGames.h"

#include "ManagerGame/FlashmanManager.h"
#include "ManagerGame/ItemsManager.h"
#include "ManagerGame/CashManager.h"

USING_NS_CC;

PopupFlashmanEnterBuy* PopupFlashmanEnterBuy::create()
{
    PopupFlashmanEnterBuy* pRet = new(std::nothrow) PopupFlashmanEnterBuy();
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

PopupFlashmanEnterBuy::PopupFlashmanEnterBuy() :
_bInitUI(false),

//

// widget
_uiContentsUIDefault(nullptr),
_uiContentsUIButtons(nullptr)
{
    
}

PopupFlashmanEnterBuy::~PopupFlashmanEnterBuy(void)
{
    
}

bool PopupFlashmanEnterBuy::init()
{
    if ( !PopupBase::init() )
    {
        return false;
    }
    
    //
    setName("PopupFlashmanEnterBuy");
    
    //
    initVar();
    initUI();
    
    return true;
}

void PopupFlashmanEnterBuy::onEnter()
{
    PopupBase::onEnter();
    
    if ( _bInitUI == false )
    {
        _bInitUI = true;
        
        //
        uiContainer();
        uiDefault();
        uiButtons();
    }
    
    //
    drawButtons();
}

#pragma mark - init
void PopupFlashmanEnterBuy::initVar()
{
    
}

void PopupFlashmanEnterBuy::initUI()
{
    // size
    Size size = Size(500, 444);

    _spriteContainer->setContentSize( size );
    
    // label title
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_notice"), GAME_FONT, 32);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5);
    _spriteContainer->addChild(labelTitle);
    
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupFlashmanEnterBuy::onClickClose, this));
    itemClose->setPosition(_spriteContainer->getContentSize().width - 50, _spriteContainer->getContentSize().height - 37.5);
    menu->addChild(itemClose);
}

#pragma mark - ui
void PopupFlashmanEnterBuy::uiContainer()
{
    Size size = _spriteContainer->getContentSize();
    
    auto layout = ui::Layout::create();
    layout->setContentSize(Size(size.width, size.height));
    layout->setIgnoreAnchorPointForPosition(false);
    layout->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    layout->setPosition(Vec2(_spriteContainer->getContentSize().width / 2, 0));
    _spriteContainer->addChild(layout);
    
    //
    _uiContentsUIButtons = ui::Layout::create();
    _uiContentsUIButtons->setContentSize( Size(size.width, 78) );
    _uiContentsUIButtons->setIgnoreAnchorPointForPosition(false);
    _uiContentsUIButtons->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _uiContentsUIButtons->setPosition( Vec2(size.width / 2, 25) );
    _uiContentsUIButtons->setLayoutType(ui::Layout::Type::HORIZONTAL);
    layout->addChild(_uiContentsUIButtons);
    
    _uiContentsUIDefault = ui::Layout::create();
    _uiContentsUIDefault->setContentSize( Size(size.width, size.height - 75 - 25 - _uiContentsUIButtons->getContentSize().height - 25) );
    _uiContentsUIDefault->setIgnoreAnchorPointForPosition(false);
    _uiContentsUIDefault->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _uiContentsUIDefault->setPosition( Vec2(size.width / 2, size.height - 75) );
    layout->addChild(_uiContentsUIDefault);
}

void PopupFlashmanEnterBuy::uiDefault()
{
    auto layout = _uiContentsUIDefault;
    Size size = layout->getContentSize();
    
    auto uiText = ui::CText::create(GAME_TEXT("t_ui_minigame_popup_enter"), GAME_FONT, 24);
    uiText->setPosition(Vec2(size.width / 2, size.height / 2));
    uiText->setTextAreaSize(Size(size.width - 10, uiText->getContentSize().height));
    uiText->setTextOverflow(Label::Overflow::SHRINK);
    uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
    uiText->setTextHorizontalAlignment(TextHAlignment::CENTER);
    layout->addChild(uiText);
}

void PopupFlashmanEnterBuy::uiButtons()
{
    auto layout = _uiContentsUIButtons;
    Size size = layout->getContentSize();
    
    auto uiBtnBuyAds = ui::Button::create("Assets/ui_common/btn_ok.png");
    uiBtnBuyAds->addClickEventListener(CC_CALLBACK_1(PopupFlashmanEnterBuy::onClickBuyAds, this));
    uiBtnBuyAds->setName("UI_BTN_ADS");
    layout->addChild(uiBtnBuyAds);
    {
        auto uiText = UtilsWidget::getInstance()->createIconText("Assets/icon/icon_ad_1_1.png", GAME_TEXT("t_ui_randombox_msg_2"), uiBtnBuyAds->getContentSize().width - 10, 24, 15);
        uiText->setPosition(Vec2(uiBtnBuyAds->getContentSize().width / 2, uiBtnBuyAds->getContentSize().height - 3 - 18));
        uiBtnBuyAds->addChild(uiText);
        
        auto uiCount = ui::CText::create("(0/0)", GAME_FONT, 24);
        uiCount->setPosition(Vec2(uiBtnBuyAds->getContentSize().width / 2, 3 + 18));
        uiCount->setName("UI_COUNT");
        uiBtnBuyAds->addChild(uiCount);
    }
    
    auto uiBtnBuyItem = ui::Button::create("Assets/ui_common/btn_ok.png");
    uiBtnBuyItem->addClickEventListener(CC_CALLBACK_1(PopupFlashmanEnterBuy::onClickBuyItem, this));
    uiBtnBuyItem->setName("UI_BTN_ITEM");
    layout->addChild(uiBtnBuyItem);
    {
        auto params = ui::LinearLayoutParameter::create();
        params->setMargin(ui::Margin(10, 0, 0, 0));
        uiBtnBuyItem->setLayoutParameter(params);
    }
}

#pragma mark - draw
void PopupFlashmanEnterBuy::drawButtons()
{
    auto layout = _uiContentsUIButtons;
    
    if ( auto uiBtn = layout->getChildByName<ui::Button*>("UI_BTN_ADS"); uiBtn != nullptr )
    {
        int count = FlashmanManager::getInstance()->getEnterBuyAdsCount();
        int countMax = FlashmanManager::getInstance()->getEnterBuyAdsCountMax();
        
        auto uiCount = uiBtn->getChildByName<ui::CText*>("UI_COUNT");
        uiCount->setString(MafUtils::format("(%d/%d)", countMax - count, countMax));
    }
    
    if ( auto uiBtn = layout->getChildByName<ui::Button*>("UI_BTN_ITEM"); uiBtn != nullptr )
    {
        uiBtn->removeChildByName("UI_COST");
        
        int price = FlashmanManager::getInstance()->getEnterBuyPrice();
        
        auto uiCost = UtilsWidget::getInstance()->createIconText("Assets/icon/icon_coin_2.png", std::to_string(price), uiBtn->getContentSize().width - 10, 24, 15);
        uiCost->setPosition(Vec2(uiBtn->getContentSize().width / 2, uiBtn->getContentSize().height / 2));
        uiCost->setName("UI_COST");
        uiBtn->addChild(uiCost);
    }
    
    //
    auto size = UtilsWidget::getInstance()->getLayoutAccumulatedSize(layout);
    layout->setContentSize(size);
    layout->requestDoLayout();
}

 
#pragma mark - set, get

#pragma mark - callback
void PopupFlashmanEnterBuy::callbackAds(ADS_RESULT result)
{
    if ( result == ADS_RESULT::SUCCESS )
    {
        //
        MafRewardAdsManager::getInstance()->setHistoryData(kAdsSlotMiniGame, CashManager::getInstance()->isAdsfreeTime());
        
        //
        FlashmanManager::getInstance()->requestBuyTicketAds(true, CC_CALLBACK_2(PopupFlashmanEnterBuy::callbackBuy, this));
    }
    else
    {
        if ( result == ADS_RESULT::LOAD )
        {
            auto popup = PopupAdsMafGames::create();
            popup->setCallback(CC_CALLBACK_1(PopupFlashmanEnterBuy::callbackAds, this));
            popup->setLogParams(kAnalEventAdsLoadParameterSlot);
            popup->setLogValue(kAdsSlotMiniGame);
            popup->show();
        }
        else
        {
            std::string strMessage = GAME_TEXT("t_ui_error_14");
            
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), strMessage);
            popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
            popup->show();
        }
    }
}

void PopupFlashmanEnterBuy::callbackBuy(bool bResult, int nResult)
{
    if ( bResult == false )
    {
        std::string strMessage = GAME_TEXT("t_ui_error_30");
        if ( nResult == 10002 )
        {
            strMessage = GAME_TEXT("t_ui_error_31");
        }
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), strMessage);
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_costume_skin_msg_4"));
    popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
    popup->show();
    
    //
    drawButtons();
}

#pragma mark - schedule

#pragma mark - click
void PopupFlashmanEnterBuy::onClickClose(Ref* sender)
{
    hide();
}

void PopupFlashmanEnterBuy::onClickBuyItem(cocos2d::Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE, E_SOUND_TYPE::CONTENTS);
    
    auto uiBUtton = dynamic_cast<ui::Button*>(sender);
    
    // cost
    auto itemNeed = std::to_string(FlashmanManager::getInstance()->getEnterBuyPrice());
    auto itemNow = ItemsManager::getInstance()->getItems(E_ITEMS::COSTUME_COIN);
    if ( MafUtils::compareStr(itemNow, itemNeed) == false )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_31"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    //
    FlashmanManager::getInstance()->requestBuyTicket(true, CC_CALLBACK_2(PopupFlashmanEnterBuy::callbackBuy, this));
}

void PopupFlashmanEnterBuy::onClickBuyAds(cocos2d::Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE, E_SOUND_TYPE::CONTENTS);
    
    auto uiBUtton = dynamic_cast<ui::Button*>(sender);
    
    int count = FlashmanManager::getInstance()->getEnterBuyAdsCount();
    int countMax = FlashmanManager::getInstance()->getEnterBuyAdsCountMax();
    if ( count >= countMax )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_15"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    //
    if ( CashManager::getInstance()->isAdsfreeTime() == true )
    {
        callbackAds(ADS_RESULT::SUCCESS);
    }
    else
    {
        MafGoogleRewardAds::ShowGoogleRewardedVideo(this, CC_CALLBACK_1(PopupFlashmanEnterBuy::callbackAds, this));
    }
}
