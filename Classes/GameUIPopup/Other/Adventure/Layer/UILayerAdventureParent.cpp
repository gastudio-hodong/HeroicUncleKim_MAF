//
//  UILayerAdventureParent.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/04/15.
//

#include "UILayerAdventureParent.h"

USING_NS_CC;

UILayerAdventureParent::UILayerAdventureParent(void)
{
    
}

UILayerAdventureParent::~UILayerAdventureParent(void)
{
    
}

bool UILayerAdventureParent::init()
{
    if ( !LayerColor::initWithColor(Color4B(0, 0, 0, 0)) )
    {
        return false;
    }
    
    
    return true;
}
