//
//  DFBattleManager.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2023/07/10.
//

#ifndef DFBattleManager_hpp
#define DFBattleManager_hpp

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

class Grid;
class MafPool;
class DFObject;
class DFMonster;
class DFBullet;
class DFEffect;
class DFExplosion;
class DFHPBar;
class DFMonsterManager;
class DFBattleManager : public cocos2d::Node
{
public:
    static DFBattleManager* create();
    
    DFBattleManager(void);
    virtual ~DFBattleManager(void);
    virtual bool init();
    virtual void update(float dt);
protected:
    //grid
    void generateGrid();
    
    void onGenerateGrid();

    //utills
    void checkCollision();
public:
    void addActivateBullet(DFBullet* bullet);
    void removeActivateBullet(DFObject* bullet);
    
    void addActivateExplosion(DFExplosion* explosion);
    void removeActivateExplosion(DFObject* explosion);
    
    Grid* getGrid();
    DFEffect* spawnEffectObject();
    DFBullet* spawnBullet();
    DFExplosion* spawnExplosion();
    DFMonster* getMonsterByTargetType(E_DF_TargetType type);
    DFHPBar* getHPBar();
    int getActivateMonsterCount();
    int getCurrentStageMonsterLeftCount() const;
    
    void startWave();
    
    //event
    void onUpdateFloor();
    void onUpdateFail();
    
    void addMonsterKillCount();
private:
    int _killCount;
    int _currentFloorMonsterCount;
    
    DFMonsterManager* _monsterManager;
    Grid* _grid;
    MafPool* _bulletPool;
    MafPool* _effectPool;
    MafPool* _explosionPool;
    MafPool* _HPBarPool;
    
    Vector<DFBullet*> _activateBullet;
    Vector<DFExplosion*> _activateExplosition;
public:
    MafDelegate<void(void)> _onKillMonster;
};

#endif /* DFBattleManager_hpp */
