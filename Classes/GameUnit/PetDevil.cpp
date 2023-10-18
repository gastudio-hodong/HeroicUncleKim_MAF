//
//  PetDevil.cpp
//  FantasyClicker
//
//  Created by SanHeo on 2015. 5. 14..
//
//

#include "PetDevil.h"

#include "GameUnit/UnitBehaviorHero.h"
#include "GameUnit/UnitBehaviorMonster.h"

#include "ManagerGame/UserInfoManager.h"
#include "ManagerGame/PetNewDevilManager.h"

USING_NS_CC;

Penguin::Penguin(){
    _ePet = E_PET::D_1_PENGUIN;
    _nAttackCount = 0;
    Vector<cocos2d::SpriteFrame*> _vec;
    Animation *animation;
    Animate *animate;
    
    _vec.clear();
    for(int i=1; i<=4; i++){
        std::string name = MafUtils::format("Assets/character_pet/devil/penguin/pet_01_attack_%d.png",i);
        auto ani = Sprite::create(name.c_str());
        ani->getTexture()->setTexParameters(texParams);
        ani->setScale(DevilScale);
        _vec.pushBack(ani->getSpriteFrame());
    }
    animation = Animation::createWithSpriteFrames(_vec, 0.1f);
    animate = Animate::create(animation);
    _animateAttack = animate;
    _animateAttack->retain();
    
    _vec.clear();
    for(int i=1; i<=4; i++){
        std::string name = MafUtils::format("Assets/character_pet/devil/penguin/pet_01_idle_%d.png",i);
        auto ani = Sprite::create(name.c_str());
        ani->getTexture()->setTexParameters(texParams);
        ani->setScale(DevilScale);
        _vec.pushBack(ani->getSpriteFrame());
    }
    animation = Animation::createWithSpriteFrames(_vec, 0.1f);
    animate = Animate::create(animation);
    _animateIdle = animate;
    _animateIdle->retain();
    
    _vec.clear();
    for(int i=1; i<=4; i++){
        std::string name = MafUtils::format("Assets/character_pet/devil/penguin/pet_01_walk_%d.png",i);
        auto ani = Sprite::create(name.c_str());
        ani->getTexture()->setTexParameters(texParams);
        ani->setScale(DevilScale);
        _vec.pushBack(ani->getSpriteFrame());
    }
    animation = Animation::createWithSpriteFrames(_vec, 0.1f);
    animate = Animate::create(animation);
    _animateMove = animate;
    _animateMove->retain();
    
    move();
}

Penguin::~Penguin(){
}


Penguin* Penguin::create()
{
    Penguin *sprite = new (std::nothrow) Penguin();
    if (sprite && sprite->init())//initWithFile("Assets/character_pet/normal/handsome/pet_handsome_idle_1.png"))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

void Penguin::update(float dt){
    
    if(_eState == MOVE){
        unscheduleUpdate();
        return;
    }
    
    auto count = UserInfoManager::getInstance()->getHitCount(_ePlace);
    if(_nAttackCount ==count)
        return;
    
    _nAttackCount = count;
    
    if(count>0 && count%5==0){
//    if(_nAttackCount>=5){
//        _nAttackCount-=5;
        if(_enemy==nullptr || _enemy->isDie()){
            return;
        }
        SoundManager::SoundEffectStart(SOUND_PET_9);
        
        CallFunc *call = CallFunc::create(CC_CALLBACK_0(Pet::idle,this));
        Sequence *seq = Sequence::create(_animateAttack,call, nullptr);
        if(_eState != E_HERO_STATE::ATTACK){
            stopAllActions();
            runAction(seq);
        }
        
        _eState = ATTACK;
        
        auto bCri   = UserInfoManager::getInstance()->getCriticalRate(_ePlace, false, false);
        std::string myAtk = UserInfoManager::getInstance()->getAttackDamage(_ePlace,E_CHARACTER::CHA_D_PET_PENGUIN, bCri, false);
        _enemy->actionHit(E_CHARACTER::CHA_D_PET_PENGUIN, E_CHARACTER_HIT::HIT_NORMAL, myAtk, bCri);
    }
}

#pragma mark-
Hedgehog::Hedgehog() :
_nAttackCount(0),

_move(nullptr),
_moveBack(nullptr)
{
    _ePet = E_PET::D_2_HEDGEHOG;
    
    Vector<cocos2d::SpriteFrame*> _vec;
    Animation *animation;
    Animate *animate;
    
    _vec.clear();
    for(int i=1; i<=4; i++){
        std::string name = MafUtils::format("Assets/character_pet/devil/hedgehog/pet_02_attack_%d.png",i);
        auto ani = Sprite::create(name.c_str());
        ani->getTexture()->setTexParameters(texParams);
        ani->setScale(DevilScale);
        _vec.pushBack(ani->getSpriteFrame());
    }
    animation = Animation::createWithSpriteFrames(_vec, 0.1f);
    animate = Animate::create(animation);
    _animateAttack = animate;
    _animateAttack->retain();
    
    _moveBack = JumpTo::create(0.2, Vec2(-90, 36), 120,1);
    _moveBack->retain();
    _move = MoveTo::create(0.1, Vec2(120, 36));
    _move->retain();
    
    
    _vec.clear();
    for(int i=1; i<=4; i++){
        std::string name = MafUtils::format("Assets/character_pet/devil/hedgehog/pet_02_idle_%d.png",i);
        auto ani = Sprite::create(name.c_str());
        ani->getTexture()->setTexParameters(texParams);
        ani->setScale(DevilScale);
        _vec.pushBack(ani->getSpriteFrame());
    }
    animation = Animation::createWithSpriteFrames(_vec, 0.1f);
    animate = Animate::create(animation);
    _animateIdle = animate;
    _animateIdle->retain();
    
    _vec.clear();
    for(int i=1; i<=4; i++){
        std::string name = MafUtils::format("Assets/character_pet/devil/hedgehog/pet_02_walk_%d.png",i);
        auto ani = Sprite::create(name.c_str());
        ani->getTexture()->setTexParameters(texParams);
        ani->setScale(DevilScale);
        _vec.pushBack(ani->getSpriteFrame());
    }
    animation = Animation::createWithSpriteFrames(_vec, 0.1f);
    animate = Animate::create(animation);
    _animateMove = animate;
    _animateMove->retain();
    
    move();
}

Hedgehog::~Hedgehog()
{
    if ( _moveBack != nullptr )
    {
        _moveBack->release();
        _moveBack = nullptr;
    }
    
    if ( _move != nullptr )
    {
        _move->release();
        _move = nullptr;
    }
}

Hedgehog* Hedgehog::create()
{
    Hedgehog *sprite = new (std::nothrow) Hedgehog();
    if (sprite && sprite->init())//initWithFile("Assets/character_pet/normal/handsome/pet_handsome_idle_1.png"))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

void Hedgehog::update(float dt){
    
    if(_eState == MOVE){
        unscheduleUpdate();
        return;
    }
    
    _coolTime -= dt;
    if(_coolTime<=0){
        if(_enemy==nullptr || _enemy->isDie())
        {
            return;
        }
        
        int count = UserInfoManager::getInstance()->getHitCountDevilHedgehog();
        if(count==10)
            return;
        
        count++;
        UserInfoManager::getInstance()->setHitCountDevilHedgehog(count);
        
        SoundManager::SoundEffectStart(SOUND_PET_10);
        _eState = E_HERO_STATE::ATTACK;
        _coolTime = _coolTimeMax;
        
        auto attackPer = PetNewDevilManager::getInstance()->getPetEffect(_ePet, PetNewDevilManager::getInstance()->getPetLevel(_ePet));
        attackPer*=100;
        std::string myAtk = _enemy->getMonsterHp();
        myAtk = MafUtils::bigMulNum(myAtk, MafUtils::doubleToString(attackPer));
        if(myAtk.size()<=4){
            myAtk="1";
        }
        else{
            myAtk.pop_back();
            myAtk.pop_back();
            myAtk.pop_back();
            myAtk.pop_back();
        }
        if(myAtk=="0")
            myAtk = "1";
        
        _enemy->actionHit(E_CHARACTER::CHA_D_PET_HEDGEHOG, E_CHARACTER_HIT::HIT_NORMAL, myAtk, false);
        
        CallFunc *call = CallFunc::create(CC_CALLBACK_0(Pet::idle,this));
        Spawn *spawn = Spawn::create(_animateAttack,_move, nullptr);
//        Sequence *seq = Sequence::create(_animateAttack,call, nullptr);
        Sequence *seq = Sequence::create(spawn,_moveBack, call, nullptr);
        stopAllActions();
        runAction(seq);
        
    }
}

void Hedgehog::move(){
    if(_eState != MOVE)
    {
        stopAllActions();
    }
    Pet::move();
    setPosition(Vec2(-90, 36));
}

#pragma mark-
Gatekeeper::Gatekeeper(){
    
    _ePet = E_PET::D_3_GATEKEEPER;
    
    _nAttackCount = 0;
    _onBuffEffect = nullptr;
    
    
    Vector<cocos2d::SpriteFrame*> _vec;
    Animation *animation;
    Animate *animate;
    
    //
    _vec.clear();
    for(int i=1; i<=4; i++){
        std::string name = MafUtils::format("Assets/character_pet/devil/gatekeeper/pet_03_idle_%d.png",i);
        auto ani = Sprite::create(name.c_str());
        ani->getTexture()->setTexParameters(texParams);
        ani->setScale(DevilScale);
        _vec.pushBack(ani->getSpriteFrame());
    }
    animation = Animation::createWithSpriteFrames(_vec, 0.1f);
    animate = Animate::create(animation);
    _animateIdle = animate;
    _animateIdle->retain();
    
    //
    _vec.clear();
    for(int i=1; i<=4; i++){
        std::string name = MafUtils::format("Assets/character_pet/devil/gatekeeper/pet_03_walk_%d.png",i);
        auto ani = Sprite::create(name.c_str());
        ani->getTexture()->setTexParameters(texParams);
        ani->setScale(DevilScale);
        _vec.pushBack(ani->getSpriteFrame());
    }
    animation = Animation::createWithSpriteFrames(_vec, 0.1f);
    animate = Animate::create(animation);
    _animateMove = animate;
    _animateMove->retain();
 
    //
    _vec.clear();
    for(int i=1; i<=4; i++){
        std::string name = MafUtils::format("Assets/character_pet/devil/gatekeeper/pet_03_attack_%d.png",i);
        auto ani = Sprite::create(name.c_str());
        ani->getTexture()->setTexParameters(texParams);
        ani->setScale(DevilScale);
        _vec.pushBack(ani->getSpriteFrame());
    }
    animation = Animation::createWithSpriteFrames(_vec, 0.1f);
    animate = Animate::create(animation);
    _animateAttack = animate;
    _animateAttack->retain();
    
    move();
}

Gatekeeper::~Gatekeeper(){
    _onBuffEffect = nullptr;
}


Gatekeeper* Gatekeeper::create()
{
    Gatekeeper *sprite = new (std::nothrow) Gatekeeper();
    if (sprite && sprite->init())//initWithFile("Assets/character_pet/normal/handsome/pet_handsome_idle_1.png"))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

void Gatekeeper::update(float dt){

    _coolTime -= dt;
    
    if ( _coolTime <= 0 )
    {
        _eState = ATTACK;
        _coolTime = _coolTimeMax;
        
        int ran = random(1, 100);
        E_GATEKEEPER_BUFF buff = E_GATEKEEPER_BUFF::BUFF_NONE_GATEKEEPER;
        if(ran <=25)        buff = E_GATEKEEPER_BUFF::BUFF_ATTACK_GATEKEEPER;
        else if(ran <=50)   buff = E_GATEKEEPER_BUFF::BUFF_MOVESPPED_GATEKEEPER;
        else if(ran <=75)   buff = E_GATEKEEPER_BUFF::BUFF_CRITICAL_GATEKEEPER;
        else if(ran <=100)  buff = E_GATEKEEPER_BUFF::BUFF_ATTACKSPPED_GATEKEEPER;
        
        int level = PetNewDevilManager::getInstance()->getPetLevel(_ePet);
        auto per = PetNewDevilManager::getInstance()->getPetEffect(_ePet, level);
        _onBuffEffect(buff,per);
        
        
        stopAllActions();
        CallFunc *call = CallFunc::create(CC_CALLBACK_0(Pet::idle,this));
        Sequence *seq = Sequence::create(_animateAttack,call, nullptr);
        runAction(seq);
        
        //sound
        SoundManager::SoundEffectStart(SOUND_PET_11);
        runAction(Sequence::create(DelayTime::create(0.5f), CallFunc::create([=](){
            SoundManager::SoundEffectStart(SOUND_PET_11_BUFF);
        }), NULL));
    }
    
}


#pragma mark-
Flash::Flash(){
    
    _ePet = E_PET::D_4_FLASH;
    
    Vector<cocos2d::SpriteFrame*> _vec;
    Animation *animation;
    Animate *animate;
    
    _vec.clear();
    for(int i=1; i<=4; i++){
        std::string name = MafUtils::format("Assets/character_pet/devil/flash/pet_04_idle_%d.png",i);
        auto ani = Sprite::create(name.c_str());
        ani->getTexture()->setTexParameters(texParams);
        ani->setScale(DevilScale);
        _vec.pushBack(ani->getSpriteFrame());
    }
    animation = Animation::createWithSpriteFrames(_vec, 0.1f);
    animate = Animate::create(animation);
    _animateIdle = animate;
    _animateIdle->retain();
    
    _vec.clear();
    for(int i=1; i<=4; i++){
        std::string name = MafUtils::format("Assets/character_pet/devil/flash/pet_04_walk_%d.png",i);
        auto ani = Sprite::create(name.c_str());
        ani->getTexture()->setTexParameters(texParams);
        ani->setScale(DevilScale);
        _vec.pushBack(ani->getSpriteFrame());
    }
    animation = Animation::createWithSpriteFrames(_vec, 0.1f);
    animate = Animate::create(animation);
    _animateMove = animate;
    _animateMove->retain();
    
    _vec.clear();
    for(int i=1; i<=4; i++){
        std::string name = MafUtils::format("Assets/character_pet/devil/flash/pet_04_attack_%d.png",i);
        auto ani = Sprite::create(name.c_str());
        ani->getTexture()->setTexParameters(texParams);
        ani->setScale(DevilScale);
        _vec.pushBack(ani->getSpriteFrame());
    }
    
    animation = Animation::createWithSpriteFrames(_vec, 0.1f);
    animate = Animate::create(animation);
    _animateAttack = animate;
    _animateAttack->retain();
    
    
    _vec.clear();
    for(int i=1; i<=4; i++){
        std::string name = MafUtils::format("Assets/character_pet/devil/effect/pet_04_attack_effect_%d.png",i);
        auto ani = Sprite::create(name.c_str());
        ani->getTexture()->setTexParameters(texParams);
        ani->setScale(DevilScale);
        _vec.pushBack(ani->getSpriteFrame());
    }
    animation = Animation::createWithSpriteFrames(_vec, 0.025f);
    animate = Animate::create(animation);
    _aniAttackEffect = animate;
    _aniAttackEffect->retain();
    
    _sprAttackEffect = Sprite::create("Assets/character_pet/devil/effect/pet_04_attack_effect_1.png");
    _sprAttackEffect->setPosition(150, -10);
    _sprAttackEffect->setAnchorPoint(Vec2(0.5f, 0));
    _sprAttackEffect->setVisible(false);
    addChild(_sprAttackEffect);
    
    move();
}

Flash::~Flash(){
    
}


Flash* Flash::create()
{
    Flash *sprite = new (std::nothrow) Flash();
    if (sprite && sprite->init())//initWithFile("Assets/character_pet/normal/handsome/pet_handsome_idle_1.png"))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

void Flash::attack(UnitBehaviorMonster& mon){
    
    _enemy = &mon;
    
    if ( _eState == E_HERO_STATE::IDLE || _eState == E_HERO_STATE::ATTACK )
        return;
    
    if ( _enemy == nullptr || _enemy->isDie() == true )
        return;
    
    if( UserInfoManager::getInstance()->getHitCountDevilFlash() >= 1 )
        return;
    
    SoundManager::SoundEffectStart(SOUND_PET_DRAGON);
    
    _eState = E_HERO_STATE::ATTACK;
    _bNoIdle = false;
    
    int level = PetNewDevilManager::getInstance()->getPetLevel(_ePet);
    double attackPer = PetNewDevilManager::getInstance()->getPetEffect(_ePet, level);
    attackPer *= 10;
    
    std::string myAtk = _enemy->getMonsterHp();
    myAtk = MafUtils::bigMulNum(myAtk, MafUtils::toString(attackPer));
    if (myAtk.length() > 3) {
        myAtk.pop_back();
        myAtk.pop_back();
        myAtk.pop_back();
    }
    
    _enemy->actionHit(E_CHARACTER::CHA_D_PET_FLASH, E_CHARACTER_HIT::HIT_NORMAL, myAtk, false);
    _sprAttackEffect->runAction(Sequence::create(Show::create(), _aniAttackEffect, Hide::create(), nullptr));
    
    stopAllActions();
    CallFunc *call = CallFunc::create(CC_CALLBACK_0(Pet::idle,this));
    Sequence *seq = Sequence::create(_animateAttack,call, nullptr);
    runAction(seq);
}
