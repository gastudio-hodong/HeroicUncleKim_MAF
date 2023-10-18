#ifndef __DEVILDOM_SCENE_H__
#define __DEVILDOM_SCENE_H__

#include "GameScene/Base/BaseGameScene.h"

#include "Common/ConfigEnum.h"
#include "Common/Observer/IRefresh.h"
#include "Common/Observer/IMove.h"
#include "Common/Observer/IBattleInfo.h"
 

#include "GameUI/UICell/ModelCellDevil.h"
#include "GameUIPopup/Other/Pet/PopupPetDevil.h"

class DevilDomScene : public BaseGameScene, public IRefresh, public IMove, public cocos2d::extension::ScrollViewDelegate, public IBattleInfo
{
public:
    enum E_EXPLAIN_CATEGORY
    {
        TEXT,
        IMAGE
    };
    enum E_GAME_STATE{ //C_COUNT_GAME_STATE
        QUEST=0,
        EQUIP,
        ARTIFACT,
        SHOP,
        PET,
        MINE,
        DUNGEON,
    };
    
    cocos2d::Size visibleSize;
    
    E_PLACE _ePlace;
    E_GAME_STATE _eGameState;
    
    
    cocos2d::Size _sizeMapPattern;
    
    
    cocos2d::Vector<Node*> _vecNodeTemp;
    cocos2d::Vector<Node*> _vecMenuTemp;
    cocos2d::Vector<ModelCellDevil::CellDevilQuest*> _vecCellQuest;
    cocos2d::Vector<ModelCellDevil::CellDevilEquip*> _vecCellEquip;
    cocos2d::Vector<ModelCellDevil::CellDevilPet*> _vecCellPet;
    cocos2d::Vector<ModelCell::CellDungeon*> _vecCellDungeon;
    
    cocos2d::Vector<ModelCellDevil::CellDevilArtifact*> _vecCellDevilArtifact[C_COUNT_ARTIFACT_TYPE_SORT];
    cocos2d::Vector<ModelCellDevil::CellDevilArtifactForOriginal*> _vecCellDevilArtifactForOriginal;
    
    ModelCellDevil::CellDevilArtifactTop *_topArtifact;
    ModelCellDevil::CellDevilEquip *_selectEquip;
    ModelCellDevil::CellDevilPet *_selectPet;
    ModelCellDevil::CellDevilQuestTop *_cellQuestTop;
    ModelCellDevil::CellEquipTop *_cellEquipTop;
    
    int _iPrestigeSpecial;
#pragma mark- UI
    
    cocos2d::Vector<cocos2d::Sprite*> _vecMapNode;
    int _nMapIdx;
    int _nMapType;
    
    cocos2d::ui::Scale9Sprite *_sprReceivePopup;
    MafNode::MafMenu *_menu_btn;
    cocos2d::Label *_lbFloor, *_lbWave;
    
    MafNode::MafMenuItemSprite *_itemReinforceEvolution;
    MafNode::MafMenuItemSprite *_itemPetGrade;
    
    cocos2d::extension::ScrollView *_scroll[C_COUNT_GAME_STATE_DEVIL];
    cocos2d::extension::ScrollView *_scrollArtifact;
    
    cocos2d::Sprite *_sprBuff[11]; //9개중 4개만 사용 nullptr
    cocos2d::Label *_lbBuff[11];
    
    cocos2d::LayerColor* _layerSaveWarning;
    PopupPetDevil *_popupPet;
    
#pragma mark-
    
    float _fTimer;
    float _fTimer1Min;
    float _fTimerQuestSave;
    float _fBuffRedrawTimer;
    float _fAdsRedrawTimer;
    

private:
    bool _bInitUI;
    
    UnitBehaviorHero     *_hero;
    UnitBehaviorMonster  *_enemy;
    cocos2d::Vector<Pet*> _vecPet;
    
public:
#pragma mark-
    static DevilDomScene* getInstance();
    virtual bool init() override;
    
    CREATE_FUNC(DevilDomScene);
    DevilDomScene();
    virtual ~DevilDomScene();
    virtual void update(float delta) override;
    virtual void onEnter() override;
    
    // refresh
    virtual void refresh(E_REFRESH eType, cocos2d::ValueMap listData) override;
    
    // move
    virtual void move(E_AREA_MOVE eType) override;
    
    virtual bool onKeyRelease(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)  override;
    
    void onButtonTouched(cocos2d::Ref* pSender);
    
    void setGameState(E_GAME_STATE state);

    void startScene();
    void initUI();
    void initBottomAddUI();
    
    void drawFloor();
    
    void ResultCheckCostumeCoin(cocos2d::network::HttpResponse* response,std::string &data);
    void requestCheckCostumeCoin();//(Label *label);
    
    void createMapIdx();
    void createMap();
    void removeMap();
    void recycleMap();
    void moveMap();
    
    void createHero();
    void createEnemy();
    void createPet(E_PET pet);
    
    void heroAttack();
    void heroMoveDone();
    void upstairsDungeon();
    void startDungeon();
    void moveScreenEffect(bool turnOn);

    void scheduleTimer(float dt);
    void scheduleMove(float dt);
    
    
    void createTable();
    void showTable();
    void removeTable();
    int createArtifactTable(bool redraw = false);
    
    void drawQuestTime();
    void drawEquip();
    void reDrawEquip();
    void reDrawQuest();
    void reDrawArtifact(E_D_ARTIFACT_TYPE type);
    
    void showArtifactTable(E_D_ARTIFACT_TYPE type);//탭 선택 변수 필요
    bool sortArtifactList(cocos2d::Ref *a, cocos2d::Ref *b);
    
    void startBuffTimer(E_SHOP eShop);
    void drawBuffTimer();
    
    void removeNodeTemp();
    
    void onRemovePopupPet(cocos2d::Ref* pSender);
    
    void onSuccessPetGrade();
    void requestPetGradeCostumeCoin();
    void ResultPetGradeCostumeCoin(cocos2d::network::HttpResponse* response,std::string &data);
    

    void onPrestige();
    void reStart();
    
    void showReceiveAdsPopup(E_SHOP_ADS adsType);

    void showSaveWarning();
    void hideSaveWarning();
    void showAutoSaveToast();
    
    
    virtual UnitBehaviorHero* getUnitHero() override;
    virtual UnitBehaviorMonster* getUnitMonster() override;
    virtual cocos2d::Vector<Pet*> getUnitPets() override;
    
    
    bool _bRevivalAds;
    void ResultRevivalAdsCallBack(ADS_RESULT result);
    
    void callbackQuestTab(int tab);
    void callbackEquipTab(int tab);
    void callBackRevive(int nType, bool bAds, bool bFree, int nFloorSub);
private:
    
    cocos2d::ui::Layout* _uiContainerDungeon;
};

#endif // __HELLOWORLD_SCENE_H__
