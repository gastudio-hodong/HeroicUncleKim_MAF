//
//  StoryManager.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 19/09/2019.
//

#ifndef StoryManager_h
#define StoryManager_h

#include "Libs/maf/MafSingleton.h"

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

#include "GameObject/InfoStory.h"

class StoryManager : public MafSingleton<StoryManager>
{
public:
    StoryManager(void);
    virtual ~StoryManager(void);
    virtual bool init();
    
public:
    // load
    void setLoad();
    
    // cloud
    void setCloudStory(std::string strData);
    std::string getCloudStory();
    
    //
    cocos2d::Vector<InfoStory*> getStoryAll();
    InfoStory* getStory(int nIdx);
    
    //
    bool isStoryOpen(int nIdx);
    bool isStoryOpenException(int nIdx);
    void setStoryOpen(int nIdx, bool bOpen);
    bool isStoryFile(int nIdx);
    int getStoryDonwnloadFileCount(int nIdx);
    
    // show
    void showStory(int nIdx);
    void showStory(int nIdx, const std::function<void(void)>& callback );
    
    // download
    void download(int nIdx, const std::function<void(int)>& callbackStart, const std::function<void(double)>& callbackProgress, const std::function<void(int, int)>& callbackCount, const std::function<void(void)>& callbackEnd);
    void downloadStart();
    void downloadEnd();
    void downloadTaskProgress(const cocos2d::network::DownloadTask& task, int64_t bytesReceived, int64_t totalBytesReceived, int64_t totalBytesExpected);
    void downloadFileSuccess(const cocos2d::network::DownloadTask& task);
    void downloadTaskError(const cocos2d::network::DownloadTask& task, int errorCode, int errorCodeInternal, const std::string& errorStr);
private:
    
    
    
private:
    int _nIdxMax;
    cocos2d::Vector<InfoStory*> _listStory;
    
    //
    std::unique_ptr<cocos2d::network::Downloader> _downloader;
    std::vector<std::string> _listDownload;
    int _nDownloadTotal;
    int _nDownloadSuccess;
    int _nDownloadError;
    std::string _strStoragePathFull;
    std::string _strStoragePath;
    
    //
    std::function<void(int)> _callbackDownloadStart;
    std::function<void(double)> _callbackDownloadProgress;
    std::function<void(int, int)> _callbackDownloadCount;
    std::function<void(void)> _callbackDownloadEnd;
};

#endif /* StoryManager_h */
