//
//  LayerEventSunfishGame.h
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 09/01/2020.
//

#ifndef LayerEventSunfishGame_h
#define LayerEventSunfishGame_h

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

class LayerEventSunfishGame : public cocos2d::ui::Layout
{
public:
    CREATE_FUNC(LayerEventSunfishGame);

    LayerEventSunfishGame(void);
    virtual ~LayerEventSunfishGame(void);
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void update(float dt) override;
    
    // override touch
    virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unusedEvent) override;
    virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unusedEvent) override;

    
public:
    void onStart();
    void onLevelUP();
    void onDie();
    void onWeightAdd(double weightAdd);
    void onAttack(int idx);
    void onFeverTime();
    void onFeverTimeActive();
    void onFeverTimeActiveFinish();
    
protected:
    // init
    void initVar();
    void initUi();
    
    // ui
    
    // ui : draw
    
    // ui : action
    
    // ui : click
    
    // callback
    
    // schedule
    
    // event
    void heroSpawn(int levelIdx);
    void heroLevelUp(int levelIdx);
    void heroMoveAction();
    void heroAttack(int enemyIdx);
    void heroWeightAdd(double weightAdd);
    void enemySpawn(int enemyIdx);
    void enemyHit(cocos2d::Sprite* enemy);
    void enemySave();
    void feverSpawn();
    
private:
    bool _bInitUI;
    
    //
    bool _bGameStart;
    
    int _enemyAttackCount;
    
    double _enemySpawnTime;
    double _touchTime;
    double _feverActiveTime;
    
    // unit
    cocos2d::ui::Layout* _uiContentsBattle;
    
    cocos2d::Sprite* _heroUnit;
    cocos2d::Sprite* _feverUnit;
    cocos2d::Vector<cocos2d::Sprite*> _listEnemyUnit;
    
    cocos2d::ParticleSystemQuad* _bubbleParticle;
    cocos2d::ParticleSystemQuad* _bubbleParticleFever;

};
#endif /* LayerEventSunfishGame_h */
