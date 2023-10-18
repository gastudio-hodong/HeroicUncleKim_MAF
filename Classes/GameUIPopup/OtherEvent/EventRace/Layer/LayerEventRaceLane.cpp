//
//  LayerEventRaceLane.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/07/22.
//

#include "LayerEventRaceLane.h"

#include "GameUIPopup/OtherEvent/EventRace/UI/UIRaceNumber.h"

#include "ManagerEvent/EventRaceManager.h"
#include "ManagerGame/CostumeSkinManager.h"

USING_NS_CC;

LayerEventRaceLane* LayerEventRaceLane::create(const cocos2d::Size size, const int nIdx)
{
    LayerEventRaceLane* pRet = new(std::nothrow) LayerEventRaceLane();
    if ( pRet && pRet->init(size, nIdx) )
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

LayerEventRaceLane::LayerEventRaceLane(void):
_isGameStart(false)
,_isCurrentSelect(false)
,_isCompleteRankMarkAction(false)
,_isRunningRankMarkAction(false)
,_isCurrentBoost(false)
,_nIdx(0)
,_nTick(0)
,_nTimer(0)
,_curSpeed(0)
,_currentStamina(0)
,_infoHorse(nullptr)
,_clipper(nullptr)
,_lyLane(nullptr)
,_lyHorse(nullptr)
,_lyLobby(nullptr)
,_sprDoubleEffect(nullptr)
,_sprSelect(nullptr)
,_sprRankMark(nullptr)
,_sprHorse(nullptr)
,_sprCommentBubble(nullptr)
,_sprRunningDust(nullptr)
,_sprBetBox(nullptr)
,_sprNumber(nullptr)
,_lbBetCount(nullptr)
{
}

LayerEventRaceLane::~LayerEventRaceLane(void)
{
    EventRaceManager::getInstance()->unSubRaceReadyEvent(this);
    EventRaceManager::getInstance()->unSubRaceStartEvent(this);
    EventRaceManager::getInstance()->unSubStatusEvent(this);
    EventRaceManager::getInstance()->unSubAdsEvent(this);
}

bool LayerEventRaceLane::init(const cocos2d::Size size, const int nIdx)
{
    if (!LayerColor::init())
    {
        return false;
    }
    setContentSize(size);
    setIgnoreAnchorPointForPosition(false);
    _nIdx = nIdx;
    _infoHorse = EventRaceManager::getInstance()->getInfoHorse(_nIdx);
    if(_nIdx == 1)
        _isCurrentSelect = true;
    // init
    initVar();
    initUi();
    
    EventRaceManager::getInstance()->subRaceReadyEvent(new MafFunction<void()>(CC_CALLBACK_0(LayerEventRaceLane::onEventGameRaedy, this), this));
    EventRaceManager::getInstance()->subRaceStartEvent(new MafFunction<void()>(CC_CALLBACK_0(LayerEventRaceLane::onEventGameStart, this), this));
    EventRaceManager::getInstance()->subStatusEvent(new MafFunction<void(int)>(CC_CALLBACK_1(LayerEventRaceLane::onEventClickStatus, this), this));
    EventRaceManager::getInstance()->subAdsEvent(new MafFunction<void(int)>(CC_CALLBACK_1(LayerEventRaceLane::onEventAds, this), this));
    
    return true;
}

void LayerEventRaceLane::race(float dt)
{
    int nTick = (int)_nTick + 1;
    if(nTick >= _infoHorse->getLastSimulateTick())
        nTick = _infoHorse->getLastSimulateTick();
    
    auto objLog = _infoHorse->getSimulateSectionLog(nTick);
    if(objLog == nullptr)
        return;
    
    if(_isCurrentBoost != objLog->isBoost())
    {
        _isCurrentBoost = objLog->isBoost();
        viewRunningDust();
    }
    if(_currentStamina > 0 && objLog->getStamina() <= 0)
    {
        viewRunningDust();
    }
    
    _infoHorse->setCurrentSection(_infoHorse->getCurrentSection() + objLog->getSpeed() * dt * 2);
    _lyLane->setPosition(-_infoHorse->getCurrentSection() * 50, _lyLane->getPosition().y);
    
    _curSpeed = (objLog->getSpeed() * 50);
    _nTick += 1 * dt * 2;
}
void LayerEventRaceLane::update(float dt)
{
    _nTimer += dt;
    
    if(_nTimer <= 2.f)
        return;
    else
    {
        if(!_isRunningRankMarkAction)
        {
            _isRunningRankMarkAction = true;
            auto action1 = MoveBy::create(0.2f, Vec2(-_sprRankMark->getContentSize().width, 0));
            auto action2 = FadeIn::create(0.2f);
            _sprRankMark->runAction(Spawn::create(action1, action2, nullptr));
        }
    }
    
    if(_isRunningRankMarkAction && !_infoHorse->isComplateRace())
    {
        viewRankMark();
        return;
    }
    if(!_isCompleteRankMarkAction)
    {
        _isCompleteRankMarkAction = true;
        viewRankMark();
        
        _currentStamina = 0;
        _isCurrentBoost = false;
        viewRunningDust();
    }
    
    _lyLane->setPosition(_lyLane->getPosition() + Vec2::LEFT * _curSpeed * dt * 1.6f);
    
    _curSpeed -= 10;
    
    if(_curSpeed <= 0)
    {
        _curSpeed = 0;
        _sprRunningDust->setVisible(false);
        unscheduleUpdate();
        return;
    }
}

#pragma mark - get
const bool LayerEventRaceLane::isRaceComplete()
{
    return _infoHorse->isComplateRace();
}
#pragma mark - init
void LayerEventRaceLane::initVar()
{
    
}

void LayerEventRaceLane::initUi()
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

    _lyLane = LayerColor::create();
    _lyLane->setContentSize(getContentSize());
    _lyLane->setIgnoreAnchorPointForPosition(false);
    _lyLane->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    _lyLane->setPosition(0, getContentSize().height/2);
    clipper->addChild(_lyLane);
    
    _lyHorse = LayerColor::create();
    _lyHorse->setContentSize(getContentSize());
    _lyHorse->setIgnoreAnchorPointForPosition(false);
    _lyHorse->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _lyHorse->setPosition(getContentSize()/2);
    clipper->addChild(_lyHorse);
    
    _lyLobby = LayerColor::create();
    _lyLobby->setContentSize(getContentSize());
    _lyLobby->setIgnoreAnchorPointForPosition(false);
    _lyLobby->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _lyLobby->setPosition(getContentSize()/2);
    clipper->addChild(_lyLobby);
    
//    //
    uiLane();
    uiHorse();
    uiLobby();
}
#pragma mark - UI
void LayerEventRaceLane::uiLane()
{
    _lyLane->removeAllChildren();
    
    auto sprLaneStart = Sprite::create("Assets/ui/race_event/race_bg01_1.png");
    sprLaneStart->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    sprLaneStart->setPosition(0, _lyLane->getContentSize().height/2);
    _lyLane->addChild(sprLaneStart);
    {
        auto sprLine = Sprite::create("Assets/ui/race_event/race_line.png");
        sprLine->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
        sprLine->setPosition(sprLaneStart->getContentSize().width, 9);
        sprLaneStart->addChild(sprLine);
    }
    int nPlat = 10;
    
    Vec2 pos = sprLaneStart->getPosition() + Vec2::RIGHT * sprLaneStart->getContentSize().width;
    for(int i = 0; i < 3; ++i)
    {
        auto sprEarlyLane = Sprite::create(MafUtils::format("Assets/ui/race_event/race_bg01_%d.png", i+2));
        sprEarlyLane->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        sprEarlyLane->setPosition(pos);
        _lyLane->addChild(sprEarlyLane);
        
        if(i == 0)
        {
            auto sprNum = Sprite::create(MafUtils::format("Assets/ui/race_event/rail_num%02d.png", _nIdx));
            sprNum->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            sprNum->setPosition(3, 39);
            sprEarlyLane->addChild(sprNum);
            
            auto sprLine = Sprite::create("Assets/ui/race_event/race_line_thin.png");
            sprLine->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
            sprLine->setPosition(60, 9);
            sprEarlyLane->addChild(sprLine);
        }
        
        auto sprLine = Sprite::create("Assets/ui/race_event/race_line.png");
        sprLine->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
        sprLine->setPosition(sprEarlyLane->getContentSize().width, 9);
        sprEarlyLane->addChild(sprLine);
        
        auto sprPlate = Sprite::create(MafUtils::format("Assets/ui/race_event/race_%dm.png", nPlat));
        sprPlate->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
        sprPlate->setPosition(sprEarlyLane->getContentSize().width - 7, 78);
        sprEarlyLane->addChild(sprPlate);
        
        nPlat += 10;
        pos += Vec2::RIGHT * sprEarlyLane->getContentSize().width;
    }
    
    for(int i = 0; i < 5; ++i)
    {
        auto sprDayTimeLane = Sprite::create("Assets/ui/race_event/race_bg01_5.png");
        sprDayTimeLane->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        sprDayTimeLane->setPosition(pos);
        _lyLane->addChild(sprDayTimeLane);
        
        auto sprLine = Sprite::create("Assets/ui/race_event/race_line.png");
        sprLine->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
        sprLine->setPosition(sprDayTimeLane->getContentSize().width, 9);
        sprDayTimeLane->addChild(sprLine);
        
        auto sprPlate = Sprite::create(MafUtils::format("Assets/ui/race_event/race_%dm.png", nPlat));
        sprPlate->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
        sprPlate->setPosition(sprDayTimeLane->getContentSize().width - 7, 78);
        sprDayTimeLane->addChild(sprPlate);
        
        nPlat += 10;
        pos += Vec2::RIGHT * sprDayTimeLane->getContentSize().width;
    }
    
    for(int i = 0; i < 2; ++i)
    {
        auto sprDayTimeLane = Sprite::create(MafUtils::format("Assets/ui/race_event/race_bg01_%d.png", i+6));
        sprDayTimeLane->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        sprDayTimeLane->setPosition(pos);
        _lyLane->addChild(sprDayTimeLane);
        
        auto sprLine = Sprite::create("Assets/ui/race_event/race_line.png");
        sprLine->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
        sprLine->setPosition(sprDayTimeLane->getContentSize().width, 9);
        sprDayTimeLane->addChild(sprLine);
        
        auto sprPlate = Sprite::create(MafUtils::format("Assets/ui/race_event/race_%dm.png", nPlat));
        sprPlate->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
        sprPlate->setPosition(sprDayTimeLane->getContentSize().width - 7, 78);
        sprDayTimeLane->addChild(sprPlate);
        
        std::string strLinePath = "Assets/ui/race_event/race_line.png";
        if(nPlat == 100)
            strLinePath = "Assets/ui/race_event/race_goalline.png";
        
        if(auto sprPlate = Sprite::create(MafUtils::format("Assets/ui/race_event/race_%dm.png", nPlat)))
        {
            sprPlate->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
            sprPlate->setPosition(sprDayTimeLane->getContentSize().width - 7, 78);
            sprDayTimeLane->addChild(sprPlate);
            
            auto sprLine = Sprite::create(strLinePath);
            sprLine->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
            sprLine->setPosition(sprDayTimeLane->getContentSize().width, 9);
            sprDayTimeLane->addChild(sprLine);
        }
        
        nPlat += 10;
        pos += Vec2::RIGHT * sprDayTimeLane->getContentSize().width;
    }
    
    for(int i = 0; i < 10; ++i)
    {
        auto sprLastLane = Sprite::create("Assets/ui/race_event/race_bg01_8.png");
        sprLastLane->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        sprLastLane->setPosition(pos);
        _lyLane->addChild(sprLastLane);
        
        pos += Vec2::RIGHT * sprLastLane->getContentSize().width;
    }
}
void LayerEventRaceLane::uiHorse()
{
    _lyHorse->removeAllChildren();
    
    _sprSelect = Sprite::create("Assets/ui/race_event/skin_select_target_00.png");
    _sprSelect->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    _sprSelect->setPosition(138, 50);
    _sprSelect->setVisible(_isCurrentSelect);
    _lyHorse->addChild(_sprSelect);
    {
        auto anim = Animation::create();
        anim->setDelayPerUnit(0.1f);
        for(int i = 0; i <= 4; ++i)
        {
            anim->addSpriteFrameWithFile(MafUtils::format("Assets/ui/race_event/skin_select_target_%02d.png", i));
        }
        
        for(int i = 3; i >= 0; i--)
        {
            anim->addSpriteFrameWithFile(MafUtils::format("Assets/ui/race_event/skin_select_target_%02d.png", i));
        }
        auto animate = Animate::create(anim);
        auto repeat = RepeatForever::create(animate);
        _sprSelect->runAction(repeat);
    }
    
    auto nodeBubble = Node::create();
    nodeBubble->setPosition(Vec2::ZERO);
    _lyHorse->addChild(nodeBubble);
    
    _sprCommentBubble = ui::Scale9Sprite::create("Assets/ui/race_event/race_tipbox.png", Rect::ZERO, Rect(29,13,13,50));
    _sprCommentBubble->setContentSize(Size(410, _sprCommentBubble->getContentSize().height));
    _sprCommentBubble->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    _sprCommentBubble->setPosition(260, _lyHorse->getContentSize().height - 20);
    _sprCommentBubble->setOpacity(0);
    nodeBubble->addChild(_sprCommentBubble);
    {
        auto lbComment = Label::createWithTTF("", GAME_FONT, 24);
        lbComment->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        lbComment->setPosition(50, _sprCommentBubble->getContentSize().height/2);
        lbComment->setDimensions(_sprCommentBubble->getContentSize().width - 100, _sprCommentBubble->getContentSize().height - 10);
        lbComment->setAlignment(cocos2d::TextHAlignment::LEFT, cocos2d::TextVAlignment::CENTER);
        lbComment->setOverflow(cocos2d::Label::Overflow::SHRINK);
        lbComment->setOpacity(0);
        lbComment->enableBold();
        lbComment->setColor(Color3B::BLACK);
        _sprCommentBubble->addChild(lbComment);
        //Vec2(280, _lyHorse->getContentSize().height - 20);
        auto func = CallFunc::create([&, lbComment](){
            //10%
            int nRand = random(1, 100);
            if(nRand > 10)
                return;
            
            int nCommentIdx = random(1, 18);
            std::string strComment = GAME_TEXT(MafUtils::format("t_ui_event_race_npc_%02d", nCommentIdx));
            auto objSkin = CostumeSkinManager::getInstance()->getInfoSkin(_infoHorse->getSkinIdx());
            
            if(nCommentIdx == 12 && objSkin != nullptr)
                strComment = GAME_TEXTFORMAT(MafUtils::format("t_ui_event_race_npc_%02d", nCommentIdx), objSkin->getName().c_str());
            
            lbComment->setString(strComment);
        
            auto showAction1 = FadeIn::create(0.1f);
            auto showAction2 = MoveBy::create(0.1f, Vec2(20, 0));
            auto showSpawn = Spawn::create(showAction1, showAction2, nullptr);
            
            auto hideAction1 = FadeOut::create(0.1f);
            auto hideAction2 = MoveBy::create(0.1f, Vec2(-20, 0));
            auto hideSpawn = Spawn::create(hideAction1, hideAction2, nullptr);
            
            auto seq = Sequence::create(showSpawn, DelayTime::create(2.8f), hideSpawn, nullptr);
            _sprCommentBubble->runAction(seq);
            lbComment->runAction(seq->clone());
        });
        auto seq = Sequence::create(func, DelayTime::create(3.3f), nullptr);
        nodeBubble->runAction(RepeatForever::create(seq));
        
    }
    
    _sprDoubleEffect = Sprite::create("Assets/character_hero/normal/effect/effect_aura_1.png");
    _sprDoubleEffect->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _sprDoubleEffect->setPosition(203, -20);
    _sprDoubleEffect->setOpacity(120);
    _sprDoubleEffect->setColor(Color3B(255,252,0));
    _sprDoubleEffect->setVisible(false);
    _lyHorse->addChild(_sprDoubleEffect);
    {
        auto anim = Animation::create();
        anim->setDelayPerUnit(0.1f);
        for(int i = 1; i <= 4; ++i)
        {
            anim->addSpriteFrameWithFile(MafUtils::format("Assets/character_hero/normal/effect/effect_aura_%d.png", i));
        }
        auto animate = Animate::create(anim);
        auto repeat = RepeatForever::create(animate);
        _sprDoubleEffect->runAction(repeat);
        
        if(EventRaceManager::getInstance()->getGameRewardMag() > 1)
            _sprDoubleEffect->setVisible(true);
            
    }
    
    //skin_01_walk_1
    _sprHorse = Sprite::create(MafUtils::format("Assets/character_hero/skin/skin_%02d_walk_1.png", _infoHorse->getSkinIdx()));
    _sprHorse->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    _sprHorse->setPosition(110, _lyHorse->getContentSize().height/2 + 10);
    _sprHorse->setScale(0.75f);
    _lyHorse->addChild(_sprHorse);
    {
        auto anim = Animation::create();
        anim->setDelayPerUnit(0.1f);
        for(int i = 0; i < 4; ++i)
        {
            anim->addSpriteFrameWithFile(MafUtils::format("Assets/character_hero/skin/skin_%02d_walk_%d.png", _infoHorse->getSkinIdx(), i+1));
        }
        auto animate = Animate::create(anim);
        auto repeat = RepeatForever::create(animate);
        _sprHorse->runAction(repeat);
    }
    
    _sprRunningDust = Sprite::create("Assets/ui/race_event/race_smoke1_00.png");
    _sprRunningDust->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    _sprRunningDust->setPosition(_sprHorse->getPosition() + Vec2::RIGHT * 96  + Vec2::DOWN * 17);
    _sprRunningDust->setScale(2.f);
    _sprRunningDust->setVisible(false);
    _lyHorse->addChild(_sprRunningDust);
    {
        auto anim = Animation::create();
        anim->setDelayPerUnit(0.1f);
        for(int i = 0; i <= 3; ++i)
        {
            auto frame = Sprite::create(MafUtils::format("Assets/ui/race_event/race_smoke1_%02d.png",i));
            frame->getTexture()->setTexParameters(texParams);
            anim->addSpriteFrame(frame->getSpriteFrame());
        }
        auto animate = Animate::create(anim);
        auto repeat = RepeatForever::create(animate);
        _sprRunningDust->runAction(repeat);
    }
}
void LayerEventRaceLane::uiLobby()
{
    _lyLobby->removeAllChildren();
    
    auto uiNumber = UIRaceNumber::create(_nIdx, _isCurrentSelect);
    uiNumber->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    uiNumber->setPosition(5, _lyLobby->getContentSize().height/2);
    _lyLobby->addChild(uiNumber);
    
    _sprRankMark = Sprite::create("Assets/ui/race_event/race_rank_1.png");
    _sprRankMark->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    _sprRankMark->setPosition(_lyLobby->getContentSize().width + _sprRankMark->getContentSize().width, 8);
    _sprRankMark->setOpacity(0);
    _lyLobby->addChild(_sprRankMark);
    
    _sprBetBox = ui::Scale9Sprite::create("Assets/ui/race_event/race_coin_count_box.png",Rect::ZERO, Rect(3, 3, 50, 50));
    _sprBetBox->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    _sprBetBox->setContentSize(Size(105, 40));
    _sprBetBox->setPosition(Vec2(-_sprBetBox->getContentSize().width, _lyLobby->getContentSize().height - 5));
    _lyLobby->addChild(_sprBetBox);
    {
        auto sprCoinIcon = Sprite::create("Assets/icon/icon_race_coin.png");
        sprCoinIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        sprCoinIcon->setScale(0.6f);
        sprCoinIcon->setPosition(0, _sprBetBox->getContentSize().height/2);
        _sprBetBox->addChild(sprCoinIcon);
        
        _lbBetCount = ui::CText::create("", GAME_FONT, 30);
        _lbBetCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        _lbBetCount->setPosition(Vec2(_sprBetBox->getContentSize().width - 10, _sprBetBox->getContentSize().height/2));
        _lbBetCount->setTextAreaSize(Size(60, _sprBetBox->getContentSize().height - 10));
        _lbBetCount->setTextVerticalAlignment(cocos2d::TextVAlignment::CENTER);
        _lbBetCount->setTextHorizontalAlignment(cocos2d::TextHAlignment::RIGHT);
        _lbBetCount->setTextOverflow(cocos2d::Label::Overflow::SHRINK);
        _lbBetCount->enableBold();
        _lbBetCount->setColor(Color3B(250,255,205));
        _sprBetBox->addChild(_lbBetCount);
    }
    
    _sprNumber = Sprite::create(MafUtils::format("Assets/ui/race_event/mini_race_rail_number%d.png",_nIdx));
    _sprNumber->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    _sprNumber->setPosition(Vec2(-_sprNumber->getContentSize().width, _lyLobby->getContentSize().height - 10));
    _lyLobby->addChild(_sprNumber);
}
#pragma mark - Utils
void LayerEventRaceLane::viewRankMark()
{
    int rank = _infoHorse->getCurrentRank();
    if(_infoHorse->isComplateRace())
        rank = _infoHorse->getFinalRank();
    
    if(_infoHorse->isComplateRace())
    {
        if(rank > 3)
        {
            _sprRankMark->setTexture(MafUtils::format("Assets/ui/race_event/race_rank_%d.png", rank));
            return;
        }
        
        auto func = CallFunc::create([=](){
            std::string strPath = "Assets/ui/race_event/race_1st.png";
            if(rank == 2)
                strPath = "Assets/ui/race_event/race_2nd.png";
            else if(rank == 3)
                strPath = "Assets/ui/race_event/race_3rd.png";
            
            auto sprFinalRank = Sprite::create(strPath);
            sprFinalRank->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
            sprFinalRank->getTexture()->setTexParameters(texParams);
            sprFinalRank->setPosition(_lyLobby->getContentSize().width + sprFinalRank->getContentSize().width * sprFinalRank->getScale(), 8);
            _lyLobby->addChild(sprFinalRank);
            
            sprFinalRank->runAction(MoveBy::create(0.3f, Vec2::LEFT * (sprFinalRank->getContentSize().width * sprFinalRank->getScale())));
        });
        auto seq = Sequence::create(FadeOut::create(0.1f), func, nullptr);
        _sprRankMark->runAction(seq);
    }
    else
    {
        if(rank == 0)
            return;
        
        _sprRankMark->setTexture(MafUtils::format("Assets/ui/race_event/race_rank_%d.png", rank));
    }
}
void LayerEventRaceLane::viewRunningDust()
{
    std::string strPath = "Assets/ui/race_event/race_smoke1_%02d.png";
    if(_currentStamina <= 0)
        strPath = "Assets/ui/race_event/race_smoke0_%02d.png";
    if(_isCurrentBoost)
        strPath = "Assets/ui/race_event/race_smoke2_%02d.png";
    
    _sprRunningDust->stopAllActions();
    auto anim = Animation::create();
    anim->setDelayPerUnit(0.1f);
    for(int i = 0; i <= 3; ++i)
    {
        auto frame = Sprite::create(MafUtils::format(strPath.c_str(), i));
        frame->getTexture()->setTexParameters(texParams);
        anim->addSpriteFrame(frame->getSpriteFrame());
    }
    auto animate = Animate::create(anim);
    auto repeat = RepeatForever::create(animate);
    _sprRunningDust->runAction(repeat);
        
}

#pragma mark - Event
void LayerEventRaceLane::onEventClickStatus(const int nIdx)
{
    if(_nIdx == nIdx)
    {
        _isCurrentSelect = true;
    }
    else
        _isCurrentSelect = false;
    
    _sprSelect->setVisible(_isCurrentSelect);
}

void LayerEventRaceLane::onEventGameRaedy()
{
    _sprSelect->setVisible(false);
    _sprCommentBubble->setVisible(false);
    _sprNumber->runAction(MoveBy::create(0.2f, Vec2::RIGHT * (_sprNumber->getContentSize().width + 5)));
}

void LayerEventRaceLane::onEventGameStart()
{
    _isGameStart = true;
    _infoHorse = EventRaceManager::getInstance()->getInfoHorse(_nIdx);
    _sprHorse->stopAllActions();
    
    auto anim = Animation::create();
    anim->setDelayPerUnit(0.05f);
    for(int i = 0; i < 4; ++i)
    {
        anim->addSpriteFrameWithFile(MafUtils::format("Assets/character_hero/skin/skin_%02d_walk_%d.png", _infoHorse->getSkinIdx(), i+1));
    }
    auto animate = Animate::create(anim);
    auto repeat = RepeatForever::create(animate);
    _sprHorse->runAction(repeat);
    
    _sprRunningDust->setVisible(true);
    

    std::string strCurCount = MafUtils::doubleToString(_infoHorse->getInvest());
    MafUtils::convertNumberToShort(strCurCount);
    
    if(_infoHorse->getInvest() > 0)
    {
        _lbBetCount->setString(MafUtils::format("%s", strCurCount.c_str()));
        _sprBetBox->runAction(MoveBy::create(0.2f, Vec2::RIGHT * (_sprBetBox->getContentSize().width + _sprNumber->getContentSize().width + 10)));
    }
    
    scheduleUpdate();
}
void LayerEventRaceLane::onEventAds(int)
{
    if(_sprDoubleEffect != nullptr)
    {
        if(EventRaceManager::getInstance()->getGameRewardMag() > 1)
            _sprDoubleEffect->setVisible(true);
        else
            _sprDoubleEffect->setVisible(false);
    }
}
