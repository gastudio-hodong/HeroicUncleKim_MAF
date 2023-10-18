//
//  PopupRanking.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/11/16.
//

#ifndef PopupRanking_h
#define PopupRanking_h

#include "GameUIPopup/Base/PopupBase.h"

class RankInfo;
class CellRank;
class PopupRanking : public PopupBase
{
public:
    static PopupRanking* create(E_RANK_LOCAL_TYPE type);
    
    PopupRanking(void);
    virtual ~PopupRanking(void);
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
    
private:
    std::vector<RankInfo*> _vecUserInfo;
    std::vector<RankInfo*> _vecTopRankerInfo;
    
    MafNode::MafMenu* _menu;
    Sprite* _sprBG;
    ui::Scale9Sprite* _sprTopCell;
    E_RANK_LOCAL_TYPE _type;
    Layer* _lyRankUser;
    
    MafNode::MafMenuItemSprite* _toggleViewSpQuest;
    
    std::vector<MafNode::MafMenuItemSprite*> _vecItemTopRank;
    
    std::vector<CellRank*> _vecCellRank;
};

class CellRank : public Node
{
public:
    
    static CellRank* create(E_RANK_LOCAL_TYPE type, RankInfo* info, Size size);
    
    CellRank();
    virtual ~CellRank();
    
    bool init(E_RANK_LOCAL_TYPE type, RankInfo* info, Size size);
    
    void reDraw();
    
    void setRankInfo(RankInfo* info);
private:
    RankInfo* _info;
    E_RANK_LOCAL_TYPE _type;
    
    bool _isMyRank;
    Sprite* _sprLuctuation;
    ui::Scale9Sprite* _sprBg;
    Label* _lbRank;
    Label* _lbNick;
    Label* _lbScore;
};

#endif /* PopupRanking_h */
