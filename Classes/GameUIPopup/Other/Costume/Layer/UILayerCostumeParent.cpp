//
//  UILayerCostumeParent.cpp
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#include "UILayerCostumeParent.h"

USING_NS_CC;

UILayerCostumeParent::UILayerCostumeParent(void)
{
    
}

UILayerCostumeParent::~UILayerCostumeParent(void)
{
    
}

bool UILayerCostumeParent::init()
{
    if ( !LayerColor::initWithColor(Color4B(0, 0, 0, 0)) )
    {
        return false;
    }
    
    
    return true;
}

#pragma mark - ui

#pragma mark - set, get, add

 
