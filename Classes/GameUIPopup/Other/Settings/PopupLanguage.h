//
//  PopupLanguage.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#ifndef PopupLanguage_h
#define PopupLanguage_h

#include "GameUIPopup/Base/PopupBase.h"

class PopupLanguage : public PopupBase, public cocos2d::extension::TableViewDataSource, public cocos2d::extension::TableViewDelegate
{    
public:
    static PopupLanguage* create();
    
    PopupLanguage(void);
    virtual ~PopupLanguage(void);
    virtual bool init(void) override;
    
    // table TableViewDelegate
    virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell) override;
    virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx) override;
    virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx) override;
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table) override;
    
protected:
    
    // init
    void initVal();
    void initUi();
    
    // ui
    void uiContents();
    void uiButton();
    
    // draw
    void drawContents();
    
    // click
    void onClickSelect(cocos2d::Ref* sender);
    void onClickCancel(cocos2d::Ref* sender);
    void onClickApply(cocos2d::Ref* sender);
    
    // callback
    void callbackTable(bool bResult);
    void callbackTableProgress(int nTotal, int nSuccess, int nProgress);
    
private:
    struct element
    {
        std::string strCode;
        bool bBeta;
    };
    
    //
    std::vector<element> _list;
    std::string _strLangChange;
    
    // widget
    cocos2d::Layer* _layerContainerContents;
    cocos2d::Layer* _layerContainerButton;
    
    cocos2d::extension::TableView* _table;
    cocos2d::Label* _labelProgress;
    
};

#endif /* PopupLanguage_h */
