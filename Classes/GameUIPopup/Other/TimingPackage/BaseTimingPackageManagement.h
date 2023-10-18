//
//  BaseTimingPackageManagement.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/06/21.
//

#ifndef BaseTimingPackageManagement_hpp
#define BaseTimingPackageManagement_hpp

#include "Common/Observer/IRefresh.h"
#include "Common/Observer/ITimingPackage.h"

#include "GameObject/InfoTimingPackage.h"

#include "GameUIPopup/Other/TimingPackage/UITimingPackageBtn.h"

class BaseTimingPackageManagement : public cocos2d::LayerColor, public ITimingPackage
{
public:
    BaseTimingPackageManagement(void);
    virtual ~BaseTimingPackageManagement(void);
    virtual bool init() override;
    
    virtual void initTimingPackage();
    //timingPackage
    virtual void onNotifyTimingPackge(cocos2d::ValueMap listData) override;
    
    virtual void checkTimingPackage();
    virtual void checkOfflineTimingPackageReward();
    virtual void checkOfflineTimingPackageStart();
    virtual void callbackOfflineTimingPackageStart(const bool bResult, const int nResult);
    
protected:
    virtual void makeupTimingPackageBtnList();
    virtual void resetTimingPackageBtnList();
    virtual void reloadTimingPackage();
    
    void callbackShowTimingPackagePopup(InfoTimingPackage* info);
    
    ///interface
    virtual void setMenu();
    ///interface
    virtual void drawExpand();
    ///interface
    virtual void drawContainer();
    ///interface
    virtual void onExpand();
    
protected:
    cocos2d::Vector<InfoTimingPackage*> _listInfoTimingPackage;
    cocos2d::Vector<UITimingPackageBtn*> _listUITimingbtn;
};


#endif /* BaseTimingPackageManagement_hpp */
