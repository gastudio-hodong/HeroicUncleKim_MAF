//
//  InfoDialogScene.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/07/19.
//

#include "InfoDialogScene.h"

USING_NS_CC;

#pragma mark -
InfoDialogScene::InfoDialogScene():
_isEndMotionStart(false)
,_isSpeakerEndMotionComplete(false)
,_isListenerEndMotionComplete(false)
,_nIdx(0)
,_nSceneIdx(0)
,_nSpeakerID(0)
,_nListenerID(0)
,_strSceneName("")
,_strSpeakerNameKey("")
,_strSpeakerPosition("")
,_strSpeakerStartMotionType("")
,_strSpeakerEndMotionType("")
,_strListenerPosition("")
,_strListenerStartMotionType("")
,_strListenerEndMotionType("")
,_strEventEndType("")
,_strEventEndInfo("")
{
    _listTextKey.clear();
}

InfoDialogScene::~InfoDialogScene()
{
    _listTextKey.clear();
}

bool InfoDialogScene::init()
{
    return true;
}

#pragma mark Scene
const int InfoDialogScene::getIdx() const
{
    return _nIdx;
}
void InfoDialogScene::setIdx(const int value)
{
    _nIdx = value;
}

const std::string InfoDialogScene::getSceneName() const
{
    return _strSceneName;
}
void InfoDialogScene::setSceneName(const std::string value)
{
    _strSceneName = value;
}

const int InfoDialogScene::getSceneIdx() const
{
    return _nSceneIdx;
}
void InfoDialogScene::setSceneIdx(const int value)
{
    _nSceneIdx = value;
}
const bool InfoDialogScene::isEndMotionComplete() const
{
    bool result = false;
    
    if(_nListenerID == 0)
        result = _isSpeakerEndMotionComplete;
    else
        result = _isSpeakerEndMotionComplete && _isListenerEndMotionComplete;
    
    return result;
}
void InfoDialogScene::setIsEndMotionComplete(const bool value)
{
    _isSpeakerEndMotionComplete = value;
    _isListenerEndMotionComplete = value;
}

const bool InfoDialogScene::isEndMotionStart() const
{
    return _isEndMotionStart;
}
void InfoDialogScene::setIsEndMotionStart(const bool value)
{
    _isEndMotionStart = value;
}

#pragma mark Speaker
void InfoDialogScene::setIsSpeakerEndMotionComplete(const bool value)
{
    _isSpeakerEndMotionComplete = value;
}

const int InfoDialogScene::getSpeakerID() const
{
    return _nSpeakerID;
}
void InfoDialogScene::setSpeakerID(const int value)
{
    _nSpeakerID = value;
}

const std::string InfoDialogScene::getSpeakerPosition() const
{
    return _strSpeakerPosition;
}
void InfoDialogScene::setSpeakerPosition(const std::string value)
{
    _strSpeakerPosition = value;
}

const std::string InfoDialogScene::getSpeakerName() const
{
    return GAME_DIALOG_TEXT(_strSpeakerNameKey);
}
void InfoDialogScene::setSpeakerNameKey(const std::string value)
{
    _strSpeakerNameKey = value;
}

const std::string InfoDialogScene::getSpeakerStartMotionType() const
{
    return _strSpeakerStartMotionType;
}
void InfoDialogScene::setSpeakerStartMotionType(const std::string value)
{
    _strSpeakerStartMotionType = value;
}

const std::string InfoDialogScene::getSpeakerEndMotionType() const
{
    return _strSpeakerEndMotionType;
}
void InfoDialogScene::setSpeakerEndMotionType(const std::string value)
{
    _strSpeakerEndMotionType = value;
}

const std::vector<std::string> InfoDialogScene::getListTextKey() const
{
    return _listTextKey;
}
const std::vector<std::string> InfoDialogScene::getListText() const
{
    std::vector<std::string> result;
    result.clear();
    
    for(auto str : _listTextKey)
        result.push_back(GAME_DIALOG_TEXT(str));
    
    return result;
}
void InfoDialogScene::addTextKey(const std::string key)
{
    _listTextKey.push_back(key);
}

#pragma mark Listener
void InfoDialogScene::setIsListenerEndMotionComplete(const bool value)
{
    _isListenerEndMotionComplete = value;
}
const int InfoDialogScene::getListenerID() const
{
    return _nListenerID;
}
void InfoDialogScene::setListenerID(const int value)
{
    _nListenerID = value;
}

const std::string InfoDialogScene::getListenerPosition() const
{
    return _strListenerPosition;
}
void InfoDialogScene::setListenerPosition(const std::string value)
{
    _strListenerPosition = value;
}

const std::string InfoDialogScene::getListenerStartMotionType() const
{
    return _strListenerStartMotionType;
}
void InfoDialogScene::setListenerStartMotionType(const std::string value)
{
    _strListenerStartMotionType = value;
}

const std::string InfoDialogScene::getListenerEndMotionType() const
{
    return _strListenerEndMotionType;
}
void InfoDialogScene::setListenerEndMotionType(const std::string value)
{
    _strListenerEndMotionType = value;
}

#pragma mark Listener
const std::string InfoDialogScene::getEventEndType() const
{
    return _strEventEndType;
}
void InfoDialogScene::setEventEndType(const std::string value)
{
    _strEventEndType = value;
}

const std::string InfoDialogScene::getEventEndInfo() const
{
    return _strEventEndInfo;
}
void InfoDialogScene::setEventEndInfo(const std::string value)
{
    _strEventEndInfo = value;
}
