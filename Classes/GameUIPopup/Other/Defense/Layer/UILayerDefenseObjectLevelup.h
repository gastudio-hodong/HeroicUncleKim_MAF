//
//  UILayerDefenseObjectLevelup.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2023/08/07.
//

#ifndef UILayerDefenseObjectLevelup_hpp
#define UILayerDefenseObjectLevelup_hpp

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

class UILayerDefenseObjectLevelup : public cocos2d::LayerColor
{
public:
    static UILayerDefenseObjectLevelup* create(cocos2d::Size size, E_DF_Trench_SettingType type, int idx);
    
    UILayerDefenseObjectLevelup(void);
    virtual ~UILayerDefenseObjectLevelup(void);
    virtual bool init(cocos2d::Size size, E_DF_Trench_SettingType type, int idx);
public:
    // set, get
    
    void callbackLevelup(bool bResult, int nResult);
    void onChangeObject(int idx);
protected:
    // init
    void initVar();
    void initUi();
    
    // ui
    void uiMain();
    
    void onClickLevelUp(Ref* sender);
private:
    E_DF_Trench_SettingType _type;
    int _idx;
    
    cocos2d::Label* _lbNowCnt;
    cocos2d::Label* _lbNextCnt;
    
public:
    MafDelegate<void(void)> _onHide;
};

#endif /* UILayerDefenseObjectLevelup_hpp */
