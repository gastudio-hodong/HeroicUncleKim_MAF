//
//  Pet.cpp
//  FantasyClicker
//
//  Created by SanHeo on 2015. 5. 14..
//
//

#include "Pet.h"

#include "GameObject/InfoArtifact.h"

#include "GameUnit/UnitBehaviorHero.h"
#include "GameUnit/UnitBehaviorMonster.h"

#include "ManagerGame/UserInfoManager.h"
#include "ManagerGame/UserInfoStackManager.h"
#include "ManagerGame/PetNewManager.h"
#include "ManagerGame/DataManager.h"
#include "ManagerGame/CostumeManager.h"
#include "ManagerGame/AdventureRelicManager.h"
#include "ManagerGame/ItemsMoneyManager.h"

USING_NS_CC;

Pet::Pet() :
_hero(nullptr),
_enemy(nullptr),

_bSkinUse(true),
_nZorder(0)
{
    _ePet = E_PET::PET_NONE;
    _eState = E_HERO_STATE::NONE;
    _coolTimeMax = 1;
    _coolTime = 0;

    _ePlace = E_PLACE::DG_NORMAL;
    
    _sprRing = nullptr;
}

Pet::~Pet()
{

    unscheduleUpdate();
    stopAllActions();
    
    _animateIdle->release();
    _animateMove->release();
    _animateAttack->release();
    
    _animateIdle = nullptr;
    _animateMove = nullptr;
    _animateAttack = nullptr;
    
    _sprRing = nullptr;
}

void Pet::update(float dt){
    

}


void Pet::idle(){
    if(_eState == E_HERO_STATE::IDLE)
        return;
    
    if ( _ePet != E_PET::HAMSTER_2 && _ePet != E_PET::D_3_GATEKEEPER && _bNoIdle )
        return;
    
    stopAllActions();
    _eState = E_HERO_STATE::IDLE;
    
    RepeatForever *repeat = RepeatForever::create(_animateIdle);
    runAction(repeat);
    
    if(_ePet != E_PET::DRAGON_3 && _ePet != E_PET::D_4_FLASH)
        scheduleUpdate();
//    if(!Director::getInstance()->getScheduler()->isScheduled(schedule_selector(Pet::update), this))
//        Director::getInstance()->getScheduler()->schedule(schedule_selector(Pet::update),
//                                                      this, 0.1 , kRepeatForever, 0, false);
    
}
void Pet::move(){
    if(_eState == E_HERO_STATE::MOVE)
        return;

    if(_eState==E_HERO_STATE::IDLE)
        stopAllActions();
    
    _eState = E_HERO_STATE::MOVE;
    _bNoIdle = true;

    
    RepeatForever *repeat = RepeatForever::create(_animateMove);
    runAction(repeat);
}
void Pet::attack(UnitBehaviorMonster& mon){
    
    _enemy = &mon;
    
    if(_eState == E_HERO_STATE::IDLE || _eState == E_HERO_STATE::ATTACK)
        return;
    //    _eState = E_HERO_STATE::IDLE;
    _bNoIdle = false;
    idle();
}

void Pet::setHero(UnitBehaviorHero* hero)
{
    _hero = hero;
}

void Pet::setCoolTime(double time, bool bReset)
{
    _coolTimeMax = time;
    if ( bReset )
    {
        _coolTime = _coolTimeMax;
    }
    
    if ( _coolTime > _coolTimeMax )
        _coolTime = _coolTimeMax;
    
}

void Pet::setPlace(E_PLACE ePlace)
{
    _ePlace = ePlace;
}

void Pet::setSkinUse(bool bSkinUse){
    _bSkinUse = bSkinUse;
}


void Pet::changeState(E_HERO_STATE state){
    _eState = state;
}

void Pet::hideRing(){
    _sprRing->setVisible(false);
}

void Pet::showRing(){
    _sprRing->setVisible(true);
}

void Pet::setZorderNum(int value)
{
    _nZorder = value;
}

int Pet::getZorderNum()
{
    return _nZorder;
}

#pragma mark-

Chick::Chick(){
    
    _ePet = E_PET::CHICK_1;
    _attackCount = 0;
    
    Vector<cocos2d::SpriteFrame*> _vec;
    Animation *animation;
    Animate *animate;
    
    for(int i=1; i<=4; i++){
        std::stringstream name;
        name << "Assets/character_pet/normal/chick/pet_chick_idle_";
        name << i;
        name << ".png";
        auto ani = Sprite::create(name.str());
        _vec.pushBack(ani->getSpriteFrame());
    }
    animation = Animation::createWithSpriteFrames(_vec, 0.1f);
    animate = Animate::create(animation);
    _animateIdle = animate;
    _animateIdle->retain();
    
    _vec.clear();
    for(int i=1; i<=4; i++){
        std::stringstream name;
        name << "Assets/character_pet/normal/chick/pet_chick_walk_";
        name << i;
        name << ".png";
        auto ani = Sprite::create(name.str());
        _vec.pushBack(ani->getSpriteFrame());
    }
    animation = Animation::createWithSpriteFrames(_vec, 0.1f);
    animate = Animate::create(animation);
    _animateMove = animate;
    _animateMove->retain();
    
    _vec.clear();
    for(int i=1; i<=4; i++){
        std::stringstream name;
        name << "Assets/character_pet/normal/chick/pet_chick_attack_";
        name << i;
        name << ".png";
        auto ani = Sprite::create(name.str());
        _vec.pushBack(ani->getSpriteFrame());
    }
    animation = Animation::createWithSpriteFrames(_vec, 0.1f);
    animate = Animate::create(animation);
    _animateAttack = animate;
    _animateAttack->retain();
//    _seqMoveBody = Sequence::create(animate, nullptr);
//    _seqMoveBody->retain();

//    idle();
    
    _vec.clear();
    for(int i=1; i<=4; i++){
        std::stringstream name;
        name << "Assets/character_pet/normal/chick_evolution/pet_chick_attack_effect_";
        name << i;
        name << ".png";
        auto ani = Sprite::create(name.str());
        _vec.pushBack(ani->getSpriteFrame());
    }
    animation = Animation::createWithSpriteFrames(_vec, 0.1f);
    animate = Animate::create(animation);
    _aniEvolutionSKill = animate;
    _aniEvolutionSKill->retain();
    
    _sprEvolutionSkill = Sprite::create("Assets/character_pet/normal/chick_evolution/pet_chick_attack_effect_1.png");
    _sprEvolutionSkill->setPosition(250, 0);
    _sprEvolutionSkill->setAnchorPoint(Vec2(0.5f, 0));
    _sprEvolutionSkill->setVisible(false);
    addChild(_sprEvolutionSkill);
    
    _sprRing = Sprite::create("Assets/character_hero/normal/ring_pet.png");
    _sprRing->setPosition(60, 80);
    _sprRing->setVisible(false);
    addChild(_sprRing);
    
    //
    setEvolution();
    move();
}
Chick::~Chick(){
    _aniEvolutionSKill->release();
}
void Chick::setEvolution()
{
    bool bSkin = false;
    std::string strPath = "Assets/character_pet/normal/";
    if ( PetNewManager::getInstance()->getPetSkin(_ePet) > 0 && _bSkinUse == true )
    {
        std::string strTemp = strPath;
        strTemp.append( MafUtils::format("chick_skin_%d", PetNewManager::getInstance()->getPetSkin(_ePet)) );
        
        if ( FileUtils::getInstance()->isDirectoryExist(strTemp) == true )
        {
            bSkin = true;
            strPath = strTemp;
            strPath.append("/");
        }
    }
    
    if ( bSkin == false )
    {
        if ( PetNewManager::getInstance()->getPetEvolution(_ePet) > 0 )
        {
            bSkin = true;
            strPath.append("chick_evolution/");
        }
        else
        {
            bSkin = false;
            strPath.append("chick/");
        }
    }
    
    _sprRing->setPosition(60, 90);
    
    Vector<cocos2d::SpriteFrame*> _vec;
    Animation *animation;
    Animate *animate;
    
    _vec.clear();
    _animateIdle->release();
    for ( int i = 1; i <= 4; i++ )
    {
        std::string strTemp = strPath + "pet_chick_idle_%d.png";
        strTemp = MafUtils::format(strTemp.c_str(), i);

        auto ani = Sprite::create(strTemp);
        _vec.pushBack(ani->getSpriteFrame());
    }
    animation = Animation::createWithSpriteFrames(_vec, 0.1f);
    animate = Animate::create(animation);
    _animateIdle = animate;
    _animateIdle->retain();
    
    _vec.clear();
    _animateMove->release();
    for ( int i = 1; i <= 4; i++ )
    {
        std::string strTemp = strPath + "pet_chick_walk_%d.png";
        strTemp = MafUtils::format(strTemp.c_str(), i);
        
        auto ani = Sprite::create(strTemp);
        _vec.pushBack(ani->getSpriteFrame());
    }
    animation = Animation::createWithSpriteFrames(_vec, 0.1f);
    animate = Animate::create(animation);
    _animateMove = animate;
    _animateMove->retain();
    
    _vec.clear();
    _animateAttack->release();
    for ( int i = 1; i <= 4; i++ )
    {
        std::string strTemp = strPath + "pet_chick_attack_%d.png";
        strTemp = MafUtils::format(strTemp.c_str(), i);
        
        auto ani = Sprite::create(strTemp);
        _vec.pushBack(ani->getSpriteFrame());
    }
    animation = Animation::createWithSpriteFrames(_vec, 0.1f);
    animate = Animate::create(animation);
    _animateAttack = animate;
    _animateAttack->retain();
    
    
    if( _eState == E_HERO_STATE::IDLE )
    {
        _eState = E_HERO_STATE::NONE;
        idle();
    }
    else if( _eState == E_HERO_STATE::MOVE )
    {
        _eState = E_HERO_STATE::IDLE;
        move();
    }
}

void Chick::setEvolutionDummy(int nIdx)
{
    bool bSkin = false;
    std::string strPath = "Assets/character_pet/normal/";
    if ( PetNewManager::getInstance()->getPetSkin(_ePet) > 0 && _bSkinUse == true )
    {
        std::string strTemp = strPath;
        strTemp.append( MafUtils::format("chick_skin_%d", nIdx) );
        
        if ( FileUtils::getInstance()->isDirectoryExist(strTemp) == true )
        {
            bSkin = true;
            strPath = strTemp;
            strPath.append("/");
        }
    }
    
    if ( bSkin == false )
    {
        if ( PetNewManager::getInstance()->getPetEvolution(_ePet) > 0 )
        {
            bSkin = true;
            strPath.append("chick_evolution/");
        }
        else
        {
            bSkin = false;
            strPath.append("chick/");
        }
    }
    
    _sprRing->setPosition(60, 90);
    
    Vector<cocos2d::SpriteFrame*> _vec;
    Animation *animation;
    Animate *animate;
    
    _vec.clear();
    _animateIdle->release();
    for ( int i = 1; i <= 4; i++ )
    {
        std::string strTemp = strPath + "pet_chick_idle_%d.png";
        strTemp = MafUtils::format(strTemp.c_str(), i);

        auto ani = Sprite::create(strTemp);
        _vec.pushBack(ani->getSpriteFrame());
    }
    animation = Animation::createWithSpriteFrames(_vec, 0.1f);
    animate = Animate::create(animation);
    _animateIdle = animate;
    _animateIdle->retain();
    
    _vec.clear();
    _animateMove->release();
    for ( int i = 1; i <= 4; i++ )
    {
        std::string strTemp = strPath + "pet_chick_walk_%d.png";
        strTemp = MafUtils::format(strTemp.c_str(), i);
        
        auto ani = Sprite::create(strTemp);
        _vec.pushBack(ani->getSpriteFrame());
    }
    animation = Animation::createWithSpriteFrames(_vec, 0.1f);
    animate = Animate::create(animation);
    _animateMove = animate;
    _animateMove->retain();
    
    _vec.clear();
    _animateAttack->release();
    for ( int i = 1; i <= 4; i++ )
    {
        std::string strTemp = strPath + "pet_chick_attack_%d.png";
        strTemp = MafUtils::format(strTemp.c_str(), i);
        
        auto ani = Sprite::create(strTemp);
        _vec.pushBack(ani->getSpriteFrame());
    }
    animation = Animation::createWithSpriteFrames(_vec, 0.1f);
    animate = Animate::create(animation);
    _animateAttack = animate;
    _animateAttack->retain();
    
    
    if( _eState == E_HERO_STATE::IDLE )
    {
        _eState = E_HERO_STATE::NONE;
        idle();
    }
    else if( _eState == E_HERO_STATE::MOVE )
    {
        _eState = E_HERO_STATE::IDLE;
        move();
    }
}

Chick* Chick::create()
{
    Chick *sprite = new (std::nothrow) Chick();
    if (sprite && sprite->initWithFile("Assets/character_pet/normal/chick/pet_chick_idle_1.png"))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

void Chick::update(float dt){
    
    if(_eState == MOVE){
        unscheduleUpdate();
        return;
    }
    
    _coolTime -= dt;
    if(_coolTime<=0){
        
        if(_enemy==nullptr){
            return;
        }
        if(_enemy->isDie()){
            return;
        }
        
        CallFunc *call = CallFunc::create(CC_CALLBACK_0(Pet::idle,this));
        Sequence *seq = Sequence::create(_animateAttack,call, nullptr);
        if(_eState != E_HERO_STATE::ATTACK){
            stopAllActions();
            runAction(seq);
        }
        
        _eState = ATTACK;
        _coolTime = _coolTimeMax;
        
        auto artiInfo = DataManager::GetArtifactInfo(E_ARTIFACT::ARTI_7);
        auto lv = UserInfoManager::getInstance()->getArtifactLevel(_ePlace, E_ARTIFACT::ARTI_7);
        auto max = artiInfo->getLevelMax();
        if(lv != max){
            SoundManager::SoundEffectStart(SOUND_PET_KRRR_ATTACK);
        }
        else{
            _attackCount++;
            if(_attackCount%4==0){
                SoundManager::SoundEffectStart(SOUND_PET_KRRR_ATTACK);
            }
        }
        
        auto bCri   = UserInfoManager::getInstance()->getCriticalRate(_ePlace, false, false);
        std::string myAtk = UserInfoManager::getInstance()->getAttackDamage(_ePlace,E_CHARACTER::CHA_PET_CHICK, bCri, false);
        _enemy->actionHit(E_CHARACTER::CHA_PET_CHICK, E_CHARACTER_HIT::HIT_NORMAL, myAtk, bCri);

        int normalLevel = PetNewManager::getInstance()->getPetLevel(E_PET::CHICK_1);
        int evolutionLevel = PetNewManager::getInstance()->getPetEvolution(E_PET::CHICK_1);
        if (evolutionLevel >= 1) {
            int rnd = random(1, 100);
            int per = PetNewManager::getInstance()->getPetEvolutionEffect2(E_PET::CHICK_1, evolutionLevel);
            
            if (rnd <= per) {
                //공격력 8배 추가 데미지
                int mul = 8;
                
                if(CostumeManager::getInstance()->isExist(E_COSTUME::SETITEM, E_COSTUME_IDX::IDX_2)){
                    mul *= 10;
                }
                
                double evolutionPer = PetNewManager::getInstance()->getPetEvolutionEffect1(E_PET::CHICK_1, evolutionLevel);
                if (evolutionPer > 0) {
                    mul *= evolutionPer;
                }
                
                if (CostumeManager::getInstance()->isEquip(E_COSTUME::BODY,E_COSTUME_IDX::IDX_26)) {
                    mul += mul * (normalLevel * 0.2f);
                }
                
                mul *= PetNewManager::getInstance()->getPetEquipEffect(103, 1);
                
                std::string skillAtk = myAtk;
                skillAtk = MafUtils::bigMulNum(skillAtk, MafUtils::doubleToString(mul * 100));
                skillAtk.pop_back();
                skillAtk.pop_back();
                
                //모험 유물 적용
                double advtRelicBuff = 100.f;
                advtRelicBuff += AdventureRelicManager::getInstance()->getRelicTotalEffect(E_ADVENTURE_PROPERTY_TYPE::CHICK_SKILL_ATK);
                
                skillAtk = MafUtils::bigMulNum(skillAtk, MafUtils::doubleToString(advtRelicBuff));
                skillAtk.pop_back();
                skillAtk.pop_back();
                
                _enemy->actionHit(E_CHARACTER::CHA_PET_CHICK_EVOLUTION, E_CHARACTER_HIT::HIT_NORMAL, skillAtk, bCri);
                _sprEvolutionSkill->runAction(Sequence::create(Show::create(), _aniEvolutionSKill, Hide::create(), nullptr));
            }
        }
    }
}

#pragma mark-

Hamster::Hamster(){
    
    _ePet = E_PET::HAMSTER_2;
    
    
    Vector<cocos2d::SpriteFrame*> _vec;
    Animation *animation;
    Animate *animate;
    
    for(int i=1; i<=4; i++){
        std::stringstream name;
        name << "Assets/character_pet/normal/hamster/pet_hamster_idle_";
        name << i;
        name << ".png";
        auto ani = Sprite::create(name.str());
        _vec.pushBack(ani->getSpriteFrame());
    }
    animation = Animation::createWithSpriteFrames(_vec, 0.1f);
    animate = Animate::create(animation);
    _animateIdle = animate;
    _animateIdle->retain();
    
    _vec.clear();
    for(int i=1; i<=4; i++){
        std::stringstream name;
        name << "Assets/character_pet/normal/hamster/pet_hamster_walk_";
        name << i;
        name << ".png";
        auto ani = Sprite::create(name.str());
        _vec.pushBack(ani->getSpriteFrame());
    }
    animation = Animation::createWithSpriteFrames(_vec, 0.1f);
    animate = Animate::create(animation);
    _animateMove = animate;
    _animateMove->retain();
    
    _vec.clear();
    for(int i=1; i<=4; i++){
        std::stringstream name;
        name << "Assets/character_pet/normal/hamster/pet_hamster_jump_";
        name << i;
        name << ".png";
        auto ani = Sprite::create(name.str());
        _vec.pushBack(ani->getSpriteFrame());
    }
    animation = Animation::createWithSpriteFrames(_vec, 0.1f);
    animate = Animate::create(animation);
    _animateAttack = animate;
    _animateAttack->retain();
    //    _seqMoveBody = Sequence::create(animate, nullptr);
    //    _seqMoveBody->retain();
    
    //    idle();
    _sprRing = Sprite::create("Assets/character_hero/normal/ring_pet.png");
    _sprRing->setPosition(105, 100);
    _sprRing->setVisible(false);
    addChild(_sprRing);
    
    //
    setEvolution();
    move();
}
Hamster::~Hamster(){
    
}
void Hamster::setEvolution()
{
    bool bSkin = false;
    std::string strPath = "Assets/character_pet/normal/";
    if ( PetNewManager::getInstance()->getPetSkin(_ePet) > 0 && _bSkinUse == true )
    {
        std::string strTemp = strPath;
        strTemp.append( MafUtils::format("hamster_skin_%d", PetNewManager::getInstance()->getPetSkin(_ePet)) );
        
        if ( FileUtils::getInstance()->isDirectoryExist(strTemp) == true )
        {
            bSkin = true;
            strPath = strTemp;
            strPath.append("/");
        }
    }
    
    if ( bSkin == false )
    {
        if ( PetNewManager::getInstance()->getPetEvolution(_ePet) > 0 )
        {
            bSkin = true;
            strPath.append("hamster_evolution/");
        }
        else
        {
            bSkin = false;
            strPath.append("hamster/");
        }
    }
    
    _sprRing->setPosition(100, 90);
    
    Vector<cocos2d::SpriteFrame*> _vec;
    Animation *animation;
    Animate *animate;
    
    _vec.clear();
    _animateIdle->release();
    for ( int i = 1; i <= 4; i++ )
    {
        std::string strTemp = strPath + "pet_hamster_idle_%d.png";
        strTemp = MafUtils::format(strTemp.c_str(), i);
        
        auto ani = Sprite::create(strTemp);
        _vec.pushBack(ani->getSpriteFrame());
    }
    animation = Animation::createWithSpriteFrames(_vec, 0.1f);
    animate = Animate::create(animation);
    _animateIdle = animate;
    _animateIdle->retain();
    
    _vec.clear();
    _animateMove->release();
    for ( int i = 1; i <= 4; i++ )
    {
        std::string strTemp = strPath + "pet_hamster_walk_%d.png";
        strTemp = MafUtils::format(strTemp.c_str(), i);
        
        auto ani = Sprite::create(strTemp);
        _vec.pushBack(ani->getSpriteFrame());
    }
    animation = Animation::createWithSpriteFrames(_vec, 0.1f);
    animate = Animate::create(animation);
    _animateMove = animate;
    _animateMove->retain();
    
    _vec.clear();
    _animateAttack->release();
    for ( int i = 1; i <= 4; i++ )
    {
        std::string strTemp = strPath + "pet_hamster_jump_%d.png";
        strTemp = MafUtils::format(strTemp.c_str(), i);
        
        auto ani = Sprite::create(strTemp);
        _vec.pushBack(ani->getSpriteFrame());
    }
    animation = Animation::createWithSpriteFrames(_vec, 0.1f);
    animate = Animate::create(animation);
    _animateAttack = animate;
    _animateAttack->retain();
    
    if( _eState == E_HERO_STATE::IDLE )
    {
        _eState = E_HERO_STATE::NONE;
        idle();
    }
    else if( _eState == E_HERO_STATE::MOVE )
    {
        _eState = E_HERO_STATE::IDLE;
        move();
    }
}

void Hamster::setEvolutionDummy(int nIdx)
{
    bool bSkin = false;
    std::string strPath = "Assets/character_pet/normal/";
    if ( PetNewManager::getInstance()->getPetSkin(_ePet) > 0 && _bSkinUse == true )
    {
        std::string strTemp = strPath;
        strTemp.append( MafUtils::format("hamster_skin_%d", nIdx) );
        
        if ( FileUtils::getInstance()->isDirectoryExist(strTemp) == true )
        {
            bSkin = true;
            strPath = strTemp;
            strPath.append("/");
        }
    }
    
    if ( bSkin == false )
    {
        if ( PetNewManager::getInstance()->getPetEvolution(_ePet) > 0 )
        {
            bSkin = true;
            strPath.append("hamster_evolution/");
        }
        else
        {
            bSkin = false;
            strPath.append("hamster/");
        }
    }
    
    _sprRing->setPosition(100, 90);
    
    Vector<cocos2d::SpriteFrame*> _vec;
    Animation *animation;
    Animate *animate;
    
    _vec.clear();
    _animateIdle->release();
    for ( int i = 1; i <= 4; i++ )
    {
        std::string strTemp = strPath + "pet_hamster_idle_%d.png";
        strTemp = MafUtils::format(strTemp.c_str(), i);
        
        auto ani = Sprite::create(strTemp);
        _vec.pushBack(ani->getSpriteFrame());
    }
    animation = Animation::createWithSpriteFrames(_vec, 0.1f);
    animate = Animate::create(animation);
    _animateIdle = animate;
    _animateIdle->retain();
    
    _vec.clear();
    _animateMove->release();
    for ( int i = 1; i <= 4; i++ )
    {
        std::string strTemp = strPath + "pet_hamster_walk_%d.png";
        strTemp = MafUtils::format(strTemp.c_str(), i);
        
        auto ani = Sprite::create(strTemp);
        _vec.pushBack(ani->getSpriteFrame());
    }
    animation = Animation::createWithSpriteFrames(_vec, 0.1f);
    animate = Animate::create(animation);
    _animateMove = animate;
    _animateMove->retain();
    
    _vec.clear();
    _animateAttack->release();
    for ( int i = 1; i <= 4; i++ )
    {
        std::string strTemp = strPath + "pet_hamster_jump_%d.png";
        strTemp = MafUtils::format(strTemp.c_str(), i);
        
        auto ani = Sprite::create(strTemp);
        _vec.pushBack(ani->getSpriteFrame());
    }
    animation = Animation::createWithSpriteFrames(_vec, 0.1f);
    animate = Animate::create(animation);
    _animateAttack = animate;
    _animateAttack->retain();
    
    if( _eState == E_HERO_STATE::IDLE )
    {
        _eState = E_HERO_STATE::NONE;
        idle();
    }
    else if( _eState == E_HERO_STATE::MOVE )
    {
        _eState = E_HERO_STATE::IDLE;
        move();
    }
}

Hamster* Hamster::create()
{
    Hamster *sprite = new (std::nothrow) Hamster();
    if (sprite && sprite->initWithFile("Assets/character_pet/normal/hamster/pet_hamster_idle_1.png"))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

void Hamster::update(float dt){
    
    _coolTime -= dt;
    if ( _coolTime <= 0 )
    {
        _eState = ATTACK;
        _coolTime = _coolTimeMax;
        
        //
        SoundManager::SoundEffectStart(SOUND_PET_HAMSTER);
        
        //
        std::string gold = UserInfoManager::getInstance()->getEarnGoldFinal(DG_NORMAL, SECOND_DEFAULT);
        if ( gold.compare("0") != 0 )
        {
            gold = MafUtils::bigMulNum(gold, MafUtils::doubleToString(getAbilityMagnification()));
            
            int evolutionLevel = PetNewManager::getInstance()->getPetEvolution(E_PET::HAMSTER_2);
            double evolutionPer = PetNewManager::getInstance()->getPetEvolutionEffect1(E_PET::HAMSTER_2, evolutionLevel);
            if (evolutionPer > 0)
            {
                gold = MafUtils::bigMulNum(gold, MafUtils::doubleToString(evolutionPer));
            }
            
            int reinforceLevel = PetNewManager::getInstance()->getPetReinforce(E_PET::HAMSTER_2);
            double reinforcePer = PetNewManager::getInstance()->getPetReinforceEffect(E_PET::HAMSTER_2, reinforceLevel);
            if (reinforcePer > 0)
            {
                gold = MafUtils::bigMulNum(gold, MafUtils::doubleToString(reinforcePer));
            }
            
            gold = MafUtils::bigMulNum(gold, MafUtils::doubleToString(PetNewManager::getInstance()->getPetEquipEffect(200, 1)));
            gold = MafUtils::bigMulNum(gold, MafUtils::doubleToString(PetNewManager::getInstance()->getPetEquipEffect(202, 1)));
            
            ItemsMoneyManager::getInstance()->setGold(gold);
            RefreshManager::getInstance()->refreshCurrencyUI(E_ITEMS::GOLD, gold, true);
        }
        auto call = CallFunc::create(CC_CALLBACK_0(Pet::idle,this));
        auto seq = Sequence::create(_animateAttack,call, nullptr);
    
        stopAllActions();
        runAction(seq);
    }
}

int Hamster::getAbilityMagnification()
{
    int lv = PetNewManager::getInstance()->getPetLevel(HAMSTER_2);
    
    if(lv >= PetNewManager::getInstance()->getPetLevelMax(HAMSTER_2))
        lv = PetNewManager::getInstance()->getPetLevelMax(HAMSTER_2);
    
    return std::pow(lv, 3) + 24;
}

#pragma mark-
Puriring::Puriring(){
    _ePet = E_PET::PURIRING_5;
    
    Vector<cocos2d::SpriteFrame*> _vec;
    Animation *animation;
    Animate *animate;
    
    for(int i=1; i<=4; i++){
        std::stringstream name;
        name << "Assets/character_pet/normal/puriring/pet_puriring_idle_";
        name << i;
        name << ".png";
        auto ani = Sprite::create(name.str());
        _vec.pushBack(ani->getSpriteFrame());
    }
    animation = Animation::createWithSpriteFrames(_vec, 0.1f);
    animate = Animate::create(animation);
    _animateIdle = animate;
    _animateIdle->retain();
    
    _vec.clear();
    for(int i=1; i<=4; i++){
        std::stringstream name;
        name << "Assets/character_pet/normal/puriring/pet_puriring_walk_";
        name << i;
        name << ".png";
        auto ani = Sprite::create(name.str());
        _vec.pushBack(ani->getSpriteFrame());
    }
    animation = Animation::createWithSpriteFrames(_vec, 0.1f);
    animate = Animate::create(animation);
    _animateMove = animate;
    _animateMove->retain();
    
    _vec.clear();
    for(int i=1; i<=4; i++){
        std::stringstream name;
        name << "Assets/character_pet/normal/puriring/pet_puriring_jump_";
        name << i;
        name << ".png";
        auto ani = Sprite::create(name.str());
        _vec.pushBack(ani->getSpriteFrame());
    }
    animation = Animation::createWithSpriteFrames(_vec, 0.1f);
    animate = Animate::create(animation);
    _animateAttack = animate;
    _animateAttack->retain();
    //    _seqMoveBody = Sequence::create(animate, nullptr);
    //    _seqMoveBody->retain();
    _sprRing = Sprite::create("Assets/character_hero/normal/ring_pet.png");
    _sprRing->setPosition(65, 80);
    _sprRing->setVisible(false);
    addChild(_sprRing);
    
    //
    setEvolution();
    move();
}
Puriring::~Puriring(){
    
}
void Puriring::setEvolution()
{
    bool bSkin = false;
    std::string strPath = "Assets/character_pet/normal/";
    if ( PetNewManager::getInstance()->getPetSkin(_ePet) > 0 && _bSkinUse == true )
    {
        std::string strTemp = strPath;
        strTemp.append( MafUtils::format("puriring_skin_%d", PetNewManager::getInstance()->getPetSkin(_ePet)) );
        
        if ( FileUtils::getInstance()->isDirectoryExist(strTemp) == true )
        {
            bSkin = true;
            strPath = strTemp;
            strPath.append("/");
        }
    }
    
    if ( bSkin == false )
    {
        if ( PetNewManager::getInstance()->getPetEvolution(_ePet) > 0 )
        {
            bSkin = true;
            strPath.append("puriring_evolution/");
        }
        else
        {
            bSkin = false;
            strPath.append("puriring/");
        }
    }

    _sprRing->setPosition(65, 80);
    
    Vector<cocos2d::SpriteFrame*> _vec;
    Animation *animation;
    Animate *animate;
    
    _vec.clear();
    _animateIdle->release();
    for ( int i = 1; i <= 4; i++ )
    {
        std::string strTemp = strPath + "pet_puriring_idle_%d.png";
        strTemp = MafUtils::format(strTemp.c_str(), i);
        
        auto ani = Sprite::create(strTemp);
        _vec.pushBack(ani->getSpriteFrame());
    }
    animation = Animation::createWithSpriteFrames(_vec, 0.1f);
    animate = Animate::create(animation);
    _animateIdle = animate;
    _animateIdle->retain();
    
    _vec.clear();
    _animateMove->release();
    for ( int i = 1; i <= 4; i++ )
    {
        std::string strTemp = strPath + "pet_puriring_walk_%d.png";
        strTemp = MafUtils::format(strTemp.c_str(), i);
        
        auto ani = Sprite::create(strTemp);
        _vec.pushBack(ani->getSpriteFrame());
    }
    animation = Animation::createWithSpriteFrames(_vec, 0.1f);
    animate = Animate::create(animation);
    _animateMove = animate;
    _animateMove->retain();
    
    _vec.clear();
    _animateAttack->release();
    for ( int i = 1; i <= 4; i++ )
    {
        std::string strTemp = strPath + "pet_puriring_jump_%d.png";
        strTemp = MafUtils::format(strTemp.c_str(), i);
        
        auto ani = Sprite::create(strTemp);
        _vec.pushBack(ani->getSpriteFrame());
    }
    animation = Animation::createWithSpriteFrames(_vec, 0.1f);
    animate = Animate::create(animation);
    _animateAttack = animate;
    _animateAttack->retain();
 
    if( _eState == E_HERO_STATE::IDLE )
    {
        _eState = E_HERO_STATE::NONE;
        idle();
    }
    else if( _eState == E_HERO_STATE::MOVE )
    {
        _eState = E_HERO_STATE::IDLE;
        move();
    }
}

void Puriring::setEvolutionDummy(int nIdx)
{
    bool bSkin = false;
    std::string strPath = "Assets/character_pet/normal/";
    if ( PetNewManager::getInstance()->getPetSkin(_ePet) > 0 && _bSkinUse == true )
    {
        std::string strTemp = strPath;
        strTemp.append( MafUtils::format("puriring_skin_%d", nIdx) );
        
        if ( FileUtils::getInstance()->isDirectoryExist(strTemp) == true )
        {
            bSkin = true;
            strPath = strTemp;
            strPath.append("/");
        }
    }
    
    if ( bSkin == false )
    {
        if ( PetNewManager::getInstance()->getPetEvolution(_ePet) > 0 )
        {
            bSkin = true;
            strPath.append("puriring_evolution/");
        }
        else
        {
            bSkin = false;
            strPath.append("puriring/");
        }
    }

    _sprRing->setPosition(65, 80);
    
    Vector<cocos2d::SpriteFrame*> _vec;
    Animation *animation;
    Animate *animate;
    
    _vec.clear();
    _animateIdle->release();
    for ( int i = 1; i <= 4; i++ )
    {
        std::string strTemp = strPath + "pet_puriring_idle_%d.png";
        strTemp = MafUtils::format(strTemp.c_str(), i);
        
        auto ani = Sprite::create(strTemp);
        _vec.pushBack(ani->getSpriteFrame());
    }
    animation = Animation::createWithSpriteFrames(_vec, 0.1f);
    animate = Animate::create(animation);
    _animateIdle = animate;
    _animateIdle->retain();
    
    _vec.clear();
    _animateMove->release();
    for ( int i = 1; i <= 4; i++ )
    {
        std::string strTemp = strPath + "pet_puriring_walk_%d.png";
        strTemp = MafUtils::format(strTemp.c_str(), i);
        
        auto ani = Sprite::create(strTemp);
        _vec.pushBack(ani->getSpriteFrame());
    }
    animation = Animation::createWithSpriteFrames(_vec, 0.1f);
    animate = Animate::create(animation);
    _animateMove = animate;
    _animateMove->retain();
    
    _vec.clear();
    _animateAttack->release();
    for ( int i = 1; i <= 4; i++ )
    {
        std::string strTemp = strPath + "pet_puriring_jump_%d.png";
        strTemp = MafUtils::format(strTemp.c_str(), i);
        
        auto ani = Sprite::create(strTemp);
        _vec.pushBack(ani->getSpriteFrame());
    }
    animation = Animation::createWithSpriteFrames(_vec, 0.1f);
    animate = Animate::create(animation);
    _animateAttack = animate;
    _animateAttack->retain();
 
    if( _eState == E_HERO_STATE::IDLE )
    {
        _eState = E_HERO_STATE::NONE;
        idle();
    }
    else if( _eState == E_HERO_STATE::MOVE )
    {
        _eState = E_HERO_STATE::IDLE;
        move();
    }
}

Puriring* Puriring::create()
{
    Puriring *sprite = new (std::nothrow) Puriring();
    if (sprite && sprite->initWithFile("Assets/character_pet/normal/puriring/pet_puriring_idle_1.png"))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

void Puriring::getKey()
{
    SoundManager::SoundEffectStart(SOUND_PET_HAMSTER);
    
    //
    _eState = E_HERO_STATE::ATTACK;
    
    //
    int nFloor = UserInfoManager::getInstance()->getFloor(_ePlace);
    int nFloorCount = UserInfoManager::getInstance()->getFloorCount(_ePlace);
    int nLevelPet = PetNewManager::getInstance()->getPetLevel(E_PET::PURIRING_5);
    
    //
    std::string earnKey = PetNewManager::getInstance()->getPetEffectPuriringKey(nLevelPet, nFloor, nFloorCount);
    
    ItemsMoneyManager::getInstance()->setKey(earnKey);
    RefreshManager::getInstance()->refreshCurrencyUI(E_ITEMS::KEY, earnKey);
    
    // action
    CallFunc *call = CallFunc::create(CC_CALLBACK_0(Pet::move,this));
    Sequence *seq = Sequence::create(_animateAttack,call, nullptr);
    stopAllActions();
    runAction(seq);
}
 

#pragma mark-
Dragon::Dragon(){
    
    _ePet = E_PET::DRAGON_3;
    
    
    Vector<cocos2d::SpriteFrame*> _vec;
    Animation *animation;
    Animate *animate;
    
    for(int i=1; i<=4; i++){
        std::stringstream name;
        name << "Assets/character_pet/normal/dragon/pet_dragon_idle_";
        name << i;
        name << ".png";
        auto ani = Sprite::create(name.str());
        _vec.pushBack(ani->getSpriteFrame());
    }
    animation = Animation::createWithSpriteFrames(_vec, 0.1f);
    animate = Animate::create(animation);
    _animateIdle = animate;
    _animateIdle->retain();
    _animateMove = _animateIdle;
    _animateMove->retain();
    
    _vec.clear();
    for(int i=1; i<=6; i++){
        std::stringstream name;
        name << "Assets/character_pet/normal/dragon/pet_dragon_attack_1_";
        name << i;
        name << ".png";
        auto ani = Sprite::create(name.str());
        _vec.pushBack(ani->getSpriteFrame());
    }
    animation = Animation::createWithSpriteFrames(_vec, 0.075f);
    animate = Animate::create(animation);
    _animateAttack = animate;
    _animateAttack->retain();
    
    _sprRing = Sprite::create("Assets/character_hero/normal/ring_pet.png");
    _sprRing->setPosition(65, 160);
    _sprRing->setVisible(false);
    addChild(_sprRing);
    
    //
    setEvolution();
    move();
}

Dragon::~Dragon(){
    
}
void Dragon::setEvolution()
{
    bool bSkin = false;
    std::string strPath = "Assets/character_pet/normal/";
    if ( PetNewManager::getInstance()->getPetSkin(_ePet) > 0 && _bSkinUse == true )
    {
        std::string strTemp = strPath;
        strTemp.append( MafUtils::format("dragon_skin_%d", PetNewManager::getInstance()->getPetSkin(_ePet)) );
        
        if ( FileUtils::getInstance()->isDirectoryExist(strTemp) == true )
        {
            bSkin = true;
            strPath = strTemp;
            strPath.append("/");
        }
    }
    
    if ( bSkin == false )
    {
        if ( PetNewManager::getInstance()->getPetEvolution(_ePet) > 0 )
        {
            bSkin = true;
            strPath.append("dragon_evolution/");
        }
        else
        {
            bSkin = false;
            strPath.append("dragon/");
        }
    }
    
    Vector<cocos2d::SpriteFrame*> _vec;
    Animation *animation;
    Animate *animate;
    
    _animateIdle->release();
    _animateMove->release();
    for ( int i = 1; i <= 4; i++ )
    {
        std::string strTemp = strPath + "pet_dragon_idle_%d.png";
        strTemp = MafUtils::format(strTemp.c_str(), i);
        
        auto ani = Sprite::create(strTemp);
        _vec.pushBack(ani->getSpriteFrame());
    }
    animation = Animation::createWithSpriteFrames(_vec, 0.1f);
    animate = Animate::create(animation);
    _animateIdle = animate;
    _animateIdle->retain();
    
    _animateMove = _animateIdle;
    _animateMove->retain();
    
    _vec.clear();
    _animateAttack->release();
    for ( int i = 1; i <= 8; i++ )
    {
        std::string strTemp = strPath + "pet_dragon_attack_1_%d.png";
        strTemp = MafUtils::format(strTemp.c_str(), i);
        
        auto ani = Sprite::create(strTemp);
        _vec.pushBack(ani->getSpriteFrame());
    }
    animation = Animation::createWithSpriteFrames(_vec, 0.05f);
    animate = Animate::create(animation);
    _animateAttack = animate;
    _animateAttack->retain();
    
    if( _eState == E_HERO_STATE::IDLE )
    {
        _eState = E_HERO_STATE::NONE;
        idle();
    }
    else if( _eState == E_HERO_STATE::MOVE )
    {
        _eState = E_HERO_STATE::IDLE;
        move();
    }
}

void Dragon::setEvolutionDummy(int nIdx)
{
    bool bSkin = false;
    std::string strPath = "Assets/character_pet/normal/";
    if ( PetNewManager::getInstance()->getPetSkin(_ePet) > 0 && _bSkinUse == true )
    {
        std::string strTemp = strPath;
        strTemp.append( MafUtils::format("dragon_skin_%d", nIdx) );
        
        if ( FileUtils::getInstance()->isDirectoryExist(strTemp) == true )
        {
            bSkin = true;
            strPath = strTemp;
            strPath.append("/");
        }
    }
    
    if ( bSkin == false )
    {
        if ( PetNewManager::getInstance()->getPetEvolution(_ePet) > 0 )
        {
            bSkin = true;
            strPath.append("dragon_evolution/");
        }
        else
        {
            bSkin = false;
            strPath.append("dragon/");
        }
    }
    
    Vector<cocos2d::SpriteFrame*> _vec;
    Animation *animation;
    Animate *animate;
    
    _animateIdle->release();
    _animateMove->release();
    for ( int i = 1; i <= 4; i++ )
    {
        std::string strTemp = strPath + "pet_dragon_idle_%d.png";
        strTemp = MafUtils::format(strTemp.c_str(), i);
        
        auto ani = Sprite::create(strTemp);
        _vec.pushBack(ani->getSpriteFrame());
    }
    animation = Animation::createWithSpriteFrames(_vec, 0.1f);
    animate = Animate::create(animation);
    _animateIdle = animate;
    _animateIdle->retain();
    
    _animateMove = _animateIdle;
    _animateMove->retain();
    
    _vec.clear();
    _animateAttack->release();
    for ( int i = 1; i <= 8; i++ )
    {
        std::string strTemp = strPath + "pet_dragon_attack_1_%d.png";
        strTemp = MafUtils::format(strTemp.c_str(), i);
        
        auto ani = Sprite::create(strTemp);
        _vec.pushBack(ani->getSpriteFrame());
    }
    animation = Animation::createWithSpriteFrames(_vec, 0.05f);
    animate = Animate::create(animation);
    _animateAttack = animate;
    _animateAttack->retain();
    
    if( _eState == E_HERO_STATE::IDLE )
    {
        _eState = E_HERO_STATE::NONE;
        idle();
    }
    else if( _eState == E_HERO_STATE::MOVE )
    {
        _eState = E_HERO_STATE::IDLE;
        move();
    }
}


Dragon* Dragon::create()
{
    Dragon *sprite = new (std::nothrow) Dragon();
    if (sprite && sprite->initWithFile("Assets/character_pet/normal/dragon/pet_dragon_idle_1.png"))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

void Dragon::attack(UnitBehaviorMonster& mon){
    
    _enemy = &mon;
    
    if(_eState == E_HERO_STATE::IDLE || _eState == E_HERO_STATE::ATTACK)
        return;

    if ( _enemy == nullptr )
        return;
    
    if ( UserInfoManager::getInstance()->getHitCount(_ePlace) >= 1 )
        return;
    
    SoundManager::SoundEffectStart(SOUND_PET_DRAGON);
    
    _eState = E_HERO_STATE::ATTACK;
    _bNoIdle = false;

    int level = PetNewManager::getInstance()->getPetLevel(_ePet);
    double per = PetNewManager::getInstance()->getPetEffect(_ePet, level);
    double minPer = (level-1)*4;
    
    if (CostumeManager::getInstance()->isEquip(E_COSTUME::HAND,E_COSTUME_IDX::IDX_38)) {
        minPer += 10;
    }
    
    int evolutionLevel = PetNewManager::getInstance()->getPetEvolution(E_PET::DRAGON_3);
    double evolutionPer = PetNewManager::getInstance()->getPetEvolutionEffect1(E_PET::DRAGON_3, evolutionLevel);
    if (evolutionPer > 0) {
        minPer += evolutionPer;
    }
    
    int reinforceLevel = PetNewManager::getInstance()->getPetReinforce(E_PET::DRAGON_3);
    double reinforcePer = PetNewManager::getInstance()->getPetReinforceEffect(E_PET::DRAGON_3, reinforceLevel);
    if (reinforcePer > 0) {
        per += reinforcePer;
    }
    minPer += PetNewManager::getInstance()->getPetEquipEffect(302, 0);
    minPer *= 10;
    per *= 10;
    if ( per < minPer )
    {
        minPer = per;
    }
    
    int attackPer = random(minPer, per);
    if(_ePlace == E_PLACE::DG_PRISON){
        attackPer = minPer + (per-minPer)/2;
    }
    
    int killper = 0;
    if (CostumeManager::getInstance()->isEquip(E_COSTUME::CAPE,E_COSTUME_IDX::IDX_9)) {
        killper += 20;
    }
    
    killper = killper + PetNewManager::getInstance()->getPetEquipEffect(300, 0);
    
    if (UserInfoStackManager::getInstance()->getDragonSkillCheck() == 0) {
        if (random(1, 100) <= killper) {
            attackPer = 1000;
            UserInfoStackManager::getInstance()->setDragonSkillCheck(1);
        }
    }else{
        UserInfoStackManager::getInstance()->setDragonSkillCheck(0);
    }
    

    std::string myAtk = "0";
    
    
    if(attackPer==1000){
        myAtk = _enemy->getMonsterHp();
    }
    else{
        myAtk = _enemy->getMonsterHp();
        myAtk = MafUtils::bigMulNum(myAtk, MafUtils::toString(attackPer));
        if (myAtk.length() > 3) {
            myAtk.pop_back();
            myAtk.pop_back();
            myAtk.pop_back();
        }
    }
    _enemy->actionHit(E_CHARACTER::CHA_PET_DRAGON, E_CHARACTER_HIT::HIT_NORMAL, myAtk, false);
    CallFunc *call = CallFunc::create(CC_CALLBACK_0(Pet::idle,this));
    Sequence *seq = Sequence::create(_animateAttack,call, nullptr);
    stopAllActions();
    //seq->setTag(TAG_ATTACK);
    runAction(seq);
    
}

#pragma mark-

Princess::Princess(){

    _ePet = E_PET::PRINCESS_4;
    
    _nBuffAllCount = 0;
    _nAttackCount= 0;
    _onBuffEffect = nullptr;

    
    Vector<cocos2d::SpriteFrame*> _vec;
    Animation *animation;
    Animate *animate;
    
    for(int i=1; i<=4; i++){
        std::stringstream name;
        name << "Assets/character_pet/normal/princess/pet_princess_idle_";
        name << i;
        name << ".png";
        auto ani = Sprite::create(name.str());
        _vec.pushBack(ani->getSpriteFrame());
    }
    animation = Animation::createWithSpriteFrames(_vec, 0.1f);
    animate = Animate::create(animation);
    _animateIdle = animate;
    _animateIdle->retain();
    
    _vec.clear();
    for(int i=1; i<=4; i++){
        std::stringstream name;
        name << "Assets/character_pet/normal/princess/pet_princess_walk_";
        name << i;
        name << ".png";
        auto ani = Sprite::create(name.str());
        _vec.pushBack(ani->getSpriteFrame());
    }
    animation = Animation::createWithSpriteFrames(_vec, 0.1f);
    animate = Animate::create(animation);
    _animateMove = animate;
    _animateMove->retain();
    
    _vec.clear();
    for(int i=1; i<=4; i++){
        std::stringstream name;
        name << "Assets/character_pet/normal/princess/pet_princess_attack_";
        name << i;
        name << ".png";
        auto ani = Sprite::create(name.str());
        _vec.pushBack(ani->getSpriteFrame());
    }
    //5번째 동작을 넣어줌
    auto ani = Sprite::create("Assets/character_pet/normal/princess/pet_princess_idle_4.png");
    _vec.pushBack(ani->getSpriteFrame());
    
    animation = Animation::createWithSpriteFrames(_vec, 0.08f);
    animate = Animate::create(animation);
    _animateAttack = animate;
    _animateAttack->retain();
    
    _sprRing = Sprite::create("Assets/character_hero/normal/ring_gongju.png");
    _sprRing->setPosition(90, 165);
    _sprRing->setVisible(false);
    addChild(_sprRing);
    
    //
    setEvolution();
    move();
}
Princess::~Princess(){
    _onBuffEffect = nullptr;
}
void Princess::setEvolution()
{
    bool bSkin = false;
    std::string strPath = "Assets/character_pet/normal/";
    if ( PetNewManager::getInstance()->getPetSkin(_ePet) > 0 && _bSkinUse == true )
    {
        std::string strTemp = strPath;
        strTemp.append( MafUtils::format("princess_skin_%d", PetNewManager::getInstance()->getPetSkin(_ePet)) );
        
        if ( FileUtils::getInstance()->isDirectoryExist(strTemp) == true )
        {
            bSkin = true;
            strPath = strTemp;
            strPath.append("/");
        }
    }
    
    if ( bSkin == false )
    {
        if ( PetNewManager::getInstance()->getPetEvolution(_ePet) > 0 )
        {
            bSkin = true;
            strPath.append("princess_evolution/");
        }
        else
        {
            bSkin = false;
            strPath.append("princess/");
        }
    }
    
    _sprRing->setPosition(90, 180);

    Vector<cocos2d::SpriteFrame*> _vec;
    Animation *animation;
    Animate *animate;
    
    _vec.clear();
    _animateIdle->release();
    for ( int i = 1; i <= 4; i++ )
    {
        std::string strTemp = strPath + "pet_princess_idle_%d.png";
        strTemp = MafUtils::format(strTemp.c_str(), i);
        
        auto ani = Sprite::create(strTemp);
        _vec.pushBack(ani->getSpriteFrame());
    }
    animation = Animation::createWithSpriteFrames(_vec, 0.1f);
    animate = Animate::create(animation);
    _animateIdle = animate;
    _animateIdle->retain();
    
    _vec.clear();
    _animateMove->release();
    for ( int i = 1; i <= 4; i++ )
    {
        std::string strTemp = strPath + "pet_princess_walk_%d.png";
        strTemp = MafUtils::format(strTemp.c_str(), i);
        
        auto ani = Sprite::create(strTemp);
        _vec.pushBack(ani->getSpriteFrame());
    }
    animation = Animation::createWithSpriteFrames(_vec, 0.1f);
    animate = Animate::create(animation);
    _animateMove = animate;
    _animateMove->retain();
    
    _vec.clear();
    _animateAttack->release();
    for ( int i = 1; i <= 4; i++ )
    {
        std::string strTemp = strPath + "pet_princess_attack_%d.png";
        strTemp = MafUtils::format(strTemp.c_str(), i);
        
        auto ani = Sprite::create(strTemp);
        _vec.pushBack(ani->getSpriteFrame());
    }
    animation = Animation::createWithSpriteFrames(_vec, 0.1f);
    animate = Animate::create(animation);
    _animateAttack = animate;
    _animateAttack->retain();

    if( _eState == E_HERO_STATE::IDLE )
    {
        _eState = E_HERO_STATE::NONE;
        idle();
    }
    else if( _eState == E_HERO_STATE::MOVE )
    {
        _eState = E_HERO_STATE::IDLE;
        move();
    }
}

void Princess::setEvolutionDummy(int nIdx)
{
    bool bSkin = false;
    std::string strPath = "Assets/character_pet/normal/";
    if ( PetNewManager::getInstance()->getPetSkin(_ePet) > 0 && _bSkinUse == true )
    {
        std::string strTemp = strPath;
        strTemp.append( MafUtils::format("princess_skin_%d", nIdx) );
        
        if ( FileUtils::getInstance()->isDirectoryExist(strTemp) == true )
        {
            bSkin = true;
            strPath = strTemp;
            strPath.append("/");
        }
    }
    
    if ( bSkin == false )
    {
        if ( PetNewManager::getInstance()->getPetEvolution(_ePet) > 0 )
        {
            bSkin = true;
            strPath.append("princess_evolution/");
        }
        else
        {
            bSkin = false;
            strPath.append("princess/");
        }
    }
    
    _sprRing->setPosition(90, 180);

    Vector<cocos2d::SpriteFrame*> _vec;
    Animation *animation;
    Animate *animate;
    
    _vec.clear();
    _animateIdle->release();
    for ( int i = 1; i <= 4; i++ )
    {
        std::string strTemp = strPath + "pet_princess_idle_%d.png";
        strTemp = MafUtils::format(strTemp.c_str(), i);
        
        auto ani = Sprite::create(strTemp);
        _vec.pushBack(ani->getSpriteFrame());
    }
    animation = Animation::createWithSpriteFrames(_vec, 0.1f);
    animate = Animate::create(animation);
    _animateIdle = animate;
    _animateIdle->retain();
    
    _vec.clear();
    _animateMove->release();
    for ( int i = 1; i <= 4; i++ )
    {
        std::string strTemp = strPath + "pet_princess_walk_%d.png";
        strTemp = MafUtils::format(strTemp.c_str(), i);
        
        auto ani = Sprite::create(strTemp);
        _vec.pushBack(ani->getSpriteFrame());
    }
    animation = Animation::createWithSpriteFrames(_vec, 0.1f);
    animate = Animate::create(animation);
    _animateMove = animate;
    _animateMove->retain();
    
    _vec.clear();
    _animateAttack->release();
    for ( int i = 1; i <= 4; i++ )
    {
        std::string strTemp = strPath + "pet_princess_attack_%d.png";
        strTemp = MafUtils::format(strTemp.c_str(), i);
        
        auto ani = Sprite::create(strTemp);
        _vec.pushBack(ani->getSpriteFrame());
    }
    animation = Animation::createWithSpriteFrames(_vec, 0.1f);
    animate = Animate::create(animation);
    _animateAttack = animate;
    _animateAttack->retain();

    if( _eState == E_HERO_STATE::IDLE )
    {
        _eState = E_HERO_STATE::NONE;
        idle();
    }
    else if( _eState == E_HERO_STATE::MOVE )
    {
        _eState = E_HERO_STATE::IDLE;
        move();
    }
}

Princess* Princess::create()
{
    Princess *sprite = new (std::nothrow) Princess();
    if (sprite && sprite->initWithFile("Assets/character_pet/normal/princess/pet_princess_idle_1.png"))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

void Princess::update(float dt){
    
//    if(_eState == MOVE){
//        unscheduleUpdate();
        _coolTime -= dt;
//        return;
//    }

    _coolTime -= dt;
    if(_coolTime<=0){
        
        _eState = ATTACK;
        _coolTime = _coolTimeMax;
        CallFunc *call = CallFunc::create(CC_CALLBACK_0(Pet::idle,this));
//        if(_enemy==nullptr){
//            return;
//        }
        
        _nAttackCount++;
        if ( _nAttackCount>=C_PRINCESS_COUNT)
        {
            _nAttackCount = 0;

            E_PRINCESS_BUFF buff = E_PRINCESS_BUFF::BUFF_NONE;
            float effect = PetNewManager::getInstance()->getPetEquipEffect(401, 0) / 3;
            float ran = random(1.f, 100.f);
            
            if(ran <= 31 - effect)        buff = E_PRINCESS_BUFF::BUFF_ATTACK;
            else if(ran <= 62 - effect)   buff = E_PRINCESS_BUFF::BUFF_MOVE;
            else if(ran <= 93 - effect)   buff = E_PRINCESS_BUFF::BUFF_CRITICAL;
            else if(ran <= 100)  buff = E_PRINCESS_BUFF::BUFF_ALL;
            
            if ( _ePlace == E_PLACE::DG_PRISON && CostumeManager::getInstance()->isExist(E_COSTUME::SETITEM, E_COSTUME_IDX::IDX_39) )
            {
                if ( buff == E_PRINCESS_BUFF::BUFF_ALL )
                {
                    _nBuffAllCount = 0;
                }
                else
                {
                    if ( _nBuffAllCount >= 1 )
                    {
                        buff = E_PRINCESS_BUFF::BUFF_ALL;
                        _nBuffAllCount = 0;
                    }
                    else
                    {
                        _nBuffAllCount += 1;
                    }
                }
            }
            
            int level = PetNewManager::getInstance()->getPetLevel(_ePet);
            auto per = PetNewManager::getInstance()->getPetEffect(_ePet, level);
            _onBuffEffect(buff,per);
        }
        
        int ran = random(1, 3);
        if(ran==1)
            SoundManager::SoundEffectStart(SOUND_PET_PRINCESS);
        
        stopAllActions();
        Sequence *seq = Sequence::create(_animateAttack,call, nullptr);
        runAction(seq);
    }
}

void Princess::setPlace(E_PLACE ePlace)
{
    _ePlace = ePlace;
    if(_ePlace == E_PLACE::DG_PRISON)
    {
        _coolTime = 0;
        _nAttackCount = C_PRINCESS_COUNT-1;
    }
}


#pragma mark-
Hong::Hong(){
    _ePet = E_PET::HONG_6;
    
    Vector<cocos2d::SpriteFrame*> _vec;
    Animation *animation;
    Animate *animate;
    
    for(int i=1; i<=4; i++){
        std::stringstream name;
        name << "Assets/character_pet/normal/hong/pet_hong_idle_";
        name << i;
        name << ".png";
        auto ani = Sprite::create(name.str());
        _vec.pushBack(ani->getSpriteFrame());
    }
    animation = Animation::createWithSpriteFrames(_vec, 0.1f);
    animate = Animate::create(animation);
    _animateIdle = animate;
    _animateIdle->retain();
    _animateMove = _animateIdle;
    _animateMove->retain();
    
    _vec.clear();
    for(int i=1; i<=4; i++){
        std::stringstream name;
        name << "Assets/character_pet/normal/hong/pet_hong_attack_";
        name << i;
        name << ".png";
        auto ani = Sprite::create(name.str());
        _vec.pushBack(ani->getSpriteFrame());
    }
    animation = Animation::createWithSpriteFrames(_vec, 0.1f);
    animate = Animate::create(animation);
    _animateAttack = animate;
    _animateAttack->retain();
    
    _sprRing = Sprite::create("Assets/character_hero/normal/ring_gongju.png");
    _sprRing->setPosition(60, 166);
    _sprRing->setVisible(false);
    addChild(_sprRing);
    
    //
    setEvolution();
    move();
}
Hong::~Hong(){
    
}
void Hong::setEvolution()
{
    bool bSkin = false;
    std::string strPath = "Assets/character_pet/normal/";
    if ( PetNewManager::getInstance()->getPetSkin(_ePet) > 0 && _bSkinUse == true )
    {
        std::string strTemp = strPath;
        strTemp.append( MafUtils::format("hong_skin_%d", PetNewManager::getInstance()->getPetSkin(_ePet)) );
        
        if ( FileUtils::getInstance()->isDirectoryExist(strTemp) == true )
        {
            bSkin = true;
            strPath = strTemp;
            strPath.append("/");
        }
    }
    
    if ( bSkin == false )
    {
        if ( PetNewManager::getInstance()->getPetEvolution(_ePet) > 0 )
        {
            bSkin = true;
            strPath.append("hong_evolution/");
        }
        else
        {
            bSkin = false;
            strPath.append("hong/");
        }
    }
    
    _sprRing->setPosition(60, 180);
    
    Vector<cocos2d::SpriteFrame*> _vec;
    Animation *animation;
    Animate *animate;
    
    _vec.clear();
    _animateAttack->release();
    for ( int i = 1; i <= 4; i++ )
    {
        std::string strTemp = strPath + "pet_hong_attack_%d.png";
        strTemp = MafUtils::format(strTemp.c_str(), i);
        
        auto ani = Sprite::create(strTemp);
        _vec.pushBack(ani->getSpriteFrame());
    }
    animation = Animation::createWithSpriteFrames(_vec, 0.1f);
    animate = Animate::create(animation);
    _animateAttack = animate;
    _animateAttack->retain();
    
    _vec.clear();
    _animateIdle->release();
    _animateMove->release();
    for ( int i = 1; i <= 4; i++ )
    {
        std::string strTemp = strPath + "pet_hong_idle_%d.png";
        strTemp = MafUtils::format(strTemp.c_str(), i);
        
        auto ani = Sprite::create(strTemp);
        _vec.pushBack(ani->getSpriteFrame());
    }
    animation = Animation::createWithSpriteFrames(_vec, 0.1f);
    animate = Animate::create(animation);
    _animateIdle = animate;
    _animateIdle->retain();
    
    _animateMove = animate;
    _animateMove->retain();
    
    if( _eState == E_HERO_STATE::IDLE )
    {
        _eState = E_HERO_STATE::NONE;
        idle();
    }
    else if( _eState == E_HERO_STATE::MOVE )
    {
        _eState = E_HERO_STATE::IDLE;
        move();
    }
}

void Hong::setEvolutionDummy(int nIdx)
{
    bool bSkin = false;
    std::string strPath = "Assets/character_pet/normal/";
    if ( PetNewManager::getInstance()->getPetSkin(_ePet) > 0 && _bSkinUse == true )
    {
        std::string strTemp = strPath;
        strTemp.append( MafUtils::format("hong_skin_%d", nIdx) );
        
        if ( FileUtils::getInstance()->isDirectoryExist(strTemp) == true )
        {
            bSkin = true;
            strPath = strTemp;
            strPath.append("/");
        }
    }
    
    if ( bSkin == false )
    {
        if ( PetNewManager::getInstance()->getPetEvolution(_ePet) > 0 )
        {
            bSkin = true;
            strPath.append("hong_evolution/");
        }
        else
        {
            bSkin = false;
            strPath.append("hong/");
        }
    }
    
    _sprRing->setPosition(60, 180);
    
    Vector<cocos2d::SpriteFrame*> _vec;
    Animation *animation;
    Animate *animate;
    
    _vec.clear();
    _animateAttack->release();
    for ( int i = 1; i <= 4; i++ )
    {
        std::string strTemp = strPath + "pet_hong_attack_%d.png";
        strTemp = MafUtils::format(strTemp.c_str(), i);
        
        auto ani = Sprite::create(strTemp);
        _vec.pushBack(ani->getSpriteFrame());
    }
    animation = Animation::createWithSpriteFrames(_vec, 0.1f);
    animate = Animate::create(animation);
    _animateAttack = animate;
    _animateAttack->retain();
    
    _vec.clear();
    _animateIdle->release();
    _animateMove->release();
    for ( int i = 1; i <= 4; i++ )
    {
        std::string strTemp = strPath + "pet_hong_idle_%d.png";
        strTemp = MafUtils::format(strTemp.c_str(), i);
        
        auto ani = Sprite::create(strTemp);
        _vec.pushBack(ani->getSpriteFrame());
    }
    animation = Animation::createWithSpriteFrames(_vec, 0.1f);
    animate = Animate::create(animation);
    _animateIdle = animate;
    _animateIdle->retain();
    
    _animateMove = animate;
    _animateMove->retain();
    
    if( _eState == E_HERO_STATE::IDLE )
    {
        _eState = E_HERO_STATE::NONE;
        idle();
    }
    else if( _eState == E_HERO_STATE::MOVE )
    {
        _eState = E_HERO_STATE::IDLE;
        move();
    }
}


Hong* Hong::create()
{
    Hong *sprite = new (std::nothrow) Hong();
    if (sprite && sprite->initWithFile("Assets/character_pet/normal/hong/pet_hong_idle_1.png"))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}


void Hong::update(float dt){
    if(_eState == MOVE){
        unscheduleUpdate();
        return;
    }
    
    _coolTime -= dt;
    if(_coolTime<=0){
        
        _coolTime = _coolTimeMax;
        if(_enemy==nullptr){
            return;
        }
        if(_enemy->isDie()){
            return;
        }
        
        _eState = E_HERO_STATE::ATTACK;
        CallFunc *call = CallFunc::create(CC_CALLBACK_0(Pet::idle,this));
        Sequence *seq;
        
        
        int count = 10;
        
        int evolutionLevel = PetNewManager::getInstance()->getPetEvolution(E_PET::HONG_6);
        double evolutionPer = PetNewManager::getInstance()->getPetEvolutionEffect1(E_PET::HONG_6, evolutionLevel);
        if (evolutionPer > 0) {
            count += evolutionPer;
        }
        
        if (_ePlace == E_PLACE::DG_NORMAL)
        {
            int monsterHitCount = UserInfoManager::getInstance()->getHongHitCount(_ePlace);
            if ( monsterHitCount == count )
            {
                if ( _hero != nullptr )
                {
                    _hero->actionAttackGavel();
                }
                
                UserInfoManager::getInstance()->setHongHitCount(_ePlace, monsterHitCount + 1);
                return;
            }
            else if ( monsterHitCount > count)
            {
                return;
            }
            
            UserInfoManager::getInstance()->setHongHitCount(_ePlace, monsterHitCount + 1);
        }
        else if (_ePlace == E_PLACE::DG_PRISON)
        {
            int monsterHitCount = UserInfoManager::getInstance()->getHongHitCount(_ePlace);
            if ( monsterHitCount >= count )
            {
                return;
            }
            
            UserInfoManager::getInstance()->setHongHitCount(_ePlace, monsterHitCount + 1);
        }
        
        //SoundManager::SoundEffectStart(SOUND_PET_KRRR_ATTACK);
        
        int attackPer = PetNewManager::getInstance()->getPetEffect(_ePet, PetNewManager::getInstance()->getPetLevel(_ePet));
        
        int reinforceLevel = PetNewManager::getInstance()->getPetReinforce(E_PET::HONG_6);
        double reinforcePer = PetNewManager::getInstance()->getPetReinforceEffect(E_PET::HONG_6, reinforceLevel);
        if (reinforcePer > 0) {
            attackPer += reinforcePer;
        }
        
        std::string myAtk = "0";
        int val = attackPer/10;
        int rest = attackPer%10;
        if(val>0){
            auto ten = _enemy->getMonsterHp();
            if (ten.length() > 1) {
                ten.pop_back();
            }
            auto temp = MafUtils::bigMulNum(ten, MafUtils::toString(val));
            myAtk = MafUtils::bigAddNum(myAtk, temp);
        }
        if(rest>0){
            auto one = _enemy->getMonsterHp();
            if (one.length() > 2) {
                one.pop_back();
                one.pop_back();
            }
            auto temp = MafUtils::bigMulNum(one, MafUtils::toString(rest));
            myAtk = MafUtils::bigAddNum(myAtk, temp);
        }
        
        _enemy->actionHit(E_CHARACTER::CHA_PET_HONG, E_CHARACTER_HIT::HIT_NORMAL, myAtk, false);
        
        seq = Sequence::create(_animateAttack,call, nullptr);
        stopAllActions();
        runAction(seq);

    }
}

#pragma mark-
Mole::Mole(){
    _ePet = E_PET::MOLE_7;
    
    Vector<cocos2d::SpriteFrame*> _vec;
    Animation *animation;
    Animate *animate;
    
    for(int i=1; i<=4; i++){
        std::stringstream name;
        name << "Assets/character_pet/normal/mole/pet_mole_walk_";
        name << i;
        name << ".png";
        auto ani = Sprite::create(name.str());
        _vec.pushBack(ani->getSpriteFrame());
    }
    animation = Animation::createWithSpriteFrames(_vec, 0.1f);
    animate = Animate::create(animation);
    _animateMove = animate;
    _animateMove->retain();
    _animateIdle = _animateMove;
    _animateIdle->retain();
    
    _vec.clear();
    for(int i=1; i<=4; i++){
        std::stringstream name;
        name << "Assets/character_pet/normal/mole/pet_mole_attack_";
        name << i;
        name << ".png";
        auto ani = Sprite::create(name.str());
        _vec.pushBack(ani->getSpriteFrame());
    }
    animation = Animation::createWithSpriteFrames(_vec, 0.1f);
    animate = Animate::create(animation);
    _animateAttack = animate;
    _animateAttack->retain();
    
    //
    setEvolution();
    move();
}
Mole::~Mole(){
    
}
void Mole::setEvolution()
{
    bool bSkin = false;
    std::string strPath = "Assets/character_pet/normal/";
    if ( PetNewManager::getInstance()->getPetSkin(_ePet) > 0 && _bSkinUse == true )
    {
        std::string strTemp = strPath;
        strTemp.append( MafUtils::format("mole_skin_%d", PetNewManager::getInstance()->getPetSkin(_ePet)) );
        
        if ( FileUtils::getInstance()->isDirectoryExist(strTemp) == true )
        {
            bSkin = true;
            strPath = strTemp;
            strPath.append("/");
        }
    }
    
    if ( bSkin == false )
    {
        if ( PetNewManager::getInstance()->getPetEvolution(_ePet) > 0 )
        {
            bSkin = true;
            strPath.append("mole_evolution/");
        }
        else
        {
            bSkin = false;
            strPath.append("mole/");
        }
    }
    
    Vector<cocos2d::SpriteFrame*> _vec;
    Animation *animation;
    Animate *animate;
    
    _vec.clear();
    _animateMove->release();
    for ( int i = 1; i <= 4; i++ )
    {
        std::string strTemp = strPath + "pet_mole_walk_%d.png";
        strTemp = MafUtils::format(strTemp.c_str(), i);
        
        auto ani = Sprite::create(strTemp);
        _vec.pushBack(ani->getSpriteFrame());
    }
    animation = Animation::createWithSpriteFrames(_vec, 0.1f);
    animate = Animate::create(animation);
    _animateMove = animate;
    _animateMove->retain();
    
    _animateIdle = animate;
    _animateIdle->retain();
    
    _vec.clear();
    _animateAttack->release();
    for ( int i = 1; i <= 4; i++ )
    {
        std::string strTemp = strPath + "pet_mole_attack_%d.png";
        strTemp = MafUtils::format(strTemp.c_str(), i);
        
        auto ani = Sprite::create(strTemp);
        _vec.pushBack(ani->getSpriteFrame());
    }
    animation = Animation::createWithSpriteFrames(_vec, 0.1f);
    animate = Animate::create(animation);
    _animateAttack = animate;
    _animateAttack->retain();
    
    if( _eState == E_HERO_STATE::IDLE )
    {
        _eState = E_HERO_STATE::NONE;
        idle();
    }
    else if( _eState == E_HERO_STATE::MOVE )
    {
        _eState = E_HERO_STATE::IDLE;
        move();
    }
}

void Mole::setEvolutionDummy(int nIdx)
{
    bool bSkin = false;
    std::string strPath = "Assets/character_pet/normal/";
    if ( PetNewManager::getInstance()->getPetSkin(_ePet) > 0 && _bSkinUse == true )
    {
        std::string strTemp = strPath;
        strTemp.append( MafUtils::format("mole_skin_%d", nIdx) );
        
        if ( FileUtils::getInstance()->isDirectoryExist(strTemp) == true )
        {
            bSkin = true;
            strPath = strTemp;
            strPath.append("/");
        }
    }
    
    if ( bSkin == false )
    {
        if ( PetNewManager::getInstance()->getPetEvolution(_ePet) > 0 )
        {
            bSkin = true;
            strPath.append("mole_evolution/");
        }
        else
        {
            bSkin = false;
            strPath.append("mole/");
        }
    }
    
    Vector<cocos2d::SpriteFrame*> _vec;
    Animation *animation;
    Animate *animate;
    
    _vec.clear();
    _animateMove->release();
    for ( int i = 1; i <= 4; i++ )
    {
        std::string strTemp = strPath + "pet_mole_walk_%d.png";
        strTemp = MafUtils::format(strTemp.c_str(), i);
        
        auto ani = Sprite::create(strTemp);
        _vec.pushBack(ani->getSpriteFrame());
    }
    animation = Animation::createWithSpriteFrames(_vec, 0.1f);
    animate = Animate::create(animation);
    _animateMove = animate;
    _animateMove->retain();
    
    _animateIdle = animate;
    _animateIdle->retain();
    
    _vec.clear();
    _animateAttack->release();
    for ( int i = 1; i <= 4; i++ )
    {
        std::string strTemp = strPath + "pet_mole_attack_%d.png";
        strTemp = MafUtils::format(strTemp.c_str(), i);
        
        auto ani = Sprite::create(strTemp);
        _vec.pushBack(ani->getSpriteFrame());
    }
    animation = Animation::createWithSpriteFrames(_vec, 0.1f);
    animate = Animate::create(animation);
    _animateAttack = animate;
    _animateAttack->retain();
    
    if( _eState == E_HERO_STATE::IDLE )
    {
        _eState = E_HERO_STATE::NONE;
        idle();
    }
    else if( _eState == E_HERO_STATE::MOVE )
    {
        _eState = E_HERO_STATE::IDLE;
        move();
    }
}

Mole* Mole::create()
{
    Mole *sprite = new (std::nothrow) Mole();
    if (sprite && sprite->initWithFile("Assets/character_pet/normal/mole/pet_mole_walk_1.png"))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}
void Mole::attackMine()
{
    _eState = E_HERO_STATE::ATTACK;
    
    stopAllActions();
    runAction(_animateAttack);
}

#pragma mark-
Handsome::Handsome(){
    _ePet = E_PET::HANDSOME_8;
    
    Vector<cocos2d::SpriteFrame*> _vec;
    Animation *animation;
    Animate *animate;
    
    
    _vec.clear();
    for(int i=1; i<=4; i++){
        std::stringstream name;
        name << "Assets/character_pet/normal/handsome/pet_handsome_attack_";
        name << i;
        name << ".png";
        auto ani = Sprite::create(name.str());
        _vec.pushBack(ani->getSpriteFrame());
    }
    animation = Animation::createWithSpriteFrames(_vec, 0.1f);
    animate = Animate::create(animation);
    _animateAttack = animate;
    _animateAttack->retain();
    
    _vec.clear();
    for(int i=1; i<=4; i++){
        std::stringstream name;
        name << "Assets/character_pet/normal/handsome/pet_handsome_idle_";
        name << i;
        name << ".png";
        auto ani = Sprite::create(name.str());
        _vec.pushBack(ani->getSpriteFrame());
    }
    animation = Animation::createWithSpriteFrames(_vec, 0.1f);
    animate = Animate::create(animation);
    _animateIdle = animate;
    _animateIdle->retain();
    
    _animateMove = animate;
    _animateMove->retain();
    
    _sprRing = Sprite::create("Assets/character_hero/normal/ring_pet.png");
    _sprRing->setPosition(40, 170);
    _sprRing->setVisible(false);
    addChild(_sprRing);
    
    //
    setEvolution();
    move();
}
Handsome::~Handsome(){
    
}
void Handsome::setEvolution()
{
    bool bSkin = false;
    std::string strPath = "Assets/character_pet/normal/";
    if ( PetNewManager::getInstance()->getPetSkin(_ePet) > 0 && _bSkinUse == true )
    {
        std::string strTemp = strPath;
        strTemp.append( MafUtils::format("handsome_skin_%d", PetNewManager::getInstance()->getPetSkin(_ePet)) );
        
        if ( FileUtils::getInstance()->isDirectoryExist(strTemp) == true )
        {
            bSkin = true;
            strPath = strTemp;
            strPath.append("/");
        }
    }
    
    if ( bSkin == false )
    {
        if ( PetNewManager::getInstance()->getPetEvolution(_ePet) > 0 )
        {
            bSkin = true;
            strPath.append("handsome_evolution/");
        }
        else
        {
            bSkin = false;
            strPath.append("handsome/");
        }
    }
    
    _sprRing->setPosition(90, 180);
    
    Vector<cocos2d::SpriteFrame*> _vec;
    Animation *animation;
    Animate *animate;
    
    _vec.clear();
    _animateAttack->release();
    for ( int i = 1; i <= 4; i++ )
    {
        std::string strTemp = strPath + "pet_handsome_attack_%d.png";
        strTemp = MafUtils::format(strTemp.c_str(), i);
        
        auto ani = Sprite::create(strTemp);
        _vec.pushBack(ani->getSpriteFrame());
    }
    animation = Animation::createWithSpriteFrames(_vec, 0.1f);
    animate = Animate::create(animation);
    _animateAttack = animate;
    _animateAttack->retain();
    
    _vec.clear();
    _animateIdle->release();
    _animateMove->release();
    for ( int i = 1; i <= 4; i++ )
    {
        std::string strTemp = strPath + "pet_handsome_idle_%d.png";
        strTemp = MafUtils::format(strTemp.c_str(), i);
        
        auto ani = Sprite::create(strTemp);
        _vec.pushBack(ani->getSpriteFrame());
    }
    animation = Animation::createWithSpriteFrames(_vec, 0.1f);
    animate = Animate::create(animation);
    _animateIdle = animate;
    _animateIdle->retain();

    _animateMove = animate;
    _animateMove->retain();
    
    if( _eState == E_HERO_STATE::IDLE )
    {
        _eState = E_HERO_STATE::NONE;
        idle();
    }
    else if( _eState == E_HERO_STATE::MOVE )
    {
        _eState = E_HERO_STATE::IDLE;
        move();
    }
}

void Handsome::setEvolutionDummy(int nIdx)
{
    bool bSkin = false;
    std::string strPath = "Assets/character_pet/normal/";
    if ( PetNewManager::getInstance()->getPetSkin(_ePet) > 0 && _bSkinUse == true )
    {
        std::string strTemp = strPath;
        strTemp.append( MafUtils::format("handsome_skin_%d", nIdx) );
        
        if ( FileUtils::getInstance()->isDirectoryExist(strTemp) == true )
        {
            bSkin = true;
            strPath = strTemp;
            strPath.append("/");
        }
    }
    
    if ( bSkin == false )
    {
        if ( PetNewManager::getInstance()->getPetEvolution(_ePet) > 0 )
        {
            bSkin = true;
            strPath.append("handsome_evolution/");
        }
        else
        {
            bSkin = false;
            strPath.append("handsome/");
        }
    }
    
    _sprRing->setPosition(90, 180);
    
    Vector<cocos2d::SpriteFrame*> _vec;
    Animation *animation;
    Animate *animate;
    
    _vec.clear();
    _animateAttack->release();
    for ( int i = 1; i <= 4; i++ )
    {
        std::string strTemp = strPath + "pet_handsome_attack_%d.png";
        strTemp = MafUtils::format(strTemp.c_str(), i);
        
        auto ani = Sprite::create(strTemp);
        _vec.pushBack(ani->getSpriteFrame());
    }
    animation = Animation::createWithSpriteFrames(_vec, 0.1f);
    animate = Animate::create(animation);
    _animateAttack = animate;
    _animateAttack->retain();
    
    _vec.clear();
    _animateIdle->release();
    _animateMove->release();
    for ( int i = 1; i <= 4; i++ )
    {
        std::string strTemp = strPath + "pet_handsome_idle_%d.png";
        strTemp = MafUtils::format(strTemp.c_str(), i);
        
        auto ani = Sprite::create(strTemp);
        _vec.pushBack(ani->getSpriteFrame());
    }
    animation = Animation::createWithSpriteFrames(_vec, 0.1f);
    animate = Animate::create(animation);
    _animateIdle = animate;
    _animateIdle->retain();

    _animateMove = animate;
    _animateMove->retain();
    
    if( _eState == E_HERO_STATE::IDLE )
    {
        _eState = E_HERO_STATE::NONE;
        idle();
    }
    else if( _eState == E_HERO_STATE::MOVE )
    {
        _eState = E_HERO_STATE::IDLE;
        move();
    }
}

Handsome* Handsome::create()
{
    Handsome *sprite = new (std::nothrow) Handsome();
    if (sprite && sprite->initWithFile("Assets/character_pet/normal/handsome/pet_handsome_idle_1.png"))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

void Handsome::attack(UnitBehaviorMonster& mon)
{
    if(_eState == E_HERO_STATE::IDLE || _eState == E_HERO_STATE::ATTACK)
        return;
    
    if ( UserInfoManager::getInstance()->getHitCount(_ePlace) >= 1 )
        return;
    
    _eState = E_HERO_STATE::ATTACK;
    _bNoIdle = false;
    
    CallFunc *call = CallFunc::create(CC_CALLBACK_0(Pet::idle,this));
    Sequence *seq = Sequence::create(_animateAttack, call, nullptr);
    stopAllActions();
    runAction(seq);
}
