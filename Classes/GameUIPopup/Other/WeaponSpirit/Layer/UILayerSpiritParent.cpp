//
//  UILayerSpiritParent.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/11/23.
//

#include "UILayerSpiritParent.h"

USING_NS_CC;

UILayerSpiritParent::UILayerSpiritParent(void):
_callbackRedraw(nullptr)
{
    
}

UILayerSpiritParent::~UILayerSpiritParent(void)
{
    
}

bool UILayerSpiritParent::init()
{
    if ( !LayerColor::initWithColor(Color4B(0, 0, 0, 0)) )
    {
        return false;
    }
    
    setIgnoreAnchorPointForPosition(false);
    
    return true;
}

void UILayerSpiritParent::setCallbackRedraw(const std::function<void(void)>& pCallback)
{
    //
    _callbackRedraw = pCallback;
    
}
