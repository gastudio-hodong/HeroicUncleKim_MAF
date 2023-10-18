//
//  PopupBoardGame.hpp
//  FantasyClicker-mobile
//
//  Created by TG on 2020/07/08.
//

#ifndef PopupBoardGame_h
#define PopupBoardGame_h

#include "GameUIPopup/Base/PopupBase.h"

class InfoBoard;
class PopupBoardGame : public PopupBase
{
public:
    static PopupBoardGame* create();
    
    PopupBoardGame(void);
    virtual ~PopupBoardGame(void);
    virtual void onEnter();
    virtual bool init();
    
public:
    // set
    void setCallbackClose(const std::function<void(void)>& callback);
    
private:
    // init
    void initVar();
    void initUi();
    
    // ui
    void uiBoard();
    void uiInfo();
     
    
    // draw
    void drawInfo();
    void drawBoard();
    void drawBoardCharacter();
    
    
    
    // click
    void onClickClose(Ref* sender);
    void onClickDice(Ref* sender);
    
    // callback
    void callbackInfo(bool bResult, int nResult);
    void callbackDice(bool bResult, int nResult, int nDice, std::string strReward);
    void callbackRandBox();
    
private:
    //
    cocos2d::Vector<InfoBoard*> _listBoard;
    int _nBoardPosition;
    
    //
    int _nResultDice;
    std::string _strResultReward;
    
    //
    std::function<void(void)> _callbackClose;
    
    // widget
    cocos2d::Layer* _layerContainerBoard;
    cocos2d::Layer* _layerContainerInfo;
    
    cocos2d::Label* _labelPoint;
    cocos2d::Label* _labelPearl;
    cocos2d::Label* _labelDicePrice;
    
    cocos2d::Sprite* _spriteCharacter;
    
    
    
};


#endif /* PopupBoardGame_h */
