//
//  InfoDefenseBullet.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2023/07/13.
//

#ifndef InfoDefenseBullet_hpp
#define InfoDefenseBullet_hpp


#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

class InfoDefenseBullet : public cocos2d::Ref
{
public:
    static InfoDefenseBullet* create();
    
    InfoDefenseBullet();
    virtual ~InfoDefenseBullet();
    virtual bool init();
    
public:
    int getIndex() const;
    void setIndex(const int value);
    
    int getBulletValue() const;
    void setBulletValue(const int value);
    
    int getSpeed() const;
    void setSpeed(const int value);
    
    E_DF_AttackType getAttackType() const;
    void setAttackType(const E_DF_AttackType value);
    
    E_DF_TargetType getTargetType() const;
    void setTargetType(const E_DF_TargetType value);
    
    E_DF_ExplosionType getExplosionType() const;
    void setExplosionType(const E_DF_ExplosionType value);
    
    E_DF_MuzzleEffectType getMuzzleEffectType() const;
    void setMuzzleEffectType(const E_DF_MuzzleEffectType value);
    
    E_DF_PierceType getPierceType() const;
    void setPierceType(const E_DF_PierceType value);
    
    E_DF_ProjectileType getProjectileType() const;
    void setProjectileType(const E_DF_ProjectileType value);
    
    E_DF_TrajectoryType getTrajectoryTyp() const;
    void setTrajectoryType(const E_DF_TrajectoryType value);
    
    E_DF_HitEffectType getHitEffectType() const;
    void setHitEffectType(const E_DF_HitEffectType value);
    
    E_DF_CartridgeType getCartridgeType() const;
    void setCartridgeType(const E_DF_CartridgeType value);
    
    int getImageNum() const;
    void setImageNum(const int value);
    
    double getCollisionTimer() const;
    void setCollisionTimer(const double value);
    
    double getCollisionInterval() const;
    void setCollisionInterval(const double value);
private:
    int _index;
    int _imageNum;
    int _bulletValue;
    int speed;
    E_DF_AttackType _attackType;
    E_DF_TargetType _targetType;
    E_DF_ExplosionType _explosionType;
    E_DF_MuzzleEffectType _muzzleEffectType;
    E_DF_PierceType _pierceType;
    E_DF_ProjectileType _projectleType;
    E_DF_TrajectoryType _trajectoryType;
    E_DF_HitEffectType _hitEffectType;
    E_DF_CartridgeType _cartridgeType;
    double collisionTimer;
    double collisionInterval;
};


#endif /* InfoDefenseBullet_hpp */
