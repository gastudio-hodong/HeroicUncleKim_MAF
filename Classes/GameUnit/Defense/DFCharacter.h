//
//  DFCharacter.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2023/07/07.
//

#ifndef DFCharacter_hpp
#define DFCharacter_hpp

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

class Grid;
class DFCharacter : public cocos2d::Layer
{
public:
    static DFCharacter* create();
    
    DFCharacter(void);
    virtual ~DFCharacter(void);
    bool init(void) override;
    void update(float dt) override;
    
public:
    void activateObject();
    
    void setGrid(Grid* grid);
    void setNum(int num);
protected:
    //update
    void updateCharacter();
    
protected:
    int _num;
    Grid* _grid;
    Sprite* _character;
    
public:
    //event
    
};

#endif /* DFCharacter_hpp */
