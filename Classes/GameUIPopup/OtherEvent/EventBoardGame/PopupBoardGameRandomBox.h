//
//  PopupBoardGameRandomBox.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/10/19.
//

#ifndef PopupBoardGameRandomBox_hpp
#define PopupBoardGameRandomBox_hpp

#include "GameUIPopup/Base/PopupBase.h"

#include "GameObject/InfoItems.h"

class CellFishingQuest;
class PopupBoardGameRandomBox : public PopupBase
{

public:
    static PopupBoardGameRandomBox* create(InfoItems* infoItem);
        
    PopupBoardGameRandomBox(void);
    virtual ~PopupBoardGameRandomBox(void);
    virtual bool init(InfoItems* infoItem);
    
public:
    void setCallbackClose(std::function<void(void)> callback);
    
private:
    // init
    void initVar();
    void initUI();

    // ui
    void uiRandBox();
    void uiReward();

    // click
    void onClickClose(cocos2d::Ref* sender);

private:
    
    InfoItems* _infoItem;
    cocos2d::Layer* _layerContents;
    
    std::function<void(void)> _callbackClose;
};

#endif /* PopupBoardGameRandomBox_hpp */
