//
//  DFObject.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2023/07/19.
//

#ifndef DFObject_hpp
#define DFObject_hpp

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"
#include "Common/ObjectPool/MafPoolableObject.h"

class Grid;
class DFBattleManager;
class DFObject : public MafPoolableObject
{
public:
    static DFObject* create();
    
    DFObject(void);
    virtual ~DFObject(void);
    bool init(void) override;
    void update(float dt) override;
public:
    void activateObject() override;
    void deactivateObject() override;
    
    virtual void setBattleManager(DFBattleManager* battleManager);
    
protected:
    
public:
    
protected:
    Grid* _grid;
    DFBattleManager* _battleManager;
public:
    MafDelegate<void(DFObject*)> _onRemoveActiveList;
};

#endif /* DFObject_hpp */
