//
//  DialogManager.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/07/19.
//

#include "DialogManager.h"

#include "ManagerGame/UserInfoManager.h"

#include "GameUIPopup/Other/StoryDialog/PopupDialog.h"

#include "ManagerGame/TownManager.h"

USING_NS_CC;

DialogManager::DialogManager(void)
{
    _listInfoDialogScene.clear();
    _listInfoDialogActor.clear();
    _listInfoDialog.clear();
}

DialogManager::~DialogManager(void)
{
    _listInfoDialogScene.clear();
    _listInfoDialogActor.clear();
    _listInfoDialog.clear();
}

bool DialogManager::init()
{
    setLoadAll();
    return true;
}

#pragma mark - get
const Vector<InfoDialogScene*> DialogManager::getListDialogScene(const std::string sceneName) const
{
    Vector<InfoDialogScene*> result;
    result.clear();
    
    for(const auto obj : _listInfoDialogScene)
    {
        if(obj->getSceneName() == sceneName)
        {
            obj->setIsEndMotionComplete(false);
            obj->setIsEndMotionStart(false);
            result.pushBack(obj);
        }
    }
    
    return result;
}

InfoDialogActor* const DialogManager::getInfoActor(const int nId) const
{
    InfoDialogActor* result = nullptr;
    
    for(const auto obj : _listInfoDialogActor)
    {
        if(obj->getID() == nId)
        {
            result = obj;
            break;
        }
    }
    
    return result;
}

InfoDialog* const DialogManager::getInfoDialog(const std::string sceneName) const
{
    InfoDialog* result = nullptr;
    
    for(const auto obj : _listInfoDialog)
    {
        if(obj->getSceneName() == sceneName)
            return obj;
    }
    
    return result;
}
InfoDialog* const DialogManager::getInfoDialogByIdx(const int value) const
{
    InfoDialog* result = nullptr;
    
    for(const auto obj : _listInfoDialog)
    {
        if(obj->getIdx() == value)
            return obj;
    }
    
    return result;
}
const Vector<InfoDialog*> DialogManager::getListDialogByType(const std::string type) const
{
    Vector<InfoDialog*> result;
    result.clear();
    
    for(const auto obj : _listInfoDialog)
    {
        if(obj->getType() == type)
        {
            result.pushBack(obj);
        }
    }
    
    return result;
}
#pragma mark - Load Table Data
void DialogManager::setLoadAll()
{
    setLoadDialogScript();
    setLoadDialogActor();
    setLoadDialogList();
}

void DialogManager::setLoadDialogScript()
{
    std::string strText = TableManager::getInstance()->getTableData(E_TABLE::DIALOG_SCRIPT);
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)strText.c_str());
    if( jsonParser.HasParseError() == true )
    {
        return;
    }
    
    _listInfoDialogScene.clear();
    for ( int i = 0; i < jsonParser.Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser[i];
        
        if ( MafNative::IsLessThanCurrVersion(jsonValue["version"].GetString()) == false )
        {
            continue;
        }
        //Scene
        const int nIdx = jsonValue["idx"].GetInt();
        const std::string strSceneName = jsonValue["scene_name"].GetString();
        const int nSceneIdx = jsonValue["scene_idx"].GetInt();
        //Speaker
        const int speakerID = jsonValue["speaker_id"].GetInt();
        const std::string speakerPosition = jsonValue["speaker_position"].GetString();
        const std::string speakerName = jsonValue["speaker_name_key"].GetString();
        const std::string speakerMotionTypeStart = jsonValue["speaker_motion_type_start"].GetString();
        const std::string speakerMotionTypeEnd = jsonValue["speaker_motion_type_end"].GetString();
        const std::string strText = jsonValue["speaker_text"].GetString();
        //Listner
        const int listenerID = jsonValue["listener_id"].GetInt();
        const std::string listenerPosition = jsonValue["listener_position"].GetString();
        const std::string listenerMotionTypeStart = jsonValue["listener_motion_type_start"].GetString();
        const std::string listenerMotionTypeEnd = jsonValue["listener_motion_type_end"].GetString();
        //Event
        const std::string eventEndType = jsonValue["event_end_type"].GetString();
        const std::string eventEndInfo = jsonValue["event_end_info"].GetString();
        
        InfoDialogScene* obj = InfoDialogScene::create();
        obj->setIdx(nIdx);
        obj->setSceneName(strSceneName);
        obj->setSceneIdx(nSceneIdx);
        obj->setSpeakerID(speakerID);
        obj->setSpeakerNameKey(speakerName);
        obj->setSpeakerPosition(speakerPosition);
        obj->setSpeakerStartMotionType(speakerMotionTypeStart);
        obj->setSpeakerEndMotionType(speakerMotionTypeEnd);
        obj->setListenerID(listenerID);
        obj->setListenerPosition(listenerPosition);
        obj->setListenerStartMotionType(listenerMotionTypeStart);
        obj->setListenerEndMotionType(listenerMotionTypeEnd);
        obj->setEventEndType(eventEndType);
        obj->setEventEndInfo(eventEndInfo);
        
        rapidjson::Document jsonTextList;
        jsonTextList.ParseInsitu<0>((char*)strText.c_str());
        if( jsonTextList.HasParseError() == true )
        {
            continue;
        }
        
        for ( int j = 0; j < jsonTextList.Size(); j++ )
        {
            const rapidjson::Value& jsonTextValue = jsonTextList[j];
            
            obj->addTextKey(jsonTextValue["key"].GetString());
        }
        
        _listInfoDialogScene.pushBack(obj);
    }
}
void DialogManager::setLoadDialogList()
{
    std::string strText = TableManager::getInstance()->getTableData(E_TABLE::DIALOG_LIST);
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)strText.c_str());
    if( jsonParser.HasParseError() == true )
    {
        return;
    }
    
    _listInfoDialog.clear();
    for ( int i = 0; i < jsonParser.Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser[i];
        
        if ( MafNative::IsLessThanCurrVersion(jsonValue["version"].GetString()) == false )
        {
            continue;
        }
        
        //Scene
        const int nIdx = jsonValue["idx"].GetInt();
        const std::string strSceneName = jsonValue["scene_name"].GetString();
        const std::string strType = jsonValue["type"].GetString();
        const std::string strConditionType = jsonValue["condition_type"].GetString();
        const std::string strIconPath = jsonValue["icon_path"].GetString();
        const std::string strNameKey = jsonValue["name_key"].GetString();
        const std::string strLockKey = jsonValue["lock_text_key"].GetString();
        const int nConditionParams = jsonValue["condition_params"].GetInt();
        const bool isRepeat = (bool)jsonValue["is_repeat"].GetInt();
        
        InfoDialog* obj = InfoDialog::create();
        obj->setIdx(nIdx);
        obj->setSceneName(strSceneName);
        obj->setType(strType);
        obj->setConditionType(strConditionType);
        obj->setConditionParams(nConditionParams);
        obj->setListScene(getListDialogScene(obj->getSceneName()));
        obj->setFileNameIcon(strIconPath);
        obj->setTextNameKey(strNameKey);
        obj->setTextLockKey(strLockKey);
        obj->setIsRepeat(isRepeat);
        
        _listInfoDialog.pushBack(obj);
    }
}
void DialogManager::setLoadDialogActor()
{
    std::string strText = TableManager::getInstance()->getTableData(E_TABLE::DIALOG_ACTOR);
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)strText.c_str());
    if( jsonParser.HasParseError() == true )
    {
        return;
    }
    
    _listInfoDialogActor.clear();
    for ( int i = 0; i < jsonParser.Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser[i];
        
        if ( MafNative::IsLessThanCurrVersion(jsonValue["version"].GetString()) == false )
        {
            continue;
        }
        
        int id = jsonValue["id"].GetInt();
        std::string path = jsonValue["image_path"].GetString();
        
        InfoDialogActor* obj = InfoDialogActor::create();
        obj->setID(id);
        obj->setImagePath(path);
        _listInfoDialogActor.pushBack(obj);
    }
}

#pragma mark - cloud
void DialogManager::setCloud(std::string strData)
{
    if ( strData.length() == 0 )
    {
        return;
    }
    
    std::vector<std::string> list = MafUtils::split(strData, ',');
    for ( int i = 0; i < list.size(); i++ )
    {
        int idx = i+1;
        bool bView = std::atoi(list.at(i).c_str());
        
        auto obj = getInfoDialogByIdx(idx);
        if ( obj != nullptr )
        {
            obj->setIsView(bView);
        }
    }
}

std::string DialogManager::getCloud()
{
    std::string strResult = "";
    
    for(const auto obj : _listInfoDialog)
    {
        if ( strResult.length() != 0 )
        {
            strResult.append(",");
        }
        
        bool bOpen = obj->isView();
        if ( bOpen == true )
            strResult.append("1");
        else
            strResult.append("0");
    }
    
    return strResult;
}

#pragma mark - Utils
const int DialogManager::isViewDialog(const std::string sceneName)
{
    int result = 0;
    
    auto objDialog = DialogManager::getInstance()->getInfoDialog(sceneName);
    
    if(objDialog == nullptr)
        result = 2;
    else
        result = objDialog->isView();
    
    return result;
}

void DialogManager::showDialog(const std::string sceneName)
{
    auto objDialog = getInfoDialog(sceneName);
    if(objDialog == nullptr || (objDialog->isView() && !objDialog->isRepeat()))
        return;
    
    objDialog->setIsView(true);
    
    auto popup = PopupDialog::create(sceneName);
    popup->show();
}

FiniteTimeAction* DialogManager::getAction(const std::string strAction, float durection, Vec2 startPos, Vec2 endPos)
{
    FiniteTimeAction* result = nullptr;
    
    if(strAction.compare("FADE_IN") == 0)
    {
        result = FadeIn::create(durection);
    }
    else if(strAction.compare("FADE_OUT") == 0)
    {
        result = FadeOut::create(durection);
    }
    else if(strAction.compare("SLIDE_IN") == 0)
    {
        result = Sequence::create(Place::create(startPos), EaseSineOut::create(MoveTo::create(durection, endPos)), nullptr);
    }
    else if(strAction.compare("SLIDE_OUT") == 0)
    {
        result = Sequence::create(Place::create(startPos), EaseSineOut::create(MoveTo::create(durection, endPos)), nullptr);
    }
    else if(strAction.compare("SHAKE") == 0)
    {
        result = Shake::create(durection, 40.0f, 40.0f);
    }
    
    return result;
}

const bool DialogManager::isDialogOpenException(const int nIdx)
{
    auto objDialog = getInfoDialogByIdx(nIdx);
    
    bool bResult = isViewDialog(objDialog->getSceneName());
    if ( bResult == false )
    {
        if ( objDialog->getConditionType().compare("NORMAL_FLOOR") == 0 )
        {
            int nFloor = UserInfoManager::getInstance()->getHighFloor(E_PLACE::DG_NORMAL);
            if ( objDialog->getConditionParams() < nFloor )
            {
                bResult = true;
            }
        }
        else if ( objDialog->getConditionType().compare("TOWN_SATELLITE_OPEN") == 0 )
        {
            int nTutorial = TownManager::getInstance()->getSatelliteTutorial();
            if ( nTutorial >= 1 )
            {
                bResult = true;
            }
        }
    }
    
    if ( bResult == false )
    {
        
    }
    
    return bResult;
}
