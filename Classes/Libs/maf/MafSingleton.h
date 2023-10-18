//
//  MafSingleton.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/09/29.
//

#ifndef MafSingleton_hpp
#define MafSingleton_hpp

#include "cocos2d.h"
#include "extensions/cocos-ext.h"

template <typename T>
class MafSingleton : public cocos2d::Ref
{
private:
    static T* _instance;
    static std::mutex _mutexSingleton;
    
protected:
    MafSingleton(void)
    {
        
    }
    virtual ~MafSingleton(void)
    {
        
    }
    virtual bool init()
    {
        return true;
    }
    
public:
    static T* getInstance()
    {
        if (_instance == nullptr)
        {
            //
            std::lock_guard<std::mutex> lock(_mutexSingleton);
            
            _instance = new (std::nothrow) T();
            _instance->init();
        }
        
        return _instance;
    }
    static void destroyInstance()
    {
        //
        std::lock_guard<std::mutex> lock(_mutexSingleton);
        
        if ( _instance != nullptr )
        {
            delete _instance;
            _instance = nullptr;
        }
    }
    
};

template <typename T> std::mutex MafSingleton<T>::_mutexSingleton;
template <typename T> T * MafSingleton<T>::_instance = nullptr;

#endif /* MafSingleton_hpp */
