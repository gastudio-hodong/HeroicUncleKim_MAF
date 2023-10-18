//
//  PoolManager.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2023/07/07.
//

#ifndef PoolManager_hpp
#define PoolManager_hpp

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

class MafPool : public Node
{
    
public:
    static MafPool* create(E_Poolable_Object_Type type, int poolSize = 100);
    // Sets default values for this actor's properties
    MafPool();
    virtual ~MafPool();

protected:
    // Called when the game starts or when spawned
    virtual bool init(E_Poolable_Object_Type type, int poolSize);
    virtual void onEnter() override;
    
public:
    void createObjectPool(int poolSize);
    
    template <typename T>
    T spawnPooledObject(int createSize = -1)
    {
        Node* result = nullptr;
        
        if(_poolingIngameObject.size() <= 0)
        {
            if(createSize == -1)
                createSize = _defaultPoolSize;
            
            createObjectPool(createSize);
        }
        result = _poolingIngameObject.front();
        _poolingIngameObject.erase(0);
        return dynamic_cast<T>(result);
    }

    void onPooledObjectDespawn(Node* poolActor);

protected:
    
    Node* createPoolableObjectByType();

protected:
    E_Poolable_Object_Type _type;
    int _defaultPoolSize;
    
    Vector<Node*> _poolingIngameObject;
};


#endif /* PoolManager_hpp */
