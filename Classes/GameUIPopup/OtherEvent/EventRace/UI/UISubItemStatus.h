//
//  UISubItemStatus.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/07/26.
//

#ifndef UISubItemStatus_hpp
#define UISubItemStatus_hpp

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

#include "GameObject/EventRace/InfoEventRaceStat.h"

class UISubItemStatus : public cocos2d::LayerColor
{
public:
    static UISubItemStatus* create(const cocos2d::Size size, InfoEventRaceStat*  const infoStat);
    
    UISubItemStatus(void);
    virtual ~UISubItemStatus(void);
    virtual bool init(const cocos2d::Size size, InfoEventRaceStat*  const infoStat);
    
protected:
    // init
    void initVar();
    void initUI();
    
    const std::string getIconPath() const;
private:
    InfoEventRaceStat* _infoStat;
};

#endif /* UISubItemStatus_hpp */
