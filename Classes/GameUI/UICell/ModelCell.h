//
//  ModelCell.h
//  HexagonSnake
//
//  Created by SanHeo on 2014. 8. 7..
//
//

#ifndef __HexagonSnake__ModelCell__
#define __HexagonSnake__ModelCell__

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

class InfoArtifact;
class InfiniteArtifactInfo;
namespace ModelCell {
    
    enum E_CHANGE_STATE{ //C_COUNT_GAME_STATE
        NONE=0,
        LEVEL,
        EARN,
        SECOND,
        ALL,
    };
    
    class UpgradeBtn : public cocos2d::Ref{
        
    public:
        UpgradeBtn();
        virtual ~UpgradeBtn();
        
        void setUpgradeLabelPositionCenter();
        
        void createQuestStyle(cocos2d::Size listSize, cocos2d::Node *parent);
        void createQuestAddBtn(cocos2d::Size listSize, cocos2d::Node *parent);
        
        //style 세분화
        
        MafNode::MafMenuItemSprite *_itemBtn[3];
        cocos2d::Label *_lbMoney[3];
        cocos2d::Label *_lbTop[3];
        cocos2d::Sprite *_sprIcon[3];
        cocos2d::Label *_lbUp[3];
        
        bool _bMakeSecondBtn;
        float _fSecondTime;
        bool _bShowSecondBtn;
        void showSecondUpgradeBtn(bool val);
        bool checkTime(float dt);
        void plusTime();
    };
    
    
    class Cell : public cocos2d::extension::TableViewCell
    {
    protected:
        E_CHANGE_STATE _eState;
        std::function<void()> _onChangeStateListener;
    public:
        
        Cell();
        virtual ~Cell();
        void checkState();
        void setState(E_CHANGE_STATE state);
        void setChangeStateListener(std::function<void()> listener);
        
        std::string _levelEncrypt;

        cocos2d::ui::Scale9Sprite *_listBg;
        cocos2d::Size listSize;
        MafNode::MafMenu *menu;
        
        MafNode::MafMenuItemSprite *_itemStartBtn;
        cocos2d::Sprite *_sprIcon;
        cocos2d::Sprite *_sprUpgrade;
        
        UpgradeBtn *_btnSet;
        MafNode::MafMenuItemSprite *_itemUpgradeBtn;
        cocos2d::Label *_lbUpgrade;
        cocos2d::Label *_lbUpgrade_top;
        cocos2d::Sprite *_sprUpgradeIcon;

        cocos2d::Label *_lbTitle;
        cocos2d::Label* _lbLv;
        cocos2d::Label* _lbExp;
        cocos2d::Label *_lbMoney;
        
        bool _bUpgrade;
        float _fUpgradeTime;
        float _fUpgradeTimeMax;
        float _fRedrawTime;
        float _fRedrawTimeMax;
    };

    
    
    
    class CellArtifactTop : public cocos2d::extension::TableViewCell
    {
    public:
        CellArtifactTop(E_PLACE ePlace);
        virtual ~CellArtifactTop();
        
        void drawInfo();
        void reDrawCell();
        
        void setCallbackTab(const std::function<void(int)>& callback);
        void setCallbackShowRelic(const std::function<void(int)>& callback);
        void onClickTab(cocos2d::Ref* sender);
        int getTabType();
    private:
        
        void onClickInfo(cocos2d::Ref* sender);
        
        E_PLACE _ePlace;
        int _nType;
        std::function<void(int)> _onCallbackTab;
        
        std::function<void(int)> _onCallbackShowRelic;
        
        cocos2d::Sprite* _sprLock;
        
        MafNode::MafMenu* _menu;
    };
    
    class CellArtifact : public Cell
    {
    public:
        
        CellArtifact(E_PLACE ePlace, E_ARTIFACT eArtifact);
        virtual ~CellArtifact();
        virtual void update(float dt);
        
        void setTab(int tab);
        
        void setCellArtifactTop(CellArtifactTop* cellArtifactTop);
        
        void onAskPopup(cocos2d::Ref* pSender);
        void onAskDevilPopup(cocos2d::Ref* pSender);
        void onUpgrade(cocos2d::Ref* pSender);
        void onUpgradePoint(cocos2d::Ref* pSender);
        void onPurhcase(cocos2d::Ref* pSender);
        void onCalculator(cocos2d::Ref* pSender);
        
        void onUpgradeExpand(cocos2d::Ref* pSender);
        void onUpgradeExpandOk(cocos2d::Ref* pSender);
        
        void drawUpgrade();
        void drawUpgradeDefault();
        void drawUpgradeExpand();
        void showArtifactComic();
        
        
        void callbackCalculator();
        void ResultInAppPurchase(MafGooglePlay::RESULT eResult, int nResult, int nIdx, std::string strItems);
        void ResultInAppPurchaseExpand(MafGooglePlay::RESULT eResult, int nResult, int nIdx, std::string strItems);

        void requestArtifactDiscount(int nGeneration);
        void responseArtifactDiscount(cocos2d::network::HttpResponse* response,std::string &data);
        
        E_PLACE _ePlace;
        E_ARTIFACT   _eArtifact;
        InfoArtifact *_artInfo;
        int _nTab;
        
        MafNode::MafMenuItemSprite *_itemUpgradeCashBtn;
        MafNode::MafMenuItemSprite *_itemUpgradePointBtn;
            
        MafNode::MafMenuItemSprite *_itemUpgradeExpandBtn;
        cocos2d::Label* _labelUpgradeCashPrice;
        cocos2d::Label* _labelReinforce;
        cocos2d::Label* _labelCondition;
        
        std::function<void()> _onCompleteListener;
        std::function<void()> _onCompleteReinforceListener;
        
    private:
        CellArtifactTop* _cellArtifactTop;
    };

    class CellUpgradeInfinite : public Cell
    {
    public:
        CellUpgradeInfinite();
        virtual ~CellUpgradeInfinite();
        virtual void update(float dt);
        
        void setTab(int tab);
        void onClick(cocos2d::Ref* sender);
        void drawUpgrade();
        
        void callbackUpgrade(bool bResult);
        void setCallbackMove(std::function<void(int)> callback);
        void setCallbackUpgrade(std::function<void(bool)> callback);
    private:
        int _nTab;
        cocos2d::Label* _lbButtonContent;
        
        std::function<void(int)> _callbackMoveInfinite;
        std::function<void(bool)>_callbackUpgrade;
    };

    class CellInfiniteArtifact : public Cell
    {
    public:
        
        CellInfiniteArtifact(E_PLACE ePlace, INFINITE_ARTIFACT_TYPE type);
        virtual ~CellInfiniteArtifact();
        virtual void update(float dt);
        
        void onClick(cocos2d::Ref* sender);
        
        void drawUpgrade();
        void drawUpgradeDefault();
        
        void callbackCalculator();
        void ResultInAppPurchase(MafGooglePlay::RESULT eResult, int nResult, int nIdx, std::string strItems);
        void onUpgrade(bool onCallback);
        void onUpgradeFromPoint(bool onCallback);
        void onUpgradeFromPurchase(int purIdx);
        void upgradeInfiniteArtifact();
        
        void setCompleteListener(std::function<void()> listener);
        void setCheckUpgradeListener(std::function<void(void)> listener);
        
        
        INFINITE_ARTIFACT_TYPE getType();
        InfiniteArtifactInfo * getInfo();
        
    private:
        E_PLACE _ePlace;
        INFINITE_ARTIFACT_TYPE   _eArtiType;
        InfiniteArtifactInfo * _artiInfo;
        
        MafNode::MafMenuItemSprite *_itemUpgradeCashBtn;
        MafNode::MafMenuItemSprite *_itemUpgradePointBtn;
        cocos2d::Label* _labelUpgradeCashPrice;
        cocos2d::Label* _lbGrade;
        
        float _fRedrawTime;
        float _fRedrawTimeMax;
        std::function<void()> _onCompleteListener;
        
        std::function<void(void)> _checkUpgradeListener;
    };

    class CellRelic : public Cell
    {
    public:

        CellRelic(E_PLACE ePlace, E_RELIC eRelic);
        virtual ~CellRelic();
        virtual void update(float dt);
        
        std::function<void()> _onCompleteListener;
        void onUpgrade(cocos2d::Ref* pSender);
        void onAskPopup(cocos2d::Ref* pSender);
        void onDetailView(cocos2d::Ref* pSender);
        void drawUpgrade();
        
        E_PLACE _ePlace;
        E_RELIC  _eRelic;
        
    };
    
    class CellDungeon : public cocos2d::extension::TableViewCell
    {
    public:
        CellDungeon(E_PLACE ePlace, E_DUNGEON eDungeon);
        virtual ~CellDungeon();
        
        
    public:
        void redrawEventTime();
        int getDungeonTag();
        
    protected:
        void onClickMenu(cocos2d::Ref* sender);
        
    private:
        bool _isSceneMove;
        E_PLACE _ePlace;
        E_DUNGEON _eDungeon;
        
        //
        cocos2d::Label* _lbEventTime;
        MafNode::MafMenuItemSprite* _item;        
    };
}
    

#endif /* defined(__HexagonSnake__ModelCell__) */
