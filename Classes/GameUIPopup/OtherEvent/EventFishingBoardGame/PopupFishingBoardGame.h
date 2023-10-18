//
//  PopupFishingBoardGame.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/10/15.
//

#ifndef PopupFishingBoardGame_hpp
#define PopupFishingBoardGame_hpp

#include "GameUIPopup/Base/PopupBase.h"

#include "GameUI/UICell/ModelCell.h"
#include "GameUI/UI/UIComboBox.h"

class InfoBoard;
class PopupFishingBoardGame : public PopupBase
{
public:
    static PopupFishingBoardGame* create();
    
    PopupFishingBoardGame(void);
    virtual ~PopupFishingBoardGame(void);
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

#endif /* PopupFishingBoardGame_hpp */
