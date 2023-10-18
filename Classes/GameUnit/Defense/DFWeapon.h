//
//  DFWeapon.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2023/07/07.
//

#ifndef DFWeapon_hpp
#define DFWeapon_hpp

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

class Grid;
class DFBattleManager;
class DFBullet;
class InfoDefenseWeapon;
class DFWeapon : public cocos2d::Layer
{
public:
    static DFWeapon* create();
    
    DFWeapon(void);
    virtual ~DFWeapon(void);
    bool init(void) override;
    void update(float dt) override;
    
public:
    void activateObject();
    
    void setGrid(Grid* grid);
    void setBattleManager(DFBattleManager* battleManager);
    void setNum(int num);
    
    void shotBullet();
    void shotBulletByShotGun();
    void playShotAnimation();
    void playMuzzleEffect(DFBullet* bullet);
    void playCartridgeEffect(DFBullet* bullet);
protected:
    //update
    void updateWeapon();
    
protected:
    int _num;
    InfoDefenseWeapon* _weaponData;
    Grid* _grid;
    DFBattleManager* _battleManager;
    Sprite* _weapon;
    Sprite* _muzzleFX;
    
public:
    //event
    
};

#endif /* DFWeapon_hpp */
