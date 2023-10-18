//
//  PopupStory.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#ifndef PopupStory_h
#define PopupStory_h

#include "GameUIPopup/Base/PopupBase.h"

class PopupStory : public PopupBase
{
public:
    static PopupStory* create(int nIdx);
    
    PopupStory(void);
    virtual ~PopupStory(void);
    virtual bool init(int nIdx);
    
    //
    virtual void show() override;
    
public:
    // set
    void setCallbackFinish(const std::function<void(void)>& callback);
    
protected:
    // init
    void initVar();
    void initUi();
    
    // ui
    void uiContents();
    void drawContents();
    
    //
    void setStoryPage(int movePage);
    
    // click
    void onClickStory(cocos2d::Ref* sender);
    
private:
    int _nIdx;
    int _nPos;
    std::vector<std::string> _listStoryImage;
    std::vector<std::string> _listStoryText;
    
    std::function<void(void)> _callbackFinsh;
    
    // widget
    cocos2d::Layer* _layerContainer;
    cocos2d::Sprite* _spriteStoryImage;
    cocos2d::Sprite* _spriteStoryText;
};

#endif /* PopupStoryList_h */
