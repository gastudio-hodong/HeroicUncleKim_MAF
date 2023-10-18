//
//  PopupEventSunfishAdventure.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 09/01/2020.
//

#include "PopupEventSunfishAdventure.h"

#include "Common/ConfigGameNormal.h"

#include "GameUIPopup/Other/Ads/PopupAdsMafGames.h"
#include "GameUIPopup/OtherEvent/EventSunfish/StateStep/PopupEventSunfishAdventureResult.h"

#include "ManagerGame/CashManager.h"
#include "ManagerEvent/EventSunfishManager.h"

USING_NS_CC;

PopupEventSunfishAdventure::PopupEventSunfishAdventure(void) :
_bInitUI(false),

//
_eStep(E_STEP::USE),
_levelIdx(0),
_adventureIdx(0),

//
_uiContentsInfo(nullptr),
_uiContentsButton(nullptr),

_uiTitle(nullptr)
{
    
}

PopupEventSunfishAdventure::~PopupEventSunfishAdventure(void)
{
    
}

bool PopupEventSunfishAdventure::init()
{
    if ( !PopupBase::init() )
    {
        return false;
    }
    
    // init
    initVar();
    initUi();
    
    return true;
}

void PopupEventSunfishAdventure::onEnter()
{
    PopupBase::onEnter();
 
    if ( _bInitUI == false )
    {
        uiContainer();
        uiInfo();
        uiButtons();
        
        _bInitUI = true;
    }
    
    //
    drawInfo();
    drawButtons();
}


#pragma mark - init
void PopupEventSunfishAdventure::initVar()
{
    
}

void PopupEventSunfishAdventure::initUi()
{
    // size
    _spriteContainer->setContentSize( Size(700, 818) );
    
    // label title
    _uiTitle = ui::CText::create(GAME_TEXT("t_ui_event_sunfish_popup_title_1"), GAME_FONT, 32);
    _uiTitle->setPosition(Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5));
    _uiTitle->setTextAreaSize(Size(_spriteContainer->getContentSize().width - 200, _uiTitle->getContentSize().height));
    _uiTitle->setTextOverflow(Label::Overflow::SHRINK);
    _uiTitle->setTextVerticalAlignment(TextVAlignment::CENTER);
    _uiTitle->setTextHorizontalAlignment(TextHAlignment::CENTER);
    _uiTitle->setTextColor(Color4B(COLOR_TITLE));
    _spriteContainer->addChild(_uiTitle);
    
    // item close
    auto uiClose = ui::Button::create("Assets/ui_common/btn_x.png");
    uiClose->setPosition(Vec2(_spriteContainer->getContentSize().width - uiClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5));
    uiClose->addClickEventListener(CC_CALLBACK_1(PopupEventSunfishAdventure::onClickClose, this));
    _spriteContainer->addChild(uiClose);
}

#pragma mark - ui
void PopupEventSunfishAdventure::uiContainer()
{
    Size size = _spriteContainer->getContentSize();
    size.width = size.width - 10;
    size.height = size.height - 75 - 25;
    
    auto layout = ui::Layout::create();
    layout->setContentSize(Size(size.width, size.height));
    layout->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    layout->setPosition(Vec2(size.width / 2, 25));
    _spriteContainer->addChild(layout);
    
    //
    _uiContentsButton = ui::Layout::create();
    _uiContentsButton->setContentSize(Size(size.width, 78));
    _uiContentsButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _uiContentsButton->setPosition(Vec2(size.width / 2, 0));
    layout->addChild(_uiContentsButton);
    
    _uiContentsInfo = ui::Layout::create();
    _uiContentsInfo->setContentSize(Size(size.width, size.height - _uiContentsButton->getContentSize().height));
    _uiContentsInfo->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _uiContentsInfo->setPosition(Vec2(size.width / 2, size.height));
    layout->addChild(_uiContentsInfo);
}

void PopupEventSunfishAdventure::uiInfo()
{
    auto layout = _uiContentsInfo;
    auto size = layout->getContentSize();
    
    //
    auto uiTop = ui::Layout::create();
    uiTop->setContentSize(Size(size.width, 536));
    uiTop->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    uiTop->setPosition(Vec2(size.width / 2, size.height));
    layout->addChild(uiTop);
    {
        //
        auto uiInfoTitle = ui::Layout::create();
        uiInfoTitle->setContentSize(Size(uiTop->getContentSize().width, 248));
        uiInfoTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        uiInfoTitle->setPosition(Vec2(uiTop->getContentSize().width / 2, uiTop->getContentSize().height - 5));
        uiInfoTitle->setName("UI_INFO_TITLE");
        uiInfoTitle->setClippingEnabled(true);
        uiTop->addChild(uiInfoTitle);
        {
            auto uiBG = ui::ImageView::create("Assets/ui/event/lobster_bg_1_1.png");
            uiBG->setPosition(Vec2(uiInfoTitle->getContentSize().width / 2, uiInfoTitle->getContentSize().height / 2));
            ((ui::Scale9Sprite*)uiBG->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
            uiBG->setScale(3);
            uiBG->setName("UI_BG");
            uiInfoTitle->addChild(uiBG);
        }
         
        //
        auto uiInfoDesc = ui::Layout::create();
        uiInfoDesc->setContentSize(Size(uiTop->getContentSize().width, 278));
        uiInfoDesc->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        uiInfoDesc->setPosition(Vec2(uiTop->getContentSize().width / 2, 0));
        uiInfoDesc->setName("UI_INFO_DESC");
        uiTop->addChild(uiInfoDesc);
        {
            auto uiBG = ui::ImageView::create("Assets/ui_common/box_05_1.png");
            uiBG->setScale9Enabled(true);
            uiBG->setCapInsets(Rect(12, 12, 8, 8));
            uiBG->setContentSize(uiInfoDesc->getContentSize());
            uiBG->setPosition(Vec2(uiInfoDesc->getContentSize().width / 2, uiInfoDesc->getContentSize().height / 2));
            uiInfoDesc->addChild(uiBG);
            
            auto uiDesc = ui::CText::create("", GAME_FONT, 24);
            uiDesc->setPosition(Vec2(uiInfoDesc->getContentSize().width / 2, uiInfoDesc->getContentSize().height / 2));
            uiDesc->setTextAreaSize(Size(uiInfoDesc->getContentSize().width - 20, uiInfoDesc->getContentSize().height - 20));
            uiDesc->setTextOverflow(Label::Overflow::SHRINK);
            uiDesc->setTextVerticalAlignment(TextVAlignment::CENTER);
            uiDesc->setTextHorizontalAlignment(TextHAlignment::CENTER);
            uiDesc->setName("UI_DESC");
            uiInfoDesc->addChild(uiDesc);
        }
    }
    
    //
    auto uiInfo = ui::Layout::create();
    uiInfo->setContentSize(Size(size.width, size.height - uiTop->getContentSize().height));
    uiInfo->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    uiInfo->setPosition(Vec2(size.width / 2, 0));
    uiInfo->setName("UI_INFO");
    layout->addChild(uiInfo);
    {
        auto uiTime = ui::CText::create(GAME_TEXTFORMAT("t_ui_event_sunfish_adventure_msg_2", 0, 0, 0, 0), GAME_FONT, 28);
        uiTime->setPosition(Vec2(uiInfo->getContentSize().width / 2, uiInfo->getContentSize().height / 2));
        uiTime->setTextAreaSize(Size(uiInfo->getContentSize().width - 10, uiTime->getContentSize().height));
        uiTime->setTextOverflow(Label::Overflow::SHRINK);
        uiTime->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiTime->setTextHorizontalAlignment(TextHAlignment::CENTER);
        uiTime->setTextColor(Color4B::YELLOW);
        uiTime->setName("UI_TIME");
        uiInfo->addChild(uiTime);
        
        auto uiRate = ui::CText::create(GAME_TEXTFORMAT("t_ui_event_sunfish_survival_rate", 0), GAME_FONT, 28);
        uiRate->setPosition(Vec2(uiInfo->getContentSize().width / 2, uiInfo->getContentSize().height / 2));
        uiRate->setTextAreaSize(Size(uiInfo->getContentSize().width - 10, uiRate->getContentSize().height));
        uiRate->setTextOverflow(Label::Overflow::SHRINK);
        uiRate->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiRate->setTextHorizontalAlignment(TextHAlignment::CENTER);
        uiRate->setTextColor(Color4B::YELLOW);
        uiRate->setName("UI_RATE");
        uiInfo->addChild(uiRate);

    }
    
}

void PopupEventSunfishAdventure::uiButtons()
{
    auto layout = _uiContentsButton;
    auto size = layout->getContentSize();
    
    auto uiButtons = ui::Layout::create();
    uiButtons->setIgnoreAnchorPointForPosition(false);
    uiButtons->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    uiButtons->setPosition(Vec2(size.width / 2, size.height / 2));
    uiButtons->setLayoutType(ui::Layout::Type::HORIZONTAL);
    uiButtons->setName("UI_BUTTONS");
    layout->addChild(uiButtons);
    {
        // ad
        auto uiAD = ui::Button::create("Assets/ui_common/btn_1_7_on.png");
        uiAD->setScale9Enabled(true);
        uiAD->setCapInsets(Rect(33,33,2,2));
        uiAD->setContentSize(Size(180, 78));
        uiAD->setName("UI_BUTTON_AD");
        uiAD->addClickEventListener(CC_CALLBACK_1(PopupEventSunfishAdventure::onClickAD, this));
        uiButtons->addChild(uiAD);
        {
            auto uiIcon = ui::ImageView::create("Assets/ui_common/btn_ads_play.png");
            uiIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
            uiIcon->setPosition(Vec2(uiAD->getContentSize().width / 2 - 15, uiAD->getContentSize().height / 2));
            uiAD->addChild(uiIcon);
            
            auto uiCount = ui::CText::create("0/0", GAME_FONT, 28);
            uiCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            uiCount->setPosition(Vec2(uiAD->getContentSize().width / 2, uiAD->getContentSize().height / 2));
            uiCount->setName("UI_COUNT");
            uiAD->addChild(uiCount);
        }
        
        // use
        auto uiUse = ui::Button::create("Assets/ui_common/btn_ok.png");
        uiUse->addClickEventListener(CC_CALLBACK_1(PopupEventSunfishAdventure::onClickUse, this));
        uiUse->setName("UI_BUTTON_USE");
        uiButtons->addChild(uiUse);
        {
            auto params = ui::LinearLayoutParameter::create();
            params->setMargin(ui::Margin(10, 0, 0, 0));
            uiUse->setLayoutParameter(params);
            
            auto uiText = ui::CText::create(GAME_TEXT("t_ui_event_sunfish_bt_4"), GAME_FONT, 24);
            uiText->setPosition(uiUse->getContentSize()/2);
            uiText->setTextAreaSize(Size(uiUse->getContentSize().width - 10, uiText->getContentSize().height));
            uiText->setTextOverflow(Label::Overflow::SHRINK);
            uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
            uiText->setTextHorizontalAlignment(TextHAlignment::CENTER);
            uiUse->addChild(uiText);
        }
        
        // back
        auto uiBack = ui::Button::create("Assets/ui_common/btn_ok.png");
        uiBack->setName("UI_BUTTON_BACK");
        uiBack->addClickEventListener(CC_CALLBACK_1(PopupEventSunfishAdventure::onClickClose, this));
        uiButtons->addChild(uiBack);
        {
            auto uiText = ui::CText::create(GAME_TEXT("t_ui_event_sunfish_return_bt"), GAME_FONT, 24);
            uiText->setPosition(uiBack->getContentSize()/2);
            uiText->setTextAreaSize(Size(uiBack->getContentSize().width - 10, uiText->getContentSize().height));
            uiText->setTextOverflow(Label::Overflow::SHRINK);
            uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
            uiText->setTextHorizontalAlignment(TextHAlignment::CENTER);
            uiBack->addChild(uiText);
        }
        
        // start
        auto uiStart = ui::Button::create("Assets/ui_common/btn_ok.png");
        uiStart->addClickEventListener(CC_CALLBACK_1(PopupEventSunfishAdventure::onClickStart, this));
        uiStart->setName("UI_BUTTON_START");
        uiButtons->addChild(uiStart);
        {
            auto params = ui::LinearLayoutParameter::create();
            params->setMargin(ui::Margin(10, 0, 0, 0));
            uiStart->setLayoutParameter(params);
            
            auto uiText = ui::CText::create(GAME_TEXT("t_ui_event_sunfish_bt_4"), GAME_FONT, 24);
            uiText->setPosition(uiStart->getContentSize()/2);
            uiText->setTextAreaSize(Size(uiStart->getContentSize().width - 10, uiText->getContentSize().height));
            uiText->setTextOverflow(Label::Overflow::SHRINK);
            uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
            uiText->setTextHorizontalAlignment(TextHAlignment::CENTER);
            uiStart->addChild(uiText);
        }

    }
}

#pragma mark - ui : draw
void PopupEventSunfishAdventure::drawInfo()
{
    auto layout = _uiContentsInfo;
    
    auto adventureIdx = _adventureIdx.valueInt();
    auto objSunfishAdventure = EventSunfishManager::getInstance()->getInfoSunfishAdventure(adventureIdx);
    
    //
    if ( auto uiInfoTitle = utils::findChild<ui::Layout*>(layout, "UI_INFO_TITLE"); uiInfoTitle != nullptr )
    {
        auto uiBG = uiInfoTitle->getChildByName<ui::ImageView*>("UI_BG");
        
        if ( uiInfoTitle->getChildByName<ui::ImageView*>("UI_ITEM_BG") != nullptr )
        {
            uiInfoTitle->removeChildByName("UI_ITEM_BG");
        }
        
        //
        if ( _eStep == E_STEP::USE )
        {
            uiBG->loadTexture("Assets/ui/event/lobster_bg_1_1.png");
        }
        else if ( _eStep == E_STEP::START )
        {
            uiBG->loadTexture(MafUtils::format("Assets/ui/event/lobster_bg_1_%d.png", adventureIdx));
            
            //
            if ( EventSunfishManager::getInstance()->isReward(EventSunfishManager::E_TYPE::ADVENTURE, adventureIdx) == false )
            {
                auto listReward = objSunfishAdventure->getReward();
                auto objItem = listReward.at(0);
                
                auto uiItemBG = ui::ImageView::create("Assets/ui_common/list_bg_s_3_1.png");
                uiItemBG->setScale9Enabled(true);
                uiItemBG->setCapInsets(Rect(30, 50, 10, 10));
                uiItemBG->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
                uiItemBG->setPosition(Vec2(uiInfoTitle->getContentSize().width - 10, 10));
                uiItemBG->setName("UI_ITEM_BG");
                uiInfoTitle->addChild(uiItemBG);
                {
                    auto uiItemIcon = ui::ImageView::create(objItem->getPath());
                    uiItemIcon->setPosition(Vec2(uiItemBG->getContentSize().width / 2, 45));
                    uiItemBG->addChild(uiItemIcon);
                    
                    std::string rewardCount = objItem->getCount();
                    if ( objItem->isString() == true )
                    {
                        MafUtils::convertNumberToShort(rewardCount);
                    }
                    
                    auto uiItemCount = ui::CText::create(rewardCount, GAME_FONT, 20);
                    uiItemCount->setPosition(Vec2(uiItemBG->getContentSize().width / 2, 20));
                    uiItemCount->enableOutline(Color4B::BLACK, 2);
                    uiItemBG->addChild(uiItemCount);
                }
                
                auto uiItemTitle = ui::CText::create(GAME_TEXT("t_ui_event_sunfish_first_reward"), GAME_FONT, 20);
                uiItemTitle->setPosition(Vec2(uiItemBG->getContentSize().width / 2, uiItemBG->getContentSize().height - 19));
                uiItemTitle->setTextAreaSize(Size(uiItemBG->getContentSize().width - 10, uiItemTitle->getContentSize().height));
                uiItemTitle->setTextOverflow(Label::Overflow::SHRINK);
                uiItemTitle->setTextVerticalAlignment(TextVAlignment::CENTER);
                uiItemTitle->setTextHorizontalAlignment(TextHAlignment::CENTER);
                uiItemBG->addChild(uiItemTitle);
            }
        }
    }
    
    //
    if ( auto uiInfoDesc = utils::findChild<ui::Layout*>(layout, "UI_INFO_DESC"); uiInfoDesc != nullptr )
    {
        auto uiDesc = uiInfoDesc->getChildByName<ui::CText*>("UI_DESC");
        
        //
        if ( _eStep == E_STEP::USE )
        {
            uiDesc->setString(GAME_TEXT("t_ui_event_sunfish_adventure_msg_1"));
        }
        else if ( _eStep == E_STEP::START )
        {
            uiDesc->setString(objSunfishAdventure->getDieDialogue());
        }
    }
    
    //
    if ( auto uiInfo = utils::findChild<ui::Layout*>(layout, "UI_INFO"); uiInfo != nullptr )
    {
        auto uiTime = uiInfo->getChildByName<ui::CText*>("UI_TIME");
        uiTime->setVisible(false);
        uiTime->stopAllActions();
        
        auto uiRate = uiInfo->getChildByName<ui::CText*>("UI_RATE");
        uiRate->setVisible(false);
        
        //
        if ( _eStep == E_STEP::USE )
        {
            auto countNow = EventSunfishManager::getInstance()->getAdventureCountNow();
            auto countMax = EventSunfishManager::getInstance()->getAdventureCountMax();
            
            auto timeNow = EventSunfishManager::getInstance()->getAdventureTimeNow();
            if ( countNow >= countMax )
            {
                timeNow = EventSunfishManager::getInstance()->getAdventureTimeMax();
            }
            
            uiTime->setString(GAME_TEXTFORMAT("t_ui_event_sunfish_adventure_msg_2", timeNow / 60, timeNow % 60, countNow, countMax));
            uiTime->setVisible(true);
            {
                auto action1 = DelayTime::create(1.0);
                auto action2 = CallFuncN::create([=](Node* sender){
                    
                    auto uiTime = (ui::CText*)sender;
                    
                    auto countNow = EventSunfishManager::getInstance()->getAdventureCountNow();
                    auto countMax = EventSunfishManager::getInstance()->getAdventureCountMax();
                    
                    auto timeNow = EventSunfishManager::getInstance()->getAdventureTimeNow();
                    if ( countNow >= countMax )
                    {
                        timeNow = EventSunfishManager::getInstance()->getAdventureTimeMax();
                    }
                    
                    uiTime->setString(GAME_TEXTFORMAT("t_ui_event_sunfish_adventure_msg_2", timeNow / 60, timeNow % 60, countNow, countMax));
                });
                uiTime->runAction(RepeatForever::create(Sequence::create(action1, action2, NULL)));
            }
        }
        else if ( _eStep == E_STEP::START )
        {
            auto rate = (objSunfishAdventure->getSurvivalRate() + objSunfishAdventure->getBonusRate() * EventSunfishManager::getInstance()->getDie(EventSunfishManager::E_TYPE::ADVENTURE, adventureIdx)) / 10.0f;
            rate = MIN(rate, 99.9f);
            
            uiRate->setString(GAME_TEXTFORMAT("t_ui_event_sunfish_survival_rate", rate));
            uiRate->setVisible(true);
        }
    }
}

void PopupEventSunfishAdventure::drawButtons()
{
    auto layout = _uiContentsButton;
    
    auto uiButtons = layout->getChildByName<ui::Layout*>("UI_BUTTONS");

    //
    auto uiAD = uiButtons->getChildByName<ui::Button*>("UI_BUTTON_AD");
    uiAD->setVisible(false);
    
    auto uiUse = uiButtons->getChildByName<ui::Button*>("UI_BUTTON_USE");
    uiUse->setVisible(false);
    
    auto uiBack = uiButtons->getChildByName<ui::Button*>("UI_BUTTON_BACK");
    uiBack->setVisible(false);
    
    auto uiStart = uiButtons->getChildByName<ui::Button*>("UI_BUTTON_START");
    uiStart->setVisible(false);
     
    
    //
    if ( _eStep == E_STEP::USE )
    {
        int adMax = EventSunfishManager::getInstance()->getADMax();
        int adNow = adMax - EventSunfishManager::getInstance()->getADNow();
        adNow = MAX(adNow, 0);
        
        auto uiCount = utils::findChild<ui::CText*>(uiAD, "UI_COUNT");
        uiCount->setString(MafUtils::format("%d/%d", adNow, adMax));
        
        uiAD->setVisible(true);
        uiUse->setVisible(true);
        
    }
    else if ( _eStep == E_STEP::START )
    {
        uiBack->setVisible(true);
        uiStart->setVisible(true);
    }
     
    //
    auto sizeButtons = UtilsWidget::getInstance()->getLayoutAccumulatedSize(uiButtons);
    uiButtons->setContentSize(sizeButtons);
    uiButtons->requestDoLayout();
}

#pragma mark - ui : click
void PopupEventSunfishAdventure::onClickClose(Ref* sender)
{
    hide();
}

void PopupEventSunfishAdventure::onClickAD(Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    int adMax = EventSunfishManager::getInstance()->getADMax();
    int adNow = EventSunfishManager::getInstance()->getADNow();
    if ( adNow >= adMax )
    {
        return;
    }
    
    //
    if ( CashManager::getInstance()->isAdsfreeTime() == true )
    {
        callbackADResult(ADS_RESULT::SUCCESS);
    }
    else
    {
        MafGoogleRewardAds::ShowGoogleRewardedVideo(this, CC_CALLBACK_1(PopupEventSunfishAdventure::callbackADResult, this));
    }
}

void PopupEventSunfishAdventure::onClickUse(Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto uiBtn = dynamic_cast<ui::Button*>(sender);
    
    auto countNow = EventSunfishManager::getInstance()->getAdventureCountNow();
    if ( countNow <= 0 )
    {
        return;
    }
    
    EventSunfishManager::getInstance()->requestAdventureUse(true, CC_CALLBACK_3(PopupEventSunfishAdventure::callbackAdventureUse, this));
}

void PopupEventSunfishAdventure::onClickStart(Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto adventureIdx = _adventureIdx.valueInt();
    
    EventSunfishManager::getInstance()->requestAdventureStart(true, adventureIdx, CC_CALLBACK_3(PopupEventSunfishAdventure::callbackAdventureStart, this));
}

#pragma mark - callback
void PopupEventSunfishAdventure::callbackAdventureUse(bool bResult, int nResult, int adventureIdx)
{
    if ( bResult == false )
    {
        std::string strMessage = GAME_TEXT("t_ui_error_30");
        if ( nResult == 2 )
            strMessage = GAME_TEXT("t_ui_draw_msg_1");
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), strMessage);
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        
        hide();
        return;
    }
    
    //
    _eStep = E_STEP::START;
    _adventureIdx = adventureIdx;

    //
    drawInfo();
    drawButtons();
}

void PopupEventSunfishAdventure::callbackAdventureStart(bool bResult, int nResult, bool bSuccess)
{
    if ( bResult == false )
    {
        std::string strMessage = GAME_TEXT("t_ui_error_30");
        if ( nResult == 2 )
            strMessage = GAME_TEXT("t_ui_draw_msg_1");
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), strMessage);
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        
        hide();
        return;
    }
    
    //
    auto popup = PopupEventSunfishAdventureResult::create();
    popup->setLevelIdx(_levelIdx.valueInt());
    popup->setAdventureIdx(_adventureIdx.valueInt());
    popup->setResult(bSuccess);
    popup->setHideBackKey(false);
    popup->show();
    
    //
    hide();
}

void PopupEventSunfishAdventure::callbackADResult(ADS_RESULT result)
{
    if ( result == ADS_RESULT::SUCCESS )
    {
        //
        MafRewardAdsManager::getInstance()->setHistoryData(kAdsSlotMiniGame, CashManager::getInstance()->isAdsfreeTime());
        
        //
        EventSunfishManager::getInstance()->requestAD(true, CC_CALLBACK_2(PopupEventSunfishAdventure::callbackAD, this));
    }
    else
    {
        if ( result == ADS_RESULT::LOAD )
        {
            auto popup = PopupAdsMafGames::create();
            popup->setCallback(CC_CALLBACK_1(PopupEventSunfishAdventure::callbackADResult, this));
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

void PopupEventSunfishAdventure::callbackAD(bool bResult, int nResult)
{
    if ( bResult == false )
    {
        std::string strMessage = GAME_TEXT("t_ui_error_30");

        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), strMessage);
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    drawInfo();
    drawButtons();
}

#pragma mark - schedule

#pragma mark - set, get
void PopupEventSunfishAdventure::setLevelIdx(int idx)
{
    _levelIdx = idx;
    
    //
    if ( _bInitUI == true )
    {
        drawInfo();
        drawButtons();
    }
}

#pragma mark - event
