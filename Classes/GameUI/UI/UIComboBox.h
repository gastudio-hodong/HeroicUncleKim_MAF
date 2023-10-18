//
//  UIComboBox.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#ifndef UIComboBox_h
#define UIComboBox_h

#include "Common/ConfigDefault.h"

class UIComboBox : public cocos2d::LayerColor, public cocos2d::extension::TableViewDataSource, public cocos2d::extension::TableViewDelegate
{
public:
    enum eTag {
        tag_table_contents  = 10,
    };
    
    enum eType {
        type_up = 10,
        type_down = 20,
    };
    
    
public:
    static UIComboBox* create(cocos2d::ValueVector list, eType type, std::string strPathBtn, std::string strPathList);
    
    UIComboBox(void);
    virtual ~UIComboBox(void);
    virtual bool init(cocos2d::ValueVector list, eType type, std::string strPathBtn, std::string strPathList);
    
    
    // table TableViewDelegate
    virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell);
    
    // table TableViewDataSource
    virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx);
    virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table);
    
    
public:
    // set, get, add
    void addData(std::string str);
    
    void setDataSelect(std::string str);
    std::string getDataSelect();
    
    void setIconArrow(std::string strPathUp, std::string strPathDown);
    void setEnable(bool enable);
    void setTextListFont(std::string strName, int nSize, cocos2d::Color3B color);
    
    //
    void setCallbackSelect(const std::function<void(std::string)>& callback);
    
protected:
    // init
    void initVar();
    void initUi();
    
    // draw
    void drawComboBox();
    
    // click
    void onClickBox(cocos2d::Ref* sender);
    

private:
    cocos2d::ValueVector _listData;
    eType _type;
    std::string _strPathBtn;
    std::string _strPathList;
    std::string _strPathArrowUp;
    std::string _strPathArrowDown;
    
    bool _isOpen;
    bool _isEnable;
    std::string _strDataSelect;
    
    std::string _strTextListFontName;
    int _nTextListFontSize;
    cocos2d::Color3B _colorTextListFontColor;
    
    
    // callback
    std::function<void(std::string)> _callbackSelect;
    
    
    // widget
    cocos2d::Sprite* _spriteArrow;
    cocos2d::Sprite* _spriteList;
    cocos2d::Label* _labelContents;
    cocos2d::extension::TableView *_table;
};

#endif /* UIComboBox_h */
