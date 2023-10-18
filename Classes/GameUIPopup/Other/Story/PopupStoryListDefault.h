//
//  PopupStoryList.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#ifndef PopupStoryList_h
#define PopupStoryList_h

#include "GameUIPopup/Base/PopupBase.h"

class PopupStoryListDefault : public PopupBase
{
    enum class E_TYPE
    {
        NONE = 0,
        
        STORY,
        DIALOG
    };
public:
    static PopupStoryListDefault* create();
    
    PopupStoryListDefault(void);
    virtual ~PopupStoryListDefault(void);
    virtual bool init();
public:
    
protected:
    // init
    void initVar();
    void initUi();
    
    // ui
    void uiTop();
    void uiLoadLayer();
    void uiReload();
    
    // click
    void onClickClose(cocos2d::Ref* sender);
    void onClickTab(Ref* sender);
    
private:
    E_TYPE _eType;
    // widget
    cocos2d::Layer* _lyTop;
    cocos2d::Layer* _lyContentArea;
    cocos2d::Layer* _lyContentStoryList;
    cocos2d::Layer* _lyContentDialogList;
    cocos2d::Layer* _lyContainerNow;
};

#endif /* PopupStoryList_h */
