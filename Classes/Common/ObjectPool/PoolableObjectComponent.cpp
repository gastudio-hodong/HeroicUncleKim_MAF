//
//  PoolableObjectComponent.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2023/07/07.
//

#include "PoolableObjectComponent.h"

PoolableObjectComponent* PoolableObjectComponent::create()
{
    PoolableObjectComponent* pRet = new(std::nothrow) PoolableObjectComponent();
    if ( pRet && pRet->init() )
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}

// Sets default values for this component's properties
PoolableObjectComponent::PoolableObjectComponent()
{
    _ownerType = E_Poolable_Object_Type::NONE;
    _bActive = false;
    _name = "Poolable";
    // ...
}
PoolableObjectComponent::~PoolableObjectComponent()
{
    _onPooledObjectDespawn.Clear();
    _onPooledObjectActivate.Clear();
    _onPooledObjectDeActivate.Clear();
}


// Called when the game starts
bool PoolableObjectComponent::init()
{
    if(!Component::init())
        return false;
    
    
    return true;
}

void PoolableObjectComponent::SetOwnerType(E_Poolable_Object_Type type)
{
    _ownerType = type;
}

E_Poolable_Object_Type PoolableObjectComponent::GetOwnerType() const
{
    return _ownerType;
}

void PoolableObjectComponent::DeactivateObject()
{
    _onPooledObjectDeActivate.Invoke();
    _onPooledObjectDespawn.Invoke(getOwner());
    SetActiveObject(false);
}

void PoolableObjectComponent::ActivateObject()
{
    SetActiveObject(true);
    _onPooledObjectActivate.Invoke();
}

void PoolableObjectComponent::SetActiveObject(bool IsActive)
{
    _bActive = IsActive;
    getOwner()->setVisible(IsActive);
}

bool PoolableObjectComponent::IsActive()
{
    return _bActive;
}
