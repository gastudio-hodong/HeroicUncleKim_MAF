//
//  UIRaceCommentary.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/08/02.
//

#ifndef UIRaceCommentary_hpp
#define UIRaceCommentary_hpp

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

class UIRaceCommentary : public cocos2d::LayerColor, public cocos2d::extension::TableViewDataSource, public cocos2d::extension::TableViewDelegate
{
    enum class E_TYPE
    {
        NONE = 0,
        
        ADS,
        READY,
        START,
        MIDDLE,
        END,
        GOAL
    };
public:
    static UIRaceCommentary* create(const cocos2d::Size size);
    
    UIRaceCommentary(void);
    virtual ~UIRaceCommentary(void);
    virtual bool init(const cocos2d::Size size);
    virtual void update(float dt) override;
    
    // table TableViewDelegate
    virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell) override;
    
    // table TableViewDataSource
    virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx) override;
    virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx) override;
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table) override;
protected:
    // init
    void initVar();
    void initUI();
    void setLoadTextKey();
    
    //utils
    void nextType();
    void checkCommentType();
    const std::string getText(const std::string strKey) const;
    const std::string getCurrentComment();
    
    //event
    void onEventRaceRaedy();
    void onEventClickStart();
    void onEventClickAds(int nType);
private:
    E_TYPE _eType;
    int _nCommentaryIdx;
    float _timer;
    std::vector<std::string> _listCommentaryKeyReady;
    std::vector<std::string> _listCommentaryKeyStart;
    std::vector<std::string> _listCommentaryKeyMiddle;
    std::vector<std::string> _listCommentaryKeyEnd;
    std::vector<std::string> _listCommentaryKeyGoal;
    std::vector<std::string> _listCommentaryKeyTalk;
    std::vector<std::string> _listCommentaryKeyAds;
    
    std::vector<std::string> _listCurGameCommentary;
    
    cocos2d::extension::TableView *_table;
};

#endif /* UIRaceCommentary_hpp */
