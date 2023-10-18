//
//  Model.cpp
//  HexagonSnake
//
//  Created by SanHeo on 2014. 7. 27..
//
//

#include "UnitBehavior.h"

USING_NS_CC;

UnitBehavior::UnitBehavior() :
_layerCharacter(nullptr),

_ePlace(E_PLACE::DG_NORMAL),

_chaScale(1)
{
    
}

UnitBehavior::~UnitBehavior()
{
    
}

bool UnitBehavior::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    return true;
}

E_PLACE UnitBehavior::getPlace()
{
    return _ePlace;
}

void UnitBehavior::setPlace(E_PLACE place)
{
    _ePlace = place;
}

Layer* UnitBehavior::getLayerCharacter()
{
    return _layerCharacter;
}
