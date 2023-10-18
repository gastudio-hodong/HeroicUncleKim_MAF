//
//  LayerPrisonBattle.h
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 09/01/2020.
//

#ifndef LayerPrisonBattle_h
#define LayerPrisonBattle_h

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"
#include "Common/Observer/IBattleInfo.h"
#include "Common/Observer/IRefresh.h"

class LayerPrisonBattle : public cocos2d::ui::Layout, public IBattleInfo, public IRefresh
{
public:
    static LayerPrisonBattle* create(cocos2d::Size size);

    LayerPrisonBattle(void);
    virtual ~LayerPrisonBattle(void);
    virtual bool init(cocos2d::Size size);
    virtual void onEnter() override;
    
    // refresh
    virtual void refresh(E_REFRESH eType, cocos2d::ValueMap listData) override;
    
    
public:
    void setMinimize(bool bMinimize);
    void onDraw();
    void onBattleStart();
    
    
protected:
    // init
    void initVar();
    void initUi();
    
    // ui : defalut
    void uiLobby();
    void uiLobbyNPC();
    void uiInfo();
    void uiTimer();
    
    // ui : draw
    void drawLobbyNPC(bool bShow);
    void drawInfo();
    void drawTimer();
    void animNPCBeelzebub(bool bIdle);
    void animNPCBaphomet(bool bIdle);
    
    // ui : set, get
    
    // ui : click
    void onTouchBattleLayout(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
    void onClickBattleLayout(cocos2d::Ref* sender);
    void onClickAuto(cocos2d::Ref* sender);
    
    // hero
    void heroCreate();
    void heroRefresh();
    void heroAttack();
    void heroMoveDone();
    void heroUpstairs();
    
    // pet
    void petCreate(E_PET ePet);
    
    // enemy
    void enemyCreate();
    
    // dungeon
    void dungeonStart();
    void dungeonMove();
    void dungeonEnd();
    
    // dungeon map
    void dungeonMapCreate();
    void dungeonMapMove();
    
    // callback
    void callbackDungeonInfo(bool bResult, bool bAvailable, int nType);
    void callbackDungeonResult(bool bResult);
    
    // schedule
    void scheduleBattle(float dt);
    void scheduleMap(float dt);
    
    //
    void onPopupLockShow();
    void onPopupLockHide();
    
public:
    // game
    virtual UnitBehaviorHero* getUnitHero() override;
    virtual UnitBehaviorMonster* getUnitMonster() override;
    virtual cocos2d::Vector<Pet*> getUnitPets() override;

private:
    E_PLACE _ePlace;
    
    bool _bInitEnter;
    
    double _scaleBG;
    
    bool _bLayoutMinimize;
    bool _bLayoutMove;
    cocos2d::Vec2 _posLayoutMovePrev;
    
    double _battleTime;
    bool _battleAuto;
    
    // unit, map
    UnitBehaviorHero* _hero;
    UnitBehaviorMonster* _enemy;
    cocos2d::Vector<Pet*> _listPet;
    cocos2d::Vector<cocos2d::Sprite*> _listMap;
    
    // widget
    cocos2d::ui::Layout* _uiContentsLobby;
    cocos2d::ui::Layout* _uiContentsBattleMap;
    cocos2d::ui::Layout* _uiContentsBattle;
    cocos2d::ui::Layout* _uiContentsInfo;
    cocos2d::ui::Layout* _uiContentsTimer;
    
    cocos2d::Sprite* _spriteDark;
    cocos2d::Sprite* _spriteWhite;
    
    cocos2d::ui::ImageView* _uiNpcBeelzebub;
    cocos2d::ui::ImageView* _uiNpcBaphomet;
    cocos2d::ui::ImageView* _uiNpcGummy;
    cocos2d::ui::ImageView* _uiNpcJelly;
    
    cocos2d::ui::CText* _uiTextTitle;
    cocos2d::ui::CText* _uiTextFloor;
    
    cocos2d::ui::ImageView* _uiImgAuto;
    
    cocos2d::ui::ImageView* _uiImgDungeonTimerBG;
    cocos2d::ui::ImageView* _uiImgDungeonRewardBG;

};
#endif /* LayerPrisonBattle_h */
