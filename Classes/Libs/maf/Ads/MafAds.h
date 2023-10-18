//
//  MafAds.hpp
//  FantasyClicker-mobile
//
//  Created by maf on 16/09/2019.
//

#ifndef __MAF_ADS__
#define __MAF_ADS__

#include "cocos2d.h"
#include "Libs/maf/MafConfig.h"

USING_NS_CC;
class MafAds
{
public:
    virtual void initAds();
    virtual void showAds();
    
    static MafAds* getInstance();
};

#endif /* MAF_ADS */
