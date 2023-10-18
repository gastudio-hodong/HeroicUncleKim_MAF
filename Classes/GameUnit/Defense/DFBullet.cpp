//
//  DFBullet.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2023/07/07.
//

#include "DFBullet.h"

#include "Common/ObjectPool/PoolableObjectComponent.h"
#include "GameUnit/Defense/DFBattleManager.h"

#include "GameUnit/Defense/DFMonster.h"
#include "GameUnit/Defense/DFExplosion.h"
#include "GameUnit/Defense/DFEffect.h"

#include "GameObject/Grid/InfoGridTileData.h"
#include "GameObject/Grid/Grid.h"
#include "GameObject/Defense/InfoDefenseBullet.h"
#include "ManagerGame/DefenseManager.h"
USING_NS_CC;

DFBullet* DFBullet::create()
{
    DFBullet* pRet = new(std::nothrow) DFBullet();
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

DFBullet::DFBullet() :
_isCollisionActivate(false),
_speed(0),
_damage(0),
_weaponType(E_DF_WeaponType::NONE),
_targetLocation(Vec2::ZERO),
_data(nullptr),
_bullet(nullptr),
_tailEffect(nullptr),
_target(nullptr)
{
}

DFBullet::~DFBullet()
{
    DefenseManager::getInstance()->_onUpdateFloor.Clear(this);
}

bool DFBullet::init()
{
    if ( !DFObject::init() )
    {
        return false;
    }
    
    DefenseManager::getInstance()->_onUpdateFloor += new MafFunction<void(void)>(CC_CALLBACK_0(DFBullet::deactivateObject, this), this);
    updateBullet();
    
    return true;
}
void DFBullet::update(float dt)
{
    DFObject::update(dt);
    movement(dt);
}
void DFBullet::activateObject()
{
    if(_battleManager != nullptr)
        _battleManager->addActivateBullet(this);
    
    setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    _speed = 400;
    setRotation(355);
    setRotation(0);
    
    if(_data == nullptr)
        return;
    
    stopAllActions();
    
    updateSettingByBulletData();
    updateBullet();
    
    DFObject::activateObject();
}
void DFBullet::deactivateObject()
{
    if(_battleManager != nullptr)
        _battleManager->removeActivateBullet(this);
    
    _onRemoveActiveList.Invoke(this);
    _onRemoveActiveList.Clear();
    _target = nullptr;
    _weaponType = E_DF_WeaponType::NONE;
    
    stopAllActions();
    if(_bullet != nullptr)
        _bullet->stopAllActions();
    if(_tailEffect != nullptr)
        _tailEffect->stopAllActions();
    setRotation(300);
    setRotation(0);
    _targetLocation = Vec2::ZERO;
    _damage = 0;
    
    DFObject::deactivateObject();
    //runAction(RotateTo::create(0.1f, 0));
}
#pragma mark -get set
void DFBullet::setBulletData(InfoDefenseBullet* data)
{
    _data = data;
}
InfoDefenseBullet* DFBullet::getBulletData() const
{
    return _data;
}

void DFBullet::setTargetLocation(Vec2 target)
{
    _targetLocation = target;
}
void DFBullet::setWeaponType(E_DF_WeaponType type)
{
    _weaponType = type;
}
void DFBullet::setBulletDamage(double value)
{
    _damage = value;
}
#pragma mark -update
void DFBullet::updateSettingByBulletData()
{
    _speed = _data->getSpeed();
    if(_data->getCollisionTimer() <= 0)
        _isCollisionActivate = true;
    else
        _isCollisionActivate = false;
    
    switch (_data->getAttackType())
    {
        case E_DF_AttackType::TARGETING:
        case E_DF_AttackType::GUIDED:
            {
                _target = _battleManager->getMonsterByTargetType(_data->getTargetType());
            }
            break;
        case E_DF_AttackType::NONE_TARGETING:
            {
                if(_targetLocation == Vec2::ZERO)
                    _targetLocation = Vec2(750, getPosition().y);
            }
            break;
        default:
            break;
    }
    
}
void DFBullet::updateBullet()
{
    if(_data == nullptr)
        return;
    
    updateBulletImage();
    updateBulletTailEffect();

    
    if(_data->getCollisionTimer() > 0)
    {
        auto action1 = CallFunc::create(CC_CALLBACK_0(DFBullet::startActionCollisionTimer, this));
        runAction(Sequence::create(DelayTime::create(_data->getCollisionTimer()), action1, nullptr));
    }
}
void DFBullet::updateBulletImage()
{
    if(_bullet == nullptr)
    {
        _bullet = Sprite::create();
        _bullet->setScale(3);
        addChild(_bullet);
    }
    
    _bullet->stopAllActions();
    
    std::string imgPath = MafUtils::format("Assets/ui/defense/Defense_bullet_lv%d_00.png", _data->getImageNum());
    
    _bullet->setTexture(imgPath);
    _bullet->getTexture()->setTexParameters(texParams);
    setContentSize(_bullet->getContentSize() * _bullet->getScale());
    _bullet->setPosition(getContentSize()/2);
    
    auto anim = Animation::create();
    anim->setDelayPerUnit(0.05f);
    for(int i = 0; i <= 100; ++i)
    {
        std::string strPath = MafUtils::format("Assets/ui/defense/Defense_bullet_lv%d_%02d.png", _data->getImageNum(), i);
        std::string fullpath = FileUtils::getInstance()->fullPathForFilename(strPath);
        
        if (fullpath.size() == 0)
            break;
        
        auto sprite = Sprite::create(strPath);
        sprite->getTexture()->setTexParameters(texParams);
        anim->addSpriteFrame(sprite->getSpriteFrame());
    }
    
    if(_data->getSpeed() <= 0)
    {
        auto animate = Animate::create(anim);
        auto seq = Sequence::create(animate, CallFunc::create(CC_CALLBACK_0(DFBullet::deactivateObject, this)),nullptr);
        _bullet->runAction(seq);
    }
    else
    {
        auto animate = Animate::create(anim);
        auto repeat = RepeatForever::create(animate);
        _bullet->runAction(repeat);
    }
}

void DFBullet::updateBulletTailEffect()
{
    if(_tailEffect == nullptr)
    {
        _tailEffect = Sprite::create();
        _tailEffect->setScale(3);
        _tailEffect->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        addChild(_tailEffect);
    }
    if(_bullet != nullptr)
        _tailEffect->setPosition(0, _bullet->getContentSize().height * _bullet->getScale() / 2);
    
    _tailEffect->stopAllActions();
    if(_weaponType != E_DF_WeaponType::ROCKET_LAUNCHER)
    {
        _tailEffect->setVisible(false);
        return;
    }
    _tailEffect->setVisible(true);
    
    std::string imgPath = MafUtils::format("Assets/ui/defense/Defense_bullet_tail%02d_%02d.png", 1, 0);
    
    _tailEffect->setTexture(imgPath);
    _tailEffect->getTexture()->setTexParameters(texParams);
    
    auto anim = Animation::create();
    anim->setDelayPerUnit(0.05f);
    for(int i = 0; i <= 100; ++i)
    {
        std::string strPath = MafUtils::format("Assets/ui/defense/Defense_bullet_tail%02d_%02d.png", 1, i);
        std::string fullpath = FileUtils::getInstance()->fullPathForFilename(strPath);
        
        if (fullpath.size() == 0)
            break;
        
        auto sprite = Sprite::create(strPath);
        sprite->getTexture()->setTexParameters(texParams);
        anim->addSpriteFrame(sprite->getSpriteFrame());
    }
    auto animate = Animate::create(anim);
    auto repeat = RepeatForever::create(animate);
    _tailEffect->runAction(repeat);
}

#pragma mark -utill
void DFBullet::startActionCollisionTimer()
{
    if(_data->getCollisionInterval() > 0)
    {
        _isCollisionActivate = true;
        auto seq = Sequence::create(DelayTime::create(_data->getCollisionInterval()), CallFunc::create(CC_CALLBACK_0(DFBullet::startActionCollisionInterval, this)), nullptr);
        runAction(RepeatForever::create(seq));
    }
    else
    {
        _isCollisionActivate = true;
    }
}
void DFBullet::startActionCollisionInterval()
{
    _isCollisionActivate = !_isCollisionActivate;
}

void DFBullet::movement(float dt)
{
    if(getPosition().distance(_targetLocation) <= 20)
    {
        if(_data->getAttackType() == E_DF_AttackType::GUIDED && _data->getExplosionType() != E_DF_ExplosionType::NONE)
        {
            spawnExplosion(_targetLocation);
        }
        
        deactivateObject();
        return;
    }
    if(getPosition().x >= 800 || getPosition().y <= -50 || getPosition().y >= 1400)
    {
        deactivateObject();
        return;
    }
    
    Vec2 targetLocation = _targetLocation;
    
    if(_target && _target->IsActivate())
    {
        targetLocation = _target->getPosition();
    }
    else if(_target && !_target->IsActivate())
    {
        if(_data->getAttackType() == E_DF_AttackType::GUIDED)
        {
            _targetLocation = _target->getPosition();
            _target = nullptr;
            return;
        }
        playRemoveEffect();
        deactivateObject();
        return;
    }
    
    Vec2 dir = MafMath::getSsfeNormal(targetLocation - getPosition());
    
    IntPoint a = IntPoint(dir.x, dir.y);
    Vec2 location = getPosition() + dir * _speed * dt;
    
    //dir = MafMath::getSsfeNormal(getPosition() - targetLocation);
    setPosition(location);
    if(_weaponType == E_DF_WeaponType::LASER)
    {
        setRotation(300);
        setRotation(0);
    }
    else
    {
        setRotation(-dir.getAngle() * (180 / UE_DOUBLE_PI));
    }
}
void DFBullet::onHit(DFMonster* monster)
{
    double damage = _damage;
    //int hitEffectNumber = _data->getImageNum();
    int hitEffectNumber = 1;
    switch(_data->getHitEffectType())
    {
        case E_DF_HitEffectType::SMALL:
        {
            hitEffectNumber = random(1, 2);
        }break;
        case E_DF_HitEffectType::MEDIUM:
        {
            hitEffectNumber = random(3, 4);
        }break;
        case E_DF_HitEffectType::LARGE:
        {
            hitEffectNumber = random(5, 6);
        }break;
        default:
        {
            hitEffectNumber = random(1, 2);
        }break;
    }
    Vec2 hitLocation = getPosition() + Vec2::RIGHT * getContentSize().width;
    Vec2 hitSize = _bullet->getContentSize() * _bullet->getScale();
    
    if(_data->getSpeed() <= 0 )
        hitLocation = Vec2::ZERO;
    
    //===================관통 타입 분류====================
    if(_data->getPierceType() == E_DF_PierceType::NONE)
    {
        //===================공격 타입 분류====================
        switch(_data->getAttackType())
        {
            case E_DF_AttackType::TARGETING:
            {
                //===================타겟 타입 분류====================
                switch(_data->getTargetType())
                {
                    case E_DF_TargetType::NONE:
                    {
                        if(_data->getExplosionType() == E_DF_ExplosionType::NONE)
                        {
                            monster->onHit(damage, hitEffectNumber, hitLocation, hitSize);
                        }
                        else
                        {
                            spawnExplosion(monster->getPosition());
                        }
                        deactivateObject();
                    }break;
                    case E_DF_TargetType::HEAD:
                    case E_DF_TargetType::TAIL:
                    {
                        if(monster == _target)
                        {
                            if(_data->getExplosionType() == E_DF_ExplosionType::NONE)
                            {
                                monster->onHit(damage, hitEffectNumber, hitLocation, hitSize);
                            }
                            else
                            {
                                spawnExplosion(monster->getPosition());
                            }
                            deactivateObject();
                        }
                    }break;
                    default: break;
                }
            }break;
            case E_DF_AttackType::NONE_TARGETING:
            case E_DF_AttackType::GUIDED:
            {
                
                if(_data->getExplosionType() == E_DF_ExplosionType::NONE)
                {
                    monster->onHit(damage, hitEffectNumber, hitLocation, hitSize);
                }
                else
                {
                    spawnExplosion(monster->getPosition());
                }
                deactivateObject();
            }
            default: break;
        }
    }
    else if(_data->getPierceType() == E_DF_PierceType::PIERCING)
    {
        monster->onHit(damage, hitEffectNumber, hitLocation, hitSize);
    }
}
void DFBullet::spawnExplosion(Vec2 location)
{
    auto explosion = _battleManager->spawnExplosion();
    explosion->setBattleManager(_battleManager);
    explosion->setBulletData(_data);
    explosion->setExplotionDamage(_damage);
    explosion->setPosition(location);
    
    if(auto poolComp = (PoolableObjectComponent*)explosion->getComponent("Poolable"))
    {
        poolComp->ActivateObject();
    }
}

void DFBullet::playRemoveEffect()
{
    if(auto removeEffect = _battleManager->spawnEffectObject())
    {
        removeEffect->setPosition(getPosition());
        removeEffect->setPath("Assets/ui/defense/Remove_effect_lv%02d_%02d.png");
        removeEffect->setNum(_data->getImageNum());
        removeEffect->setPlayScale(0.5f);
        removeEffect->setPlaySpeed(2.f);
        
        if(auto poolCompFX = (PoolableObjectComponent*)removeEffect->getComponent("Poolable"))
        {
            poolCompFX->ActivateObject();
        }
    }
}


#pragma mark -override
void DFBullet::CollisionBegin(IMafColider *Other)
{
    DFObject::CollisionBegin(Other);
    
    if(!_isCollisionActivate || _data->getCollisionInterval() > 0)
        return;
    
    auto monster = dynamic_cast<DFMonster*>(Other);
    
    if(monster == nullptr)
        return;
    
    onHit(monster);
}
void DFBullet::CollisionStay(IMafColider *Other)
{
    DFObject::CollisionStay(Other);
    if(!_isCollisionActivate || _data->getCollisionInterval() <= 0)
        return;
    
    auto monster = dynamic_cast<DFMonster*>(Other);
    
    if(monster == nullptr)
        return;
    
    onHit(monster);
//
//    double damage = _damage;
//    int hitEffectNumber = _data->getImageNum();
//    Vec2 hitLocation = getPosition() + Vec2::RIGHT * getContentSize().width;
//    Vec2 hitSize = _bullet->getContentSize() * _bullet->getScale();
//
//    if(_data->getSpeed() <= 0 )
//        hitLocation = Vec2::ZERO;
//
//
//    monster->onHit(damage, hitEffectNumber, hitLocation, hitSize);
}
Rect DFBullet::GetBoundingBox()
{
    Rect result = getBoundingBox();
//    if(_data->getExplosionType() == E_DF_ExplosionType::NONE)
//        result.size = _bullet->getBoundingBox().size * _bullet->getScale();
//    else
//        result.size = _bullet->getBoundingBox().size * _bullet->getScale()/2;
        
    return result;
}
