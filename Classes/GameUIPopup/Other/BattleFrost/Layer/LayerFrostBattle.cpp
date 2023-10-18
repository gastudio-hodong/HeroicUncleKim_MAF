//
//  LayerFrostBattle.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 09/01/2020.
//

#include "LayerFrostBattle.h"

#include "Common/ConfigGameNormal.h"

#include "GameUnit/UnitBehaviorHeroFrost.h"
#include "GameUnit/UnitBehaviorMonsterFrost.h"

#include "ManagerGame/PrisonFrostManager.h"

USING_NS_CC;

LayerFrostBattle* LayerFrostBattle::create(cocos2d::Size size)
{
    LayerFrostBattle* pRet = new(std::nothrow) LayerFrostBattle();
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

LayerFrostBattle::LayerFrostBattle(void) :
_bInitEnter(false),

_eState(E_STATE::None),

_damageTotal("0"),

_bAnimation(true),
_bResult(false),

//
_unitHero(nullptr),
_unitMonster(nullptr),
_unitDoor(nullptr),

//
_uiContentsLobby(nullptr),

_uiContentsBattle(nullptr),
_uiContentsBattleUI(nullptr),
_uiContentsBattleEffect(nullptr)

{
    
}

LayerFrostBattle::~LayerFrostBattle(void)
{
    
}

bool LayerFrostBattle::init(cocos2d::Size size)
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

void LayerFrostBattle::onEnter()
{
    ui::Layout::onEnter();
 
    if ( _bInitEnter == false )
    {
        _bInitEnter = true;
        
        uiContainer();
        uiLobby();
        uiBattle();
        uiBattleUI();
    }
    
    //
    drawLobby();
    drawBattleUI();
}

void LayerFrostBattle::update(float dt)
{
    
}

void LayerFrostBattle::refresh(E_REFRESH eType, cocos2d::ValueMap listData)
{
    if ( eType == E_REFRESH::COSTUME )
    {
        
    }
}


#pragma mark - init
void LayerFrostBattle::initVar()
{
    
}

void LayerFrostBattle::initUi()
{
    
}

#pragma mark - ui
void LayerFrostBattle::uiContainer()
{
    auto size = getContentSize();
    
    // ui default
    _uiContentsLobby = ui::Layout::create();
    _uiContentsLobby->setContentSize(size);
    _uiContentsLobby->setAnchorPoint(Vec2::ZERO);
    _uiContentsLobby->setPosition(Vec2::ZERO);
    addChild(_uiContentsLobby);
    
    //
    _uiContentsBattle = ui::ScrollView::create();
    _uiContentsBattle->setContentSize(size);
    _uiContentsBattle->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    _uiContentsBattle->setPosition(Vec2(0, 0));
    _uiContentsBattle->setDirection(ui::ScrollView::Direction::HORIZONTAL);
    _uiContentsBattle->setScrollBarEnabled(false);
    _uiContentsBattle->setBounceEnabled(false);
    _uiContentsBattle->setInertiaScrollEnabled(false);
    _uiContentsBattle->setVisible(false);
    _uiContentsBattle->setTouchEnabled(false);
    addChild(_uiContentsBattle);
    
    _uiContentsBattleUI = ui::Layout::create();
    _uiContentsBattleUI->setContentSize(size);
    _uiContentsBattleUI->setAnchorPoint(Vec2::ZERO);
    _uiContentsBattleUI->setPosition(Vec2::ZERO);
    _uiContentsBattleUI->setVisible(false);
    addChild(_uiContentsBattleUI);
    
    _uiContentsBattleEffect = ui::Layout::create();
    _uiContentsBattleEffect->setContentSize(size);
    _uiContentsBattleEffect->setAnchorPoint(Vec2::ZERO);
    _uiContentsBattleEffect->setPosition(Vec2::ZERO);
    _uiContentsBattleEffect->setVisible(false);
    addChild(_uiContentsBattleEffect);
}

void LayerFrostBattle::uiLobby()
{
    auto layout = _uiContentsLobby;
    auto size = layout->getContentSize();
    
    auto uiBG = ui::ImageView::create("Assets/battle_bg/prison/bg_frost-dungeon_1.png");
    uiBG->setAnchorPoint(Vec2::ZERO);
    uiBG->setPosition(Vec2::ZERO);
    uiBG->setScale(size.height / uiBG->getContentSize().height);
    ((ui::Scale9Sprite*)uiBG->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
    layout->addChild(uiBG);
    
    // 간판
    {
        auto uiBoardFloor = ui::ImageView::create("Assets/ui/frost_prison/tower_signs.png");
        uiBoardFloor->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        uiBoardFloor->setPosition(Vec2(150*4, 55*4));
        uiBoardFloor->setName("UI_FLOOR");
        layout->addChild(uiBoardFloor);
        {
            {
                auto action1 = DelayTime::create(1.0);
                auto action2 = CallFuncN::create([=](Ref* sender){
                    
                    int floor = PrisonFrostManager::getInstance()->getClearFloor() + 1;
                    ((ui::CText*)sender)->setString(UtilsString::getInstance()->format("%dF", floor));
                });
                auto seq = Sequence::create(action1, action2, NULL);
                
                auto uiFloor = ui::CText::create("99F", GAME_FONT_AL, 34);
                uiFloor->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
                uiFloor->setPosition(Vec2(uiBoardFloor->getContentSize().width / 2, uiBoardFloor->getContentSize().height - 50));
                uiFloor->setTextColor(Color4B(210, 245, 255, 255));
                uiFloor->runAction(RepeatForever::create(seq));
                uiFloor->setName("UI_TEXT_FLOOR");
                uiBoardFloor->addChild(uiFloor);
            }
            
            {
                auto action1 = DelayTime::create(1.0);
                auto action2 = CallFuncN::create([=](Ref* sender){
                    
                    std::string hp = "0";
                    
                    int floor = PrisonFrostManager::getInstance()->getClearFloor() + 1;
                    auto objFloor = PrisonFrostManager::getInstance()->getInfoFloor(floor);
                    if ( objFloor != nullptr )
                    {
                        hp = objFloor->getHP();
                        hp = MafUtils::convertNumberToShort(hp);
                    }
                    
                    ((ui::CText*)sender)->setString(UtilsString::getInstance()->format("(%s)", hp.c_str()));
                });
                auto seq = Sequence::create(action1, action2, NULL);
                
                auto uiHp = ui::CText::create("(0)", GAME_FONT_AL, 24);
                uiHp->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
                uiHp->setPosition(Vec2(uiBoardFloor->getContentSize().width / 2, uiBoardFloor->getContentSize().height - 50));
                uiHp->setTextColor(Color4B(210, 245, 255, 255));
                uiHp->runAction(RepeatForever::create(seq));
                uiHp->setName("UI_TEXT_HP");
                uiBoardFloor->addChild(uiHp);
            }
        }
    }
    
    // 애니메이션 : 모닥불
    {//110x30
        auto animation = Animation::create();
        animation->setDelayPerUnit(0.1);
        for ( int i = 1; i <= 6; i++ )
        {
            auto spriteAni = Sprite::create(MafUtils::format("Assets/ui/frost_prison/wood_fire%02d.png",i).c_str());
            spriteAni->getTexture()->setTexParameters(texParams);
            animation->addSpriteFrame(spriteAni->getSpriteFrame());
        }
        
        auto uiAni = ui::ImageView::create("Assets/ui/frost_prison/wood_fire01.png");
        uiAni->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        uiAni->setPosition(Vec2(110*4, 22*4));
        uiAni->getVirtualRenderer()->stopAllActions();
        uiAni->getVirtualRenderer()->runAction(RepeatForever::create(Animate::create(animation)));
        ((ui::Scale9Sprite*)uiAni->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
        uiAni->setScale(3);
        layout->addChild(uiAni, 1000);
    }
    
    // 애니메이션 : 늙은이
    {
        auto animation = Animation::create();
        animation->setDelayPerUnit(0.1);
        for ( int i = 1; i <= 6; i++ )
        {
            auto spriteAni = Sprite::create(MafUtils::format("Assets/ui/frost_prison/mamon_waiting_ani%02d.png",i).c_str());
            spriteAni->getTexture()->setTexParameters(texParams);
            animation->addSpriteFrame(spriteAni->getSpriteFrame());
        }
        
        auto uiAni = ui::ImageView::create("Assets/ui/frost_prison/mamon_waiting_ani01.png");
        uiAni->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
        uiAni->setPosition(Vec2(85*4, 24*4));
        uiAni->getVirtualRenderer()->stopAllActions();
        uiAni->getVirtualRenderer()->runAction(RepeatForever::create(Animate::create(animation)));
        ((ui::Scale9Sprite*)uiAni->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
        uiAni->setScale(3);
        layout->addChild(uiAni, 1000);
    }
    
    // 말풍선
    {
        auto uiSpeechBubble = ui::ImageView::create("Assets/ui/frost_prison/tower_speech_bubble.png");
        uiSpeechBubble->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
        uiSpeechBubble->setPosition(Vec2(272, 272));
        layout->addChild(uiSpeechBubble);
        {
            
            auto action1 = CallFuncN::create([=](Ref* sender){
                
                int idx = random(1, 3);
                std::string textKey = UtilsString::getInstance()->format("t_ui_frost_prison_small_talk_%d", idx);

                ((ui::CText*)sender)->setString(GAME_TEXT(textKey));
            });
            auto action2 = DelayTime::create(5.0);
            auto seq = Sequence::create(action1, action2, NULL);
            
            auto uiText = ui::CText::create("", GAME_FONT, 24);
            uiText->setPosition(Vec2(uiSpeechBubble->getContentSize().width / 2, uiSpeechBubble->getContentSize().height - 57));
            uiText->setTextAreaSize(Size(222 - 10, 108-10));
            uiText->setTextOverflow(Label::Overflow::SHRINK);
            uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
            uiText->setTextHorizontalAlignment(TextHAlignment::CENTER);
            uiText->setTextColor(Color4B::BLACK);
            uiText->runAction(RepeatForever::create(seq));
            uiSpeechBubble->addChild(uiText);
        }
    }
}

void LayerFrostBattle::uiBattle()
{
    auto layout = _uiContentsBattle;
    auto sizeView = _uiContentsBattle->getContentSize();
    auto sizeContainer = Size::ZERO;
    
    // map
    {
        double posX = 0;
        double posY = 0;
        double width = 0;
        double height = 0;
        
        std::vector<std::string> listPath = {"Assets/battle_bg/prison/bg_frost-dungeon_1.png","Assets/battle_bg/prison/bg_frost-dungeon_2.png"};
        for ( int i = 0; i < listPath.size(); i++ )
        {
            auto uiBG = ui::ImageView::create(listPath[i]);
            uiBG->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
            uiBG->setPosition(Vec2(posX, posY));
            ((ui::Scale9Sprite*)uiBG->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
            uiBG->setScale(4);
            layout->addChild(uiBG);
            
            posX += uiBG->getBoundingBox().size.width;
            width = posX;
            height = uiBG->getBoundingBox().size.height;
        }
    
        layout->setInnerContainerSize(Size(width, height));
        sizeContainer = layout->getInnerContainerSize();
    }
    
    // 말풍선
    {
        auto uiBubbleSpeech = ui::ImageView::create("Assets/ui/frost_prison/tower_speech_bubble_revers.png");
        uiBubbleSpeech->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        uiBubbleSpeech->setPosition(Vec2(450, 272));
        uiBubbleSpeech->setName("UI_BUBBLE_SPEECH");
        uiBubbleSpeech->setVisible(false);
        layout->addChild(uiBubbleSpeech);
        {
            auto uiText = ui::CText::create("", GAME_FONT, 24);
            uiText->setPosition(Vec2(uiBubbleSpeech->getContentSize().width / 2, uiBubbleSpeech->getContentSize().height - 57));
            uiText->setTextAreaSize(Size(222 - 10, 108-10));
            uiText->setTextOverflow(Label::Overflow::SHRINK);
            uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
            uiText->setTextHorizontalAlignment(TextHAlignment::CENTER);
            uiText->setTextColor(Color4B::BLACK);
            uiText->setName("UI_TEXT");
            uiBubbleSpeech->addChild(uiText);
        }
        
        auto uiBubbleMark = ui::ImageView::create("Assets/ui/frost_prison/tower_speech_bubble_surprise.png");
        uiBubbleMark->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        uiBubbleMark->setPosition(Vec2(450, 272));
        uiBubbleMark->setName("UI_BUBBLE_MARK");
        uiBubbleMark->setVisible(false);
        layout->addChild(uiBubbleMark);
    }
    
    _unitMonster = UnitBehaviorMonsterFrost::create(E_PLACE::DG_NORMAL);
    _unitMonster->setPosition(Size(105*4, 138));
    _unitMonster->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    layout->addChild(_unitMonster);
    
    _unitHero = UnitBehaviorHeroFrost::create(E_PLACE::DG_NORMAL);
    _unitHero->setPosition(Size(70*4, 138));
    _unitHero->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    layout->addChild(_unitHero);
    
    _unitDoor = Sprite::create("Assets/ui/frost_prison/bg_frost_dungeon_door01.png");
    _unitDoor->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    _unitDoor->setPosition(Vec2(sizeContainer.width, sizeContainer.height / 2));
    _unitDoor->setScale(4);
    _unitDoor->getTexture()->setTexParameters(texParams);
    layout->addChild(_unitDoor);
}

void LayerFrostBattle::uiBattleUI()
{
    auto layout = _uiContentsBattleUI;
    auto size = layout->getContentSize();
    
    // timer
    {
        auto uiBarGauge = Sprite::create("Assets/ui/frost_prison/time_gauge_green.png");
        uiBarGauge->setContentSize(Size(254, 15));
        
        auto uiBarBG = ui::ImageView::create("Assets/ui/frost_prison/time_gauge_box.png");
        uiBarBG->setScale9Enabled(true);
        uiBarBG->setContentSize(Size(260, 23));
        uiBarBG->setPosition(Vec2(size.width / 2, size.height - 100));
        layout->addChild(uiBarBG);
        
        auto uiBar = ProgressTimer::create(uiBarGauge);
        uiBar->setType(ProgressTimer::Type::BAR);
        uiBar->setPercentage(100);
        uiBar->setMidpoint(Vec2(0,0.5));
        uiBar->setBarChangeRate(Vec2(1,0));
        uiBar->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        uiBar->setPosition(Vec2(uiBarBG->getContentSize().width, uiBarBG->getContentSize().height / 2));
        uiBar->setName("UI_TIMER");
        uiBarBG->addChild(uiBar);
        
        auto uiClock = ui::ImageView::create("Assets/ui/prison/clock.png");
        uiClock->setPosition(Vec2(0, uiBarBG->getContentSize().height / 2));
        uiClock->setScale(0.9);
        uiBarBG->addChild(uiClock, 1);
    }
    
    _uiBattleAuto = ui::ImageView::create("Assets/ui/prison/icon_auto1_2.png");
    _uiBattleAuto->setPosition(Vec2(size.width - 50, size.height - 50));
    _uiBattleAuto->setVisible(false);
    layout->addChild(_uiBattleAuto);
    {
        auto uiBtnAuto = ui::Button::create("Assets/ui/prison/icon_auto1_1.png");
        uiBtnAuto->setPosition(Vec2(_uiBattleAuto->getContentSize().width / 2, _uiBattleAuto->getContentSize().height / 2));
        uiBtnAuto->addClickEventListener(CC_CALLBACK_1(LayerFrostBattle::onClickAuto, this));
        uiBtnAuto->runAction(RepeatForever::create(RotateBy::create(1.0, -180)));
        uiBtnAuto->setName("UI_BTN_AUTO");
        _uiBattleAuto->addChild(uiBtnAuto, -1);
        
        auto uiTextCount = ui::CText::create("0/0", GAME_FONT, 20);
        uiTextCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        uiTextCount->setPosition(Vec2(_uiBattleAuto->getContentSize().width / 2, -10));
        uiTextCount->enableOutline(Color4B::BLACK, 2);
        uiTextCount->setName("UI_TEXT_COUNT");
        _uiBattleAuto->addChild(uiTextCount);
    }
}


#pragma mark - ui : draw
void LayerFrostBattle::drawLobby()
{
    auto layout = _uiContentsLobby;
    
    // floor
    auto uiBoardFloor = (ui::ImageView*)utils::findChild(layout, "UI_FLOOR");
    if ( uiBoardFloor != nullptr )
    {
        int floor = PrisonFrostManager::getInstance()->getClearFloor() + 1;
        int floorMax = PrisonFrostManager::getInstance()->getClearFloorMax();
        
        if ( floor > floorMax )
        {
            uiBoardFloor->setVisible(false);
        }
        else
        {
            uiBoardFloor->setVisible(true);
            
            //
            auto uiFloor = uiBoardFloor->getChildByName<ui::CText*>("UI_TEXT_FLOOR");
            uiFloor->setString(UtilsString::getInstance()->format("%dF", floor));
            
            //
            std::string hp = "0";
            auto objFloor = PrisonFrostManager::getInstance()->getInfoFloor(floor);
            if ( objFloor != nullptr )
            {
                hp = objFloor->getHP();
                hp = MafUtils::convertNumberToShort(hp);
            }
            
            auto uiHp = uiBoardFloor->getChildByName<ui::CText*>("UI_TEXT_HP");
            uiHp->setString(UtilsString::getInstance()->format("(%s)", hp.c_str()));
        }
    }
}

void LayerFrostBattle::drawBattleUI()
{
    auto layout = _uiContentsBattleUI;
    
    //
    bool bBattleAuto = PrisonFrostManager::getInstance()->isBattleAuto();
    int battleAutoNow = PrisonFrostManager::getInstance()->getBattleAutoNow();
    int battleAutoMax = PrisonFrostManager::getInstance()->getBattleAutoMax();
    
    if ( _eState != E_STATE::None && battleAutoMax > 1 )
    {
        auto uiBtnAuto = _uiBattleAuto->getChildByName<ui::Button*>("UI_BTN_AUTO");
        auto uiTextKey = _uiBattleAuto->getChildByName<ui::CText*>("UI_TEXT_COUNT");
        uiTextKey->setString(MafUtils::format("%d/%d", battleAutoNow, battleAutoMax));
        
        _uiBattleAuto->setVisible(true);
        if ( bBattleAuto == true )
        {
            _uiBattleAuto->loadTexture("Assets/ui/prison/icon_auto1_2.png");
            uiBtnAuto->loadTextureNormal("Assets/ui/prison/icon_auto1_1.png");
            uiBtnAuto->runAction(RepeatForever::create(RotateBy::create(1.0, -180)));
        }
        else
        {
            _uiBattleAuto->loadTexture("Assets/ui/prison/icon_auto2_2.png");
            uiBtnAuto->loadTextureNormal("Assets/ui/prison/icon_auto2_1.png");
            uiBtnAuto->stopAllActions();
        }
    }
    else
    {
        _uiBattleAuto->setVisible(false);
    }
}

#pragma mark - ui : set, get
LayerFrostBattle::E_STATE LayerFrostBattle::getState()
{
    return _eState;
}

#pragma mark - ui : click
void LayerFrostBattle::onClickAuto(cocos2d::Ref* sender)
{
    bool bBattleAuto = PrisonFrostManager::getInstance()->isBattleAuto();
    PrisonFrostManager::getInstance()->setBattleAuto(!bBattleAuto);
    
    // draw
    drawBattleUI();
}

#pragma mark - callback

#pragma mark - schedule

#pragma mark - game
void LayerFrostBattle::onDrawInfo()
{
    drawLobby();
}

void LayerFrostBattle::onBattleStart(bool bAnimation)
{    
    //
    _eState = E_STATE::Idle;
    _bAnimation = bAnimation;
    
    //
    _damageTotal = "0";
    
    //
    _uiContentsBattleEffect->setVisible(true);
    _uiContentsBattleEffect->removeAllChildren();
    _uiContentsBattleEffect->stopAllActions();
    
    //
    auto action1 = FadeIn::create(0.25);
    auto action2 = CallFunc::create([=](){
        
        _uiContentsBattle->setVisible(true);
        _uiContentsBattle->jumpToLeft();
        
        // 노인 생성 및 위치 조정
        _unitMonster->setVisible(true);
        _unitMonster->setPosition(Size(105*4, 138));
        _unitMonster->onIdle();
        
        // 캐릭터 생성 및 위치 조정
        _unitHero->setVisible(true);
        _unitHero->setMonster(_unitMonster);
        _unitHero->onWalk();
        
        // 벽 생성
        _unitDoor->setVisible(true);
        _unitDoor->setTexture("Assets/ui/frost_prison/bg_frost_dungeon_door01.png");
        
        // 말풍선 보이기
        auto uiBubbleSpeech = _uiContentsBattle->getInnerContainer()->getChildByName<ui::ImageView*>("UI_BUBBLE_SPEECH");
        if ( uiBubbleSpeech != nullptr )
        {
            uiBubbleSpeech->setVisible(true);
            
            auto uiText = uiBubbleSpeech->getChildByName<ui::CText*>("UI_TEXT");
            uiText->setString(GAME_TEXT(UtilsString::getInstance()->format("t_ui_frost_prison_small_talk_att_%d", random(1, 3))));
        }
        
    });
    auto action3 = FadeOut::create(0.25);
    auto action4 = CallFunc::create([=](){
        // 공격 함수 호출
        onBattleAttack();
    });
    auto seq = Sequence::create(action1, action2, action3, action4, NULL);
    
    //
    auto uiBlack = ui::Layout::create();
    uiBlack->setContentSize(_uiContentsBattleEffect->getContentSize());
    uiBlack->setPosition(Vec2::ZERO);
    uiBlack->setBackGroundColor(Color3B::BLACK);
    uiBlack->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    uiBlack->setOpacity(0);
    uiBlack->runAction(seq);
    _uiContentsBattleEffect->addChild(uiBlack);
    
    //
    drawLobby();
    drawBattleUI();
}

void LayerFrostBattle::onBattleAttack()
{
    //
    _eState = E_STATE::Attack;
    
    //
    double timeAttack = PrisonFrostManager::getInstance()->getInfoSetting("frost_prison_att_time")->getValueToInt();;
    
    //
    _unitHero->onAttack([=](std::string damage){
      
        //
        _damageTotal = MafUtils::bigAddNum(_damageTotal, damage);
      
    });
    
    //
    _uiContentsBattleUI->setVisible(true);
    auto uiBar = (ProgressTimer*)utils::findChild(_uiContentsBattleUI, "UI_TIMER");
    if ( uiBar != nullptr )
    {
        auto action1 = ProgressFromTo::create(timeAttack, 100, 0);
        auto action2 = CallFunc::create([=](){
            
            _uiContentsBattleUI->setVisible(false);
            
            //
            _unitHero->onWalk();
            
            //
            _bResult = false;
            {
                int floor = PrisonFrostManager::getInstance()->getClearFloor() + 1;
                
                std::string damage = _damageTotal;
                std::string damageMax = "0";
                
                auto objFloor = PrisonFrostManager::getInstance()->getInfoFloor(floor);
                if ( objFloor != nullptr )
                {
                    damageMax = objFloor->getHP();
                }
                
                if ( damageMax.compare("0") != 0 && MafUtils::compareStr(damage, damageMax) == true )
                {
                    _bResult = true;
                }
            }
            
            // 말풍선 보이기
            auto uiBubbleSpeech = _uiContentsBattle->getInnerContainer()->getChildByName<ui::ImageView*>("UI_BUBBLE_SPEECH");
            if ( uiBubbleSpeech != nullptr )
            {
                uiBubbleSpeech->setVisible(false);
            }
            
            auto uiBubbleMark = _uiContentsBattle->getInnerContainer()->getChildByName<ui::ImageView*>("UI_BUBBLE_MARK");
            if ( uiBubbleMark != nullptr )
            {
                uiBubbleMark->setVisible(true);
                
                auto action1 = DelayTime::create(1.0);
                auto action2 = Hide::create();
                auto seq = Sequence::create(action1, action2, NULL);
                uiBubbleMark->runAction(seq);
            }
        });
        auto action3 = DelayTime::create(1.0);
        auto action4 = CallFunc::create(CC_CALLBACK_0(LayerFrostBattle::onBattleMonsterAngry, this));
        auto seq = Sequence::create(action1, action2, action3, action4, NULL);
        uiBar->runAction(seq);
    }
    
}

void LayerFrostBattle::onBattleMonsterAngry()
{
    //
    _eState = E_STATE::MonsterAngry;
    
    // 말풍선 감추기
    auto uiSpeechBubble = _uiContentsBattle->getInnerContainer()->getChildByName<ui::ImageView*>("UI_SPEECHBUBBLE");
    if ( uiSpeechBubble != nullptr )
    {
        uiSpeechBubble->setVisible(false);
    }
    
    if ( _bAnimation == true )
    {
        _unitMonster->onAngry([=](){
            onBattleMonsterMove();
        });
    }
    else
    {
        onBattleFinish();
    }
    
}

void LayerFrostBattle::onBattleMonsterMove()
{
    auto sizeContainer = _uiContentsBattle->getInnerContainerSize();
    
    //
    _eState = E_STATE::MonsterMoving;
    _unitMonster->onWalk();
    
    //
    auto action1 = MoveTo::create(1.0, Vec2(sizeContainer.width - 435, _unitMonster->getPositionY()));
    auto action2 = CallFunc::create([=](){
        onBattleMonsterAttack();
    });
    auto seq = Sequence::create(action1, action2, NULL);
    _unitMonster->runAction(seq);
    
    //
    _uiContentsBattle->scrollToRight(1.0, false);
}

void LayerFrostBattle::onBattleMonsterAttack()
{
    //
    _eState = E_STATE::MonsterAttack;
    
    _unitMonster->setVisible(false);
    
    //
    {
        // ani step 1
        auto animation1 = Animation::create();
        animation1->setDelayPerUnit(0.1);
        for ( int i = 1; i <= 10; i++ )
        {
            auto spriteAni = Sprite::create(MafUtils::format("Assets/ui/frost_prison/mamon_attack_ani%02d.png",i).c_str());
            spriteAni->getTexture()->setTexParameters(texParams);
            animation1->addSpriteFrame(spriteAni->getSpriteFrame());
        }
        
        // ani step 2
        auto animation2 = Animation::create();
        animation2->setDelayPerUnit(0.1);
        if ( _bResult )
        {
            for ( int i = 11; i <= 15; i++ )
            {
                auto spriteAni = Sprite::create(MafUtils::format("Assets/ui/frost_prison/mamon_attack_ani%02d.png",i).c_str());
                spriteAni->getTexture()->setTexParameters(texParams);
                animation2->addSpriteFrame(spriteAni->getSpriteFrame());
            }
        }
        else
        {
            for ( int i = 11; i <= 15; i++ )
            {
                auto spriteAni = Sprite::create(MafUtils::format("Assets/ui/frost_prison/mamon_attack_ani%02d_fail.png",i).c_str());
                spriteAni->getTexture()->setTexParameters(texParams);
                animation2->addSpriteFrame(spriteAni->getSpriteFrame());
            }
        }
        
        // ani step 3
        auto animation3 = Animation::create();
        animation3->setDelayPerUnit(0.1);
        for ( int i = 16; i <= 19; i++ )
        {
            auto spriteAni = Sprite::create(MafUtils::format("Assets/ui/frost_prison/mamon_attack_ani%02d.png",i).c_str());
            spriteAni->getTexture()->setTexParameters(texParams);
            animation3->addSpriteFrame(spriteAni->getSpriteFrame());
        }
        
        auto action1 = Animate::create(animation1);
        auto action2 = CallFunc::create([=](){
            
            //
            animationActionCut();
        });
        auto action3 = DelayTime::create(1.2);
        auto action4 = Animate::create(animation2);
        auto action5 = CallFunc::create([=](){
            //
            std::string damage = _damageTotal;
            damage = MafUtils::convertNumberToShort(damage);
            
            //
            auto labelDamage = ui::CText::create(damage, GAME_FONT_AL, 48);
            labelDamage->setPosition(Vec2(_uiContentsBattle->getInnerContainerSize().width - 200, _uiContentsBattle->getInnerContainerSize().height * 0.75));
            labelDamage->setOpacity(0);
            labelDamage->setScale(2.0);
            _uiContentsBattle->addChild(labelDamage);
            {
                labelDamage->setColor(Color3B(255,204,65));

                auto action1_1 = FadeIn::create(0.2);
                auto action1_2 = DelayTime::create(0.7);
                auto action1_3 = FadeOut::create(0.1);
                auto seq1 = Sequence::create(action1_1, action1_2, action1_3, NULL);

                auto action2_1 = ScaleTo::create(0.2, 2.2);
                auto action2_2 = DelayTime::create(0.7);
                auto action2_3 = ScaleTo::create(0.1, 1.8);
                auto seq2 = Sequence::create(action2_1, action2_2, action2_3,  RemoveSelf::create(), NULL);

                auto spawn = Spawn::create(seq1, seq2, NULL);
                labelDamage->runAction(spawn);
            }
        });
        auto action6 = Animate::create(animation3);
        auto action7 = CallFunc::create([=](){
            
            //
            animationDoor();
        });
        auto action8 = DelayTime::create(0.7);
        auto action9 = CallFunc::create(CC_CALLBACK_0(LayerFrostBattle::onBattleFinish, this));
        auto seq = Sequence::create(action1, action2, action3, action4, action5, action6, action7, action8, action9, NULL);
        
        auto uiAni = Sprite::create("Assets/ui/frost_prison/mamon_attack_ani01.png");
        uiAni->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        uiAni->setPosition(Vec2::ZERO);
        uiAni->setScale(3);
        uiAni->getTexture()->setTexParameters(texParams);
        uiAni->runAction(seq);
        _uiContentsBattleEffect->removeAllChildren();
        _uiContentsBattleEffect->addChild(uiAni);
    }
}

void LayerFrostBattle::onBattleFinish()
{
    //
    _eState = E_STATE::None;
    
    auto action1 = DelayTime::create(0.8);
    auto action2 = CallFunc::create([=](){
    
        //
        _uiContentsBattle->setVisible(false);
        _uiContentsBattleEffect->setVisible(false);
        
        // 통신
        int floor = PrisonFrostManager::getInstance()->getClearFloor() + 1;
        PrisonFrostManager::getInstance()->requestResult(true, floor, _bResult, _damageTotal);
    });
    auto seq = Sequence::create(action1, action2, NULL);
    this->runAction(seq);
}

void LayerFrostBattle::animationActionCut()
{
    auto layout = _uiContentsBattleEffect;
    auto size = layout->getContentSize();
    
    SoundManager::SoundEffectStart(SOUND_FROST_CUTSCENE);
    
    //
    auto animation = Animation::create();
    animation->setDelayPerUnit(0.1);
    if ( _bResult )
    {
        for ( int i = 1; i <= 8; i++ )
        {
            auto spriteAni = Sprite::create(MafUtils::format("Assets/ui/frost_prison/mamon_attack_cutscene_success%02d.png",i).c_str());
            spriteAni->getTexture()->setTexParameters(texParams);
            animation->addSpriteFrame(spriteAni->getSpriteFrame());
        }
    }
    else
    {
        for ( int i = 1; i <= 8; i++ )
        {
            auto spriteAni = Sprite::create(MafUtils::format("Assets/ui/frost_prison/mamon_attack_cutscene_fail%02d.png",i).c_str());
            spriteAni->getTexture()->setTexParameters(texParams);
            animation->addSpriteFrame(spriteAni->getSpriteFrame());
        }
    }
    
    auto action1 = DelayTime::create(0.8);
    auto action2 = FadeOut::create(0.1);
    auto action3 = RemoveSelf::create();
    auto seq = Sequence::create(action1, action2, action3, NULL);
    
    auto uiAni = Sprite::create("Assets/ui/frost_prison/mamon_attack_ani01.png");
    uiAni->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    uiAni->setPosition(Vec2(size.width, size.height / 2));
    uiAni->setScale(4);
    uiAni->getTexture()->setTexParameters(texParams);
    uiAni->runAction(Animate::create(animation));
    uiAni->runAction(seq);
    _uiContentsBattleEffect->addChild(uiAni);
}

void LayerFrostBattle::animationDoor()
{
    auto animation = Animation::create();
    animation->setDelayPerUnit(0.1);
    
    if ( _bResult )
    {
        SoundManager::SoundEffectStart(SOUND_FROST_ATTACK_SUCCCESS);
        
        for ( int i = 1; i <= 7; i++ )
        {
            auto spriteAni = Sprite::create(MafUtils::format("Assets/ui/frost_prison/bg_frost_dungeon_door%02d.png",i).c_str());
            spriteAni->getTexture()->setTexParameters(texParams);
            animation->addSpriteFrame(spriteAni->getSpriteFrame());
        }
        
        auto action1 = Animate::create(animation);
        auto action2 = Hide::create();
        auto seq = Sequence::create(action1, action2, NULL);
        
        _unitDoor->runAction(seq);
    }
    else
    {
        SoundManager::SoundEffectStart(SOUND_FROST_ATTACK_FAIL);
        
        _unitDoor->runAction(Shake::create(0.7, 4));
    }
    
    
}
