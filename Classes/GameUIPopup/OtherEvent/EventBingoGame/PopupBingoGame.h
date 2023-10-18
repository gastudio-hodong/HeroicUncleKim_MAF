//
//  PopupBingoGame.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/07/21.
//

#ifndef PopupBingoGame_h
#define PopupBingoGame_h

#include "GameUIPopup/Base/PopupBase.h"

class InfoItems;
class PopupBingoGame : public PopupBase
{
public:
    static PopupBingoGame* create();
    
    PopupBingoGame(void);
    virtual ~PopupBingoGame(void);
    virtual void onEnter() override;
    virtual bool init() override;
    virtual void update(float dt) override;
    
public:
    // set
    void setCallbackClose(const std::function<void(void)>& callback);
    
private:
    // init
    void initVar();
    void initUi();
    
    // ui
    void uiTop();
    void uiBoard();
    void uiBottom();
    
    void uiBingoLine();
    void uiBoardBlockReward();
    
    // draw
    void uiRedraw();;
    
    //utils
    cocos2d::Sprite* getCrossLineBlock(int nIdx);
    
    // click
    void onClickClose(Ref* sender);
    void onClickLineReward(Ref* sender);
    void onClickQuestReward(Ref* sender);
    void onClickUseBingoTicket(Ref* sender);
    void onClickNextRound();
    void onClickHelp(Ref* sender);
    
    //callback
    void callbackInfo(bool bResult, int nResult);
    void callbackReward(bool bResult, int nResult);
    void callbackNextRound(bool bResult, int nResult);
    void callbackUseTicket(bool bResult, int nResult, int nNun, cocos2d::Vector<InfoItems*> listReward);
    
private:
    
    float _timer;
    //
    std::function<void(void)> _callbackClose;
    
    // widget
    cocos2d::Layer* _lyTopUI;
    cocos2d::Layer* _lyBoard;
    cocos2d::Layer* _lyBottomUI;
    
    cocos2d::Sprite* _sprBlockEffect;
    
    cocos2d::Vector<cocos2d::Sprite*> _listCossLineBlock;
    cocos2d::Vector<cocos2d::Sprite*> _listBoardBlock;
    
};

#endif /* PopupBingoGame_hpp */
