//
//  PopupAdventureRelicCraft.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/05/11.
//

#ifndef PopupAdventureRelicCraft_hpp
#define PopupAdventureRelicCraft_hpp

#include "GameUIPopup/Base/PopupBase.h"

#include "Common/Observer/IRefresh.h"

#include "GameObject/Adventure/InfoAdventureCore.h"

class InfoItems;
class PopupAdventureRelicCraft : public PopupBase, public cocos2d::extension::TableViewDataSource, public cocos2d::extension::TableViewDelegate, public IRefresh
{
public:
    static PopupAdventureRelicCraft* create();
    
    PopupAdventureRelicCraft();
    virtual ~PopupAdventureRelicCraft();
    
    
    // table TableViewDelegate
    virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell) override;
    
    // table TableViewDataSource
    virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx) override;
    virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx) override;
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table) override;
    
    
    virtual bool init() override;
    
    // init
    void initVar();
    void initUI();
    
    void uiTop();
    void uiMiddle();
    void uiBottom();
    
    virtual void show() override;
    virtual void refresh(E_REFRESH eType, cocos2d::ValueMap listData)  override;
    //utils
    
    void setCallback(const std::function<void(void)>& callback);
    void setCallbackTutorial(const std::function<void(void)>& callback);
    void setCallbackTransferShop(const std::function<void(void)>& callback);
    void setReward(cocos2d::Vector<InfoItems*> vecReward);
    void setCore(InfoAdventureCore* infoCore);
    
    void onClickClose(Ref* sender);
    void onClickCraft(Ref* sender);
    //callback
    void callbackSummon(bool bResult, int nResult, cocos2d::Vector<InfoItems*> listReward);
    void callbackTranferShop();
private:
    bool _isClose;
    bool _isMakeStart;
    bool _isMakeEnd;
    cocos2d::Layer* _lyBG;
    cocos2d::Layer* _lyTopUI;
    cocos2d::Layer* _lyMiddleUI;
    cocos2d::Layer* _lyBottomUI;
    cocos2d::extension::TableView *_table;
    InfoAdventureCore* _infoCore;
    
    std::function<void(void)> _callback;
    std::function<void(void)> _callbackTutorial;
    std::function<void(void)> _callbackTransferShop;
    cocos2d::Vector<InfoItems*> _vecReward;
};
#endif /* PopupAdventureRelicCraft_hpp */
