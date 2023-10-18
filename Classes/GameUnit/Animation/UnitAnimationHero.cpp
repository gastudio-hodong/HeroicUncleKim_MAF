//
//  Model.cpp
//  HexagonSnake
//
//  Created by SanHeo on 2014. 7. 27..
//
//

#include "UnitAnimationHero.h"

USING_NS_CC;

UnitAnimationHero* UnitAnimationHero::create()
{
    UnitAnimationHero *pRet = new (std::nothrow) UnitAnimationHero();
    if ( pRet && pRet->init() )
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        return nullptr;
    }
}

UnitAnimationHero::UnitAnimationHero() :
_eType(E_TYPE::DEFAULT),

_delayUnitWalk(0.125),
_delayUnitAttack(0.5),
_bAura(false)

{
    
}

UnitAnimationHero::~UnitAnimationHero()
{

}

bool UnitAnimationHero::init()
{
    if ( !LayerColor::initWithColor(Color4B(0, 0, 0, 0)) )
    {
        return false;
    }
    
    setContentSize(Size(232, 232));
    setIgnoreAnchorPointForPosition(false);
    setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);

    Size size = getContentSize();
   

    //
    auto uiDefault = Layer::create();
    uiDefault->setContentSize(size);
    uiDefault->setTag(E_TYPE::DEFAULT);
    addChild(uiDefault);
    {
        Vec2 pos = Vec2(size.width + 182 - size.width / 2, -39);
        
        //
        auto partShadow = Sprite::create("Assets/character_hero/normal/shadow.png");
        partShadow->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
        partShadow->setPosition(Vec2(pos.x, pos.y + 2));
        partShadow->setTag(enumToInt(E_PART::SHADOW));
        uiDefault->addChild(partShadow);
        
        //
        auto partAura = Sprite::create("Assets/character_hero/normal/effect/effect_aura_1.png");
        partAura->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
        partAura->setPosition(Vec2(pos.x - 60, pos.y));
        partAura->setTag(enumToInt(E_PART::AURA));
        uiDefault->addChild(partAura);
        {
            auto partAuraMini = Sprite::create("Assets/character_hero/normal/effect/effect_aura_1.png");
            partAuraMini->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
            partAuraMini->setPosition(Vec2(partAura->getContentSize().width / 2, 10));
            partAuraMini->setScale(0.8);
            partAuraMini->setOpacity(200);
            partAura->addChild(partAuraMini);
        }
        
        //
        std::vector<E_PART> listPart = {E_PART::BODY, E_PART::HEAD, E_PART::HAND, E_PART::FOOT, E_PART::CAPE, E_PART::WEAPON};
        for ( int i = 0; i < listPart.size(); i++ )
        {
            auto ePart = listPart.at(i);
            
            auto part = Sprite::create();
            part->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
            part->setPosition(pos);
            part->setTag(enumToInt(ePart));
            uiDefault->addChild(part, 1);
            
            if ( ePart != E_PART::BODY )
            {
                part->setScale(4);
            }
        }
    }
    
    auto uiSkin = Layer::create();
    uiSkin->setContentSize(size);
    uiSkin->setTag(E_TYPE::SKIN);
    addChild(uiSkin);
    {
        Vec2 pos = Vec2(size.width + 182 - size.width / 2, -39);
        
        auto part = Sprite::create();
        part->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
        part->setPosition(pos);
        part->setTag(enumToInt(E_PART::BODY));
        uiSkin->addChild(part, 1);
    }
    
    auto uiHero = Layer::create();
    uiHero->setContentSize(size);
    uiHero->setTag(E_TYPE::HERO);
    addChild(uiHero);
    {
        
    }
    
    
    
    //
    setMotionCostume(1, 1, 1, 1, 1, 1);
    
    return true;
}

#pragma mark -
void UnitAnimationHero::setMotionCostume(int weapon, int body, int head, int hand, int foot, int cape)
{
    //
    _eType = E_TYPE::DEFAULT;
    
    //
    std::string pathWalkWeapon = MafUtils::format("Assets/character_hero/normal/weapon/weapon_%.2d_walk_%s.png", weapon, "%d");
    std::string pathWalkBody = MafUtils::format("Assets/character_hero/normal/body/body_%.2d_walk_%s.png", body, "%d");
    std::string pathWalkHead = MafUtils::format("Assets/character_hero/normal/head/head_%.2d_walk_%s.png", head, "%d");
    std::string pathWalkHand = MafUtils::format("Assets/character_hero/normal/hand/hand_%.2d_walk_%s.png", hand, "%d");
    std::string pathWalkFoot = MafUtils::format("Assets/character_hero/normal/foot/foot_%.2d_walk_%s.png", foot, "%d");
    std::string pathWalkCape = MafUtils::format("Assets/character_hero/normal/cape/cape_%.2d_walk_%s.png", cape, "%d");
    _listAnimateMove.clear();
    _listAnimateMove.insert(E_PART::BODY, getMotionAnimate(pathWalkBody, _delayUnitWalk));
    _listAnimateMove.insert(E_PART::HEAD, getMotionAnimate(pathWalkHead, _delayUnitWalk));
    _listAnimateMove.insert(E_PART::HAND, getMotionAnimate(pathWalkHand, _delayUnitWalk));
    _listAnimateMove.insert(E_PART::FOOT, getMotionAnimate(pathWalkFoot, _delayUnitWalk));
    _listAnimateMove.insert(E_PART::CAPE, getMotionAnimate(pathWalkCape, _delayUnitWalk));
    _listAnimateMove.insert(E_PART::WEAPON, getMotionAnimate(pathWalkWeapon, _delayUnitWalk));
    _listAnimateMove.insert(E_PART::AURA, getMotionAnimate("Assets/character_hero/normal/effect/effect_aura_%d.png", 0.1));
    
    //
    std::string pathAttackWeapon = MafUtils::format("Assets/character_hero/normal/weapon/weapon_%.2d_attack_%s.png", weapon, "%d");
    std::string pathAttackBody = MafUtils::format("Assets/character_hero/normal/body/body_%.2d_attack_%s.png", body, "%d");
    std::string pathAttackHead = MafUtils::format("Assets/character_hero/normal/head/head_%.2d_attack_%s.png", head, "%d");
    std::string pathAttackHand = MafUtils::format("Assets/character_hero/normal/hand/hand_%.2d_attack_%s.png", hand, "%d");
    std::string pathAttackFoot = MafUtils::format("Assets/character_hero/normal/foot/foot_%.2d_attack_%s.png", foot, "%d");
    std::string pathAttackCape = MafUtils::format("Assets/character_hero/normal/cape/cape_%.2d_attack_%s.png", cape, "%d");
    _listAnimateAttack.clear();
    _listAnimateAttack.insert(E_PART::BODY, getMotionAnimate(pathAttackBody, _delayUnitAttack));
    _listAnimateAttack.insert(E_PART::HEAD, getMotionAnimate(pathAttackHead, _delayUnitAttack));
    _listAnimateAttack.insert(E_PART::HAND, getMotionAnimate(pathAttackHand, _delayUnitAttack));
    _listAnimateAttack.insert(E_PART::FOOT, getMotionAnimate(pathAttackFoot, _delayUnitAttack));
    _listAnimateAttack.insert(E_PART::CAPE, getMotionAnimate(pathAttackCape, _delayUnitAttack));
    _listAnimateAttack.insert(E_PART::WEAPON, getMotionAnimate(pathAttackWeapon, _delayUnitAttack));
    _listAnimateAttack.insert(E_PART::AURA, getMotionAnimate("Assets/character_hero/normal/effect/effect_aura_%d.png", 0.1));
}

void UnitAnimationHero::setMotionSkin(int skin)
{
    //
    _eType = E_TYPE::SKIN;
    
    //
    std::string pathWalk = MafUtils::format("Assets/character_hero/skin/skin_%02d_walk_%s.png", skin, "%d");
    _listAnimateMove.clear();
    _listAnimateMove.insert(E_PART::BODY, getMotionAnimate(pathWalk, _delayUnitWalk));
    
    //
    std::string pathAttack = MafUtils::format("Assets/character_hero/skin/skin_%02d_attack_%s.png", skin, "%d");
    _listAnimateAttack.clear();
    _listAnimateAttack.insert(E_PART::BODY, getMotionAnimate(pathAttack, _delayUnitAttack));
}

void UnitAnimationHero::setMotionHero()
{
}

void UnitAnimationHero::setDelayUnitWalk(double delayUnit)
{
    _delayUnitWalk = delayUnit;
}

void UnitAnimationHero::setDelayUnitAttack(double delayUnit)
{
    _delayUnitAttack = delayUnit;
}

void UnitAnimationHero::setAura(bool bAura, cocos2d::Color3B color)
{
    _bAura = bAura;
    _colorAura = color;
}

#pragma mark - animation
Animate* UnitAnimationHero::getMotionAnimate(std::string path, double delayPerUnit)
{
    Vector<cocos2d::SpriteFrame*> list;
    
    int count = 0;
    
    for ( int i = 1; i <= 100; i++ )
    {
        std::string strTempPath = MafUtils::format(path.c_str(), i);
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
    
    delayPerUnit = delayPerUnit / (count / 4);
    
    if ( list.size() <= 0 )
    {
        return nullptr;
    }
    else
    {
        auto animation = Animation::createWithSpriteFrames(list, delayPerUnit);
        auto animate = Animate::create(animation);
        return animate;
    }
}

void UnitAnimationHero::animationWalk()
{
    auto listChild = getChildren();
    for ( int i = 0; i < listChild.size(); i++ )
    {
        auto child = listChild.at(i);
        child->setVisible(false);
    }
    
    //
    if ( _eType == E_TYPE::DEFAULT )
    {
        auto uiLayout = getChildByTag<Layer*>(E_TYPE::DEFAULT);
        uiLayout->setVisible(true);
        
        // body
        {
            auto animate = _listAnimateMove.at(E_PART::BODY)->clone();

            auto part = uiLayout->getChildByTag<Sprite*>(E_PART::BODY);
            part->stopAllActions();
            part->runAction(RepeatForever::create(animate));
        }
        
        std::vector<E_PART> listPart = {E_PART::HEAD, E_PART::HAND, E_PART::FOOT, E_PART::CAPE, E_PART::WEAPON};
        for ( int i = 0; i < listPart.size(); i++ )
        {
            auto ePart = listPart.at(i);

            auto animate = _listAnimateMove.at(ePart)->clone();

            auto part = uiLayout->getChildByTag<Sprite*>(ePart);
            part->stopAllActions();
            part->runAction(RepeatForever::create(animate));
        }
        
        //
        auto partShadow = uiLayout->getChildByTag<Sprite*>(E_PART::SHADOW);
        partShadow->setVisible(true);
        
        //
        if ( _bAura == true )
        {
            auto partAura = uiLayout->getChildByTag<Sprite*>(E_PART::AURA);
            partAura->setVisible(true);
            partAura->setColor(_colorAura);
            partAura->stopAllActions();
            partAura->runAction(RepeatForever::create(_listAnimateMove.at(E_PART::AURA)));
        }
        else
        {
            auto partAura = uiLayout->getChildByTag<Sprite*>(E_PART::AURA);
            partAura->stopAllActions();
            partAura->setVisible(false);
        }
    }
    else if ( _eType == E_TYPE::SKIN )
    {
        auto uiLayout = getChildByTag<Layer*>(E_TYPE::SKIN);
        uiLayout->setVisible(true);
        
        // body
        {
            auto animate = _listAnimateMove.at(E_PART::BODY)->clone();

            auto part = uiLayout->getChildByTag<Sprite*>(E_PART::BODY);
            part->stopAllActions();
            part->runAction(RepeatForever::create(animate));
        }
    }
    else if ( _eType == E_TYPE::HERO )
    {
        
    }
}

void UnitAnimationHero::animationAttack(bool bLoop)
{
    auto listChild = getChildren();
    for ( int i = 0; i < listChild.size(); i++ )
    {
        auto child = listChild.at(i);
        child->setVisible(false);
    }
    
    //
    if ( _eType == E_TYPE::DEFAULT )
    {
        auto uiLayout = getChildByTag<Layer*>(E_TYPE::DEFAULT);
        uiLayout->setVisible(true);
        
        // body
        {
            auto animate = _listAnimateAttack.at(E_PART::BODY)->clone();

            auto part = uiLayout->getChildByTag<Sprite*>(E_PART::BODY);
            if ( bLoop == true )
            {
                part->stopAllActions();
                part->runAction(RepeatForever::create(animate));
            }
            else
            {
                part->stopAllActions();
                part->runAction(Sequence::create(animate, CallFunc::create(CC_CALLBACK_0(UnitAnimationHero::animationWalk, this)), nullptr));
            }
        }

        //
        std::vector<E_PART> listPart = {E_PART::HEAD, E_PART::HAND, E_PART::FOOT, E_PART::CAPE, E_PART::WEAPON};
        for ( int i = 0; i < listPart.size(); i++ )
        {
            auto ePart = listPart.at(i);

            auto animate = _listAnimateAttack.at(ePart)->clone();

            auto part = uiLayout->getChildByTag<Sprite*>(ePart);
            if ( bLoop == true )
            {
                part->stopAllActions();
                part->runAction(RepeatForever::create(animate));
            }
            else
            {
                part->stopAllActions();
                part->runAction(animate);
            }
        }

        //
        auto partShadow = uiLayout->getChildByTag<Sprite*>(E_PART::SHADOW);
        partShadow->setVisible(true);

        //
        if ( _bAura == true )
        {
            auto partAura = uiLayout->getChildByTag<Sprite*>(E_PART::AURA);
            partAura->setVisible(true);
            partAura->setColor(_colorAura);
            partAura->stopAllActions();
            partAura->runAction(RepeatForever::create(_listAnimateAttack.at(E_PART::AURA)));
        }
        else
        {
            auto partAura = uiLayout->getChildByTag<Sprite*>(E_PART::AURA);
            partAura->stopAllActions();
            partAura->setVisible(false);
        }
    }
    else if ( _eType == E_TYPE::SKIN )
    {
        auto uiLayout = getChildByTag<Layer*>(E_TYPE::SKIN);
        uiLayout->setVisible(true);
        
        // body
        {
            auto animate = _listAnimateAttack.at(E_PART::BODY)->clone();

            
            auto part = uiLayout->getChildByTag<Sprite*>(E_PART::BODY);
            if ( bLoop == true )
            {
                part->stopAllActions();
                part->runAction(RepeatForever::create(animate));
            }
            else
            {
                part->stopAllActions();
                part->runAction(Sequence::create(animate, CallFunc::create(CC_CALLBACK_0(UnitAnimationHero::animationWalk, this)), nullptr));
            }
        }
    }
    else if ( _eType == E_TYPE::HERO )
    {
        
    }
    
}
