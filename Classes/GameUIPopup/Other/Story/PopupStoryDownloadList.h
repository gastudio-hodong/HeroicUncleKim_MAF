//
//  PopupStoryDownloadList.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#ifndef PopupStoryDownloadList_h
#define PopupStoryDownloadList_h

#include "GameUIPopup/Base/PopupBase.h"

class PopupStoryDownloadList : public PopupBase
{
public:
    static PopupStoryDownloadList* create(int nIdx);
    
    PopupStoryDownloadList(void);
    virtual ~PopupStoryDownloadList(void);
    virtual bool init(int nIdx);
    
public:
    // set
    void setCallbackDownload(const std::function<void(int)>& callback);
    
protected:
    // init
    void initVar();
    void initUi();
    
    // ui
    void uiContents();
    void uiButton();
    
    void drawProgress();
    
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
    
    std::function<void(int)> _callbackDownload;
    
    // widget
    cocos2d::Layer* _layerContainerContents;
    cocos2d::Layer* _layerContainerButton;
    
    cocos2d::ProgressTimer* _progressStory;
    cocos2d::Label* _labelProgressProgress;
    cocos2d::Label* _labelProgressCount;
    
};

#endif /* PopupStoryDownloadList_h */
