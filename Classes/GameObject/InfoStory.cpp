//
//  InfoStory.cpp
//  FantasyClicker-mobile
//
//  Created by MAF Games on 15/07/2019.
//

#include "InfoStory.h"

#include "ManagerGame/StoryManager.h"


USING_NS_CC;

InfoStory::InfoStory() :
_nIdx(0)
{
    
}

InfoStory::~InfoStory()
{
    
}

bool InfoStory::init()
{
    return true;
}

int InfoStory::getIdx()
{
    return _nIdx;
}

void InfoStory::setIdx(int nIdx)
{
    _nIdx = nIdx;
}

#pragma mark - condition
std::string InfoStory::getConditionType()
{
    return _strConditionType;
}

void InfoStory::setConditionType(std::string strType)
{
    _strConditionType = strType;
}

int InfoStory::getConditionParams()
{
    return _nConditionParams;
}

void InfoStory::setConditionParams(int nParams)
{
    _nConditionParams = nParams;
}

#pragma mark - file
std::string InfoStory::getFileLanguage()
{
    std::string strResult = "ko";
    
    std::string strLang = TextManager::getInstance()->getLang();
    std::vector<std::string> listLang = MafUtils::split(_strFileLanguage, ',');
    
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

void InfoStory::setFileLanguage(std::string strLang)
{
    _strFileLanguage = strLang;
}

std::string InfoStory::getFileNameIcon()
{
    std::string strResult = MafUtils::format("Assets/story/icon/comic_%02d_on.png", _nIdx);
    return strResult;
}

std::vector<std::string> InfoStory::getFileNameImage()
{
    std::string strName = std::string("Assets/story/image/").append(_strFileName);
    strName.append("%d.jpg");
    
    std::vector<std::string> list;
    for ( int i = 1; i <= _nFileCount; i++ )
    {
        std::string strTemp = MafUtils::format(strName.c_str(), i);
        list.push_back(strTemp);
    }
    
    if ( _nIdx == 67 )
    {
        list.push_back("Assets/story/image/event_2_1_1.png");
        list.push_back("Assets/story/image/event_2_1_2.png");
        list.push_back("Assets/story/image/event_2_1_3.png");
        list.push_back("Assets/story/image/event_2_1_4.png");
        list.push_back("Assets/story/image/event_2_1_5.png");
        list.push_back("Assets/story/image/event_2_1_6.png");
        list.push_back("Assets/story/image/event_2_1_7.png");
        list.push_back("Assets/story/image/event_2_1_8.png");
    }
    
    return list;
}

std::vector<std::string> InfoStory::getFileNameText()
{
    std::string strLang = getFileLanguage();
    std::string strName = std::string("Assets/story/text_%s/").append(_strFileName);
    strName.append("%d.png");
    
    std::vector<std::string> list;
    for ( int i = 1; i <= _nFileCount; i++ )
    {
        std::string strTemp = MafUtils::format(strName.c_str(), strLang.c_str(), i);
        list.push_back(strTemp);
    }
    
    if ( _strFileName.compare("d_10_") == 0 )
    {
        std::string strTemp = std::string("Assets/story/text_%s/d_10_7_1.png");
        strTemp = MafUtils::format(strTemp.c_str(), strLang.c_str());
        
        list.push_back(strTemp);
    }
    
    
    return list;
}

void InfoStory::setFileName(std::string strName)
{
    _strFileName = strName;
}

int InfoStory::getFileCount()
{
    return _nFileCount;
}

void InfoStory::setFileCount(int nCount)
{
    _nFileCount = nCount;
}

#pragma mark -
std::string InfoStory::getTextName()
{
    std::string strName = GAME_TEXT(MafUtils::format("t_ui_story_title_%d", _nIdx));
    if ( _nIdx == (int)E_COMIC::CLEAR_REALENDING && StoryManager::getInstance()->isStoryOpenException(_nIdx) == false )
    {
        strName = "???";
    }
    
    return strName;
}

std::string InfoStory::getTextCondition()
{
    std::string strResult = "";
    if ( _strConditionType.compare("FLOOR") == 0 )
    {
        strResult = GAME_TEXTFORMAT("t_ui_story_clear", _nConditionParams);
    }
    else if ( _strConditionType.compare("FLOOR_DEVIL") == 0 )
    {
        strResult = GAME_TEXTFORMAT("t_ui_story_clear_devil", _nConditionParams);
    }
    else
    {
        strResult = GAME_TEXT(MafUtils::format("t_ui_story_clear_%d", _nIdx));
    }
    
    return strResult;
}
