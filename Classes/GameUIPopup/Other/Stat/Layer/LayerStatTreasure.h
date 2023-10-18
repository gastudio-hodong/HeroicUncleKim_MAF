//
//  LayerStatTreasure.h
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 10/01/2020.
//

#ifndef LayerStatTreasure_h
#define LayerStatTreasure_h

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

class StatTreasure : public cocos2d::LayerColor
{
public:
    static StatTreasure* create(E_PLACE ePlace);
    StatTreasure(void);
    virtual ~StatTreasure(void);
    virtual bool init(E_PLACE ePlace);
    
protected:
    void initUI();
    void initVar();
    void initHighestTreasure();
    void initTreasure();
    void initBottom();
private:
    E_PLACE _ePlace;
    
    cocos2d::Vector<cocos2d::Label*> _lbCurrentValue;
    int _leftPoint;
    
    //
    cocos2d::Layer* _layerContainerTreasure;
};
#endif /* LayerStatTreasure_h */
