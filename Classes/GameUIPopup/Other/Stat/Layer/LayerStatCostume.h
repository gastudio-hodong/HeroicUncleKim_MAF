//
//  LayerCostume.h
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 09/01/2020.
//

#ifndef LayerStatCostume_h
#define LayerStatCostume_h

#include "Common/ConfigDefault.h"

class StatCostume : public cocos2d::LayerColor
{
public:
    static StatCostume* create();
    StatCostume(void);
    virtual ~StatCostume(void);
    virtual bool init();
    
protected:
    void initUI();
    void initVar();
    void initCostume();
    
private:
    cocos2d::Layer* _layerContainerCostume;
    cocos2d::Vector<cocos2d::Label*> _lbCurrentValue;
};
#endif /* LayerCostume_h */
