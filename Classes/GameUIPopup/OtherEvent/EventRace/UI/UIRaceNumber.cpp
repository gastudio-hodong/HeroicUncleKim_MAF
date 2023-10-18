//
//  UIRaceNumber.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/07/26.
//

#include "UIRaceNumber.h"

#include "GameUIPopup/OtherEvent/EventRace/UI/UISubItemStatus.h"

#include "ManagerEvent/EventRaceManager.h"

USING_NS_CC;

UIRaceNumber* UIRaceNumber::create(const int nIdx, const bool isCurrent)
{
    UIRaceNumber* pRet = new(std::nothrow) UIRaceNumber();
    if ( pRet && pRet->init(nIdx, isCurrent) )
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

UIRaceNumber::UIRaceNumber(void):
_isCurrent(false)
,_nIdx(0)
,_infoHorse(nullptr)
,_sprCoinIcon(nullptr)
{
}

UIRaceNumber::~UIRaceNumber(void)
{
    EventRaceManager::getInstance()->unSubRaceStartEvent(this);
    EventRaceManager::getInstance()->unSubStatusEvent(this);
    EventRaceManager::getInstance()->unSubInvestmentEvent(this);
    EventRaceManager::getInstance()->unSubRaceReadyEvent(this);
}

bool UIRaceNumber::init(const int nIdx, const bool isCurrent)
{
    if (!LayerColor::init())
    {
        return false;
    }
    _nIdx = nIdx;
    _isCurrent = isCurrent;
    
    _infoHorse = EventRaceManager::getInstance()->getInfoHorse(_nIdx);
    // init
    initVar();
    initUI();
    
    EventRaceManager::getInstance()->subRaceStartEvent(new MafFunction<void()>(CC_CALLBACK_0(UIRaceNumber::onEventClickStart, this), this));
    EventRaceManager::getInstance()->subStatusEvent(new MafFunction<void(int)>(CC_CALLBACK_1(UIRaceNumber::onEventClickStatus, this), this));
    EventRaceManager::getInstance()->subInvestmentEvent(new MafFunction<void(int)>(CC_CALLBACK_1(UIRaceNumber::onEvnetInvestment, this), this));
    EventRaceManager::getInstance()->subRaceReadyEvent(new MafFunction<void()>(CC_CALLBACK_0(UIRaceNumber::onEventRaceRaedy, this), this));
    return true;
}
#pragma mark - init
void UIRaceNumber::initVar()
{
    setContentSize(Size(128,128));
    setIgnoreAnchorPointForPosition(false);
}

void UIRaceNumber::initUI()
{
    removeAllChildren();
    
    auto btnRaceNumber = ui::Button::create("Assets/ui/race_event/skin_num_box.png");
    btnRaceNumber->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    btnRaceNumber->setPosition(getContentSize()/2);
    btnRaceNumber->addClickEventListener(CC_CALLBACK_1(UIRaceNumber::onClick, this));
    addChild(btnRaceNumber);
    
    auto menu = MafNode::MafMenu::create();
    menu->setPosition(Vec2::ZERO);
    addChild(menu);
    
    //
    auto itemClickArea = MafNode::MafMenuItemSprite::create(Layer::create(), nullptr, CC_CALLBACK_1(UIRaceNumber::onClick, this));
    itemClickArea->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    itemClickArea->setPosition(btnRaceNumber->getPosition() + Vec2::RIGHT * btnRaceNumber->getContentSize().width/2);
    itemClickArea->setContentSize(getContentSize() + Size(30, 0));
    menu->addChild(itemClickArea);
    
    auto sprNumber = Sprite::create(MafUtils::format("Assets/ui/race_event/race_rail_num%02d.png", _nIdx));
    sprNumber->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    sprNumber->setPosition(getContentSize().width/2, getContentSize().height - 5);
    addChild(sprNumber);
    
    auto sprFx = Sprite::create("Assets/ui/race_event/skin_num_box_select.png");
    sprFx->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    sprFx->setPosition(getContentSize()/2);
    sprFx->setVisible(_isCurrent);
    addChild(sprFx);
    
    _sprCoinIcon = Sprite::create("Assets/ui/race_event/icon_race_coin00.png");
    _sprCoinIcon->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    _sprCoinIcon->setScale(0.75f);
    _sprCoinIcon->setPosition(18, 21);
    addChild(_sprCoinIcon);
    
    //104, 44 34
    std::string strCount = MafUtils::doubleToString(_infoHorse->getInvest());
    MafUtils::convertNumberToShort(strCount);
    
    auto lbBetCount = Label::createWithTTF(strCount, GAME_FONT, 30);
    lbBetCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    lbBetCount->setPosition(110, 36);
    lbBetCount->setDimensions(60, 44);
    lbBetCount->setAlignment(cocos2d::TextHAlignment::RIGHT, cocos2d::TextVAlignment::CENTER);
    lbBetCount->setOverflow(cocos2d::Label::Overflow::SHRINK);
    lbBetCount->enableBold();
    lbBetCount->setColor(Color3B(250,255,205));
    addChild(lbBetCount);
}

#pragma mark - Utils
void UIRaceNumber::hide()
{
    removeFromParent();
}
void UIRaceNumber::onClick(Ref* sender)
{
    if(EventRaceManager::getInstance()->isGameRunning())
        return;
    if(_isCurrent)
        EventRaceManager::getInstance()->invokeStatusEvent(0);
    else
        EventRaceManager::getInstance()->invokeStatusEvent(_nIdx);
    
    SoundManager::SoundEffectStart(SOUND_BOX_CREATE, E_SOUND_TYPE::CONTENTS);
}
#pragma mark - Event
void UIRaceNumber::onEventRaceRaedy()
{
    runAction(MoveBy::create(0.2f, Vec2(-(getContentSize().width + 30), 0)));
}

void UIRaceNumber::onEventClickStart()
{
    runAction(Sequence::create(DelayTime::create(0.01), CallFunc::create(CC_CALLBACK_0(UIRaceNumber::hide, this)), nullptr));
}

void UIRaceNumber::onEventClickStatus(const int nIdx)
{
    if(_nIdx == nIdx)
        _isCurrent = true;
    else
        _isCurrent = false;
    
    initUI();
}
void UIRaceNumber::onEvnetInvestment(const int nIdx)
{
    _infoHorse = EventRaceManager::getInstance()->getInfoHorse(_nIdx);
    initUI();
    
    if(nIdx != _nIdx)
        return;
    
    auto anim = Animation::create();
    anim->setDelayPerUnit(0.05f);
    for(int i = 0; i < 6; ++i)
    {
        anim->addSpriteFrameWithFile(MafUtils::format("Assets/ui/race_event/icon_race_coin%02d.png", i));
    }
    anim->addSpriteFrameWithFile("Assets/ui/race_event/icon_race_coin00.png");
    
    auto animate = Animate::create(anim);
    _sprCoinIcon->runAction(animate);
}
