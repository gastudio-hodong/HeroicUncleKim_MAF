//
//  PopupTierRanking.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/12/04.
//

#ifndef PopupTierRanking_h
#define PopupTierRanking_h

#include "GameUIPopup/Base/PopupBase.h"

class RankInfo;
class CellTierRank;
class PopupTierRanking : public PopupBase
{
public:
    static PopupTierRanking* create(E_RANK_LOCAL_TYPE type);
    
    PopupTierRanking(void);
    virtual ~PopupTierRanking(void);
    virtual bool init(E_RANK_LOCAL_TYPE type);
    
    // init
    void initVar();
    void initUI();
    
    void initTopUI();
    void initBottomUI();
    
    virtual void show() override;
    
    //network 이후
    void afterInit();
    void afterInitTopUI();
    void afterInitBottomUI();
    
    
    //callback
    void callbackUserRankInfoInit(bool bResult, std::vector<RankInfo*> vecUserInfo, std::vector<RankInfo*> vecTopRank);
    
    void callbackUpdateAfterInit(bool bResult);
    void callbackNicknameInit();
    
    
    void callbackUserRankInfoUpdate(bool bResult, std::vector<RankInfo*> vecUserInfo, std::vector<RankInfo*> vecTopRank);
    void callbackUpdateAfterUpdate(bool bResult);
    void callbackNicnameUpdate();
    //utils
    void onClick(Ref* sender);
    
    void onClickTier(int idx);
    
    void onClickPage(E_RANK_PAGE page);
    
    RankInfo* getUserRankInfoFromIdx(int idx);
    
private:
    std::vector<RankInfo*> _vecUserInfo;
    std::vector<RankInfo*> _vecTopRankerInfo;
    
    MafNode::MafMenu* _menu;
    Sprite* _sprBG;
    ui::Scale9Sprite* _sprTopCell;
    E_RANK_LOCAL_TYPE _type;
    Layer* _lyRankUser;
    
    MafNode::MafMenuItemSprite* _toggleRankAutoRefresh;
    MafNode::MafMenuItemSprite* _itemTierDropDown;
    
    Sprite* _sprDropDownBg;
    
    Sprite* _sprMyTier;
    Label* _lbMyRank;
    
    Label* _lbNick;
    
    
    std::vector<MafNode::MafMenuItemSprite*> _vecItemTopRank;
    
    std::vector<CellTierRank*> _vecCellRank;
};

class CellTierRank : public Node
{
public:
    
    static CellTierRank* create(E_RANK_LOCAL_TYPE type, RankInfo* info, Size size);
    
    CellTierRank();
    virtual ~CellTierRank();
    
    bool init(E_RANK_LOCAL_TYPE type, RankInfo* info, Size size);
    
    void reDraw();
    
    void hide();
    
    void setRankInfo(RankInfo* info);
    void setItem(MafNode::MafMenuItemSprite* item);
    MafNode::MafMenuItemSprite* getItem();
private:
    RankInfo* _info;
    E_RANK_LOCAL_TYPE _type;
    
    bool _isMyRank;
    Sprite* _sprLuctuation;
    ui::Scale9Sprite* _sprBg;
    Label* _lbRank;
    Label* _lbNick;
    Label* _lbScore;
    
    MafNode::MafMenuItemSprite* _item;
};


#endif /* PopupTierRanking_hpp */
