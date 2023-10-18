//
//  PopupSpiritSummonDrawInfo.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/12/07.
//

#include "PopupSpiritSummonDrawInfo.h"

#include "ManagerGame/WeaponSpiritManager.h"

USING_NS_CC;

PopupSpiritSummonDrawInfo* PopupSpiritSummonDrawInfo::create()
{
    PopupSpiritSummonDrawInfo* pRet = new(std::nothrow) PopupSpiritSummonDrawInfo();
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

PopupSpiritSummonDrawInfo::PopupSpiritSummonDrawInfo(void):
_nLevel(0),
_lyTopUI(nullptr),
_lyMiddleUI(nullptr),
_lyBottomUI(nullptr),
_table(nullptr)
{
    _listRate.clear();
}

PopupSpiritSummonDrawInfo::~PopupSpiritSummonDrawInfo(void)
{
    _listRate.clear();
}


#pragma mark - table
void PopupSpiritSummonDrawInfo::tableCellTouched(extension::TableView* table, extension::TableViewCell* cell)
{
    
}

Size PopupSpiritSummonDrawInfo::tableCellSizeForIndex(extension::TableView *table, ssize_t idx)
{
    return Size(table->getContentSize().width, 70);
}

extension::TableViewCell* PopupSpiritSummonDrawInfo::tableCellAtIndex(extension::TableView *table, ssize_t idx)
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
        auto lbText = Label::createWithTTF(GAME_TEXT("t_ui_spirit_grade_" + std::to_string(obj.first)), GAME_FONT, 26);
        lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        lbText->setPosition(20, sprGradeBox->getContentSize().height/2);
        lbText->setAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
        lbText->setDimensions(sprGradeBox->getContentSize().width - 30, sprGradeBox->getContentSize().height - 5);
        lbText->setColor(COLOR_KEY);
        sprGradeBox->addChild(lbText);
        
    }
    
    auto sprRateBox = ui::Scale9Sprite::create("Assets/ui_common/base_box_1_9.png");
    sprRateBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    sprRateBox->setContentSize(Size(214,44));
    sprRateBox->setPosition(sprGradeBox->getPosition() + Vec2::RIGHT * (sprGradeBox->getContentSize().width + 10));
    cell->addChild(sprRateBox);
    {
        float rate = obj.second * 0.001f;
        
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

ssize_t PopupSpiritSummonDrawInfo::numberOfCellsInTableView(extension::TableView *table)
{
    int size = 0;
    
    if(_infoRateNow != nullptr)
        size = _infoRateNow->getListRate().size();
    
    return size;
}

#pragma mark -init
bool PopupSpiritSummonDrawInfo::init()
{
    if ( !PopupBase::init())
    {
        return false;
    }
    _nLevel = WeaponSpiritManager::getInstance()->getCurrentMileageLv();
    
    loadData();
    
    _infoRateNow = getInfoRate(_nLevel);
    
    initVar();
    initUI();
    
    return true;
}
void PopupSpiritSummonDrawInfo::loadData()
{
    std::string strText = TableManager::getInstance()->getTableData(E_TABLE::WEAPON_SPIRIT_DRAW_RATE);
    
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
        
        if ( MafNative::IsLessThanCurrVersion(jsonValue["version"].GetString()) == false || jsonValue["type"].GetInt() != 0)
        {
            continue;
        }
        
        int nIdx = jsonValue["idx"].GetInt();
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
        obj->setLevel(nIdx);
        obj->setListRate(listRate);
        _listRate.pushBack(obj);
    }
    
}

void PopupSpiritSummonDrawInfo::initVar()
{
    _spriteContainer->setContentSize(Size(672, 1034));
}

void PopupSpiritSummonDrawInfo::initUI()
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
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupSpiritSummonDrawInfo::onClickClose, this));
    itemClose->setPosition( Vec2(_spriteContainer->getContentSize().width - itemClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5) );
    itemClose->setTouchRect( itemClose->getContentSize() * 2 );
    menu->addChild(itemClose);
    
    //650
    Size size = _spriteContainer->getContentSize();
    
    _lyTopUI = LayerColor::create();
    _lyTopUI->setContentSize( Size(size.width - 10, 230));
    _lyTopUI->setIgnoreAnchorPointForPosition(false);
    _lyTopUI->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _lyTopUI->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75));
    _spriteContainer->addChild(_lyTopUI);
    
    _lyMiddleUI = LayerColor::create();
    _lyMiddleUI->setContentSize( Size(size.width - 10, 570));
    _lyMiddleUI->setIgnoreAnchorPointForPosition(false);
    _lyMiddleUI->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _lyMiddleUI->setPosition(_lyTopUI->getPosition() + Vec2::DOWN * _lyTopUI->getContentSize().height);
    _spriteContainer->addChild(_lyMiddleUI);
    
    _lyBottomUI = LayerColor::create();
    _lyBottomUI->setContentSize( Size(size.width - 10, size.height - 75 - 10 - _lyTopUI->getContentSize().height - _lyMiddleUI->getContentSize().height));
    _lyBottomUI->setIgnoreAnchorPointForPosition(false);
    _lyBottomUI->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _lyBottomUI->setPosition(_lyMiddleUI->getPosition() + Vec2::DOWN * _lyMiddleUI->getContentSize().height);
    _spriteContainer->addChild(_lyBottomUI);
    
    uiTop();
    uiMiddle();
    uiBottom();
}

#pragma mark -ui
void PopupSpiritSummonDrawInfo::uiTop()
{
    _lyTopUI->removeAllChildren();
    
    auto lbText = Label::createWithTTF(GAME_TEXT("t_ui_raidshop_spiritbox_drawrate_pop_msg"), GAME_FONT, 26);
    lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    lbText->setPosition(Vec2(_lyTopUI->getContentSize().width/2, _lyTopUI->getContentSize().height - 30));
    lbText->setAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::TOP);
    lbText->setDimensions(_lyTopUI->getContentSize().width - 30, 80);
    _lyTopUI->addChild(lbText);
    
    auto sprLevelBox = ui::Scale9Sprite::create("Assets/ui_common/box_02.png");
    sprLevelBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    sprLevelBox->setContentSize(Size(340,58));
    sprLevelBox->setPosition(Vec2(_lyTopUI->getContentSize().width/2, 30));
    _lyTopUI->addChild(sprLevelBox);
    {
        auto lbText = Label::createWithTTF(GAME_TEXT("t_ui_spirit_draw_lv_1") + " " +  std::to_string(_nLevel), GAME_FONT, 26);
        lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        lbText->setPosition(sprLevelBox->getContentSize()/2);
        lbText->setAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
        lbText->setDimensions(sprLevelBox->getContentSize().width - 10, sprLevelBox->getContentSize().height - 5);
        sprLevelBox->addChild(lbText);
    }
    
    auto btnPlus = ui::Button::create("Assets/ui_common/btn_quantity_up.png");
    btnPlus->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    btnPlus->setPosition(sprLevelBox->getPosition() + Vec2::UP * sprLevelBox->getContentSize().height/2 + Vec2::RIGHT * (sprLevelBox->getContentSize().width/2 + 15));
    btnPlus->setZoomScale(0.05f);
    btnPlus->setName("PLUS");
    btnPlus->addClickEventListener(CC_CALLBACK_1(PopupSpiritSummonDrawInfo::onClickLevel, this));
    _lyTopUI->addChild(btnPlus);
    
    auto btnMinus = ui::Button::create("Assets/ui_common/btn_quantity_down.png");
    btnMinus->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    btnMinus->setPosition(sprLevelBox->getPosition() + Vec2::UP * sprLevelBox->getContentSize().height/2 + Vec2::LEFT * (sprLevelBox->getContentSize().width/2 + 15));
    btnMinus->setZoomScale(0.05f);
    btnMinus->setName("MINUS");
    btnMinus->addClickEventListener(CC_CALLBACK_1(PopupSpiritSummonDrawInfo::onClickLevel, this));
    _lyTopUI->addChild(btnMinus);
}

void PopupSpiritSummonDrawInfo::uiMiddle()
{
    _lyMiddleUI->removeAllChildren();
    
    auto sprBox = ui::Scale9Sprite::create("Assets/ui_common/base_box_1_6.png");
    sprBox->setContentSize(Size(616, 44));
    sprBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    sprBox->setPosition(_lyMiddleUI->getContentSize().width/2, _lyMiddleUI->getContentSize().height);
    _lyMiddleUI->addChild(sprBox);
    {
        auto lbText = Label::createWithTTF(GAME_TEXT("t_ui_spirit_draw_lv_2") + " " +  std::to_string(WeaponSpiritManager::getInstance()->getCurrentMileageLv()), GAME_FONT, 26);
        lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        lbText->setPosition(20, sprBox->getContentSize().height/2);
        lbText->setAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
        lbText->setDimensions(sprBox->getContentSize().width - 30, sprBox->getContentSize().height - 5);
        lbText->setColor(COLOR_KEY);
        sprBox->addChild(lbText);
        
    }
    
    _table = extension::TableView::create(this, Size(_lyMiddleUI->getContentSize().width, _lyMiddleUI->getContentSize().height - 50));
    _table->setIgnoreAnchorPointForPosition(false);
    _table->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _table->setPosition(_lyMiddleUI->getContentSize().width / 2, 0);
    _table->setDirection(extension::TableView::Direction::VERTICAL);
    _table->setVerticalFillOrder(extension::TableView::VerticalFillOrder::TOP_DOWN);
    _table->setDelegate(this);
    _table->setBounceable(false);
    _lyMiddleUI->addChild(_table);
    
    _table->reloadData();
}

void PopupSpiritSummonDrawInfo::uiBottom()
{
    _lyBottomUI->removeAllChildren();
    
    auto sprGradeBox = ui::Scale9Sprite::create("Assets/ui_common/base_box_1_2.png");
    sprGradeBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    sprGradeBox->setContentSize(Size(616,44));
    sprGradeBox->setPosition(_lyBottomUI->getContentSize().width/2, _lyBottomUI->getContentSize().height - 10);
    _lyBottomUI->addChild(sprGradeBox);
    
    auto sprPerBox = ui::Scale9Sprite::create("Assets/ui_common/base_box_1_3.png");
    sprPerBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    sprPerBox->setContentSize(Size(616,44));
    sprPerBox->setPosition(sprGradeBox->getPosition() + Vec2::DOWN * sprGradeBox->getContentSize().height);
    _lyBottomUI->addChild(sprPerBox);
    
    std::vector<std::pair<std::string, std::string>> list = {
        std::make_pair(GAME_TEXTFORMAT("t_ui_equip_msg_5", 4), "40%"),
        std::make_pair(GAME_TEXTFORMAT("t_ui_equip_msg_5", 3), "30%"),
        std::make_pair(GAME_TEXTFORMAT("t_ui_equip_msg_5", 2), "20%"),
        std::make_pair(GAME_TEXTFORMAT("t_ui_equip_msg_5", 1), "10%"),
    };
    
    for(int i = 0; i < list.size(); ++i)
    {
        auto obj = list[i];
        
        auto lbGrade = Label::createWithTTF(obj.first, GAME_FONT, 24);
        lbGrade->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        lbGrade->setPosition(80 + sprGradeBox->getContentSize().width/4 * i ,sprGradeBox->getContentSize().height/2);
        lbGrade->setAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
        lbGrade->setDimensions(sprGradeBox->getContentSize().width/4, sprGradeBox->getContentSize().height/2);
        sprGradeBox->addChild(lbGrade);
        
        auto lbPer = Label::createWithTTF(obj.second, GAME_FONT, 24);
        lbPer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        lbPer->setPosition(80 + sprPerBox->getContentSize().width/4 * i ,sprPerBox->getContentSize().height/2);
        lbPer->setAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
        lbPer->setDimensions(sprPerBox->getContentSize().width/4, sprPerBox->getContentSize().height/2);
        sprPerBox->addChild(lbPer);
    }
}

#pragma mark -utils
void PopupSpiritSummonDrawInfo::onClickClose(Ref* sender)
{
    hide();
}

void PopupSpiritSummonDrawInfo::onClickLevel(Ref* sender)
{
    auto node = (Node*)sender;
    
    
    if(node->getName().compare("PLUS") == 0)
    {
        _nLevel++;
        
        if(_nLevel >= _listRate.size())
            _nLevel = _listRate.size();
    }
    else if(node->getName().compare("MINUS") == 0)
    {
        _nLevel--;
        
        if(_nLevel < 1)
            _nLevel = 1;
    }
    _infoRateNow = getInfoRate(_nLevel);
    uiTop();
    _table->reloadData();
}


InfoDrawRate* PopupSpiritSummonDrawInfo::getInfoRate(int value)
{
    InfoDrawRate* result = nullptr;
    
    for(auto obj : _listRate)
    {
        if(obj->getLevel() == value)
        {
            result = obj;
            break;
        }
    }
    
    return result;
}
