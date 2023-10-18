//
//  DFBullet.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2023/07/07.
//

#ifndef DFBullet_hpp
#define DFBullet_hpp

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"
#include "GameUnit/Defense/DFObject.h"

class InfoDefenseBullet;
class DFMonster;
class DFBullet : public DFObject
{
public:
    static DFBullet* create();
    
    DFBullet(void);
    virtual ~DFBullet(void);
    bool init(void) override;
    void update(float dt) override;
    
public:
    void activateObject() override;
    void deactivateObject() override;
    
    void setBulletData(InfoDefenseBullet* data);
    InfoDefenseBullet* getBulletData() const;
    void setTargetLocation(Vec2 target);
    void setWeaponType(E_DF_WeaponType type);
    void setBulletDamage(double value);
    
protected:
    //update
    void updateSettingByBulletData();
    void updateBullet();
    void updateBulletImage();
    void updateBulletTailEffect();
    
    void startActionCollisionTimer();
    void startActionCollisionInterval();
    //
    void movement(float dt);
    
    void onHit(DFMonster* monster);
    
    void spawnExplosion(Vec2 location);
    void playRemoveEffect();
public://override
    void CollisionBegin(IMafColider* Other) override;
    void CollisionStay(IMafColider* Other) override;
    Rect GetBoundingBox() override;
    
protected:
    bool _isCollisionActivate;
    double _damage;
    E_DF_WeaponType _weaponType;
    float _speed;
    Vec2 _targetLocation;
    InfoDefenseBullet* _data;
    Sprite* _bullet;
    Sprite* _tailEffect;
    MafPoolableObject* _target;
    
};

#endif /* DFBullet_hpp */
