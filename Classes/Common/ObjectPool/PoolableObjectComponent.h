//
//  PoolableObjectComponent.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2023/07/07.
//

#ifndef PoolableObjectComponent_hpp
#define PoolableObjectComponent_hpp

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"


class PoolableObjectComponent : public Component
{
public:
    static PoolableObjectComponent* create();
    
    PoolableObjectComponent();
    virtual~PoolableObjectComponent();
    
protected:
    // Called when the game starts
    virtual bool init() override;

public:
    void SetOwnerType(E_Poolable_Object_Type type);
    E_Poolable_Object_Type GetOwnerType() const;
    void DeactivateObject();
    void ActivateObject();
    
    bool IsActive();

    void SetActiveObject(bool IsActive);
    
protected:
    bool _bActive;
    E_Poolable_Object_Type _ownerType;
public:
    MafDelegate<void(Node*)> _onPooledObjectDespawn;
    MafDelegate<void()> _onPooledObjectActivate;
    MafDelegate<void()> _onPooledObjectDeActivate;
};


#endif /* PoolableObjectComponent_hpp */
