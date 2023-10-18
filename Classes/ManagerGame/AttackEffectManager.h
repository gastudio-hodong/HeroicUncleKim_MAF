//
//  AttackEffectManager.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/10/20.
//

#ifndef AttackEffectManager_h
#define AttackEffectManager_h

#include "Libs/maf/MafSingleton.h"

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

class SkinAttackEffect
{
private:
    int _idx;
    cocos2d::Animate* _anim;
    cocos2d::Sprite* _sprFront;
    cocos2d::Node* _nowScene;
    std::string strBasePath;
    bool _isActive;
    
public:
    SkinAttackEffect(int idx, cocos2d::Animate* anim, std::string path);
    ~SkinAttackEffect();
    
    void init();
    
    int getIdx();
    cocos2d::Animate* getAnim();
    cocos2d::Sprite* getSprite();
    cocos2d::Node* getNowScene();
    std::string getPath();
    
    bool isActive();
    void setActive(bool active);
    void setSprite(cocos2d::Sprite* base);
    void setNowScene(cocos2d::Node* scene);
};

class AttackEffectManager : public MafSingleton<AttackEffectManager>
{
public:
    AttackEffectManager();
    virtual ~AttackEffectManager();
    virtual bool init();
    
public:
    
    
    std::vector<SkinAttackEffect*> getAllEffect();
    SkinAttackEffect* getAttackEffectForIdx(int idx);
    void addEffect(int idx, cocos2d::Animate* anim, std::string path);
    void addEffect(SkinAttackEffect* effect);
    
    void playSkinEffect(int idx, cocos2d::Vec2 pos);
    bool hasSkinEffect(int idx);
    
    void stopAllEffect();
    void reactivationEffect();
    
private:
    std::vector<SkinAttackEffect*> _vecEffect;
    bool _isActive;
};


#endif /* SkinEffect_hpp */
