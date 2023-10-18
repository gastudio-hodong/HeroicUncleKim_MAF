//
//  PopupEventSunfish.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/11/23.
//

#include "PopupEventSunfish.h"

#include "Common/ConfigGameNormal.h"

#include "GameUI/UI/UIReddot.h"

#include "GameUIPopup/Common/PopupHelp.h"
#include "GameUIPopup/OtherEvent/EventSunfish/StateStep/PopupEventSunfishAdventure.h"

#include "GameUIPopup/OtherEvent/EventSunfish/PopupEventSunfishDex.h"
#include "GameUIPopup/OtherEvent/EventSunfish/PopupEventSunfishReinforce.h"
#include "GameUIPopup/OtherEvent/EventSunfish/Layer/LayerEventSunfishGame.h"

#include "ManagerEvent/EventManager.h"
#include "ManagerEvent/EventSunfishManager.h"
#include "ManagerGame/DialogManager.h"

USING_NS_CC;

PopupEventSunfish* PopupEventSunfish::create()
{
    PopupEventSunfish* pRet = new(std::nothrow) PopupEventSunfish();
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

PopupEventSunfish::PopupEventSunfish():
_bInitUI(false),

//
_uiContentsInfo(nullptr),
_uiContentsGame(nullptr),
_uiContentsReward(nullptr),

_uiTime(nullptr),
_uiLife(nullptr),
_uiWeight(nullptr)
{
    
}

PopupEventSunfish::~PopupEventSunfish(void)
{
    //
    EventSunfishManager::getInstance()->onDelegateInfoUpdate.Clear(this);

    //
    EventSunfishManager::getInstance()->requestWeight(false);
}

bool PopupEventSunfish::init()
{
    if (!PopupBase::init())
    {
        return false;
    }
    
    //
    initVar();
    initUI();
    
    //
    EventSunfishManager::getInstance()->onDelegateInfoUpdate += new MafFunction<void(void)>(CC_CALLBACK_0(PopupEventSunfish::callbackInfoUpdate, this), this);
    
    return true;
}

void PopupEventSunfish::onEnter()
{
    PopupBase::onEnter();
    
    if ( _bInitUI == false )
    {
        //
        uiContainer();
        uiInfo();
        uiReward();
        
        //
        EventSunfishManager::getInstance()->requestInfo(true, CC_CALLBACK_2(PopupEventSunfish::callbackInfo, this));
        
        // 시나리오 연출
        if ( DialogManager::getInstance()->isViewDialog("EVENT_SUNFISH_OPENNING") == 0 )
        {
            DialogManager::getInstance()->showDialog("EVENT_SUNFISH_OPENNING");
        }
        
        //
        _bInitUI = true;
    }
    
    //
    drawInfo();
    drawReward();
}

#pragma mark - ui
void PopupEventSunfish::initVar()
{
    auto feverTime = EventSunfishManager::getInstance()->getFeverTime();
    if ( feverTime == 0 )
    {
        feverTime = UtilsDate::getInstance()->getTime() + EventSunfishManager::getInstance()->getFeverCoolTime();
        EventSunfishManager::getInstance()->setFeverTime(feverTime);
    }
    
    if ( feverTime < UtilsDate::getInstance()->getTime() + EventSunfishManager::getInstance()->getFeverCoolAgainTime() )
    {
        feverTime = UtilsDate::getInstance()->getTime() + EventSunfishManager::getInstance()->getFeverCoolAgainTime();
        EventSunfishManager::getInstance()->setFeverTime(feverTime);
    }
}

void PopupEventSunfish::initUI()
{
    // size
    _spriteContainer->setContentSize( Size(750, 1334) );
    
    // label title
    auto uiTitle = ui::CText::create(GAME_TEXT("t_ui_event_event_sunfish_title"), GAME_FONT, 32);
    uiTitle->setPosition(Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5));
    uiTitle->setTextAreaSize(Size(_spriteContainer->getContentSize().width - 200, uiTitle->getContentSize().height));
    uiTitle->setTextOverflow(Label::Overflow::SHRINK);
    uiTitle->setTextVerticalAlignment(TextVAlignment::CENTER);
    uiTitle->setTextHorizontalAlignment(TextHAlignment::CENTER);
    uiTitle->setTextColor(Color4B(COLOR_TITLE));
    _spriteContainer->addChild(uiTitle);
    
    // item close
    auto uiClose = ui::Button::create("Assets/ui_common/btn_x.png");
    uiClose->setPosition(Vec2(_spriteContainer->getContentSize().width - uiClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5));
    uiClose->addClickEventListener(CC_CALLBACK_1(PopupEventSunfish::onClickClose, this));
    _spriteContainer->addChild(uiClose);
}

void PopupEventSunfish::uiContainer()
{
    Size size = _spriteContainer->getContentSize();
    size.width = size.width - 10;
    size.height = size.height  - 75 - 25;
    
    auto layout = ui::Layout::create();
    layout->setContentSize(Size(size.width, size.height));
    layout->setIgnoreAnchorPointForPosition(false);
    layout->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    layout->setPosition(Vec2(_spriteContainer->getContentSize().width / 2, 25));
    _spriteContainer->addChild(layout);
    
    // 184 * 241
    _uiContentsReward = ui::Layout::create();
    _uiContentsReward->setContentSize( Size(size.width, 214) );
    _uiContentsReward->setIgnoreAnchorPointForPosition(false);
    _uiContentsReward->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _uiContentsReward->setPosition( Vec2(size.width / 2, 0) );
    _uiContentsReward->setOpacity(100);
    layout->addChild(_uiContentsReward);
    
    _uiContentsGame = LayerEventSunfishGame::create();
    _uiContentsGame->setContentSize( Size(736, size.height - _uiContentsReward->getContentSize().height) );
    _uiContentsGame->setIgnoreAnchorPointForPosition(false);
    _uiContentsGame->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _uiContentsGame->setPosition( Vec2(size.width / 2, size.height) );
    layout->addChild(_uiContentsGame);
    
    _uiContentsInfo = ui::Layout::create();
    _uiContentsInfo->setContentSize(Size(size.width, size.height - _uiContentsReward->getContentSize().height));
    _uiContentsInfo->setIgnoreAnchorPointForPosition(false);
    _uiContentsInfo->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _uiContentsInfo->setPosition( Vec2(size.width / 2, size.height) );
    _uiContentsInfo->setOpacity(100);
    layout->addChild(_uiContentsInfo);
    
    
}
 
void PopupEventSunfish::uiInfo()
{
    auto layout = _uiContentsInfo;
    auto size = layout->getContentSize();
    
    // 설명 버튼
    auto uiHelp = ui::Button::create("Assets/ui/race_event/race_btn_gameinfo.png");
    uiHelp->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    uiHelp->setPosition(Vec2(25, size.height - 35));
    uiHelp->addClickEventListener(CC_CALLBACK_1(PopupEventSunfish::onClickHelp, this));
    layout->addChild(uiHelp);
    {
        auto uiText = ui::CText::create(GAME_TEXT("t_ui_frost_prison_info"), GAME_FONT, 20);
        uiText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        uiText->setPosition(Vec2(9, uiHelp->getContentSize().height / 2 + 2));
        uiText->setTextAreaSize(Size(92, uiText->getContentSize().height));
        uiText->setTextOverflow(Label::Overflow::SHRINK);
        uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiText->setTextHorizontalAlignment(TextHAlignment::CENTER);
        uiHelp->addChild(uiText);
    }
    
    //
    _uiTime = ui::CText::create(GAME_TEXTFORMAT("t_ui_token_text_eventtime", 0, 0, 0), GAME_FONT, 28);
    _uiTime->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    _uiTime->setPosition(Vec2(size.width, size.height - 25));
    _uiTime->setTextAreaSize(Size(350, _uiTime->getContentSize().height));
    _uiTime->setTextOverflow(Label::Overflow::SHRINK);
    _uiTime->setTextVerticalAlignment(TextVAlignment::CENTER);
    _uiTime->setTextHorizontalAlignment(TextHAlignment::RIGHT);
    _uiTime->enableOutline(Color4B::BLACK, 2);
    layout->addChild(_uiTime);
    
    //
    _uiLife = ui::CText::create(GAME_TEXTFORMAT("t_ui_event_sunfish_info_1", 0), GAME_FONT, 28);
    _uiLife->setPosition(Vec2(size.width / 2, size.height - 75));
    _uiLife->enableOutline(Color4B::BLACK, 2);
    layout->addChild(_uiLife);
    
    //
    _uiWeight = ui::CText::create(GAME_TEXTFORMAT("t_ui_event_sunfish_info_2", 0), GAME_FONT, 28);
    _uiWeight->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    _uiWeight->setPosition(Vec2(size.width - 25, size.height - 75));
    _uiWeight->enableOutline(Color4B::BLACK, 2);
    layout->addChild(_uiWeight);
    {
        auto uiIcon = ui::ImageView::create("Assets/ui/event/icon_scales.png");
        uiIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        uiIcon->setPosition(Vec2(-10, _uiWeight->getContentSize().height / 2));
        _uiWeight->addChild(uiIcon);
    }
    
    //
    auto uiButtons = ui::Layout::create();
    uiButtons->setContentSize(Size(size.width, 84));
    uiButtons->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    uiButtons->setPosition(Vec2(size.width / 2, 25));
    uiButtons->setLayoutType(ui::Layout::Type::HORIZONTAL);
    uiButtons->setName("UI_BUTTONS");
    layout->addChild(uiButtons);
    {
        {
            auto uiButton = ui::Button::create("Assets/ui_common/box_05_2.png");
            uiButton->setScale9Enabled(true);
            uiButton->setCapInsets(Rect(12,12,8,8));
            uiButton->setContentSize(Size(194, uiButtons->getContentSize().height));
            uiButton->addClickEventListener(CC_CALLBACK_1(PopupEventSunfish::onClickAdventure, this));
            uiButton->setName("UI_BUTTON_ADVENTURE");
            uiButtons->addChild(uiButton);
            
            auto uiText = ui::CText::create(GAME_TEXTFORMAT("t_ui_event_sunfish_bt_1", 0, 0), GAME_FONT, 24);
            uiText->setPosition(Vec2(uiButton->getContentSize().width / 2, uiButton->getContentSize().height / 2));
            uiText->setTextAreaSize(Size(uiButton->getContentSize().width - 10, uiText->getContentSize().height));
            uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
            uiText->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
            uiText->setTextOverflow(Label::Overflow::SHRINK);
            uiText->setName("UI_TEXT");
            uiButton->addChild(uiText);
        }
        
        {
            auto uiButton = ui::Button::create("Assets/ui_common/box_05_2.png");
            uiButton->setScale9Enabled(true);
            uiButton->setCapInsets(Rect(12,12,8,8));
            uiButton->setContentSize(Size(194, uiButtons->getContentSize().height));
            uiButton->addClickEventListener(CC_CALLBACK_1(PopupEventSunfish::onClickReinforce, this));
            uiButtons->addChild(uiButton);
            
            auto uiText = ui::CText::create(GAME_TEXT("t_ui_event_sunfish_bt_2"), GAME_FONT, 24);
            uiText->setPosition(Vec2(uiButton->getContentSize().width / 2, uiButton->getContentSize().height / 2));
            uiText->setTextAreaSize(Size(uiButton->getContentSize().width - 10, uiText->getContentSize().height));
            uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
            uiText->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
            uiText->setTextOverflow(Label::Overflow::SHRINK);
            uiButton->addChild(uiText);
            
            auto params = ui::LinearLayoutParameter::create();
            params->setMargin(ui::Margin(10, 0, 0, 0));
            uiButton->setLayoutParameter(params);
        }
        
        {
            auto uiButton = ui::Button::create("Assets/ui_common/box_05_2.png");
            uiButton->setScale9Enabled(true);
            uiButton->setCapInsets(Rect(12,12,8,8));
            uiButton->setContentSize(Size(194, uiButtons->getContentSize().height));
            uiButton->addClickEventListener(CC_CALLBACK_1(PopupEventSunfish::onClickDex, this));
            uiButtons->addChild(uiButton);
            
            auto uiText = ui::CText::create(GAME_TEXT("t_ui_event_sunfish_bt_3"), GAME_FONT, 24);
            uiText->setPosition(Vec2(uiButton->getContentSize().width / 2, uiButton->getContentSize().height / 2));
            uiText->setTextAreaSize(Size(uiButton->getContentSize().width - 10, uiText->getContentSize().height));
            uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
            uiText->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
            uiText->setTextOverflow(Label::Overflow::SHRINK);
            uiButton->addChild(uiText);
            
            auto uiReddot = UIReddot::create(E_REDDOT_TYPE::NOMAL, Vec2::ANCHOR_MIDDLE);
            uiReddot->setPosition(uiButton->getContentSize().width - 10, uiButton->getContentSize().height - 10);
            uiReddot->setVisible(false);
            uiButton->addChild(uiReddot);
            {
                RefreshReddotManager::getInstance()->addUI(E_REFRESH_REDDOT::EVENT_SUNFISH_DEX_ALL, uiReddot);
            }
            
            auto params = ui::LinearLayoutParameter::create();
            params->setMargin(ui::Margin(10, 0, 0, 0));
            uiButton->setLayoutParameter(params);
        }
        
        auto sizeButtons = UtilsWidget::getInstance()->getLayoutAccumulatedSize(uiButtons);
        uiButtons->setContentSize(sizeButtons);
        uiButtons->requestDoLayout();
    }
}

void PopupEventSunfish::uiReward()
{
    auto layout = _uiContentsReward;
    auto size = layout->getContentSize();
    
    auto uiBG = ui::ImageView::create("Assets/ui_common/box_05.png");
    uiBG->setScale9Enabled(true);
    uiBG->setCapInsets(Rect(12, 12, 8, 8));
    uiBG->setContentSize(Size(size.width, size.height));
    uiBG->setPosition(Vec2(size.width / 2, size.height / 2));
    layout->addChild(uiBG);
    
    auto uiTitle = ui::CText::create(GAME_TEXT("t_ui_event_sunfish_reward_info"), GAME_FONT, 20);
    uiTitle->setPosition(Vec2(size.width / 2, size.height - 30));
    uiTitle->setTextAreaSize(Size(size.width - 10, uiTitle->getContentSize().height));
    uiTitle->setTextOverflow(Label::Overflow::SHRINK);
    uiTitle->setTextVerticalAlignment(TextVAlignment::CENTER);
    uiTitle->setTextHorizontalAlignment(TextHAlignment::CENTER);
    layout->addChild(uiTitle);
    
    auto uiRewardPanel = ui::ImageView::create("Assets/ui/event/event_list_box_01.png");
    uiRewardPanel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    uiRewardPanel->setPosition(Vec2(size.width / 2, size.height - 55));
    uiRewardPanel->setName("UI_REWARD_PANEL");
    layout->addChild(uiRewardPanel);
    {
        auto uiBarBG = ui::ImageView::create("Assets/ui/event/8th_progressbar_1_1.png");
        uiBarBG->setPosition(Vec2(uiRewardPanel->getContentSize().width / 2, 15));
        uiRewardPanel->addChild(uiBarBG);
        
        auto uiBar = ui::LoadingBar::create("Assets/ui/event/8th_progressbar_1_2.png");
        uiBar->setPercent(50);
        uiBar->setPosition(Vec2(uiBarBG->getContentSize().width / 2, uiBarBG->getContentSize().height / 2));
        uiBar->setName("UI_REWARD_BAR");
        uiBarBG->addChild(uiBar);
        
        auto uiBarCount = ui::CText::create("(0/0)", GAME_FONT, 16);
        uiBarCount->setPosition(Vec2(uiBarBG->getContentSize().width / 2, uiBarBG->getContentSize().height / 2));
        uiBarCount->setName("UI_REWARD_BAR_COUNT");
        uiBarBG->addChild(uiBarCount);
        
        auto uiReward = ui::Layout::create();
        uiReward->setContentSize(Size(646, 72));
        uiReward->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        uiReward->setPosition(Vec2(uiRewardPanel->getContentSize().width / 2, 65));
        uiReward->setLayoutType(ui::Layout::Type::HORIZONTAL);
        uiReward->setName("UI_REWARD");
        uiRewardPanel->addChild(uiReward);
    }
}

#pragma mark - draw
void PopupEventSunfish::drawInfo()
{
    auto layout = _uiContentsInfo;
    
    //
    auto action1 = CallFuncN::create([=](Ref* sender){
        
        int64_t timeDiff = EventSunfishManager::getInstance()->getEndTime() - UtilsDate::getInstance()->getTime();
        timeDiff = MAX(timeDiff, 0);
        
        int nDay = (int)(timeDiff / 86400);
        int nHour = timeDiff % 86400 / 3600;
        int nMin = timeDiff % 3600 / 60;
         
        
        std::string strTime = GAME_TEXTFORMAT("t_ui_token_text_eventtime", nDay, nHour, nMin);
        if ( EventSunfishManager::getInstance()->isExtraTime() )
        {
            strTime = GAME_TEXTFORMAT("t_ui_event_text_reward_time", nDay, nHour, nMin);
        }
        
        ((ui::CText*)sender)->setString(strTime);
        if ( timeDiff <= 0 )
        {
            ((ui::CText*)sender)->stopAllActions();
            return;
        }
    });
    auto action2 = DelayTime::create(1);
    auto seq = Sequence::create(action1, action2, NULL);
    _uiTime->stopAllActions();
    _uiTime->runAction(RepeatForever::create(seq));
    
    //
    auto weight = EventSunfishManager::getInstance()->getGameWeight();
    _uiWeight->setString(GAME_TEXTFORMAT("t_ui_event_sunfish_info_2", weight));
    
    //
    auto die = EventSunfishManager::getInstance()->getDieTotal();
    _uiLife->setString(GAME_TEXTFORMAT("t_ui_event_sunfish_info_1", die));
    
    //
    auto uiButtons = layout->getChildByName<ui::Layout*>("UI_BUTTONS");
    if ( auto uiButton = uiButtons->getChildByName<ui::Button*>("UI_BUTTON_ADVENTURE"); uiButton != nullptr )
    {
        auto countNow = EventSunfishManager::getInstance()->getAdventureCountNow();
        auto countMax = EventSunfishManager::getInstance()->getAdventureCountMax();
        
        auto uiText = uiButton->getChildByName<ui::CText*>("UI_TEXT");
        uiText->setString(GAME_TEXTFORMAT("t_ui_event_sunfish_bt_1", countNow, countMax));
        {
        
            auto action1 = DelayTime::create(1.0);
            auto action2 = CallFuncN::create([=](Node* sender){
                
                auto uiText = (ui::CText*)sender;
                
                auto countNow = EventSunfishManager::getInstance()->getAdventureCountNow();
                auto countMax = EventSunfishManager::getInstance()->getAdventureCountMax();
                uiText->setString(GAME_TEXTFORMAT("t_ui_event_sunfish_bt_1", countNow, countMax));
            });
            uiText->stopAllActions();
            uiText->runAction(RepeatForever::create(Sequence::create(action1, action2, NULL)));
        }
    }
    
}

void PopupEventSunfish::drawReward()
{
    auto layout = _uiContentsReward;
    
    
    
    if ( auto uiRewardPanel = utils::findChild<ui::ImageView*>(layout, "UI_REWARD_PANEL"); uiRewardPanel != nullptr )
    {
        int rewardLevel = 1;
        bool rewardLast = false;
        
        auto listSunfish = EventSunfishManager::getInstance()->getListInfoSunfish(EventSunfishManager::E_TYPE::LEVEL);
        for ( auto& objSunfish : listSunfish )
        {
            bool bReinforce = EventSunfishManager::getInstance()->isReinforce(EventSunfishManager::E_TYPE::LEVEL, objSunfish->getLevel());
            if ( bReinforce == false )
                break;
            
            rewardLevel = MAX(rewardLevel, objSunfish->getLevel());
        }
        
        auto objSunfishNext = EventSunfishManager::getInstance()->getInfoSunfishLevel(rewardLevel+1);
        if ( objSunfishNext == nullptr )
        {
            objSunfishNext = EventSunfishManager::getInstance()->getInfoSunfishLevel(rewardLevel);
            
            rewardLast = true;
        }
        
        double weightNow = EventSunfishManager::getInstance()->getGameWeight();
        double weightMax = objSunfishNext->getWeight();
        
        double percent = (double)weightNow / weightMax * 100;
        if ( percent > 100 )
            percent = 100;
        
        //
        auto uiBar = utils::findChild<ui::LoadingBar*>(uiRewardPanel, "UI_REWARD_BAR");
        uiBar->setPercent(percent);
        
        //
        auto uiBarCount = utils::findChild<ui::CText*>(uiRewardPanel, "UI_REWARD_BAR_COUNT");
        uiBarCount->setString(MafUtils::format("(%.2f/%.2f)", weightNow, weightMax));
        if ( rewardLast == true )
        {
            uiBarCount->setString("MAX");
        }
        
        //
        auto uiReward = utils::findChild<ui::Layout*>(uiRewardPanel, "UI_REWARD");
        uiReward->removeAllChildren();
        
        for ( int i = 0; i < listSunfish.size(); i++ )
        {
            auto objSunfish = listSunfish.at(i);
            
            auto listReward = objSunfish->getReward();
            if ( listReward.size() <= 0 )
            {
                continue;
            }
            
            bool bReinforce = EventSunfishManager::getInstance()->isReinforce(EventSunfishManager::E_TYPE::LEVEL, objSunfish->getLevel());
            bool bReward = EventSunfishManager::getInstance()->isReward(EventSunfishManager::E_TYPE::LEVEL, objSunfish->getLevel());
            
            auto objItem = listReward.at(0);
            
            std::string itemCount = objItem->getCount();
            if ( objItem->isString() == true )
            {
                MafUtils::convertNumberToShort(itemCount);
            }
            
            //
            auto pathBG = "Assets/ui_common/btn_2_6_on.png";
            if ( bReinforce == true )
            {
                pathBG = "Assets/ui_common/btn_2_8_on.png";
            }
            
            auto uiItemBG = ui::Button::create(pathBG);
            uiItemBG->setScale9Enabled(true);
            uiItemBG->setContentSize(Size(72, 72));
            uiItemBG->addClickEventListener(CC_CALLBACK_1(PopupEventSunfish::onClickReward, this));
            uiItemBG->setTag(objSunfish->getLevel());
            uiReward->addChild(uiItemBG);
            {
                auto params = ui::LinearLayoutParameter::create();
                params->setMargin(ui::Margin(0, 0, 10, 0));
                uiItemBG->setLayoutParameter(params);
            }
            
            //
            auto uiItem = ui::ImageView::create(objItem->getPath());
            uiItem->setPosition(Vec2(uiItemBG->getContentSize().width / 2, uiItemBG->getContentSize().height / 2 + 5));
            uiItemBG->addChild(uiItem);
            
            auto uiItemCount = ui::CText::create(itemCount, GAME_FONT, 20);
            uiItemCount->setPosition(Vec2(uiItemBG->getContentSize().width / 2, 10));
            uiItemCount->enableOutline(Color4B::BLACK, 2);
            uiItemBG->addChild(uiItemCount);
            
            if ( bReward == true )
            {
                auto uiDisable = ui::Layout::create();
                uiDisable->setContentSize(uiItemBG->getContentSize());
                uiDisable->setBackGroundColor(Color3B::BLACK);
                uiDisable->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
                uiDisable->setOpacity(200);
                uiItemBG->addChild(uiDisable);
                
                auto uiCheck = ui::ImageView::create("Assets/icon/icon_check_01.png");
                uiCheck->setPosition(Vec2(uiItemBG->getContentSize().width / 2, uiItemBG->getContentSize().height / 2));
                uiItemBG->addChild(uiCheck);
            }
            
            //
            auto uiItemIdxBG = ui::ImageView::create("Assets/ui_common/number_box_1_1.png");
            uiItemIdxBG->setPosition(Vec2(5, uiItemBG->getContentSize().height - 10));
            uiItemBG->addChild(uiItemIdxBG);
            if ( bReinforce == true )
            {
                uiItemIdxBG->loadTexture("Assets/ui_common/number_box_1_2.png");
            }
            
            auto uiItemIdx = ui::CText::create(MafUtils::format("%d", objSunfish->getLevel()), GAME_FONT, 18);
            uiItemIdx->setPosition(Vec2(uiItemIdxBG->getContentSize().width / 2, uiItemIdxBG->getContentSize().height / 2));
            uiItemIdxBG->addChild(uiItemIdx);
        }
        
        auto sizeReward = UtilsWidget::getInstance()->getLayoutAccumulatedSize(uiReward);
        uiReward->setContentSize(sizeReward);
        uiReward->requestDoLayout();
    }
    
    
//
//    int rewardLevel = EventTreasureHuntManager::getInstance()->getRewardLevel(type);
//
//    auto objRewardPrev = EventTreasureHuntManager::getInstance()->getInfoReward(type, rewardLevel);
//    auto objRewardNext = EventTreasureHuntManager::getInstance()->getInfoReward(type, rewardLevel+1);
//    if ( objRewardNext == nullptr )
//    {
//        objRewardNext = EventTreasureHuntManager::getInstance()->getInfoReward(type, rewardLevel);
//    }
//
//    int rewardValueNow = EventTreasureHuntManager::getInstance()->getRewardValue(type);
//    int rewardValueMax = objRewardNext->getValue();
//    if ( objRewardPrev != nullptr )
//    {
//        rewardValueNow -= objRewardPrev->getValue();
//        rewardValueNow = MAX(rewardValueNow, 0);
//
//        rewardValueMax -= objRewardPrev->getValue();
//        rewardValueMax = MAX(rewardValueMax, 0);
//    }
//
//    double percent = (double)rewardValueNow / rewardValueMax * 100;
//    if ( percent > 100 )
//        percent = 100;
}

#pragma mark - click
void PopupEventSunfish::onClickClose(Ref* sender)
{
    hide();
}

void PopupEventSunfish::onClickHelp(Ref* sender)
{
    auto popup = PopupHelp::create(1, "t_ui_event_sunfish_info_title_%d", "t_ui_event_sunfish_info_msg_%d");
    popup->show();
}

void PopupEventSunfish::onClickAdventure(Ref* sender)
{
    if ( EventSunfishManager::getInstance()->isExtraTime() == true )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_draw_msg_1"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        
        return;
    }
    
    if ( EventSunfishManager::getInstance()->getEndTime() - UtilsDate::getInstance()->getTime() <= 0 )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_draw_msg_1"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        
        return;
    }
    
    auto levelIdx = EventSunfishManager::getInstance()->getGameLevelNow();
    auto weight = EventSunfishManager::getInstance()->getGameWeight();
    
    auto popup = PopupEventSunfishAdventure::create();
    popup->setLevelIdx(levelIdx);
    popup->show();
}

void PopupEventSunfish::onClickReinforce(Ref* sender)
{
    auto popup = PopupEventSunfishReinforce::create();
    popup->show();
}

void PopupEventSunfish::onClickDex(Ref* sender)
{
    auto popup = PopupEventSunfishDex::create();
    popup->show();
}

void PopupEventSunfish::onClickReward(Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto uiButton = dynamic_cast<ui::Button*>(sender);
    auto levelIdx = uiButton->getTag();
    
    bool bReinforce = EventSunfishManager::getInstance()->isReinforce(EventSunfishManager::E_TYPE::LEVEL, levelIdx);
    bool bReward = EventSunfishManager::getInstance()->isReward(EventSunfishManager::E_TYPE::LEVEL, levelIdx);
    
    if ( bReinforce == false )
    {
        return;
    }
    
    if ( bReward == true )
    {
        return;
    }
    
    EventSunfishManager::getInstance()->requestReward(true, enumToInt(EventSunfishManager::E_TYPE::LEVEL), levelIdx, CC_CALLBACK_2(PopupEventSunfish::callbackReward, this));
}

#pragma mark - callback
void PopupEventSunfish::callbackInfo(bool bResult, int nResult)
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
    auto layout = dynamic_cast<LayerEventSunfishGame*>(_uiContentsGame);
    if ( layout != nullptr )
    {
        if ( EventSunfishManager::getInstance()->isExtraTime() == false )
        {
            layout->onStart();
        }
    }
    
    //
    drawInfo();
    drawReward();
}

void PopupEventSunfish::callbackReward(bool bResult, int nResult)
{
    if ( bResult == false )
    {
        std::string strMessage = GAME_TEXT("t_ui_error_30");
        if ( nResult == 2 )
            strMessage = GAME_TEXT("t_ui_draw_msg_1");
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), strMessage);
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    //
    drawInfo();
    drawReward();
}

void PopupEventSunfish::callbackInfoUpdate()
{
    drawInfo();
    drawReward();
}
