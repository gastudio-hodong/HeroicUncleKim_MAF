//
//  PopupDefenseObjectList.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2023/08/03.
//

#include "PopupDefenseObjectList.h"

#include "Common/ConfigGameNormal.h"

#include "GameUI/UI/UIReddot.h"
#include "GameObject/Defense/InfoDefenseWeapon.h"
#include "GameObject/Defense/InfoDefenseCharacter.h"
#include "GameObject/Defense/InfoDefenseTrench.h"
#include "GameObject/InfoSetting.h"

#include "GameUIPopup/Common/PopupLoadingSequentialUpgrade.h"
#include "GameUIPopup/Other/Defense/PopupDefenseObjectInfo.h"

#include "ManagerGame/DefenseManager.h"

PopupDefenseObjectList* PopupDefenseObjectList::create(E_DF_Trench_SettingType type)
{
    PopupDefenseObjectList* pRet = new(std::nothrow) PopupDefenseObjectList();
    if ( pRet && pRet->init(type) )
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

PopupDefenseObjectList::PopupDefenseObjectList(void):
_type(E_DF_Trench_SettingType::NONE),
_nCurrentRankUpIdx(0),
_isAllRankup(false),
_lyTop(nullptr),
_lyTable(nullptr),
_lyBottomUI(nullptr),
_table(nullptr)
{
    _listInfoWeapon.clear();
    _listInfoCharacter.clear();
}

PopupDefenseObjectList::~PopupDefenseObjectList(void)
{
    _listInfoWeapon.clear();
    _listInfoCharacter.clear();
    DefenseManager::getInstance()->_onServerInfo.Clear(this);
    DefenseManager::getInstance()->_onServerEquip.Clear(this);
    DefenseManager::getInstance()->_onServerRankup.Clear(this);
    DefenseManager::getInstance()->_onServerLevelup.Clear(this);
    DefenseManager::getInstance()->_onServerSmelting.Clear(this);
}


#pragma mark - table
void PopupDefenseObjectList::tableCellTouched(extension::TableView* table, extension::TableViewCell* cell)
{
    
}

Size PopupDefenseObjectList::tableCellSizeForIndex(extension::TableView *table, ssize_t idx)
{
    return Size(table->getContentSize().width, 283);
}

extension::TableViewCell* PopupDefenseObjectList::tableCellAtIndex(extension::TableView *table, ssize_t idx)
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
    
    return cell;
}

ssize_t PopupDefenseObjectList::numberOfCellsInTableView(extension::TableView *table)
{
    size_t size = 0;
    switch(_type)
    {
        case E_DF_Trench_SettingType::CHARACTER: size = _listInfoCharacter.size() / 5; break;
        case E_DF_Trench_SettingType::WEAPON: size = _listInfoWeapon.size() / 5; break;
        default: break;
    }
    return size;
}
void PopupDefenseObjectList::createCell(Node* cell, const int cellNumber)
{
    
    if(cell == nullptr)
        return;
    
    int grade = cellNumber + 1;
    std::string bgPath = MafUtils::format("Assets/ui/defense/Ui_list_titlename_box%02d.png", grade);
    
    auto sprBG = Sprite::create(bgPath);
    sprBG->setPosition(cell->getContentSize()/2);
    sprBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    cell->addChild(sprBG);
    
    
    auto lyTop = LayerColor::create();
    lyTop->setContentSize(Size(sprBG->getContentSize().width - 10, 52));
    lyTop->setIgnoreAnchorPointForPosition(false);
    lyTop->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    lyTop->setPosition(Vec2(sprBG->getContentSize().width / 2, sprBG->getContentSize().height));
    sprBG->addChild(lyTop);
    
    auto lbGrade = Label::createWithTTF(GAME_TEXT(MafUtils::format("t_ui_spirit_grade_%d", grade)), GAME_FONT, 24);
    lbGrade->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    lbGrade->setPosition(15, lyTop->getContentSize().height/2);
    lbGrade->setDimensions(120, lyTop->getContentSize().height);
    lbGrade->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
    lbGrade->setOverflow(Label::Overflow::SHRINK);
    lbGrade->enableBold();
    lyTop->addChild(lbGrade);
    
    auto menu = MafNode::MafMenu::create();
    menu->setPosition(Vec2::ZERO);
    cell->addChild(menu);
    
    Vec2 location = Vec2(10, 10);
    for(int i = 0; i < 5; ++i)
    {
        const int idx = i + 1 + cellNumber * 5;
        
        auto objCharacter = DefenseManager::getInstance()->getCharacterData(idx);
        auto objWeapon = DefenseManager::getInstance()->getWeaponData(idx);
        bool isHave = false;
        
        auto layerBG = Layer::create();
        layerBG->setContentSize(Size(138, 198));
        
        auto itemBG = MafNode::MafMenuItemSprite::create(layerBG, nullptr, CC_CALLBACK_1(PopupDefenseObjectList::onClickObject, this));
        itemBG->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        itemBG->setPosition(location);
        itemBG->setTag(idx);
        menu->addChild(itemBG);
        
        auto sprBox = Sprite::create(MafUtils::format("Assets/ui/defense/Ui_list_ch_box%02d.png", cellNumber + 1));
        sprBox->setPosition(itemBG->getContentSize()/2);
        itemBG->addChild(sprBox);
        
        int trenchNumber = 0;
        int lv = 0;
        int smelting = 0;
        int count = 0;
        bool isEquip = false;
        std::string name = "";
        switch(_type)
        {
            case E_DF_Trench_SettingType::CHARACTER:
            {
                lv = objCharacter->getCurrentLv();
                smelting = objCharacter->getCurrentSmelting();
                count = objCharacter->getCurrentCount();
                name = objCharacter->getName();
                isEquip = DefenseManager::getInstance()->isEquipCharacter(objCharacter, trenchNumber);
            }break;
            case E_DF_Trench_SettingType::WEAPON:
            {
                lv = objWeapon->getCurrentLv();
                smelting = objWeapon->getCurrentSmelting();
                count = objWeapon->getCurrentCount();
                name = objWeapon->getName();
                isEquip = DefenseManager::getInstance()->isEquipWeapon(objWeapon, trenchNumber);
            }break;
            default: break;
        }
        if(isEquip)
        {
            std::string framePath = "Assets/ui/defense/Ui_list_ch_box_equip.png";
            
            auto sprFrame = Sprite::create(framePath);
            sprFrame->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            sprFrame->setPosition(sprBox->getContentSize() / 2);
            sprFrame->getTexture()->setTexParameters(texParams);
            sprBox->addChild(sprFrame);
            
            //18,16
            auto lbTrenchNum = MafNode::createBmFont(MafUtils::format("%d", trenchNumber), GAME_BM_FONT_AL, 22);
            lbTrenchNum->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            lbTrenchNum->setPosition(Vec2(19,sprFrame->getContentSize().height - 16));
            lbTrenchNum->setColor(Color3B(85,240,255));
            sprFrame->addChild(lbTrenchNum);
        }
        
        auto lySmelting = LayerColor::create();
        lySmelting->setIgnoreAnchorPointForPosition(false);
        lySmelting->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        lySmelting->setPosition(sprBox->getContentSize().width/2, sprBox->getContentSize().height - 5);
        lySmelting->setContentSize(Size(48, 21));
        sprBox->addChild(lySmelting);
        
        auto sprSmelting = Sprite::create("Assets/ui_common/icon_star1_3.png");
        sprSmelting->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        sprSmelting->setPosition(5, lySmelting->getContentSize().height/2);
        lySmelting->addChild(sprSmelting);
        
        auto lbSmelting = MafNode::createBmFont(MafUtils::format("%d", smelting), GAME_BM_FONT_AL, 20);
        lbSmelting->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        lbSmelting->setPosition(Vec2(lySmelting->getContentSize().width - 13, lySmelting->getContentSize().height/2));
        lbSmelting->setColor(COLOR_TITLE);
        lySmelting->addChild(lbSmelting);
        
        auto lyName = LayerColor::create();
        lyName->setIgnoreAnchorPointForPosition(false);
        lyName->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        lyName->setPosition(sprBox->getContentSize().width/2, 6);
        lyName->setContentSize(Size(120, 32));
        sprBox->addChild(lyName);
        
        auto lbName =  Label::createWithTTF(name, GAME_FONT, 22);
        lbName->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        lbName->setPosition(lyName->getContentSize()/2);
        lbName->setDimensions(lyName->getContentSize().width - 20, lyName->getContentSize().height - 5);
        lbName->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        lbName->setOverflow(cocos2d::Label::Overflow::SHRINK);
        lyName->addChild(lbName);
        
        //name
        auto lyInfo = LayerColor::create();
        lyInfo->setIgnoreAnchorPointForPosition(false);
        lyInfo->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        lyInfo->setPosition(lyName->getPosition() + Vec2::UP * lyName->getContentSize().height);
        lyInfo->setContentSize(Size(126, 24));
        sprBox->addChild(lyInfo);
        
        auto lbLv = MafNode::createBmFont(MafUtils::format("Lv.%02d", lv), GAME_BM_FONT_AL, 20);
        lbLv->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        lbLv->setPosition(Vec2(7, lyInfo->getContentSize().height/2));
        lbLv->setColor(Color3B(255,220,110));
        lyInfo->addChild(lbLv);
        
        auto lbCount = MafNode::createBmFont(MafUtils::format("x%d", count), GAME_BM_FONT_AL, 20);
        lbCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        lbCount->setPosition(Vec2(lyInfo->getContentSize().width - 7, lyInfo->getContentSize().height/2));
        lbCount->setDimensions(lyInfo->getContentSize().width/2, lbCount->getContentSize().height);
        lbCount->setAlignment(TextHAlignment::RIGHT, TextVAlignment::CENTER);
        lbCount->setOverflow(cocos2d::Label::Overflow::SHRINK);
        lyInfo->addChild(lbCount);
        
        auto lyIcon = LayerColor::create();
        lyIcon->setIgnoreAnchorPointForPosition(false);
        lyIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        lyIcon->setContentSize(sprBox->getContentSize() - Size(20, 70));
        
        auto clipper = ClippingNode::create(lyIcon);
        clipper->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        clipper->setPosition(sprBox->getContentSize()/2);
        clipper->setIgnoreAnchorPointForPosition(false);
        sprBox->addChild(clipper);
        
        
        std::string path = MafUtils::format("Assets/ui/defense/Defense_mole_lv%d_00.png", idx);
        switch(_type)
        {
            case E_DF_Trench_SettingType::CHARACTER:
            {
                path = MafUtils::format("Assets/ui/defense/Defense_mole_lv%d_00.png", idx);
                isHave = objCharacter->isHave();
            }break;
            case E_DF_Trench_SettingType::WEAPON:
            {
                path = MafUtils::format("Assets/ui/defense/Icon_gun_lv%d.png", idx);
                isHave = objWeapon->isHave();
            }break;
            default: break;
        }
        
        auto sprIcon = Sprite::create(path);
        sprIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        sprIcon->setPosition(Vec2(clipper->getContentSize()/2) + Vec2::UP * 15);
        sprIcon->getTexture()->setTexParameters(texParams);
        clipper->addChild(sprIcon);
        if(_type == E_DF_Trench_SettingType::CHARACTER)
            sprIcon->setPosition(sprIcon->getPosition() + Vec2::UP * 15);
        
        if(isHave)
        {
            auto uiReddot = UIReddot::create(E_REDDOT_TYPE::NOMAL, Vec2::ANCHOR_MIDDLE);
            uiReddot->setPosition(sprBox->getContentSize() - Size(15));
            sprBox->addChild(uiReddot);
            uiReddot->setVisible(false);

            auto action1 = CallFuncN::create([&, objCharacter, objWeapon](Ref* sender){
                auto uiReddot = (UIReddot*)sender;
                
                switch(_type)
                {
                    case E_DF_Trench_SettingType::CHARACTER:
                    {
                        if (objCharacter->isReddotCondition())
                        {
                            uiReddot->setVisible(true);
                        }
                        else
                        {
                            uiReddot->setVisible(false);
                        }
                    }break;
                    case E_DF_Trench_SettingType::WEAPON:
                    {
                        if (objWeapon->isReddotCondition())
                        {
                            uiReddot->setVisible(true);
                        }
                        else
                        {
                            uiReddot->setVisible(false);
                        }
                    }break;
                    default: break;
                }
                
            });
            auto action2 = DelayTime::create(2);
            auto seq = Sequence::create(action1, action2, NULL);
            uiReddot->runAction(RepeatForever::create(seq));
        }
        
        if(!isHave)
        {
            sprIcon->setColor(Color3B(15, 15, 15));
            lbCount->setVisible(false);
            lbLv->setVisible(false);
            lbName->setString("???");
            lbName->setColor(Color3B(158,158,158));
            sprSmelting->setVisible(false);
            lbSmelting->setVisible(false);
            sprBox->setColor(Color3B(158,158,158));
        }
        
        location += Vec2::RIGHT * (layerBG->getContentSize().width + 7);
    }
    
}
#pragma mark - override
void PopupDefenseObjectList::hide()
{
    switch(_type)
    {
        case E_DF_Trench_SettingType::CHARACTER:
        {
            auto characterList = DefenseManager::getInstance()->getCharacterList();
            for(auto obj : characterList)
            {
                if(obj->isHave())
                    obj->completeFirstReddotCondition();
            }
        }break;
        case E_DF_Trench_SettingType::WEAPON:
        {
            auto weaponList = DefenseManager::getInstance()->getWeaponList();
            for(auto obj : weaponList)
            {
                if(obj->isHave())
                    obj->completeFirstReddotCondition();
            }
        }break;
        default: break;
    }
    
    PopupBase::hide();
}
#pragma mark -init
bool PopupDefenseObjectList::init(E_DF_Trench_SettingType type)
{
    if ( !PopupBase::init())
    {
        return false;
    }
    _type = type;
    _listInfoWeapon = DefenseManager::getInstance()->getWeaponList();
    _listInfoCharacter = DefenseManager::getInstance()->getCharacterList();
    initVar();
    initUI();
    
    DefenseManager::getInstance()->_onServerInfo += new MafFunction<void(bool,int)>(CC_CALLBACK_2(PopupDefenseObjectList::callbackServer, this), this);
    DefenseManager::getInstance()->_onServerEquip += new MafFunction<void(bool,int)>(CC_CALLBACK_2(PopupDefenseObjectList::callbackServer, this), this);
    DefenseManager::getInstance()->_onServerRankup += new MafFunction<void(bool,int)>(CC_CALLBACK_2(PopupDefenseObjectList::callbackAllRankUp, this), this);
    DefenseManager::getInstance()->_onServerLevelup += new MafFunction<void(bool,int)>(CC_CALLBACK_2(PopupDefenseObjectList::callbackServer, this), this);
    DefenseManager::getInstance()->_onServerSmelting += new MafFunction<void(bool,int)>(CC_CALLBACK_2(PopupDefenseObjectList::callbackServer, this), this);
    
    RefreshManager::getInstance()->addUI(E_REFRESH::SPIRIT, this);
    
    return true;
}

void PopupDefenseObjectList::onEnter()
{
    PopupBase::onEnter();
}

void PopupDefenseObjectList::initVar()
{
    _spriteContainer->setContentSize(Size(750, 1290));
    _spriteContainer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _spriteContainer->setPosition(getContentSize().width / 2, 0);
}

void PopupDefenseObjectList::initUI()
{
    //650
    Size size = _spriteContainer->getContentSize();
    
    _lyTop = LayerColor::create();
    _lyTop->setContentSize( Size(size.width, 75));
    _lyTop->setIgnoreAnchorPointForPosition(false);
    _lyTop->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _lyTop->setPosition(Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height));
    _spriteContainer->addChild(_lyTop);
    
    _lyTable = LayerColor::create();
    _lyTable->setContentSize( Size(size.width - 10, _spriteContainer->getContentSize().height - 285));
    _lyTable->setIgnoreAnchorPointForPosition(false);
    _lyTable->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _lyTable->setPosition(Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75));
    _spriteContainer->addChild(_lyTable);
    
    _lyBottomUI = LayerColor::create();
    _lyBottomUI->setContentSize( Size(_spriteContainer->getContentSize().width - 10, _spriteContainer->getContentSize().height - _lyTable->getContentSize().height - 10));
    _lyBottomUI->setIgnoreAnchorPointForPosition(false);
    _lyBottomUI->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _lyBottomUI->setPosition( _lyTable->getPosition() + Vec2::DOWN * _lyTable->getContentSize().height);
    _spriteContainer->addChild(_lyBottomUI);
    
    uiTop();
    uiTable();
    uiBottom();
}

#pragma mark -ui
void PopupDefenseObjectList::uiTop()
{
    _lyTop->removeAllChildren();
    
    // title
    std::string titleKey = "";
    std::string tabIconPath = "";
    std::string tabKey = "";
    switch (_type)
    {
        case E_DF_Trench_SettingType::CHARACTER:
        {
            titleKey = "t_ui_defense_squad_member";
            tabKey = "t_ui_defense_squad_weapon";
            tabIconPath = "Assets/icon/icon_defense_gun.png";
        }break;
        case E_DF_Trench_SettingType::WEAPON:
        {
            titleKey = "t_ui_defense_squad_weapon";
            tabKey = "t_ui_defense_squad_member";
            tabIconPath = "Assets/icon/icon_defense_mole.png";
        }break;
        default:
            break;
    }
    auto labelTitle = Label::createWithTTF(GAME_TEXT(titleKey), GAME_FONT, 28);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition( Vec2(_lyTop->getContentSize().width / 2, _lyTop->getContentSize().height/2) );
    _lyTop->addChild(labelTitle);
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _lyTop->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupDefenseObjectList::onClickClose, this));
    itemClose->setPosition( Vec2(_lyTop->getContentSize().width - itemClose->getContentSize().width, _lyTop->getContentSize().height/2) );
    itemClose->setTouchRect( itemClose->getContentSize() * 2 );
    menu->addChild(itemClose);

    auto btnTab = ui::Button::create("Assets/ui_common/btn_5.png");
    btnTab->setScale9Enabled(true);
    btnTab->setContentSize(Size(154, 58));
    btnTab->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    btnTab->setPosition(Vec2(60, _lyTop->getContentSize().height/2));
    btnTab->addClickEventListener(CC_CALLBACK_1(PopupDefenseObjectList::onClickTab, this));
    btnTab->setZoomScale(0.05f);
    btnTab->setTitleFontSize(26);
    btnTab->setTitleText(GAME_TEXT(tabKey));
    btnTab->setTitleAlignment(cocos2d::TextHAlignment::RIGHT, cocos2d::TextVAlignment::CENTER);
    btnTab->getTitleLabel()->setDimensions(90, btnTab->getTitleLabel()->getContentSize().height);
    btnTab->getTitleLabel()->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    btnTab->getTitleLabel()->setPosition(Vec2(btnTab->getContentSize().width - 10, btnTab->getContentSize().height/2));
    btnTab->getTitleLabel()->setOverflow(cocos2d::Label::Overflow::SHRINK);
    _lyTop->addChild(btnTab);
    
    
    auto icon = Sprite::create(tabIconPath);
    icon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    icon->setPosition(0, btnTab->getContentSize().height/2);
    btnTab->addChild(icon);
    {
        auto uiReddot = UIReddot::create(E_REDDOT_TYPE::NOMAL, Vec2::ANCHOR_MIDDLE);
        uiReddot->setPosition(Vec2(15, btnTab->getContentSize().height) - Size(0, 15));
        btnTab->addChild(uiReddot);
        uiReddot->setVisible(false);

        auto action1 = CallFuncN::create([=](Ref* sender){
            auto uiReddot = (UIReddot*)sender;
            
            if(_type == E_DF_Trench_SettingType::WEAPON)
            {
                if ( DefenseManager::getInstance()->isReddotCondition(E_DF_Trench_SettingType::CHARACTER))
                {
                    uiReddot->setVisible(true);
                }
                else
                {
                    uiReddot->setVisible(false);
                }
            }
            else if(_type == E_DF_Trench_SettingType::CHARACTER)
            {
                if ( DefenseManager::getInstance()->isReddotCondition(E_DF_Trench_SettingType::WEAPON))
                {
                    uiReddot->setVisible(true);
                }
                else
                {
                    uiReddot->setVisible(false);
                }
            }
        });
        auto action2 = DelayTime::create(2);
        auto seq = Sequence::create(action1, action2, NULL);
        uiReddot->runAction(RepeatForever::create(seq));
    }
}
void PopupDefenseObjectList::uiTable()
{
    _lyTable->removeAllChildren();
    
    _table = extension::TableView::create(this, Size(_lyTable->getContentSize().width, _lyTable->getContentSize().height - 10));
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

void PopupDefenseObjectList::uiBottom()
{
    _lyBottomUI->removeAllChildren();
    
    auto sprBox = ui::Scale9Sprite::create("Assets/ui_common/popup_mini_scale2_5.png");
    sprBox->setContentSize(Size(_lyBottomUI->getContentSize().width - 20, 214));
    sprBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    sprBox->setPosition(_lyBottomUI->getContentSize().width/2, _lyBottomUI->getContentSize().height);
    _lyBottomUI->addChild(sprBox);
    
    auto lbTitle = Label::createWithTTF(GAME_TEXT("t_ui_total_stat_1"), GAME_FONT, 28);
    lbTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    lbTitle->setColor(COLOR_TITLE);
    lbTitle->setPosition( Vec2(20, sprBox->getContentSize().height - 27.5) );
    sprBox->addChild(lbTitle);
   
    
    std::string effectWeaponDmg = "100";
    std::string effectCharacterDmg = "100";
    
    effectCharacterDmg = DefenseManager::getInstance()->getCharacterAttackCalculation(effectCharacterDmg);
    effectCharacterDmg = MafUtils::bigSubNum(effectCharacterDmg, "100");
    effectWeaponDmg = DefenseManager::getInstance()->getWeaponAttackCalculation(effectWeaponDmg);
    effectWeaponDmg = MafUtils::bigSubNum(effectWeaponDmg, "100");
    
    std::string totalEffectDmg = "100";
    totalEffectDmg = DefenseManager::getInstance()->getAttackCalculation(totalEffectDmg);
    totalEffectDmg = MafUtils::bigSubNum(totalEffectDmg, "100");
    
    
    std::string tmpTotalEffect = totalEffectDmg;
    MafUtils::convertNumberToShort(tmpTotalEffect);
    tmpTotalEffect += "%";
    auto lbTotalEffect = Label::createWithTTF(tmpTotalEffect, GAME_FONT, 28);
    lbTotalEffect->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    lbTotalEffect->setColor(COLOR_TITLE);
    lbTotalEffect->setPosition( Vec2(sprBox->getContentSize().width - 20, sprBox->getContentSize().height - 27.5) );
    sprBox->addChild(lbTotalEffect);
    {
        auto menu = MafNode::MafMenu::create();
        menu->setPosition(Vec2::ZERO);
        lbTotalEffect->addChild(menu);

        auto area = Layer::create();
        area->setContentSize(lbTotalEffect->getContentSize());

        auto btn = MafNode::MafMenuItemSprite::create(area, nullptr, nullptr, [=](Ref*){
            Vec2 posToast = _lyBottomUI->getParent()->convertToWorldSpace(_lyBottomUI->getPosition());
            posToast.y -= _lyBottomUI->getContentSize().height / 4.f;

            std::string strEffect = totalEffectDmg;
            strEffect = MafUtils::addCommaToString(strEffect, 3, ',');
            strEffect += "%";
            PopupToast::show(strEffect, posToast.x, posToast.y);
        });
        btn->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        btn->setPosition(lbTotalEffect->getContentSize()/2);
        menu->addChild(btn);
    }

    std::vector<std::pair<std::string, std::string>> listEffect = {
        std::make_pair(GAME_TEXT("t_ui_defense_squad_member") + " " + GAME_TEXT("t_ui_town_msg_2"), effectCharacterDmg),
        std::make_pair(GAME_TEXT("t_ui_defense_squad_weapon") + " " + GAME_TEXT("t_ui_town_msg_2"), effectWeaponDmg),
        std::make_pair(GAME_TEXT("t_ui_defense_setting_best_floor") + " " + GAME_TEXT("t_ui_town_msg_2"), std::to_string(DefenseManager::getInstance()->getHighFloor() * 100))
    };

    for( int i = 0; i < listEffect.size(); i++)
    {
        auto obj = listEffect[i];

        double posY = (sprBox->getContentSize().height - 75) - 5 * i - 25 * i;

        auto lbTitle = Label::createWithTTF(obj.first, GAME_FONT, 24);
        lbTitle->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        lbTitle->setPosition(20, posY);
        sprBox->addChild(lbTitle);

        std::string strEffect = "%s%%";

        std::string strEffectTmp = obj.second;
        //strEffectTmp = MafUtils::addCommaToString(strEffectTmp, 3, ',');

        MafUtils::convertNumberToShort(strEffectTmp);
        strEffect = MafUtils::format(strEffect.c_str(), strEffectTmp.c_str());

        auto lbEffect = Label::createWithTTF(strEffect, GAME_FONT, 24);
        lbEffect->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
        lbEffect->setPosition(sprBox->getContentSize().width - 140, posY);
        lbEffect->setDimensions(sprBox->getContentSize().width/2 - 140, lbEffect->getContentSize().height);
        lbEffect->setAlignment(cocos2d::TextHAlignment::RIGHT, cocos2d::TextVAlignment::TOP);
        lbEffect->setOverflow(cocos2d::Label::Overflow::SHRINK);
        {
            auto menu = MafNode::MafMenu::create();
            menu->setPosition(Vec2::ZERO);
            lbEffect->addChild(menu);

            auto area = Layer::create();
            area->setContentSize(lbEffect->getContentSize());

            auto btn = MafNode::MafMenuItemSprite::create(area, nullptr, nullptr, [=](Ref*){
                Vec2 posToast = _lyBottomUI->getParent()->convertToWorldSpace(_lyBottomUI->getPosition());
                posToast.y -= _lyBottomUI->getContentSize().height / 4.f;

                std::string strEffect = obj.second;
                strEffect = MafUtils::addCommaToString(strEffect, 3, ',');
                strEffect += "%";
                PopupToast::show(strEffect, posToast.x, posToast.y);
            });
            btn->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            btn->setPosition(lbEffect->getContentSize()/2);
            menu->addChild(btn);
        }

        sprBox->addChild(lbEffect);
    }
    
    auto btnAllRankUp = ui::Button::create("Assets/ui_common/btn_upgrade_bg_1.png");
    btnAllRankUp->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    btnAllRankUp->setPosition(Vec2(sprBox->getContentSize().width - 10, 25));
    btnAllRankUp->setZoomScale(0.05f);
    btnAllRankUp->addClickEventListener(CC_CALLBACK_1(PopupDefenseObjectList::onClickAllRankUP, this));
    btnAllRankUp->setTitleText(GAME_TEXT("t_ui_spirit_rank_up_2"));
    btnAllRankUp->setTitleAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
    btnAllRankUp->getTitleLabel()->setDimensions(btnAllRankUp->getContentSize().width, btnAllRankUp->getContentSize().height);
    btnAllRankUp->setTitleFontSize(30);
    btnAllRankUp->getTitleLabel()->setOverflow(cocos2d::Label::Overflow::SHRINK);
    sprBox->addChild(btnAllRankUp);
}

#pragma mark -utils
void PopupDefenseObjectList::onClickTab(Ref* sender)
{
    switch(_type)
    {
        case E_DF_Trench_SettingType::CHARACTER:
        {
            _type = E_DF_Trench_SettingType::WEAPON;
        }break;
        case E_DF_Trench_SettingType::WEAPON:
        {
            _type = E_DF_Trench_SettingType::CHARACTER;
        }break;
        default: break;
    }
    
    _table->reloadData();
    uiTop();
    uiBottom();
}
void PopupDefenseObjectList::onClickClose(Ref* sender)
{
    hide();
}

void PopupDefenseObjectList::onClickObject(Ref* sender)
{
    auto node = (Node*)sender;

    auto popup = PopupDefenseObjectInfo::create(_type, node->getTag());
    popup->show();
}

void PopupDefenseObjectList::onClickAllRankUP(Ref* sender)
{
    std::string popupMsgKey = "";
    std::string popupTextKey = "";
    switch(_type)
    {
        case E_DF_Trench_SettingType::CHARACTER:
        {
            popupMsgKey = "t_ui_defense_promotion_msg1";
            popupTextKey = "t_ui_defense_promotion_msg2";
        }break;
        case E_DF_Trench_SettingType::WEAPON:
        {
            popupMsgKey = "t_ui_defense_promotion_msg3";
            popupTextKey = "t_ui_defense_promotion_msg4";
        }break;
        default: break;
    }
    auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT(popupMsgKey));
    popup->addButton(GAME_TEXT("t_ui_close"), true, nullptr);
    popup->addButton(GAME_TEXT("t_ui_confirm"), false, [=](Ref*){
        _nCurrentRankUpIdx = 1;
        _isAllRankup = true;
        PopupLoadingSequentialUpgrade::show();
        PopupLoadingSequentialUpgrade::setTextKey(popupTextKey);
        PopupLoadingSequentialUpgrade::setCurrentIdx(_nCurrentRankUpIdx + 1);
        requestAllRankUp();
    });
    popup->show();
    
}

void PopupDefenseObjectList::requestAllRankUp()
{
    
    int makeSettingCount = 5;
    
    if(auto objSetting = DefenseManager::getInstance()->getInfoSetting("defense_promotion"))
        makeSettingCount = objSetting->getValueToInt();
    
    int size = 0;
    switch(_type)
    {
        case E_DF_Trench_SettingType::CHARACTER: size = DefenseManager::getInstance()->getCharacterDataCount(); break;
        case E_DF_Trench_SettingType::WEAPON: size = DefenseManager::getInstance()->getWeaponDataCount(); break;
        default: break;
    }

    for(; _nCurrentRankUpIdx <= size-1; ++_nCurrentRankUpIdx)
    {
        auto characterData = DefenseManager::getInstance()->getCharacterData(_nCurrentRankUpIdx);
        auto weaponData = DefenseManager::getInstance()->getWeaponData(_nCurrentRankUpIdx);
        int idx = 0;
        int currentCount = 0;
        switch(_type)
        {
            case E_DF_Trench_SettingType::CHARACTER:
            {
                if(characterData == nullptr || !characterData->isHave())
                    continue;
                currentCount = characterData->getCurrentCount();
                idx = characterData->getIndex();
            }break;
            case E_DF_Trench_SettingType::WEAPON:
            {
                if(weaponData == nullptr || !weaponData->isHave())
                    continue;
                currentCount = weaponData->getCurrentCount();
                idx = weaponData->getIndex();
            }break;
        }
        
        int nMakeCount = currentCount / makeSettingCount;

        if(nMakeCount > 0)
        {
            PopupLoadingSequentialUpgrade::setCurrentIdx(_nCurrentRankUpIdx + 1);
            DefenseManager::getInstance()->requestRankup(_type, idx, nMakeCount, false);
            return;
        }
    }

    if(_nCurrentRankUpIdx >= size)
    {
        PopupLoadingSequentialUpgrade::hide();
        auto offset = _table->getContentOffset();
        _table->reloadData();
        _table->setContentOffset(offset);
        uiBottom();
        _isAllRankup = false;
    }
}
#pragma mark -callback
void PopupDefenseObjectList::callbackServer(bool bResult, int nResult)
{
    auto offset = _table->getContentOffset();
    _table->reloadData();
    _table->setContentOffset(offset);
    uiBottom();
}

void PopupDefenseObjectList::callbackAllRankUp(bool bResult, int nResult)
{
    if(bResult == false)
    {
        PopupLoading::hide();
        
        if(nResult != 9)
        {
            std::string strMessageKey = "t_ui_error_30";
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT(strMessageKey));
            popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
            popup->show();
        }
        
        hide();
        return;
    }
    
    if(_isAllRankup)
    {
        auto func = CallFunc::create([=](){
            requestAllRankUp();
        });
        
        runAction(Sequence::create(DelayTime::create(0.01f), func, NULL));
    }
    else
    {
        auto offset = _table->getContentOffset();
        _table->reloadData();
        _table->setContentOffset(offset);
        uiBottom();
    }
}
