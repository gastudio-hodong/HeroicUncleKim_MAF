//
//  InfoScene.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 14/02/2020.
//

#include "InfoScene.h"

InfoScene::InfoScene() :
_nParams(0),
_strParams("")
{
    
}

InfoScene::~InfoScene()
{
    
}

bool InfoScene::init()
{
    return true;
}

void InfoScene::setParams(int nParams)
{
    _nParams = nParams;
}

int InfoScene::getParams()
{
    return _nParams;
}

void InfoScene::setParamsString(std::string strParams)
{
    _strParams = strParams;
}

std::string InfoScene::getParamsString()
{
    return _strParams;
}
