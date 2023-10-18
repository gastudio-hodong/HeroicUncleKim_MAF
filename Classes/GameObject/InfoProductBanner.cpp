//
//  InfoProductBanner.cpp
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 8. 3..
//

#include "InfoProductBanner.h"

USING_NS_CC;

InfoProductBanner::InfoProductBanner() :
_nItemIdx(0),
_bReceive(false),
_strBGPath("")
{
    
}

InfoProductBanner::~InfoProductBanner()
{
    
}

bool InfoProductBanner::init()
{
    return true;
}

int InfoProductBanner::getItemIdx()
{
    return _nItemIdx;
}

void InfoProductBanner::setItemIdx(int nItemIdx)
{
    _nItemIdx = nItemIdx;
}

bool InfoProductBanner::isReceive()
{
    return _bReceive;
}

void InfoProductBanner::setReceive(bool bReceive)
{
    _bReceive = bReceive;
}

std::string InfoProductBanner::getBGPath()
{
    std::string strLang = getBGLang();
    std::string strPath = "Assets/ui/shop_package/";
    strPath.append(_strBGPath);
    strPath = MafUtils::format(strPath.c_str(), strLang.c_str());
    
    return strPath;
}

void InfoProductBanner::setBGPath(std::string strPath)
{
    _strBGPath = strPath;
}

std::string InfoProductBanner::getBGLang()
{
    std::string strResult = "ko";
    
    std::string strLang = TextManager::getInstance()->getLang();
    std::vector<std::string> listLang = MafUtils::split(_strBGLang, ',');
    
    if ( std::find(listLang.begin(), listLang.end(), strLang) != listLang.end() )
    {
        strResult = strLang;
    }
    else if ( std::find(listLang.begin(), listLang.end(), "en") != listLang.end() )
    {
        strResult = "en";
    }
    
    return strResult;
}

void InfoProductBanner::setBGLang(std::string strLang)
{
    _strBGLang = strLang;
}
