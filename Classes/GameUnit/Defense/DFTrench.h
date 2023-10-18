//
//  DFTrench.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2023/07/07.
//

#ifndef DFTrench_hpp
#define DFTrench_hpp

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

class Grid;
class DFCharacter;
class DFWeapon;
class DFBattleManager;
class DFTrench : public cocos2d::Layer
{
public:
    static DFTrench* create();
    
    DFTrench(void);
    virtual ~DFTrench(void);
    bool init(void) override;
    void update(float dt) override;
    
public:
    void activateObject();
    
    void setGrid(Grid* grid);
    void setBattleManager(DFBattleManager* battleManager);
    void setNum(int num);
protected:
    void updateCharacter();
    void updateWeapon();
    //update
    void updateTrench();
    
protected:
    int _num;
    Grid* _grid;
    Sprite* _trench;
    Sprite* _trenchFront;
    Sprite* _trenchSandBag;
    DFCharacter* _character;
    DFWeapon* _weapon;
    DFBattleManager* _battleManager;
    
public:
    //event
    
};

#endif /* DFTrench_hpp */
