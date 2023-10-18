//
//  DFExplosion.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2023/07/19.
//

#include "DFExplosion.h"

#include "Common/ObjectPool/PoolableObjectComponent.h"
#include "GameUnit/Defense/DFBattleManager.h"

#include "GameUnit/Defense/DFMonster.h"

#include "GameObject/Grid/InfoGridTileData.h"
#include "GameObject/Grid/Grid.h"
#include "GameObject/Defense/InfoDefenseBullet.h"
#include "ManagerGame/DefenseManager.h"
USING_NS_CC;

DFExplosion* DFExplosion::create()
{
    DFExplosion* pRet = new(std::nothrow) DFExplosion();
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

DFExplosion::DFExplosion() :
_isCollisionActivate(false),
_damage(0),
_bulletData(nullptr),
_explosion(nullptr)
{
}

DFExplosion::~DFExplosion()
{
}

bool DFExplosion::init()
{
    if ( !DFObject::init() )
    {
        return false;
    }
    
    updateExplosion();
    
    return true;
}
void DFExplosion::activateObject()
{
    DFObject::activateObject();
    
    if(_battleManager != nullptr)
        _battleManager->addActivateExplosion(this);
    
    _isCollisionActivate = true;
    setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    stopAllActions();
    
    updateExplosion();
    
    auto seq = Sequence::create(DelayTime::create(0.1f), CallFunc::create([=](){
        _isCollisionActivate = false;
    }), nullptr);
    runAction(seq);
}
void DFExplosion::deactivateObject()
{
    if(_battleManager != nullptr)
        _battleManager->removeActivateExplosion(this);
    
    _isCollisionActivate = false;
    
    _damage = 0;
    stopAllActions();
    _explosion->stopAllActions();
    DFObject::deactivateObject();
}
#pragma mark -get set
void DFExplosion::setBulletData(InfoDefenseBullet* data)
{
    _bulletData = data;
}
void DFExplosion::setExplotionDamage(const double value)
{
    _damage = value;
}
#pragma mark -update
void DFExplosion::updateExplosion()
{
    if(_explosion == nullptr)
    {
        _explosion = Sprite::create();
        addChild(_explosion);
    }
    
    _explosion->stopAllActions();
    
    if(_bulletData == nullptr)
        return;
    
    int rand = 0;
    switch(_bulletData->getExplosionType())
    {
        case E_DF_ExplosionType::EXPLOSION_3X3:
        {
            rand = random(1, 2);
        }break;
        case E_DF_ExplosionType::EXPLOSION_4X4:
        {
            
        }break;
        default: rand = random(1, 2); break;
    }
    std::string imgPath = MafUtils::format("Assets/ui/defense/Explosion_effect%2d_00.png", rand);
    
    _explosion->setTexture(imgPath);
    _explosion->getTexture()->setTexParameters(texParams);
    setContentSize(_explosion->getContentSize() * _explosion->getScale());
    _explosion->setPosition(getContentSize()/2);
    
    auto anim = Animation::create();
    anim->setDelayPerUnit(0.05f);
    for(int i = 0; i <= 100; ++i)
    {
        std::string strPath = MafUtils::format("Assets/ui/defense/Explosion_effect%02d_%02d.png", rand, i);
        std::string fullpath = FileUtils::getInstance()->fullPathForFilename(strPath);
        
        if (fullpath.size() == 0)
            break;
        
        auto sprite = Sprite::create(strPath);
        sprite->getTexture()->setTexParameters(texParams);
        anim->addSpriteFrame(sprite->getSpriteFrame());
    }
    
    auto animate = Animate::create(anim);
    auto seq = Sequence::create(animate, CallFunc::create(CC_CALLBACK_0(DFExplosion::deactivateObject, this)), nullptr);
    _explosion->runAction(seq);
}

#pragma mark -override
void DFExplosion::CollisionBegin(IMafColider *Other)
{
    DFObject::CollisionBegin(Other);
    
    if(!_isCollisionActivate)
        return;
    
    auto monster = dynamic_cast<DFMonster*>(Other);
    
    if(monster == nullptr)
        return;
    
    double damage = _damage;
    int hitEffectNumber = _bulletData->getImageNum();
    Vec2 hitLocation = monster->getPosition();
    Vec2 hitSize = Vec2(50, 50);
    
    monster->onHit(damage, hitEffectNumber, hitLocation, hitSize, true);
}

void DFExplosion::CollisionStay(IMafColider *Other)
{
    DFObject::CollisionStay(Other);
}

Rect DFExplosion::GetBoundingBox()
{
    Rect result = getBoundingBox();
    Size size;
    
    switch(_bulletData->getExplosionType())
    {
        case E_DF_ExplosionType::EXPLOSION_3X3:
        {
            size = Size(120);
        }break;
        case E_DF_ExplosionType::EXPLOSION_4X4:
        {
            size = Size(220);
        }break;
        default: break;
    }
    result.size = size;
    return result;
}
