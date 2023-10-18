//
//  UILayerRaidParent.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/11/04.
//

#include "UILayerRaidParent.h"

USING_NS_CC;

UILayerRaidParent::UILayerRaidParent(void)
{
    
}

UILayerRaidParent::~UILayerRaidParent(void)
{
    
}

bool UILayerRaidParent::init()
{
    if ( !LayerColor::initWithColor(Color4B(0, 0, 0, 0)) )
    {
        return false;
    }
    
    
    return true;
}
