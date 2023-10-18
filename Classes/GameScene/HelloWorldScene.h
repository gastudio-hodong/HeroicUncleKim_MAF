#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "GameScene/Base/BaseGameScene.h"

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"
#include "Common/ConfigGameNormal.h"

#include "Common/Observer/IRefresh.h"
#include "Common/Observer/IMove.h"
#include "Common/Observer/IBattleInfo.h"
#include "Common/Observer/ITimingPackage.h"
#include "Common/Observer/ITutorialRedraw.h"

#include "GameUI/UICell/ModelCell.h"
#include "GameUI/UI/UICurrency.h"

#include "GameUIPopup/Other/TimingPackage/UITimingPackageBtn.h"

class UISettingMenu;
class HelloWorldScene : public BaseGameScene, public IRefresh, public IMove, public cocos2d::extension::ScrollViewDelegate, public ITutorialRedraw, public IBattleInfo
{
public:
    enum E_GAME_STATE{ //C_COUNT_GAME_STATE
        QUEST=0,
        EQUIP,
        RELIC,
        ARTIFACT,
        SHOP,
        PET,
        MINE,
        DUNGEON,
        MYHOME,
        BUILDING,
        ARTIFACT_INFINITE
    };
    
public:
    static HelloWorldScene* getInstance();
    CREATE_FUNC(HelloWorldScene);
    
    HelloWorldScene();
    virtual ~HelloWorldScene();
    virtual bool init() override;
    virtual void update(float delta) override;
    
    // override base
    virtual void onEnter() override;
    virtual bool onKeyRelease(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) override;
    
    // refresh
    virtual void refresh(E_REFRESH eType, cocos2d::ValueMap listData) override;
    
    // move
    virtual void move(E_AREA_MOVE eType) override;
    
    
public:
    
    void setGameState(E_GAME_STATE state);
    void setGameStateToInt(int state);
    
    MafNode::MafMenuItemSprite* getMenu(std::string name);
    MafNode::MafMenuItemSprite* getMenuTopWidget(std::string name);
 
public:
    //
    void onButtonTouched(cocos2d::Ref* pSender);
    void onLockButtonTouched(cocos2d::Ref* pSender);
    

    //
    void startScene();
    void initUI();
    void initTopUI();
    void initBottomUI();
    
    void createTable();
    int createArtifactTable(int type_tab);
    void createInfiniteArtifactTable();
    void showTable();
    
    void drawFloor();
    void drawReviveButton();
    void reDrawArtifact(E_ARTIFACT arti = E_ARTIFACT::E_ARTIFACT_999_NONE);
    void reDrawSpecialQuest();
    
    
    void drawBuffTimer();
    void reDrawBuffTimer();

    void reSetHeroMotion();
    void onPrestige();
    void reStart();
    
    void createHero();
    void createEnemy();
    void createPet(E_PET pet);
    void evolutionPet(E_PET pet);
    void redrawPetCoolTime(bool bReset = false);
    void redrawPetCoolTime(E_PET pet,bool bReset = false);
    
    void createMapIdx();
    void createMap();
    void removeMap();
    void recycleMap();
    void moveMap();
    
    void moveScreenEffect(bool turnOn);
    void startDungeon();
    void heroAttack();
    void showBuffRecommend();
    void onBuffRecommend(cocos2d::Ref* pSender);
    void heroMoveDone();
    void showHeroWeaponUpgrade();
    void upstairsDungeon();
    
    void checkTutorial();
    virtual void redrawTutorialObject() override;

    
    void showFeverTime();
    void hideFeverTIme();
    
    void showItemFeverTime();
    void hideItemFeverTime();
    
    void showSaveWarning();
    void hideSaveWarning();
    
    void showAutoSaveToast();
    
    void showTimeTravel();
    
    float _fTimer;
    float _fTimerQuestSave;
    float _fTimer1sec;
    
    void scheduleMove(float dt);
    void scheduleTimer(float dt);
    void scheduleFever(float dt);
    void scheduleItemFever(float dt);
    
    virtual UnitBehaviorHero* getUnitHero() override;
    virtual UnitBehaviorMonster* getUnitMonster() override;
    virtual cocos2d::Vector<Pet*> getUnitPets() override;
    
    E_PLACE _ePlace;
    E_GAME_STATE _eGameState;
    
    int _iPrestigeSpecial;
    int _selectadPath;
    
    
    cocos2d::Vector<ModelCell::CellArtifact*> _vecCellArtifact;
    cocos2d::Vector<ModelCell::CellInfiniteArtifact*> _vecCellInfiniteArti;
    cocos2d::Vector<ModelCell::CellRelic*> _vecCellRelic;
    cocos2d::Vector<ModelCell::CellDungeon*> _vecCellDungeon;
    
    cocos2d::Size _sizeMapPattern;
    cocos2d::extension::ScrollView *_scroll[C_COUNT_GAME_STATE];

    
    cocos2d::Vector<cocos2d::Sprite*> _vecMapNode;
    int _nMapIdx;
    int _nMapType;
    
    //fold mission UI
    cocos2d::ui::Button *_itemBuffNewbie;
    MafNode::MafMenuItemSprite *_itemSpecialMission;
    MafNode::MafMenuItemSprite *_itemPurchaseBanner;
    MafNode::MafMenuItemSprite *_itemMission;
    MafNode::MafMenuItemSprite *_itemNewYear2020;
    MafNode::MafMenuItemSprite *_itemPurchaseFirst;
    MafNode::MafMenuItemSprite *_itemBuffRecommend;
    cocos2d::ui::Button* _itemCostumeLock;
    cocos2d::ui::Button* _itemDungeonPass;
    
    cocos2d::ui::Scale9Sprite *_sprDarkScreen;
    cocos2d::ui::Scale9Sprite *_sprReceivePopup;
    MafNode::MafMenu *_menu_btn;
    MafNode::MafMenu *_menu_TopWidget;
    
    cocos2d::Layer* _layerTopWidget;
    cocos2d::Layer* _layerTableWidget;
    cocos2d::LayerColor* _layerFever;
    cocos2d::Layer* _layerItemFever;
    cocos2d::LayerColor* _layerSaveWarning;
    cocos2d::LayerColor* _layerTimeTravel;
    cocos2d::LayerColor* _layerDungeonInfo;
    
    ModelCell::CellArtifactTop *_cellArtifacTop;
    cocos2d::ui::Layout* _uiContainerPet;
    cocos2d::ui::Layout* _uiContainerQuest;
    cocos2d::ui::Layout* _uiContainerWeapon;
    cocos2d::ui::Layout* _uiContainerDungeon;
    
    
    UISettingMenu* _uiSettingMenu;

    int _nTimeTravelTime;
    
    void timeSchedule(float dt);
    void ResultRevivalAdsCallBack(ADS_RESULT result);
    bool _bRevivalAds;
    bool _bRevivalFree;
    int _nRevivalFloorSub;
    
     
    void CompleteSpecialMission(bool complete);
    void CheckServerMission();
    
    // callback
    void callbackArtifactTab(int tab);
    void callbackArtifactReinforce();
    void callbackRevive(int nType, bool bAds, bool bFree, int nFloorSub);
    void callbackRecommendCostume(E_COSTUME eType, E_COSTUME_IDX eIdx);
    void callbackBuffRecommendAds(ADS_RESULT result);
    
    void callbackUpgradeInfiniteArtifact(bool bFirstUpgrade = false);
    void callbackCheckInfiniteUpgrade();
    
    void callbackTimeTravel();
    void callbackTimeTravelCalculation();
    
    void onClickoRevive();
    void onClickDefenseBackground(Ref* sender);
    
    void showRecommendCostume(E_COSTUME eType,E_COSTUME_IDX eIdx);
    
private:
    bool _bInitUI;
    
    //
    UnitBehaviorHero     *_hero;
    UnitBehaviorMonster  *_enemy;
    cocos2d::Vector<Pet*> _vecPet;
    
public:
    //버튼, 코스튬 정보, 시간
    struct tagRecommendCostumeData
    {
        MafNode::MafMenuItemSprite* _button;
        E_COSTUME _eType;
        E_COSTUME_IDX _eIdx;
        float _timer;
        
        tagRecommendCostumeData(MafNode::MafMenuItemSprite *button, E_COSTUME eType, E_COSTUME_IDX eIdx, float timer)
        : _button(button),
        _eType(eType),
        _eIdx(eIdx),
        _timer(timer)
        {}
    };
    std::vector<tagRecommendCostumeData> _vecRecommendCostumes;
public:
    enum E_STEPWISE_TYPE
    {//열리는 조건 타입
        FLOOR,
        SPECIAL_MISSION,
        ETC,
        INIT
        
    };
    void stepwiseContentsOpen(bool init = true, E_STEPWISE_TYPE type = E_STEPWISE_TYPE::INIT);
};

#endif // __HELLOWORLD_SCENE_H__
