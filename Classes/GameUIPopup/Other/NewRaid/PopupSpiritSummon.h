//
//  PopupSpiritSummon.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/12/01.
//

#ifndef PopupSpiritSummon_hpp
#define PopupSpiritSummon_hpp

#include "GameUIPopup/Base/PopupBase.h"

class InfoItems;
class PopupSpiritSummon : public PopupBase, public cocos2d::extension::TableViewDataSource, public cocos2d::extension::TableViewDelegate
{
public:
    enum class E_TYPE
    {
        CLOSE = -1,
        NONE = 0,
        
        SUMMON_1 = 1,
        SUMMON_11,
        SUMMON_33,
    };
    static PopupSpiritSummon* create();
    
    PopupSpiritSummon();
    virtual ~PopupSpiritSummon();
    
    
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
    //utils
    
    void setCallback(const std::function<void(void)>& callback);
    void setCallbackTransferShop(const std::function<void(void)>& callback);
    void setReward(cocos2d::Vector<InfoItems*> vecReward);
    
    void onClick(Ref* sender);
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
    
    std::function<void(void)> _callback;
    std::function<void(void)> _callbackTransferShop;
    cocos2d::Vector<InfoItems*> _vecReward;
};


#endif /* PopupSpiritSummon_hpp */
