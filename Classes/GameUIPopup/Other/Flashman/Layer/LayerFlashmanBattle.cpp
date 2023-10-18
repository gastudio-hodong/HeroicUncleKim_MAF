//
//  LayerFlashmanBattle.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 09/01/2020.
//

#include "LayerFlashmanBattle.h"

#include "GameUnit/UnitBehaviorHeroFlashman.h"
#include "GameUnit/UnitBehaviorMonsterFlashman.h"

#include "ManagerGame/FlashmanManager.h"

USING_NS_CC;

LayerFlashmanBattle* LayerFlashmanBattle::create(cocos2d::Size size)
{
    LayerFlashmanBattle* pRet = new(std::nothrow) LayerFlashmanBattle();
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

LayerFlashmanBattle::LayerFlashmanBattle(void) :
_bInitUI(false),

//
_eState(E_FLASHMAN_STATE::None),

_kill(0),
_killFever(0),
_point(0),
_pointFever(0),

_heart(0),
_heartMax(0),

_timer(0),
_timerMax(0),
_timerFever(0),
_timerFeverMax(0),
_timerGodMode(0),

_bFever(false),

_monsterCount(0),

//
_unitHero(nullptr),

//
_uiContentsLobby(nullptr),
_uiContentsBattle(nullptr),
_uiContentsBattleUI(nullptr),
_uiContentsBattleEffectFront(nullptr),
_uiContentsBattleEffectBack(nullptr),

_uiBattleTimerBar(nullptr),
_uiBattleTimer(nullptr),
_uiBattleKillPerSec(nullptr),
_uiBattleKillPerSecIcon(nullptr),
_uiBattlePoint(nullptr),
_uiBattleHeart(nullptr)

{
    
}

LayerFlashmanBattle::~LayerFlashmanBattle(void)
{
    
}

bool LayerFlashmanBattle::init(cocos2d::Size size)
{
    if ( !ui::Layout::init() )
    {
        return false;
    }
    
    //
    setContentSize(size);
    setIgnoreAnchorPointForPosition(false);
    setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    setClippingEnabled(true);
    setTouchEnabled(true);
    
    // init
    initVar();
    initUi();
    
    //
    scheduleUpdate();
    
    return true;
}

void LayerFlashmanBattle::onEnter()
{
    ui::Layout::onEnter();
 
    if ( _bInitUI == false )
    {
        uiContainer();
        uiLobby();
        uiBattle();
        uiBattleUI();
        
        _bInitUI = true;
    }
    
    //
    drawLobby();
    drawBattleUI();
}

void LayerFlashmanBattle::update(float dt)
{
    
}

void LayerFlashmanBattle::refresh(E_REFRESH eType, cocos2d::ValueMap listData)
{

    
}


#pragma mark - init
void LayerFlashmanBattle::initVar()
{
    _listMonster.reserve(20);
}

void LayerFlashmanBattle::initUi()
{
    
}

#pragma mark - ui
void LayerFlashmanBattle::uiContainer()
{
    auto size = getContentSize();
    
    // ui default
    _uiContentsLobby = ui::Layout::create();
    _uiContentsLobby->setContentSize(size);
    _uiContentsLobby->setAnchorPoint(Vec2::ZERO);
    _uiContentsLobby->setPosition(Vec2::ZERO);
    addChild(_uiContentsLobby);
    
    //
    _uiContentsBattle = ui::Layout::create();
    _uiContentsBattle->setContentSize(size);
    _uiContentsBattle->setAnchorPoint(Vec2::ZERO);
    _uiContentsBattle->setPosition(Vec2::ZERO);
    _uiContentsBattle->setVisible(false);
    addChild(_uiContentsBattle);
    
    _uiContentsBattleEffectFront = ui::Layout::create();
    _uiContentsBattleEffectFront->setContentSize(size);
    _uiContentsBattleEffectFront->setAnchorPoint(Vec2::ZERO);
    _uiContentsBattleEffectFront->setPosition(Vec2::ZERO);
    _uiContentsBattleEffectFront->setVisible(false);
    addChild(_uiContentsBattleEffectFront);
    
    _uiContentsBattleUI = ui::Layout::create();
    _uiContentsBattleUI->setContentSize(size);
    _uiContentsBattleUI->setAnchorPoint(Vec2::ZERO);
    _uiContentsBattleUI->setPosition(Vec2::ZERO);
    _uiContentsBattleUI->setVisible(false);
    addChild(_uiContentsBattleUI);
    
    _uiContentsBattleEffectBack = ui::Layout::create();
    _uiContentsBattleEffectBack->setContentSize(size);
    _uiContentsBattleEffectBack->setAnchorPoint(Vec2::ZERO);
    _uiContentsBattleEffectBack->setPosition(Vec2::ZERO);
    _uiContentsBattleEffectBack->setVisible(false);
    addChild(_uiContentsBattleEffectBack);
}

void LayerFlashmanBattle::uiLobby()
{
    auto layout = _uiContentsLobby;
    auto size = layout->getContentSize();
    
    auto uiBG = ui::ImageView::create("Assets/ui/flashman/bsbs_bg_1_1.png");
    uiBG->setAnchorPoint(Vec2::ZERO);
    uiBG->setPosition(Vec2::ZERO);
    uiBG->setScale(size.height / uiBG->getContentSize().height);
    ((ui::Scale9Sprite*)uiBG->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
    layout->addChild(uiBG);
    
    //
    {
        auto hero = UnitBehaviorHeroFlashman::create(E_PLACE::DG_NORMAL);
        hero->setPosition(Size(180, 120));
        hero->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        layout->addChild(hero);
    }
    
    // 무전기
    {
        auto uiWalkietalkie = ui::ImageView::create("Assets/ui/flashman/icon_walkie-talkie.png");
        uiWalkietalkie->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        uiWalkietalkie->setPosition(Vec2(370, 120));
        layout->addChild(uiWalkietalkie);
    }
    
    // 말풍선
    {
        auto uiSpeechBubble = ui::ImageView::create("Assets/ui/flashman/bsbs_textbox.png");
        uiSpeechBubble->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        uiSpeechBubble->setPosition(Vec2(370, 180));
        layout->addChild(uiSpeechBubble);
        
        auto uiText = ui::CText::create(GAME_TEXT("t_ui_minigame_small_talk_1"), GAME_FONT, 24);
        uiText->setPosition(Vec2(uiSpeechBubble->getContentSize().width / 2, uiSpeechBubble->getContentSize().height - 50));
        uiText->setTextAreaSize(Size(212, 92));
        uiText->setTextOverflow(Label::Overflow::SHRINK);
        uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiText->setTextHorizontalAlignment(TextHAlignment::CENTER);
        uiSpeechBubble->addChild(uiText);
        auto action1 = CallFuncN::create([=](Ref* sender){
            
            int idx = random(1, 3);
            std::string textKey = UtilsString::getInstance()->format("t_ui_minigame_small_talk_%d", idx);

            ((ui::CText*)sender)->setString(GAME_TEXT(textKey));
        });
        auto action2 = DelayTime::create(5.0);
        uiText->runAction(RepeatForever::create(Sequence::create(action1, action2, NULL)));
    }
}

void LayerFlashmanBattle::uiBattle()
{
    auto layout = _uiContentsBattle;
    auto size = _uiContentsBattle->getContentSize();
    
    auto uiBG = ui::ImageView::create("Assets/ui/flashman/bsbs_bg_1_1.png");
    uiBG->setAnchorPoint(Vec2::ZERO);
    uiBG->setPosition(Vec2::ZERO);
    uiBG->setScale(size.height / uiBG->getContentSize().height);
    ((ui::Scale9Sprite*)uiBG->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
    layout->addChild(uiBG);
    
    _unitHero = UnitBehaviorHeroFlashman::create(E_PLACE::DG_NORMAL);
    _unitHero->setPosition(Size(110, 120));
    _unitHero->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    layout->addChild(_unitHero);
}

void LayerFlashmanBattle::uiBattleUI()
{
    auto layout = _uiContentsBattleUI;
    auto size = layout->getContentSize();
    
    // timer
    {
        auto uiBarBG = ui::ImageView::create("Assets/ui/flashman/bsbs_time_gauge1_1.png");
        uiBarBG->setPosition(Vec2(size.width / 2, size.height - 35));
        layout->addChild(uiBarBG);
        
        _uiBattleTimerBar = ui::LoadingBar::create("Assets/ui/flashman/bsbs_time_gauge1_2.png");
        _uiBattleTimerBar->setPercent(100);
        _uiBattleTimerBar->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        _uiBattleTimerBar->setPosition(Vec2(uiBarBG->getContentSize().width, uiBarBG->getContentSize().height / 2));
        uiBarBG->addChild(_uiBattleTimerBar);
        
        _uiBattleTimer = ui::CText::create("00:00:00", GAME_FONT_AL, 34);
        _uiBattleTimer->setPosition(Vec2(uiBarBG->getContentSize().width / 2, uiBarBG->getContentSize().height / 2));
        _uiBattleTimer->enableOutline(Color4B::BLACK, 1);
        uiBarBG->addChild(_uiBattleTimer);
        
        auto uiClock = ui::ImageView::create("Assets/ui/prison/clock.png");
        uiClock->setPosition(Vec2(uiBarBG->getContentSize().width, uiBarBG->getContentSize().height / 2));
        uiBarBG->addChild(uiClock, 1);
    }
    
    // per kill
    {
        auto uiBG = ui::ImageView::create("Assets/ui_common/base_text_2_7.png");
        uiBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        uiBG->setPosition(Vec2(0, size.height - 115));
        layout->addChild(uiBG);
        
        _uiBattleKillPerSec = ui::CText::create("0 KILL/S", GAME_FONT_AL, 34);
        _uiBattleKillPerSec->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        _uiBattleKillPerSec->setPosition(Vec2(10, uiBG->getContentSize().height / 2));
        _uiBattleKillPerSec->enableOutline(Color4B::BLACK, 1);
        uiBG->addChild(_uiBattleKillPerSec);
        
        _uiBattleKillPerSecIcon = ui::ImageView::create("Assets/ui/flashman/bsbs_star_1.png");
        _uiBattleKillPerSecIcon->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
        _uiBattleKillPerSecIcon->setPosition(Vec2(uiBG->getContentSize().width - 80, uiBG->getContentSize().height));
        uiBG->addChild(_uiBattleKillPerSecIcon);
    }
    
    // point
    {
        _uiBattlePoint = Label::createWithCharMap("Assets/ui/flashman/bm_number_red.png", 52, 57, '0');
        _uiBattlePoint->setPosition(Vec2(size.width / 2, size.height - 140));
        _uiBattlePoint->setString("86521");
        layout->addChild(_uiBattlePoint);
    }
    
    // heart
    {
        _uiBattleHeart = ui::Layout::create();
        _uiBattleHeart->setContentSize(Size(size.width, 88));
        _uiBattleHeart->setIgnoreAnchorPointForPosition(false);
        _uiBattleHeart->setPosition(Vec2(0, 0));
        _uiBattleHeart->setLayoutType(Type::HORIZONTAL);
        layout->addChild(_uiBattleHeart);
    }
}


#pragma mark - ui : draw
void LayerFlashmanBattle::drawLobby()
{
    auto layout = _uiContentsLobby;
     
}

void LayerFlashmanBattle::drawBattleUI()
{
    auto layout = _uiContentsBattleUI;
    
    double percent = _timer / _timerMax * 100;
    if ( percent > 100 )
        percent = 100;
    
    //
    _uiBattleTimerBar->setPercent(percent);;
    _uiBattleTimer->setString(UtilsDate::getInstance()->getTimeString(_timer, UtilsDate::eType::date_user_type_1));
    _uiBattlePoint->setString(MafUtils::toString(_point.value()));
    _uiBattleKillPerSec->setString(MafUtils::format("%d KILL/S", getMonsterKillPerSec()));
    
    int point = FlashmanManager::getInstance()->getMonsterKillPoint(getMonsterKillPerSec());
    _uiBattleKillPerSecIcon->loadTexture(MafUtils::format("Assets/ui/flashman/bsbs_star_%d.png", point));
    
    // 하트 UI
    for ( int i = 0; i < _heartMax; i++ )
    {
        auto uiHeartBG = _uiBattleHeart->getChildByTag<ui::ImageView*>(i);
        if ( uiHeartBG == nullptr )
        {
            uiHeartBG = ui::ImageView::create("Assets/ui/flashman/bsbs_life_02.png");
            uiHeartBG->setTag(i);
            _uiBattleHeart->addChild(uiHeartBG);
            
            auto params = ui::LinearLayoutParameter::create();
            params->setMargin(ui::Margin(20, 0, 0, 0));
            params->setGravity(ui::LinearLayoutParameter::LinearGravity::CENTER_VERTICAL);
            uiHeartBG->setLayoutParameter(params);
        }
        
        auto uiHeart = uiHeartBG->getChildByName<ui::ImageView*>("UI_HEART");
        if ( uiHeart == nullptr )
        {
            uiHeart = ui::ImageView::create("Assets/ui/flashman/bsbs_life_01.png");
            uiHeart->setPositionType(PositionType::PERCENT);
            uiHeart->setPositionPercent(Vec2::ANCHOR_MIDDLE);
            uiHeart->setVisible(false);
            uiHeart->setName("UI_HEART");
            uiHeartBG->addChild(uiHeart);
        }
        
        if ( _heart > i )
            uiHeart->setVisible(true);
        else
            uiHeart->setVisible(false);
    }
}



#pragma mark - ui : click

#pragma mark - set, get
E_FLASHMAN_STATE LayerFlashmanBattle::getState()
{
    return _eState;
}

int LayerFlashmanBattle::getFeverKill()
{
    return _killFever.valueInt();
}

int LayerFlashmanBattle::getFeverTimer()
{
    return _timerFever;
}

int LayerFlashmanBattle::getFeverTimerMax()
{
    return _timerFeverMax;
}

int LayerFlashmanBattle::getMonsterKillPerSec()
{
    double timeDiff = _timerMax - _timer;
    timeDiff = MIN(timeDiff, _timerMax);
    
    if ( _kill == 0 || timeDiff == 0 )
        return 0;
    
    return _kill / timeDiff;
}

#pragma mark - callback

#pragma mark - schedule
void LayerFlashmanBattle::scheduleBattle(float dt)
{
    std::lock_guard<std::mutex> lock(_mutexBattle);
    
    if ( _eState != E_FLASHMAN_STATE::Battle )
    {
        return;
    }
    
    _timerGodMode -= dt;
    if ( _timerGodMode <= 0 )
    {
        _timerGodMode = 0;
    }
    
    if ( _bFever == true )
    {
        _timerFever -= dt;
        if ( _timerFever <= 0 )
        {
            _timerFever = 0;
            _bFever = false;
            
            // 캐릭터 스킨 변경
            _unitHero->setMotion();
            _unitHero->onWalk();
            
            // 피버 이펙트 삭제
            feverEffectHide();
            feverTimerHide();
        }
        else
        {
            // 피버 이펙트 업데이트
            feverTimerUpdate();
        }
    }
    
    
    _timer -= dt;
    if ( _timer <= 0 )
    {
        _timer = 0;
        
        onBattleFinish();
    }
    
    drawBattleUI();
}

#pragma mark - game
void LayerFlashmanBattle::onBattleWait()
{
    //
    _eState = E_FLASHMAN_STATE::None;
    
    //
    _uiContentsBattle->setVisible(false);
    _uiContentsBattleUI->setVisible(false);
    _uiContentsBattleEffectFront->setVisible(false);
    _uiContentsBattleEffectFront->removeAllChildren();
    _uiContentsBattleEffectBack->setVisible(false);
    _uiContentsBattleEffectBack->removeAllChildren();
}

void LayerFlashmanBattle::onBattleStart()
{    
    //
    _eState = E_FLASHMAN_STATE::Idle;
    
    _kill = 0;
    _killFever = 0;
    _point = 0;
    _pointFever = 0;
    
    _heart = FlashmanManager::getInstance()->getInfoSetting("player_hp")->getValueToInt();
    _heartMax = _heart;
    
    _timer = FlashmanManager::getInstance()->getInfoSetting("time_playset")->getValueToInt();
    _timerMax = _timer;
    _timerFever = 0;
    _timerFeverMax = 0;
    _timerGodMode = 0;
    
    _bFever = false;
    
    _monsterCount = 0;
    
    //
    for (auto& monster : _listMonster)
    {
        monster->removeFromParent();
    }
    _listMonster.clear();

    //
    _uiContentsBattle->setVisible(false);
    _uiContentsBattleUI->setVisible(false);
    _uiContentsBattleEffectFront->setVisible(true);
    _uiContentsBattleEffectFront->removeAllChildren();
    _uiContentsBattleEffectFront->stopAllActions();
    _uiContentsBattleEffectBack->setVisible(true);
    _uiContentsBattleEffectBack->removeAllChildren();
    _uiContentsBattleEffectBack->stopAllActions();
    
    //
    auto action1 = FadeIn::create(0.25);
    auto action2 = CallFunc::create([=](){
        
        _uiContentsBattle->setVisible(true);
        _uiContentsBattleUI->setVisible(true);
        
        // 캐릭터 생성 및 위치 조정
        _unitHero->setVisible(true);
        _unitHero->onWalk();
        
        // 몬스터 생성
        createMonster();
        updateMonsterPosition();
        
        // 몬스터 위치
        for ( int i = 0; i < _listMonster.size(); i++ )
        {
            auto monster = _listMonster.at(i);
            auto pos = _listMonsterPositions.at(i);
            
            monster->setPosition(pos);
            monster->setVisible(true);
        }
    });
    auto action3 = FadeOut::create(0.25);
    auto action4 = CallFunc::create([=](){
        
        auto animation = Animation::create();
        animation->setDelayPerUnit(1.0f);
        animation->addSpriteFrameWithFile("Assets/ui/flashman/bsbs_round_1_1.png");
        animation->addSpriteFrameWithFile("Assets/ui/flashman/bsbs_round_1_2.png");
        animation->addSpriteFrameWithFile("Assets/ui/flashman/bsbs_round_1_3.png");
        animation->addSpriteFrameWithFile("Assets/ui/flashman/bsbs_round_2_1.png");
        
        auto uiCounter = Sprite::create("Assets/ui/flashman/bsbs_round_1_1.png");
        uiCounter->setPosition(Vec2(_uiContentsBattleEffectBack->getContentSize().width / 2, _uiContentsBattleEffectBack->getContentSize().height / 2 - 50));
        uiCounter->stopAllActions();
        uiCounter->runAction(Sequence::create(Animate::create(animation), RemoveSelf::create(), NULL));
        _uiContentsBattleEffectBack->addChild(uiCounter);
    });
    auto action5 = DelayTime::create(3.0);
    auto action6 = CallFunc::create([=](){
        
        // 공격 함수 호출
        onBattleBattle();
    });
    auto seq = Sequence::create(action1, action2, action3, action4, action5, action6, NULL);
    
    //
    auto uiBlack = ui::Layout::create();
    uiBlack->setContentSize(_uiContentsBattleEffectBack->getContentSize());
    uiBlack->setPosition(Vec2::ZERO);
    uiBlack->setBackGroundColor(Color3B::BLACK);
    uiBlack->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    uiBlack->setOpacity(0);
    uiBlack->runAction(seq);
    _uiContentsBattleEffectBack->addChild(uiBlack);
    
    //
    drawLobby();
    drawBattleUI();
}

void LayerFlashmanBattle::onBattleBattle()
{
    //
    _eState = E_FLASHMAN_STATE::Battle;
    
    //
    this->schedule(schedule_selector(LayerFlashmanBattle::scheduleBattle));
}

void LayerFlashmanBattle::onBattleAttack(E_FLASHMAN_TYPE eType)
{
    std::lock_guard<std::mutex> lock(_mutexBattle);
    
    if ( _eState != E_FLASHMAN_STATE::Battle )
    {
        return;
    }
    
    // 몬스터 타입 체크
    auto monster = _listMonster.at(0);
    
    // 피버 타임이 아닐때 검사
    if ( _timerFever <= 0 )
    {
        if ( monster->getType() != E_FLASHMAN_TYPE::Bonus && eType != monster->getType() )
        {
            if ( _timerGodMode <= 0 ) // 무적 상태
            {
                _heart--;
                
                // 0.5초간 무적 상태
                _timerGodMode = 0.5f;
            }
            
            // 종료 여부
            if ( _heart <= 0 )
            {
                onBattleFinish();
            }
            
            // draw
            drawBattleUI();
            
            //
            auto uiBlack = ui::Layout::create();
            uiBlack->setContentSize(_uiContentsBattleEffectFront->getContentSize());
            uiBlack->setPosition(Vec2::ZERO);
            uiBlack->setBackGroundColor(Color3B::RED);
            uiBlack->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
            uiBlack->setOpacity(0);
            uiBlack->runAction(Sequence::create(FadeTo::create(0.1, 150), FadeOut::create(0.1), RemoveSelf::create(), NULL));
            _uiContentsBattleEffectFront->addChild(uiBlack);
            
            //
            auto uiBlackHit = ui::ImageView::create("Assets/ui/flashman/icon_bsbs_mark_01.png");
            uiBlackHit->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
            uiBlackHit->setPosition(Vec2(_unitHero->getPositionX(), _unitHero->getPositionY() + 150));
            uiBlackHit->runAction(Sequence::create(DelayTime::create(0.2), RemoveSelf::create(), NULL));
            _uiContentsBattleEffectFront->addChild(uiBlackHit);
            
            // 공격 모션
            _unitHero->onAttack();
            
            return;
        }
    }
    
    // 몬스터 공격 이펙트
    std::string pathEffect = "";
    switch (monster->getType()) {
        case E_FLASHMAN_TYPE::Red:      pathEffect = "Assets/Particle/flashman/red.plist"; break;
        case E_FLASHMAN_TYPE::Green:    pathEffect = "Assets/Particle/flashman/green.plist"; break;
        case E_FLASHMAN_TYPE::Blue:     pathEffect = "Assets/Particle/flashman/blue.plist"; break;
        case E_FLASHMAN_TYPE::Bonus:    pathEffect = "Assets/Particle/flashman/yellow.plist"; break;
            
            
        default:
            break;
    }
    if ( pathEffect.length() != 0 )
    {
        auto particle = ParticleSystemQuad::create(pathEffect);
        particle->setPosition(Vec2(monster->getPositionX() + monster->getContentSize().width / 2, monster->getPositionY() + monster->getContentSize().height / 2));
        particle->runAction(Sequence::create(DelayTime::create(particle->getLife()), RemoveSelf::create(), NULL));
        _uiContentsBattleEffectFront->addChild(particle);
    }
    
    // 피버 타임이 아닐때 검사
    if ( _timerFever <= 0 )
    {
        // 몬스터 체력 검사
        int hp = monster->getHP() - 1;
        if ( hp > 0 )
        {
            monster->setHP(hp);
            
            // 공격 모션
            _unitHero->onAttack();
            return;
        }
    }
    
    // 킬수 증가
    _kill++;
    
    // 점수 증가
    int point = FlashmanManager::getInstance()->getMonsterKillPoint(getMonsterKillPerSec());
    if ( _timerFever <= 0 || _pointFever <= FlashmanManager::getInstance()->getInfoSetting("fever_max_scroe")->getValueToInt() )
    {
        _point += point;
    }
    
    // 피버 타임일때 획득 점수 증가
    if ( _timerFever > 0 )
    {
        _pointFever += point;
    }
    else
    {
        _pointFever = 0;
    }
    
    // 보너스 시간 증가
    if ( monster->getType() == E_FLASHMAN_TYPE::Bonus )
    {
        _timer += FlashmanManager::getInstance()->getMonsterKillBonusTime();
        _timerMax += FlashmanManager::getInstance()->getMonsterKillBonusTime();
    }
    
    // 피버 타임
    if ( _timerFever <= 0 )
    {
        // 피버 킬수 증가
        _killFever++;
        
        // 피버 타임 적용
        if ( _killFever % FlashmanManager::getInstance()->getFeverCondition() == 0 )
        {
            _timerFever = FlashmanManager::getInstance()->getFeverTime();
            _timerFeverMax = _timerFever;
            
            _bFever = true;
            
            // 캐릭터 스킨 변경
            _unitHero->setMotion(94);
            
            // 피버 컷씬
            feverCut();
            feverEffectShow();
            feverTimerShow();
        }
    }
    else
    {
        _killFever = 0;
    }
    
    // 공격 모션
    _unitHero->onAttack();
    
    // 몬스터 삭제
    monster->removeFromParent();
    _listMonster.erase(0);
    
    // 몬스터 위치 재설정
    updateMonsterPosition();
    
    // 몬스터 이동
    for ( int i = 0; i < _listMonster.size(); i++ )
    {
        auto monster = _listMonster.at(i);
        auto pos = _listMonsterPositions.at(i);
        
        auto action1 = MoveTo::create(0.05, pos);
        monster->stopAllActions();
        monster->runAction(action1);
        monster->setVisible(true);
    }
    
    //
    if ( _listMonster.size() < 15 )
    {
        // 몬스터 생성
        createMonster();
    }
}
 
void LayerFlashmanBattle::onBattleFinish()
{
    //
    _eState = E_FLASHMAN_STATE::Finish;
    
    
    // 캐릭터 스킨 변경
    _unitHero->setMotion();
    
    // 피버 이펙트 삭제
    feverEffectHide();
    feverTimerHide();
    
    //
    auto action1 = FadeIn::create(0.25);
    auto action2 = CallFunc::create([=](){
        
        auto uiCounter = Sprite::create("Assets/ui/flashman/bsbs_round_2_2.png");
        uiCounter->setPosition(Vec2(_uiContentsBattleEffectBack->getContentSize().width / 2, _uiContentsBattleEffectBack->getContentSize().height / 2 - 50));
        _uiContentsBattleEffectBack->addChild(uiCounter);
        
    });
    auto action3 = FadeOut::create(0.25);
    auto action4 = CallFunc::create([=](){
        //
        _eState = E_FLASHMAN_STATE::None;
        
        //
        FlashmanManager::getInstance()->requestResult(true, _point.valueInt());
    });
    auto seq = Sequence::create(action1, action2, action3, action4, NULL);
    this->runAction(seq);
    
    //
    drawLobby();
    drawBattleUI();
    
    //
    this->unschedule(schedule_selector(LayerFlashmanBattle::scheduleBattle));
}

void LayerFlashmanBattle::createMonster()
{
    int monsterGenerateCount = FlashmanManager::getInstance()->getMonsterGenerateCount();
    int monsterGenerateBossCount = FlashmanManager::getInstance()->getMonsterGenerateBossCount();
    int monsterGenerateBonusPos = FlashmanManager::getInstance()->getMonsterGenerateBonusPos();
    
    // 보스 몬스터 위치 설정
    std::vector<int> listBossPosTotal;
    std::vector<int> listBossPos;
    for ( int i = 1; i <= monsterGenerateCount; i++ )
    {
        if ( i == monsterGenerateBonusPos )
            continue;
        
        listBossPosTotal.push_back(i);
    }
    
    std::sample(listBossPosTotal.begin(), listBossPosTotal.end(), std::back_inserter(listBossPos), monsterGenerateBossCount, std::mt19937{std::random_device{}()});
    
    // 몬스터 생성
    for ( int i = 1; i <= monsterGenerateCount; i++ )
    {
        bool bBoss = false;
        int hp = 1;
        
        if ( find(listBossPos.begin(), listBossPos.end(), i) != listBossPos.end())
        {
            bBoss = true;
            hp =  FlashmanManager::getInstance()->getMonsterBossHp();
        }
        
        E_FLASHMAN_TYPE eType = E_FLASHMAN_TYPE::None;
        if ( i == monsterGenerateBonusPos )
        {
            eType = E_FLASHMAN_TYPE::Bonus;
        }
        else
        {
            eType = (E_FLASHMAN_TYPE)random(1, 3);
        }
        
        auto unitMonster = UnitBehaviorMonsterFlashman::create();
        unitMonster->setMotion(eType, bBoss);
        unitMonster->setHP(hp);
        unitMonster->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        unitMonster->setPosition(Vec2(2000, 120));
        unitMonster->setTag(_monsterCount.valueInt());
        unitMonster->setVisible(false);
        _uiContentsBattle->addChild(unitMonster, 200000 - _monsterCount.valueInt());
        
        _listMonster.pushBack(unitMonster);
        
        // 생성된 몬스터 수 증가
        _monsterCount++;
    }
    
}

void LayerFlashmanBattle::updateMonsterPosition()
{
    double posX = 240;
    double posY = 120;
    
    _listMonsterPositions.clear();
    for ( auto& monster : _listMonster )
    {
        Vec2 pos = Vec2(posX, posY);
        _listMonsterPositions.push_back(pos);
        
        // pos add x
        posX += (monster->getContentSize().width * 0.8);
    }
}

void LayerFlashmanBattle::feverCut()
{
    auto layout = _uiContentsBattleEffectBack;
    
    auto uiCut = ui::ImageView::create("Assets/ui/flashman/bsbs_fever_popup.png");
    uiCut->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    uiCut->setPosition(Vec2(-50, -50));
    uiCut->runAction(Sequence::create(MoveBy::create(0.5, Vec2(50, 50)), RemoveSelf::create(), NULL));
    layout->addChild(uiCut);
}

void LayerFlashmanBattle::feverEffectShow()
{
    auto layout = _uiContentsBattleEffectFront;
    auto size = layout->getContentSize();
    
    auto animation = Animation::create();
    animation->setDelayPerUnit(0.1f);
    for ( int i = 1; i <= 6; i++ )
    {
        animation->addSpriteFrameWithFile(MafUtils::format("Assets/ui/flashman/bsbs_fever_bg1_%d.png", i));
    }
    auto uiFeverEffect = ui::ImageView::create("Assets/ui/flashman/bsbs_fever_bg1_1.png");
    uiFeverEffect->setPosition(Vec2(size.width / 2, size.height / 2));
    uiFeverEffect->getVirtualRenderer()->stopAllActions();
    uiFeverEffect->getVirtualRenderer()->runAction(RepeatForever::create(Animate::create(animation)));
    uiFeverEffect->setName("UI_EFFECT");
    layout->addChild(uiFeverEffect);
}

void LayerFlashmanBattle::feverEffectHide()
{
    auto layout = _uiContentsBattleEffectFront;
    
    layout->removeChildByName("UI_EFFECT");
}

void LayerFlashmanBattle::feverTimerShow()
{
    auto layout = _uiContentsBattleEffectBack;
    auto size = layout->getContentSize();
    
    if ( _timerFever <= 0 )
    {
        return;
    }
    
    // 피버 타임 UI
    if ( _timerFever <= 5 )
    {
        int count = (int)(_timerFever) + 1;
        
        auto uiFeverCount = ui::ImageView::create(MafUtils::format("Assets/ui/flashman/bsbs_fever_ed_%d.png", count));
        uiFeverCount->setPosition(Vec2(size.width / 2, size.height / 2 - 50));
        uiFeverCount->setName("UI_FEVER_COUNT");
        layout->addChild(uiFeverCount);
    }
    
    auto uiFeverTimerBarBG = ui::ImageView::create("Assets/ui/flashman/bsbs_progressbar_2_1.png");
    uiFeverTimerBarBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    uiFeverTimerBarBG->setPosition(Vec2(size.width / 2, size.height / 2 - 50 + 80));
    uiFeverTimerBarBG->setName("UI_FEVER_BAR_BG");
    layout->addChild(uiFeverTimerBarBG);
    
    auto uiFeverTimerBar = ui::LoadingBar::create("Assets/ui/flashman/bsbs_progressbar_2_2.png");
    uiFeverTimerBar->setPosition(Vec2(uiFeverTimerBarBG->getContentSize().width / 2, uiFeverTimerBarBG->getContentSize().height / 2));
    uiFeverTimerBar->setPercent(100);
    uiFeverTimerBar->setName("UI_FEVER_BAR");
    uiFeverTimerBarBG->addChild(uiFeverTimerBar);
}

void LayerFlashmanBattle::feverTimerUpdate()
{
    auto layout = _uiContentsBattleEffectBack;
    
    if ( _timerFever <= 0 )
    {
        return;
    }
    
    //
    if ( _timerFever <= 5 )
    {
        int count = (int)(_timerFever) + 1;
        
        auto uiFeverCount = layout->getChildByName<ui::ImageView*>("UI_FEVER_COUNT");
        uiFeverCount->loadTexture(MafUtils::format("Assets/ui/flashman/bsbs_fever_ed_%d.png", count));
    }
    
    //
    double percent = _timerFever / _timerFeverMax * 100;
    if ( percent > 100 )
        percent = 100;
    
    auto uiFeverTimerBar = (ui::LoadingBar*)utils::findChild(layout, "UI_FEVER_BAR");
    uiFeverTimerBar->setPercent(percent);
}

void LayerFlashmanBattle::feverTimerHide()
{
    auto layout = _uiContentsBattleEffectBack;
    
    layout->removeChildByName("UI_FEVER_COUNT");
    layout->removeChildByName("UI_FEVER_BAR_BG");
}
