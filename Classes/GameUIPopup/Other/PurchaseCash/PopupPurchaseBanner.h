//
//  PopupPurchaseBanner.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#ifndef PopupPurchaseBanner_h
#define PopupPurchaseBanner_h

#include "GameUIPopup/Base/PopupBase.h"

#include "GameObject/InfoProductBanner.h"

class PopupPurchaseBanner : public PopupBase, public cocos2d::extension::TableViewDataSource, public cocos2d::extension::TableViewDelegate
{
public:
    static PopupPurchaseBanner* create();
    
    PopupPurchaseBanner(void);
    virtual ~PopupPurchaseBanner(void);
    virtual bool init();
    virtual void onEnter();
    virtual void show();
    
    // table TableViewDelegate
    virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell);
    
    // table TableViewDataSource
    virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx);
    virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table);
    
public:
    
protected:
    // init
    void initVar();
    void initUi();
    
    // ui
    void uiContents();
    void uiButton();
    
    // draw
    void drawContetns();
    void drawButton();
    
    // click
    void onClickClose(cocos2d::Ref* sender);
    void onClickMove(cocos2d::Ref* sender);
    void onClickBuy(cocos2d::Ref* sender);
    void onClickBonus(cocos2d::Ref* sender);
    
    // callback
    void callbackInfo(bool bResult);
    void callbackPurchaseCheck(bool bResult);
    void callbackPurchase(MafGooglePlay::RESULT eResult, int nResult, int nIdx, std::string strItems);
    void callbackBonusCheck(bool bResult);
    void callbackBonus(bool bResult);
    
    // download
    void downloadStart();
    void downloadEnd();
    void downloadTaskProgress(const cocos2d::network::DownloadTask& task, int64_t bytesReceived, int64_t totalBytesReceived, int64_t totalBytesExpected);
    void downloadFileSuccess(const cocos2d::network::DownloadTask& task);
    void downloadTaskError(const cocos2d::network::DownloadTask& task, int errorCode, int errorCodeInternal, const std::string& errorStr);
    
   
private:
    cocos2d::Vector<InfoProductBanner*> _listBanner;
    int _nBannerIdx;
    
    int _nBannerSelectIdx;
    int _nBannerTotal;
    int _nBannerCount;
    int _nBannerBonusCount;
    
    //
    std::unique_ptr<cocos2d::network::Downloader> _downloader;
    std::vector<std::string> _listDownload;
    int _nDownloadSuccess;
    int _nDownloadError;
    
    
    // widget
    cocos2d::Layer* _layerContainerTitle;
    cocos2d::Layer* _layerContainerContents;
    cocos2d::Layer* _layerContainerButton;
    
    cocos2d::extension::TableView* _table;
    cocos2d::Label* _labelTime;
    cocos2d::Label* _labelMove;
};

#endif /* PopupPurchase_h */
