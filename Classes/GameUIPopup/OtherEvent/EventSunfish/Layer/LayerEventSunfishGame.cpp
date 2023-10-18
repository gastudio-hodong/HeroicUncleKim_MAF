//
//  LayerEventSunfishGame.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 09/01/2020.
//

#include "LayerEventSunfishGame.h"

#include "Common/ConfigKey.h"
#include "Common/ConfigGameNormal.h"

#include "GameUIPopup/OtherEvent/EventSunfish/StateStep/PopupEventSunfishFood.h"
#include "GameUIPopup/OtherEvent/EventSunfish/StateStep/PopupEventSunfishLevel.h"

#include "ManagerEvent/EventSunfishManager.h"
#include "ManagerGame/DialogManager.h"

USING_NS_CC;

LayerEventSunfishGame::LayerEventSunfishGame(void) :
_bInitUI(false),

//
_bGameStart(false),

_enemyAttackCount(0),

_enemySpawnTime(0),
_touchTime(0),
_feverActiveTime(0),

//
_uiContentsBattle(nullptr),

_heroUnit(nullptr),
_feverUnit(nullptr),

_bubbleParticle(nullptr),
_bubbleParticleFever(nullptr)
{
    
}

LayerEventSunfishGame::~LayerEventSunfishGame(void)
{
    EventSunfishManager::getInstance()->onDelegateGameLevelUp.Clear(this);
    EventSunfishManager::getInstance()->onDelegateGameDie.Clear(this);
    EventSunfishManager::getInstance()->onDelegateGameWeight.Clear(this);
}

bool LayerEventSunfishGame::init()
{
    if ( !ui::Layout::init() )
    {
        return false;
    }
    
    setClippingEnabled(true);
    setTouchEnabled(true);
    
    
    
    //
    EventSunfishManager::getInstance()->onDelegateGameLevelUp += new MafFunction<void(void)>(CC_CALLBACK_0(LayerEventSunfishGame::onLevelUP, this), this);
    EventSunfishManager::getInstance()->onDelegateGameDie += new MafFunction<void(void)>(CC_CALLBACK_0(LayerEventSunfishGame::onDie, this), this);
    EventSunfishManager::getInstance()->onDelegateGameWeight += new MafFunction<void(double)>(CC_CALLBACK_1(LayerEventSunfishGame::onWeightAdd, this), this);
    
    return true;
}

void LayerEventSunfishGame::onEnter()
{
    ui::Layout::onEnter();
 
    if ( _bInitUI == false )
    {
        // init
        initVar();
        initUi();
        
        _bInitUI = true;
    }
}

#pragma mark - override : touch
bool LayerEventSunfishGame::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unusedEvent)
{
    auto ret = ui::Layout::onTouchBegan(touch, unusedEvent);
    return ret;
}

void LayerEventSunfishGame::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unusedEvent)
{
    if ( EventSunfishManager::getInstance()->getEndTime() - UtilsDate::getInstance()->getTime() <= 0 )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_draw_msg_1"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        
        return;
    }
    
    //
    bool bAttack = false;
    
    //
    for ( int i = 0; i < _listEnemyUnit.size(); i++ )
    {
        auto enemyUnit = _listEnemyUnit.at(i);
        
        if ( enemyUnit->getBoundingBox().containsPoint(_uiContentsBattle->convertToNodeSpace(touch->getLocation())) == true )
        {
            onAttack(i);
            
            //
            _touchTime = UtilsDate::getInstance()->getTime();
            bAttack = true;
            break;
        }
    }
    
    if ( bAttack == false && _feverUnit != nullptr && _feverUnit->isVisible() == true )
    {
        if ( _feverUnit->getBoundingBox().containsPoint(_uiContentsBattle->convertToNodeSpace(touch->getLocation())) == true )
        {
            onFeverTimeActive();
            
            //
            _touchTime = UtilsDate::getInstance()->getTime();
            bAttack = true;
        }
    }
}

void LayerEventSunfishGame::update(float dt)
{
    if ( _bGameStart == false )
    {
        return;
    }
    
    if ( _feverActiveTime > 0 )
    {
        _feverActiveTime -= dt;
        if ( _feverActiveTime <= 0 )
        {
            _feverActiveTime = 0;
            onFeverTimeActiveFinish();;
        }
    }
    
    bool bEnemySpawn = false;
    
    _enemySpawnTime += dt;
    if ( _feverActiveTime > 0 && _enemySpawnTime >= 0.1 )
    {
        _enemySpawnTime -= 0.1;
        
        bEnemySpawn = true;
    }
    else if ( _enemySpawnTime >= 2 )
    {
        _enemySpawnTime -= 2;
        
        bEnemySpawn = true;
    }
    
    if ( bEnemySpawn == true )
    {
        int enemyCountNow = (int)_listEnemyUnit.size();
        int enemyCountMax = EventSunfishManager::getInstance()->getGameEnemyCountMax();
        if ( enemyCountNow < enemyCountMax )
        {
            auto listSunfish = EventSunfishManager::getInstance()->getListInfoSunfish(EventSunfishManager::E_TYPE::FOOD);
            
            std::vector<int> listEnemyIdx;
            for ( auto& objSunfish : listSunfish )
            {
                if ( EventSunfishManager::getInstance()->isReinforce(EventSunfishManager::E_TYPE::FOOD, objSunfish->getLevel()) == true )
                {
                    listEnemyIdx.push_back(objSunfish->getLevel());
                }
            }
            
            //
            auto enemyIdx = listEnemyIdx.at(random(0, (int)(listEnemyIdx.size() -1)));
            enemySpawn(enemyIdx);
            enemySave();
        }
    }
        
    //
    auto feverTime = EventSunfishManager::getInstance()->getFeverTime();
    if ( feverTime < UtilsDate::getInstance()->getTime() )
    {
        feverTime = UtilsDate::getInstance()->getTime() + EventSunfishManager::getInstance()->getFeverCoolTime();
        EventSunfishManager::getInstance()->setFeverTime(feverTime);
        
        onFeverTime();
    }
}


#pragma mark - init
void LayerEventSunfishGame::initVar()
{
    _touchTime = UtilsDate::getInstance()->getTime();
}

void LayerEventSunfishGame::initUi()
{
    auto size = getContentSize();
    
    auto uiBG = ui::ImageView::create("Assets/ui/event/event_bg_4_1.png");
    uiBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    uiBG->setPosition(Vec2(size.width / 2, size.height));
    ((ui::Scale9Sprite*)uiBG->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
    uiBG->setScale(4);
    addChild(uiBG);
    
    _uiContentsBattle = ui::Layout::create();
    _uiContentsBattle->setContentSize(Size(size.width, size.height - 220));
    _uiContentsBattle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _uiContentsBattle->setPosition(Vec2(size.width / 2, size.height - 100));
    addChild(_uiContentsBattle);
    
    _bubbleParticle = ParticleSystemQuad::create("Assets/Particle/sunfish/bubble_slow.plist");
    _bubbleParticle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _bubbleParticle->setPosition(Vec2(getContentSize().width / 2, 0));
    _bubbleParticle->setSourcePosition(Vec2(0, 0));
    addChild(_bubbleParticle);
    
    _bubbleParticleFever = ParticleSystemQuad::create("Assets/Particle/sunfish/bubble_fever.plist");
    _bubbleParticleFever->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _bubbleParticleFever->setPosition(Vec2(getContentSize().width / 2, 0));
    _bubbleParticleFever->setSourcePosition(Vec2(0, 0));
    _bubbleParticleFever->setVisible(false);
    addChild(_bubbleParticleFever);
}

#pragma mark - ui

#pragma mark - ui : draw

#pragma mark - ui : click

#pragma mark - callback

#pragma mark - schedule

#pragma mark - set, get

#pragma mark - event
void LayerEventSunfishGame::onStart()
{
    _bGameStart = true;
    
    //
    auto levelIdx = EventSunfishManager::getInstance()->getGameLevelNow();
    heroSpawn(levelIdx);
    
    
    //
    std::string dataEnemy = UserDefault::getInstance()->getStringForKey(KEY_EVENT_SUNFISH_ENEMY);
    if ( !dataEnemy.empty() )
    {
        dataEnemy = MafAes256::Decrypt(dataEnemy);
        
        std::vector<std::string> listEnemyIdx = UtilsString::getInstance()->split(dataEnemy, ',');
        for ( auto& enemyIdx : listEnemyIdx )
        {
            enemySpawn(std::atoi(enemyIdx.c_str()));
        }
    }
    
    
    //
    scheduleUpdate();
}

void LayerEventSunfishGame::onLevelUP()
{
    PopupLoadingLock::show();
    
    //
    auto levelIdx = EventSunfishManager::getInstance()->getGameLevelNow();
    heroLevelUp(levelIdx);
    
    EventSunfishManager::getInstance()->onDelegateInfoUpdate();
    
    //
    auto scene = UtilsScene::getInstance()->getRunningScene();
    
    auto uiBlank = ui::Layout::create();
    uiBlank->setContentSize(scene->getContentSize());
    uiBlank->setBackGroundColor(Color3B::WHITE);
    uiBlank->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    uiBlank->setOpacity(0);
    scene->addChild(uiBlank, TAG_SCENE_POPUP);
    {
        auto action0 = DelayTime::create(1.0);
        auto action1 = FadeIn::create(0.4);
        auto action2 = FadeOut::create(0.4);
        auto action3 = CallFunc::create([=](){
    
            PopupLoadingLock::hide();
            
            auto levelIdx = EventSunfishManager::getInstance()->getGameLevelNow();
            auto levelMax = EventSunfishManager::getInstance()->getGameLevelMax();
            if ( levelIdx >= levelMax )
            {
                // 시나리오 연출
                if ( DialogManager::getInstance()->isViewDialog("EVENT_SUNFISH_EP") == 0 )
                {
                    DialogManager::getInstance()->showDialog("EVENT_SUNFISH_EP");
                }
            }
        });
        uiBlank->runAction(Sequence::create(action0, action1, action2, action3, RemoveSelf::create(), NULL));
    }
}

void LayerEventSunfishGame::onDie()
{
    PopupLoadingLock::show();

    //
    EventSunfishManager::getInstance()->setGameWeight(0);
    EventSunfishManager::getInstance()->onDelegateInfoUpdate();
    
    //
    auto scene = UtilsScene::getInstance()->getRunningScene();

    //
    auto uiBlank = ui::Layout::create();
    uiBlank->setContentSize(scene->getContentSize());
    uiBlank->setBackGroundColor(Color3B::WHITE);
    uiBlank->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    uiBlank->setOpacity(0);
    scene->addChild(uiBlank, TAG_SCENE_POPUP);
    {
        auto action1 = FadeIn::create(1.0);
        auto action2 = CallFunc::create([=](){

            _heroUnit->stopAllActions();
            _heroUnit->removeFromParent();
            _heroUnit = nullptr;

            for ( auto iter = _listEnemyUnit.rbegin(); iter != _listEnemyUnit.rend(); ++iter )
            {
                auto enemyUnit = *iter;

                _listEnemyUnit.eraseObject(enemyUnit);
                enemyUnit->stopAllActions();
                enemyUnit->removeFromParent();
            }
            enemySave();

            //
            onStart();
            
            // 시나리오 연출
            if ( DialogManager::getInstance()->isViewDialog("EVENT_SUNFISH_DIE") == 0 )
            {
                DialogManager::getInstance()->showDialog("EVENT_SUNFISH_DIE");
            }
        });
        auto action3 = FadeOut::create(1.0);
        auto action4 = CallFunc::create([=](){

            PopupLoadingLock::hide();
        });
        uiBlank->runAction(Sequence::create(action1, action2, action3, action4, RemoveSelf::create(), NULL));
    }
    
    //
    _bGameStart = false;
    
    _enemySpawnTime = 0;
    _touchTime = UtilsDate::getInstance()->getTime();
    
    
    //
    unscheduleUpdate();
}

void LayerEventSunfishGame::onWeightAdd(double weightAdd)
{
    if ( weightAdd <= 0 )
    {
        return;
    }
    
    auto weight = EventSunfishManager::getInstance()->getGameWeight() + weightAdd;
    
    //
    EventSunfishManager::getInstance()->setGameWeight(weight);
    EventSunfishManager::getInstance()->onDelegateInfoUpdate();
    
    //
    heroWeightAdd(weightAdd);
    
    //
    auto levelNow = EventSunfishManager::getInstance()->getGameLevelNow();
    auto levelMax = EventSunfishManager::getInstance()->getGameLevelMax();
    if ( levelNow < levelMax )
    {
        auto levelNext = levelNow + 1;
    
        auto objSunfishStep = EventSunfishManager::getInstance()->getInfoSunfishLevel(levelNext);
        if ( objSunfishStep->getWeight() <= weight )
        {
            // 성장 체크 팝업
            auto popup = PopupEventSunfishLevel::create();
            popup->setLevelIdx(levelNow);
            popup->setHideBackKey(false);
            popup->show();
        }
    }
    
    //
    if ( _enemyAttackCount % 5 == 0 )
    {
        EventSunfishManager::getInstance()->requestWeight(false);
    }
}

void LayerEventSunfishGame::onAttack(int idx)
{
    if ( idx >= _listEnemyUnit.size() )
    {
        return;
    }
    
    PopupLoadingLock::show();
    
    auto enemyUnit = _listEnemyUnit.at(idx);
    
    // 마지막으로 먹은 Food 설정
    auto levelIdx = EventSunfishManager::getInstance()->getGameLevelNow();
    auto foodIdx = enemyUnit->getTag();
    
    //
    heroAttack(idx);
    
    //
    _enemyAttackCount++;
    
    //
    auto distance = _heroUnit->getPosition().distance(enemyUnit->getPosition());
    distance = MIN(500, distance);
    
    auto actionDelay = DelayTime::create(distance * 0.001);
    auto actionAttack = CallFuncN::create([=](Node* sender){
        enemyHit((Sprite*)sender);
        enemySave();
    });
    auto actionFinish = CallFunc::create([=](){
        PopupLoadingLock::hide();
        
        // Food 정보 가져오기
        auto objSunfish = EventSunfishManager::getInstance()->getInfoSunfishFood(foodIdx);
        if ( objSunfish != nullptr )
        {
            auto rate = (objSunfish->getSurvivalRate() + objSunfish->getBonusRate() * EventSunfishManager::getInstance()->getDie(EventSunfishManager::E_TYPE::FOOD, foodIdx)) / 10.0f;
            rate = MIN(rate, 99.9f);
            
            if ( random(0.0f, 100.0f) > rate )
            {
                // 사망 체크 팝업
                auto popup = PopupEventSunfishFood::create();
                popup->setFoodIdx(foodIdx);
                popup->setLevelIdx(levelIdx);
                popup->setHideBackKey(false);
                popup->show();
            }
            else
            {
                // 무게 증가
                double weight = objSunfish->getWeight();
                double weightBonus = objSunfish->getWeight() * (objSunfish->getBonusWeight() * MIN(10, EventSunfishManager::getInstance()->getDie(EventSunfishManager::E_TYPE::FOOD, foodIdx)) / 1000.0f);
                onWeightAdd(weight + weightBonus);
            }
        }
    });
    enemyUnit->stopActionByTag(10);
    enemyUnit->stopActionByTag(11);
    enemyUnit->runAction(Sequence::create(actionDelay, actionAttack, actionFinish, NULL));
}

void LayerEventSunfishGame::onFeverTime()
{
    feverSpawn();
}

void LayerEventSunfishGame::onFeverTimeActive()
{
    _feverActiveTime = EventSunfishManager::getInstance()->getFeverActiveTime();
    
    //
    _feverUnit->stopAllActions();
    _feverUnit->setVisible(false);
    
    _bubbleParticle->setVisible(false);
    _bubbleParticleFever->setVisible(true);
    
    //
    auto uiFever = ui::Layout::create();
    uiFever->setContentSize(getContentSize());
    uiFever->setName("UI_FEVER");
    addChild(uiFever);
    {
        auto uiEffect1 = ui::ImageView::create("Assets/ui/event/event_spotlight_1_1.png");
        uiEffect1->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        uiEffect1->setPosition(Vec2(0, uiFever->getContentSize().height + 100));
        uiEffect1->setRotation(-20);
        uiEffect1->runAction(RepeatForever::create(Blink::create(2, 5)));
        uiFever->addChild(uiEffect1);
        
        auto uiEffect2 = ui::ImageView::create("Assets/ui/event/event_spotlight_1_1.png");
        uiEffect2->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        uiEffect2->setPosition(Vec2(uiFever->getContentSize().width / 2, uiFever->getContentSize().height + 100));
        uiEffect2->runAction(RepeatForever::create(Blink::create(2, 5)));
        uiFever->addChild(uiEffect2);
        
        auto uiEffect3 = ui::ImageView::create("Assets/ui/event/event_spotlight_1_1.png");
        uiEffect3->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        uiEffect3->setPosition(Vec2(uiFever->getContentSize().width, uiFever->getContentSize().height + 100));
        uiEffect3->setRotation(+20);
        uiEffect3->runAction(RepeatForever::create(Blink::create(2, 5)));
        uiFever->addChild(uiEffect3);
        
        auto uiBarBG = ui::ImageView::create("Assets/ui/fever/fever_bar_1.png");
        uiBarBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        uiBarBG->setPosition(Vec2(uiFever->getContentSize().width / 2, uiFever->getContentSize().height - 100));
        uiFever->addChild(uiBarBG);
        {
            auto uiBar = ui::LoadingBar::create("Assets/ui/fever/fever_bar_2.png");
            uiBar->setPercent(100);
            uiBar->setPosition(Vec2(uiBarBG->getContentSize().width / 2, uiBarBG->getContentSize().height / 2));
            uiBar->setName("UI_REWARD_BAR");
            uiBar->runAction(RepeatForever::create(Sequence::create(CallFuncN::create([=](Node* sender){
            
                auto timeNow = _feverActiveTime;
                auto timeMax = EventSunfishManager::getInstance()->getFeverActiveTime();
                
                double percent = (double)timeNow / timeMax * 100.0f;
                if ( percent > 100 )
                    percent = 100;
                
                auto uiBar = (ui::LoadingBar*)sender;
                uiBar->setPercent(percent);
            }), NULL)));
            uiBarBG->addChild(uiBar);
            
            
            auto uiBarText = Sprite::create("Assets/ui/fever/fever_bar_3.png");
            uiBarText->setPosition(Vec2(uiBarBG->getContentSize().width / 2, uiBarBG->getContentSize().height / 2));
            uiBarBG->addChild(uiBarText);
        }
    }
}

void LayerEventSunfishGame::onFeverTimeActiveFinish()
{
    //
    _bubbleParticle->setVisible(true);
    _bubbleParticleFever->setVisible(false);
    
    //
    removeChildByName("UI_FEVER");
}

#pragma mark - unit
void LayerEventSunfishGame::heroSpawn(int levelIdx)
{
    auto layout = _uiContentsBattle;
    auto size = layout->getContentSize();
    
    if ( _heroUnit != nullptr )
    {
        _heroUnit->removeFromParent();
        _heroUnit = nullptr;
    }
    _heroUnit = Sprite::create(MafUtils::format("Assets/ui/event/lobster_event_%d_1.png", levelIdx));
    _heroUnit->getTexture()->setTexParameters(texParams);
    _heroUnit->setScale(4);
    layout->addChild(_heroUnit, 10);
    
    // animation
    auto animation = Animation::create();
    animation->setDelayPerUnit(0.1f);
    for ( int i = 1; i <= 2; i++ )
    {
        auto ani = Sprite::create(MafUtils::format("Assets/ui/event/lobster_event_%d_%d.png", levelIdx, i));
        ani->getTexture()->setTexParameters(texParams);
        
        animation->addSpriteFrame(ani->getSpriteFrame());
    }
    _heroUnit->runAction(RepeatForever::create(Animate::create(animation)));
    
    //
    auto mark = Sprite::create("Assets/ui/event/event_icon_mark_1_1.png");
    mark->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    mark->setPosition(Vec2(_heroUnit->getContentSize().width / 2, _heroUnit->getContentSize().height));
    mark->setName("UI_MARK");
    _heroUnit->addChild(mark, 10);
    {
        auto animation = Animation::create();
        animation->setDelayPerUnit(0.2f);
        for ( int i = 1; i <= 2; i++ )
        {
            auto ani = Sprite::create(MafUtils::format("Assets/ui/event/event_icon_mark_1_%d.png", i));
            ani->getTexture()->setTexParameters(texParams);
            
            animation->addSpriteFrame(ani->getSpriteFrame());
        }
        mark->stopAllActions();
        mark->runAction(RepeatForever::create(Animate::create(animation)));
    }
    
    // position
    int posX = size.width / 2;
    int posY = size.height / 2;
    _heroUnit->setPosition(Vec2(posX, posY));
    
    //
    heroMoveAction();
}

void LayerEventSunfishGame::heroLevelUp(int levelIdx)
{
    auto layout = _uiContentsBattle;
    auto size = layout->getContentSize();
    
    //
    _heroUnit->stopAllActions();
    
    //
    for ( int i = 0; i < 3; i++ )
    {
        auto spriteTemp = Sprite::createWithSpriteFrame(_heroUnit->getSpriteFrame());
        spriteTemp->setPosition(_heroUnit->getPosition());
        spriteTemp->getTexture()->setTexParameters(texParams);
        spriteTemp->setScale(4);
        spriteTemp->setOpacity(100);
        spriteTemp->setFlippedX(_heroUnit->isFlippedX());
        layout->addChild(spriteTemp, 9);
        
        auto action1_1 = DelayTime::create(i * 0.2);
        auto action1_2 = ScaleBy::create(0.5, 3);
        spriteTemp->runAction(Sequence::create(action1_1, action1_2, RemoveSelf::create(), NULL));
        
        auto action2_1 = DelayTime::create(i * 0.2);
        auto action2_2 = FadeOut::create(0.5);
        spriteTemp->runAction(Sequence::create(action2_1, action2_2, RemoveSelf::create(), NULL));
    }
    
    //
    auto action1 = DelayTime::create(1.4);
    auto action2 = CallFunc::create([=](){
        
        //
        _heroUnit->setTexture(MafUtils::format("Assets/ui/event/lobster_event_%d_1.png", levelIdx));
        
        auto animation = Animation::create();
        animation->setDelayPerUnit(0.1f);
        for ( int i = 1; i <= 2; i++ )
        {
            auto ani = Sprite::create(MafUtils::format("Assets/ui/event/lobster_event_%d_%d.png", levelIdx, i));
            ani->getTexture()->setTexParameters(texParams);
            
            animation->addSpriteFrame(ani->getSpriteFrame());
        }
        _heroUnit->runAction(RepeatForever::create(Animate::create(animation)));
        
        //
        auto mark = _heroUnit->getChildByName<Sprite*>("UI_MARK");
        if ( mark != nullptr )
        {
            mark->setPosition(Vec2(_heroUnit->getContentSize().width / 2, _heroUnit->getContentSize().height));
        }
        
        //
        heroMoveAction();
    });
    _heroUnit->runAction(Sequence::create(action1, action2, NULL));
}

void LayerEventSunfishGame::heroMoveAction()
{
    //
    auto actionIdle1 = MoveBy::create(0.5, Vec2(0, +10));
    auto actionIdle2 = MoveBy::create(0.5, Vec2(0, -10));
    auto actionMove = CallFuncN::create([=](Node* sender){
        
        int timeDiff = UtilsDate::getInstance()->getTime() - _touchTime;
        if ( timeDiff <= 3 )
        {
            return;
        }
        
        auto unit = (Sprite*)sender;
        
        double posX = sender->getPositionX() + random(-100, 100);
        double posY = sender->getPositionY() + random(-100, 100);
        
        posX = MIN(_uiContentsBattle->getContentSize().width - unit->getBoundingBox().size.width /2, posX);
        posX = MAX(unit->getBoundingBox().size.width / 2, posX);
        posY = MIN(_uiContentsBattle->getContentSize().height - unit->getBoundingBox().size.height /2, posY);
        posY = MAX(unit->getBoundingBox().size.height / 2, posY);
        
        //
        auto action = EaseSineOut::create(MoveTo::create(1.5, Vec2(posX, posY)));
        action->setTag(11);
        sender->runAction(action);
        
        //
        if ( sender->getPositionX() > posX )
        {
            ((Sprite*)sender)->setFlippedX(false);
        }
        else
        {
            ((Sprite*)sender)->setFlippedX(true);
        }
    });
    auto actionDelay = DelayTime::create(1.5);
    auto action = RepeatForever::create(Sequence::create(actionIdle1, actionIdle2, actionMove, actionDelay, NULL));
    action->setTag(10);
    _heroUnit->runAction(action);
}

void LayerEventSunfishGame::heroAttack(int enemyIdx)
{
    auto enemyUnit = _listEnemyUnit.at(enemyIdx);
    
    //
    auto distance = _heroUnit->getPosition().distance(enemyUnit->getPosition());
    distance = MIN(500, distance);
    
    auto actionMove = EaseSineInOut::create(MoveTo::create(distance * 0.001, enemyUnit->getPosition()));
    actionMove->setTag(11);
    _heroUnit->stopActionByTag(11);
    _heroUnit->runAction(actionMove);
    
    //
    if ( enemyUnit->getPositionX() < _heroUnit->getPositionX() )
    {
        _heroUnit->setFlippedX(false);
    }
    else
    {
        _heroUnit->setFlippedX(true);
    }
}

void LayerEventSunfishGame::heroWeightAdd(double weightAdd)
{
    auto layout = _uiContentsBattle;
    auto size = layout->getContentSize();
    
    auto uiWeight = ui::CText::create(MafUtils::format("+%.2f", weightAdd), GAME_FONT, 24);
    uiWeight->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    uiWeight->setPosition(Vec2(_heroUnit->getPositionX(), _heroUnit->getPositionY()));
    uiWeight->setTextColor(Color4B::YELLOW);
    uiWeight->enableOutline(Color4B::BLACK, 2);
    uiWeight->runAction(Sequence::create(MoveBy::create(0.5, Vec2(0, 20)), RemoveSelf::create(), NULL));
    layout->addChild(uiWeight, 10);
}

void LayerEventSunfishGame::enemySpawn(int enemyIdx)
{
    auto layout = _uiContentsBattle;
    auto size = layout->getContentSize();
    
    auto enemyUnit = Sprite::create(MafUtils::format("Assets/ui/event/lobster_mon_%d_1.png", enemyIdx));
    enemyUnit->getTexture()->setTexParameters(texParams);
    enemyUnit->setScale(4);
    enemyUnit->setTag(enemyIdx);
    layout->addChild(enemyUnit);
    
    _listEnemyUnit.pushBack(enemyUnit);
    
    //
    auto animation = Animation::create();
    animation->setDelayPerUnit(0.2f);
    for ( int i = 1; i <= 2; i++ )
    {
        auto ani = Sprite::create(MafUtils::format("Assets/ui/event/lobster_mon_%d_%d.png", enemyIdx, i));
        ani->getTexture()->setTexParameters(texParams);
        
        animation->addSpriteFrame(ani->getSpriteFrame());
    }
    enemyUnit->stopAllActions();
    enemyUnit->runAction(RepeatForever::create(Animate::create(animation)));
    
    //
    int posX = random(enemyUnit->getBoundingBox().size.width / 2, size.width - enemyUnit->getBoundingBox().size.width / 2);
    int posY = random(enemyUnit->getBoundingBox().size.height / 2, size.height - enemyUnit->getBoundingBox().size.height /2);
    enemyUnit->setPosition(Vec2(posX, posY));
    
    //
    auto actionDelay = DelayTime::create(random(1.0f, 2.0f));
    auto actionIdle1 = MoveBy::create(0.5, Vec2(0, +10));
    auto actionIdle2 = MoveBy::create(0.5, Vec2(0, -10));
    auto actionMove = CallFuncN::create([=](Node* sender){
        
        auto enemyUnit = (Sprite*)sender;
        
        double posX = sender->getPositionX() + random(-100, 100);
        double posY = sender->getPositionY() + random(-100, 100);
        posX = MIN(_uiContentsBattle->getContentSize().width - enemyUnit->getBoundingBox().size.width /2, posX);
        posX = MAX(enemyUnit->getBoundingBox().size.width / 2, posX);
        posY = MIN(_uiContentsBattle->getContentSize().height - enemyUnit->getBoundingBox().size.height /2, posY);
        posY = MAX(enemyUnit->getBoundingBox().size.height / 2, posY);
        
        //
        auto action = EaseSineOut::create(MoveTo::create(1.5, Vec2(posX, posY)));
        action->setTag(11);
        sender->runAction(action);
        
        //
        if ( sender->getPositionX() > posX )
        {
            ((Sprite*)sender)->setFlippedX(false);
        }
        else
        {
            ((Sprite*)sender)->setFlippedX(true);
        }
    });
    auto action = RepeatForever::create(Sequence::create(actionDelay, actionIdle1, actionIdle2, actionMove, NULL));
    action->setTag(10);
    enemyUnit->runAction(action);
}

void LayerEventSunfishGame::enemyHit(cocos2d::Sprite* enemyUnit)
{
    // Food 제거
    _listEnemyUnit.eraseObject(enemyUnit);
    enemyUnit->stopAllActions();
    enemyUnit->removeFromParent();
    
    // Food 소환 시간 초기화
    int enemyCountNow = (int)_listEnemyUnit.size();
    int enemyCountMax = EventSunfishManager::getInstance()->getGameEnemyCountMax();
    if ( enemyCountNow >= enemyCountMax )
    {
        _enemySpawnTime = 0;
    }
}
void LayerEventSunfishGame::enemySave()
{
    std::vector<std::string> listEnemyIdx;
    
    for ( auto& enemyUnit :_listEnemyUnit )
    {
        int idx = enemyUnit->getTag();
        listEnemyIdx.push_back(std::to_string(idx));
    }
    
    auto str = UtilsString::getInstance()->splitJoin(listEnemyIdx, ",");
    UserDefault::getInstance()->setStringForKey(KEY_EVENT_SUNFISH_ENEMY, MafAes256::Encrypt(str.c_str()));
}

void LayerEventSunfishGame::feverSpawn()
{
    auto layout = _uiContentsBattle;
    auto size = layout->getContentSize();
    
    if ( _feverUnit == nullptr )
    {
        _feverUnit = Sprite::create("Assets/ui/event/lobster_gold_hareubang_1_1.png");
        layout->addChild(_feverUnit);
        
        auto effect1 = Sprite::create("Assets/ui_common/box_item_fx1_2.png");
        effect1->setPosition(Vec2(35, _feverUnit->getContentSize().height / 2));
        effect1->runAction(RepeatForever::create(RotateBy::create(1.0, -180)));
        _feverUnit->addChild(effect1, -1);
        
        auto effect2 = Sprite::create("Assets/ui/event/event_fx_1_1.png");
        effect2->setPosition(Vec2(_feverUnit->getContentSize().width, _feverUnit->getContentSize().height / 2));
        _feverUnit->addChild(effect2, -1);
        {
            auto animation = Animation::create();
            animation->setDelayPerUnit(0.1f);
            for ( int i = 1; i <= 4; i++ )
            {
                auto ani = Sprite::create(MafUtils::format("Assets/ui/event/event_fx_1_%d.png", i));
                ani->getTexture()->setTexParameters(texParams);
                
                animation->addSpriteFrame(ani->getSpriteFrame());
            }
            effect2->runAction(RepeatForever::create(Animate::create(animation)));
        }
    }
    
    //
    auto animation = Animation::create();
    animation->setDelayPerUnit(0.2f);
    for ( int i = 1; i <= 2; i++ )
    {
        auto ani = Sprite::create(MafUtils::format("Assets/ui/event/lobster_gold_hareubang_1_%d.png", i));
        ani->getTexture()->setTexParameters(texParams);
        
        animation->addSpriteFrame(ani->getSpriteFrame());
    }
    _feverUnit->stopAllActions();
    _feverUnit->runAction(RepeatForever::create(Animate::create(animation)));
    
    //
    int posX = size.width + _feverUnit->getBoundingBox().size.width;
    int posY = size.height / 2;
    _feverUnit->setPosition(Vec2(posX, posY));
    
    //
    auto actionMove = MoveTo::create(30, Vec2(-_feverUnit->getBoundingBox().size.width, size.height / 2));
    auto actionEvent = CallFunc::create([=](){
                
        auto feverTime = UtilsDate::getInstance()->getTime() + EventSunfishManager::getInstance()->getFeverCoolAgainTime();
        EventSunfishManager::getInstance()->setFeverTime(feverTime);

        _feverUnit->setVisible(false);
    });
    _feverUnit->runAction(Sequence::create(actionMove, actionEvent, NULL));
}
