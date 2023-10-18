//
//  StoryManager.cpp
//  FantasyClicker-mobile
//
//  Created by MAF Games on 19/09/2019.
//

#include "StoryManager.h"

#include "GameUIPopup/Other/Story/PopupStory.h"
#include "GameUIPopup/Other/Story/PopupStoryDownload.h"

#include "ManagerGame/UserInfoManager.h"
#include "ManagerGame/DataManager.h"

USING_NS_CC;

StoryManager::StoryManager(void) :
_nIdxMax(0),

_nDownloadTotal(0),
_nDownloadSuccess(0),
_nDownloadError(0),

_callbackDownloadStart(nullptr),
_callbackDownloadProgress(nullptr),
_callbackDownloadCount(nullptr),
_callbackDownloadEnd(nullptr)
{
    
}

StoryManager::~StoryManager(void)
{
    
}

bool StoryManager::init()
{
    _downloader.reset(new network::Downloader());
    _downloader->onTaskProgress = std::bind(&StoryManager::downloadTaskProgress, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
    _downloader->onFileTaskSuccess = std::bind(&StoryManager::downloadFileSuccess, this, std::placeholders::_1);
    _downloader->onTaskError = std::bind(&StoryManager::downloadTaskError, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
    
    // load
    setLoad();
    
    return true;
}

#pragma mark - load
void StoryManager::setLoad()
{
    std::string strText = TableManager::getInstance()->getTableData(E_TABLE::STORY);
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)strText.c_str());
    if( jsonParser.HasParseError() )
    {
        return;
    }
    
    _listStory.clear();
    for ( int i = 0; i < jsonParser.Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser[i];
        
        int nIdx = jsonValue["idx"].GetInt();
        std::string strConditionType = jsonValue["condition_type"].GetString();
        int nConditionParams = jsonValue["condition_params"].GetInt();
        
        std::string strFileLanguage = jsonValue["file_language"].GetString();
        std::string strFileName = jsonValue["file_name"].GetString();
        int nFileCount = jsonValue["file_count"].GetInt();
        
        // obj
        auto obj = InfoStory::create();
        obj->setIdx(nIdx);
        obj->setConditionType(strConditionType);
        obj->setConditionParams(nConditionParams);
        obj->setFileLanguage(strFileLanguage);
        obj->setFileName(strFileName);
        obj->setFileCount(nFileCount);
        
        _listStory.pushBack(obj);
        
        //
        if ( nIdx > _nIdxMax )
        {
            _nIdxMax = nIdx;
        }
    }
}

#pragma mark - cloud
void StoryManager::setCloudStory(std::string strData)
{
    if ( strData.length() == 0 )
    {
        return;
    }
    
    std::vector<std::string> list = MafUtils::split(strData, ',');
    for ( int i = 0; i < list.size(); i++ )
    {
        setStoryOpen(i+1, std::atoi(list.at(i).c_str()));
    }
}

std::string StoryManager::getCloudStory()
{
    std::string strResult = "";
    for ( int i = 1; i <= _nIdxMax; i++ )
    {
        if ( strResult.length() != 0 )
        {
            strResult.append(",");
        }
        
        bool bOpen = isStoryOpen(i);
        if ( bOpen == true )
            strResult.append("1");
        else
            strResult.append("0");
    }

    return strResult;
}

#pragma mark - get, set, other
Vector<InfoStory*> StoryManager::getStoryAll()
{
    return _listStory;
}

InfoStory* StoryManager::getStory(int nIdx)
{
    InfoStory *pResult = nullptr;
    
    for ( int i = 0; i < _listStory.size(); i++ )
    {
        auto obj = _listStory.at(i);
        
        if ( obj->getIdx() == nIdx )
        {
            pResult = obj;
            break;
        }
    }
    
    return pResult;
}

bool StoryManager::isStoryOpen(int nIdx)
{
    bool bResult = false;
    
    std::string strKey = MafUtils::format("cm_%d", nIdx);
    
    bResult = UserDefault::getInstance()->getBoolForKey(strKey.c_str(), false);
    return bResult;
}

bool StoryManager::isStoryOpenException(int nIdx)
{
    bool bResult = isStoryOpen(nIdx);
    if ( bResult == false )
    {
        auto objStory = StoryManager::getInstance()->getStory(nIdx);
        if ( objStory->getConditionType().compare("FLOOR") == 0 )
        {
            int nFloor = UserInfoManager::getInstance()->getHighFloor(E_PLACE::DG_NORMAL);
            if ( objStory->getConditionParams() < nFloor )
            {
                bResult = true;
            }
        }
        else if ( objStory->getConditionType().compare("FLOOR_DEVIL") == 0 )
        {
            int nFloor = UserInfoManager::getInstance()->getHighFloor(E_PLACE::DG_DEVIL_DOM);
            if ( objStory->getConditionParams() < nFloor )
            {
                bResult = true;
            }
        }
    }
    
    if ( bResult == false )
    {
        switch ((E_COMIC)nIdx) {
            case E_COMIC::ARTIFACT_RING:
                if ( UserInfoManager::getInstance()->getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_1_EVOL1) >= 25 )
                    bResult = true;
                break;
            case E_COMIC::ARTIFACT_STORMY:
                if ( UserInfoManager::getInstance()->getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_3_EVOL1) >= 50 )
                    bResult = true;
                break;
            case E_COMIC::ARTIFACT_EAGLE:
                if ( UserInfoManager::getInstance()->getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_4_EVOL1) >= 50 )
                    bResult = true;
                break;
            
                
            case E_COMIC::MAX_ARTIFACT_1:
            {
                auto objArtifact = DataManager::GetArtifactInfo(E_ARTIFACT::ARTI_1_EVOL1);
                if ( UserInfoManager::getInstance()->getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_1_EVOL1) >= objArtifact->getLevelMax() )
                    bResult = true;
                break;
            }
            case E_COMIC::MAX_ARTIFACT_3:
            {
                auto objArtifact = DataManager::GetArtifactInfo(E_ARTIFACT::ARTI_3_EVOL1);
                if ( UserInfoManager::getInstance()->getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_3_EVOL1) >= objArtifact->getLevelMax() )
                    bResult = true;
                break;
            }
            case E_COMIC::MAX_ARTIFACT_4:
            {
                auto objArtifact = DataManager::GetArtifactInfo(E_ARTIFACT::ARTI_4_EVOL1);
                if ( UserInfoManager::getInstance()->getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_4_EVOL1) >= objArtifact->getLevelMax() )
                    bResult = true;
                break;
            }
            case E_COMIC::MAX_ARTIFACT_8:
            {
                auto objArtifact = DataManager::GetArtifactInfo(E_ARTIFACT::ARTI_8_EVOL1);
                if ( UserInfoManager::getInstance()->getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_8_EVOL1) >= objArtifact->getLevelMax() )
                    bResult = true;
                break;
            }
            case E_COMIC::MAX_ARTIFACT_12:
            {
                auto objArtifact = DataManager::GetArtifactInfo(E_ARTIFACT::ARTI_12_CROSS_EVOL1);
                if ( UserInfoManager::getInstance()->getArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_12_CROSS_EVOL1) >= objArtifact->getLevelMax() )
                    bResult = true;
                break;
            }
                
            case E_COMIC::QUEST_OPEN_7:
            {
                int nLevel = UserInfoManager::getInstance()->getQuestLevel(E_PLACE::DG_NORMAL, E_QUEST::QUEST_7);
                if ( nLevel >= 1 )
                    bResult = true;
                break;
            }
            case E_COMIC::QUEST_OPEN_11:
            {
                int nLevel = UserInfoManager::getInstance()->getQuestLevel(E_PLACE::DG_NORMAL, E_QUEST::QUEST_11);
                if ( nLevel >= 1 )
                    bResult = true;
                break;
            }
            case E_COMIC::QUEST_OPEN_24:
            {
                int nLevel = UserInfoManager::getInstance()->getQuestLevel(E_PLACE::DG_NORMAL, E_QUEST::QUEST_24);
                if ( nLevel >= 1 )
                    bResult = true;
                break;
            }
            case E_COMIC::MYHOME:
            {
                
                for(int i = 1; i >= C_MAX_MYHOME; ++i)
                {
                    int nLevel = UserInfoManager::getInstance()->getMyhomeLevel(i);
                    
                    if ( nLevel > 1 )
                    {
                        bResult = true;
                        break;
                    }
                }
                break;
            }
            case E_COMIC::RAID_JOIN: bResult = true; break;
            case E_COMIC::EXPEDITION_JOIN: bResult = true; break;
            default:
                break;
        }
    }
    
    return bResult;
}

void StoryManager::setStoryOpen(int nIdx, bool bOpen)
{
    //if(nIdx == (int)E_COMIC::TUTORIAL)
        
    std::string strKey = MafUtils::format("cm_%d", nIdx);
    UserDefault::getInstance()->setBoolForKey(strKey.c_str(), bOpen);
}

bool StoryManager::isStoryFile(int nIdx)
{
    bool bResult = false;
    
    auto obj = getStory(nIdx);
    if ( obj != nullptr )
    {
        bResult = true;
        
        //
        std::vector<std::string> listImage = obj->getFileNameImage();
        for ( std::string str : listImage )
        {
            if ( FileUtils::getInstance()->isFileExist(str) == false )
            {
                bResult = false;
            }
        }
        
        //
        std::vector<std::string> listText = obj->getFileNameText();
        for ( std::string str : listText )
        {
            if ( FileUtils::getInstance()->isFileExist(str) == false )
            {
                bResult = false;
            }
        }
    }
    
    return bResult;
}

int StoryManager::getStoryDonwnloadFileCount(int nIdx)
{
    int nResult = 0;
    
    std::vector<std::string> list;
    for ( const auto &objStory : _listStory)
    {
        if ( nIdx != 0 && objStory->getIdx() != nIdx )
        {
            continue;
        }
        
        std::vector<std::string> listImage = objStory->getFileNameImage();
        std::vector<std::string> listText = objStory->getFileNameText();
        for(const auto &str : listImage) {
            if ( FileUtils::getInstance()->isFileExist(str) == false )
            {
                list.push_back(str);
            }
        }
        for(const auto &str : listText) {
            if ( FileUtils::getInstance()->isFileExist(str) == false )
            {
                list.push_back(str);
            }
        }
    }
    
    nResult = (int)list.size();
    return nResult;
}

#pragma mark - show
void StoryManager::showStory(int nIdx)
{
    showStory(nIdx, nullptr);
}

void StoryManager::showStory(int nIdx, const std::function<void(void)>& callback)
{
    bool bOpen = isStoryOpen(nIdx);
    if ( nIdx == (int)E_COMIC::EVENT_SUMMER || nIdx == (int)E_COMIC::EVENT_HALLOWEEN )
    {// 이벤트 : 바캉스, 할로윈 예외
        bOpen = false;
    }
    
    if ( bOpen == true )
    {// 이미 오픈된 상태이면 넘어가기
        return;
    }
    
    //
    setStoryOpen(nIdx, true);
    
    bool bFile = isStoryFile(nIdx);
    if ( bFile == false )
    {
        auto popup = PopupStoryDownload::create(nIdx);
        popup->setHideBackKey(false);
        popup->show();
        return;
    }
    
    // open
    auto popup = PopupStory::create(nIdx);
    popup->setCallbackFinish(callback);
    popup->setHideAll(false);
    popup->show();
}

#pragma mark - download
void StoryManager::download(int nIdx, const std::function<void(int)>& callbackStart, const std::function<void(double)>& callbackProgress, const std::function<void(int, int)>& callbackCount, const std::function<void(void)>& callbackEnd)
{
    //
    PopupLoadingLock::show();
    
    //
    _callbackDownloadStart = callbackStart;
    _callbackDownloadProgress = callbackProgress;
    _callbackDownloadCount = callbackCount;
    _callbackDownloadEnd = callbackEnd;
    
    _listDownload.clear();
    for ( const auto &objStory : _listStory)
    {
        if ( nIdx != 0 && objStory->getIdx() != nIdx )
        {
            continue;
        }
    
        std::vector<std::string> listImage = objStory->getFileNameImage();
        std::vector<std::string> listText = objStory->getFileNameText();
        for(const auto &str : listImage) {
            if ( FileUtils::getInstance()->isFileExist(str) == false )
            {
                _listDownload.push_back(str);
            }
        }
        for(const auto &str : listText) {
            if ( FileUtils::getInstance()->isFileExist(str) == false )
            {
                _listDownload.push_back(str);
            }
        }

    }
    
    //
    _nDownloadTotal = (int)_listDownload.size();
    _nDownloadSuccess = 0;
    _nDownloadError = 0;
    if ( _callbackDownloadStart != nullptr )
    {
        _callbackDownloadStart((int)_listDownload.size());
    }
    downloadStart();
}

void StoryManager::downloadStart()
{
    if ( _listDownload.size() == 0 )
    {
        downloadEnd();
        return;
    }
    
    // 파일 경로
    std::string strFilePath = _listDownload.at(0);
    _listDownload.erase(_listDownload.begin());
    
    // 다운 경로
    std::string strUrl = "https://storage.googleapis.com/indigo-lambda-174903_gamedata/images/";
    strUrl.append(strFilePath);
    
    // 저장 경로
    _strStoragePathFull = FileUtils::getInstance()->getWritablePath() + strFilePath;
    _strStoragePath = _strStoragePathFull.substr(0, _strStoragePathFull.find_last_of("/") + 1);

    // 파일 생성
    if ( FileUtils::getInstance()->isDirectoryExist(_strStoragePath) == false )
    {
        FileUtils::getInstance()->createDirectory(_strStoragePath);
    }
    
    // 이전에 파일이 있으면 삭제.
    FileUtils::getInstance()->removeDirectory(_strStoragePathFull);
    FileUtils::getInstance()->removeDirectory(_strStoragePathFull + ".tmp");
    FileUtils::getInstance()->removeFile(_strStoragePathFull);
    FileUtils::getInstance()->removeFile(_strStoragePathFull + ".tmp");
    
    //
    _downloader->createDownloadFileTask(strUrl, _strStoragePathFull);
}

void StoryManager::downloadEnd()
{
    //
    PopupLoadingLock::hide();
    
    //
    if ( _callbackDownloadEnd != nullptr )
    {
        _callbackDownloadEnd();
    }
}

void StoryManager::downloadTaskProgress(const cocos2d::network::DownloadTask& task, int64_t bytesReceived, int64_t totalBytesReceived, int64_t totalBytesExpected)
{
    double nPercent = double(totalBytesReceived * 100) / totalBytesExpected;
    if ( _callbackDownloadProgress != nullptr )
    {
        _callbackDownloadProgress(nPercent);
    }
    
}

void StoryManager::downloadFileSuccess(const cocos2d::network::DownloadTask& task)
{
    _nDownloadSuccess++;
    if ( _callbackDownloadCount != nullptr )
    {
        _callbackDownloadCount(_nDownloadSuccess, _nDownloadError);
    }
    
    //
    downloadStart();
}

void StoryManager::downloadTaskError(const cocos2d::network::DownloadTask& task, int errorCode, int errorCodeInternal, const std::string& errorStr)
{
    _nDownloadError++;
    if ( _callbackDownloadCount != nullptr )
    {
        _callbackDownloadCount(_nDownloadSuccess, _nDownloadError);
    }
    
    //
    downloadStart();
}
