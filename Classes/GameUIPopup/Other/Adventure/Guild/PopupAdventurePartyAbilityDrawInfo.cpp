//
//  PopupAdventurePartyAbilityDrawInfo.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/04/29.
//

#include "PopupAdventurePartyAbilityDrawInfo.h"

#include "GameObject/Adventure/InfoAdventurePartyAbilityOrigin.h"

#include "GameUI/UI/UIReddot.h"

#include "GameUIPopup/Other/Adventure/Guild/PopupAdventureMemberUpgrade.h"

#include "ManagerGame/AdventureManager.h"

USING_NS_CC;

PopupAdventurePartyAbilityDrawInfo* PopupAdventurePartyAbilityDrawInfo::create()
{
    PopupAdventurePartyAbilityDrawInfo* pRet = new(std::nothrow) PopupAdventurePartyAbilityDrawInfo();
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

PopupAdventurePartyAbilityDrawInfo::PopupAdventurePartyAbilityDrawInfo(void):
_isValueMax(true),
_lyTopUI(nullptr),
_lyTable(nullptr),
_lyBottomUI(nullptr),
_table(nullptr)
{
    _listAbilityOrigin.clear();
}

PopupAdventurePartyAbilityDrawInfo::~PopupAdventurePartyAbilityDrawInfo(void)
{
}


#pragma mark - table
void PopupAdventurePartyAbilityDrawInfo::tableCellTouched(extension::TableView* table, extension::TableViewCell* cell)
{
    
}

Size PopupAdventurePartyAbilityDrawInfo::tableCellSizeForIndex(extension::TableView *table, ssize_t idx)
{
    return Size(table->getContentSize().width, 47);
}

extension::TableViewCell* PopupAdventurePartyAbilityDrawInfo::tableCellAtIndex(extension::TableView *table, ssize_t idx)
{
    auto cell = table->dequeueCell();
    if ( cell == nullptr )
    {
        cell = new extension::TableViewCell();
        cell->autorelease();
    }
    cell->removeAllChildren();
    
    
    auto objAbility = _listAbilityOrigin.at(idx);
    
    // size
    Size size = tableCellSizeForIndex(table, idx);
    cell->setContentSize(size);
    
    std::string strTitleBoxPath = "Assets/ui/adventure/adventure_table_bg1_3.png";
    if((idx + 1) % 2 == 0)
        strTitleBoxPath = "Assets/ui/adventure/adventure_table_bg1_4.png";
    
    auto sprTitleBox = Sprite::create(strTitleBoxPath);
    sprTitleBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    sprTitleBox->setPosition(2, cell->getContentSize().height/2);
    cell->addChild(sprTitleBox);
    
    auto lbAbilityTitle = Label::createWithTTF(GAME_TEXT(MafUtils::format("t_ui_advt_property_type_%d", objAbility->getType())), GAME_FONT, 20);
    lbAbilityTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    lbAbilityTitle->setPosition(5, sprTitleBox->getContentSize().height/2);
    lbAbilityTitle->setDimensions(sprTitleBox->getContentSize().width - 10, sprTitleBox->getContentSize().height - 5);
    lbAbilityTitle->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
    lbAbilityTitle->setOverflow(Label::Overflow::SHRINK);
    sprTitleBox->addChild(lbAbilityTitle);
    
    Vec2 pos = sprTitleBox->getPosition() + Vec2::RIGHT * sprTitleBox->getContentSize().width;
    auto listAbilityGrade = AdventureManager::getInstance()->getListInfoPartyAbilityGradeRate();
    for(int i = 0; i < listAbilityGrade.size(); ++i)
    {
        auto objGrade = listAbilityGrade.at(i);
        
        std::string strBoxPath = "Assets/ui/adventure/adventure_table_bg1_5.png";
        auto sprBox = Sprite::create(strBoxPath);
        sprBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        sprBox->setPosition(pos);
        cell->addChild(sprBox);
        
        pos += Vec2::RIGHT * sprBox->getContentSize().width;
        
        auto objEffect = objAbility->getInfoEffect(objGrade->getGrade());
        if(objEffect == nullptr)
            continue;
        
        double dEffect = objEffect->getEffectMax();
        if(!_isValueMax)
            dEffect = objEffect->getEffectMin();
        
        auto lbEffect = Label::createWithTTF(AdventureManager::getPropertyEffectText(objAbility->getType(), dEffect), GAME_FONT, 22);
        lbEffect->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        lbEffect->setPosition(sprBox->getContentSize()/2);
        lbEffect->setDimensions(sprBox->getContentSize().width - 10, sprBox->getContentSize().height - 5);
        lbEffect->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        lbEffect->setOverflow(Label::Overflow::SHRINK);
        lbEffect->setColor(objGrade->getColor());
        sprBox->addChild(lbEffect);
    }
    
    return cell;
}

ssize_t PopupAdventurePartyAbilityDrawInfo::numberOfCellsInTableView(extension::TableView *table)
{
    return AdventureManager::getInstance()->getListInfoPartyAbilityOriginal().size();
}

#pragma mark -init
bool PopupAdventurePartyAbilityDrawInfo::init()
{
    if ( !PopupBase::init())
    {
        return false;
    }
    
    _listAbilityOrigin = AdventureManager::getInstance()->getListInfoPartyAbilityOriginal();
    initVar();
    initUI();
    if(AdventureManager::getInstance()->getTutorialStep() < 7)
        RefreshManager::getInstance()->addUI(E_REFRESH::ADVENTURE_TUTORIAL_COMPLETE, this);
    
    return true;
}

void PopupAdventurePartyAbilityDrawInfo::onEnter()
{
    PopupBase::onEnter();
    
}
#pragma mark - Override
void PopupAdventurePartyAbilityDrawInfo::refresh(E_REFRESH eType, cocos2d::ValueMap listData)
{
    if(eType == E_REFRESH::ADVENTURE_TUTORIAL_COMPLETE)
        this->setLocalZOrder(ZORDER_SCENE_POPUP);
}
void PopupAdventurePartyAbilityDrawInfo::show()
{
    PopupBase::show();
    
    if(AdventureManager::getInstance()->getTutorialStep() < 7)
        this->setLocalZOrder(ZORDER_SCENE_FORCE_TUTORIAL);
}
void PopupAdventurePartyAbilityDrawInfo::initVar()
{
    _spriteContainer->setContentSize(Size(750, 1010));
}

void PopupAdventurePartyAbilityDrawInfo::initUI()
{
    // title
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_advt_party_ability_info_title_1"), GAME_FONT, 28);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    labelTitle->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5) );
    labelTitle->setDimensions(_spriteContainer->getContentSize().width - 200, labelTitle->getContentSize().height);
    labelTitle->setAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
    labelTitle->setOverflow(cocos2d::Label::Overflow::SHRINK);
    _spriteContainer->addChild(labelTitle);
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupAdventurePartyAbilityDrawInfo::onClickClose, this));
    itemClose->setPosition( Vec2(_spriteContainer->getContentSize().width - itemClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5) );
    itemClose->setTouchRect( itemClose->getContentSize() * 2 );
    menu->addChild(itemClose);
    
    //650
    Size size = _spriteContainer->getContentSize();
    _lyTopUI = LayerColor::create();
    _lyTopUI->setContentSize( Size(size.width - 10, 250));
    _lyTopUI->setIgnoreAnchorPointForPosition(false);
    _lyTopUI->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _lyTopUI->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75));
    _spriteContainer->addChild(_lyTopUI);
    
    _lyTable = LayerColor::create();
    _lyTable->setContentSize( Size(size.width - 10, _spriteContainer->getContentSize().height - 75 - _lyTopUI->getContentSize().height - 200));
    _lyTable->setIgnoreAnchorPointForPosition(false);
    _lyTable->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _lyTable->setPosition( _lyTopUI->getPosition() + Vec2::DOWN * _lyTopUI->getContentSize().height);
    _spriteContainer->addChild(_lyTable);
    
    _lyBottomUI = LayerColor::create();
    _lyBottomUI->setContentSize( Size(_spriteContainer->getContentSize().width - 10, 200));
    _lyBottomUI->setIgnoreAnchorPointForPosition(false);
    _lyBottomUI->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _lyBottomUI->setPosition( _lyTable->getPosition() + Vec2::DOWN * _lyTable->getContentSize().height);
    _spriteContainer->addChild(_lyBottomUI);
    
    uiTop();
    uiTable();
    uiBottom();
}

#pragma mark -ui
void PopupAdventurePartyAbilityDrawInfo::uiTop()
{
    _lyTopUI->removeAllChildren();
    
    
    auto lbText = Label::createWithTTF(GAME_TEXT("t_ui_raidshop_spiritbox_drawrate_pop_title"), GAME_FONT, 24);
    lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    lbText->setPosition(_lyTopUI->getContentSize().width/2, _lyTopUI->getContentSize().height - 7);
    lbText->setAlignment(TextHAlignment::CENTER, TextVAlignment::TOP);
    lbText->setOverflow(Label::Overflow::SHRINK);
    _lyTopUI->addChild(lbText);
    
    auto listAbilityGrade = AdventureManager::getInstance()->getListInfoPartyAbilityGradeRate();
    
    int size = listAbilityGrade.size();
    
    float width = 506 / size;
    float posX = _lyTopUI->getContentSize().width/2 - width * (size/2);
    
    for(int i = 0; i < listAbilityGrade.size(); ++i)
    {
        auto objGrade = listAbilityGrade.at(i);
        
        auto sprBox = ui::Scale9Sprite::create("Assets/ui_common/base_box_1_10.png");
        sprBox->setContentSize(Size(width, 82));
        sprBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        sprBox->setPosition(Vec2(posX, lbText->getPosition().y - lbText->getContentSize().height - 5));
        _lyTopUI->addChild(sprBox);
        
        auto lbGrade = Label::createWithTTF( GAME_TEXT(MafUtils::format("t_ui_advt_party_ability_grade_%d", objGrade->getGrade())), GAME_FONT, 24);
        lbGrade->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        lbGrade->setPosition(sprBox->getContentSize().width/2, sprBox->getContentSize().height/2 + 5);
        lbGrade->setDimensions(sprBox->getContentSize().width-5, sprBox->getContentSize().height-55);
        lbGrade->setAlignment(TextHAlignment::CENTER, TextVAlignment::BOTTOM);
        lbGrade->setOverflow(Label::Overflow::SHRINK);
        lbGrade->setColor(objGrade->getColor());
        sprBox->addChild(lbGrade);
        
        auto lbRate = Label::createWithTTF(MafUtils::format("%.1f%%", objGrade->getRate()*0.1f), GAME_FONT, 24);
        lbRate->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        lbRate->setPosition(sprBox->getContentSize().width/2, sprBox->getContentSize().height/2 - 5);
        lbRate->setDimensions(sprBox->getContentSize().width-5, sprBox->getContentSize().height-5);
        lbRate->setAlignment(TextHAlignment::CENTER, TextVAlignment::TOP);
        lbRate->setOverflow(Label::Overflow::SHRINK);
        lbRate->setColor(objGrade->getColor());
        sprBox->addChild(lbRate);
        
        posX += width ;
    }
    
    
    auto sprTableTitleBg = ui::Scale9Sprite::create("Assets/ui/adventure/adventure_table_bg1_1.png");
    sprTableTitleBg->setContentSize(Size(266,74));
    sprTableTitleBg->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    sprTableTitleBg->setPosition(2, 0);
    
    Vec2 pos = sprTableTitleBg->getPosition() + Vec2::RIGHT * sprTableTitleBg->getContentSize().width;
    
    _lyTopUI->addChild(sprTableTitleBg);
    {
        std::string strText = "MAX";
        if(!_isValueMax)
            strText = "MIN";
        
        auto lbText = Label::createWithTTF( GAME_TEXTFORMAT("t_ui_advt_party_avility_info_1", strText.c_str()), GAME_FONT, 24);
        lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        lbText->setPosition(sprTableTitleBg->getContentSize()/2);
        lbText->setDimensions(sprTableTitleBg->getContentSize().width - 10, lbText->getContentSize().height);
        lbText->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        lbText->setOverflow(Label::Overflow::SHRINK);
        sprTableTitleBg->addChild(lbText);
    }
    
    for(int i = 0; i < listAbilityGrade.size(); ++i)
    {
        auto objGrade = listAbilityGrade.at(i);
        
        std::string strBoxPath = "Assets/ui/adventure/adventure_table_bg1_2.png";
        if((i + 1) % 2 == 0)
            strBoxPath = "Assets/ui/adventure/adventure_table_bg1_1.png";
        
        auto sprBox = Sprite::create(strBoxPath);
        sprBox->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        sprBox->setPosition(pos);
        _lyTopUI->addChild(sprBox);
        
        pos += Vec2::RIGHT * sprBox->getContentSize().width;
        
        auto lbGrade = Label::createWithTTF( GAME_TEXT(MafUtils::format("t_ui_advt_party_ability_grade_%d", objGrade->getGrade())), GAME_FONT, 24);
        lbGrade->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        lbGrade->setPosition(sprBox->getContentSize()/2);
        lbGrade->setDimensions(sprBox->getContentSize().width - 10, lbGrade->getContentSize().height);
        lbGrade->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        lbGrade->setOverflow(Label::Overflow::SHRINK);
        lbGrade->setColor(objGrade->getColor());
        sprBox->addChild(lbGrade);
    }
    
}
void PopupAdventurePartyAbilityDrawInfo::uiTable()
{
    _lyTable->removeAllChildren();
    
    _table = extension::TableView::create(this, Size(_lyTable->getContentSize().width, _lyTable->getContentSize().height - 5));
    _table->setIgnoreAnchorPointForPosition(false);
    _table->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _table->setPosition(_lyTable->getContentSize().width / 2, 0);
    _table->setDirection(extension::TableView::Direction::VERTICAL);
    _table->setVerticalFillOrder(extension::TableView::VerticalFillOrder::TOP_DOWN);
    _table->setDelegate(this);
    _table->setBounceable(false);
    _lyTable->addChild(_table);
    
    _table->reloadData();
}

void PopupAdventurePartyAbilityDrawInfo::uiBottom()
{
    _lyBottomUI->removeAllChildren();
    
    auto btnSwitch = ui::Button::create("Assets/ui/adventure/adventure_table__btn1_2.png");
    btnSwitch->addClickEventListener(CC_CALLBACK_1(PopupAdventurePartyAbilityDrawInfo::onClickSwitching, this));
    btnSwitch->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    btnSwitch->setZoomScale(0.05f);
    btnSwitch->setPosition(Vec2(_lyBottomUI->getContentSize().width - 10, _lyBottomUI->getContentSize().height - 10));
    _lyBottomUI->addChild(btnSwitch);
    
    auto lbInfoText = Label::createWithTTF(GAME_TEXT("t_ui_advt_party_ability_info_mag_1"), GAME_FONT, 20);
    lbInfoText->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    lbInfoText->setPosition(20, btnSwitch->getPosition().y - btnSwitch->getContentSize().height - 5);
    lbInfoText->setDimensions(_lyBottomUI->getContentSize().width - 40, _lyBottomUI->getContentSize().height - btnSwitch->getContentSize().height - 15);
    lbInfoText->setAlignment(TextHAlignment::LEFT, TextVAlignment::TOP);
    lbInfoText->setOverflow(Label::Overflow::SHRINK);
    _lyBottomUI->addChild(lbInfoText);
}

#pragma mark -click
void PopupAdventurePartyAbilityDrawInfo::onClickClose(Ref* const pSender)
{
    hide();
}

void PopupAdventurePartyAbilityDrawInfo::onClickSwitching(Ref* const pSender)
{
    auto btnSwitch = (ui::Button * )pSender;
    
    _isValueMax = !_isValueMax;
    
    if(btnSwitch == nullptr)
        return;
    
    std::string strPath = "Assets/ui/adventure/adventure_table__btn1_2.png";
    if(!_isValueMax)
        strPath = "Assets/ui/adventure/adventure_table__btn1_1.png";
    
    btnSwitch->loadTextureNormal(strPath);
    
    uiTop();
    _table->reloadData();
}
