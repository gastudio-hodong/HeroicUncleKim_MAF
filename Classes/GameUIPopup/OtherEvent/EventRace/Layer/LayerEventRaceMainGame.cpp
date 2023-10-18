//
//  LayerEventRaceMainGame.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/07/22.
//

#include "LayerEventRaceMainGame.h"

#include "GameUIPopup/OtherEvent/EventRace/UI/UIRaceStatus.h"
#include "GameUIPopup/OtherEvent/EventRace/PopupEventRaceReward.h"

#include "ManagerEvent/EventRaceManager.h"

USING_NS_CC;

LayerEventRaceMainGame* LayerEventRaceMainGame::create(const cocos2d::Size size)
{
    LayerEventRaceMainGame* pRet = new(std::nothrow) LayerEventRaceMainGame();
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

LayerEventRaceMainGame::LayerEventRaceMainGame(void):
_nCurrentStatusIdx(1)
,_lyLane(nullptr)
{
    _listLane.clear();
}

LayerEventRaceMainGame::~LayerEventRaceMainGame(void)
{
    _listLane.clear();
    EventRaceManager::getInstance()->unSubRaceReadyEvent(this);
    EventRaceManager::getInstance()->unSubRaceStartEvent(this);
    EventRaceManager::getInstance()->unSubStatusEvent(this);
}

bool LayerEventRaceMainGame::init(const cocos2d::Size size)
{
    if (!LayerColor::initWithColor(Color4B::RED))
    {
        return false;
    }
    setContentSize(size);
    setIgnoreAnchorPointForPosition(false);
    
    EventRaceManager::getInstance()->subRaceReadyEvent(new MafFunction<void()>(CC_CALLBACK_0(LayerEventRaceMainGame::onEventClickReady, this), this));
    EventRaceManager::getInstance()->subRaceStartEvent(new MafFunction<void()>(CC_CALLBACK_0(LayerEventRaceMainGame::onEventClickStart, this), this));
    EventRaceManager::getInstance()->subStatusEvent(new MafFunction<void(int)>(CC_CALLBACK_1(LayerEventRaceMainGame::onEventClickStatus, this), this));
    
    // init
    initVar();
    initUi();
    
    return true;
}

#pragma mark - init
void LayerEventRaceMainGame::initVar()
{
    
}

void LayerEventRaceMainGame::initUi()
{
    _lyLane = LayerColor::create();
    _lyLane->setContentSize(getContentSize());
    _lyLane->setIgnoreAnchorPointForPosition(false);
    _lyLane->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _lyLane->setPosition(getContentSize()/2);
    addChild(_lyLane);
    
//    //
    uiLane();
}
void LayerEventRaceMainGame::update(float dt)
{
    int nCompleteCount = 0;
    
    EventRaceManager::getInstance()->setCurrentRank();
    for(auto obj : _listLane)
    {
        if(obj->isRaceComplete())
        {
            nCompleteCount++;
            continue;
        }
        obj->race(dt);
    }
    
    if(nCompleteCount >= _listLane.size())
    {
        unscheduleUpdate();
        onActionFinish();
        return;
    }
}
#pragma mark - UI
void LayerEventRaceMainGame::uiLane()
{
    _lyLane->removeAllChildren();
    _listLane.clear();
    
    float posY = _lyLane->getContentSize().height;
    for(int i = 0; i < 6; ++i)
    {
        auto uiLane = LayerEventRaceLane::create(Size(750, 142), i+1);
        uiLane->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        uiLane->setPosition(_lyLane->getContentSize().width/2, posY);
        _lyLane->addChild(uiLane);
        _listLane.pushBack(uiLane);
        posY -= uiLane->getContentSize().height;
    }
    
    auto uiStatus = UIRaceStatus::create(_nCurrentStatusIdx);
    uiStatus->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    uiStatus->setPosition(getContentSize().width, getContentSize().height/2);
    addChild(uiStatus);
}

#pragma mark - Click
void LayerEventRaceMainGame::onClickStatus(Ref* const sender)
{
    
}

#pragma mark - Event
void LayerEventRaceMainGame::onEventClickStatus(const int nIdx)
{
    if(_nCurrentStatusIdx == nIdx)
        return;
    
    _nCurrentStatusIdx = nIdx;
    
    if(_nCurrentStatusIdx == 0)
        return;
    
    auto uiStatus = UIRaceStatus::create(_nCurrentStatusIdx);
    uiStatus->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    uiStatus->setPosition(getContentSize().width, getContentSize().height/2);
    addChild(uiStatus);
}

void LayerEventRaceMainGame::onEventClickReady()
{
    auto sprCast = Sprite::create("Assets/ui/dialog/imo_caster_normal.png");
    sprCast->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    sprCast->setPosition(getContentSize().width + sprCast->getContentSize().width * sprCast->getScale(), 0);
    sprCast->getTexture()->setTexParameters(texParams);
    sprCast->setScale(3);
    sprCast->setOpacity(0);
    sprCast->setFlippedX(true);
    addChild(sprCast);
    
    auto sprSpeechBubble = Sprite::create("Assets/ui/race_event/race_start1.png");
    sprSpeechBubble->setAnchorPoint(Vec2(0.78f, 0));
    sprSpeechBubble->setPosition(Vec2(getContentSize().width - 243, 281));
    sprSpeechBubble->getTexture()->setTexParameters(texParams);
    sprSpeechBubble->setVisible(false);
    addChild(sprSpeechBubble);
    
    
    auto funcStep1 = CallFunc::create([&, sprSpeechBubble, sprCast]{
        sprSpeechBubble->setVisible(true);
        
        auto funcStep2 = CallFunc::create([&, sprSpeechBubble, sprCast]{
            sprSpeechBubble->setTexture("Assets/ui/race_event/race_start2.png");
            
            auto funcStep3 = CallFunc::create([&, sprSpeechBubble, sprCast]{
                sprSpeechBubble->setTexture("Assets/ui/race_event/race_start3.png");
                sprCast->setTexture("Assets/ui/dialog/imo_caster_start.png");
                sprCast->getTexture()->setTexParameters(texParams);
                
                auto seq = Sequence::create(ScaleTo::create(0.1f, 2), DelayTime::create(0.5f), Spawn::create(ScaleTo::create(0.1f, 1), FadeOut::create(0.1f),nullptr), nullptr);
                sprSpeechBubble->runAction(seq);
                
                SoundManager::SoundEffectStart(SOUND_PISTOL_SHOT, E_SOUND_TYPE::CONTENTS);
                auto funcStep4 = CallFunc::create([=](){
                    //레이스 연출 끝난 후 실제 시작 시키는 이벤트
                    EventRaceManager::getInstance()->invokeRaceStartEvent();
                });
                sprCast->runAction(Sequence::create(DelayTime::create(0.5f), FadeOut::create(0.1f), funcStep4, nullptr));
            });
            
            auto seq = Sequence::create(ScaleTo::create(0.1f, 2), DelayTime::create(1.f), ScaleTo::create(0.1f, 1), funcStep3, nullptr);
            sprSpeechBubble->runAction(seq);
        });
        
        auto seq = Sequence::create(ScaleTo::create(0.1f, 2), DelayTime::create(0.5f), ScaleTo::create(0.1f, 1), funcStep2, nullptr);
        sprSpeechBubble->runAction(seq);
    });
    
    auto move = MoveTo::create(0.2f, Vec2(getContentSize().width, 0));
    auto action1 = Spawn::create(move, FadeIn::create(0.3f), nullptr);
    sprCast->runAction(Sequence::create(action1, DelayTime::create(0.3f), funcStep1, nullptr));
    
}
void LayerEventRaceMainGame::onActionFinish()
{
    auto sprFinish = Sprite::create("Assets/ui/race_event/race_finish.png");
    sprFinish->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    sprFinish->setPosition(Vec2(getContentSize().width + sprFinish->getContentSize().width, getContentSize().height/2));
    sprFinish->setScale(2.f);
    sprFinish->getTexture()->setTexParameters(texParams);
    addChild(sprFinish);
    
    auto move1 = MoveTo::create(0.3f, Vec2(getContentSize().width/2, getContentSize().height/2));
    auto move2 = MoveTo::create(0.3f, Vec2(-sprFinish->getContentSize().width, getContentSize().height/2));
    auto func = CallFunc::create([=](){
        auto popup = PopupEventRaceReward::create();
        popup->show();
    });
    auto action1 = Sequence::create(move1, DelayTime::create(0.3f), Sequence::create(ScaleTo::create(0.2f, 2.5), ScaleTo::create(0.2f, 2), nullptr), DelayTime::create(0.3f), move2, func, nullptr);
    sprFinish->runAction(action1);
}
void LayerEventRaceMainGame::onEventClickStart()
{
    scheduleUpdate();
}
void LayerEventRaceMainGame::onEventClickReset()
{
    unscheduleUpdate();
    uiLane();
}
