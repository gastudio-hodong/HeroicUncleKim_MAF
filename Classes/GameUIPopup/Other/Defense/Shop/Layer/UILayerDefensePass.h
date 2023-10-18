//
//  UILayerDefensePass.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2023/08/25.
//

#ifndef UILayerDefensePass_hpp
#define UILayerDefensePass_hpp


#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

class InfoProduct;
class InfoItems;
class InfoProductGrowth;
class UILayerDefensePass : public cocos2d::LayerColor
{
public:
    static UILayerDefensePass* create(cocos2d::Size size);
    
    UILayerDefensePass(void);
    virtual ~UILayerDefensePass(void);
    virtual bool init(cocos2d::Size size);
    virtual void onEnter() override;
public:
    // set, get
    
protected:
    // init
    void initVar();
    void initUi();
    
    // ui
    void uiTop();
    void uiMain();
    void uiBottom();
    
    bool isCondition(InfoProductGrowth* objGrowth);
    
    void onClickPage(Ref* sender);
    void onClickBuy(Ref* sender);
    void onClickReward(Ref* sender);
    
    void callbackInfo(bool bResult, int bBuy);
    void callbackPurchase(MafGooglePlay::RESULT eResult, int nResult, int nIdx, std::string strItems);
    void callbackReceive(bool bResult);
    
    void onFloorUpdate();
private:
    int _level;
    int _levelNow;
    int _levelMax;
    int _idxCash;
    bool _bBuy;
    std::string _type;
    
    InfoProduct* _objProduct;
    cocos2d::Vector<InfoItems*> _listProductItmes;
    cocos2d::Vector<InfoProductGrowth*> _listGrowth;
    
    
    cocos2d::LayerColor* _lyMain;
    cocos2d::LayerColor* _lyBottom;
public:
    
    MafDelegate<void(void)> _onRefresh;
};


#endif /* UILayerDefensePass_hpp */
