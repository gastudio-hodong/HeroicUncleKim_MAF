//
//  InfoDefenseBullet.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2023/07/13.
//

#include "InfoDefenseBullet.h"

USING_NS_CC;

InfoDefenseBullet* InfoDefenseBullet::create()
{
    InfoDefenseBullet *pRet = new(std::nothrow) InfoDefenseBullet();
    if (pRet && pRet->init())
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

InfoDefenseBullet::InfoDefenseBullet() :
_index(0),
_imageNum(0),
_bulletValue(0),
speed(0),
collisionTimer(0),
collisionInterval(0),
_attackType(E_DF_AttackType::NONE),
_targetType(E_DF_TargetType::NONE),
_explosionType(E_DF_ExplosionType::NONE),
_muzzleEffectType(E_DF_MuzzleEffectType::NONE),
_pierceType(E_DF_PierceType::NONE),
_projectleType(E_DF_ProjectileType::NONE),
_trajectoryType(E_DF_TrajectoryType::NONE),
_hitEffectType(E_DF_HitEffectType::NONE),
_cartridgeType(E_DF_CartridgeType::NONE)
{
}

InfoDefenseBullet::~InfoDefenseBullet()
{
}

bool InfoDefenseBullet::init()
{
    return true;
}

int InfoDefenseBullet::getIndex() const
{
    return _index;
}

void InfoDefenseBullet::setIndex(const int value)
{
    _index = value;
}

int InfoDefenseBullet::getBulletValue() const
{
    return _bulletValue;
}

void InfoDefenseBullet::setBulletValue(const int value)
{
    _bulletValue = value;
}

int InfoDefenseBullet::getSpeed() const
{
    return speed;
}

void InfoDefenseBullet::setSpeed(const int value)
{
    speed = value;
}

E_DF_AttackType InfoDefenseBullet::getAttackType() const
{
    return _attackType;
}
void InfoDefenseBullet::setAttackType(const E_DF_AttackType value)
{
    _attackType = value;
}

E_DF_TargetType InfoDefenseBullet::getTargetType() const
{
    return _targetType;
}
void InfoDefenseBullet::setTargetType(const E_DF_TargetType value)
{
    _targetType = value;
}

E_DF_ExplosionType InfoDefenseBullet::getExplosionType() const
{
    return _explosionType;
}
void InfoDefenseBullet::setExplosionType(const E_DF_ExplosionType value)
{
    _explosionType = value;
}

E_DF_MuzzleEffectType InfoDefenseBullet::getMuzzleEffectType() const
{
    return _muzzleEffectType;
}
void InfoDefenseBullet::setMuzzleEffectType(const E_DF_MuzzleEffectType value)
{
    _muzzleEffectType = value;
}

E_DF_PierceType InfoDefenseBullet::getPierceType() const
{
    return _pierceType;
}
void InfoDefenseBullet::setPierceType(const E_DF_PierceType value)
{
    _pierceType = value;
}

E_DF_ProjectileType InfoDefenseBullet::getProjectileType() const
{
    return _projectleType;
}
void InfoDefenseBullet::setProjectileType(const E_DF_ProjectileType value)
{
    _projectleType = value;
}

E_DF_TrajectoryType InfoDefenseBullet::getTrajectoryTyp() const
{
    return _trajectoryType;
}
void InfoDefenseBullet::setTrajectoryType(const E_DF_TrajectoryType value)
{
    _trajectoryType = value;
}

E_DF_HitEffectType InfoDefenseBullet::getHitEffectType() const
{
    return _hitEffectType;
}
void InfoDefenseBullet::setHitEffectType(const E_DF_HitEffectType value)
{
    _hitEffectType = value;
}

E_DF_CartridgeType InfoDefenseBullet::getCartridgeType() const
{
    return _cartridgeType;
}
void InfoDefenseBullet::setCartridgeType(const E_DF_CartridgeType value)
{
    _cartridgeType = value;
}

int InfoDefenseBullet::getImageNum() const
{
    return _imageNum;
}
void InfoDefenseBullet::setImageNum(const int value)
{
    _imageNum = value;
}
double InfoDefenseBullet::getCollisionTimer() const
{
    return collisionTimer;
}
void InfoDefenseBullet::setCollisionTimer(const double value)
{
    collisionTimer = value;
}
double InfoDefenseBullet::getCollisionInterval() const
{
    return collisionInterval;
}
void InfoDefenseBullet::setCollisionInterval(const double value)
{
    collisionInterval = value;
}
