//
//  PopupDefenseTrenchSelect.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2023/08/01.
//

#include "PopupDefenseTrenchSelect.h"

#include "GameObject/Defense/InfoDefenseTrench.h"
#include "GameObject/Defense/InfoDefenseWeapon.h"
#include "GameObject/Defense/InfoDefenseCharacter.h"


#include "GameUI/UI/UIReddot.h"

#include "GameUIPopup/Common/PopupHelp.h"
#include "GameUIPopup/Other/Ads/PopupAdsMafGames.h"

#include "ManagerGame/UserInfoManager.h"
#include "ManagerGame/DialogManager.h"
#include "ManagerGame/CashManager.h"
#include "ManagerGame/DefenseManager.h"


USING_NS_CC;


PopupDefenseTrenchSelect* PopupDefenseTrenchSelect::create(E_DF_Trench_SettingType type, int trenchNumber)
{
    PopupDefenseTrenchSelect* pRet = new(std::nothrow) PopupDefenseTrenchSelect();
    if ( pRet && pRet->init(type, trenchNumber) )
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

PopupDefenseTrenchSelect::PopupDefenseTrenchSelect(void):
_type(E_DF_Trench_SettingType::NONE),
_trenchNumber(0),
_lyTable(nullptr),
_lyBottom(nullptr),
_table(nullptr)
{
}

PopupDefenseTrenchSelect::~PopupDefenseTrenchSelect(void)
{
    DefenseManager::getInstance()->_onServerEquip.Clear(this);
    DefenseManager::getInstance()->_onUpdateCharacter.Clear(this);
    DefenseManager::getInstance()->_onUpdateWeapon.Clear(this);
}

#pragma mark - table
void PopupDefenseTrenchSelect::tableCellTouched(extension::TableView* table, extension::TableViewCell* cell)
{
    
}

Size PopupDefenseTrenchSelect::tableCellSizeForIndex(extension::TableView *table, ssize_t idx)
{
    return Size(table->getContentSize().width, 150);
}

extension::TableViewCell* PopupDefenseTrenchSelect::tableCellAtIndex(extension::TableView *table, ssize_t idx)
{
    auto cell = table->dequeueCell();
    if ( cell == nullptr )
    {
        cell = new extension::TableViewCell();
        cell->autorelease();
    }
    cell->removeAllChildren();
    
    // size
    Size size = tableCellSizeForIndex(table, idx);
    cell->setContentSize(size);
    
    createCell(cell, (int)idx);
//    switch(_type)
//    {
//        case E_DF_Trench_SettingType::CHARACTER: createCellbyCharacter(cell, (int)idx); break;
//        case E_DF_Trench_SettingType::WEAPON: createCellbyWeapon(cell, (int)idx); break;
//        default: break;
//    }
    
    return cell;
}

ssize_t PopupDefenseTrenchSelect::numberOfCellsInTableView(extension::TableView *table)
{
    ssize_t size = 0;
    
    switch(_type)
    {
        case E_DF_Trench_SettingType::CHARACTER: size = DefenseManager::getInstance()->getCharacterDataCount(); break;
        case E_DF_Trench_SettingType::WEAPON: size = DefenseManager::getInstance()->getWeaponDataCount();  break;
        default: break;
    }
    
    return size / 5;
}


#pragma mark -init
bool PopupDefenseTrenchSelect::init(E_DF_Trench_SettingType type, int trenchNumber)
{
    if ( !PopupBase::init())
    {
        return false;
    }
    _type = type;
    _trenchNumber = trenchNumber;
    initVar();
    initUI();
    
    DefenseManager::getInstance()->_onUpdateCharacter += new MafFunction<void(void)>(CC_CALLBACK_0(PopupDefenseTrenchSelect::uiRefresh, this), this);
    DefenseManager::getInstance()->_onServerEquip += new MafFunction<void(bool, int)>(CC_CALLBACK_2(PopupDefenseTrenchSelect::onServerEquip, this), this);
    DefenseManager::getInstance()->_onUpdateWeapon += new MafFunction<void(void)>(CC_CALLBACK_0(PopupDefenseTrenchSelect::uiRefresh, this), this);
    
    return true;
}

void PopupDefenseTrenchSelect::initVar()
{
    _spriteContainer->setContentSize(Size(740, 1000));
}

void PopupDefenseTrenchSelect::initUI()
{
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupDefenseTrenchSelect::onClickClose, this));
    itemClose->setPosition( Vec2(_spriteContainer->getContentSize().width - itemClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5) );
    itemClose->setTouchRect( itemClose->getContentSize() * 2 );
    menu->addChild(itemClose);
    
    // UI
    _lyTop = LayerColor::create();
    _lyTop->setContentSize(Size(_spriteContainer->getContentSize().width, 75));
    _lyTop->setIgnoreAnchorPointForPosition(false);
    _lyTop->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _lyTop->setPosition(_spriteContainer->getContentSize().width/2, _spriteContainer->getContentSize().height);
    _spriteContainer->addChild(_lyTop);
    
    _lyTable = LayerColor::create();
    _lyTable->setContentSize(Size(_spriteContainer->getContentSize().width, _spriteContainer->getContentSize().height - 75 - 10));
    _lyTable->setIgnoreAnchorPointForPosition(false);
    _lyTable->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _lyTable->setPosition(_spriteContainer->getContentSize().width/2, _spriteContainer->getContentSize().height - 75);
    _spriteContainer->addChild(_lyTable);
    
    _lyBottom = LayerColor::create();
    _lyBottom->setContentSize(Size(_spriteContainer->getContentSize().width, 10));
    _lyBottom->setIgnoreAnchorPointForPosition(false);
    _lyBottom->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _lyBottom->setPosition(_spriteContainer->getContentSize().width/2, 0);
    _spriteContainer->addChild(_lyBottom);
    
    uiTop();
    uiTable();
    uiBottom();
}

#pragma mark -ui
void PopupDefenseTrenchSelect::uiTop()
{
    _lyTop->removeAllChildren();
    
    auto sprBox = ui::Scale9Sprite::create("Assets/ui_common/base_box_1_15.png");
    sprBox->setContentSize(Size(280, 48));
    sprBox->setPosition(_lyTop->getContentSize()/2);
    _lyTop->addChild(sprBox);
    
    auto lbTitle = Label::createWithTTF(MafUtils::format("%d", _trenchNumber), GAME_FONT, 28);
    lbTitle->setPosition(sprBox->getContentSize()/2);
    lbTitle->setAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
    lbTitle->setOverflow(cocos2d::Label::Overflow::SHRINK);
    lbTitle->setDimensions(sprBox->getContentSize().width - 10 , sprBox->getContentSize().height - 5);
    sprBox->addChild(lbTitle);
    
    
    auto btnLeft = ui::Button::create("Assets/ui_common/btn_quantity_down.png");
    btnLeft->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    btnLeft->setPosition(sprBox->getPosition() + Vec2::LEFT * (sprBox->getContentSize().width/2 + 5));
    btnLeft->addClickEventListener(CC_CALLBACK_1(PopupDefenseTrenchSelect::onClickChangeTrench, this));
    btnLeft->setTag(-1);
    btnLeft->setZoomScale(0.05f);
    _lyTop->addChild(btnLeft);
    
    auto btnRight = ui::Button::create("Assets/ui_common/btn_quantity_up.png");
    btnRight->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    btnRight->setPosition(sprBox->getPosition() + Vec2::RIGHT * (sprBox->getContentSize().width/2 + 5));
    btnRight->addClickEventListener(CC_CALLBACK_1(PopupDefenseTrenchSelect::onClickChangeTrench, this));
    btnRight->setTag(1);
    btnRight->setZoomScale(0.05f);
    _lyTop->addChild(btnRight);
}
void PopupDefenseTrenchSelect::uiTable()
{
    _lyTable->removeAllChildren();
    
    auto lyEffectInfo = LayerColor::create();
    lyEffectInfo->setContentSize( Size(_lyTable->getContentSize().width, _lyTable->getContentSize().height - 10));
    lyEffectInfo->setIgnoreAnchorPointForPosition(false);
    lyEffectInfo->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    lyEffectInfo->setPosition( Vec2(_lyTable->getContentSize().width / 2, 20));
    _lyTable->addChild(lyEffectInfo);
    
    _table = extension::TableView::create(this, Size(lyEffectInfo->getContentSize().width, lyEffectInfo->getContentSize().height - 10));
    _table->setIgnoreAnchorPointForPosition(false);
    _table->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _table->setPosition(lyEffectInfo->getContentSize().width / 2, 0);
    _table->setDirection(extension::TableView::Direction::VERTICAL);
    _table->setVerticalFillOrder(extension::TableView::VerticalFillOrder::TOP_DOWN);
    _table->setDelegate(this);
    _table->setBounceable(false);
    lyEffectInfo->addChild(_table);
    _table->reloadData();
}
void PopupDefenseTrenchSelect::uiBottom()
{
    _lyBottom->removeAllChildren();
    
    std::string weaponTabPath = "Assets/ui_common/tab_btn_bottom_off.png";
    std::string characterTabPath = "Assets/ui_common/tab_btn_bottom_off.png";
    
    switch(_type)
    {
        case E_DF_Trench_SettingType::CHARACTER: characterTabPath = "Assets/ui_common/tab_btn_bottom_on.png"; break;
        case E_DF_Trench_SettingType::WEAPON: weaponTabPath = "Assets/ui_common/tab_btn_bottom_on.png"; break;
    }
    
    auto btnCharacter = ui::Button::create(characterTabPath);
    btnCharacter->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    btnCharacter->setPosition(Vec2(_lyBottom->getContentSize().width/2 - 10, 5));
    btnCharacter->setTag((int)E_DF_Trench_SettingType::CHARACTER);
    btnCharacter->addClickEventListener(CC_CALLBACK_1(PopupDefenseTrenchSelect::onClickTab, this));
    btnCharacter->setTitleText(GAME_TEXT("t_ui_defense_squad_member"));
    btnCharacter->setTitleAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
    btnCharacter->getTitleLabel()->setDimensions(btnCharacter->getContentSize().width, btnCharacter->getContentSize().height);
    btnCharacter->setTitleFontSize(30);
    btnCharacter->setZoomScale(0.05f);
    _lyBottom->addChild(btnCharacter);
    
    auto btnWeapon = ui::Button::create(weaponTabPath);
    btnWeapon->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    btnWeapon->setPosition(Vec2(_lyBottom->getContentSize().width/2 + 10, 5));
    btnWeapon->setTag((int)E_DF_Trench_SettingType::WEAPON);
    btnWeapon->addClickEventListener(CC_CALLBACK_1(PopupDefenseTrenchSelect::onClickTab, this));
    btnWeapon->setTitleText(GAME_TEXT("t_ui_defense_squad_weapon"));
    btnWeapon->setTitleAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
    btnWeapon->getTitleLabel()->setDimensions(btnWeapon->getContentSize().width, btnWeapon->getContentSize().height);
    btnWeapon->setTitleFontSize(30);
    btnWeapon->setZoomScale(0.05f);
    _lyBottom->addChild(btnWeapon);
}
void PopupDefenseTrenchSelect::uiRefresh()
{
    uiTop();
    uiBottom();
    Vec2 pos = _table->getContentOffset();
    _table->reloadData();
    _table->setContentOffset(pos);
}
#pragma mark -utils
void PopupDefenseTrenchSelect::onClickClose(Ref* sender)
{
    hide();
}
void PopupDefenseTrenchSelect::onClickTab(Ref* sender)
{
    auto node = (Node*)sender;
    
    _type = (E_DF_Trench_SettingType)node->getTag();
    
    uiRefresh();
}
void PopupDefenseTrenchSelect::onClickSelect(Ref* sender)
{
    auto node = (Node*)sender;
    
    bool isHave = false;
    int idx = node->getTag();
    
    switch(_type)
    {
        case E_DF_Trench_SettingType::CHARACTER:
        {
            if(auto data = DefenseManager::getInstance()->getCharacterData(idx))
                isHave = data->isHave();
        }break;
        case E_DF_Trench_SettingType::WEAPON:
        {
            if(auto data = DefenseManager::getInstance()->getWeaponData(idx))
                isHave = data->isHave();
        }break;
        default: break;
    }
    if(!isHave)
        return;
    
    DefenseManager::getInstance()->requestEquip(_type, idx, _trenchNumber);
}
void PopupDefenseTrenchSelect::onClickChangeTrench(Ref* sender)
{
    auto node = (Node*)sender;
    _trenchNumber += node->getTag();
    if(_trenchNumber > DefenseManager::getInstance()->getMaxTrenchCount())
        _trenchNumber = 1;
    else if(_trenchNumber <= 0)
        _trenchNumber = DefenseManager::getInstance()->getMaxTrenchCount();
    
    uiRefresh();
}
#pragma mark -ui
void PopupDefenseTrenchSelect::createCell(Node* cell, const int cellNumber)
{
    if(cell == nullptr)
        return;
    
    auto menu = MafNode::MafMenu::create();
    menu->setPosition(Vec2::ZERO);
    cell->addChild(menu);
    
    Vec2 location = Vec2(10, cell->getContentSize().height/2);
    for(int i = 0; i < 5; ++i)
    {
        const int idx = i + 1 + cellNumber * 5;
        
        auto objCharacter = DefenseManager::getInstance()->getCharacterData(idx);
        auto objWeapon = DefenseManager::getInstance()->getWeaponData(idx);
        
        auto layerBG = Layer::create();
        layerBG->setContentSize(Size(138, 141));
        
        auto itemBG = MafNode::MafMenuItemSprite::create(layerBG, nullptr, CC_CALLBACK_1(PopupDefenseTrenchSelect::onClickSelect, this));
        itemBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        itemBG->setPosition(location);
        itemBG->setTag(idx);
        menu->addChild(itemBG);
        
        //
        std::string iconPath = "";
        int grade = 1;
        bool isHave = false;
        std::string name = "";
        
        switch(_type)
        {
            case E_DF_Trench_SettingType::CHARACTER:
            {
                iconPath = MafUtils::format("Assets/ui/defense/Defense_mole_lv%d_00.png", objCharacter->getIndex());
                grade = objCharacter->getGrade();
                isHave = objCharacter->isHave();
                name = objCharacter->getName();
            }break;
            case E_DF_Trench_SettingType::WEAPON:
            {
                iconPath = MafUtils::format("Assets/ui/defense/Icon_gun_lv%d.png", objWeapon->getIndex());
                grade = objWeapon->getGrade();
                isHave = objWeapon->isHave();
                name = objWeapon->getName();
            }break;
            default: break;
        }
        
        std::string bgPath = MafUtils::format("Assets/ui/defense/Ui_list_mini_box%02d.png", grade);
        
        auto sprBG = Sprite::create(bgPath);
        sprBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        sprBG->setPosition(layerBG->getContentSize() / 2);
        layerBG->addChild(sprBG);
      
        auto lyIcon = LayerColor::create(Color4B::RED);
        lyIcon->setIgnoreAnchorPointForPosition(false);
        lyIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        lyIcon->setContentSize(sprBG->getContentSize() - Size(20));
        
        auto clipper = ClippingNode::create(lyIcon);
        clipper->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        clipper->setPosition(sprBG->getContentSize()/2);
        clipper->setIgnoreAnchorPointForPosition(false);
        sprBG->addChild(clipper);
        
        auto sprIcon = Sprite::create(iconPath);
        sprIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        sprIcon->setPosition(clipper->getContentSize()/2);
        sprIcon->getTexture()->setTexParameters(texParams);
        clipper->addChild(sprIcon);
        if(_type == E_DF_Trench_SettingType::CHARACTER)
            sprIcon->setPosition(sprIcon->getPosition() + Vec2::UP * 20);
        
        auto sprName = Sprite::create("Assets/ui/defense/Ui_list_mini_namebox.png");
        sprName->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        sprName->setPosition(sprBG->getContentSize().width/2, 3);
        sprBG->addChild(sprName);
        
        auto lbName = Label::createWithTTF(name, GAME_FONT, 22);
        lbName->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        lbName->setPosition(sprName->getContentSize()/2);
        lbName->setDimensions(sprName->getContentSize().width - 10, lbName->getContentSize().height);
        lbName->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        lbName->setOverflow(Label::Overflow::SHRINK);
        sprName->addChild(lbName);
        
        if(!isHave)
        {
            sprIcon->setColor(Color3B(15, 15, 15));
            
            auto sprBlank = Sprite::create("Assets/ui/defense/Ui_list_mini_box.png");
            sprBlank->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            sprBlank->setPosition(sprBG->getContentSize()/2);
            sprBG->addChild(sprBlank);
        }
        
        int trenchNumber = 0;
        bool isEquip = false;
        switch(_type)
        {
            case E_DF_Trench_SettingType::CHARACTER: isEquip = DefenseManager::getInstance()->isEquipCharacter(objCharacter, trenchNumber); break;
            case E_DF_Trench_SettingType::WEAPON: isEquip = DefenseManager::getInstance()->isEquipWeapon(objWeapon, trenchNumber); break;
            default: break;
        }
        if(isEquip)
        {
            std::string framePath = "Assets/ui/defense/Ui_list_mini_box_equip.png";
            if(_trenchNumber == trenchNumber)
                framePath = "Assets/ui/defense/Ui_list_mini_box_select.png";
            
            auto sprFrame = Sprite::create(framePath);
            sprFrame->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            sprFrame->setPosition(layerBG->getContentSize() / 2);
            sprFrame->getTexture()->setTexParameters(texParams);
            sprBG->addChild(sprFrame);
            
            //18,16
            auto lbTrenchNum = MafNode::createBmFont(MafUtils::format("%d", trenchNumber), GAME_BM_FONT_AL, 22);
            lbTrenchNum->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            lbTrenchNum->setPosition(Vec2(18,sprFrame->getContentSize().height - 16));
            lbTrenchNum->setColor(Color3B(255,220,110));
            if(_trenchNumber == trenchNumber)
                lbTrenchNum->setColor(Color3B(85,240,255));
            sprFrame->addChild(lbTrenchNum);
        }
        
        
        location += Vec2::RIGHT * 145;
    }
}

#pragma mark -callback
void PopupDefenseTrenchSelect::onServerEquip(bool bResult, int nResult)
{
    if(!bResult || nResult != 1)
    {
        hide();
        return;
    }
    uiRefresh();
}
