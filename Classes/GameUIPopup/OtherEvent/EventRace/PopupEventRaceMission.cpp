//
//  lkjkl.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 17/02/2020.
//

#include "PopupEventRaceMission.h"

#include "Common/ConfigGameNormal.h"

#include "GameObject/InfoQuest.h"

#include "GameUI/UI/UIReddot.h"
#include "GameUI/UI/UICurrency.h"

#include "GameUIPopup/OtherEvent/EventRace/PopupEventRaceTitle.h"

#include "ManagerEvent/EventRaceManager.h"

USING_NS_CC;

PopupEventRaceMission* PopupEventRaceMission::create()
{
    PopupEventRaceMission* pRet = new(std::nothrow) PopupEventRaceMission();
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

PopupEventRaceMission::PopupEventRaceMission(void) :
_bInitUI(false)
{
    
}

PopupEventRaceMission::~PopupEventRaceMission(void)
{
    
}

bool PopupEventRaceMission::init()
{
    if( !PopupBase::init() )
    {
        return false;
    }
    
    //
    setName("PopupEventRaceMission");
    
    //
    initVar();
    initUi();
    
    return true;
}

void PopupEventRaceMission::onEnter()
{
    PopupBase::onEnter();
    
    if ( _bInitUI == false )
    {
        _bInitUI = true;
        
        //
        uiContainer();
        uiInfo();
        uiList();
        uiButtons();
    }
    
    //
    drawInfo();
    drawList();
    drawButtons();
    
    //
    PopupLoading::show();
    EventRaceManager::getInstance()->requestMission(CC_CALLBACK_2(PopupEventRaceMission::callbackInfo, this));
}

#pragma mark - override DelegateListView
void PopupEventRaceMission::listviewScrollView(ui::CListView* listview, ui::ScrollView::EventType type)
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

Size PopupEventRaceMission::listviewCellSize(ui::CListView* listview, ssize_t idx)
{
    double width = listview->getContentSize().width;
    double height = 112;

    return Size(width, height);
}

void PopupEventRaceMission::listviewCell(ui::CListView *listview, ui::Widget* childview, ssize_t idx)
{
    // size
    Size size = childview->getContentSize();
    Size sizeLayout = Size(size.width, 102);

    // data
    auto objMission = _listInfoMission.at(idx);

    uint64_t countNow = objMission->getCountNow();
    uint64_t countMax = objMission->getCountMax();
    double countBigNow = objMission->getCountBigNow();
    double countBigMax = objMission->getCountBigMax();
    bool countShort = objMission->isCountShort();
    bool bComplete = false;
    
    if ( objMission->isReceive() == false )
    {
        if ( countShort == false && countNow >= countMax )
        {
            bComplete = true;
        }
        else if ( countShort == true && countBigNow >= countBigMax )
        {
            bComplete = true;
        }
    }
    
    float countPercent = (float)countNow / (float)countMax * 100;
    if ( countPercent > 100 )
        countPercent = 100;
    
    std::string nameText = objMission->getTitle();
    std::string countText = MafUtils::format("%lld/%lld", countNow, countMax);
    if ( countShort == true )
    {
        std::string countNowText = MafUtils::doubleToString(countBigNow);
        std::string countMaxText = MafUtils::doubleToString(countBigMax);
        MafUtils::convertNumberToShort(countNowText);
        MafUtils::convertNumberToShort(countMaxText);
        
        countText = MafUtils::format("%s/%s", countNowText.c_str(), countMaxText.c_str());
        
        countPercent = (float)countBigNow / (float)countBigMax * 100;
        if ( countPercent > 100 )
            countPercent = 100;
    }
    
    // ui
    childview->removeAllChildren();

    auto uiLayout = ui::Layout::create();
    uiLayout->setContentSize(sizeLayout);
    uiLayout->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    uiLayout->setPosition(Vec2(size.width / 2, size.height / 2));
    uiLayout->setTag((int)idx);
    childview->addChild(uiLayout);
    
    auto uiBG = ui::ImageView::create("Assets/ui_common/list_bg_s_2_1.png");
    uiBG->setScale9Enabled(true);
    uiBG->setCapInsets(Rect(25,50,1,1));
    uiBG->setContentSize(sizeLayout);
    uiBG->setPosition(Vec2(sizeLayout.width / 2, sizeLayout.height / 2));
    uiLayout->addChild(uiBG);
     
    // name
    auto uiName = ui::CText::create(nameText, GAME_FONT, 26);
    uiName->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    uiName->setPosition(Vec2(30, sizeLayout.height - 23));
    uiName->setTextAreaSize(Size(400, uiName->getContentSize().height));
    uiName->setTextVerticalAlignment(TextVAlignment::CENTER);
    uiName->setTextHorizontalAlignment(TextHAlignment::LEFT);
    uiName->setTextOverflow(Label::Overflow::SHRINK);
    uiLayout->addChild(uiName);
    
    // count
    auto uiCountBG = ui::ImageView::create("Assets/ui/adventure/adventure_mission_gauge_bg.png");
    uiCountBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    uiCountBG->setPosition(Vec2(50, 28));
    uiLayout->addChild(uiCountBG);
    {
        auto uiCountBar = ui::LoadingBar::create("Assets/ui/adventure/adventure_mission_gauge_orange.png");
        uiCountBar->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        uiCountBar->setPercent(countPercent);
        uiCountBG->addChild(uiCountBar);
        
        if(objMission->isReceive())
            uiCountBar->setColor(Color3B(90,90,90));
        
        auto uiCount = ui::CText::create(countText, GAME_FONT, 20);
        uiCount->setPosition(Vec2(uiCountBG->getContentSize().width / 2, uiCountBG->getContentSize().height / 2));
        uiCountBG->addChild(uiCount);
    }
    
    // items
    auto listReward = objMission->getListReward();
    
    auto uiItems = ui::Layout::create();
    uiItems->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    uiItems->setPosition(Vec2(sizeLayout.width - 20, sizeLayout.height / 2));
    uiItems->setLayoutType(ui::Layout::Type::HORIZONTAL);
    uiLayout->addChild(uiItems);
    
    for ( int i = 0; i < listReward.size(); i++ )
    {
        auto objItem = listReward.at(i);
        
        auto uiItemBG = ui::ImageView::create("Assets/ui_common/box_item.png");
        uiItemBG->setScale9Enabled(true);
        uiItemBG->setContentSize(Size(75,75));
        uiItems->addChild(uiItemBG);
        
        auto uiItem = ui::ImageView::create(objItem->getPath());
        uiItem->setPosition(Vec2(uiItemBG->getContentSize().width / 2, uiItemBG->getContentSize().height / 2 + 10));
        uiItem->setScale(1.2);
        uiItemBG->addChild(uiItem);
        
        auto uiItemCount = ui::CText::create(objItem->getCount(), GAME_FONT, 18);
        uiItemCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        uiItemCount->setPosition(Vec2(uiItemBG->getContentSize().width / 2, 0));
        uiItemBG->addChild(uiItemCount);
        
        if ( i != 0 )
        {
            auto params = ui::LinearLayoutParameter::create();
            params->setMargin(ui::Margin(10, 0, 0, 0));
            uiItemBG->setLayoutParameter(params);
        }
    }
    
    Size sizeItems = UtilsWidget::getInstance()->getLayoutAccumulatedSize(uiItems);
    uiItems->setContentSize(sizeItems);
    uiItems->requestDoLayout();
    
    //
    if ( objMission->isReceive() == true )
    {
        auto uiDisableBG = ui::ImageView::create(PIXEL);
        uiDisableBG->setScale9Enabled(true);
        uiDisableBG->setContentSize(sizeLayout);
        uiDisableBG->setPosition(Vec2(sizeLayout.width / 2, sizeLayout.height / 2));
        uiDisableBG->setColor(Color3B::BLACK);
        uiDisableBG->setOpacity(150);
        uiLayout->addChild(uiDisableBG);
    }
    else if ( bComplete == true )
    {
        auto uiEffect = ui::ImageView::create("Assets/ui_common/list_bg_s_2_0.png");
        uiEffect->setScale9Enabled(true);
        uiEffect->setCapInsets(Rect(25,50,1,1));
        uiEffect->setContentSize(sizeLayout);
        uiEffect->setPosition(Vec2(sizeLayout.width / 2, sizeLayout.height / 2));
        uiLayout->addChild(uiEffect);
    }
}

int64_t PopupEventRaceMission::listviewNumberOfCells(ui::CListView *listview)
{
    return _listInfoMission.size();
}


#pragma mark - init
void PopupEventRaceMission::initVar()
{
    _listInfoMission = EventRaceManager::getInstance()->getListMission();
}

void PopupEventRaceMission::initUi()
{
    Size size = Size(730, 960);
    
    // size
    _spriteContainer->setContentSize( size );
    
    // label title
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_event_race_mission_bt"), GAME_FONT, 32);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5);
    _spriteContainer->addChild(labelTitle);
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupEventRaceMission::onClickClose, this));
    itemClose->setPosition(_spriteContainer->getContentSize().width - itemClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5);
    itemClose->setTouchRect( itemClose->getContentSize() * 2 );
    menu->addChild(itemClose);
}

#pragma mark - ui
void PopupEventRaceMission::uiContainer()
{
    Size size = _spriteContainer->getContentSize();
    size.width = size.width - 28 - 28;
    size.height = size.height - 75 - 28;
    
    auto layout = ui::Layout::create();
    layout->setContentSize(Size(size.width, size.height));
    layout->setIgnoreAnchorPointForPosition(false);
    layout->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    layout->setPosition(Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75));
    _spriteContainer->addChild(layout);
    
    _layerContainerButtons = ui::Layout::create();
    _layerContainerButtons->setContentSize( Size(size.width, 88) );
    _layerContainerButtons->setIgnoreAnchorPointForPosition(false);
    _layerContainerButtons->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _layerContainerButtons->setPosition( Vec2(size.width / 2, 0) );
    _layerContainerButtons->setLayoutType(ui::Layout::Type::HORIZONTAL);
    layout->addChild(_layerContainerButtons);
    
    _layerContainerInfo = ui::Layout::create();
    _layerContainerInfo->setContentSize( Size(size.width, 160) );
    _layerContainerInfo->setIgnoreAnchorPointForPosition(false);
    _layerContainerInfo->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _layerContainerInfo->setPosition( Vec2(size.width / 2, size.height) );
    layout->addChild(_layerContainerInfo);
    
    _layerContainerList = ui::Layout::create();
    _layerContainerList->setContentSize( Size(size.width, size.height - _layerContainerButtons->getContentSize().height - _layerContainerInfo->getContentSize().height) );
    _layerContainerList->setIgnoreAnchorPointForPosition(false);
    _layerContainerList->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _layerContainerList->setPosition( Vec2(size.width / 2, size.height - _layerContainerInfo->getContentSize().height) );
    layout->addChild(_layerContainerList);
}

void PopupEventRaceMission::uiInfo()
{
    auto layout = _layerContainerInfo;
    Size size = layout->getContentSize();
    
    
    auto uiBG = ui::ImageView::create("Assets/ui/race_event/race_mission_top_img.png");
    uiBG->setPositionType(ui::Widget::PositionType::PERCENT);
    uiBG->setPositionPercent(Vec2::ANCHOR_MIDDLE);
    layout->addChild(uiBG);
    {
        // desc
        auto uiDesc = ui::CText::create(GAME_TEXT("t_ui_event_race_mission_msg"), GAME_FONT, 30);
        uiDesc->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        uiDesc->setPosition(Vec2(15, 46));
        uiDesc->setTextAreaSize(Size(470, 64));
        uiDesc->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiDesc->setTextHorizontalAlignment(TextHAlignment::LEFT);
        uiDesc->setTextOverflow(Label::Overflow::SHRINK);
        uiBG->addChild(uiDesc);
    }
}

void PopupEventRaceMission::uiList()
{
    auto layout = _layerContainerList;
    Size size = layout->getContentSize();
    
    _uiList = ui::CListView::create(this, Size(size.width, size.height), ui::ScrollView::Direction::VERTICAL);
    _uiList->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _uiList->setPosition(Vec2(size.width / 2, 0));
    _uiList->setScrollBarPositionFromCorner(Vec2(7, 7));
    _uiList->reloadData();
    layout->addChild(_uiList);
}

void PopupEventRaceMission::uiButtons()
{
    auto layout = _layerContainerButtons;
    Size size = layout->getContentSize();

    
    _uiBtnReward = ui::Button::create("Assets/ui_common/btn_1_9_on.png");
    _uiBtnReward->setScale9Enabled(true);
    _uiBtnReward->setContentSize(Size(size.width, size.height));
    _uiBtnReward->setCapInsets(Rect(33,33,2,2));
    _uiBtnReward->addClickEventListener(CC_CALLBACK_1(PopupEventRaceMission::onClickRewardAll, this));
    layout->addChild(_uiBtnReward);
    {
        auto uiText = ui::CText::create(GAME_TEXT("t_ui_attend_receive_all"), GAME_FONT, 28);
        uiText->setPositionType(ui::Widget::PositionType::PERCENT);
        uiText->setPositionPercent(Vec2::ANCHOR_MIDDLE);
        _uiBtnReward->addChild(uiText);
    }
}

#pragma mark - ui
void PopupEventRaceMission::drawInfo()
{
    auto layout = _layerContainerInfo;
    Size size = layout->getContentSize();
    
}

void PopupEventRaceMission::drawList()
{
    Vec2 pos = _uiList->getContainerOffset();
    _uiList->reloadData();
    _uiList->setContainerOffset(pos);
}

void PopupEventRaceMission::drawButtons()
{
    auto layout = _layerContainerButtons;
    
    std::string pathReward = "Assets/ui_common/btn_1_off.png";
    if ( EventRaceManager::getInstance()->isCompleteMission() == true )
    {
        pathReward = "Assets/ui_common/btn_1_9_on.png";
    }
    _uiBtnReward->loadTextureNormal(pathReward);
}

#pragma mark - set, get, other

#pragma mark - callback
void PopupEventRaceMission::callbackInfo(bool bResult, int nResult)
{
    PopupLoading::hide();
    
    if ( bResult == false )
    {
        hide();
        return;
    }
    
    //
    _listInfoMission = EventRaceManager::getInstance()->getListMission();
    
    //
    drawList();
    drawInfo();
    drawButtons();
}

void PopupEventRaceMission::callbackReward(bool bResult, int nResult)
{
    PopupLoading::hide();
    
    if ( bResult == false )
    {
        std::string strMessage = GAME_TEXT("t_ui_error_30");
        if ( nResult == 30001 )
        {
            strMessage = GAME_TEXT("t_ui_bong_town_reward_error");
        }
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), strMessage);
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
                
        hide();
        return;
    }
    
    //
    _listInfoMission = EventRaceManager::getInstance()->getListMission();
    
    //
    drawList();
    drawInfo();
    drawButtons();
}

#pragma mark - click
void PopupEventRaceMission::onClickClose(Ref* sender)
{
    hide();
}

void PopupEventRaceMission::onClickRewardAll(Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto uiBtn = dynamic_cast<ui::Button*>(sender);
    
    
    if ( EventRaceManager::getInstance()->isCompleteMission() == false )
    {
        return;
    }
        
    PopupLoading::show();
    EventRaceManager::getInstance()->requestMissionReward(CC_CALLBACK_2(PopupEventRaceMission::callbackReward, this));
}


