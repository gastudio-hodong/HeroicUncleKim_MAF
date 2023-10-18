//
//  LayerFlashmanBattle.h
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 09/01/2020.
//

#ifndef LayerFlashmanBattle_h
#define LayerFlashmanBattle_h

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"
#include "Common/Observer/IRefresh.h"

#include "GameUIPopup/Other/Flashman/EnumFlashman.h"

class UnitBehaviorHeroFlashman;
class UnitBehaviorMonsterFlashman;
class LayerFlashmanBattle : public cocos2d::ui::Layout, public IRefresh
{
public:
    static LayerFlashmanBattle* create(cocos2d::Size size);

    LayerFlashmanBattle(void);
    virtual ~LayerFlashmanBattle(void);
    virtual bool init(cocos2d::Size size);
    virtual void onEnter() override;
    virtual void update(float dt) override;
    
    // refresh
    virtual void refresh(E_REFRESH eType, cocos2d::ValueMap listData) override;
    
public:
    E_FLASHMAN_STATE getState();
    int getFeverKill();
    int getFeverTimer();
    int getFeverTimerMax();
    
    void onBattleWait();
    void onBattleStart();
    void onBattleBattle();
    void onBattleAttack(E_FLASHMAN_TYPE eType);
    void onBattleFinish();
    
    void createMonster();
    void updateMonsterPosition();
    
    void feverCut();
    void feverEffectShow();
    void feverEffectHide();
    void feverTimerShow();
    void feverTimerUpdate();
    void feverTimerHide();
    
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
    
    // ui : click
    
    // set, get
    int getMonsterKillPerSec();
    
    // callback
    
    // schedule
    void scheduleBattle(float dt);
    
    //

private:
    bool _bInitUI;
    
    //
    E_FLASHMAN_STATE _eState;
    
    TInteger _kill;
    TInteger _killFever;
    TInteger _point;
    TInteger _pointFever;
    
    TInteger _heart;
    TInteger _heartMax;
    
    double _timer;
    double _timerMax;
    double _timerFever;
    double _timerFeverMax;
    double _timerGodMode;

    bool _bFever;
    
    TInteger _monsterCount;
    
    
    //
    std::mutex _mutexBattle;
 
    // unit, map
    UnitBehaviorHeroFlashman* _unitHero;
    cocos2d::Vector<UnitBehaviorMonsterFlashman*> _listMonster;
    std::vector<cocos2d::Vec2> _listMonsterPositions;
    
    
    // widget
    cocos2d::ui::Layout* _uiContentsLobby;
    cocos2d::ui::Layout* _uiContentsBattle;
    cocos2d::ui::Layout* _uiContentsBattleUI;
    cocos2d::ui::Layout* _uiContentsBattleEffectFront;
    cocos2d::ui::Layout* _uiContentsBattleEffectBack;
    
    cocos2d::ui::LoadingBar* _uiBattleTimerBar;
    cocos2d::ui::CText* _uiBattleTimer;
    cocos2d::ui::CText* _uiBattleKillPerSec;
    cocos2d::ui::ImageView* _uiBattleKillPerSecIcon;
    cocos2d::Label* _uiBattlePoint;
    cocos2d::ui::Layout* _uiBattleHeart;
};
#endif /* LayerFlashmanBattle_h */
