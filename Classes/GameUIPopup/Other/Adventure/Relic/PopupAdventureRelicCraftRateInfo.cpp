//
//  PopupAdventureRelicCraftRateInfo.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/05/17.
//

#include "PopupAdventureRelicCraftRateInfo.h"

#include "ManagerGame/AdventureRelicManager.h"
#include "ManagerGame/AdventureManager.h"

USING_NS_CC;

PopupAdventureRelicCraftRateInfo* PopupAdventureRelicCraftRateInfo::create()
{
    PopupAdventureRelicCraftRateInfo* pRet = new(std::nothrow) PopupAdventureRelicCraftRateInfo();
    if ( pRet && pRet->init() )
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}

PopupAdventureRelicCraftRateInfo::PopupAdventureRelicCraftRateInfo(void):
_nLevel(0),
_eType(PopupAdventureRelicCraftRateInfo::E_TYPE::AREA_CORE),
_lyTopUI(nullptr),
_lyTableUI(nullptr),
_table(nullptr)
{
    _listRate.clear();
}

PopupAdventureRelicCraftRateInfo::~PopupAdventureRelicCraftRateInfo(void)
{
    _listRate.clear();
}


#pragma mark - table
void PopupAdventureRelicCraftRateInfo::tableCellTouched(extension::TableView* table, extension::TableViewCell* cell)
{
    
}

Size PopupAdventureRelicCraftRateInfo::tableCellSizeForIndex(extension::TableView *table, ssize_t idx)
{
    return Size(table->getContentSize().width, 70);
}

extension::TableViewCell* PopupAdventureRelicCraftRateInfo::tableCellAtIndex(extension::TableView *table, ssize_t idx)
{
    auto cell = table->dequeueCell();
    if ( cell == nullptr )
    {
        cell = new extension::TableViewCell();
        cell->autorelease();
    }
    cell->removeAllChildren();
    
    auto list = _infoRateNow->getListRate();
    
    auto obj = list[idx];
    
    Size size = tableCellSizeForIndex(table, idx);
    cell->setContentSize(size);
    
    auto sprGradeBox = ui::Scale9Sprite::create("Assets/ui_common/base_box_1_3.png");
    sprGradeBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    sprGradeBox->setContentSize(Size(388,44));
    sprGradeBox->setPosition(Vec2(15, cell->getContentSize().height/2));
    cell->addChild(sprGradeBox);
    {
        int size = obj.first;
        float width = 22;
        float scale = 1.3f;
        float posX = sprGradeBox->getContentSize().width/2 - width * scale * (size/2);
        
        for(int i = 0 ; i < obj.first; ++i)
        {
            auto sprGrade = Sprite::create("Assets/ui_common/icon_star1_2.png");
            sprGrade->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            sprGrade->setPosition(posX, sprGradeBox->getContentSize().height/2);
            sprGrade->getTexture()->setTexParameters(texParams);
            sprGrade->setScale(scale);
            sprGradeBox->addChild(sprGrade);
            
            if(size % 2 == 0)
                sprGrade->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            
            posX += width * scale;
        }
    }
    
    auto sprRateBox = ui::Scale9Sprite::create("Assets/ui_common/base_box_1_9.png");
    sprRateBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    sprRateBox->setContentSize(Size(214,44));
    sprRateBox->setPosition(sprGradeBox->getPosition() + Vec2::RIGHT * (sprGradeBox->getContentSize().width + 10));
    cell->addChild(sprRateBox);
    {
        float rate = obj.second * 0.01f;
        
        auto lbText = Label::createWithTTF(MafUtils::format("%.3f%%", rate), GAME_FONT, 26);
        lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        lbText->setPosition(sprRateBox->getContentSize()/2);
        lbText->setAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
        lbText->setDimensions(sprRateBox->getContentSize().width - 10, sprRateBox->getContentSize().height - 5);
        lbText->setColor(COLOR_KEY);
        sprRateBox->addChild(lbText);
    }
    
    
    return cell;
}

ssize_t PopupAdventureRelicCraftRateInfo::numberOfCellsInTableView(extension::TableView *table)
{
    int size = 0;
    
    if(_infoRateNow != nullptr)
        size = _infoRateNow->getListRate().size();
    
    return size;
}

#pragma mark -init
bool PopupAdventureRelicCraftRateInfo::init()
{
    if ( !PopupBase::init())
    {
        return false;
    }
    auto listCore = AdventureRelicManager::getInstance()->getListInfoCore();
    
    for(const auto obj : listCore)
    {
        if(obj->getCurrentLv() > _nLevel)
            _nLevel = obj->getCurrentLv();
    }
    
    loadData();
    
    _infoRateNow = getInfoRate((int)_eType, _nLevel);
    
    initVar();
    initUI();
    
    if(AdventureManager::getInstance()->getTutorialStep() < 7)
        RefreshManager::getInstance()->addUI(E_REFRESH::ADVENTURE_TUTORIAL_COMPLETE, this);
    return true;
}
#pragma mark - Override
void PopupAdventureRelicCraftRateInfo::refresh(E_REFRESH eType, cocos2d::ValueMap listData)
{
    if(eType == E_REFRESH::ADVENTURE_TUTORIAL_COMPLETE)
        this->setLocalZOrder(ZORDER_SCENE_POPUP);
}
void PopupAdventureRelicCraftRateInfo::show()
{
    PopupBase::show();
    
    if(AdventureManager::getInstance()->getTutorialStep() < 7)
        this->setLocalZOrder(ZORDER_SCENE_FORCE_TUTORIAL);
}
void PopupAdventureRelicCraftRateInfo::loadData()
{
    std::string strText = TableManager::getInstance()->getTableData(E_TABLE::ADVENTURE_RELIC_RATE);
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)strText.c_str());
    if( jsonParser.HasParseError() == true )
    {
        return;
    }
    
    _listRate.clear();
    for ( int i = 0; i < jsonParser.Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser[i];
        
        if ( MafNative::IsLessThanCurrVersion(jsonValue["version"].GetString()) == false)
        {
            continue;
        }
        
        int nType = jsonValue["type"].GetInt();
        
        int nLv = jsonValue["lv"].GetInt();
        std::string strRate = jsonValue["rate"].GetString();
        rapidjson::Document rateParser;
        rateParser.ParseInsitu<0>((char*)strRate.c_str());
        
        std::vector<std::pair<int,int>> listRate;
        listRate.clear();
        
        for(int j = 0; j < rateParser.Size(); ++j)
        {
            const rapidjson::Value& data = rateParser[j];
            
            int grade = data["value"].GetInt();
            int rate = data["rate"].GetInt();
            
            listRate.push_back(std::make_pair(grade, rate));
        }
        
        auto obj = new InfoDrawRate();
        obj->setLevel(nLv);
        obj->setType(nType);
        obj->setListRate(listRate);
        _listRate.pushBack(obj);
    }
    
}

void PopupAdventureRelicCraftRateInfo::initVar()
{
    _spriteContainer->setContentSize(Size(672, 900));
}

void PopupAdventureRelicCraftRateInfo::initUI()
{
    // title
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_raidshop_spiritbox_drawrate_pop_title"), GAME_FONT, 28);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5) );
    _spriteContainer->addChild(labelTitle);
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupAdventureRelicCraftRateInfo::onClickClose, this));
    itemClose->setPosition( Vec2(_spriteContainer->getContentSize().width - itemClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5) );
    itemClose->setTouchRect( itemClose->getContentSize() * 2 );
    menu->addChild(itemClose);
    
    //650
    Size size = _spriteContainer->getContentSize();
    
    _lyTabUI = LayerColor::create();
    _lyTabUI->setContentSize( Size(size.width - 10, 90));
    _lyTabUI->setIgnoreAnchorPointForPosition(false);
    _lyTabUI->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _lyTabUI->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75));
    _spriteContainer->addChild(_lyTabUI);
    
    _lyTopUI = LayerColor::create();
    _lyTopUI->setContentSize( Size(size.width - 10, 360));
    _lyTopUI->setIgnoreAnchorPointForPosition(false);
    _lyTopUI->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _lyTopUI->setPosition(_lyTabUI->getPosition() + Vec2::DOWN * _lyTabUI->getContentSize().height);
    _spriteContainer->addChild(_lyTopUI);
    
    _lyTableUI = LayerColor::create();
    _lyTableUI->setContentSize( Size(size.width - 10, _spriteContainer->getContentSize().height - 75 - _lyTopUI->getContentSize().height));
    _lyTableUI->setIgnoreAnchorPointForPosition(false);
    _lyTableUI->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _lyTableUI->setPosition(_lyTopUI->getPosition() + Vec2::DOWN * _lyTopUI->getContentSize().height);
    _spriteContainer->addChild(_lyTableUI);
    
    uiTab();
    uiTable();
}

#pragma mark -ui
void PopupAdventureRelicCraftRateInfo::uiTab()
{
    _lyTabUI->removeAllChildren();
    
    std::vector<PopupAdventureRelicCraftRateInfo::E_TYPE> listTab =
    {
        PopupAdventureRelicCraftRateInfo::E_TYPE::AREA_CORE,
        PopupAdventureRelicCraftRateInfo::E_TYPE::CASH_CORE
    };
    
    int size = (int)listTab.size();
    
    float width = _lyTabUI->getContentSize().width / size - 10;
    float posX = _lyTabUI->getContentSize().width/2 - width * (size/2) - 5 * (size/2);
    
    for(int i = 0; i < listTab.size(); ++i)
    {
        auto obj = listTab[i];
        
        std::string strPath = "Assets/ui_common/btn_tap_2_off.png";
        if(obj == _eType)
        {
            strPath = "Assets/ui_common/btn_tap_2_on.png";
        }
        std::string strText = "";
        switch (obj) {
            case PopupAdventureRelicCraftRateInfo::E_TYPE::AREA_CORE: strText = GAME_TEXT("t_ui_currency_core_0"); break;
            case PopupAdventureRelicCraftRateInfo::E_TYPE::CASH_CORE: strText = GAME_TEXT("t_ui_currency_core_5"); break;
        }
        
        auto btnTab = ui::Button::create(strPath);
        btnTab->setScale9Enabled(true);
        btnTab->setContentSize(Size(width, 90));
        btnTab->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        btnTab->setPosition(Vec2(posX, _lyTabUI->getContentSize().height));
        btnTab->addClickEventListener(CC_CALLBACK_1(PopupAdventureRelicCraftRateInfo::onClickTab, this));
        btnTab->setZoomScale(0.05f);
        btnTab->setTag((int)obj);
        _lyTabUI->addChild(btnTab);
        {
            auto lbText = Label::createWithTTF(strText, GAME_FONT, 25);
            lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            lbText->setPosition(btnTab->getContentSize()/2);
            lbText->setDimensions(btnTab->getContentSize().width - 10, lbText->getContentSize().height);
            lbText->setAlignment(TextHAlignment::CENTER, TextVAlignment::TOP);
            lbText->setOverflow(Label::Overflow::SHRINK);
            btnTab->addChild(lbText);
        }
        
        if(size % 2 == 0)
            btnTab->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        
        posX += width + 10;
    }
    
    if(_eType == PopupAdventureRelicCraftRateInfo::E_TYPE::CASH_CORE)
        uiCashCore();
    else
        uiAreaCore();
    
}
void PopupAdventureRelicCraftRateInfo::uiAreaCore()
{
    _lyTopUI->removeAllChildren();
    
    auto lbText = Label::createWithTTF(GAME_TEXT("t_ui_advt_relic_crafting_info_msg"), GAME_FONT, 26);
    lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    lbText->setPosition(Vec2(_lyTopUI->getContentSize().width/2, _lyTopUI->getContentSize().height - 20));
    lbText->setAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::TOP);
    lbText->setDimensions(_lyTopUI->getContentSize().width - 10, _lyTopUI->getContentSize().height - 60);
    lbText->setOverflow(cocos2d::Label::Overflow::SHRINK);
    _lyTopUI->addChild(lbText);
    
    auto sprLevelBox = ui::Scale9Sprite::create("Assets/ui_common/box_02.png");
    sprLevelBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    sprLevelBox->setContentSize(Size(340,58));
    sprLevelBox->setPosition(lbText->getPosition() + Vec2::DOWN * (lbText->getContentSize().height + 15));
    _lyTopUI->addChild(sprLevelBox);
    {
        auto lbText = Label::createWithTTF(GAME_TEXTFORMAT("t_ui_advt_relic_crafting_info_title", _nLevel), GAME_FONT, 26);
        lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        lbText->setPosition(sprLevelBox->getContentSize()/2);
        lbText->setAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
        lbText->setDimensions(sprLevelBox->getContentSize().width - 10, sprLevelBox->getContentSize().height - 5);
        sprLevelBox->addChild(lbText);
    }
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _lyTopUI->addChild(menu);
    
    auto btnPlus = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_quantity_up.png"), nullptr, CC_CALLBACK_1(PopupAdventureRelicCraftRateInfo::onClickLevel, this));
    btnPlus->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    btnPlus->setPosition(sprLevelBox->getPosition() + Vec2::UP * sprLevelBox->getContentSize().height/2 + Vec2::RIGHT * (sprLevelBox->getContentSize().width/2 + 15));
    btnPlus->setName("PLUS");
    btnPlus->setTouchRect(btnPlus->getContentSize() * 2.f);
    menu->addChild(btnPlus);
    
    auto btnMinus = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_quantity_down.png"), nullptr, CC_CALLBACK_1(PopupAdventureRelicCraftRateInfo::onClickLevel, this));
    btnMinus->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    btnMinus->setPosition(sprLevelBox->getPosition() + Vec2::UP * sprLevelBox->getContentSize().height/2 + Vec2::LEFT * (sprLevelBox->getContentSize().width/2 + 15));
    btnMinus->setName("MINUS");
    btnMinus->setTouchRect(btnMinus->getContentSize() * 2.f);
    menu->addChild(btnMinus);
}
void PopupAdventureRelicCraftRateInfo::uiCashCore()
{
    _lyTopUI->removeAllChildren();
    
    auto lbText = Label::createWithTTF(GAME_TEXT("t_ui_advt_relic_crafting_info_msg_1"), GAME_FONT, 26);
    lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    lbText->setPosition(Vec2(_lyTopUI->getContentSize()/2));
    lbText->setAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
    lbText->setDimensions(_lyTopUI->getContentSize().width - 10, _lyTopUI->getContentSize().height - 10);
    _lyTopUI->addChild(lbText);
}
void PopupAdventureRelicCraftRateInfo::uiTable()
{
    _lyTableUI->removeAllChildren();
    
    _table = extension::TableView::create(this, Size(_lyTableUI->getContentSize().width, _lyTableUI->getContentSize().height - 10));
    _table->setIgnoreAnchorPointForPosition(false);
    _table->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _table->setPosition(_lyTableUI->getContentSize().width / 2, _lyTableUI->getContentSize().height);
    _table->setDirection(extension::TableView::Direction::VERTICAL);
    _table->setVerticalFillOrder(extension::TableView::VerticalFillOrder::TOP_DOWN);
    _table->setDelegate(this);
    _table->setBounceable(false);
    _lyTableUI->addChild(_table);
    
    _table->reloadData();
}

#pragma mark -utils
void PopupAdventureRelicCraftRateInfo::onClickClose(Ref* sender)
{
    hide();
}

void PopupAdventureRelicCraftRateInfo::onClickLevel(Ref* sender)
{
    auto node = (Node*)sender;
    
    if(node->getName().compare("PLUS") == 0)
    {
        _nLevel++;
        
        if(_nLevel >= getAreaCoreMaxLv())
            _nLevel = getAreaCoreMaxLv();
    }
    else if(node->getName().compare("MINUS") == 0)
    {
        _nLevel--;
        
        if(_nLevel < 1)
            _nLevel = 1;
    }
    _infoRateNow = getInfoRate((int)_eType, _nLevel);
    uiTab();
    _table->reloadData();
}
void PopupAdventureRelicCraftRateInfo::onClickTab(Ref* sender)
{
    auto node = (Node*)sender;
    _eType = (PopupAdventureRelicCraftRateInfo::E_TYPE)node->getTag();
    
    if(_eType == PopupAdventureRelicCraftRateInfo::E_TYPE::CASH_CORE)
        _nLevel = 1;
    else
    {
        auto listCore = AdventureRelicManager::getInstance()->getListInfoCore();
        for(const auto obj : listCore)
        {
            if(obj->getCurrentLv() > _nLevel)
                _nLevel = obj->getCurrentLv();
        }
    }
    
    _infoRateNow = getInfoRate((int)_eType, _nLevel);
    uiTab();
    _table->reloadData();
}
InfoDrawRate* const PopupAdventureRelicCraftRateInfo::getInfoRate(const int nType, const int nLevel)
{
    InfoDrawRate* result = nullptr;
    
    for(auto obj : _listRate)
    {
        if(obj->getType() == nType && obj->getLevel() == nLevel)
        {
            result = obj;
            break;
        }
    }
    
    return result;
}
const int PopupAdventureRelicCraftRateInfo::getAreaCoreMaxLv() const
{
    int result = 0;
    
    for(auto obj : _listRate)
    {
        if(obj->getLevel() > result)
            result = obj->getLevel();
    }
    
    return result;
}
