//
//  UILayerDefenseObjectSmelting.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2023/08/08.
//

#include "UILayerDefenseObjectSmelting.h"

#include "Common/ConfigGameNormal.h"

#include "GameObject/Defense/InfoDefenseWeapon.h"
#include "GameObject/Defense/InfoDefenseCharacter.h"

#include "GameObject/InfoSetting.h"

#include "GameUI/UI/UICurrency.h"

#include "GameUIPopup/Other/CurrencyDetail/PopupCurrencyDetail.h"

#include "ManagerGame/DefenseManager.h"
#include "ManagerGame/ItemsManager.h"
#include "ManagerGame/ItemsMoneyManager.h"
#include "ManagerGame/CostumeManager.h"

USING_NS_CC;

UILayerDefenseObjectSmelting* UILayerDefenseObjectSmelting::create(cocos2d::Size size, E_DF_Trench_SettingType type, int idx)
{
    UILayerDefenseObjectSmelting* pRet = new(std::nothrow) UILayerDefenseObjectSmelting();
    if ( pRet && pRet->init(size, type, idx) )
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

UILayerDefenseObjectSmelting::UILayerDefenseObjectSmelting(void):
_type(E_DF_Trench_SettingType::NONE)
,_idx(0)
,_lbNowCnt(nullptr)
,_lbNextCnt(nullptr)
{
}

UILayerDefenseObjectSmelting::~UILayerDefenseObjectSmelting(void)
{
    DefenseManager::getInstance()->_onServerSmelting.Clear(this);
    _onHide.Clear();
}

bool UILayerDefenseObjectSmelting::init(cocos2d::Size size, E_DF_Trench_SettingType type, int idx)
{
    if ( !LayerColor::init())
    {
        return false;
    }
    
    setContentSize(size);
    setIgnoreAnchorPointForPosition(false);
    setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _type = type;
    _idx = idx;
    
    DefenseManager::getInstance()->_onServerSmelting += new MafFunction<void(bool, int)>(CC_CALLBACK_2(UILayerDefenseObjectSmelting::callbackSmelting, this), this);
    //
    
    // init
    initVar();
    initUi();
    
    return true;
}

#pragma mark - init
void UILayerDefenseObjectSmelting::initVar()
{
}

void UILayerDefenseObjectSmelting::initUi()
{
    //
    uiMain();
}

#pragma mark - ui
void UILayerDefenseObjectSmelting::uiMain()
{
    removeAllChildren();
    
    auto characterData = DefenseManager::getInstance()->getCharacterData(_idx);
    auto weaponData = DefenseManager::getInstance()->getWeaponData(_idx);
    
    int count = 0;
    int cost = 0;
    int smeltingSameCost = 0;
    int maxSmelting = 0;
    int currentSmelting = 0;
    int nextSmelting = 0;
    double currentDmage = 0;
    double nextDamage = 0;
    std::string iconPath = "";
    std::string name = "";
    
    auto setting = DefenseManager::getInstance()->getInfoSetting("defense_smelting_cost");
    
    smeltingSameCost = setting->getValueToInt();
    
    switch(_type)
    {
        case E_DF_Trench_SettingType::CHARACTER:
        {
            count = characterData->getCurrentCount();
            cost = characterData->getSmeltingCost();
            maxSmelting = characterData->getMaxSmelting();
            currentDmage = characterData->getBSAttack();
            nextDamage = characterData->getBSAttack(characterData->getCurrentSmelting() + 1);
            currentSmelting = characterData->getCurrentSmelting();
            nextSmelting = currentSmelting + 1;
            iconPath = characterData->getResourcePath();
            name = characterData->getName();
        }break;
        case E_DF_Trench_SettingType::WEAPON:
        {
            count = weaponData->getCurrentCount();
            cost = weaponData->getSmeltingCost();
            maxSmelting = weaponData->getMaxSmelting();
            currentDmage = weaponData->getBSAttack();
            nextDamage = weaponData->getBSAttack(weaponData->getCurrentSmelting() + 1);
            currentSmelting = weaponData->getCurrentSmelting();
            nextSmelting = currentSmelting + 1;
            iconPath = weaponData->getResourceIconPath();
            name = weaponData->getName();
        }break;
    }
    
    auto sprCurrencyBox = Sprite::create("Assets/ui_common/base_text_2_4.png");
    sprCurrencyBox->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    sprCurrencyBox->setPosition(5, getContentSize().height - 5);
    addChild(sprCurrencyBox);
    {
        std::vector<E_ITEMS> list;
        list.push_back(E_ITEMS::COSTUME_COIN);
        
        auto uiCurrency = UICurrency::create(list, Size(getContentSize().width, 40));
        uiCurrency->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        uiCurrency->setPosition(Vec2(10, sprCurrencyBox->getContentSize().height / 2));
        sprCurrencyBox->addChild(uiCurrency, E_ZORER::UI+1);
    }
    
    if(currentSmelting < maxSmelting)
    {
        auto sprAfterBox = Sprite::create("Assets/ui_common/base_text_3_1.png");
        sprAfterBox->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
        sprAfterBox->setPosition(getContentSize().width - 5, getContentSize().height - 5);
        addChild(sprAfterBox);
        {
            auto lbText = Label::createWithTTF("AFTER", GAME_FONT, 26);
            lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            lbText->setPosition(sprAfterBox->getContentSize()/2);
            lbText->setDimensions(sprAfterBox->getContentSize().width - 5, sprAfterBox->getContentSize().height - 5);
            lbText->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
            lbText->setOverflow(Label::Overflow::SHRINK);
            lbText->setColor(Color3B(51,255,0));
            lbText->enableBold();
            sprAfterBox->addChild(lbText);
        }
    }
    
    auto sprStatBox = ui::Scale9Sprite::create("Assets/ui_common/base_box_1_2.png");
    sprStatBox->setContentSize(Size(674, 50));
    sprStatBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    sprStatBox->setPosition(getContentSize().width/2, sprCurrencyBox->getPosition().y - sprCurrencyBox->getContentSize().height - 10);
    addChild(sprStatBox);
    
    
    std::vector<std::pair<std::string, std::pair<double, double>>> listEffect = {
        std::make_pair(GAME_TEXT("t_ui_town_msg_2"), std::make_pair(currentDmage, nextDamage))
    };
    
    for( int i = 0; i < listEffect.size(); i++)
    {
        auto obj = listEffect[i];
        
        double posY = (sprStatBox->getContentSize().height - 5) - 40 *i;
        
        auto lbTitle = Label::createWithTTF(obj.first, GAME_FONT, 26);
        lbTitle->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        lbTitle->setPosition(15, posY);
        sprStatBox->addChild(lbTitle);
        
        double nEffect = obj.second.first;
        double nPlusEffect = obj.second.second;
        std::string strEffect = MafUtils::doubleToString(nEffect);
        std::string strPlusEffect = MafUtils::doubleToString(nPlusEffect);
        
        MafUtils::convertNumberToShort(strEffect);
        MafUtils::convertNumberToShort(strPlusEffect);
        
        auto lbEffect = Label::createWithTTF(strEffect + "%", GAME_FONT, 26);
        lbEffect->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
        lbEffect->setPosition(sprStatBox->getContentSize().width - 230, posY);
        lbEffect->setDimensions(120, lbEffect->getContentSize().height);
        lbEffect->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        lbEffect->setOverflow(Label::Overflow::SHRINK);
        sprStatBox->addChild(lbEffect);
        {
            auto menu = MafNode::MafMenu::create();
            menu->setPosition(Vec2::ZERO);
            lbEffect->addChild(menu);
            
            auto area = Layer::create();
            area->setContentSize(lbEffect->getContentSize());
            
            auto btn = MafNode::MafMenuItemSprite::create(area, nullptr, nullptr, [=](Ref*){
                Vec2 posToast = getParent()->convertToWorldSpace(getPosition());
                posToast.y += getContentSize().height / 2.2f;
                
                std::string strEffect = MafUtils::doubleToString(nEffect);
                strEffect = MafUtils::addCommaToString(strEffect, 3, ',');
                strEffect += "%";
                PopupToast::show(strEffect, posToast.x, posToast.y);
            });
            btn->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            btn->setPosition(lbEffect->getContentSize()/2);
            menu->addChild(btn);
        }
        
        if(currentSmelting < maxSmelting)
        {
        
            auto lbPlusEffect = Label::createWithTTF(strPlusEffect + "%", GAME_FONT, 26);
            lbPlusEffect->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
            lbPlusEffect->setPosition(sprStatBox->getContentSize().width-15, posY);
            lbPlusEffect->setColor(Color3B(240,40,240));
            lbPlusEffect->setDimensions(120, lbPlusEffect->getContentSize().height);
            lbPlusEffect->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
            lbPlusEffect->setOverflow(Label::Overflow::SHRINK);
            sprStatBox->addChild(lbPlusEffect);
            
            auto sprArrow = Sprite::create("Assets/ui_common/arrow_R05.png");
            sprArrow->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
            sprArrow->setPosition(sprStatBox->getContentSize().width - sprStatBox->getContentSize().width/3.7 ,posY - 10);
            sprStatBox->addChild(sprArrow);
            
            {
                auto menu = MafNode::MafMenu::create();
                menu->setPosition(Vec2::ZERO);
                lbPlusEffect->addChild(menu);
                
                auto area = Layer::create();
                area->setContentSize(lbPlusEffect->getContentSize());
                
                auto btn = MafNode::MafMenuItemSprite::create(area, nullptr, nullptr, [=](Ref*){
                    Vec2 posToast = getParent()->convertToWorldSpace(getPosition());
                    posToast.y += getContentSize().height / 2.2f;
                    
                    std::string strPlusEffect = MafUtils::doubleToString(nPlusEffect);
                    strPlusEffect = MafUtils::addCommaToString(strPlusEffect, 3, ',');
                    strPlusEffect += "%";
                    PopupToast::show(strPlusEffect, posToast.x, posToast.y);
                });
                btn->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                btn->setPosition(lbPlusEffect->getContentSize()/2);
                menu->addChild(btn);
            }
        }
        else
        {
            lbEffect->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
            lbEffect->setPosition(sprStatBox->getContentSize().width-15, posY);
        }
    }
    
    
    auto btnRankUp = ui::Button::create("Assets/ui_common/btn_upgrade_bg.png");
    btnRankUp->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    btnRankUp->setPosition(Vec2(getContentSize().width/2, 25));
    btnRankUp->setZoomScale(0.05f);
    btnRankUp->addClickEventListener(CC_CALLBACK_1(UILayerDefenseObjectSmelting::onClickSmelting, this));
    addChild(btnRankUp);
    {
        auto lbText = Label::createWithTTF(GAME_TEXT("t_ui_smelting_1"), GAME_FONT, 26);
        lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        lbText->setPosition(btnRankUp->getContentSize().width/2, btnRankUp->getContentSize().height - btnRankUp->getContentSize().height/4);
        lbText->setDimensions(btnRankUp->getContentSize().width - 10, btnRankUp->getContentSize().height/2);
        lbText->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        lbText->setOverflow(Label::Overflow::SHRINK);
        btnRankUp->addChild(lbText);
        
        auto item = ItemsManager::getInstance()->getInfoItems(E_ITEMS::COSTUME_COIN);
        auto sprCurrencyIcon = Sprite::create(item->getPath());
        sprCurrencyIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        sprCurrencyIcon->setPosition(btnRankUp->getContentSize().width/4, btnRankUp->getContentSize().height/4);
        btnRankUp->addChild(sprCurrencyIcon);
        
        auto lbCount = Label::createWithTTF(std::to_string(cost), GAME_FONT, 26);
        lbCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        lbCount->setPosition(sprCurrencyIcon->getPosition() + Vec2::RIGHT * sprCurrencyIcon->getContentSize().width/2);
        lbCount->setDimensions(100, btnRankUp->getContentSize().height/2);
        lbCount->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
        lbCount->setOverflow(Label::Overflow::SHRINK);
        btnRankUp->addChild(lbCount);
        
    }
    
    if(currentSmelting >= maxSmelting)
    {
        btnRankUp->setVisible(false);
    }
    
    auto sprInfoBG = ui::Scale9Sprite::create("Assets/ui_common/list_bg_s_2_1.png");
    sprInfoBG->setContentSize(Size(674, 102));
    sprInfoBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    sprInfoBG->setPosition(getContentSize().width/2, sprStatBox->getPosition().y - sprStatBox->getContentSize().height - 10);
    addChild(sprInfoBG);
    {
        auto sprIconBox = ui::Scale9Sprite::create("Assets/ui_common/box_item.png");
        sprIconBox->setContentSize(Size(82, 82));
        sprIconBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        sprIconBox->setPosition(5, sprInfoBG->getContentSize().height/2);
        sprInfoBG->addChild(sprIconBox);
        
        auto lyIcon = LayerColor::create();
        lyIcon->setIgnoreAnchorPointForPosition(false);
        lyIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        lyIcon->setContentSize(sprIconBox->getContentSize() - Size(10));
        
        auto clipper = ClippingNode::create(lyIcon);
        clipper->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        clipper->setPosition(sprIconBox->getContentSize()/2);
        clipper->setIgnoreAnchorPointForPosition(false);
        sprIconBox->addChild(clipper);
        
        auto sprIcon = Sprite::create(iconPath);
        sprIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        sprIcon->setPosition(clipper->getContentSize()/2);
        clipper->addChild(sprIcon);
        
        std::string strName = "%d. %s";
        
        auto lbName = Label::createWithTTF(MafUtils::format(strName.c_str(), _idx, name.c_str()), GAME_FONT, 26);
        lbName->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        lbName->setPosition(sprIconBox->getPosition().x + sprIconBox->getContentSize().width + 5, sprInfoBG->getContentSize().height - sprInfoBG->getContentSize().height/4);
        lbName->setDimensions(sprInfoBG->getContentSize().width - sprIconBox->getContentSize().width - 10, sprInfoBG->getContentSize().height/2);
        lbName->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
        lbName->setOverflow(Label::Overflow::SHRINK);
        sprInfoBG->addChild(lbName);
        
        auto lbCount = Label::createWithTTF(GAME_TEXTFORMAT("t_ui_have_it_1", count), GAME_FONT, 26);
        lbCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        lbCount->setPosition(sprIconBox->getPosition().x + sprIconBox->getContentSize().width + 5, sprInfoBG->getContentSize().height/4);
        lbCount->setDimensions(200, sprInfoBG->getContentSize().height/2);
        lbCount->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
        lbCount->setOverflow(Label::Overflow::SHRINK);
        sprInfoBG->addChild(lbCount);
        
        
        if(currentSmelting < maxSmelting)
        {
            std::string costTitleText = "";
            std::string typeText = "";
            switch(_type)
            {
                case E_DF_Trench_SettingType::CHARACTER :
                {
                    typeText = GAME_TEXT("t_ui_defense_squad_member");
                    
                    costTitleText = GAME_TEXTFORMAT("t_ui_defense_smelting_cost_1", smeltingSameCost, typeText.c_str());
                }break;
                case E_DF_Trench_SettingType::WEAPON :
                {
                    typeText = GAME_TEXT("t_ui_defense_squad_weapon");
                    
                    costTitleText = GAME_TEXTFORMAT("t_ui_defense_smelting_cost_2", smeltingSameCost, typeText.c_str());
                }break;
                    
            }
            auto lbSmeltingCount = Label::createWithTTF(costTitleText, GAME_FONT, 26);
            lbSmeltingCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
            lbSmeltingCount->setPosition(Vec2(sprInfoBG->getContentSize().width - 10, sprInfoBG->getContentSize().height/4));
            lbSmeltingCount->setDimensions(340, lbSmeltingCount->getContentSize().height);
            lbSmeltingCount->setAlignment(TextHAlignment::RIGHT, TextVAlignment::CENTER);
            lbSmeltingCount->setOverflow(Label::Overflow::SHRINK);
            lbSmeltingCount->setColor(Color3B(51,255,0));
            sprInfoBG->addChild(lbSmeltingCount);
        }
    }
}

#pragma mark -utils
void UILayerDefenseObjectSmelting::onClickSmelting(Ref* sender)
{
    int count = 0;
    int cost = 0;
    int maxSmelting = 0;
    int smeltingSameCost = 0;
    
    auto characterData = DefenseManager::getInstance()->getCharacterData(_idx);
    auto weaponData = DefenseManager::getInstance()->getWeaponData(_idx);
    auto setting = DefenseManager::getInstance()->getInfoSetting("defense_smelting_cost");
    
    smeltingSameCost = setting->getValueToInt();
    
    switch(_type)
    {
        case E_DF_Trench_SettingType::CHARACTER:
        {
            cost = characterData->getSmeltingCost();
            count = characterData->getCurrentCount();
            maxSmelting = characterData->getMaxSmelting();
        }break;
        case E_DF_Trench_SettingType::WEAPON:
        {
            cost = weaponData->getSmeltingCost();
            count = weaponData->getCurrentCount();
            maxSmelting = weaponData->getMaxSmelting();
        }break;
    }
    
    if(cost > std::atoi(ItemsManager::getInstance()->getItems(E_ITEMS::COSTUME_COIN).c_str()))
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_31"));
        popup->addButton(GAME_TEXT("t_ui_system_update_confirm"), true, nullptr);
        popup->show();
        return;
    }
    if(count < smeltingSameCost)
    {
        std::string key = "";
        
        switch(_type)
        {
            case E_DF_Trench_SettingType::CHARACTER: key = "t_ui_defense_smelting_error_1"; break;
            case E_DF_Trench_SettingType::WEAPON: key = "t_ui_defense_smelting_error_2"; break;
        }
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT(key));
        popup->addButton(GAME_TEXT("t_ui_system_update_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    DefenseManager::getInstance()->requestSmelting(_type, _idx);
}

#pragma mark -callback
void UILayerDefenseObjectSmelting::callbackSmelting(bool bResult, int nResult)
{
    if ( bResult == false )
    {
        if(nResult != 9)
        {
            std::string strMessageKey = "t_ui_error_30";
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT(strMessageKey));
            popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
            popup->show();
        }
        _onHide.Invoke();
        return;
    }
    uiMain();
}

void UILayerDefenseObjectSmelting::onChangeObject(int idx)
{
    _idx = idx;
    uiMain();
}
