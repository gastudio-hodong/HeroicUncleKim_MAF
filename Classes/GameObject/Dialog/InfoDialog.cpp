//
//  InfoDialog.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/08/05.
//

#include "InfoDialog.h"

#include "GameObject/Dialog/InfoDialogScene.h"

#include "ManagerGame/DialogManager.h"

USING_NS_CC;

InfoDialog::InfoDialog():
_isRepeat(false)
,_nIdx(0)
,_nConditionParams(0)
,_strSceneName("")
,_strType("")
,_strIconPath("")
,_strConditionType("")
,_strTextNameKey("")
,_strTextLockKey("")
{
    _listScene.clear();
}

InfoDialog::~InfoDialog()
{
    _listScene.clear();
}

bool InfoDialog::init()
{
    return true;
}

const bool InfoDialog::isView() const
{
    return UserDefault::getInstance()->getBoolForKey(MafUtils::format("dl_%d", _nIdx).c_str(), false);
}
void InfoDialog::setIsView(const bool value)
{
    UserDefault::getInstance()->setBoolForKey(MafUtils::format("dl_%d", _nIdx).c_str(), value);
}

const bool InfoDialog::isRepeat() const
{
    return _isRepeat;
}
void InfoDialog::setIsRepeat(const bool value)
{
    _isRepeat = value;
}

const int InfoDialog::getIdx() const
{
    return _nIdx;
}
void InfoDialog::setIdx(const int value)
{
    _nIdx = value;
}

const std::string InfoDialog::getSceneName() const
{
    return _strSceneName;
}
void InfoDialog::setSceneName(const std::string value)
{
    _strSceneName = value;
}

const std::string InfoDialog::getType() const
{
    return _strType;
}
void InfoDialog::setType(const std::string value)
{
    _strType = value;
}

const Vector<InfoDialogScene*> InfoDialog::getListScene() const
{
    return _listScene;
}
void InfoDialog::setListScene(const Vector<InfoDialogScene*> list)
{
    _listScene = list;
}

#pragma mark - condition
const std::string InfoDialog::getConditionType()const
{
    return _strConditionType;
}

void InfoDialog::setConditionType(const std::string strType)
{
    _strConditionType = strType;
}

const int InfoDialog::getConditionParams()const
{
    return _nConditionParams;
}

void InfoDialog::setConditionParams(const int nParams)
{
    _nConditionParams = nParams;
}

const std::string InfoDialog::getFileNameIcon()const
{
    std::string strResult = MafUtils::format("Assets/ui/dialog/icon/%s", _strIconPath.c_str());
    return strResult;
}
void InfoDialog::setFileNameIcon(const std::string value)
{
    _strIconPath = value;
}
const std::string InfoDialog::getTextName()const
{
    std::string strName = GAME_TEXT(_strTextNameKey);
    /* 사용하지 않음
    if (_strConditionType != "NONE" && DialogManager::getInstance()->isDialogOpenException(_nIdx) == false )
    {
        strName = "???";
    }
     */
    
    return strName;
}
void InfoDialog::setTextNameKey(const std::string value)
{
    _strTextNameKey = value;
}
const std::string InfoDialog::getTextLock() const
{
    return GAME_TEXT(_strTextLockKey);
}
void InfoDialog::setTextLockKey(const std::string value)
{
    _strTextLockKey = value;
}
