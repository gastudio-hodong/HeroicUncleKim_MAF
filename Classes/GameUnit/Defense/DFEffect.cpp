//
//  DFHitEffect.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2023/07/11.
//

#include "DFEffect.h"

#include "Common/ObjectPool/PoolableObjectComponent.h"


#include "GameObject/Grid/InfoGridTileData.h"
#include "GameObject/Grid/Grid.h"
#include "GameObject/Defense/InfoDefenseMap.h"
#include "ManagerGame/DefenseManager.h"
USING_NS_CC;

DFEffect* DFEffect::create()
{
    DFEffect* pRet = new(std::nothrow) DFEffect();
    if ( pRet && pRet->init() )
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

DFEffect::DFEffect() :
_num(0),
_playSpeed(1),
_playScale(1),
_effect(nullptr)
{
}

DFEffect::~DFEffect()
{
}

bool DFEffect::init()
{
    if ( !MafPoolableObject::init() )
    {
        return false;
    }
    setPosition(Vec2(9999,9999));
    
    return true;
}
void DFEffect::activateObject()
{
    MafPoolableObject::activateObject();
    updateEffect();
}
void DFEffect::deactivateObject()
{
    _effect->stopAllActions();
    _playScale = 1;
    _playSpeed = 1;
    MafPoolableObject::deactivateObject();
}
void DFEffect::setNum(int num)
{
    _num = num;
}
void DFEffect::setPlaySpeed(float value)
{
    _playSpeed = value;
}
void DFEffect::setPlayScale(float value)
{
    _playScale = value;
}
void DFEffect::setPath(std::string path)
{
    _path = path;
}
#pragma mark -utill
void DFEffect::updateEffect()
{
    //TODO 총알에 따라 변화할 수 있도록
    if(_effect == nullptr)
    {
        _effect = Sprite::create();
        _effect->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        addChild(_effect);
    }
    
    _effect->stopAllActions();
    _effect->setScale(_playScale);
    _effect->setTexture(MafUtils::format(_path.c_str(), _num ,0));
    setContentSize(_effect->getContentSize());
    _effect->setPosition(getContentSize()/2);
    
    auto anim = Animation::create();
    anim->setDelayPerUnit(0.05f / _playSpeed);
    for(int i = 0; i <= 100; ++i)
    {
        std::string strPath = MafUtils::format(_path.c_str(), _num, i);
        std::string fullpath = FileUtils::getInstance()->fullPathForFilename(strPath);
        
        if (fullpath.size() == 0)
            break;
        
        auto sprite = Sprite::create(strPath);
        sprite->getTexture()->setTexParameters(texParams);
        anim->addSpriteFrame(sprite->getSpriteFrame());
    }
    auto animate = Animate::create(anim);
    _effect->runAction(Sequence::create(animate, CallFunc::create(CC_CALLBACK_0(DFEffect::deactivateObject, this)), nullptr));
}
#pragma mark -override
Rect DFEffect::GetBoundingBox()
{
    return getBoundingBox();
}
