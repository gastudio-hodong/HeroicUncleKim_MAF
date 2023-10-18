//
//  LayerEventRaceTopUI.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/08/12.
//

#include "LayerEventRaceTopUI.h"

#include "GameUIPopup/Common/PopupHelp.h"
#include "GameUIPopup/OtherEvent/EventRace/PopupEventRaceRankRewardInfo.h"
#include "GameUIPopup/OtherEvent/EventRace/UI/UIRaceNumber.h"

#include "ManagerEvent/EventRaceManager.h"

USING_NS_CC;

LayerEventRaceTopUI* LayerEventRaceTopUI::create(const cocos2d::Size size)
{
    LayerEventRaceTopUI* pRet = new(std::nothrow) LayerEventRaceTopUI();
    if ( pRet && pRet->init(size) )
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

LayerEventRaceTopUI::LayerEventRaceTopUI(void):
_clipper(nullptr)
,_lyLobby(nullptr)
,_lyMiniMap(nullptr)
,_lyMiniMapLane(nullptr)
{
    _listNumber.clear();
}

LayerEventRaceTopUI::~LayerEventRaceTopUI(void)
{
    _listNumber.clear();
    EventRaceManager::getInstance()->unSubRaceReadyEvent(this);
    EventRaceManager::getInstance()->unSubRaceStartEvent(this);
}

bool LayerEventRaceTopUI::init(const cocos2d::Size size)
{
    if (!LayerColor::init())
    {
        return false;
    }
    setContentSize(size);
    setIgnoreAnchorPointForPosition(false);
    // init
    initVar();
    initUi();
    
    EventRaceManager::getInstance()->subRaceReadyEvent(new MafFunction<void()>(CC_CALLBACK_0(LayerEventRaceTopUI::onEventGameRaedy, this), this));
    EventRaceManager::getInstance()->subRaceStartEvent(new MafFunction<void()>(CC_CALLBACK_0(LayerEventRaceTopUI::onEventGameStart, this), this));
    
    return true;
}

void LayerEventRaceTopUI::update(float dt)
{
    for(auto obj : _listNumber)
    {
        auto objHorse = EventRaceManager::getInstance()->getInfoHorse(obj->getTag());
        if(objHorse == nullptr)
            continue;
        
        float posX = _lyMiniMapLane->getContentSize().width * objHorse->getCurrentSection() / 100;
        
        obj->setPosition(posX, obj->getPosition().y);
        obj->setLocalZOrder((int)objHorse->getCurrentSection() - objHorse->getCurrentRank());
    }
}

#pragma mark - init
void LayerEventRaceTopUI::initVar()
{
    
}

void LayerEventRaceTopUI::initUi()
{
    auto ly = LayerColor::create();
    ly->setIgnoreAnchorPointForPosition(false);
    ly->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    ly->setContentSize(getContentSize());
    ly->setPosition(getContentSize()/2);

    auto clipper = ClippingNode::create(ly);
    clipper->setContentSize(ly->getContentSize());
    clipper->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    clipper->setPosition(getContentSize()/2);
    clipper->setIgnoreAnchorPointForPosition(false);
    addChild(clipper);

    _lyLobby = LayerColor::create();
    _lyLobby->setContentSize(getContentSize());
    _lyLobby->setIgnoreAnchorPointForPosition(false);
    _lyLobby->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _lyLobby->setPosition(getContentSize().width/2, getContentSize().height);
    clipper->addChild(_lyLobby);
    
    _lyMiniMap = LayerColor::create();
    _lyMiniMap->setContentSize(getContentSize());
    _lyMiniMap->setIgnoreAnchorPointForPosition(false);
    _lyMiniMap->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _lyMiniMap->setPosition(_lyLobby->getPosition() + Vec2::DOWN * _lyLobby->getContentSize().height);
    clipper->addChild(_lyMiniMap);
    
//    //
    uiLobby();
    uiMinimap();
}
#pragma mark - UI
void LayerEventRaceTopUI::uiLobby()
{
    _lyLobby->removeAllChildren();
    
    auto btnHelp = ui::Button::create("Assets/ui/race_event/race_btn_gameinfo.png");
    btnHelp->addClickEventListener(CC_CALLBACK_1(LayerEventRaceTopUI::onClickHelp, this));
    btnHelp->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    btnHelp->setZoomScale(0.05f);
    btnHelp->setPosition(Vec2(10 , _lyLobby->getContentSize().height/2));
    _lyLobby->addChild(btnHelp);
    {
        auto lbText = Label::createWithTTF(GAME_TEXT("t_ui_event_race_info_title_1"), GAME_FONT, 20);
        lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        lbText->setPosition(15, btnHelp->getContentSize().height/2);
        lbText->setDimensions(btnHelp->getContentSize().width - 55, btnHelp->getContentSize().height - 5);
        lbText->setOverflow(cocos2d::Label::Overflow::SHRINK);
        lbText->setAlignment(cocos2d::TextHAlignment::LEFT, cocos2d::TextVAlignment::CENTER);
        lbText->setColor(Color3B(150,120,55));
        btnHelp->addChild(lbText);
    }
    
    auto btnRankReward = ui::Button::create("Assets/icon/icon_reward_info_box.png");
    btnRankReward->addClickEventListener(CC_CALLBACK_1(LayerEventRaceTopUI::onClickRankReward, this));
    btnRankReward->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    btnRankReward->setZoomScale(0.05f);
    btnRankReward->setPosition(btnHelp->getPosition() + Vec2::RIGHT * (btnHelp->getContentSize().width + 10));
    _lyLobby->addChild(btnRankReward);
    
    auto sprTicketIcon = Sprite::create("Assets/icon/icon_race_ticket.png");
    sprTicketIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    sprTicketIcon->setPosition(_lyLobby->getContentSize().width - 300, _lyLobby->getContentSize().height/2);
    _lyLobby->addChild(sprTicketIcon);
    {
        auto lbCount = MafNode::createBmFont(std::to_string(EventRaceManager::getInstance()->getTicket()), GAME_BM_FONT_AL, 30);
        lbCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        lbCount->setPosition(sprTicketIcon->getPosition() + Vec2::RIGHT * (sprTicketIcon->getContentSize().width));
        lbCount->setDimensions(50, lbCount->getContentSize().height);
        lbCount->setOverflow(cocos2d::Label::Overflow::SHRINK);
        lbCount->setAlignment(cocos2d::TextHAlignment::LEFT, cocos2d::TextVAlignment::CENTER);
        lbCount->setColor(COLOR_COIN);
        lbCount->enableShadow(Color4B::BLACK,Size(3,-3),5);
        _lyLobby->addChild(lbCount);
    }
    
    auto sprCoinIcon = Sprite::create("Assets/icon/icon_race_coin.png");
    sprCoinIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    sprCoinIcon->setPosition(_lyLobby->getContentSize().width - 180, _lyLobby->getContentSize().height/2);
    _lyLobby->addChild(sprCoinIcon);
    {
        std::string strCount = MafUtils::doubleToString(EventRaceManager::getInstance()->getCoin());
        MafUtils::convertNumberToShort(strCount);
        
        auto lbCount = MafNode::createBmFont(strCount, GAME_BM_FONT_AL, 30);
        lbCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        lbCount->setPosition(sprCoinIcon->getPosition() + Vec2::RIGHT * (sprCoinIcon->getContentSize().width));
        lbCount->setDimensions(_lyLobby->getContentSize().width - lbCount->getPosition().x - 5, lbCount->getContentSize().height);
        lbCount->setOverflow(cocos2d::Label::Overflow::SHRINK);
        lbCount->setAlignment(cocos2d::TextHAlignment::LEFT, cocos2d::TextVAlignment::CENTER);
        lbCount->setColor(Color3B(164,0,200));
        lbCount->enableShadow(Color4B::BLACK,Size(3,-3),5);
        _lyLobby->addChild(lbCount);
    }
    
}
void LayerEventRaceTopUI::uiMinimap()
{
    _lyMiniMap->removeAllChildren();
    
    auto sprLine = ui::Scale9Sprite::create("Assets/ui/race_event/minimap_rail.png", Rect::ZERO, Rect(78, 0, 5, 46));
    sprLine->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    sprLine->setContentSize(Size(_lyMiniMap->getContentSize().width - 10, sprLine->getContentSize().height));
    sprLine->setPosition(_lyMiniMap->getContentSize().width/2, 5);
    _lyMiniMap->addChild(sprLine);
    
    _lyMiniMapLane = LayerColor::create();
    _lyMiniMapLane->setContentSize(_lyMiniMap->getContentSize() - Size(158, sprLine->getContentSize().height));
    _lyMiniMapLane->setIgnoreAnchorPointForPosition(false);
    _lyMiniMapLane->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _lyMiniMapLane->setPosition(_lyMiniMap->getContentSize().width/2, sprLine->getPositionY() + 10);
    _lyMiniMap->addChild(_lyMiniMapLane);
    
    auto listHorse = EventRaceManager::getInstance()->getListHorse();
    for(int i = 0; i < listHorse.size(); ++i)
    {
        auto sprNum = Sprite::create(MafUtils::format("Assets/ui/race_event/minimap_race_rail_number%d.png", i+1));
        sprNum->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        sprNum->setPosition(Vec2::ZERO);
        sprNum->setTag(i+1);
        _lyMiniMapLane->addChild(sprNum);
        
        _listNumber.pushBack(sprNum);
    }
}

#pragma mark - Event
void LayerEventRaceTopUI::onClickHelp(Ref* const sender)
{
    if(EventRaceManager::getInstance()->isGameRunning())
        return;
    
    auto popup = PopupHelp::create(4, "t_ui_event_race_info_title_%d", "t_ui_event_race_info_msg_%d");
    popup->show();
}
void LayerEventRaceTopUI::onClickRankReward(Ref* const sender)
{
    if(EventRaceManager::getInstance()->isGameRunning())
        return;
    
    auto popup = PopupEventRaceRankRewardInfo::create();
    popup->show();
}

#pragma mark - Event
void LayerEventRaceTopUI::onEventGameRaedy()
{
    _lyLobby->runAction(MoveBy::create(0.2f, Vec2::UP * (_lyLobby->getContentSize().height)));
    _lyMiniMap->runAction(MoveBy::create(0.2f, Vec2::UP * (_lyLobby->getContentSize().height)));
}

void LayerEventRaceTopUI::onEventGameStart()
{
    scheduleUpdate();
}
