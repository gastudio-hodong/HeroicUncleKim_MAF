//
//  Model.cpp
//  HexagonSnake
//
//  Created by SanHeo on 2014. 7. 27..
//
//

#include "UnitHeroShadow.h"

#include "ManagerGame/UserInfoManager.h"

USING_NS_CC;

UnitHeroShadow* UnitHeroShadow::create()
{
    UnitHeroShadow *pRet = new (std::nothrow) UnitHeroShadow();
    if ( pRet )
    {
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

UnitHeroShadow::UnitHeroShadow() :
_bShow(false),

_animatePartDefaultAttack(nullptr),
_spritePartDefault(nullptr)
{

}

UnitHeroShadow::~UnitHeroShadow()
{
    if ( _animatePartDefaultAttack != nullptr )    _animatePartDefaultAttack->release();
}

bool UnitHeroShadow::init()
{
    if ( !Layer::init())
    {
        return false;
    }
        
    //
    initParts();
    
    return true;
}

void UnitHeroShadow::initParts()
{
    auto size = Size(232, 232);
    
    double delayUnitAttack = UserInfoManager::getInstance()->getAttackSpeed(E_PLACE::DG_NORMAL);
    delayUnitAttack *= 0.25;
    
    //
    setContentSize(size);
    setIgnoreAnchorPointForPosition(false);
    setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    
    //
    _spritePartDefault = Sprite::create();
    _spritePartDefault->setAnchorPoint(Vec2::ZERO);
    _spritePartDefault->setPosition(Vec2::ZERO);
    _spritePartDefault->getTexture()->setTexParameters(texParams);
    _spritePartDefault->setScale(NormalScale);
    _spritePartDefault->setFlippedX(true);
    _spritePartDefault->setVisible(false);
    addChild(_spritePartDefault);
   
    
    
    // body
    {
        Vector<cocos2d::SpriteFrame*> list;
        for ( int i = 1; i <= 4; i++ )
        {
            std::string strTempPath = MafUtils::format("Assets/battle/attack_%d.png", i);
            
            auto spriteAni = Sprite::create(strTempPath);
            spriteAni->getTexture()->setTexParameters(texParams);
            list.pushBack(spriteAni->getSpriteFrame());
        }
        
        auto animation = Animation::createWithSpriteFrames(list, delayUnitAttack);
        _animatePartDefaultAttack = Animate::create(animation);
        _animatePartDefaultAttack->retain();
    }
}

#pragma mark - Hero : set, get
void UnitHeroShadow::setShow()
{
    if ( !_bShow )
    {
        _bShow = true;
        
        _spritePartDefault->setVisible(true);
    }
}

void UnitHeroShadow::setHide()
{
    if (_bShow)
    {
        _bShow = false;
        
        _spritePartDefault->setVisible(false);
        if ( _spritePartDefault->getNumberOfRunningActions() != 0 )
        {
            _spritePartDefault->stopAllActions();
        }
    }
}

#pragma mark - Shadow : action default
void UnitHeroShadow::actionAttack()
{
    if ( this->getNumberOfRunningActions() != 0 )
    {
        this->stopAllActions();
    }
    
    if ( _spritePartDefault->getNumberOfRunningActions() != 0 )
    {
        _spritePartDefault->stopAllActions();
    }
    _spritePartDefault->runAction(RepeatForever::create(_animatePartDefaultAttack->clone()));
}

