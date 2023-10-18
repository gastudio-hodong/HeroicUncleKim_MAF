//
//  PopupEventDrawLimit.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/10/13.
//

#include "PopupEventDrawLimit.h"

#include "Common/ConfigGameNormal.h"

#include "GameObject/EventDraw/InfoEventDraw.h"
#include "GameObject/EventDraw/InfoEventDrawBonus.h"

#include "GameUI/UI/UIReddot.h"
#include "GameUI/UI/UICurrency.h"

#include "GameUIPopup/Other/Settings/Mail/MailPopup.h"
#include "GameUIPopup/Other/PurchaseCash/PopupCash.h"
#include "GameUIPopup/OtherEvent/EventToken/PopupEventDrawLimitInfo.h"
#include "GameUIPopup/OtherEvent/EventToken/Layer/LayerEventDrawLimitAni.h"
#include "GameUIPopup/OtherEvent/EventToken/Layer/LayerEventDrawLimitResult.h"

#include "ManagerEvent/EventDrawLimitManager.h"

USING_NS_CC;

PopupEventDrawLimit::PopupEventDrawLimit(void) :
_bInitUI(false),

//

// widget
_uiContentsDefault(nullptr),
_uiContentsBestItems(nullptr),
_uiContentsInfo(nullptr),
_uiContentsButtons(nullptr),

_uiList(nullptr)
{
    
}

PopupEventDrawLimit::~PopupEventDrawLimit(void)
{
    
}

bool PopupEventDrawLimit::init()
{
    if ( !PopupBase::init() )
    {
        return false;
    }
    
    //
    initVar();
    initUi();
    
    // 로그 이벤트
    MafAnalyticsManager::LogEvent(kAnalEventTokenEnter, kRepeatTrue);

    return true;
}

void PopupEventDrawLimit::onEnter()
{
    PopupBase::onEnter();
    
    if ( _bInitUI == false )
    {
        uiContainer();
        uiDefault();
        uiBestItems();
        uiInfo();
        uiButtons();
        
        // network
        EventDrawLimitManager::getInstance()->requestInfo(true, CC_CALLBACK_2(PopupEventDrawLimit::callbackNetworkInfo, this));
        
        _bInitUI = true;
    }
    
    //
    drawDefault();
    drawBestItems();
    drawInfo();
    drawButtons();
}

void PopupEventDrawLimit::backkey()
{
    if ( _spriteContainer->getChildByName("UI_DRAW_ANI") != nullptr )
    {
        return;
    }
    
    if ( _spriteContainer->getChildByName("UI_DRAW_RESULT") != nullptr )
    {
        return;
    }
    
    hide();
}

#pragma mark - override DelegateListView
void PopupEventDrawLimit::listviewScrollView(ui::CListView* listview, ui::ScrollView::EventType type)
{
    switch (type) {
        case ui::ScrollView::EventType::SCROLL_TO_BOTTOM:
            break;
        case ui::ScrollView::EventType::SCROLL_TO_TOP:
            break;
        default:
            break;
    }
}

Size PopupEventDrawLimit::listviewCellSize(ui::CListView* listview, ssize_t idx)
{
    double width = listview->getContentSize().width ;
    double height = 232 + 20 ;

    return Size(width, height);
}

void PopupEventDrawLimit::listviewCell(ui::CListView *listview, ui::Widget* childview, ssize_t idx)
{
    // size
    Size size = childview->getContentSize();
    Size sizeLayout = Size(702, size.height - 20);

    // obj
    ssize_t nIdxStart = idx * 4;
    ssize_t nIdxEnd = nIdxStart + 4;
    if ( nIdxEnd > _listInfoDraw.size() )
    {
        nIdxEnd = _listInfoDraw.size();
    }
    
    // ui
    childview->removeAllChildren();

    auto uiLayout = ui::Layout::create();
    uiLayout->setContentSize(Size(sizeLayout.width, sizeLayout.height));
    uiLayout->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    uiLayout->setPosition(Vec2(size.width / 2, size.height));
    uiLayout->setLayoutType(ui::Layout::Type::HORIZONTAL);
    childview->addChild(uiLayout);
    
    for ( int i = 0; nIdxStart + i < nIdxEnd; i++ )
    {
        auto obj = _listInfoDraw.at(nIdxStart + i);
        
        auto uiBG = ui::ImageView::create("Assets/ui/event/token_reward_1_1.png");
        uiLayout->addChild(uiBG);
        if ( i != 0 )
        {
            auto params = ui::LinearLayoutParameter::create();
            params->setMargin(ui::Margin(10, 0, 0, 0));
            uiBG->setLayoutParameter(params);
        }
        
        auto objReward = obj->getReward();
        if ( objReward == nullptr )
        {
            continue;
        }
        
        auto uiIcon = ui::ImageView::create(objReward->getPath());
        uiIcon->setPosition(Vec2(uiBG->getContentSize().width / 2, uiBG->getContentSize().height - 70));
        ((ui::Scale9Sprite*)uiIcon->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
        uiIcon->setScale(1.8);
        uiBG->addChild(uiIcon);
        
        auto uiCount = ui::CText::create(MafUtils::format("x %s", objReward->getCount().c_str()), GAME_FONT, 20);
        uiCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        uiCount->setPosition(Vec2(uiBG->getContentSize().width / 2, uiBG->getContentSize().height / 2 - 20));
        uiBG->addChild(uiCount);
        
        auto uiName = ui::CText::create(objReward->getText(), GAME_FONT, 20);
        uiName->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        uiName->setPosition(Vec2(uiBG->getContentSize().width / 2, 40));
        uiName->setColor(Color3B(160, 148, 109));
        uiBG->addChild(uiName);
        
        auto uiCountLimit = ui::CText::create(GAME_TEXTFORMAT("t_ui_token_text_remain", obj->getCountLimit()), GAME_FONT, 20);
        uiCountLimit->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        uiCountLimit->setPosition(Vec2(uiBG->getContentSize().width / 2, 40));
        uiCountLimit->setColor(Color3B(160, 148, 109));
        uiBG->addChild(uiCountLimit);
    }
}

int64_t PopupEventDrawLimit::listviewNumberOfCells(ui::CListView *listview)
{
    int64_t count = _listInfoDraw.size() / 4;
    if ( _listInfoDraw.size() % 4 != 0 )
        count++;
    
    return count;
}

#pragma mark - init
void PopupEventDrawLimit::initVar()
{
    
}

void PopupEventDrawLimit::initUi()
{
    // size
    _spriteContainer->setContentSize( Size(750, 1334) );
    
    // label title
    auto uiTitle = ui::CText::create(GAME_TEXT("t_ui_token_title"), GAME_FONT, 32);
    uiTitle->setPosition(Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5));
    uiTitle->setTextColor(Color4B(COLOR_TITLE));
    _spriteContainer->addChild(uiTitle);
    
    // item close
    auto uiClose = ui::Button::create("Assets/ui_common/btn_x.png");
    uiClose->setPosition(Vec2(_spriteContainer->getContentSize().width - uiClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5));
    uiClose->addClickEventListener(CC_CALLBACK_1(PopupEventDrawLimit::onClickClose, this));
    _spriteContainer->addChild(uiClose);
    
    //
    std::vector<E_ITEMS> list;
    list.push_back(E_ITEMS::EVENT_TOKEN);
    list.push_back(E_ITEMS::EVENT_TOKEN_PIECE);
    
    auto uiCurrency = UICurrency::create(list, Size(_spriteContainer->getContentSize().width, 50));
    uiCurrency->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    uiCurrency->setPosition(Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75));
    _spriteContainer->addChild(uiCurrency);
}

#pragma mark - ui :
void PopupEventDrawLimit::uiContainer()
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
    
    //
    _uiContentsDefault = ui::Layout::create();
    _uiContentsDefault->setContentSize( Size(size.width, 416) );
    _uiContentsDefault->setIgnoreAnchorPointForPosition(false);
    _uiContentsDefault->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _uiContentsDefault->setPosition( Vec2(size.width / 2, size.height) );
    layout->addChild(_uiContentsDefault);
    
    _uiContentsButtons = ui::Layout::create();
    _uiContentsButtons->setContentSize(Size(size.width - 42, 100));
    _uiContentsButtons->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _uiContentsButtons->setPosition( Vec2(size.width / 2, 0) );
    layout->addChild(_uiContentsButtons);
    
    _uiContentsInfo = ui::Layout::create();
    _uiContentsInfo->setContentSize(Size(size.width - 42, 342));
    _uiContentsInfo->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _uiContentsInfo->setPosition( Vec2(size.width / 2, _uiContentsButtons->getContentSize().height) );
    layout->addChild(_uiContentsInfo);
    
    _uiContentsBestItems = ui::Layout::create();
    _uiContentsBestItems->setContentSize( Size(size.width, size.height - _uiContentsDefault->getContentSize().height - _uiContentsButtons->getContentSize().height - _uiContentsInfo->getContentSize().height ) );
    _uiContentsBestItems->setIgnoreAnchorPointForPosition(false);
    _uiContentsBestItems->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _uiContentsBestItems->setPosition( Vec2(size.width / 2, size.height - _uiContentsDefault->getContentSize().height) );
    layout->addChild(_uiContentsBestItems);
    
}

void PopupEventDrawLimit::uiDefault()
{
    auto layout = _uiContentsDefault;
    auto size = layout->getContentSize();
    
    auto uiBG = ui::ImageView::create("Assets/ui/event/gacha_bg_1.png");
    uiBG->setPosition(Vec2(size.width / 2, size.height / 2));
    ((ui::Scale9Sprite*)uiBG->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
    uiBG->setScale(4);
    layout->addChild(uiBG);

    auto uiTime = ui::CText::create(" ", GAME_FONT, 24);
    uiTime->setPosition(Vec2(size.width / 2, size.height - 18));
    uiTime->enableBold();
    uiTime->setName("UI_TIME");
    layout->addChild(uiTime);
    
    auto uiHelp = ui::Button::create("Assets/ui_common/icon_help.png");
    uiHelp->setPosition(Vec2(46, size.height - 90));
    uiHelp->addClickEventListener(CC_CALLBACK_1(PopupEventDrawLimit::onClickHelp, this));
    layout->addChild(uiHelp);
    {
        auto uiText = ui::CText::create(GAME_TEXT("t_ui_token_event_info_bt"), GAME_FONT, 20);
        uiText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        uiText->setPosition(Vec2(uiHelp->getContentSize().width / 2, 0));
        uiText->enableOutline(Color4B::BLACK, 2);
        uiHelp->addChild(uiText);
    }
    
}

void PopupEventDrawLimit::uiBestItems()
{
    auto layout = _uiContentsBestItems;
    auto size = layout->getContentSize();
    
    auto uiTime = ui::CText::create(" ", GAME_FONT, 24);
    uiTime->setPosition(Vec2(size.width / 2, size.height - 31));
    uiTime->enableBold();
    uiTime->setName("UI_TIME");
    layout->addChild(uiTime);
    
    _uiList = ui::CListView::create(this, Size(size.width, 263), ui::ScrollView::Direction::VERTICAL);
    _uiList->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _uiList->setPosition(Vec2(size.width / 2, 0));
    _uiList->setBounceEnabled(false);
    _uiList->setScrollBarPositionFromCorner(Vec2(7, 7));
    _uiList->setScrollBarColor(Color3B::YELLOW);
    _uiList->setScrollBarAutoHideEnabled(false);
    _uiList->reloadData();
    layout->addChild(_uiList);
}

void PopupEventDrawLimit::uiInfo()
{
    auto layout = _uiContentsInfo;
    auto size = layout->getContentSize();
    
    auto uiLayout = ui::Layout::create();
    uiLayout->setContentSize(Size(696, size.height));
    uiLayout->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    uiLayout->setPosition(Vec2(size.width / 2, size.height));
    uiLayout->setLayoutType(ui::Layout::Type::VERTICAL);
    layout->addChild(uiLayout);
    
    auto uiBonus = ui::ImageView::create("Assets/ui_common/list_bg_1.png");
    uiBonus->setScale9Enabled(true);
    uiBonus->setCapInsets(Rect(0,51,150,10));
    uiBonus->setContentSize(Size(696, 150));
    uiBonus->setPosition(Vec2(uiLayout->getContentSize().width / 2, uiLayout->getContentSize().height / 2));
    uiBonus->setName("UI_BONUS");
    uiLayout->addChild(uiBonus);
    {
        auto uiIcon = ui::ImageView::create("Assets/ui/event/icon_tokenbox.png");
        uiIcon->setPosition(Vec2(30, uiBonus->getContentSize().height - 25.5));
        uiBonus->addChild(uiIcon);
        
        auto uiText = ui::CText::create(GAME_TEXT("t_ui_token_text_bonus"), GAME_FONT, 24);
        uiText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        uiText->setPosition(Vec2(70, uiBonus->getContentSize().height - 25.5));
        uiText->setTextAreaSize(Size(600, uiText->getContentSize().height));
        uiText->setTextOverflow(Label::Overflow::SHRINK);
        uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiText->setTextHorizontalAlignment(TextHAlignment::LEFT);
        uiBonus->addChild(uiText);
        
        auto uiBarBG = ui::ImageView::create("Assets/ui/event/event_progressbar_2_1.png");
        uiBarBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        uiBarBG->setPosition(Vec2(uiBonus->getContentSize().width - 188, 49.5));
        uiBonus->addChild(uiBarBG);
        
        auto uiBar = ui::LoadingBar::create("Assets/ui/event/event_progressbar_2_2.png");
        uiBar->setPercent(50);
        uiBar->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        uiBar->setPosition(Vec2(uiBarBG->getContentSize().width, uiBarBG->getContentSize().height / 2));
        uiBar->setName("UI_BAR");
        uiBarBG->addChild(uiBar);
        
        auto uiBarCount = ui::CText::create("LV0", GAME_FONT_AL, 24);
        uiBarCount->setPosition(Vec2(uiBarBG->getContentSize().width / 2, uiBarBG->getContentSize().height / 2));
        uiBarCount->setName("UI_BAR_COUNT");
        uiBarBG->addChild(uiBarCount);
        
        auto uiReward = ui::Button::create("Assets/ui_common/btn_2_1_on.png");
        uiReward->setCapInsets(Rect(15,25,10,10));
        uiReward->setScale9Enabled(true);
        uiReward->setContentSize(Size(162, 68));
        uiReward->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        uiReward->setPosition(Vec2(uiBonus->getContentSize().width - 180, 49.5));
        uiReward->addClickEventListener(CC_CALLBACK_1(PopupEventDrawLimit::onClickBonus, this));
        uiReward->setName("UI_REWARD");
        uiBonus->addChild(uiReward);
    }
    
    auto uiMission = ui::ImageView::create("Assets/ui_common/list_bg_1.png");
    uiMission->setScale9Enabled(true);
    uiMission->setCapInsets(Rect(0,51,150,10));
    uiMission->setContentSize(Size(696, 150));
    uiMission->setPosition(Vec2(uiLayout->getContentSize().width / 2, uiLayout->getContentSize().height / 2));
    uiMission->setName("UI_MISSION");
    uiLayout->addChild(uiMission);
    {
        auto params = ui::LinearLayoutParameter::create();
        params->setMargin(ui::Margin(0, 10, 0, 0));
        uiMission->setLayoutParameter(params);
        
        auto uiIcon = ui::ImageView::create("Assets/ui/bg_icon/icon_mission.png");
        uiIcon->setPosition(Vec2(30, uiMission->getContentSize().height - 25.5));
        uiMission->addChild(uiIcon);
        
        auto uiText = ui::CText::create(GAME_TEXT("t_ui_token_event_msg1"), GAME_FONT, 24);
        uiText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        uiText->setPosition(Vec2(70, uiMission->getContentSize().height - 25.5));
        uiText->setTextAreaSize(Size(600, uiText->getContentSize().height));
        uiText->setTextOverflow(Label::Overflow::SHRINK);
        uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiText->setTextHorizontalAlignment(TextHAlignment::LEFT);
        uiMission->addChild(uiText);
        
        auto uiBarBG = ui::ImageView::create("Assets/ui/event/event_progressbar_1_1.png");
        uiBarBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        uiBarBG->setPosition(Vec2(uiMission->getContentSize().width - 188, 49.5));
        uiMission->addChild(uiBarBG);
        
        auto uiBar = ui::LoadingBar::create("Assets/ui/event/event_progressbar_1_2.png");
        uiBar->setPercent(50);
        uiBar->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        uiBar->setPosition(Vec2(uiBarBG->getContentSize().width, uiBarBG->getContentSize().height / 2));
        uiBar->setName("UI_BAR");
        uiBarBG->addChild(uiBar);
        
        auto uiBarCount = ui::CText::create("0/0", GAME_FONT_AL, 24);
        uiBarCount->setPosition(Vec2(uiBarBG->getContentSize().width / 2, uiBarBG->getContentSize().height / 2));
        uiBarCount->setName("UI_BAR_COUNT");
        uiBarBG->addChild(uiBarCount);
        
        auto uiReward = ui::Button::create("Assets/ui_common/btn_2_1_on.png");
        uiReward->setCapInsets(Rect(15,25,10,10));
        uiReward->setScale9Enabled(true);
        uiReward->setContentSize(Size(162, 68));
        uiReward->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        uiReward->setPosition(Vec2(uiMission->getContentSize().width - 180, 49.5));
        uiReward->addClickEventListener(CC_CALLBACK_1(PopupEventDrawLimit::onClickTokenPieceExchange, this));
        uiReward->setName("UI_REWARD");
        uiMission->addChild(uiReward);
        {
            auto uiCost = UtilsWidget::getInstance()->createIconText("Assets/ui/event/icon_token.png", GAME_TEXT("t_ui_token_event_receive_bt"), uiReward->getContentSize().width - 10, 24, 15);
            uiCost->setPosition(Vec2(uiReward->getContentSize().width / 2, uiReward->getContentSize().height / 2));
            uiCost->setName("UI_COST");
            uiReward->addChild(uiCost);
        }
    }
}

void PopupEventDrawLimit::uiButtons()
{
    auto layout = _uiContentsButtons;
    auto size = layout->getContentSize();
    
    auto uiDefault = ui::Layout::create();
    uiDefault->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    uiDefault->setPositionType(ui::Widget::PositionType::PERCENT);
    uiDefault->setPositionPercent(Vec2::ANCHOR_MIDDLE);
    uiDefault->setLayoutType(ui::Layout::Type::HORIZONTAL);
    uiDefault->setName("UI_DEFAULT");
    layout->addChild(uiDefault);
    {
        auto uiDrawOnce = ui::Button::create("Assets/ui_common/btn_1_1_on.png");
        uiDrawOnce->setCapInsets(Rect(33,33,2,2));
        uiDrawOnce->setScale9Enabled(true);
        uiDrawOnce->setContentSize(Size(290, 100));
        uiDrawOnce->addClickEventListener(CC_CALLBACK_1(PopupEventDrawLimit::onClickDraw, this));
        uiDrawOnce->setTag(1);
        uiDrawOnce->setName("UI_DRAW_ONCE");
        uiDefault->addChild(uiDrawOnce);
        {
            auto uiText = ui::CText::create(GAME_TEXT("t_ui_token_btn_buy_single_1"), GAME_FONT, 28);
            uiText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
            uiText->setPosition(Vec2(uiDrawOnce->getContentSize().width / 2, uiDrawOnce->getContentSize().height / 2));
            uiText->setTextAreaSize(Size(uiDrawOnce->getContentSize().width - 10, uiText->getContentSize().height));
            uiText->setTextOverflow(Label::Overflow::SHRINK);
            uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
            uiText->setTextHorizontalAlignment(TextHAlignment::CENTER);
            uiDrawOnce->addChild(uiText);
        }
        
        auto uiDrawMany = ui::Button::create("Assets/ui_common/btn_1_1_on.png");
        uiDrawMany->setCapInsets(Rect(33,33,2,2));
        uiDrawMany->setScale9Enabled(true);
        uiDrawMany->setContentSize(Size(290, 100));
        uiDrawMany->addClickEventListener(CC_CALLBACK_1(PopupEventDrawLimit::onClickDraw, this));
        uiDrawMany->setTag(10);
        uiDrawMany->setName("UI_DRAW_MANY");
        uiDefault->addChild(uiDrawMany);
        {
            auto params = ui::LinearLayoutParameter::create();
            params->setMargin(ui::Margin(10, 0, 0, 0));
            uiDrawMany->setLayoutParameter(params);
            
            auto uiText = ui::CText::create(GAME_TEXT("t_ui_token_btn_buy_multiple_1"), GAME_FONT, 28);
            uiText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
            uiText->setPosition(Vec2(uiDrawMany->getContentSize().width / 2, uiDrawMany->getContentSize().height / 2));
            uiText->setTextAreaSize(Size(uiDrawMany->getContentSize().width - 10, uiText->getContentSize().height));
            uiText->setTextOverflow(Label::Overflow::SHRINK);
            uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
            uiText->setTextHorizontalAlignment(TextHAlignment::CENTER);
            uiDrawMany->addChild(uiText);
        }
    }
    
    
    auto uiExtraTime = ui::Layout::create();
    uiExtraTime->setContentSize(size);
    uiExtraTime->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    uiExtraTime->setPositionType(ui::Widget::PositionType::PERCENT);
    uiExtraTime->setPositionPercent(Vec2::ANCHOR_MIDDLE);
    uiExtraTime->setName("UI_EXTRA_TIME");
    layout->addChild(uiExtraTime);
    {
        auto uiText = ui::CText::create(GAME_TEXT("t_ui_token_event_end_exchange_msg"), GAME_FONT, 24);
        uiText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        uiText->setPosition(Vec2(0, uiExtraTime->getContentSize().height / 2));
        uiText->setTextAreaSize(Size(450, uiExtraTime->getContentSize().height));
        uiText->setTextOverflow(Label::Overflow::CLAMP);
        uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiText->setTextHorizontalAlignment(TextHAlignment::LEFT);
        uiText->setTextColor(Color4B(COLOR_TITLE));
        uiExtraTime->addChild(uiText);
        
        auto uiExchange = ui::Button::create("Assets/ui_common/btn_1_1_on.png");
        uiExchange->setCapInsets(Rect(33,33,2,2));
        uiExchange->setScale9Enabled(true);
        uiExchange->setContentSize(Size(222, 116));
        uiExchange->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        uiExchange->setPosition(Vec2(uiExtraTime->getContentSize().width, uiExtraTime->getContentSize().height / 2));
        uiExchange->addClickEventListener(CC_CALLBACK_1(PopupEventDrawLimit::onClickTokenExchange, this));
        uiExtraTime->addChild(uiExchange);
        {
            auto uiText = ui::CText::create(GAME_TEXT("t_ui_token_event_end_exchange_bt"), GAME_FONT, 28);
            uiText->setPosition(Vec2(uiExchange->getContentSize().width / 2, uiExchange->getContentSize().height / 2));
            uiText->setTextAreaSize(Size(uiExchange->getContentSize().width - 10, uiText->getContentSize().height));
            uiText->setTextOverflow(Label::Overflow::SHRINK);
            uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
            uiText->setTextHorizontalAlignment(TextHAlignment::CENTER);
            uiExchange->addChild(uiText);
        }
    }
    
}

#pragma mark - ui : draw
void PopupEventDrawLimit::drawDefault()
{
    auto layout = _uiContentsDefault;
    
    if ( auto uiTime = layout->getChildByName<ui::CText*>("UI_TIME"); uiTime != nullptr )
    {
        auto action1 = CallFuncN::create([=](Ref* sender){
            
            int64_t timeDiff = EventDrawLimitManager::getInstance()->getEndTime() - UtilsDate::getInstance()->getTime();
            timeDiff = MAX(timeDiff, 0);
            
            int nDay = (int)(timeDiff / 86400);
            int nHour = timeDiff % 86400 / 3600;
            int nMin = timeDiff % 3600 / 60;
            
            if ( EventDrawLimitManager::getInstance()->isExtraTime() == false )
            {// t_ui_token_text_eventtime -> 이벤트 종료 : %d일 %d시간 %d분
                ((ui::CText*)sender)->setString(GAME_TEXTFORMAT("t_ui_token_text_eventtime", nDay, nHour, nMin));
            }
            else
            {// t_ui_token_text_endtime -> [이벤트 종료] 토큰 교환 기간: %d일 %d시간 %d분
                ((ui::CText*)sender)->setString(GAME_TEXTFORMAT("t_ui_token_text_endtime", nDay, nHour, nMin));
            }
            
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
}

void PopupEventDrawLimit::drawBestItems()
{
    auto layout = _uiContentsBestItems;
    
    if ( auto uiTime = layout->getChildByName<ui::CText*>("UI_TIME"); uiTime != nullptr )
    {
        auto action1 = CallFuncN::create([=](Ref* sender){
            
            int64_t timeDiff = EventDrawLimitManager::getInstance()->getResetTime() - UtilsDate::getInstance()->getTime();
            timeDiff = MAX(timeDiff, 0);
            
            int nHour = (int)(timeDiff / 3600);
            int nMin = timeDiff % 3600 / 60;
            
            auto text = GAME_TEXT("t_ui_token_text_reset");
            text.append(" : ");
            text.append(GAME_TEXTFORMAT("t_ui_token_text_resettime", nHour, nMin));
            
            ((ui::CText*)sender)->setString(text);
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
    Vec2 pos = _uiList->getContainerOffset();
    _uiList->reloadData();
    _uiList->setContainerOffset(pos);
}

void PopupEventDrawLimit::drawInfo()
{
    auto layout = _uiContentsInfo;
    
    auto uiBonus = utils::findChild<ui::ImageView*>(layout, "UI_BONUS");
    if ( uiBonus != nullptr )
    {
        bool bFinish = EventDrawLimitManager::getInstance()->isBonusFinish();
        
        auto idx = EventDrawLimitManager::getInstance()->getBonusIdx();
        int countNow = EventDrawLimitManager::getInstance()->getBonusCountNow();
        int countMax = EventDrawLimitManager::getInstance()->getBonusCountMax();
        
        double percent = (double)countNow / countMax * 100;
        percent = MIN(percent, 100);
        
        //
        if ( auto uiBar = utils::findChild<ui::LoadingBar*>(uiBonus, "UI_BAR"); uiBar != nullptr )
        {
            if ( bFinish == true )
                uiBar->setPercent(100);
            else
                uiBar->setPercent(percent);
        }
        
        //
        if ( auto uiBarCount = utils::findChild<ui::CText*>(uiBonus, "UI_BAR_COUNT"); uiBarCount != nullptr )
        {
            if ( bFinish == true )
            {
                uiBarCount->setString("MAX");
            }
            else
            {
                uiBarCount->setString(MafUtils::format("LV%d (%d/%d)", idx, countNow, countMax));
            }
        }
        
        //
        if ( auto uiReward = utils::findChild<ui::Button*>(uiBonus, "UI_REWARD"); uiReward != nullptr )
        {
            if ( bFinish == true )
            {
                uiReward->setVisible(false);
            }
            else
            {
                uiReward->setVisible(true);
                uiReward->removeChildByName("UI_COST");
                
                if ( percent >= 100 )
                {
                    uiReward->loadTextureNormal("Assets/ui_common/btn_2_1_on.png");
                }
                else
                {
                    uiReward->loadTextureNormal("Assets/ui_common/btn_2_1_off.png");
                }
                
                auto objBonus = EventDrawLimitManager::getInstance()->getInfoBonus(idx);
                if ( objBonus->getReward() != nullptr )
                {
                    auto costText =  MafUtils::format("x %s", objBonus->getReward()->getCount().c_str());
                    
                    auto uiCost = UtilsWidget::getInstance()->createIconText(objBonus->getReward()->getPath(), costText, uiReward->getContentSize().width - 10, 24, 15);
                    uiCost->setPosition(Vec2(uiReward->getContentSize().width / 2, uiReward->getContentSize().height / 2));
                    uiCost->setName("UI_COST");
                    uiReward->addChild(uiCost);
                }
            }
        }
    }
    
    auto uiMission = utils::findChild<ui::ImageView*>(layout, "UI_MISSION");
    if ( uiMission != nullptr )
    {
        int countNow = EventDrawLimitManager::getInstance()->getTokenPieceNow();
        int countMax = EventDrawLimitManager::getInstance()->getTokenPieceMax();
        
        double percent = (double)countNow / countMax * 100;
        percent = MIN(percent, 100);
        
        //
        if ( auto uiBar = utils::findChild<ui::LoadingBar*>(uiMission, "UI_BAR"); uiBar != nullptr )
        {
            uiBar->setPercent(percent);
        }
        
        //
        if ( auto uiBarCount = utils::findChild<ui::CText*>(uiMission, "UI_BAR_COUNT"); uiBarCount != nullptr )
        {
            uiBarCount->setString(MafUtils::format("%d/%d", countNow, countMax));
        }
        
        //
        if ( auto uiReward = utils::findChild<ui::Button*>(uiMission, "UI_REWARD"); uiReward != nullptr )
        {
            if ( percent >= 100 )
            {
                uiReward->loadTextureNormal("Assets/ui_common/btn_2_1_on.png");
            }
            else
            {
                uiReward->loadTextureNormal("Assets/ui_common/btn_2_1_off.png");
            }
            
            //
            uiReward->setVisible(true);
            if ( EventDrawLimitManager::getInstance()->isExtraTime() == true )
            {
                uiReward->setVisible(false);
            }
            else
            {
                int64_t timeDiff = EventDrawLimitManager::getInstance()->getEndTime() - UtilsDate::getInstance()->getTime();
                timeDiff = MAX(timeDiff, 0);
                if ( timeDiff <= 0 )
                {
                    uiReward->setVisible(false);
                }
            }
        }
    }
}

void PopupEventDrawLimit::drawButtons()
{
    auto layout = _uiContentsButtons;
    
    auto uiDefault = layout->getChildByName<ui::Layout*>("UI_DEFAULT");
    auto uiExtraTime = layout->getChildByName<ui::Layout*>("UI_EXTRA_TIME");
    
    bool bExtraTime = EventDrawLimitManager::getInstance()->isExtraTime();
    if ( bExtraTime == true )
    {
        uiDefault->setVisible(false);
        uiExtraTime->setVisible(true);
    }
    else
    {
        uiDefault->setVisible(true);
        uiExtraTime->setVisible(false);
        
        auto size = UtilsWidget::getInstance()->getLayoutAccumulatedSize(uiDefault);
        uiDefault->setContentSize(size);
        uiDefault->requestDoLayout();
        
        //
        if ( auto uiDrawOnce = utils::findChild<ui::Button*>(uiDefault, "UI_DRAW_ONCE"); uiDrawOnce != nullptr )
        {
            uiDrawOnce->removeChildByName("UI_COST");
            
            auto costText =  GAME_TEXTFORMAT("t_ui_token_btn_buy_single_2", EventDrawLimitManager::getInstance()->getTokenNow());
            
            auto uiCost = UtilsWidget::getInstance()->createIconText("Assets/ui/event/icon_token.png", costText, uiDrawOnce->getContentSize().width - 10, 24, 15);
            uiCost->setPosition(Vec2(uiDrawOnce->getContentSize().width / 2, 3 + 22.5));
            uiCost->setName("UI_COST");
            uiDrawOnce->addChild(uiCost);
            
            if ( auto uiText = utils::findChild<ui::CText*>(uiCost, "WIDGET_TEXT"); uiText != nullptr )
            {
                uiText->setTextColor(Color4B(199, 106, 0, 255));
            }
        }
        
        if ( auto uiDrawMany = utils::findChild<ui::Button*>(uiDefault, "UI_DRAW_MANY"); uiDrawMany != nullptr )
        {
            uiDrawMany->removeChildByName("UI_COST");
            
            auto costText =  GAME_TEXTFORMAT("t_ui_token_btn_buy_multiple_2", EventDrawLimitManager::getInstance()->getTokenNow());
            
            auto uiCost = UtilsWidget::getInstance()->createIconText("Assets/ui/event/icon_token.png", costText, uiDrawMany->getContentSize().width - 10, 24, 15);
            uiCost->setPosition(Vec2(uiDrawMany->getContentSize().width / 2, 3 + 22.5));
            uiCost->setName("UI_COST");
            uiDrawMany->addChild(uiCost);
            
            if ( auto uiText = utils::findChild<ui::CText*>(uiCost, "WIDGET_TEXT"); uiText != nullptr )
            {
                uiText->setTextColor(Color4B(199, 106, 0, 255));
            }
        }
    }
    
    
}

#pragma mark - click
void PopupEventDrawLimit::onClickClose(cocos2d::Ref* sender)
{
    if ( _spriteContainer->getChildByName("UI_DRAW_ANI") != nullptr )
    {
        return;
    }
    
    if ( _spriteContainer->getChildByName("UI_DRAW_RESULT") != nullptr )
    {
        return;
    }
    
    hide();
}

void PopupEventDrawLimit::onClickHelp(cocos2d::Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto popup = PopupEventDrawLimitInfo::create();
    popup->show();
}

void PopupEventDrawLimit::onClickDraw(cocos2d::Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto uiBtn = dynamic_cast<ui::Button*>(sender);
    
    int count = uiBtn->getTag();
    
    int result = EventDrawLimitManager::getInstance()->onDraw(count, CC_CALLBACK_2(PopupEventDrawLimit::callbackNetworkDraw, this));
    if ( result != 0 )
    {
        if ( result == 1 )
        {
            // network
            EventDrawLimitManager::getInstance()->requestInfo(true, CC_CALLBACK_2(PopupEventDrawLimit::callbackNetworkInfo, this));
        }
        else if ( result == 2 )
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_31"));
            popup->addButton(GAME_TEXT("t_ui_cancel"), true, nullptr);
            popup->addButton(GAME_TEXT("t_ui_purchase_bonus_msg_4"), false, [=](Ref* sender){
                auto popup = PopupCash::create();
                popup->setTab(PopupCash::E_TAB::BONUS);
                popup->show();
                
                hide();
            });
            popup->show();
        }
        
        return;
    }
    
    //
    auto size = _spriteContainer->getContentSize();
    size.height = size.height - 75;
    
    auto layoutDrawAni = LayerEventDrawLimitAni::create(size);
    layoutDrawAni->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    layoutDrawAni->setPosition(Vec2(size.width / 2, 0));
    layoutDrawAni->setCallbackFinish(CC_CALLBACK_0(PopupEventDrawLimit::callbackDrawAniFinish, this));
    layoutDrawAni->setName("UI_DRAW_ANI");
    _spriteContainer->addChild(layoutDrawAni);
}

void PopupEventDrawLimit::onClickBonus(cocos2d::Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    int result = EventDrawLimitManager::getInstance()->onDrawBonus(CC_CALLBACK_2(PopupEventDrawLimit::callbackNetworkDrawBonus, this));
    if ( result != 0 )
    {
        return;
    }
}

void PopupEventDrawLimit::onClickTokenPieceExchange(cocos2d::Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    int result = EventDrawLimitManager::getInstance()->onTokenPieceExchange(CC_CALLBACK_2(PopupEventDrawLimit::callbackNetworkTokenPieceExchange, this));
    if ( result != 0 )
    {
        if ( result == 1 )
        {
            // network
            EventDrawLimitManager::getInstance()->requestInfo(true, CC_CALLBACK_2(PopupEventDrawLimit::callbackNetworkInfo, this));
        }
        return;
    }
}

void PopupEventDrawLimit::onClickTokenExchange(cocos2d::Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    int result = EventDrawLimitManager::getInstance()->onTokenExchange(CC_CALLBACK_2(PopupEventDrawLimit::callbackNetworkTokenExchange, this));
    if ( result != 0 )
    {
        return;
    }
}

#pragma mark - callback
void PopupEventDrawLimit::callbackNetworkInfo(bool bResult, int nResult)
{
    if ( bResult == false )
    {
        std::string strMessageKey = "t_ui_error_30";
        if ( nResult == 2 )
        {
            strMessageKey = "t_ui_draw_msg_1";
        }

        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT(strMessageKey));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, [=](Ref* sender){
            hide();
        });
        popup->show();
        return;
    }
    
    _listInfoDraw.clear();
    _listInfoDraw = EventDrawLimitManager::getInstance()->getListInfoItemsLimit();
    
    //
    drawDefault();
    drawBestItems();
    drawInfo();
    drawButtons();
}

void PopupEventDrawLimit::callbackNetworkDraw(bool bResult, int nResult)
{
    if ( bResult == false )
    {//'_result' : 1 : 성공, 2 : 이벤트 기간아님, 3 : 이미 받은 보상, 10002 : 보유중인 조각갯수가 부족할때
        std::string strMessageKey = "t_ui_error_30";
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT(strMessageKey));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        
        _spriteContainer->removeChildByName("UI_DRAW_ANI");
        
        if ( nResult == 2 )
        {
            //network
            EventDrawLimitManager::getInstance()->requestInfo(true, CC_CALLBACK_2(PopupEventDrawLimit::callbackNetworkInfo, this));
        }
        return;
    }
    
    //
    _listInfoDraw.clear();
    _listInfoDraw = EventDrawLimitManager::getInstance()->getListInfoItemsLimit();
    
    //
    auto layoutDrawAni = _spriteContainer->getChildByName<LayerEventDrawLimitAni*>("UI_DRAW_ANI");
    if ( layoutDrawAni != nullptr )
    {
        layoutDrawAni->onDrawStart();
    }
    
    //
    drawDefault();
    drawBestItems();
    drawInfo();
    drawButtons();
}

void PopupEventDrawLimit::callbackNetworkDrawBonus(bool bResult, int nResult)
{
    if ( bResult == false )
    {//'_result' : 1 : 성공, 0 : 횟수 부족, 2 : 이벤트 종료, 3 : 이미 받은 아이템
        std::string strMessageKey = "t_ui_error_30";
        if ( nResult == 2 )
        {
            strMessageKey = "t_ui_draw_msg_1";
        }
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT(strMessageKey));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, [=](Ref* sender){
            hide();
        });
        popup->show();
        return;
    }
    
    //
    drawInfo();
}
 
void PopupEventDrawLimit::callbackNetworkTokenPieceExchange(bool bResult, int nResult)
{
    if ( bResult == false )
    {//'_result' : 1 : 성공, 0 : 횟수 부족, 2 : 이벤트 종료, 3 : 이미 받은 아이템
        std::string strMessageKey = "t_ui_error_30";
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT(strMessageKey));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        
        if ( nResult == 2 )
        {
            //network
            EventDrawLimitManager::getInstance()->requestInfo(true, CC_CALLBACK_2(PopupEventDrawLimit::callbackNetworkInfo, this));
        }
        return;
    }
    
    //
    drawInfo();
    drawButtons();
}

void PopupEventDrawLimit::callbackNetworkTokenExchange(bool bResult, int nResult)
{
    if ( bResult == false )
    {//'_result' : 1 : 성공, 0 : 횟수 부족, 2 : 이벤트 종료, 3 : 이미 받은 아이템
        std::string strMessageKey = "t_ui_error_30";
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT(strMessageKey));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, [=](Ref* sender){
            hide();
        });
        popup->show();
        return;
    }
    
    //
    drawInfo();
    drawButtons();
}

void PopupEventDrawLimit::callbackDrawAniFinish()
{
    //
    auto size = _spriteContainer->getContentSize();
    size.height = size.height - 75;
    
    auto layoutDrawResult = LayerEventDrawLimitResult::create(size);
    layoutDrawResult->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    layoutDrawResult->setPosition(Vec2(size.width / 2, 0));
    layoutDrawResult->setCallbackAgain(CC_CALLBACK_1(PopupEventDrawLimit::callbackDrawAniAgain, this));
    layoutDrawResult->setName("UI_DRAW_RESULT");
    _spriteContainer->addChild(layoutDrawResult);
}

void PopupEventDrawLimit::callbackDrawAniAgain(int count)
{
    int result = EventDrawLimitManager::getInstance()->onDraw(count, CC_CALLBACK_2(PopupEventDrawLimit::callbackNetworkDraw, this));
    if ( result != 0 )
    {
        if ( result == 1 )
        {
            // network
            EventDrawLimitManager::getInstance()->requestInfo(true, CC_CALLBACK_2(PopupEventDrawLimit::callbackNetworkInfo, this));
        }
        else if ( result == 2 )
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_31"));
            popup->addButton(GAME_TEXT("t_ui_cancel"), true, nullptr);
            popup->addButton(GAME_TEXT("t_ui_purchase_bonus_msg_4"), false, [=](Ref* sender){
                auto popup = PopupCash::create();
                popup->setTab(PopupCash::E_TAB::BONUS);
                popup->show();
                
                hide();
            });
            popup->show();
        }
        return;
    }
    
    //
    auto size = _spriteContainer->getContentSize();
    size.height = size.height - 75;
    
    auto layoutDrawAni = LayerEventDrawLimitAni::create(size);
    layoutDrawAni->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    layoutDrawAni->setPosition(Vec2(size.width / 2, 0));
    layoutDrawAni->setCallbackFinish(CC_CALLBACK_0(PopupEventDrawLimit::callbackDrawAniFinish, this));
    layoutDrawAni->setName("UI_DRAW_ANI");
    _spriteContainer->addChild(layoutDrawAni);
}
