//
//  ModelCell.h
//  HexagonSnake
//
//  Created by SanHeo on 2017. 11. 1..
//
//

#ifndef __HexagonSnake__ModelCellDevil__
#define __HexagonSnake__ModelCellDevil__

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

#include "GameObject/ModelInfoDevil.h"

#include "GameUI/UICell/ModelCell.h"

namespace ModelCellDevil {
    
    class CellDevilQuestTop : public cocos2d::extension::TableViewCell
    {
    public:
        CellDevilQuestTop(UtilsScene::eType eSceneType = UtilsScene::eType::scene_dungeon_normal);
        virtual ~CellDevilQuestTop();
        
        void update(float dt) override;
        
        void setCallbackTab(const std::function<void(int)>& callback);
        
        void changeTotalGold();
        
    private:
        
        void onClickTab(cocos2d::Ref* sender);
        
        float _fRedrawTime;
        float _fRedrawTimeMax;
        int _nType;
        std::function<void(int)> _onCallbackTab;
        cocos2d::Label* _lbTotalMoney;
        
        MafNode::MafMenuItemSprite *_itemTab0;
        MafNode::MafMenuItemSprite *_itemTab1;
        MafNode::MafMenuItemSprite *_itemTab2;
        MafNode::MafMenuItemSprite *_itemTab3;
    };

    class CellDevilQuest : public ModelCell::Cell
    {
    public:
        
        CellDevilQuest(E_PLACE ePlace, E_QUEST eQuest);
        virtual ~CellDevilQuest();
        virtual void update(float dt);
        
        void start();
        void startResume();
        void drawTimer();
        void onCompleteCallBack(CellDevilQuest *cell);
        
        void setLevelAdd(int level);
        
    public:
        E_PLACE _ePlace;
        E_QUEST _eQuest;
        
        std::string _uQuestEarnMoney;
        cocos2d::ProgressTimer *_bar;
    private:
        bool _bDoubleCoinResume;
        bool _bQuestStart;
        
        std::string _uNextSpend[3];
        
        std::string _uQuestNextEarnMoney;
        
        std::string _uNextSpendShort;
        std::string _uQuestEarnMoneyShort;
        std::string _uQuestNextEarnMoneyShort;
        
        std::string _strLevelAdd;
        
        cocos2d::Sprite *_sprMoney;
        cocos2d::Sprite *_itemStartBtn_disable;
        cocos2d::Sprite *_sprLvUpEffect;
        cocos2d::Label *_lbTime;
        
    protected:
        void drawChangeState();
        void drawUpgrade(ModelCell::E_CHANGE_STATE change);
        void setSpend(ModelCell::E_CHANGE_STATE change);
        
        void onUpgrade(cocos2d::Ref* pSender);
    };
    
    class CellEquipTop : public cocos2d::extension::TableViewCell
    {
    public:
        CellEquipTop();
        virtual ~CellEquipTop();
        
        void setCallbackTab(const std::function<void(int)>& callback);
        
    private:
        
        void onClickTab(cocos2d::Ref* sender);
        
        int _nType;
        std::function<void(int)> _onCallbackTab;
        
        MafNode::MafMenuItemSprite *_itemTab0;
        MafNode::MafMenuItemSprite *_itemTab1;
        MafNode::MafMenuItemSprite *_itemTab2;
        MafNode::MafMenuItemSprite *_itemTab3;
    };

    class CellDevilEquip : public ModelCell::Cell
    {
    public:
        CellDevilEquip(E_PLACE ePlace, E_EQUIP_TYPE eEquipType);
        virtual ~CellDevilEquip();
        virtual void update(float dt);

    public:
        std::function<void(CellDevilEquip*)> _onShowDetailListener;
        std::function<void()> _onCompleteListener;
        
        E_PLACE _ePlace;
        E_EQUIP_TYPE   _eEquipType;
        E_EQUIP_IDX   _eEquipIdx;

        void startSchedule();
        void stopSchedule();
        
        void setLevelAdd(int level);
        
    private:
//        std::string _uNextSpend;
        std::string _uNextSpend[3];
        std::string _uAtk;
        
        std::string _strLevelAdd;
        
    protected:
        
        void onShowDetail(cocos2d::Ref* pSender);
        
        void drawChangeState();
        void onLevelUp(cocos2d::Ref* pSender);
        void drawUpgrade(ModelCell::E_CHANGE_STATE change);
        void setSpend(ModelCell::E_CHANGE_STATE change);
    };

    class CellDevilArtifactTop : public cocos2d::extension::TableViewCell
    {
    public:
        CellDevilArtifactTop();
        virtual ~CellDevilArtifactTop();

        void onClickItem(cocos2d::Ref* pSender);
        std::function<void(E_D_ARTIFACT_TYPE)> _onSelectListener;
        MafNode::MafMenuItemSprite *_item[C_COUNT_ARTIFACT_TYPE_SORT];
        MafNode::MafMenuItemSprite *_itemArtifact;
        E_D_ARTIFACT_TYPE _currentType;
    };
    
    class CellDevilArtifact : public ModelCell::Cell
    {
    public:

        CellDevilArtifact(E_PLACE ePlace, ModelInfoDevil::DevilArtifactInfo &info);
        virtual ~CellDevilArtifact();
        virtual void update(float dt);

        void drawArtifact();
        void onBuyArtifact(cocos2d::Ref* pSender);

        E_PLACE _ePlace;
        ModelInfoDevil::DevilArtifactInfo *_info;
        std::function<void(E_D_ARTIFACT_TYPE)> _onCompleteListener;
    };
    
    class CellDevilArtifactForOriginal : public ModelCell::Cell
    {
    public:
        
        CellDevilArtifactForOriginal(E_PLACE ePlace, E_ARTIFACT eArtifact);
        virtual ~CellDevilArtifactForOriginal();
        virtual void update(float dt);
        
        E_PLACE _ePlace;
        E_ARTIFACT   _eArtifact;
        InfoArtifact *_artInfo;
        
        std::function<void()> _onCompleteListener;
        void onUpgrade(cocos2d::Ref* pSender);
        void drawUpgrade();
        
        MafNode::MafMenuItemSprite *_itemBuyBtn;
        cocos2d::Label* _labelBuyPrice;
        
        void drawBuyBtnVisible();
        void onPurhcase(cocos2d::Ref* pSender);
        void ResultInAppPurchasePackage(MafGooglePlay::RESULT eResult, int nResult, int nIdx, std::string strItems);
        
        MafNode::MafMenuItemSprite *_itemPointBtn;
        void onUpgradePoint(cocos2d::Ref* pSender);
        
        void onCalculator(cocos2d::Ref* pSender);
        void callbackCalculator();
    };
    
    class CellDevilPet : public ModelCell::Cell
    {
    public:
        CellDevilPet(E_PLACE ePlace, E_PET ePet);
        virtual ~CellDevilPet();
        virtual void update(float dt);
        
        std::function<void(ModelCell::Cell*)> _onCompleteListener;
        void onAskPopup(cocos2d::Ref* pSender);
        void onDetailView(cocos2d::Ref* pSender);
        void drawUpgrade();
        
    public:
        E_PLACE _ePlace;
        E_PET  _ePet;
        int _grade;
        
    };
    
}
    

#endif /* defined(__HexagonSnake__ModelCell__) */
