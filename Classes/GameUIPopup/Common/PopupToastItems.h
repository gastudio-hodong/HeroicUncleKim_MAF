//
//  PopupToastItems.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 6. 4..
//

#ifndef PopupToastItems_h
#define PopupToastItems_h

#include "Common/ConfigDefault.h"

class InfoItems;
class PopupToastItems : public cocos2d::Layer
{
public:
    static PopupToastItems* create(cocos2d::Vector<InfoItems*> listItems, bool bSpecial, bool bAttend);
    
    PopupToastItems(void);
    virtual ~PopupToastItems(void);
    virtual bool init(cocos2d::Vector<InfoItems*> listReward, bool bSpecial, bool bAttend);
    
public:
    static void showSpecial(cocos2d::Vector<InfoItems*> listItems);
    static void showAttend(cocos2d::Vector<InfoItems*> listItems);
    static void show(cocos2d::Vector<InfoItems*> listItems);
    static void show(cocos2d::Vector<InfoItems*> listItems, bool bSpecial, bool bAttend);
    
protected:
    
    // init
    void initVar();
    void initUi();
    
    // ui
    void uiReward();
    void uiAttend();
    
private:
    static int _nCountVisible;
    
    int _nRewardPaddingX;
    int _nRewardPaddingY;
    int _nRewardDrawCol;
    int _nRewardDrawRow;
    double _nRewardWidth;
    double _nRewardHeight;
    double _nRewardBoxSize;
    
    bool _bSpecial;
    bool _bAttend;
    
    cocos2d::Vector<InfoItems*> _listItems;
    
    //
    cocos2d::Layer* _layerContainerBottom;
    cocos2d::Layer* _layerContainerAttend;
    
};

#endif /* PopupToastItems_h */
