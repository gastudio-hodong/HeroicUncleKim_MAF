//
//  DebugLog.h
//
//

#ifndef DebugLog_h
#define DebugLog_h

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"

class DebugLog : public cocos2d::ui::Layout
{
public:
    CREATE_FUNC(DebugLog);
    
    DebugLog(void);
    virtual ~DebugLog(void);
    virtual bool init() override;
    
    void onInit();
    void onStart();
    void onRefresh();
    
public :
    // game
    static void log(const char * format, ...);
    static void log(std::string strLog);
    
    std::string getHierarchy();
    std::string getTreeHierarchy(cocos2d::Node* tree, int depth);
    
    bool isScreen();
    
protected:
    // ui : init
    void uiInitScreen();
    void uiInitFunction();
    void uiInitLog();
    
    // ui : draw
    void uiDrawScreen();
    void uiDrawFunction();
    void uiDrawLog();
    
    // ui : get, set
    
    // ui : click
    void onTouchExpand(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
    void onClickExpand(cocos2d::Ref* sender);
    void onClickFunc(cocos2d::Ref* sender);
    
    // schedule
        
    // callback
    
private:
    bool _bInitUI;
    
    //
    bool _bScreen;
    
    bool _bButtonExpandMove;
    int64_t _paddingButtonExpand;
    
    static cocos2d::Vector<cocos2d::ui::Layout*> _listLog;
    
    
    // widget
    cocos2d::ui::Layout* _uiContainerScreen;
    cocos2d::ui::Layout* _uiContainerFunction;
    cocos2d::ui::Layout* _uiContainerLog;
    
    cocos2d::ui::ScrollView* _uiInfoLog;
    
    cocos2d::ui::Layout* _uiBtnExpand;
};

#endif /* DebugLog_h */
