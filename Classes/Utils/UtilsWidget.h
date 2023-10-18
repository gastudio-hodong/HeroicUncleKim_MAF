//
//  UtilsWidget.h
//

#ifndef UtilsWidget_h
#define UtilsWidget_h

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class UtilsWidget
{
public:
    static UtilsWidget* getInstance();
    
    UtilsWidget(void);
    virtual ~UtilsWidget(void);
    virtual bool init();
    
public:
    
    /**
     
     */
    cocos2d::Node* seekWidgetParent(cocos2d::Node* nodeRoot);
    
    /**
     
     */
    void sortWidgetGrid(cocos2d::Node* root, int col, double width, double height, double lineSpacing, bool trimX = true, bool trimY = true);
    void sortWidgetGrid(cocos2d::Node* root, double padding = 10);
  
    /**
     type HORIZONTAL, VERTICAL
     */
    cocos2d::Size getLayoutAccumulatedSize(cocos2d::ui::Layout* root);
    
    /**
     
     */
    cocos2d::Node* createIconText(std::string strIconPath, std::string strText, double nWidthParent = 0, int nFontSize = 30, int nIconPadding = 0);
    
    /**
     
     */
    cocos2d::Vec2 getSizeToPercent(cocos2d::Vec2 size, double posX, double posY);
private:
    
private:
    
    
};

#endif /* UtilsWidget_h */
