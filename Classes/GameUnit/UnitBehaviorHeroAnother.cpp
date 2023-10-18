//
//  Model.cpp
//  HexagonSnake
//
//  Created by SanHeo on 2014. 7. 27..
//
//

#include "UnitBehaviorHeroAnother.h"

#include "Common/ConfigGameNormal.h"

#include "ManagerGame/WeaponManager.h"

USING_NS_CC;

UnitBehaviorHeroAnother* UnitBehaviorHeroAnother::create(E_PLACE ePlace)
{
    UnitBehaviorHeroAnother *pRet = new (std::nothrow) UnitBehaviorHeroAnother();
    if ( pRet )
    {
        pRet->setPlace(ePlace);
        pRet->init();
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        return nullptr;
    }
}
UnitBehaviorHeroAnother::UnitBehaviorHeroAnother() :
_time(0),
_isAttack(false),
_isSkin(false),
_animatePartSkinMove(nullptr),
_animatePartSkinAttack(nullptr),
_spritePartSkin(nullptr)
{
    _chaScale = NormalScale;
}

UnitBehaviorHeroAnother::~UnitBehaviorHeroAnother()
{

}

bool UnitBehaviorHeroAnother::init()
{
    if ( !UnitBehavior::init())
    {
        return false;
    }

    //
    initParts();
    
    return true;
}

void UnitBehaviorHeroAnother::initParts()
{
    //
    _layerCharacter = Layer::create();
    _layerCharacter->setContentSize(Size(232, 232));
    _layerCharacter->setIgnoreAnchorPointForPosition(false);
    _layerCharacter->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _layerCharacter->setPosition(Vec2::ZERO);
    addChild(_layerCharacter);
    
    //
    _spritePartDefault[E_HERO::BODY] = Sprite::create("Assets/character_hero/normal/body/body_01_walk_1.png");
    _spritePartDefault[E_HERO::BODY]->setAnchorPoint(Vec2::ZERO);
    _spritePartDefault[E_HERO::BODY]->setPosition(Vec2::ZERO);
    _layerCharacter->addChild(_spritePartDefault[E_HERO::BODY],E_HERO::BODY);
    
    _spritePartDefault[E_HERO::WEAPON] = Sprite::create("Assets/character_hero/normal/weapon/weapon_01_walk_1.png");
    _spritePartDefault[E_HERO::WEAPON]->setAnchorPoint(Vec2::ZERO);
    _spritePartDefault[E_HERO::WEAPON]->setPosition(Vec2::ZERO);
    _spritePartDefault[E_HERO::WEAPON]->getTexture()->setTexParameters(texParams);
    _spritePartDefault[E_HERO::WEAPON]->setScale(_chaScale);
    _layerCharacter->addChild(_spritePartDefault[E_HERO::WEAPON],E_HERO::WEAPON);
    
    _spritePartDefault[E_HERO::HEAD] = Sprite::create("Assets/character_hero/normal/head/head_01_walk_1.png");
    _spritePartDefault[E_HERO::HEAD]->setAnchorPoint(Vec2::ZERO);
    _spritePartDefault[E_HERO::HEAD]->setPosition(Vec2::ZERO);
    _spritePartDefault[E_HERO::HEAD]->getTexture()->setTexParameters(texParams);
    _spritePartDefault[E_HERO::HEAD]->setScale(_chaScale);
    _layerCharacter->addChild(_spritePartDefault[E_HERO::HEAD],E_HERO::HEAD);
    
    _spritePartDefault[E_HERO::HAND] = Sprite::create("Assets/character_hero/normal/hand/hand_01_walk_1.png");
    _spritePartDefault[E_HERO::HAND]->setAnchorPoint(Vec2::ZERO);
    _spritePartDefault[E_HERO::HAND]->setPosition(Vec2::ZERO);
    _spritePartDefault[E_HERO::HAND]->getTexture()->setTexParameters(texParams);
    _spritePartDefault[E_HERO::HAND]->setScale(_chaScale);
    _layerCharacter->addChild(_spritePartDefault[E_HERO::HAND],E_HERO::HAND);
    
    _spritePartDefault[E_HERO::FOOT] = Sprite::create("Assets/character_hero/normal/foot/foot_01_walk_1.png");
    _spritePartDefault[E_HERO::FOOT]->setAnchorPoint(Vec2::ZERO);
    _spritePartDefault[E_HERO::FOOT]->setPosition(Vec2::ZERO);
    _spritePartDefault[E_HERO::FOOT]->getTexture()->setTexParameters(texParams);
    _spritePartDefault[E_HERO::FOOT]->setScale(_chaScale);
    _layerCharacter->addChild(_spritePartDefault[E_HERO::FOOT],E_HERO::FOOT);
    
    _spritePartDefault[E_HERO::CAPE] = Sprite::create("Assets/character_hero/normal/cape/cape_01_walk_1.png");
    _spritePartDefault[E_HERO::CAPE]->setAnchorPoint(Vec2::ZERO);
    _spritePartDefault[E_HERO::CAPE]->setPosition(Vec2::ZERO);
    _spritePartDefault[E_HERO::CAPE]->getTexture()->setTexParameters(texParams);
    _spritePartDefault[E_HERO::CAPE]->setScale(_chaScale);
    _layerCharacter->addChild(_spritePartDefault[E_HERO::CAPE],E_HERO::CAPE);
    
    _labName = Label::createWithTTF("", GAME_FONT, 24);
    _labName->setPosition(_layerCharacter->getContentSize().width*0.25, _layerCharacter->getContentSize().height-50);
    _layerCharacter->addChild(_labName);
    
    _spritePartSkin = Sprite::create();
    _spritePartSkin->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    _spritePartSkin->setPosition(_layerCharacter->getContentSize().width, 0);
    _layerCharacter->addChild(_spritePartSkin);
    _spritePartSkin->setVisible(false);
}

#pragma mark - AnotherHero : set, get
void UnitBehaviorHeroAnother::setName(const char* name, bool up)
{
    if (up)
    {
        _labName->setPosition(_labName->getPosition().x, _labName->getPosition().y+30);
    }
    _labName->setString(name);
}

#pragma mark - AnotherHero : character 모양 및 위치
void UnitBehaviorHeroAnother::setMotion(int weapon, int p1,int p2,int p3,int p4, int p5)
{
    if (weapon > WeaponManager::getInstance()->getCount()) weapon = WeaponManager::getInstance()->getCount();
    if (p1 > C_COUNT_COSTUME)   p1 = C_COUNT_COSTUME;
    if (p2 > C_COUNT_COSTUME)   p2 = C_COUNT_COSTUME;
    if (p3 > C_COUNT_COSTUME)   p3 = C_COUNT_COSTUME;
    if (p4 > C_COUNT_COSTUME)   p4 = C_COUNT_COSTUME;
    if (p5 > C_COUNT_COSTUME)   p5 = C_COUNT_COSTUME;

    Vector<cocos2d::SpriteFrame*> _vec;
    Animation *animation;
    Animate *animate;

    _vec.clear();
    for(int i=1; i<=4; i++){
        auto name = MafUtils::format("Assets/character_hero/normal/weapon/weapon_%.2d_walk_%d.png",weapon,i);
        auto ani = Sprite::create(name);
        ani->getTexture()->setTexParameters(texParams);
        ani->setScale(_chaScale);
        _vec.pushBack(ani->getSpriteFrame());
    }
    animation = Animation::createWithSpriteFrames(_vec, 0.125f);
    animate = Animate::create(animation);
    _seqMoveWeapon = Sequence::create(animate, nullptr);
    _vecSequence.pushBack(_seqMoveWeapon);

    _vec.clear();
    for(int i=1; i<=4; i++){
        int idx = p2;
        auto name = MafUtils::format("Assets/character_hero/normal/body/body_%.2d_walk_%d.png",idx,i);
        auto ani = Sprite::create(name);
        _vec.pushBack(ani->getSpriteFrame());
    }
    animation = Animation::createWithSpriteFrames(_vec, 0.125f);
    animate = Animate::create(animation);
    _seqMoveBody = Sequence::create(animate, nullptr);
    _vecSequence.pushBack(_seqMoveBody);

    _vec.clear();
    for(int i=1; i<=4; i++){
        int idx = p1;
        auto name = MafUtils::format("Assets/character_hero/normal/head/head_%.2d_walk_%d.png",idx,i);
        auto ani = Sprite::create(name);
        ani->getTexture()->setTexParameters(texParams);
        ani->setScale(_chaScale);
        _vec.pushBack(ani->getSpriteFrame());
    }
    animation = Animation::createWithSpriteFrames(_vec, 0.125f);
    animate = Animate::create(animation);
    _seqMoveHead = Sequence::create(animate, nullptr);
    _vecSequence.pushBack(_seqMoveHead);

    _vec.clear();
    for(int i=1; i<=4; i++){
        int idx = p4;
        auto name = MafUtils::format("Assets/character_hero/normal/hand/hand_%.2d_walk_%d.png",idx,i);
        auto ani = Sprite::create(name);
        ani->getTexture()->setTexParameters(texParams);
        ani->setScale(_chaScale);
        _vec.pushBack(ani->getSpriteFrame());
    }
    animation = Animation::createWithSpriteFrames(_vec, 0.125f);
    animate = Animate::create(animation);
    _seqMoveHand = Sequence::create(animate, nullptr);
    _vecSequence.pushBack(_seqMoveHand);

    _vec.clear();
    for(int i=1; i<=4; i++){
        int idx = p5;
        auto name = MafUtils::format("Assets/character_hero/normal/foot/foot_%.2d_walk_%d.png",idx,i);
        auto ani = Sprite::create(name);
        ani->getTexture()->setTexParameters(texParams);
        ani->setScale(_chaScale);
        _vec.pushBack(ani->getSpriteFrame());
    }
    animation = Animation::createWithSpriteFrames(_vec, 0.125f);
    animate = Animate::create(animation);
    _seqMoveFoot = Sequence::create(animate, nullptr);
    _vecSequence.pushBack(_seqMoveFoot);

    _vec.clear();
    for(int i=1; i<=4; i++){
        int idx = p3;
        auto name = MafUtils::format("Assets/character_hero/normal/cape/cape_%.2d_walk_%d.png",idx,i);
        auto ani = Sprite::create(name);
        ani->getTexture()->setTexParameters(texParams);
        ani->setScale(_chaScale);
        _vec.pushBack(ani->getSpriteFrame());
    }
    animation = Animation::createWithSpriteFrames(_vec, 0.125f);
    animate = Animate::create(animation);
    _seqMoveCape = Sequence::create(animate, nullptr);
    _vecSequence.pushBack(_seqMoveCape);
    
    _vec.clear();
    for(int i=1; i<=4; i++){
        auto name = MafUtils::format("Assets/character_hero/normal/weapon/weapon_%.2d_attack_%d.png",weapon,i);
        auto ani = Sprite::create(name);
        ani->getTexture()->setTexParameters(texParams);
        ani->setScale(_chaScale);
        _vec.pushBack(ani->getSpriteFrame());
    }
    animation = Animation::createWithSpriteFrames(_vec, 0.125f);
    animate = Animate::create(animation);
    _seqAttackWeapon = Sequence::create(animate, nullptr);
    _vecSequence.pushBack(_seqAttackWeapon);

    _vec.clear();
    for(int i=1; i<=4; i++){
        int idx = p2;
        auto name = MafUtils::format("Assets/character_hero/normal/body/body_%.2d_attack_%d.png",idx,i);
        auto ani = Sprite::create(name);
        _vec.pushBack(ani->getSpriteFrame());
    }
    animation = Animation::createWithSpriteFrames(_vec, 0.125f);
    animate = Animate::create(animation);
    _seqAttackBody = Sequence::create(animate, nullptr);
    _vecSequence.pushBack(_seqAttackBody);

    _vec.clear();
    for(int i=1; i<=4; i++){
        int idx = p1;
        auto name = MafUtils::format("Assets/character_hero/normal/head/head_%.2d_attack_%d.png",idx,i);
        auto ani = Sprite::create(name);
        ani->getTexture()->setTexParameters(texParams);
        ani->setScale(_chaScale);
        _vec.pushBack(ani->getSpriteFrame());
    }
    animation = Animation::createWithSpriteFrames(_vec, 0.125f);
    animate = Animate::create(animation);
    _seqAttackHead = Sequence::create(animate, nullptr);
    _vecSequence.pushBack(_seqAttackHead);

    _vec.clear();
    for(int i=1; i<=4; i++){
        int idx = p4;
        auto name = MafUtils::format("Assets/character_hero/normal/hand/hand_%.2d_attack_%d.png",idx,i);
        auto ani = Sprite::create(name);
        ani->getTexture()->setTexParameters(texParams);
        ani->setScale(_chaScale);
        _vec.pushBack(ani->getSpriteFrame());
    }
    animation = Animation::createWithSpriteFrames(_vec, 0.125f);
    animate = Animate::create(animation);
    _seqAttackHand = Sequence::create(animate, nullptr);
    _vecSequence.pushBack(_seqAttackHand);

    _vec.clear();
    for(int i=1; i<=4; i++){
        int idx = p5;
        auto name = MafUtils::format("Assets/character_hero/normal/foot/foot_%.2d_attack_%d.png",idx,i);
        auto ani = Sprite::create(name);
        ani->getTexture()->setTexParameters(texParams);
        ani->setScale(_chaScale);
        _vec.pushBack(ani->getSpriteFrame());
    }
    animation = Animation::createWithSpriteFrames(_vec, 0.125f);
    animate = Animate::create(animation);
    _seqAttackFoot = Sequence::create(animate, nullptr);
    _vecSequence.pushBack(_seqAttackFoot);

    _vec.clear();
    for(int i=1; i<=4; i++){
        int idx = p3;
        auto name = MafUtils::format("Assets/character_hero/normal/cape/cape_%.2d_attack_%d.png",idx,i);
        auto ani = Sprite::create(name);
        ani->getTexture()->setTexParameters(texParams);
        ani->setScale(_chaScale);
        _vec.pushBack(ani->getSpriteFrame());
    }
    animation = Animation::createWithSpriteFrames(_vec, 0.125f);
    animate = Animate::create(animation);
    _seqAttackCape = Sequence::create(animate, nullptr);
    _vecSequence.pushBack(_seqAttackCape);
}

void UnitBehaviorHeroAnother::setHeroFlipX(bool value)
{
    _spritePartDefault[E_HERO::HEAD]->setFlippedX(value);
    _spritePartDefault[E_HERO::BODY]->setFlippedX(value);
    _spritePartDefault[E_HERO::CAPE]->setFlippedX(value);
    _spritePartDefault[E_HERO::HAND]->setFlippedX(value);
    _spritePartDefault[E_HERO::FOOT]->setFlippedX(value);
    _spritePartDefault[E_HERO::WEAPON]->setFlippedX(value);

    if (value)
    {
        _labName->setPosition(_spritePartDefault[E_HERO::BODY]->getContentSize().width*0.75, _labName->getPosition().y);
    }
}


void UnitBehaviorHeroAnother::setImage(int weapon, int p1, int p2, int p3, int p4, int p5)
{
    _spritePartDefault[E_HERO::BODY]->setVisible(true);
    _spritePartDefault[E_HERO::HEAD]->setVisible(true);
    _spritePartDefault[E_HERO::HAND]->setVisible(true);
    _spritePartDefault[E_HERO::FOOT]->setVisible(true);
    _spritePartDefault[E_HERO::CAPE]->setVisible(true);
    _spritePartDefault[E_HERO::WEAPON]->setVisible(true);
    _isSkin = false;
    
    _spritePartDefault[E_HERO::BODY]->setTexture(MafUtils::format("Assets/character_hero/normal/body/body_%.2d_walk_1.png",p2));
    _spritePartDefault[E_HERO::HEAD]->setTexture(MafUtils::format("Assets/character_hero/normal/head/head_%.2d_walk_1.png",p1));
    _spritePartDefault[E_HERO::HAND]->setTexture(MafUtils::format("Assets/character_hero/normal/hand/hand_%.2d_walk_1.png",p4));
    _spritePartDefault[E_HERO::FOOT]->setTexture(MafUtils::format("Assets/character_hero/normal/foot/foot_%.2d_walk_1.png",p5));
    _spritePartDefault[E_HERO::CAPE]->setTexture(MafUtils::format("Assets/character_hero/normal/cape/cape_%.2d_walk_1.png",p3));
    _spritePartDefault[E_HERO::WEAPON]->setTexture(MafUtils::format("Assets/character_hero/normal/weapon/weapon_%.2d_walk_1.png",weapon));
}

void UnitBehaviorHeroAnother::setSkin(int skinIdx)
{
    std::string strPathWalk = "";
    std::string strPathAttack = "";
    
    _animatePartSkinMove = nullptr;
    _animatePartSkinAttack = nullptr;
    
    int idx = skinIdx;
    
    strPathWalk = MafUtils::format("Assets/character_hero/skin/skin_%02d_walk_%s.png", idx, "%d");
    strPathAttack = MafUtils::format("Assets/character_hero/skin/skin_%02d_attack_%s.png", idx, "%d");
        
    _vecSequence.clear();
    _spritePartSkin->stopAllActions();
    _animatePartSkinMove = getMotionDefault(strPathWalk, 0.125f);
    _animatePartSkinAttack = getMotionDefault(strPathAttack, 0.125f);
    _vecSequence.pushBack(Sequence::create(_animatePartSkinMove, nullptr));
    _vecSequence.pushBack(Sequence::create(_animatePartSkinAttack, nullptr));
    
    //
    
    _spritePartDefault[E_HERO::BODY]->setVisible(false);
    _spritePartDefault[E_HERO::HEAD]->setVisible(false);
    _spritePartDefault[E_HERO::HAND]->setVisible(false);
    _spritePartDefault[E_HERO::FOOT]->setVisible(false);
    _spritePartDefault[E_HERO::CAPE]->setVisible(false);
    _spritePartDefault[E_HERO::WEAPON]->setVisible(false);
    _isSkin = true;
}

#pragma mark - AnotherHero : action default
void UnitBehaviorHeroAnother::actionMove()
{
    if(_isSkin)
    {
        _spritePartSkin->setVisible(true);
        _spritePartSkin->stopAllActions();
        _spritePartSkin->runAction(RepeatForever::create(_animatePartSkinMove->clone()));
    }
    else
    {
        _spritePartSkin->setVisible(false);
        _spritePartDefault[E_HERO::BODY]->stopAllActions();
        _spritePartDefault[E_HERO::HEAD]->stopAllActions();
        _spritePartDefault[E_HERO::HAND]->stopAllActions();
        _spritePartDefault[E_HERO::FOOT]->stopAllActions();
        _spritePartDefault[E_HERO::CAPE]->stopAllActions();
        _spritePartDefault[E_HERO::WEAPON]->stopAllActions();
        
        _spritePartDefault[E_HERO::BODY]->runAction(RepeatForever::create(_seqMoveBody->clone()));
        _spritePartDefault[E_HERO::HEAD]->runAction(RepeatForever::create(_seqMoveHead->clone()));
        _spritePartDefault[E_HERO::HAND]->runAction(RepeatForever::create(_seqMoveHand->clone()));
        _spritePartDefault[E_HERO::FOOT]->runAction(RepeatForever::create(_seqMoveFoot->clone()));
        _spritePartDefault[E_HERO::CAPE]->runAction(RepeatForever::create(_seqMoveCape->clone()));
        _spritePartDefault[E_HERO::WEAPON]->runAction(RepeatForever::create(_seqMoveWeapon->clone()));
    }
}

void UnitBehaviorHeroAnother::actionAttack(bool isAutoMove)
{
    if(_isAttack)
        return;
    
    _isAttack = true;
    if(_isSkin)
    {
        _spritePartSkin->setVisible(true);
        _spritePartSkin->stopAllActions();
        if(isAutoMove)
            _spritePartSkin->runAction(Sequence::create(_animatePartSkinAttack->clone(), CallFunc::create([=](){
                _isAttack = false;
                actionMove();
            }), nullptr));
        else
            _spritePartSkin->runAction(Sequence::create(_animatePartSkinAttack->clone(), CallFunc::create([=](){
                _isAttack = false;
            }), nullptr));
    }
    else
    {
        _spritePartSkin->setVisible(false);
        _spritePartDefault[E_HERO::BODY]->stopAllActions();
        _spritePartDefault[E_HERO::HEAD]->stopAllActions();
        _spritePartDefault[E_HERO::HAND]->stopAllActions();
        _spritePartDefault[E_HERO::FOOT]->stopAllActions();
        _spritePartDefault[E_HERO::CAPE]->stopAllActions();
        _spritePartDefault[E_HERO::WEAPON]->stopAllActions();
        
        if(isAutoMove)
        {
            _spritePartDefault[E_HERO::BODY]->runAction(_seqAttackBody->clone());
            _spritePartDefault[E_HERO::HEAD]->runAction(_seqAttackHead->clone());
            _spritePartDefault[E_HERO::HAND]->runAction(_seqAttackHand->clone());
            _spritePartDefault[E_HERO::FOOT]->runAction(_seqAttackFoot->clone());
            _spritePartDefault[E_HERO::CAPE]->runAction(_seqAttackCape->clone());
            _spritePartDefault[E_HERO::WEAPON]->runAction(Sequence::create(_seqAttackWeapon->clone(), CallFunc::create([=](){
                _isAttack = false;
                actionMove();
            }), nullptr));
        }
        else
        {
            _spritePartDefault[E_HERO::BODY]->runAction(_seqAttackBody->clone());
            _spritePartDefault[E_HERO::HEAD]->runAction(_seqAttackHead->clone());
            _spritePartDefault[E_HERO::HAND]->runAction(_seqAttackHand->clone());
            _spritePartDefault[E_HERO::FOOT]->runAction(_seqAttackFoot->clone());
            _spritePartDefault[E_HERO::CAPE]->runAction(_seqAttackCape->clone());
            _spritePartDefault[E_HERO::WEAPON]->runAction(Sequence::create(_seqAttackWeapon->clone(), CallFunc::create([=](){
                _isAttack = false;
            }), nullptr));
        }
    }
}
 
Animate* UnitBehaviorHeroAnother::getMotionDefault(std::string strPath, double delay)
{
    Vector<cocos2d::SpriteFrame*> list;
    
    int count = 0;
    
    for ( int i = 1; i <= 100; i++ )
    {
        std::string strTempPath = MafUtils::format(strPath.c_str(), i);
        std::string fullpath = FileUtils::getInstance()->fullPathForFilename(strTempPath);
        
        if(fullpath.size() == 0)
            break;
        
        auto spriteAni = Sprite::create(strTempPath);
        if ( spriteAni == nullptr )
        {
            return nullptr;
        }
        
        spriteAni->getTexture()->setTexParameters(texParams);
        list.pushBack(spriteAni->getSpriteFrame());
        count = i;
    }
    
    delay = delay / (count / 4);
    
    if ( list.size() <= 0 )
    {
        return nullptr;
    }
    else
    {
        auto animation = Animation::createWithSpriteFrames(list, delay);
        auto animate = Animate::create(animation);
        return animate;
    }
}
