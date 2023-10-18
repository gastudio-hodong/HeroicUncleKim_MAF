//
//  PopupJelly.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#ifndef PopupJelly_h
#define PopupJelly_h

#include "GameUIPopup/Base/PopupBase.h"

class PopupJelly : public PopupBase, public cocos2d::extension::TableViewDataSource, public cocos2d::extension::TableViewDelegate
{
public:
    static PopupJelly* create();
    
    PopupJelly(void);
    virtual ~PopupJelly(void);
    virtual bool init();
    virtual void onEnter();
    
    // table TableViewDelegate
    virtual void scrollViewDidScroll(cocos2d::extension::ScrollView* view){};
    virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell);
    
    // table TableViewDataSource
    virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx);
    virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table);
    
public:
    // set
    void setCallbackHeart(const std::function<void(void)>& callback);
    
    
protected:
    // init
    void initVar();
    void initUi();
    
    // ui
    void uiInfo();
    void uiContentBuy();
    void uiContentList();
    void uiButtons();
    
    // ui draw
    void drawInfo();
    void drawButtons();
    void drawContentList();
    
    // click
    void onClickClose(cocos2d::Ref* sender);
    void onClickJellyBuy(cocos2d::Ref* sender);
    void onClickItemBuy(cocos2d::Ref* sender);
    void onClickReward(cocos2d::Ref* sender);
    void onClickAds(cocos2d::Ref* sender);
    
    // callback
    void callbackInfo(bool bResult);
    void callbackAds(ADS_RESULT result);
    void callbackAdsTicket(bool bResult);
    
    // network
    void responseInfo(cocos2d::network::HttpResponse* response,std::string &data);
    void requestJellyBuy();
    void responseJellyBuy(cocos2d::network::HttpResponse* response,std::string &data);
    void requestItemBuy();
    void responseItemBuy(cocos2d::network::HttpResponse* response,std::string &data);
    void requestReward();
    void responseReward(cocos2d::network::HttpResponse* response,std::string &data);
    
    
    
private:
    E_JELLY _typeSelected;
    int64_t _timeElapse;
    
    
    int _listItemLevel[3];

    //
    std::function<void(void)> _onCallbackHeart;
    
    // widget
    cocos2d::ui::Layout* _uiContentsUIInfo;
    cocos2d::ui::Layout* _uiContentsUIContentBuy;
    cocos2d::ui::Layout* _uiContentsUIContentList;
    cocos2d::ui::Layout* _uiContentsUIButton;
    
    cocos2d::ui::ImageView* _uiImgInfoBG;
    cocos2d::ui::ImageView* _uiImgInfoGummy;
    cocos2d::ui::CText* _uiTextInfo;
    
    cocos2d::ui::Button* _uiBtnAds;
    cocos2d::ui::Button* _uiBtnBuy;
    cocos2d::ui::Button* _uiBtnReward;
    
    cocos2d::extension::TableView *_table;
};

#endif /* PopupJelly_h */
