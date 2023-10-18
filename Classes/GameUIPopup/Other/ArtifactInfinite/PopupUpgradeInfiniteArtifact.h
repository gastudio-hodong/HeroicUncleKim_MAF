//
//  PopupActivateInfiniteArtifact.h
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/10/28.
//

#ifndef PopupUpgradeInfiniteArtifact_h
#define PopupUpgradeInfiniteArtifact_h

#include "GameUIPopup/Base/PopupBase.h"

class PopupFirstUpgradeInfiniteArtifact : public PopupBase
{
public:
    static PopupFirstUpgradeInfiniteArtifact* create();
    PopupFirstUpgradeInfiniteArtifact();
    virtual ~PopupFirstUpgradeInfiniteArtifact();
    virtual bool init();
    
    // init
    void initVar();
    void initUI();
    void initUIContents();
    void onClickButton(Ref* sender);
};

class PopupUpgradeInfiniteArtifact : public PopupBase
{
public:
    static PopupUpgradeInfiniteArtifact* create();
    
    PopupUpgradeInfiniteArtifact(void);
    virtual ~PopupUpgradeInfiniteArtifact(void);
    virtual bool init();
    
    // init
    void initVar();
    void initUI();
    
    void initUIContents();
    
    void onClickButton(Ref* sender);
    
    void callbackBuy(bool nResult);
    
    void setCallbackMoveInfinite(std::function<void(bool)> callback);
    
private:
    
    std::function<void(bool)> _callbackMoveInfinite;
};

#endif /* PopupUpgradeInfiniteArtifact_h */

