//
//  MafPoolableObject.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2023/07/07.
//

#ifndef MafPoolableObject_hpp
#define MafPoolableObject_hpp

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

class PoolableObjectComponent;
class MafPoolableObject : public cocos2d::Layer, public IMafColider
{
public:
    static MafPoolableObject* create();
    
    MafPoolableObject(void);
    virtual ~MafPoolableObject(void);
    
    bool init() override;
    void update(float dt) override;
public:
    virtual void activateObject();
    virtual void deactivateObject();
    
protected:
    
public://override
    Rect GetBoundingBox() override;
    bool IsActivate() override;
    
protected:
    PoolableObjectComponent* _poolComp;
};

#endif /* MafPoolableObject_hpp */
