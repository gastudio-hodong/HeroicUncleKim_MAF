//
//  PopupStoryDownload.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#ifndef PopupStoryDownload_h
#define PopupStoryDownload_h

#include "GameUIPopup/Base/PopupBase.h"

class PopupStoryDownload : public PopupBase
{
public:
    static PopupStoryDownload* create(int nIdx);
    
    PopupStoryDownload(void);
    virtual ~PopupStoryDownload(void);
    virtual bool init(int nIdx);
    virtual void onEnter() override;
    
    //
    virtual void show() override;
    
public:
    
protected:
    // init
    void initVar();
    void initUi();
    
    // ui
    void uiContents();
    void uiButton();
    
    void drawProgress();
    void drawButton();
    
    // schedule
    void scheduleHide(float dt);
    
    // click
    void onClickClose(cocos2d::Ref* sender);
    void onClickDownload(cocos2d::Ref* sender);
    
    // callback
    void callbackDownloadStart(int nTotal);
    void callbackDownloadProgress(double nPercent);
    void callbackDownloadCount(int nSuccess, int nError);
    void callbackDownloadEnd();
    
private:
    int _nIdx;
    int _nDownloadTotal;
    int _nDownloadSuccess;
    int _nDownloadError;
    double _nDownloadPercent;
    
    int _nTimeHide;
    int _nTimeHideMax;
    
    // widget
    cocos2d::Layer* _layerContainerContents;
    cocos2d::Layer* _layerContainerButton;
    
    cocos2d::ProgressTimer* _progressStory;
    cocos2d::Label* _labelProgressProgress;
    cocos2d::Label* _labelProgressCount;
    cocos2d::Label* _labelButtonClose;
};

#endif /* PopupStoryDownload_h */
