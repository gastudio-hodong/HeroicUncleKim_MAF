//
//  UILayerDefenseObjectSmelting.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2023/08/08.
//

#ifndef UILayerDefenseObjectSmelting_hpp
#define UILayerDefenseObjectSmelting_hpp

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

class UILayerDefenseObjectSmelting : public cocos2d::LayerColor
{
public:
    static UILayerDefenseObjectSmelting* create(cocos2d::Size size, E_DF_Trench_SettingType type, int idx);
    
    UILayerDefenseObjectSmelting(void);
    virtual ~UILayerDefenseObjectSmelting(void);
    virtual bool init(cocos2d::Size size, E_DF_Trench_SettingType type, int idx);
    
public:
    // set, get
    void callbackSmelting(bool bResult, int nResult);
    void onChangeObject(int idx);
    
protected:
    // init
    void initVar();
    void initUi();
    
    // ui
    void uiMain();
    
    void onClickSmelting(Ref* sender);
private:
    E_DF_Trench_SettingType _type;
    int _idx;
    
    
    cocos2d::Label* _lbNowCnt;
    cocos2d::Label* _lbNextCnt;
public:
    MafDelegate<void(void)> _onHide;
};


#endif /* UILayerDefenseObjectSmelting_hpp */
