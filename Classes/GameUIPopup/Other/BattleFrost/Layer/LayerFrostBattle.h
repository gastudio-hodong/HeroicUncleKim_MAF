//
//  LayerFrostBattle.h
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 09/01/2020.
//

#ifndef LayerFrostBattle_h
#define LayerFrostBattle_h

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"
#include "Common/Observer/IRefresh.h"

class UnitBehaviorHeroFrost;
class UnitBehaviorMonsterFrost;
class LayerFrostBattle : public cocos2d::ui::Layout, public IRefresh
{
public:
    enum class E_STATE {
        None,
        Idle,
        Attack,
        MonsterAngry,
        MonsterMoving,
        MonsterAttack
    };
    
public:
    static LayerFrostBattle* create(cocos2d::Size size);

    LayerFrostBattle(void);
    virtual ~LayerFrostBattle(void);
    virtual bool init(cocos2d::Size size);
    virtual void onEnter() override;
    virtual void update(float dt) override;
    
    // refresh
    virtual void refresh(E_REFRESH eType, cocos2d::ValueMap listData) override;
    
public:
    E_STATE getState();
    
    void onDrawInfo();
    
    void onBattleStart(bool bAnimation);
    void onBattleAttack();
    void onBattleMonsterAngry();
    void onBattleMonsterMove();
    void onBattleMonsterAttack();
    void onBattleFinish();
    
    void animationActionCut();
    void animationDoor();
    
protected:
    // init
    void initVar();
    void initUi();
    
    // ui : defalut
    void uiContainer();
    void uiLobby();
    void uiBattle();
    void uiBattleUI();
    
    // ui : draw
    void drawLobby();
    void drawBattleUI();
    
    // ui : set, get
    
    // ui : click
    void onClickAuto(cocos2d::Ref* sender);
    
    // callback
    
    // schedule
    
    //

private:
    bool _bInitEnter;
    
    E_STATE _eState;
 
    std::string _damageTotal;
    
    bool _bAnimation;
    bool _bResult;
    
    
    // unit, map
    cocos2d::Vector<cocos2d::Sprite*> _listMap;
    UnitBehaviorHeroFrost* _unitHero;
    UnitBehaviorMonsterFrost* _unitMonster;
    cocos2d::Sprite* _unitDoor;
    
    // widget
    cocos2d::ui::Layout* _uiContentsLobby;
    
    cocos2d::ui::ScrollView* _uiContentsBattle;
    cocos2d::ui::Layout* _uiContentsBattleUI;
    cocos2d::ui::Layout* _uiContentsBattleEffect;
    
    cocos2d::ui::ImageView* _uiBattleAuto;
    
};
#endif /* LayerFrostBattle_h */
