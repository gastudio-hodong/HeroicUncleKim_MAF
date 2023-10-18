//
//  DFExplosion.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2023/07/19.
//

#ifndef DFExplosion_hpp
#define DFExplosion_hpp


#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"
#include "GameUnit/Defense/DFObject.h"

class InfoDefenseBullet;
class DFExplosion : public DFObject
{
public:
    static DFExplosion* create();
    
    DFExplosion(void);
    virtual ~DFExplosion(void);
    bool init(void) override;
    
public:
    void activateObject() override;
    void deactivateObject() override;
    
    void setBulletData(InfoDefenseBullet* data);
    void setExplotionDamage(const double value);
protected:
    //update
    void updateExplosion();
    
public://override
    void CollisionBegin(IMafColider* Other) override;
    void CollisionStay(IMafColider* Other) override;
    Rect GetBoundingBox() override;
    
protected:
    bool _isCollisionActivate;
    double _damage;
    
    InfoDefenseBullet* _bulletData;
    Sprite* _explosion;
};

#endif /* DFExplosion_hpp */
