//
//  PopupEventTreasureHunt.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/11/23.
//

#include "PopupEventTreasureHunt.h"

#include "Common/ConfigGameNormal.h"

#include "GameUI/UI/UIReddot.h"
#include "GameUI/UI/UICurrency.h"

#include "GameUIPopup/Common/PopupHelp.h"
#include "GameUIPopup/Other/Ads/PopupAdsMafGames.h"

#include "ManagerEvent/EventManager.h"
#include "ManagerEvent/EventTreasureHuntManager.h"
#include "ManagerGame/DialogManager.h"
#include "ManagerGame/AccountManager.h"
#include "ManagerGame/ItemsManager.h"
#include "ManagerGame/CashManager.h"

USING_NS_CC;

PopupEventTreasureHunt* PopupEventTreasureHunt::create()
{
    PopupEventTreasureHunt* pRet = new(std::nothrow) PopupEventTreasureHunt();
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

PopupEventTreasureHunt::PopupEventTreasureHunt():
_bInitUI(false),

//
_uiContentsInfo(nullptr),
_uiContentsBoards(nullptr),
_uiContentsRewards(nullptr),

_unitPet(nullptr)
{
    
}

PopupEventTreasureHunt::~PopupEventTreasureHunt(void)
{
    
}

bool PopupEventTreasureHunt::init()
{
    if (!PopupBase::init())
    {
        return false;
    }
    
    //
    initVar();
    initUI();
    
    return true;
}

void PopupEventTreasureHunt::onEnter()
{
    PopupBase::onEnter();
    
    if ( _bInitUI == false )
    {
        //
        uiContainer();
        uiInfo();
        uiBoards();
        uiRewards();
        
        //
        EventTreasureHuntManager::getInstance()->requestInfo(true, CC_CALLBACK_2(PopupEventTreasureHunt::callbackInfo, this));
        
        // 시나리오 연출
        if(DialogManager::getInstance()->isViewDialog("EVENT_TREASURE_HUNT_OPENNING") == 0)
        {
            DialogManager::getInstance()->showDialog("EVENT_TREASURE_HUNT_OPENNING");
        }
        
        //
        _bInitUI = true;
    }
    
    //
    drawInfo();
    drawBoards();
    drawRewards();
}

#pragma mark - ui
void PopupEventTreasureHunt::initVar()
{

}

void PopupEventTreasureHunt::initUI()
{
    // size
    _spriteContainer->setContentSize( Size(750, 1334) );
    
    // label title
    auto uiTitle = ui::CText::create(GAME_TEXT("t_ui_event_treasure_hunt_title"), GAME_FONT, 32);
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
    uiClose->addClickEventListener(CC_CALLBACK_1(PopupEventTreasureHunt::onClickClose, this));
    _spriteContainer->addChild(uiClose);
    
    auto uiInfo = ui::Button::create("Assets/ui_common/icon_info_1_2.png");
    uiInfo->setPosition(Vec2(50, _spriteContainer->getContentSize().height -37.5));
    uiInfo->addClickEventListener(CC_CALLBACK_1(PopupEventTreasureHunt::onClickHelp, this));
    _spriteContainer->addChild(uiInfo);
    
    //
    std::vector<E_ITEMS> list;
    list.push_back(E_ITEMS::COSTUME_COIN);
    list.push_back(E_ITEMS::EVENT_TREASURE_CHANCE);
    
    auto uiCurrency = UICurrency::create(list, Size(390, 50));
    uiCurrency->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    uiCurrency->setPosition(Vec2(0, _spriteContainer->getContentSize().height - 75));
    _spriteContainer->addChild(uiCurrency);
}

void PopupEventTreasureHunt::uiContainer()
{
    Size size = _spriteContainer->getContentSize();
    size.width = size.width - 10;
    size.height = size.height  - 75 - 25 - 50;
    
    auto layout = ui::Layout::create();
    layout->setContentSize(Size(size.width, size.height));
    layout->setIgnoreAnchorPointForPosition(false);
    layout->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    layout->setPosition(Vec2(_spriteContainer->getContentSize().width / 2, 25));
    _spriteContainer->addChild(layout);
    
    // 186 * 269(744 * 1076)
    _uiContentsBoards = ui::Layout::create();
    _uiContentsBoards->setContentSize( Size(size.width, 1076) );
    _uiContentsBoards->setIgnoreAnchorPointForPosition(false);
    _uiContentsBoards->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _uiContentsBoards->setPosition( Vec2(size.width / 2, size.height) );
    layout->addChild(_uiContentsBoards);
    
    //
    _uiContentsInfo = ui::Layout::create();
    _uiContentsInfo->setContentSize( Size(size.width, size.height) );
    _uiContentsInfo->setIgnoreAnchorPointForPosition(false);
    _uiContentsInfo->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _uiContentsInfo->setPosition( Vec2(size.width / 2, size.height) );
    layout->addChild(_uiContentsInfo);
    
    _uiContentsRewards = ui::Layout::create();
    _uiContentsRewards->setContentSize( Size(728, 358) );
    _uiContentsRewards->setIgnoreAnchorPointForPosition(false);
    _uiContentsRewards->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _uiContentsRewards->setPosition( Vec2(size.width / 2, 0) );
    layout->addChild(_uiContentsRewards);
    
}
 
void PopupEventTreasureHunt::uiInfo()
{
    auto layout = _uiContentsInfo;
    auto size = layout->getContentSize();
    
    //
    auto uiTime = ui::CText::create(GAME_TEXTFORMAT("t_ui_token_text_eventtime", 0, 0, 0), GAME_FONT, 24);
    uiTime->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    uiTime->setPosition(Vec2(size.width, size.height + 10));
    uiTime->setTextAreaSize(Size(350, uiTime->getContentSize().height));
    uiTime->setTextOverflow(Label::Overflow::SHRINK);
    uiTime->setTextVerticalAlignment(TextVAlignment::CENTER);
    uiTime->setTextHorizontalAlignment(TextHAlignment::RIGHT);
    uiTime->setName("UI_TIME");
    layout->addChild(uiTime);
    
    //
    auto uiCompassPanel = ui::Layout::create();
    uiCompassPanel->setContentSize(Size(560, 160));
    uiCompassPanel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    uiCompassPanel->setPosition(Vec2(35, size.height));
    uiCompassPanel->setName("UI_COMPASS_PANEL");
    layout->addChild(uiCompassPanel);
    {
        auto uiCompass = ui::Button::create("Assets/ui/event/event_compass1_1.png");
        uiCompass->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        uiCompass->setPosition(Vec2(0, uiCompassPanel->getContentSize().height / 2));
        uiCompass->addClickEventListener(CC_CALLBACK_1(PopupEventTreasureHunt::onClickCompass, this));
        uiCompass->setName("UI_COMPASS");
        uiCompassPanel->addChild(uiCompass);
        
        auto uiCompassActive = ui::ImageView::create("Assets/ui/event/event_compass1_2.png");
        uiCompassActive->setPosition(Vec2(uiCompass->getContentSize().width / 2, uiCompass->getContentSize().height / 2));
        uiCompassActive->runAction(RepeatForever::create(Sequence::create(FadeOut::create(1.0), FadeIn::create(1.0), NULL)));
        uiCompassActive->setName("UI_COMPASS_ACTIVE");
        uiCompassActive->setVisible(false);
        uiCompass->addChild(uiCompassActive);
        
        auto uiCompassArrow = ui::ImageView::create("Assets/ui/event/event_compass1_3.png");
        uiCompassArrow->setPosition(Vec2(uiCompass->getContentSize().width / 2, uiCompass->getContentSize().height / 2));
        uiCompassArrow->runAction(RepeatForever::create(RotateBy::create(4.0, 360)));
        uiCompass->addChild(uiCompassArrow);
        
        auto uiCompassDescBG = ui::ImageView::create("Assets/ui/event/event_hint_01.png");
        uiCompassDescBG->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        uiCompassDescBG->setPosition(Vec2(135, uiCompassPanel->getContentSize().height - 20));
        uiCompassPanel->addChild(uiCompassDescBG);
        {
            auto uiText = ui::CText::create(GAME_TEXT("t_ui_event_treasure_hunt_msg_2"), GAME_FONT, 24);
            uiText->setPosition(Vec2(uiCompassDescBG->getContentSize().width / 2 + 4.5, uiCompassDescBG->getContentSize().height / 2));
            uiText->setTextAreaSize(Size(uiCompassDescBG->getContentSize().width - 20, uiText->getContentSize().height));
            uiText->setTextOverflow(Label::Overflow::SHRINK);
            uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
            uiText->setTextHorizontalAlignment(TextHAlignment::CENTER);
            uiText->setTextColor(Color4B::BLACK);
            uiCompassDescBG->addChild(uiText);
        }
        
        auto uiCompassCountBG = ui::ImageView::create("Assets/ui_common/base_text_2_7.png");
        uiCompassCountBG->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        uiCompassCountBG->setPosition(Vec2(145, uiCompassPanel->getContentSize().height - 80));
        uiCompassPanel->addChild(uiCompassCountBG);
        {
            auto uiText = ui::CText::create(GAME_TEXTFORMAT("t_ui_event_treasure_hunt_msg_3", 0, 0), GAME_FONT, 20);
            uiText->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
            uiText->setPosition(Vec2(10, 12));
            uiText->setTextAreaSize(Size(uiCompassCountBG->getContentSize().width - 20, uiText->getContentSize().height));
            uiText->setTextOverflow(Label::Overflow::SHRINK);
            uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
            uiText->setTextHorizontalAlignment(TextHAlignment::LEFT);
            uiText->enableOutline(Color4B::BLACK, 2);
            uiText->setName("UI_BAR_TEXT");
            uiCompassCountBG->addChild(uiText);
            
            auto uiBarBG = ui::ImageView::create("Assets/ui/event/s_progressbar_1_1.png");
            uiBarBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            uiBarBG->setPosition(Vec2(10, 10));
            uiCompassCountBG->addChild(uiBarBG);
            
            auto uiBar = ui::LoadingBar::create("Assets/ui/event/s_progressbar_1_2.png");
            uiBar->setPercent(50);
            uiBar->setPosition(Vec2(uiBarBG->getContentSize().width / 2, uiBarBG->getContentSize().height / 2));
            uiBar->setName("UI_BAR");
            uiBarBG->addChild(uiBar);
        }
    }
    
    //
    auto uiRefresh = ui::Button::create("Assets/ui/event/btn_event_1_1.png");
    uiRefresh->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    uiRefresh->setPosition(Vec2(size.width - 25, size.height - 40));
    uiRefresh->addClickEventListener(CC_CALLBACK_1(PopupEventTreasureHunt::onClickRefresh, this));
    uiRefresh->setName("UI_REFRESH");
    layout->addChild(uiRefresh);
    {
        auto uiEffect = ui::ImageView::create("Assets/ui/event/btn_event_1_3.png");
        uiEffect->setPosition(Vec2(uiRefresh->getContentSize().width / 2, uiRefresh->getContentSize().height / 2));
        uiEffect->setVisible(false);
        uiEffect->setName("UI_EFFECT");
        uiRefresh->addChild(uiEffect);
        
        auto uiIcon = ui::ImageView::create("Assets/ui_common/btn_refresh2.png");
        uiIcon->setPosition(Vec2(12 + 45 / 2, uiRefresh->getContentSize().height / 2));
        uiIcon->setScale(0.9);
        uiRefresh->addChild(uiIcon);
        
        auto objItem = ItemsManager::getInstance()->getInfoItems(E_ITEMS::COSTUME_COIN);
        if ( objItem != nullptr )
        {
            auto uiCost = UtilsWidget::getInstance()->createIconText(objItem->getPath(), "500", 91, 24, 15);
            uiCost->setPosition(Vec2(uiRefresh->getContentSize().width - 12 - 91 / 2, uiRefresh->getContentSize().height / 2));
            uiCost->setName("UI_COST");
            uiRefresh->addChild(uiCost);
        }
    }
    
    //
    auto uiAD = ui::Button::create("Assets/ui/event/btn_event_1_2.png");
    uiAD->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    uiAD->setPosition(Vec2(size.width - 25, size.height - 110));
    uiAD->addClickEventListener(CC_CALLBACK_1(PopupEventTreasureHunt::onClickAD, this));
    uiAD->setName("UI_AD");
    layout->addChild(uiAD);
    {
        auto uiIcon = ui::ImageView::create("Assets/ui/event/icon_shovel.png");
        uiIcon->setPosition(Vec2(12 + 45 / 2, uiAD->getContentSize().height / 2));
        uiAD->addChild(uiIcon);
        
        auto uiIconAD = ui::ImageView::create("Assets/icon/icon_ad_1_2.png");
        uiIconAD->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        uiIconAD->setPosition(Vec2(uiAD->getContentSize().width, uiAD->getContentSize().height - 4));
        uiAD->addChild(uiIconAD);
        
        auto uiCount = ui::CText::create("0/0", GAME_FONT, 24);
        uiCount->setPosition(Vec2(uiAD->getContentSize().width - 12 - 91 / 2, uiAD->getContentSize().height / 2));
        uiCount->setTextColor(Color4B::YELLOW);
        uiCount->enableOutline(Color4B::BLACK, 2);
        uiCount->setName("UI_COUNT");
        uiAD->addChild(uiCount);
    }
    
    //
    auto uiBonusPanel = ui::Layout::create();
    uiBonusPanel->setContentSize(Size(265, 20));
    uiBonusPanel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    uiBonusPanel->setPosition(Vec2(size.width - 25, size.height - 160));
    uiBonusPanel->setOpacity(100);
    uiBonusPanel->setName("UI_BONUS_PANEL");
    layout->addChild(uiBonusPanel);
    {
        double posX = uiBonusPanel->getContentSize().width;
        double posY = 0;
        
        auto uiReward = ui::ImageView::create("Assets/ui_common/icon_reward_1_1.png");
        uiReward->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
        uiReward->setPosition(Vec2(posX, posY));
        uiBonusPanel->addChild(uiReward);
        posX -= uiReward->getContentSize().width;
        
        auto uiMulti = ui::ImageView::create(MafUtils::format("Assets/ui_common/icon_mul_%02d.png", EventTreasureHuntManager::getInstance()->getBonusRatio()));
        uiMulti->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
        uiMulti->setPosition(Vec2(posX, posY));
        uiBonusPanel->addChild(uiMulti);
        posX -= uiMulti->getContentSize().width;
        
        int bonusMax = EventTreasureHuntManager::getInstance()->getBonusMax();
        for (int i = bonusMax - 1; i >= 1; i-- )
        {
            auto uiBar = ui::ImageView::create("Assets/ui_common/reward_stack_1_1.png");
            uiBar->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
            uiBar->setPosition(Vec2(posX, posY));
            uiBar->setTag(i);
            uiBonusPanel->addChild(uiBar);
            posX -= uiBar->getContentSize().width;
        }
    }
}

void PopupEventTreasureHunt::uiBoards()
{
    auto layout = _uiContentsBoards;
    auto size = layout->getContentSize();
    
    auto uiBG = ui::ImageView::create("Assets/ui/event/event_drawing_bg_01.png");
    uiBG->setPosition(Vec2(size.width / 2, size.height / 2));
    ((ui::Scale9Sprite*)uiBG->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
    uiBG->setScale(4);
    layout->addChild(uiBG);
    
    //
    auto uiBoards = ui::Layout::create();
    uiBoards->setIgnoreAnchorPointForPosition(false);
    uiBoards->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    uiBoards->setPosition( Vec2(size.width / 2, size.height - 180) );
    uiBoards->setName("UI_BOARDS");
    layout->addChild(uiBoards);
    
    //
    _unitPet = Sprite::create("Assets/ui/event/event_mole_idle_1.png");
    layout->addChild(_unitPet);
}

void PopupEventTreasureHunt::uiRewards()
{
    auto layout = _uiContentsRewards;
    auto size = layout->getContentSize();
    
    auto uiBG = ui::ImageView::create("Assets/ui_common/box_05.png");
    uiBG->setScale9Enabled(true);
    uiBG->setCapInsets(Rect(12, 12, 8, 8));
    uiBG->setContentSize(Size(728, 358));
    uiBG->setPosition(Vec2(size.width / 2, size.height / 2));
    ((ui::Scale9Sprite*)uiBG->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
    layout->addChild(uiBG);
    
    auto uiText = ui::CText::create(GAME_TEXT("t_ui_event_treasure_hunt_msg_1"), GAME_FONT, 24);
    uiText->setPosition(Vec2(size.width / 2, size.height - 28));
    uiText->setTextAreaSize(Size(size.width - 10, uiText->getContentSize().height));
    uiText->setTextOverflow(Label::Overflow::SHRINK);
    uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
    uiText->setTextHorizontalAlignment(TextHAlignment::CENTER);
    layout->addChild(uiText);
    
    // so reward
    auto uiSoRewardPanel = ui::ImageView::create("Assets/ui/event/event_list_box_01.png");
    uiSoRewardPanel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    uiSoRewardPanel->setPosition(Vec2(size.width / 2, size.height - 55));
    uiSoRewardPanel->setName("UI_SO_REWARD");
    layout->addChild(uiSoRewardPanel);
    {
        auto uiText = ui::CText::create(GAME_TEXT("t_ui_event_treasure_hunt_reward_1"), GAME_FONT, 20);
        uiText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        uiText->setPosition(Vec2(20, uiSoRewardPanel->getContentSize().height - 3 - 13));
        uiSoRewardPanel->addChild(uiText);
        
        auto uiBarBG = ui::ImageView::create("Assets/ui/event/8th_progressbar_1_1.png");
        uiBarBG->setPosition(Vec2(uiSoRewardPanel->getContentSize().width / 2, 15));
        uiSoRewardPanel->addChild(uiBarBG);
        
        auto uiBar = ui::LoadingBar::create("Assets/ui/event/8th_progressbar_1_2.png");
        uiBar->setPercent(50);
        uiBar->setPosition(Vec2(uiBarBG->getContentSize().width / 2, uiBarBG->getContentSize().height / 2));
        uiBar->setName("UI_BAR");
        uiBarBG->addChild(uiBar);
        
        auto uiBarText = ui::CText::create("(0/0)", GAME_FONT, 16);
        uiBarText->setPosition(Vec2(uiBarBG->getContentSize().width / 2, uiBarBG->getContentSize().height / 2));
        uiBarText->setName("UI_BAR_TEXT");
        uiBarBG->addChild(uiBarText);
        
        auto uiReward = ui::Layout::create();
        uiReward->setContentSize(Size(646, 72));
        uiReward->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        uiReward->setPosition(Vec2(uiSoRewardPanel->getContentSize().width / 2, 65));
        uiReward->setLayoutType(ui::Layout::Type::HORIZONTAL);
        uiReward->setName("UI_REWARD");
        uiSoRewardPanel->addChild(uiReward);
    }
    
    // co reward
    auto uiCoRewardPanel = ui::ImageView::create("Assets/ui/event/event_list_box_01.png");
    uiCoRewardPanel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    uiCoRewardPanel->setPosition(Vec2(size.width / 2, size.height - 200));
    uiCoRewardPanel->setName("UI_CO_REWARD");
    layout->addChild(uiCoRewardPanel);
    {
        auto uiText = ui::CText::create(GAME_TEXT("t_ui_event_treasure_hunt_reward_2"), GAME_FONT, 20);
        uiText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        uiText->setPosition(Vec2(20, uiCoRewardPanel->getContentSize().height - 3 - 13));
        uiCoRewardPanel->addChild(uiText);
        
        auto uiBarBG = ui::ImageView::create("Assets/ui/event/8th_progressbar_2_1.png");
        uiBarBG->setPosition(Vec2(uiCoRewardPanel->getContentSize().width / 2, 15));
        uiCoRewardPanel->addChild(uiBarBG);
        
        auto uiBar = ui::LoadingBar::create("Assets/ui/event/8th_progressbar_2_2.png");
        uiBar->setPercent(50);
        uiBar->setPosition(Vec2(uiBarBG->getContentSize().width / 2, uiBarBG->getContentSize().height / 2));
        uiBar->setName("UI_BAR");
        uiBarBG->addChild(uiBar);
        
        auto uiBarText = ui::CText::create("(0/0)", GAME_FONT, 16);
        uiBarText->setPosition(Vec2(uiBarBG->getContentSize().width / 2, uiBarBG->getContentSize().height / 2));
        uiBarText->setName("UI_BAR_TEXT");
        uiBarBG->addChild(uiBarText);
        
        auto uiReward = ui::Layout::create();
        uiReward->setContentSize(Size(646, 72));
        uiReward->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        uiReward->setPosition(Vec2(uiCoRewardPanel->getContentSize().width / 2, 65));
        uiReward->setLayoutType(ui::Layout::Type::HORIZONTAL);
        uiReward->setName("UI_REWARD");
        uiCoRewardPanel->addChild(uiReward);
    }
}

#pragma mark - draw
void PopupEventTreasureHunt::drawInfo()
{
    auto layout = _uiContentsInfo;
    
    if ( auto uiTime = layout->getChildByName<ui::CText*>("UI_TIME"); uiTime != nullptr )
    {
        auto action1 = CallFuncN::create([=](Ref* sender){
            
            int64_t timeDiff = EventTreasureHuntManager::getInstance()->getEndTime() - UtilsDate::getInstance()->getTime();
            timeDiff = MAX(timeDiff, 0);
            
            int nDay = (int)(timeDiff / 86400);
            int nHour = timeDiff % 86400 / 3600;
            int nMin = timeDiff % 3600 / 60;
             
            
            
            std::string strTime = GAME_TEXTFORMAT("t_ui_token_text_eventtime", nDay, nHour, nMin);
            if ( EventTreasureHuntManager::getInstance()->isExtraTime() )
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
        uiTime->stopAllActions();
        uiTime->runAction(RepeatForever::create(seq));
    }
    
    //
    if ( auto uiRefresh = utils::findChild<ui::Button*>(layout, "UI_REFRESH"); uiRefresh != nullptr )
    {
        uiRefresh->removeChildByName("UI_COST");
        
        auto uiEffect = utils::findChild<ui::ImageView*>(uiRefresh, "UI_EFFECT");
      
        
        //
        bool bFinish = EventTreasureHuntManager::getInstance()->isBoardFinish();
        if ( bFinish == false )
        {
            int itemIdx = 0;
            int itemCount = 0;
            std::string data = EventManager::getInstance()->getEventOhterDataByEventIdx(39);
            if ( data.empty() == false )
            {
                rapidjson::Document jsonParser;
                jsonParser.ParseInsitu<0>((char*)data.c_str());
                if( jsonParser.HasParseError() == false )
                {
                    itemIdx = jsonParser["reset_cost"]["idx"].GetInt();
                    itemCount = jsonParser["reset_cost"]["count"].GetInt();
                }
            }
            
            auto objItem = ItemsManager::getInstance()->getInfoItems((E_ITEMS)itemIdx);
            if ( objItem != nullptr )
            {
                auto uiCost = UtilsWidget::getInstance()->createIconText(objItem->getPath(), std::to_string(itemCount), 91, 24, 15);
                uiCost->setPosition(Vec2(uiRefresh->getContentSize().width - 12 - 91 / 2, uiRefresh->getContentSize().height / 2));
                uiCost->setName("UI_COST");
                uiRefresh->addChild(uiCost);
            }
            
            //
            uiEffect->stopAllActions();
            uiEffect->setVisible(false);
        }
        else
        {
            //
            uiEffect->stopAllActions();
            uiEffect->runAction(RepeatForever::create(Sequence::create(FadeOut::create(0.5), FadeIn::create(0.5), NULL)));
            uiEffect->setVisible(true);
            
            //
            auto uiText = ui::CText::create(GAME_TEXT("t_ui_free"), GAME_FONT, 24);
            uiText->setPosition(Vec2(uiRefresh->getContentSize().width - 12 - 91 / 2, uiRefresh->getContentSize().height / 2));
            uiText->setTextAreaSize(Size(91, uiText->getContentSize().height));
            uiText->setTextOverflow(Label::Overflow::SHRINK);
            uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
            uiText->setTextHorizontalAlignment(TextHAlignment::CENTER);
            uiText->setName("UI_COST");
            uiRefresh->addChild(uiText);
        }
        
    }
    
    //
    if ( auto uiAD = utils::findChild<ui::Button*>(layout, "UI_AD"); uiAD != nullptr )
    {
        int adMax = EventTreasureHuntManager::getInstance()->getADMax();
        int adNow = adMax - EventTreasureHuntManager::getInstance()->getADNow();
        adNow = MAX(adNow, 0);
        
        auto uiCount = utils::findChild<ui::CText*>(uiAD, "UI_COUNT");
        uiCount->setString(MafUtils::format("%d/%d", adNow, adMax));
        
        if ( EventTreasureHuntManager::getInstance()->isExtraTime() == true )
            uiAD->setEnabled(false);
        else
            uiAD->setEnabled(true);
            
    }
    
    //
    if ( auto uiCompassPanel = utils::findChild<ui::Layout*>(layout, "UI_COMPASS_PANEL"); uiCompassPanel != nullptr )
    {
        int compassMax = EventTreasureHuntManager::getInstance()->getCompassMax();
        int compassNow = EventTreasureHuntManager::getInstance()->getCompassNow();
        
        double percent = (double)compassNow / compassMax * 100;
        if ( percent > 100 )
            percent = 100;
        
        auto uiBar = utils::findChild<ui::LoadingBar*>(uiCompassPanel, "UI_BAR");
        uiBar->setPercent(percent);
        
        auto uiBarText = utils::findChild<ui::CText*>(uiCompassPanel, "UI_BAR_TEXT");
        uiBarText->setString(GAME_TEXTFORMAT("t_ui_event_treasure_hunt_msg_3", compassNow, compassMax));
        
        auto uiCompassActive = utils::findChild<ui::ImageView*>(uiCompassPanel, "UI_COMPASS_ACTIVE");
        if ( compassNow >= compassMax )
        {
            uiCompassActive->setVisible(true);
        }
        else
        {
            uiCompassActive->setVisible(false);
        }
    }
    
    //
    if ( auto uiBonusPanel = utils::findChild<ui::Layout*>(layout, "UI_BONUS_PANEL"); uiBonusPanel != nullptr )
    {
        int bonusMax = EventTreasureHuntManager::getInstance()->getBonusMax();
        int bonusNow = EventTreasureHuntManager::getInstance()->getBonusNow() % bonusMax;
        
        for (int i = 1; i <= bonusMax - 1; i++ )
        {
            auto path = "Assets/ui_common/reward_stack_1_1.png";
            if ( i <= bonusNow )
            {
                path = "Assets/ui_common/reward_stack_1_2.png";
            }
                
            auto uiBar = uiBonusPanel->getChildByTag<ui::ImageView*>(i);
            uiBar->loadTexture(path);
        }
    }
}

void PopupEventTreasureHunt::drawBoards()
{
    auto layout = _uiContentsBoards;
    
    auto uiBoards = utils::findChild<ui::Layout*>(layout, "UI_BOARDS");
    if ( uiBoards != nullptr )
    {
        //
        auto listChild = uiBoards->getChildren();
        
        if ( listChild.size() <= 0 )
        {
            std::vector<int> listBoardBlockPos = {2, 3, 4, 8, 12, 15, 19, 23, 24, 25, 29, 33, 36, 40, 44, 45, 46};
            
            for ( int i = 0; i < 49; i++ )
            {
                auto pathBG = MafUtils::format("Assets/ui/event/farm_blocks_2_%d.png", random(1, 5));
                if ( std::find(listBoardBlockPos.begin(), listBoardBlockPos.end(), i) != listBoardBlockPos.end() )
                {
                    pathBG = MafUtils::format("Assets/ui/event/farm_blocks_3_%d.png", random(1, 3));
                }
                
                auto uiBoard = ui::Button::create(pathBG);
                uiBoard->addClickEventListener(CC_CALLBACK_1(PopupEventTreasureHunt::onClickBoard, this));
                uiBoard->setTag(i);
                uiBoards->addChild(uiBoard);
            }
            
            //
            UtilsWidget::getInstance()->sortWidgetGrid(uiBoards, 7, 504 + 120, 0, 20);
        }
        
        for ( int i = 0; i < listChild.size(); i++ )
        {
            auto uiBoard = (ui::Button*)listChild.at(i);
            
            if ( uiBoard->getChildByName("UI_ITEM") != nullptr )
            {
                uiBoard->removeChildByName("UI_ITEM");
            }
            
            if ( uiBoard->getChildByName("UI_EFFECT") != nullptr )
            {
                uiBoard->removeChildByName("UI_EFFECT");
            }
            
            if ( uiBoard->getChildByName("UI_MULTI") != nullptr )
            {
                uiBoard->removeChildByName("UI_MULTI");
            }
            
            
            //
            int idx = EventTreasureHuntManager::getInstance()->getBoardPosToIdx(i);
            if ( EventTreasureHuntManager::getInstance()->isBoardOpen(idx) == true )
            {
                if ( idx == 1 )
                {
                    uiBoard->loadTextureNormal("Assets/ui/event/farm_blocks_1_2.png");
                    
                    auto uiEffect = ui::ImageView::create("Assets/ui/event/farm_blocks_4_2.png");
                    uiEffect->setPosition(Vec2(uiBoard->getContentSize().width / 2, uiBoard->getContentSize().height / 2));
                    uiEffect->setName("UI_EFFECT");
                    uiBoard->addChild(uiEffect, -1);
                }
                else
                {
                    uiBoard->loadTextureNormal("Assets/ui/event/farm_blocks_1_1.png");
                    
                    auto objMap = EventTreasureHuntManager::getInstance()->getInfoMap(idx);
                    if ( objMap != nullptr )
                    {
                        auto objItem = objMap->getReward().at(0);
                        
                        std::string itemCount = objItem->getCount();
                        if ( objItem->isString() == true )
                        {
                            MafUtils::convertNumberToShort(itemCount);
                        }
                                    
                        auto uiItem = ui::ImageView::create(objItem->getPath());
                        uiItem->setPosition(Vec2(uiBoard->getContentSize().width / 2, uiBoard->getContentSize().height / 2));
                        uiItem->setName("UI_ITEM");
                        uiBoard->addChild(uiItem);
                        
                        auto uiItemCount = ui::CText::create(itemCount, GAME_FONT, 16);
                        uiItemCount->setPosition(Vec2(uiBoard->getContentSize().width / 2, 10));
                        uiItemCount->enableOutline(Color4B::BLACK, 2);
                        uiBoard->addChild(uiItemCount);
                    }
                }
            }
            else if ( EventTreasureHuntManager::getInstance()->isBonusEffect() == true )
            {
                auto uiEffect = ui::ImageView::create("Assets/ui/event/farm_blocks_4_1.png");
                uiEffect->setPosition(Vec2(uiBoard->getContentSize().width / 2, uiBoard->getContentSize().height / 2));
                uiEffect->setName("UI_EFFECT");
                uiEffect->runAction(RepeatForever::create(Sequence::create(FadeOut::create(1), FadeIn::create(1), NULL)));
                uiBoard->addChild(uiEffect, -1);
                
            }
            
            if ( EventTreasureHuntManager::getInstance()->isBonus(idx) == true )
            {
                auto uiMulti = ui::ImageView::create(MafUtils::format("Assets/ui_common/icon_mul_%02d.png", EventTreasureHuntManager::getInstance()->getBonusRatio()));
                uiMulti->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
                uiMulti->setPosition(Vec2(uiBoard->getContentSize().width, uiBoard->getContentSize().height));
                uiMulti->setName("UI_MULTI");
                uiBoard->addChild(uiMulti);
            }
        }
    }
    
    //
    if ( _unitPet != nullptr )
    {
        int idx = EventTreasureHuntManager::getInstance()->getBoardLastIdx();
        int pos = EventTreasureHuntManager::getInstance()->getBoardIdxToPos(idx);
        
        if ( idx == 0 || idx == 1 )
        {
            _unitPet->setPosition(Vec2(465, 965));
        }
        else
        {
            auto uiBoard = uiBoards->getChildByTag<ui::Button*>(pos);
            
            auto posWorld = uiBoard->getParent()->convertToWorldSpace(uiBoard->getPosition());
            auto posNode = _uiContentsBoards->convertToNodeSpace(posWorld);
            _unitPet->setPosition(Vec2(posNode.x + uiBoard->getContentSize().width / 2 + 12, posNode.y - uiBoard->getContentSize().height / 2 + 28));
        }
        
        setUnitState(E_HERO_STATE::IDLE);
    }
    
    //
    if ( EventTreasureHuntManager::getInstance()->isCompassUsed() == true )
    {
        int idx = EventTreasureHuntManager::getInstance()->getBoardLastIdx();
        int pos = EventTreasureHuntManager::getInstance()->getBoardIdxToPos(idx);
        int posBox = EventTreasureHuntManager::getInstance()->getBoardIdxToPos(1);
        
        int posPetX = pos % 7;
        int posPetY = pos / 7;
        int posBoxX = posBox % 7;
        int posBoxY = posBox / 7;
        
        double rotation = 0;
        if ( posPetY == posBoxY )
        {
            if ( posBoxX > posPetX )    rotation = 90;
            else                        rotation = 270;
        }
        else if ( posPetX == posBoxX )
        {
            if ( posBoxY > posPetY )    rotation = 180;
            else                        rotation = 0;
        }
        else if ( posPetX > posBoxX )
        {
            if ( posBoxY > posPetY )    rotation = 180 + 45;
            else                        rotation = 270 + 45;
        }
        else if ( posPetX < posBoxX )
        {
            if ( posBoxY > posPetY )    rotation = 90 + 45;
            else                        rotation = 0 + 45;
        }
        
        auto uiBoard = uiBoards->getChildByTag<ui::Button*>(pos);
        
        auto posWorld = uiBoard->getParent()->convertToWorldSpace(uiBoard->getPosition());
        auto posNode = _uiContentsBoards->convertToNodeSpace(posWorld);
        
        auto uiArrow = ui::ImageView::create("Assets/ui/event/event_compass3_1.png");
        uiArrow->setAnchorPoint(Vec2(0.5, -1));
        uiArrow->setPosition(Vec2(posNode.x + uiBoard->getContentSize().width / 2, posNode.y - uiBoard->getContentSize().height / 2));
        uiArrow->setName("UI_ARROW");
        uiArrow->setRotation(rotation);
        uiArrow->runAction(RepeatForever::create(Sequence::create(FadeOut::create(0.5), FadeIn::create(0.5), NULL)));
        layout->addChild(uiArrow);
    }
    else
    {
        if ( layout->getChildByName("UI_ARROW") != nullptr )
        {
            layout->removeChildByName("UI_ARROW");
        }
    }
    
    
}

void PopupEventTreasureHunt::drawRewards()
{
    auto layout = _uiContentsRewards;
    
    // so reward
    if ( auto uiRewardPanel = utils::findChild<ui::ImageView*>(layout, "UI_SO_REWARD"); uiRewardPanel != nullptr )
    {
        int type = 1;
        int rewardLevel = EventTreasureHuntManager::getInstance()->getRewardLevel(type);
        bool rewardLast = false;

        auto objRewardPrev = EventTreasureHuntManager::getInstance()->getInfoReward(type, rewardLevel);
        auto objRewardNext = EventTreasureHuntManager::getInstance()->getInfoReward(type, rewardLevel+1);
        if ( objRewardNext == nullptr )
        {
            objRewardNext = EventTreasureHuntManager::getInstance()->getInfoReward(type, rewardLevel);
            
            rewardLast = true;
        }
        
        int rewardValueNow = EventTreasureHuntManager::getInstance()->getRewardValue(type);
        int rewardValueMax = objRewardNext->getValue();
        if ( objRewardPrev != nullptr )
        {
            rewardValueNow -= objRewardPrev->getValue();
            rewardValueNow = MAX(rewardValueNow, 0);
            
            rewardValueMax -= objRewardPrev->getValue();
            rewardValueMax = MAX(rewardValueMax, 0);
        }
        
        double percent = (double)rewardValueNow / rewardValueMax * 100;
        if ( percent > 100 )
            percent = 100;
        
        //
        auto uiBar = utils::findChild<ui::LoadingBar*>(uiRewardPanel, "UI_BAR");
        uiBar->setPercent(percent);
        
        //
        auto uiBarText = utils::findChild<ui::CText*>(uiRewardPanel, "UI_BAR_TEXT");
        uiBarText->setString(MafUtils::format("(%d/%d)", rewardValueNow, rewardValueMax));
        if ( rewardLast == true && percent >= 100 )
        {
            uiBarText->setString("MAX");
        }
            
        
        //
        auto uiReward = utils::findChild<ui::Layout*>(uiRewardPanel, "UI_REWARD");
        uiReward->removeAllChildren();
        
        int idxStart = rewardLevel / 8 * 8 + 1;
        if ( auto obj = EventTreasureHuntManager::getInstance()->getInfoReward(type, idxStart); obj == nullptr )
        {
            idxStart = (rewardLevel / 8 - 1) * 8 + 1;
            idxStart = MAX(idxStart, 1);
        }
        for ( int i = idxStart; i < idxStart + 8; i++ )
        {
            auto objReward = EventTreasureHuntManager::getInstance()->getInfoReward(type, i);
            if ( objReward == nullptr )
            {
                continue;
            }
            
            auto objItem = objReward->getReward().at(0);
            
            std::string itemCount = objItem->getCount();
            if ( objItem->isString() == true )
            {
                MafUtils::convertNumberToShort(itemCount);
            }
            
            //
            auto pathBG = "Assets/ui_common/btn_2_6_on.png";
            if ( rewardLevel + 1 == i && percent >= 100 )
            {
                pathBG = "Assets/ui_common/btn_2_8_on.png";
            }
            
            auto uiItemBG = ui::Button::create(pathBG);
            uiItemBG->setScale9Enabled(true);
            uiItemBG->setContentSize(Size(72, 72));
            uiItemBG->addClickEventListener(CC_CALLBACK_1(PopupEventTreasureHunt::onClickMissioinRewardSo, this));
            uiItemBG->setTag(i);
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
            
            if ( i <= rewardLevel )
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
            if ( i <= rewardLevel )
            {
                uiItemIdxBG->loadTexture("Assets/ui_common/number_box_1_2.png");
            }
            
            auto uiItemIdx = ui::CText::create(MafUtils::format("%d", i), GAME_FONT, 18);
            uiItemIdx->setPosition(Vec2(uiItemIdxBG->getContentSize().width / 2, uiItemIdxBG->getContentSize().height / 2));
            uiItemIdxBG->addChild(uiItemIdx);
            
            //
            if ( rewardLevel + 1 == i && percent >= 100 )
            {
                auto uiArrow = ui::ImageView::create("Assets/ui/event/event_here_01.png");
                uiArrow->setPosition(Vec2(uiItemBG->getContentSize().width / 2, uiItemBG->getContentSize().height));
                uiArrow->runAction(RepeatForever::create(Sequence::create(MoveBy::create(0.5, Vec2(0, 10)), MoveBy::create(0.5, Vec2(0, -10)), NULL)));
                uiItemBG->addChild(uiArrow);
            }
            
        }
    }
    
    // co reward
    if ( auto uiRewardPanel = utils::findChild<ui::ImageView*>(layout, "UI_CO_REWARD"); uiRewardPanel != nullptr )
    {
        int type = 2;
        int rewardLevel = EventTreasureHuntManager::getInstance()->getRewardLevel(type);

        auto objRewardPrev = EventTreasureHuntManager::getInstance()->getInfoReward(type, rewardLevel);
        auto objRewardNext = EventTreasureHuntManager::getInstance()->getInfoReward(type, rewardLevel+1);
        if ( objRewardNext == nullptr )
        {
            objRewardNext = EventTreasureHuntManager::getInstance()->getInfoReward(type, rewardLevel);
        }
        
        int rewardValueNow = EventTreasureHuntManager::getInstance()->getRewardValue(type);
        int rewardValueMax = objRewardNext->getValue();
        if ( objRewardPrev != nullptr )
        {
            rewardValueNow -= objRewardPrev->getValue();
            rewardValueNow = MAX(rewardValueNow, 0);
            
            rewardValueMax -= objRewardPrev->getValue();
            rewardValueMax = MAX(rewardValueMax, 0);
        }
        
        double percent = (double)rewardValueNow / rewardValueMax * 100;
        if ( percent > 100 )
            percent = 100;
        
        //
        auto uiBar = utils::findChild<ui::LoadingBar*>(uiRewardPanel, "UI_BAR");
        uiBar->setPercent(percent);
        
        //
        auto uiBarText = utils::findChild<ui::CText*>(uiRewardPanel, "UI_BAR_TEXT");
        uiBarText->setString(MafUtils::format("(%d/%d)", rewardValueNow, rewardValueMax));
        
        //
        auto uiReward = utils::findChild<ui::Layout*>(uiRewardPanel, "UI_REWARD");
        uiReward->removeAllChildren();
        
        int idxStart = rewardLevel / 8 * 8 + 1;
        if ( auto obj = EventTreasureHuntManager::getInstance()->getInfoReward(type, idxStart); obj == nullptr )
        {
            idxStart = (rewardLevel / 8 - 1) * 8 + 1;
            idxStart = MAX(idxStart, 1);
        }
        for ( int i = idxStart; i < idxStart + 8; i++ )
        {
            auto objReward = EventTreasureHuntManager::getInstance()->getInfoReward(type, i);
            if ( objReward == nullptr )
            {
                continue;
            }
            
            auto objItem = objReward->getReward().at(0);
            
            std::string itemCount = objItem->getCount();
            if ( objItem->isString() == true )
            {
                MafUtils::convertNumberToShort(itemCount);
            }
            
            //
            auto pathBG = "Assets/ui_common/btn_2_6_on.png";
            if ( rewardLevel + 1== i && percent >= 100 )
            {
                pathBG = "Assets/ui_common/btn_2_8_on.png";
            }
            
            auto uiItemBG = ui::Button::create(pathBG);
            uiItemBG->setScale9Enabled(true);
            uiItemBG->setContentSize(Size(72, 72));
            uiItemBG->addClickEventListener(CC_CALLBACK_1(PopupEventTreasureHunt::onClickMissioinRewardCo, this));
            uiItemBG->setTag(i);
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
            
            if ( i <= rewardLevel )
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
            if ( i <= rewardLevel )
            {
                uiItemIdxBG->loadTexture("Assets/ui_common/number_box_1_2.png");
            }
            
            auto uiItemIdx = ui::CText::create(MafUtils::format("%d", i), GAME_FONT, 18);
            uiItemIdx->setPosition(Vec2(uiItemIdxBG->getContentSize().width / 2, uiItemIdxBG->getContentSize().height / 2));
            uiItemIdxBG->addChild(uiItemIdx);
            
            //
            if ( rewardLevel + 1 == i && percent >= 100 )
            {
                auto uiArrow = ui::ImageView::create("Assets/ui/event/event_here_01.png");
                uiArrow->setPosition(Vec2(uiItemBG->getContentSize().width / 2, uiItemBG->getContentSize().height));
                uiArrow->runAction(RepeatForever::create(Sequence::create(MoveBy::create(0.5, Vec2(0, 10)), MoveBy::create(0.5, Vec2(0, -10)), NULL)));
                uiItemBG->addChild(uiArrow);
            }
        }
    }
}

//
void PopupEventTreasureHunt::setUnitState(E_HERO_STATE eState)
{
    std::vector<std::string> listAnimation;
    if ( eState == E_HERO_STATE::IDLE )
    {
        listAnimation.push_back("Assets/ui/event/event_mole_idle_1.png");
        listAnimation.push_back("Assets/ui/event/event_mole_idle_2.png");
        listAnimation.push_back("Assets/ui/event/event_mole_idle_3.png");
        listAnimation.push_back("Assets/ui/event/event_mole_idle_4.png");
    }
    else if ( eState == E_HERO_STATE::MOVE )
    {
        listAnimation.push_back("Assets/ui/event/event_mole_walk_1.png");
        listAnimation.push_back("Assets/ui/event/event_mole_walk_2.png");
        listAnimation.push_back("Assets/ui/event/event_mole_walk_3.png");
        listAnimation.push_back("Assets/ui/event/event_mole_walk_4.png");
    }
    else if ( eState == E_HERO_STATE::ATTACK )
    {
        listAnimation.push_back("Assets/ui/event/event_mole_digging_1.png");
        listAnimation.push_back("Assets/ui/event/event_mole_digging_2.png");
        listAnimation.push_back("Assets/ui/event/event_mole_digging_3.png");
        listAnimation.push_back("Assets/ui/event/event_mole_digging_4.png");
    }
    
    auto animation = Animation::create();
    animation->setDelayPerUnit(0.1f);
    for ( int i = 0; i < listAnimation.size(); i++ )
    {
        auto ani = Sprite::create(listAnimation.at(i));
        ani->getTexture()->setTexParameters(texParams);
        
        animation->addSpriteFrame(ani->getSpriteFrame());
    }
    _unitPet->stopAllActions();
    _unitPet->runAction(RepeatForever::create(Animate::create(animation)));
}

#pragma mark - click
void PopupEventTreasureHunt::onClickClose(Ref* sender)
{
    hide();
}

void PopupEventTreasureHunt::onClickHelp(Ref* sender)
{
    auto popup = PopupHelp::create(3, "t_ui_event_treasure_hunt_info_title_%d", "t_ui_event_treasure_hunt_info_msg_%d");
    popup->show();
}

void PopupEventTreasureHunt::onClickBoard(Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto uiBoard = dynamic_cast<ui::Button*>(sender);
    
    int pos = uiBoard->getTag();
    int idx = EventTreasureHuntManager::getInstance()->getBoardPosToIdx(pos);
    
    if ( EventTreasureHuntManager::getInstance()->isBoardOpen(idx) == true )
    {
        return;
    }
    
    auto itemNeed = 1;
    auto itemNow = std::atoi(ItemsManager::getInstance()->getItems(E_ITEMS::EVENT_TREASURE_CHANCE).c_str());
    if ( itemNow < itemNeed )
    {
        return;
    }
    
    double timeDiff =  EventTreasureHuntManager::getInstance()->getEndTime() - UtilsDate::getInstance()->getTime();
    if ( timeDiff <= 0 )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_server_error_1"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, [=](Ref* sender){
            
            EventTreasureHuntManager::getInstance()->requestInfo(true, CC_CALLBACK_2(PopupEventTreasureHunt::callbackInfo, this));
            
        });
        popup->show();
        return;
    }
    
    //
    if ( _uiContentsBoards->getChildByName("UI_ARROW") != nullptr )
    {
        _uiContentsBoards->removeChildByName("UI_ARROW");
    }
    
    //
    auto posWorld = uiBoard->getParent()->convertToWorldSpace(uiBoard->getPosition());
    auto posNode = _uiContentsBoards->convertToNodeSpace(posWorld);
    _unitPet->setPosition(Vec2(posNode.x + uiBoard->getContentSize().width / 2 + 12, posNode.y - uiBoard->getContentSize().height / 2 + 28));
    setUnitState(E_HERO_STATE::ATTACK);
    
    //
    PopupLoadingLock::show();
    
    //
    auto action1 = DelayTime::create(0.5);
    auto action2 = CallFunc::create([=](){
        EventTreasureHuntManager::getInstance()->requestBoard(false, idx, CC_CALLBACK_2(PopupEventTreasureHunt::callbackBoard, this));
    });
    auto seq = Sequence::create(action1, action2, NULL);
    _unitPet->runAction(seq);
}

void PopupEventTreasureHunt::onClickCompass(Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    double timeDiff =  EventTreasureHuntManager::getInstance()->getEndTime() - UtilsDate::getInstance()->getTime();
    if ( timeDiff <= 0 )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_server_error_1"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, [=](Ref* sender){
            
            EventTreasureHuntManager::getInstance()->requestInfo(true, CC_CALLBACK_2(PopupEventTreasureHunt::callbackInfo, this));
            
        });
        popup->show();
        return;
    }
    
    int idxLast = EventTreasureHuntManager::getInstance()->getBoardLastIdx();
    if ( idxLast == 0 )
    {
        return;
    }
    
    bool bFinish = EventTreasureHuntManager::getInstance()->isBoardFinish();
    if ( bFinish == true )
    {
        return;
    }
    
    bool bUsed = EventTreasureHuntManager::getInstance()->isCompassUsed();
    if ( bUsed == true )
    {
        return;
    }
    
    int compassMax = EventTreasureHuntManager::getInstance()->getCompassMax();
    int compassNow = EventTreasureHuntManager::getInstance()->getCompassNow();
    if ( compassNow < compassMax )
    {
        return;
    }
    
    
    
    
    EventTreasureHuntManager::getInstance()->requestCompass(true, CC_CALLBACK_2(PopupEventTreasureHunt::callbackCompass, this));
}

void PopupEventTreasureHunt::onClickRefresh(Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto uiBtn = dynamic_cast<ui::Button*>(sender);
    
    double timeDiff =  EventTreasureHuntManager::getInstance()->getEndTime() - UtilsDate::getInstance()->getTime();
    if ( timeDiff <= 0 )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_server_error_1"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, [=](Ref* sender){
            
            EventTreasureHuntManager::getInstance()->requestInfo(true, CC_CALLBACK_2(PopupEventTreasureHunt::callbackInfo, this));
            
        });
        popup->show();
        return;
    }
    
    bool bFinish = EventTreasureHuntManager::getInstance()->isBoardFinish();
    if ( bFinish == true )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_event_treasure_hunt_popup_msg_1"));
        popup->addButton(GAME_TEXT("t_ui_cancel"), true, nullptr);
        popup->addButton(GAME_TEXT("t_ui_confirm"), false, [=](Ref* sender){
            
            EventTreasureHuntManager::getInstance()->requestReset(true, CC_CALLBACK_2(PopupEventTreasureHunt::callbackRefresh, this));
            
        });
        popup->show();
    }
    else
    {
        int itemIdx = 0;
        int itemNeed = 0;
        std::string data = EventManager::getInstance()->getEventOhterDataByEventIdx(39);
        if ( data.empty() == false )
        {
            rapidjson::Document jsonParser;
            jsonParser.ParseInsitu<0>((char*)data.c_str());
            if( jsonParser.HasParseError() == false )
            {
                itemIdx = jsonParser["reset_cost"]["idx"].GetInt();
                itemNeed = jsonParser["reset_cost"]["count"].GetInt();
            }
        }
        
        if ( itemIdx == 0 || itemNeed == 0 )
        {
            return;
        }
        
        auto itemNow = std::atoi(ItemsManager::getInstance()->getItems((E_ITEMS)itemIdx).c_str());
        if ( itemNow < itemNeed )
        {
            return;
        }
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_event_treasure_hunt_popup_msg_2"));
        popup->addButton(GAME_TEXT("t_ui_cancel"), true, nullptr);
        popup->addButton(GAME_TEXT("t_ui_confirm"), false, [=](Ref* sender){
            
            EventTreasureHuntManager::getInstance()->requestReset(true, CC_CALLBACK_2(PopupEventTreasureHunt::callbackRefresh, this));
            
        });
        popup->show();
    }
    
    
}

void PopupEventTreasureHunt::onClickMissioinRewardSo(Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto uiBtn = dynamic_cast<ui::Button*>(sender);
    
    double timeDiff =  EventTreasureHuntManager::getInstance()->getEndTime() - UtilsDate::getInstance()->getTime();
    if ( timeDiff <= 0 )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_server_error_1"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, [=](Ref* sender){
            
            EventTreasureHuntManager::getInstance()->requestInfo(true, CC_CALLBACK_2(PopupEventTreasureHunt::callbackInfo, this));
            
        });
        popup->show();
        return;
    }
    
    int type = 1;
    int rewardLevelNow = EventTreasureHuntManager::getInstance()->getRewardLevel(type) + 1;
    int rewardLevelRequest = uiBtn->getTag();
    
    if ( rewardLevelNow != rewardLevelRequest )
    {
        return;
    }

    auto objRewardNow = EventTreasureHuntManager::getInstance()->getInfoReward(type, rewardLevelNow);
    if ( objRewardNow == nullptr )
    {
        return;
    }
    
    int rewardValueNow = EventTreasureHuntManager::getInstance()->getRewardValue(type);
    int rewardValueMax = objRewardNow->getValue();
    if ( rewardValueNow < rewardValueMax )
    {
        return;
    }
    
    EventTreasureHuntManager::getInstance()->requestMissionReward(true, type, rewardLevelNow, CC_CALLBACK_2(PopupEventTreasureHunt::callbackMissionReward, this));
}

void PopupEventTreasureHunt::onClickMissioinRewardCo(Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto uiBtn = dynamic_cast<ui::Button*>(sender);
    
    double timeDiff =  EventTreasureHuntManager::getInstance()->getEndTime() - UtilsDate::getInstance()->getTime();
    if ( timeDiff <= 0 )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_server_error_1"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, [=](Ref* sender){
            
            EventTreasureHuntManager::getInstance()->requestInfo(true, CC_CALLBACK_2(PopupEventTreasureHunt::callbackInfo, this));
            
        });
        popup->show();
        return;
    }
    
    int type = 2;
    int rewardLevelNow = EventTreasureHuntManager::getInstance()->getRewardLevel(type) + 1;
    int rewardLevelRequest = uiBtn->getTag();
    
    if ( rewardLevelNow != rewardLevelRequest )
    {
        return;
    }

    auto objRewardNow = EventTreasureHuntManager::getInstance()->getInfoReward(type, rewardLevelNow);
    if ( objRewardNow == nullptr )
    {
        return;
    }
    
    int rewardValueNow = EventTreasureHuntManager::getInstance()->getRewardValue(type);
    int rewardValueMax = objRewardNow->getValue();
    if ( rewardValueNow < rewardValueMax )
    {
        return;
    }
    
    EventTreasureHuntManager::getInstance()->requestMissionReward(true, type, rewardLevelNow, CC_CALLBACK_2(PopupEventTreasureHunt::callbackMissionReward, this));
}

void PopupEventTreasureHunt::onClickAD(Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    double timeDiff =  EventTreasureHuntManager::getInstance()->getEndTime() - UtilsDate::getInstance()->getTime();
    if ( timeDiff <= 0 )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_server_error_1"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, [=](Ref* sender){
            
            EventTreasureHuntManager::getInstance()->requestInfo(true, CC_CALLBACK_2(PopupEventTreasureHunt::callbackInfo, this));
            
        });
        popup->show();
        return;
    }
    
    int adMax = EventTreasureHuntManager::getInstance()->getADMax();
    int adNow = EventTreasureHuntManager::getInstance()->getADNow();
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
        MafGoogleRewardAds::ShowGoogleRewardedVideo(this, CC_CALLBACK_1(PopupEventTreasureHunt::callbackADResult, this));
    }
}
 

#pragma mark - callback
void PopupEventTreasureHunt::callbackInfo(bool bResult, int nResult)
{
    if ( bResult == false )
    {
        std::string strMessage = GAME_TEXT("t_ui_error_30");
        if ( nResult == 0 )
            strMessage = GAME_TEXT("t_ui_draw_msg_1");
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), strMessage);
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        
        hide();
        return;
    }
    
    //
    drawInfo();
    drawBoards();
    drawRewards();
}

void PopupEventTreasureHunt::callbackRefresh(bool bResult, int nResult)
{
    if ( bResult == false )
    {
        std::string strMessage = GAME_TEXT("t_ui_error_30");
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), strMessage);
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        
        hide();
        return;
    }
    
    //
    auto uiBoards = utils::findChild<ui::Layout*>(_uiContentsBoards, "UI_BOARDS");
    if ( uiBoards != nullptr )
    {
        uiBoards->removeAllChildren();
    }
    
    //
    drawInfo();
    drawBoards();
    drawRewards();
}

void PopupEventTreasureHunt::callbackCompass(bool bResult, int nResult)
{
    if ( bResult == false )
    {
        std::string strMessage = GAME_TEXT("t_ui_error_30");
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), strMessage);
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    //
    drawInfo();
    drawBoards();
}

void PopupEventTreasureHunt::callbackBoard(bool bResult, int nResult)
{
    PopupLoadingLock::hide();
    
    if ( bResult == false )
    {
        std::string strMessage = GAME_TEXT("t_ui_error_30");
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), strMessage);
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        
        hide();
        return;
    }
    
    //
    drawInfo();
    drawBoards();
    drawRewards();
    
    //
    if ( auto uiBoards = utils::findChild<ui::Layout*>(_uiContentsBoards, "UI_BOARDS"); uiBoards != nullptr )
    {
        int idx = EventTreasureHuntManager::getInstance()->getBoardLastIdx();
        int pos = EventTreasureHuntManager::getInstance()->getBoardIdxToPos(idx);
        
        auto uiBoard = uiBoards->getChildByTag<ui::Button*>(pos);

        auto posWorld = uiBoard->getParent()->convertToWorldSpace(uiBoard->getPosition());
        auto posNode = _uiContentsBoards->convertToNodeSpace(posWorld);
        
        //
        auto animation = Animation::create();
        animation->setDelayPerUnit(0.1f);
        for ( int i = 1; i <= 6; i++ )
        {
            auto ani = Sprite::create(MafUtils::format("Assets/ui/event/farm_blocks_fx_1_%d.png", i));
            ani->getTexture()->setTexParameters(texParams);
            
            animation->addSpriteFrame(ani->getSpriteFrame());
        }
        
        auto uiEffect = Sprite::create("Assets/ui/event/farm_blocks_fx_1_1.png");
        uiEffect->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        uiEffect->setPosition(Vec2(uiBoard->getContentSize().width / 2, 0));
        uiEffect->setPosition(Vec2(posNode.x + uiBoard->getContentSize().width / 2, posNode.y - uiBoard->getContentSize().height));
        uiEffect->runAction(Sequence::create(Animate::create(animation), RemoveSelf::create(), NULL));
        _uiContentsBoards->addChild(uiEffect, 10);
    }
    
    auto uiPos = ui::ImageView::create("Assets/ui/event/event_cha_select.png");
    uiPos->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    uiPos->setPosition(Vec2(_unitPet->getContentSize().width / 2 - 10, _unitPet->getContentSize().height - 20));
    uiPos->runAction(RepeatForever::create(Sequence::create(FadeOut::create(0.5), FadeIn::create(0.5), NULL)));
    uiPos->runAction(Sequence::create(DelayTime::create(3.0), RemoveSelf::create(), NULL));
    uiPos->setName("UI_POS");
    _unitPet->removeChildByName("UI_POS");
    _unitPet->addChild(uiPos);
    
}

void PopupEventTreasureHunt::callbackMissionReward(bool bResult, int nResult)
{
    PopupLoadingLock::hide();
    
    if ( bResult == false )
    {
        std::string strMessage = GAME_TEXT("t_ui_error_30");
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), strMessage);
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        
        hide();
        return;
    }
    
    //
    drawInfo();
    drawBoards();
    drawRewards();
}

void PopupEventTreasureHunt::callbackADResult(ADS_RESULT result)
{
    if ( result == ADS_RESULT::SUCCESS )
    {
        //
        MafRewardAdsManager::getInstance()->setHistoryData(kAdsSlotMiniGame, CashManager::getInstance()->isAdsfreeTime());
        
        //
        EventTreasureHuntManager::getInstance()->requestAD(true, CC_CALLBACK_2(PopupEventTreasureHunt::callbackAD, this));
    }
    else
    {
        if ( result == ADS_RESULT::LOAD )
        {
            auto popup = PopupAdsMafGames::create();
            popup->setCallback(CC_CALLBACK_1(PopupEventTreasureHunt::callbackADResult, this));
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

void PopupEventTreasureHunt::callbackAD(bool bResult, int nResult)
{
    PopupLoadingLock::hide();
    
    if ( bResult == false )
    {
        std::string strMessage = GAME_TEXT("t_ui_error_30");
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), strMessage);
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    //
    drawInfo();
}

