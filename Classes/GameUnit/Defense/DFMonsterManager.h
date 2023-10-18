//
//  DFMonsterManager.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2023/07/20.
//

#ifndef DFMonsterManager_hpp
#define DFMonsterManager_hpp

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

class Grid;
class DFObject;
class DFMonster;
class MafPool;
class DFBattleManager;
class InfoDefenseFloor;
class DFMonsterManager : public cocos2d::Node
{
public:
    static DFMonsterManager* create();
    
    DFMonsterManager(void);
    virtual ~DFMonsterManager(void);
    virtual bool init();
    
    void activateManager();
    void deactivateObject();
protected:
    //utills
    void addActivateMonster(DFMonster* monster);
    void removeActivateMonster(DFObject* monster);
    void updateMonsterZOrder(float t);
    void createSpwanList(InfoDefenseFloor* data);
public:
    Vector<DFMonster*> getActivateMonsters();
    int getActivateMonsterCount() const;
    void setBattleManager(DFBattleManager* manager);
    void setGrid(Grid* grid);

    DFMonster* spawnMonster();
    E_DF_MonsterType getSpawnType();
    DFMonster* getMonsterByTargetType(E_DF_TargetType type);
    
    void startWave();
    void failWave();
    
private:
    
    DFBattleManager* _battleManager;
    Grid* _grid;
    MafPool* _monsterPool;
    Vector<DFMonster*> _activateMonster;
    std::vector<E_DF_MonsterType> _spwanList;
};

#endif /* DFMonsterManager_hpp */
