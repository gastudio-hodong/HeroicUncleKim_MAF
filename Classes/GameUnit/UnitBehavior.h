//
//  Model.h
//  Tens
//
//  Created by SanHeo on 2014. 10. 11..
//
//

#ifndef UnitBehavior_h
#define UnitBehavior_h

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

class UnitBehavior : public cocos2d::Layer
{
public:
    UnitBehavior(void);
    virtual ~UnitBehavior(void);
    virtual bool init(void) override;
    
public:
    E_PLACE getPlace();
    void setPlace(E_PLACE place);
    
    cocos2d::Layer* getLayerCharacter();
    
protected:
    //
    E_PLACE _ePlace;
    
    cocos2d::Layer *_layerCharacter;
    
    //
    double _chaScale;
};

#endif /* UnitBehavior_h */
