//
//  PopupPurchaseGrowth.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#ifndef PopupPurchaseGrowth_h
#define PopupPurchaseGrowth_h

#include "GameUIPopup/Base/PopupBase.h"

#include "GameObject/InfoProduct.h"

class InfoItems;
class InfoProduct;
class InfoProductGrowth;
class PopupPurchaseGrowth : public PopupBase, public cocos2d::ui::DelegateListView
{    
public:
    static PopupPurchaseGrowth* create(std::string type);
    
    PopupPurchaseGrowth(void);
    virtual ~PopupPurchaseGrowth(void);
    virtual bool init(std::string type);
    virtual void onEnter() override;
    
    // listview
    virtual void listviewScrollView(cocos2d::ui::CListView* listview, cocos2d::ui::ScrollView::EventType type) override;
    virtual cocos2d::Size listviewCellSize(cocos2d::ui::CListView* listview, ssize_t idx) override;
    virtual void listviewCell(cocos2d::ui::CListView *listview, cocos2d::ui::Widget* childview, ssize_t idx) override;
    virtual int64_t listviewNumberOfCells(cocos2d::ui::CListView *listview) override;
    
public:
    // set, get

    
protected:
    // init
    void initVar();
    void initUi();
    
    // ui
    void uiInfo();
    void uiContentCash();
    void uiContentList();
    
    // ui draw
    void drawInfo();
    void drawContentCash();
    void drawContentList();
 
    
    // set, get
    std::string getGrowthName(InfoProductGrowth* objGrowth);
    bool isCondition(InfoProductGrowth* objGrowth);
    
    // click
    void onClickClose(cocos2d::Ref* sender);
    void onClickMove(cocos2d::Ref* sender);
    void onClickLevel(cocos2d::Ref* sender);
    void onClickBuy(cocos2d::Ref* sender);
    void onClickGet(cocos2d::Ref* sender);
    
    // callback
    void callbackInfo(bool isResult, bool bBuy);
    void callbackPurchase(MafGooglePlay::RESULT eResult, int nResult, int nIdx, std::string strItems);
    void callbackReceive(bool isResult);
    
private:
    std::string _type;
    int _level;
    int _levelNow;
    int _levelMax;
    int _idxCash;
    bool _bBuy;
    
    InfoProduct* _objProduct;
    cocos2d::Vector<InfoItems*> _listProductItmes;
    cocos2d::Vector<InfoProductGrowth*> _listGrowth;
    
    // widget
    cocos2d::ui::Layout* _uiContentsUIInfo;
    cocos2d::ui::Layout* _uiContentsUIContentCash;
    cocos2d::ui::Layout* _uiContentsUIContentList;
    
    cocos2d::ui::ImageView* _uiInfoBG;
    cocos2d::ui::CText* _uiInfoLevel;
    
    cocos2d::ui::CListView* _uiList;
 
};

#endif /* PopupPurchaseGrowth_h */
