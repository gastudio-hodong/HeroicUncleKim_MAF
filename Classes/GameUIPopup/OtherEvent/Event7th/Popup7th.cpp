//
//  Popup7th.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/12/03.
//

#include "Popup7th.h"

#include "Common/ConfigGameNormal.h"

#include "GameObject/InfoChat.h"

#include "ManagerEvent/EventManager.h"
#include "ManagerEvent/Event7thManager.h"
#include "ManagerGame/ItemsManager.h"

USING_NS_CC;

Popup7th* Popup7th::create()
{
    Popup7th* pRet = new(std::nothrow) Popup7th();
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

Popup7th::Popup7th():

_uiContainerContents(nullptr),
_uiContainerMissionCoop(nullptr),
_uiContainerMissionPersonal(nullptr),

_uiListMissionCoop(nullptr),
_uiListMissionPersonalReward(nullptr),

_uiBarPersonal(nullptr),
_uiBtnPersonal(nullptr),
_uiTextPersonalName(nullptr),
_uiTextPersonalPercent(nullptr)
{
    
}

Popup7th::~Popup7th()
{
    
}

bool Popup7th::init()
{
    if ( !PopupBase::init() )
    {
        return false;
    }
    
    //
    initVar();
    initUI();
    
    return true;
}


void Popup7th::onEnter()
{
    PopupBase::onEnter();
    
    PopupLoading::show();
    Event7thManager::getInstance()->requestInfo(CC_CALLBACK_2(Popup7th::callbackInfo, this));
}

#pragma mark - init
void Popup7th::initVar()
{
    
}

void Popup7th::initUI()
{
    // size
    Size size = Size(750, 1334);

    _spriteContainer->setContentSize(size);
    
    // label title
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_coevt_ready_title"), GAME_FONT, 32);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5);
    _spriteContainer->addChild(labelTitle);
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(Popup7th::onClickClose, this));
    itemClose->setPosition(_spriteContainer->getContentSize().width - itemClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5);
    itemClose->setTouchRect( itemClose->getContentSize() * 2 );
    menu->addChild(itemClose);
    
    //
    _uiContainerContents = ui::Layout::create();
    _uiContainerContents->setContentSize( Size(size.width - 25, 108) );
    _uiContainerContents->setIgnoreAnchorPointForPosition(false);
    _uiContainerContents->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _uiContainerContents->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75) );
    _spriteContainer->addChild(_uiContainerContents);
    
    _uiContainerMissionPersonal = ui::Layout::create();
    _uiContainerMissionPersonal->setContentSize( Size(size.width - 25, 565) );
    _uiContainerMissionPersonal->setIgnoreAnchorPointForPosition(false);
    _uiContainerMissionPersonal->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _uiContainerMissionPersonal->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, 25) );
    _spriteContainer->addChild(_uiContainerMissionPersonal);
    
    _uiContainerMissionCoop = ui::Layout::create();
    _uiContainerMissionCoop->setContentSize( Size(size.width - 25, size.height - 75 -  _uiContainerContents->getContentSize().height - _uiContainerMissionPersonal->getContentSize().height - 25) );
    _uiContainerMissionCoop->setIgnoreAnchorPointForPosition(false);
    _uiContainerMissionCoop->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _uiContainerMissionCoop->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75 - _uiContainerContents->getContentSize().height) );
    _spriteContainer->addChild(_uiContainerMissionCoop);
    
    //
    uiContent();
    uiMissionCoop();
    uiMissionPersonal();
}

#pragma mark - override DelegateListView
void Popup7th::listviewScrollView(ui::CListView* listview, ui::ScrollView::EventType type)
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

Size Popup7th::listviewCellSize(ui::CListView* listview, ssize_t idx)
{
    double width = 0;
    double height = 0;

    if ( listview->getName().compare("Mission_Coop") == 0 )
    {
        width = listview->getContentSize().width;
        height = 78;
    }
    else if ( listview->getName().compare("Mission_PersonalReward") == 0 )
    {
        width = 100;
        if ( idx == 0 )
        {
            width = 72;
        }
        height = listview->getContentSize().height;
    }
    
    return Size(width, height);
}

void Popup7th::listviewCell(ui::CListView *listview, ui::Widget* childview, ssize_t idx)
{
    if ( listview->getName().compare("Mission_Coop") == 0 )
    {
        drawCellMissionCoop(childview, idx);
    }
    else if ( listview->getName().compare("Mission_PersonalReward") == 0 )
    {
        drawCellMissionPersonalReward(childview, idx);
    }
}

int64_t Popup7th::listviewNumberOfCells(ui::CListView *listview)
{
    int64_t count = 0;
    if ( listview->getName().compare("Mission_Coop") == 0 )
    {
        count = _listCoopInfo.size();
    }
    else if ( listview->getName().compare("Mission_PersonalReward") == 0 )
    {
        count = _listPersonalInfo.size();
    }
    
    return count;
}
 

#pragma mark - ui
void Popup7th::uiContent()
{
    _uiContainerContents->removeAllChildren();
    
    //
    auto layout = _uiContainerContents;
    
    auto uiImgTitle = ui::ImageView::create("Assets/ui/event/white_day_event_title_box_1_1.png");
    uiImgTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    uiImgTitle->setPosition(Vec2(layout->getContentSize().width / 2, layout->getContentSize().height));
    layout->addChild(uiImgTitle);
    
    auto uiTextTime = ui::CText::create(GAME_TEXTFORMAT("t_ui_raid_event_time_remain", 0,0,0), GAME_FONT, 24);
    uiTextTime->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    uiTextTime->setPosition(Vec2(layout->getContentSize().width / 2, layout->getContentSize().height - 2));
    uiTextTime->setTextColor(Color4B(255, 255, 0, 255));
    uiTextTime->enableOutline(Color4B::BLACK, 2);
    layout->addChild(uiTextTime);
    {
        auto action1 = CallFuncN::create([=](Node* sender){
            
            auto objEvent = EventManager::getInstance()->getActivatedEvent(30);
            if ( objEvent == nullptr )
            {
                return;
            }
            
            auto timeDiff = objEvent->getEventEnd() - UtilsDate::getInstance()->getTime();
            if ( timeDiff <= 0 )
                timeDiff = 0;
            
            int32_t d = int32_t(timeDiff / 86400);
            int32_t h = timeDiff % 86400 / 3600;
            int32_t m = timeDiff % 3600 / 60;
            int32_t s = timeDiff % 3600 % 60;
            
            auto uiTextTime = (ui::CText*)sender;
            uiTextTime->setString(GAME_TEXTFORMAT("t_ui_token_text_eventtime", d, h, m));
        });
        auto action2 = DelayTime::create(1.0f);
        auto seq = Sequence::create(action1, action2, NULL);
        uiTextTime->runAction(RepeatForever::create(seq));
    }
     
}

void Popup7th::uiMissionCoop()
{
    _uiContainerMissionCoop->removeAllChildren();
    
    //
    auto layout = _uiContainerMissionCoop;
    
    _uiListMissionCoop = ui::CListView::create(this, Size(layout->getContentSize().width, layout->getContentSize().height), ui::ScrollView::Direction::VERTICAL);
    _uiListMissionCoop->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _uiListMissionCoop->setPosition(Vec2(layout->getContentSize().width / 2, 0));
    _uiListMissionCoop->setBounceEnabled(true);
    _uiListMissionCoop->setScrollBarPositionFromCorner(Vec2(7, 7));
    _uiListMissionCoop->setName("Mission_Coop");
    layout->addChild(_uiListMissionCoop);
}

void Popup7th::uiMissionPersonal()
{
    _uiContainerMissionPersonal->removeAllChildren();
    
    //
    auto layout = _uiContainerMissionPersonal;
    
    auto uiImgTitle = ui::ImageView::create("Assets/ui/event/white_day_event_title_box_1_2.png");
    uiImgTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    uiImgTitle->setPosition(Vec2(layout->getContentSize().width / 2, layout->getContentSize().height));
    layout->addChild(uiImgTitle);
    
    // info
    auto uiImgInfoBG = ui::ImageView::create("Assets/ui_common/box_02.png");
    uiImgInfoBG->setContentSize(Size(layout->getContentSize().width - 10, 164));
    uiImgInfoBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    uiImgInfoBG->setPosition(Vec2(layout->getContentSize().width / 2, 0));
    uiImgInfoBG->setScale9Enabled(true);
    uiImgInfoBG->setCapInsets(Rect(10,10,10,10));
    layout->addChild(uiImgInfoBG);
    {
        auto uiTextInfo = ui::CText::create(GAME_TEXT("t_ui_evt_detail"), GAME_FONT, 20);
        uiTextInfo->setPosition(Vec2(uiImgInfoBG->getContentSize().width / 2, uiImgInfoBG->getContentSize().height / 2));
        uiTextInfo->setTextAreaSize(Size(uiImgInfoBG->getContentSize().width - 10, uiImgInfoBG->getContentSize().height - 10));
        uiTextInfo->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiTextInfo->setTextHorizontalAlignment(TextHAlignment::CENTER);
        ((Label*)uiTextInfo->getVirtualRenderer())->setOverflow(Label::Overflow::SHRINK);
        uiImgInfoBG->addChild(uiTextInfo);
    }
    
    // mission
    auto uiLayoutMission = ui::Layout::create();
    uiLayoutMission->setContentSize(Size(layout->getContentSize().width - 10, 74 + 20));
    uiLayoutMission->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    uiLayoutMission->setPosition(Vec2(layout->getContentSize().width / 2, uiImgTitle->getPositionY() - uiImgTitle->getContentSize().height));
    layout->addChild(uiLayoutMission);
    {
        // progress
        auto uiImgBG = ui::ImageView::create("Assets/ui/event/list_progressbar_1_1.png");
        uiImgBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        uiImgBG->setPosition(Vec2(0, uiLayoutMission->getContentSize().height / 2));
        uiLayoutMission->addChild(uiImgBG);
        {
            _uiTextPersonalName = ui::CText::create(" ", GAME_FONT, 24);
            _uiTextPersonalName->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
            _uiTextPersonalName->setPosition(Vec2(5, uiImgBG->getContentSize().height));
            _uiTextPersonalName->setTextAreaSize(Size(uiImgBG->getContentSize().width - 120, _uiTextPersonalName->getContentSize().height));
            _uiTextPersonalName->setTextVerticalAlignment(TextVAlignment::CENTER);
            _uiTextPersonalName->setTextHorizontalAlignment(TextHAlignment::LEFT);
            ((Label*)_uiTextPersonalName->getVirtualRenderer())->setOverflow(Label::Overflow::SHRINK);
            uiImgBG->addChild(_uiTextPersonalName);
            
            auto uiTextDesc = ui::CText::create(GAME_TEXT("t_ui_coevt_person_m_d"), GAME_FONT, 20);
            uiTextDesc->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
            uiTextDesc->setPosition(Vec2(5, _uiTextPersonalName->getPositionY() - _uiTextPersonalName->getContentSize().height));
            uiTextDesc->setTextAreaSize(Size(uiImgBG->getContentSize().width - 120, uiTextDesc->getContentSize().height));
            uiTextDesc->setTextVerticalAlignment(TextVAlignment::CENTER);
            uiTextDesc->setTextHorizontalAlignment(TextHAlignment::LEFT);
            ((Label*)uiTextDesc->getVirtualRenderer())->setOverflow(Label::Overflow::SHRINK);
            uiImgBG->addChild(uiTextDesc);

            _uiTextPersonalPercent = ui::CText::create(GAME_TEXTFORMAT("t_ui_prison_jelly_item_option_2", 0), GAME_FONT, 20);
            _uiTextPersonalPercent->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
            _uiTextPersonalPercent->setPosition(Vec2(uiImgBG->getContentSize().width - 10, 0));
            _uiTextPersonalPercent->setTextColor(Color4B(255, 68, 53, 255));
            _uiTextPersonalPercent->setTextHorizontalAlignment(TextHAlignment::RIGHT);
            uiImgBG->addChild(_uiTextPersonalPercent);
            
            // progress
            _uiBarPersonal = ui::LoadingBar::create("Assets/ui/event/list_progressbar_1_2.png");
            _uiBarPersonal->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
            _uiBarPersonal->setPosition(Vec2(0, 0));
            _uiBarPersonal->setPercent(0);
            uiImgBG->addChild(_uiBarPersonal);
        }
        
        // btn
        _uiBtnPersonal = ui::Button::create("Assets/ui_common/btn_1_1_on.png", "", "Assets/ui_common/btn_1_off.png");
        _uiBtnPersonal->setScale9Enabled(true);
        _uiBtnPersonal->setCapInsets(Rect(33,33,2,2));
        _uiBtnPersonal->setContentSize(Size(150, 74));
        _uiBtnPersonal->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        _uiBtnPersonal->setPosition(Vec2(uiLayoutMission->getContentSize().width, uiLayoutMission->getContentSize().height / 2));
        _uiBtnPersonal->addClickEventListener(CC_CALLBACK_1(Popup7th::onClickRewardPersonal, this));
        _uiBtnPersonal->setZoomScale(0.05f);
        _uiBtnPersonal->setEnabled(false);
        uiLayoutMission->addChild(_uiBtnPersonal);
        {
            auto uiTextGet = ui::CText::create(GAME_TEXT("t_ui_raid_event_cooper_msg_7"), GAME_FONT, 20);
            uiTextGet->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
            uiTextGet->setPosition(Vec2(_uiBtnPersonal->getContentSize().width / 2, _uiBtnPersonal->getContentSize().height / 2));
            uiTextGet->setTextAreaSize(Size(_uiBtnPersonal->getContentSize().width * 0.9, uiTextGet->getContentSize().height));
            uiTextGet->setTextVerticalAlignment(TextVAlignment::CENTER);
            uiTextGet->setTextHorizontalAlignment(TextHAlignment::CENTER);
            ((Label*)uiTextGet->getVirtualRenderer())->setOverflow(Label::Overflow::SHRINK);
            uiTextGet->setName("UI_GET");
            _uiBtnPersonal->addChild(uiTextGet);
            
            auto uiImgItemIcon = ui::ImageView::create("");
            uiImgItemIcon->setPosition(Vec2(50, _uiBtnPersonal->getContentSize().height * 0.75));
            uiImgItemIcon->setName("UI_ICON");
            _uiBtnPersonal->addChild(uiImgItemIcon);
            
            auto uiTextItemCount = ui::CText::create(" ", GAME_FONT, 20);
            uiTextItemCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            uiTextItemCount->setPosition(Vec2(70, _uiBtnPersonal->getContentSize().height * 0.75));
            uiTextItemCount->setName("UI_COUNT");
            _uiBtnPersonal->addChild(uiTextItemCount);
            
            auto uiTextComplete = ui::CText::create(GAME_TEXT("t_ui_complete"), GAME_FONT, 20);
            uiTextComplete->setPosition(Vec2(_uiBtnPersonal->getContentSize().width / 2, _uiBtnPersonal->getContentSize().height / 2));
            uiTextComplete->setTextAreaSize(Size(_uiBtnPersonal->getContentSize().width * 0.9, uiTextComplete->getContentSize().height));
            uiTextComplete->setTextVerticalAlignment(TextVAlignment::CENTER);
            uiTextComplete->setTextHorizontalAlignment(TextHAlignment::CENTER);
            ((Label*)uiTextComplete->getVirtualRenderer())->setOverflow(Label::Overflow::SHRINK);
            uiTextComplete->setVisible(false);
            uiTextComplete->setName("UI_COMPLETE");
            _uiBtnPersonal->addChild(uiTextComplete);
        }
    }
    
    // reward list
    auto uiLayoutReward = ui::Layout::create();
    uiLayoutReward->setContentSize(Size(layout->getContentSize().width - 10, layout->getContentSize().height - uiImgTitle->getContentSize().height - uiImgInfoBG->getContentSize().height - uiLayoutMission->getContentSize().height));
    uiLayoutReward->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    uiLayoutReward->setPosition(Vec2(layout->getContentSize().width / 2, uiLayoutMission->getPositionY() - uiLayoutMission->getContentSize().height));
    layout->addChild(uiLayoutReward);
    {
        _uiListMissionPersonalReward = ui::CListView::create(this, Size(672, 100), ui::ScrollView::Direction::HORIZONTAL);
        _uiListMissionPersonalReward->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        _uiListMissionPersonalReward->setPosition(Vec2(uiLayoutReward->getContentSize().width / 2, uiLayoutReward->getContentSize().height / 2 - 14));
        _uiListMissionPersonalReward->setBounceEnabled(true);
        _uiListMissionPersonalReward->setScrollBarPositionFromCorner(Vec2(7, 7));
        _uiListMissionPersonalReward->setName("Mission_PersonalReward");
        uiLayoutReward->addChild(_uiListMissionPersonalReward);
        
        auto uiTextInfo = ui::CText::create(GAME_TEXT("t_ui_personal_reward"), GAME_FONT, 32);
        uiTextInfo->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        uiTextInfo->setPosition(Vec2(20, _uiListMissionPersonalReward->getPositionY() + _uiListMissionPersonalReward->getContentSize().height / 2));
        uiTextInfo->setTextColor(Color4B(186, 76, 96, 255));
        uiLayoutReward->addChild(uiTextInfo);
    }
}
 
#pragma mark - draw
void Popup7th::drawContent()
{
    
}

void Popup7th::drawList()
{
    _uiListMissionCoop->reloadData();
}

void Popup7th::drawListPersonalReward()
{
    ssize_t count = _listPersonalInfo.size();
    count = count > 7 ? 7 : count;
    
    int nPersonalLevel = Event7thManager::getInstance()->getPersonalLevel();
    nPersonalLevel = nPersonalLevel <= 0 ? 0 : nPersonalLevel;
    
    double width = count * 100 - 28;
    double height = 100;
    _uiListMissionPersonalReward->setContentSize(Size(width, height));
    _uiListMissionPersonalReward->reloadData();
    _uiListMissionPersonalReward->jumpToItem(nPersonalLevel, Vec2::ANCHOR_MIDDLE, Vec2::ANCHOR_MIDDLE);
}

void Popup7th::drawPersonal()
{
    auto objEventPersonal = Event7thManager::getInstance()->getPersonalInfoCurrent();
    if ( objEventPersonal == nullptr )
    {
        return;
    }
    
    int nPersonalLevel = Event7thManager::getInstance()->getPersonalLevel();
    
    int nCountMax = objEventPersonal->getPointCountMax() - objEventPersonal->getPointCountMin();
    int nCount = Event7thManager::getInstance()->getPersonalCount() - objEventPersonal->getPointCountMin();
    nCount = nCount <= 0 ? 0 : nCount;
    
    int nPercent = (double)nCount / nCountMax * 100;
    if ( nPercent > 100 )
        nPercent = 100;
    
    // item
    auto objItem = ItemsManager::getInstance()->getConvert(objEventPersonal->getReward());
    
    // progress
    _uiBarPersonal->setPercent(nPercent);
    _uiTextPersonalName->setString(GAME_TEXTFORMAT("t_ui_coevt_person_m", objEventPersonal->getLevel()));
    
    std::string strPercent = MafUtils::format("%d/%d", nCount, nCountMax);
    strPercent.append("\n");
    strPercent.append(GAME_TEXTFORMAT("t_ui_prison_jelly_item_option_2", nPercent));
    _uiTextPersonalPercent->setString(strPercent);
    
    // button
    {
        auto uiTextGet = _uiBtnPersonal->getChildByName<ui::CText*>("UI_GET");
        auto uiImgItemIcon = _uiBtnPersonal->getChildByName<ui::ImageView*>("UI_ICON");
        auto uiTextItemCount = _uiBtnPersonal->getChildByName<ui::CText*>("UI_COUNT");
        auto uiTextComplete = _uiBtnPersonal->getChildByName<ui::CText*>("UI_COMPLETE");
        
        if ( objEventPersonal->getLevel() > nPersonalLevel && nCount >= nCountMax )
        {
            _uiBtnPersonal->setEnabled(true);
        }
        else
        {
            _uiBtnPersonal->setEnabled(false);
        }
        
        if ( objEventPersonal->getLevel() == nPersonalLevel )
        {
            uiTextGet->setVisible(false);
            uiImgItemIcon->setVisible(false);
            uiTextItemCount->setVisible(false);
            
            //
            uiTextComplete->setVisible(true);
        }
        else if ( objItem->getType().compare("COSTUME") == 0 && objItem->getTypeDetail().compare("SKIN") == 0 )
        {
            uiTextGet->setVisible(true);
            uiImgItemIcon->setVisible(true);
            uiImgItemIcon->loadTexture(objItem->getPath());
            uiImgItemIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
            uiImgItemIcon->setPosition(Vec2(_uiBtnPersonal->getContentSize().width / 2, _uiBtnPersonal->getContentSize().height / 2));
            uiTextItemCount->setVisible(false);
            
            //
            uiTextComplete->setVisible(false);
            
        }
        else
        {
            uiTextGet->setVisible(true);
            uiTextGet->setTextColor(Color4B::WHITE);
            uiImgItemIcon->setVisible(true);
            uiImgItemIcon->loadTexture(objItem->getPath());
            uiTextItemCount->setVisible(true);
            uiTextItemCount->setString(objItem->getCount());
            uiTextItemCount->setTextColor(Color4B::WHITE);
            
            //
            uiTextComplete->setVisible(false);
            
            if ( _uiBtnPersonal->isEnabled() == false )
            {
                uiTextGet->setTextColor(Color4B::GRAY);
                uiTextItemCount->setTextColor(Color4B::GRAY);
            }
        }
    }
}

void Popup7th::drawCellMissionCoop(cocos2d::ui::Widget* childview,  ssize_t idx)
{
    // size
    Size size = childview->getContentSize();

    // data
    auto obj = _listCoopInfo.at(idx);
        
    auto objItem = ItemsManager::getInstance()->getConvert(obj->getReward());
    
    int nCoopLevel = Event7thManager::getInstance()->getCoopLevel(obj->getType());
    int nPersonalLevel = Event7thManager::getInstance()->getPersonalLevel();
    
    int nCountPersonal = Event7thManager::getInstance()->getCoopPersonalCount(obj->getType());
    int nCountPersonalLimit = obj->getPointCountPersonalLimit();
    
    int nCountMax = obj->getPointCountMax() - obj->getPointCountMin();
    int nCount = Event7thManager::getInstance()->getCoopCount(obj->getType()) - obj->getPointCountMin();
    nCount = nCount <= 0 ? 0 : nCount;
    
    int nPercent = (double)nCount / nCountMax * 100;
    if ( nPercent > 100 )
        nPercent = 100;
    
    // ui
    childview->removeAllChildren();
    
    auto uiLayout = ui::Layout::create();
    uiLayout->setContentSize(Size(size.width - 10, size.height - 4));
    uiLayout->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    uiLayout->setPosition(Vec2(size.width / 2, size.height / 2));
    childview->addChild(uiLayout);
    
    // bg
    auto uiImgBG = ui::ImageView::create("Assets/ui/event/list_progressbar_1_1.png");
    uiImgBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    uiImgBG->setPosition(Vec2(0, uiLayout->getContentSize().height / 2));
    uiLayout->addChild(uiImgBG);
    {
        std::string pathIcon = "Assets/ui/daily_event/icon_stamp_off.png";
        if ( nCountPersonal >= nCountPersonalLimit )
        {
            pathIcon = "Assets/ui/daily_event/icon_stamp_on.png";
        }
        auto uiImgIcon = ui::ImageView::create(pathIcon);
        uiImgIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        uiImgIcon->setPosition(Vec2(0, uiImgBG->getContentSize().height / 2));
        uiImgBG->addChild(uiImgIcon);
        
        auto uiTextName = ui::CText::create(obj->getName(), GAME_FONT, 24);
        uiTextName->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        uiTextName->setPosition(Vec2(60, uiImgBG->getContentSize().height));
        uiTextName->setTextAreaSize(Size(uiImgBG->getContentSize().width - 120, uiTextName->getContentSize().height));
        uiTextName->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiTextName->setTextHorizontalAlignment(TextHAlignment::LEFT);
        ((Label*)uiTextName->getVirtualRenderer())->setOverflow(Label::Overflow::SHRINK);
        uiImgBG->addChild(uiTextName);
        
        auto uiTextDesc = ui::CText::create(obj->getDesc(), GAME_FONT, 20);
        uiTextDesc->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        uiTextDesc->setPosition(Vec2(60, uiTextName->getPositionY() - uiTextName->getContentSize().height));
        uiTextDesc->setTextAreaSize(Size(uiImgBG->getContentSize().width - 120, uiTextDesc->getContentSize().height));
        uiTextDesc->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiTextDesc->setTextHorizontalAlignment(TextHAlignment::LEFT);
        ((Label*)uiTextDesc->getVirtualRenderer())->setOverflow(Label::Overflow::SHRINK);
        uiImgBG->addChild(uiTextDesc);

        std::string strPercent = MafUtils::format("%d/%d", nCount, nCountMax);
        strPercent.append("\n");
        strPercent.append(GAME_TEXTFORMAT("t_ui_prison_jelly_item_option_2", nPercent));
        
        auto uiTextPercent = ui::CText::create(strPercent, GAME_FONT, 20);
        uiTextPercent->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
        uiTextPercent->setPosition(Vec2(uiImgBG->getContentSize().width - 5, 5));
        uiTextPercent->setTextColor(Color4B(255, 68, 53, 255));
        uiTextPercent->setTextHorizontalAlignment(TextHAlignment::RIGHT);
        uiImgBG->addChild(uiTextPercent);
        
        // progress
        auto uiBar = ui::LoadingBar::create("Assets/ui/event/list_progressbar_1_2.png");
        uiBar->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        uiBar->setPosition(Vec2(0, 0));
        uiBar->setPercent(nPercent);
        uiImgBG->addChild(uiBar);
    }
     
    if ( objItem != nullptr )
    {
        auto uiBtnReward = ui::Button::create("Assets/ui_common/btn_1_off.png");
        uiBtnReward->setScale9Enabled(true);
        uiBtnReward->setCapInsets(Rect(33,33,2,2));
        uiBtnReward->setContentSize(Size(150, 74));
        uiBtnReward->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        uiBtnReward->setPosition(Vec2(uiLayout->getContentSize().width, uiLayout->getContentSize().height / 2));
        uiBtnReward->addClickEventListener(CC_CALLBACK_1(Popup7th::onClickRewardCoop, this));
        uiBtnReward->setZoomScale(0.05f);
        uiBtnReward->setTag((int)idx);
        uiLayout->addChild(uiBtnReward);
            
        if ( obj->getConditionPersonLevel() <= nPersonalLevel && obj->getLevel() > nCoopLevel && nCount >= nCountMax )
        {
            uiBtnReward->loadTextureNormal("Assets/ui_common/btn_1_1_on.png");
        }
        
        if ( obj->getLevel() == nCoopLevel )
        {
            auto uiTextComplete = ui::CText::create(GAME_TEXT("t_ui_complete"), GAME_FONT, 20);
            uiTextComplete->setPosition(Vec2(uiBtnReward->getContentSize().width / 2, uiBtnReward->getContentSize().height / 2));
            uiTextComplete->setTextAreaSize(Size(uiBtnReward->getContentSize().width * 0.9, uiTextComplete->getContentSize().height));
            uiTextComplete->setTextVerticalAlignment(TextVAlignment::CENTER);
            uiTextComplete->setTextHorizontalAlignment(TextHAlignment::CENTER);
            ((Label*)uiTextComplete->getVirtualRenderer())->setOverflow(Label::Overflow::SHRINK);
            uiBtnReward->addChild(uiTextComplete);
        }
        else
        {
            auto uiTextGet = ui::CText::create(GAME_TEXT("t_ui_raid_event_cooper_msg_7"), GAME_FONT, 20);
            uiTextGet->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
            uiTextGet->setPosition(Vec2(uiBtnReward->getContentSize().width / 2, uiBtnReward->getContentSize().height / 2));
            uiTextGet->setTextColor(Color4B::WHITE);
            uiTextGet->setTextAreaSize(Size(uiBtnReward->getContentSize().width * 0.9, uiTextGet->getContentSize().height));
            uiTextGet->setTextVerticalAlignment(TextVAlignment::CENTER);
            uiTextGet->setTextHorizontalAlignment(TextHAlignment::CENTER);
            ((Label*)uiTextGet->getVirtualRenderer())->setOverflow(Label::Overflow::SHRINK);
            uiBtnReward->addChild(uiTextGet);
            if ( uiBtnReward->isEnabled() == false )
            {
                uiTextGet->setTextColor(Color4B::GRAY);
            }
            
            auto uiImgItemIcon = ui::ImageView::create(objItem->getPath());
            uiImgItemIcon->setPosition(Vec2(50, uiBtnReward->getContentSize().height * 0.75));
            uiImgItemIcon->setScale(0.6f);
            uiBtnReward->addChild(uiImgItemIcon);
            
            auto uiTextItemCount = ui::CText::create(objItem->getCount(), GAME_FONT, 20);
            uiTextItemCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            uiTextItemCount->setPosition(Vec2(75, uiBtnReward->getContentSize().height * 0.75));
            uiTextItemCount->setTextColor(Color4B::WHITE);
            uiBtnReward->addChild(uiTextItemCount);
            if ( uiBtnReward->isEnabled() == false )
            {
                uiTextItemCount->setTextColor(Color4B::GRAY);
            }
        }
    }
}

void Popup7th::drawCellMissionPersonalReward(cocos2d::ui::Widget* childview,  ssize_t idx)
{
    // size
    Size size = childview->getContentSize();

    // data
    auto obj = _listPersonalInfo.at(idx);
    
    // item
    auto objItem = ItemsManager::getInstance()->getConvert(obj->getReward());
    
    int nPersonalLevel = Event7thManager::getInstance()->getPersonalLevel();
    
    // ui
    childview->removeAllChildren();
    
    auto uiLayout = ui::Layout::create();
    uiLayout->setContentSize(Size(size.width, size.height));
    uiLayout->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    uiLayout->setPosition(Vec2(size.width / 2, size.height / 2));
    childview->addChild(uiLayout);
    
    auto uiImgItemBG = ui::ImageView::create("Assets/ui_common/box_item.png");
    uiImgItemBG->setContentSize(Size(72, 72));
    uiImgItemBG->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    uiImgItemBG->setPosition(Vec2(uiLayout->getContentSize().width, uiLayout->getContentSize().height));
    uiImgItemBG->setScale9Enabled(true);
    uiImgItemBG->setCapInsets(Rect(5,30,10,10));
    uiLayout->addChild(uiImgItemBG);
    if ( objItem != nullptr )
    {
        auto uiImgItemIcon = ui::ImageView::create(objItem->getPath());
        uiImgItemIcon->setPosition(Vec2(uiImgItemBG->getContentSize().width / 2, uiImgItemBG->getContentSize().height * 0.58));
        uiImgItemBG->addChild(uiImgItemIcon);
        
        auto uiTextItemCount = ui::CText::create(objItem->getCount(), GAME_FONT, 20);
        uiTextItemCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        uiTextItemCount->setPosition(Vec2(uiImgItemBG->getContentSize().width / 2, 0));
        uiTextItemCount->enableOutline(Color4B::BLACK, 2);
        uiImgItemBG->addChild(uiTextItemCount);
    }
    
    //
    auto uiTextLevel = ui::CText::create(StringUtils::format("Lv.%d", obj->getLevel()), GAME_FONT, 20);
    uiTextLevel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    uiTextLevel->setPosition(Vec2(uiImgItemBG->getContentSize().width / 2, 0));
    uiTextLevel->setTextColor(Color4B(255, 149, 0, 255));
    uiImgItemBG->addChild(uiTextLevel);
    
    //
    if ( idx != 0 )
    {
        auto uiImgArrow = ui::ImageView::create("Assets/ui_common/arrow_R01.png");
        uiImgArrow->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        uiImgArrow->setPosition(Vec2(-6, uiImgItemBG->getContentSize().height / 2));
        uiImgItemBG->addChild(uiImgArrow);
    }
    
    if ( obj->getLevel() <= nPersonalLevel )
    {
        auto uiImgCheck = ui::ImageView::create("Assets/icon/icon_check_01.png");
        uiImgCheck->setPosition(Vec2(uiImgItemBG->getContentSize().width / 2, uiImgItemBG->getContentSize().height / 2));
        uiImgItemBG->addChild(uiImgCheck);
    }
}

#pragma mark - set, get

#pragma mark - callback
void Popup7th::callbackInfo(bool bResult, int nResult)
{
    PopupLoading::hide();
    
    if ( bResult == false )
    {
        /*
         0 : 오류
         1 : 성공
         2 : 이벤트 기간이 아님
         */
        std::string strMessage = GAME_TEXT("t_ui_error_30");
        if ( nResult == 2 )
        {
            strMessage = GAME_TEXT("t_ui_error_6");
        }
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), strMessage);
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, [=](Ref* sender){
            hide();
        });
        popup->show();
        return;
    }
    
    
    _listCoopInfo.clear();
    _listCoopInfo = Event7thManager::getInstance()->getListCoopInfo();
    
    _listPersonalInfo.clear();
    _listPersonalInfo = Event7thManager::getInstance()->getPersonalInfo();
    
    // draw
    drawContent();
    drawList();
    drawListPersonalReward();
    drawPersonal();
}

void Popup7th::callbackReward(bool bResult, int nResult, std::string strReward)
{
    PopupLoading::hide();
    
    if ( bResult == false )
    {
        /*
         0 : 오류
         1 : 성공
         2 : 이벤트 기간이 아님
         3 : 이미 보상 획득,
         4 : 현재 받을 수 있는 보상 레벨이 아님,
         5 : 개인 미션 레벨 부족
         */
        std::string strMessage = GAME_TEXT("t_ui_error_30");
        if ( nResult == 2 )
        {
            strMessage = "";
        }
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), strMessage);
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, [=](Ref* sender){
            hide();
        });
        popup->show();
        return;
    }
    
    PopupLoading::show();
    Event7thManager::getInstance()->requestInfo(CC_CALLBACK_2(Popup7th::callbackInfo, this));
}
 
#pragma mark - click
void Popup7th::onClickClose(Ref* sender)
{
    hide();
}

void Popup7th::onClickRewardCoop(Ref* sender)
{
    auto uiBtn = dynamic_cast<ui::Button*>(sender);
    
    auto objCoop = _listCoopInfo.at(uiBtn->getTag());
    
    int nType = objCoop->getType();
    
    int nCoopLevel = Event7thManager::getInstance()->getCoopLevel(nType);
    
    int nCount = Event7thManager::getInstance()->getCoopCount(nType);
    int nCountMax= objCoop->getPointCountMax();
    
    if ( objCoop->getLevel() <= nCoopLevel || nCount < nCountMax )
    {
        return;
    }
    
    if ( objCoop->getConditionPersonLevel() > Event7thManager::getInstance()->getPersonalLevel() )
    {
        std::string strMessageTitle = GAME_TEXT("t_ui_personal_mission");
        std::string strMessage = GAME_TEXTFORMAT("t_ui_lv_require", objCoop->getConditionPersonLevel(), strMessageTitle.c_str());
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), strMessage);
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        
        return;
    }
    
    PopupLoading::show();
    Event7thManager::getInstance()->requestReward(nType, objCoop->getLevel(), CC_CALLBACK_3(Popup7th::callbackReward, this));
}

void Popup7th::onClickRewardPersonal(Ref* sender)
{
    auto uiBtn = dynamic_cast<ui::Button*>(sender);
    
    auto objPersonal = Event7thManager::getInstance()->getPersonalInfoCurrent();
    if ( objPersonal == nullptr )
    {
        return;
    }
    
    int nPersonalLevel = Event7thManager::getInstance()->getPersonalLevel();
    
    int nCount = Event7thManager::getInstance()->getPersonalCount();
    int nCountMax= objPersonal->getPointCountMax();
    if ( objPersonal->getLevel() <= nPersonalLevel || nCount < nCountMax )
    {
        return;
    }
    
    PopupLoading::show();
    Event7thManager::getInstance()->requestReward(101, objPersonal->getLevel(), CC_CALLBACK_3(Popup7th::callbackReward, this));
}

 
