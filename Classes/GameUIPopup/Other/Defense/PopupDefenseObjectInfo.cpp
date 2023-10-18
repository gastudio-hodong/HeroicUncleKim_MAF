//
//  PopupDefenseObjectInfo.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2023/08/04.
//

#include "PopupDefenseObjectInfo.h"

#include "Common/ConfigGameNormal.h"
#include "GameObject/Defense/InfoDefenseCharacter.h"
#include "GameObject/Defense/InfoDefenseWeapon.h"

#include "GameUIPopup/Other/Defense/Layer/UILayerDefenseObjectLevelup.h"
#include "GameUIPopup/Other/Defense/Layer/UILayerDefenseObjectRankUp.h"
#include "GameUIPopup/Other/Defense/Layer/UILayerDefenseObjectSmelting.h"

#include "ManagerGame/DefenseManager.h"

PopupDefenseObjectInfo* PopupDefenseObjectInfo::create(E_DF_Trench_SettingType type, const int idx)
{
    PopupDefenseObjectInfo* pRet = new(std::nothrow) PopupDefenseObjectInfo();
    if ( pRet && pRet->init(type, idx) )
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

PopupDefenseObjectInfo::PopupDefenseObjectInfo(void):
_tabType(PopupDefenseObjectInfo::E_TYPE::RANK_UP)
,_type(E_DF_Trench_SettingType::NONE)
,_weaponData(nullptr)
,_characterData(nullptr)
,_lyTopUI(nullptr)
,_lyMiddleUI(nullptr)
,_lyBottomUI(nullptr)
,_lyContainerNow(nullptr)
,_lyLevelUp(nullptr)
,_lyRankUp(nullptr)
,_lySmeltingUp(nullptr)
{
}

PopupDefenseObjectInfo::~PopupDefenseObjectInfo(void)
{
    DefenseManager::getInstance()->_onServerInfo.Clear(this);
    DefenseManager::getInstance()->_onServerEquip.Clear(this);
    DefenseManager::getInstance()->_onServerRankup.Clear(this);
    DefenseManager::getInstance()->_onServerLevelup.Clear(this);
    DefenseManager::getInstance()->_onServerSmelting.Clear(this);
}

#pragma mark -init
bool PopupDefenseObjectInfo::init(E_DF_Trench_SettingType type, const int idx)
{
    if ( !PopupBase::init())
    {
        return false;
    }
    
    _type = type;
    _idx = idx;
    
    
    DefenseManager::getInstance()->_onServerInfo += new MafFunction<void(bool,int)>(CC_CALLBACK_2(PopupDefenseObjectInfo::callbackServer, this), this);
    DefenseManager::getInstance()->_onServerEquip += new MafFunction<void(bool,int)>(CC_CALLBACK_2(PopupDefenseObjectInfo::callbackServer, this), this);
    DefenseManager::getInstance()->_onServerRankup += new MafFunction<void(bool,int)>(CC_CALLBACK_2(PopupDefenseObjectInfo::callbackServer, this), this);
    DefenseManager::getInstance()->_onServerLevelup += new MafFunction<void(bool,int)>(CC_CALLBACK_2(PopupDefenseObjectInfo::callbackServer, this), this);
    DefenseManager::getInstance()->_onServerSmelting += new MafFunction<void(bool,int)>(CC_CALLBACK_2(PopupDefenseObjectInfo::callbackServer, this), this);
    
    switch(_type)
    {
        case E_DF_Trench_SettingType::CHARACTER: _characterData = DefenseManager::getInstance()->getCharacterData(_idx); break;
        case E_DF_Trench_SettingType::WEAPON: _weaponData = DefenseManager::getInstance()->getWeaponData(_idx); break;
        default: break;
    }
    initVar();
    initUI();
    
    return true;
}

void PopupDefenseObjectInfo::initVar()
{
    _spriteContainer->setContentSize(Size(710, 1100));
    _spriteContainer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _spriteContainer->setPosition(getContentSize().width / 2, 0);
}

void PopupDefenseObjectInfo::initUI()
{
    // title
    
    std::string titleKey = "";
    switch (_type)
    {
        case E_DF_Trench_SettingType::CHARACTER: titleKey = "t_ui_defense_squad_member"; break;
        case E_DF_Trench_SettingType::WEAPON: titleKey = "t_ui_defense_squad_weapon"; break;
        default:
            break;
    }
    
    auto lbTitle = Label::createWithTTF(GAME_TEXT(titleKey), GAME_FONT, 28);
    lbTitle->setColor(COLOR_TITLE);
    lbTitle->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5) );
    _spriteContainer->addChild(lbTitle);
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupDefenseObjectInfo::onClickClose, this));
    itemClose->setPosition( Vec2(_spriteContainer->getContentSize().width - itemClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5) );
    itemClose->setTouchRect( itemClose->getContentSize() * 2 );
    menu->addChild(itemClose);
    
    
    _lyTopUI = LayerColor::create();
    _lyTopUI->setContentSize( Size(_spriteContainer->getContentSize().width - 10, 500));
    _lyTopUI->setIgnoreAnchorPointForPosition(false);
    _lyTopUI->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _lyTopUI->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75));
    _spriteContainer->addChild(_lyTopUI);
    
    _lyMiddleUI = LayerColor::create();
    _lyMiddleUI->setContentSize( Size(_spriteContainer->getContentSize().width - 10, 400));
    _lyMiddleUI->setIgnoreAnchorPointForPosition(false);
    _lyMiddleUI->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _lyMiddleUI->setPosition( _lyTopUI->getPosition() + Vec2::DOWN * _lyTopUI->getContentSize().height);
    _spriteContainer->addChild(_lyMiddleUI);
    
    _lyBottomUI = LayerColor::create();
    _lyBottomUI->setContentSize( Size(682, _spriteContainer->getContentSize().height - 85 - _lyTopUI->getContentSize().height - _lyMiddleUI->getContentSize().height));
    _lyBottomUI->setIgnoreAnchorPointForPosition(false);
    _lyBottomUI->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _lyBottomUI->setPosition( _lyMiddleUI->getPosition() + Vec2::DOWN * _lyMiddleUI->getContentSize().height);
    _spriteContainer->addChild(_lyBottomUI);
    
    uiTop();
    uiMiddle();
    uiBottom();
}

#pragma mark -init
void PopupDefenseObjectInfo::uiTop()
{
    _lyTopUI->removeAllChildren();
    
    std::string bgPath = "";
    switch (_type)
    {
        case E_DF_Trench_SettingType::CHARACTER: bgPath = "Assets/ui/defense/Defense_info_bg_unit.png"; break;
        case E_DF_Trench_SettingType::WEAPON: bgPath = "Assets/ui/defense/Defense_info_bg_gun.png"; break;
            
        default:
            break;
    }
    
    auto sprBg = Sprite::create(bgPath);
    sprBg->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    sprBg->setPosition(_lyTopUI->getContentSize().width/2, _lyTopUI->getContentSize().height);
    sprBg->getTexture()->setTexParameters(texParams);
    sprBg->setScale(3);
    _lyTopUI->addChild(sprBg);
    
    auto lyBG = LayerColor::create();
    lyBG->setContentSize(Size(sprBg->getContentSize() * sprBg->getScale()));
    lyBG->setIgnoreAnchorPointForPosition(false);
    lyBG->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    lyBG->setPosition(sprBg->getPosition());
    _lyTopUI->addChild(lyBG);
    
    auto lyInfo = LayerColor::create();
    lyInfo->setContentSize(Size(_lyTopUI->getContentSize().width, _lyTopUI->getContentSize().height - (sprBg->getContentSize().height * sprBg->getScale())));
    lyInfo->setIgnoreAnchorPointForPosition(false);
    lyInfo->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    lyInfo->setPosition( sprBg->getPosition() + Vec2::DOWN * sprBg->getContentSize().height * sprBg->getScale());
    _lyTopUI->addChild(lyInfo);
    
    bool isHave = false;
    int trenchNumver = 0;
    int smelting = 0;
    int lv = 0;
    int grade = 0;
    double bongDamage = 0;
    double dfDamage = 0;
    double dfAttackSpeed = 0;
    Vec2 iconLocation = Vec2::ZERO;
    
    std::string iconPath = "";
    std::string name = "";
    switch(_type)
    {
        case E_DF_Trench_SettingType::CHARACTER:
        {
            iconPath = _characterData->getResourcePath();
            isHave = _characterData->isHave();
            DefenseManager::getInstance()->isEquipCharacter(_characterData, trenchNumver);
            
            smelting = _characterData->getCurrentSmelting();
            lv = _characterData->getCurrentLv();
            bongDamage = _characterData->getBSAttack();
            dfDamage = _characterData->getDFAttack();
            dfAttackSpeed = _characterData->getBaseDFAttackSpeed();
            name = _characterData->getName();
            grade = _characterData->getGrade();
            
            iconLocation = Vec2(lyBG->getContentSize().width/2 - 5, lyBG->getContentSize().height - 30);
            if(isHave)
            {
                _characterData->completeFirstReddotCondition();
            }
            //name =
        }break;
        case E_DF_Trench_SettingType::WEAPON:
        {
            iconPath = _weaponData->getResourceIconPath();
            isHave = _weaponData->isHave();
            DefenseManager::getInstance()->isEquipWeapon(_weaponData, trenchNumver);
            
            smelting = _weaponData->getCurrentSmelting();
            lv = _weaponData->getCurrentLv();
            bongDamage = _weaponData->getBSAttack();
            dfDamage = _weaponData->getDFAttack();
            dfAttackSpeed = _weaponData->getBaseDFAttackSpeed();
            name = _weaponData->getName();
            grade = _weaponData->getGrade();
            
            iconLocation = Vec2(lyBG->getContentSize().width/2, lyBG->getContentSize().height - 80);
            if(isHave)
            {
                _weaponData->completeFirstReddotCondition();
            }
            //name =
        }break;
        default: break;
    }
    if(isHave)
    {
        auto sprBGEffect = Sprite::create("Assets/ui/defense/Defense_info_bg_eff00.png");
        sprBGEffect->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        sprBGEffect->setPosition(sprBg->getContentSize()/2);
        sprBGEffect->getTexture()->setTexParameters(texParams);
        sprBg->addChild(sprBGEffect);
        
        auto anim = Animation::create();
        for(int i = 0; i <= 7; ++i)
        {
            std::string strPath = MafUtils::format("Assets/ui/defense/Defense_info_bg_eff%02d.png", i);
            
            auto sprite = Sprite::create(strPath);
            sprite->getTexture()->setTexParameters(texParams);
            anim->addSpriteFrame(sprite->getSpriteFrame());
        }
        anim->setDelayPerUnit(0.1f);
        auto animate = Animate::create(anim);
        sprBGEffect->runAction(RepeatForever::create(animate));
    }
    auto sprIcon = Sprite::create(iconPath);
    sprIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    sprIcon->setPosition(iconLocation);
    sprIcon->setScale(2);
    sprIcon->getTexture()->setTexParameters(texParams);
    lyBG->addChild(sprIcon);
    {
        if(isHave)
        {
            switch (_type)
            {
                case E_DF_Trench_SettingType::CHARACTER:
                {
                    auto anim = Animation::create();
                    for(int i = 0; i < 2; ++i)
                    {
                        std::string strPath = MafUtils::format("Assets/ui/defense/Defense_mole_lv%d_%02d.png", _idx, i);
                        
                        auto sprite = Sprite::create(strPath);
                        sprite->getTexture()->setTexParameters(texParams);
                        anim->addSpriteFrame(sprite->getSpriteFrame());
                    }
                    anim->setDelayPerUnit(0.3f);
                    auto animate = Animate::create(anim);
                    sprIcon->runAction(RepeatForever::create(animate));
                    
                }break;
                case E_DF_Trench_SettingType::WEAPON:
                {
                    auto action1 = MoveBy::create(1.f,Vec2(0, 15));
                    auto action2 = action1->reverse();
                    auto seq = Sequence::create(action1,action2, NULL);
                    sprIcon->runAction(RepeatForever::create(seq));
                }break;
                    
                default:
                    break;
            }
        }
        else
        {
            sprIcon->setColor(Color3B(15, 15, 15));
        }
    }
    
    auto btnNext = ui::Button::create("Assets/ui_common/arrow_R06.png");
    btnNext->setScale(1.3f);
    btnNext->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    btnNext->setPosition(Vec2(lyBG->getContentSize().width - 30, lyBG->getContentSize().height/2));
    btnNext->setZoomScale(0.05f);
    btnNext->setName("NEXT");
    btnNext->addClickEventListener(CC_CALLBACK_1(PopupDefenseObjectInfo::onClickAnother, this));
    lyBG->addChild(btnNext);
    
    auto btnPrev = ui::Button::create("Assets/ui_common/arrow_L06.png");
    btnPrev->setScale(1.3f);
    btnPrev->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    btnPrev->setPosition(Vec2(30, lyBG->getContentSize().height/2));
    btnPrev->setZoomScale(0.05f);
    btnPrev->setName("PREV");
    btnPrev->addClickEventListener(CC_CALLBACK_1(PopupDefenseObjectInfo::onClickAnother, this));
    lyBG->addChild(btnPrev);
    
    Vec2 trenchNumberLocation = Vec2(5, lyBG->getContentSize().height - 5);
    for(int i = 0; i < DefenseManager::getInstance()->getMaxTrenchCount(); ++i)
    {
        std::string boxPath = "Assets/ui/defense/Ui_linenumber_box_gray.png";
        if(i + 1 == trenchNumver)
            boxPath = "Assets/ui/defense/Ui_linenumber_box_blue.png";
        auto trenchNumberBox = Sprite::create(boxPath);
        trenchNumberBox->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        trenchNumberBox->setPosition(trenchNumberLocation);
        lyBG->addChild(trenchNumberBox);
        
        auto lbNumber = MafNode::createBmFont(MafUtils::format("%d", i + 1), GAME_BM_FONT_AL, 25);
        lbNumber->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        lbNumber->setPosition(trenchNumberBox->getContentSize()/2);
        lbNumber->setColor(Color3B::GRAY);
        if(i + 1 == trenchNumver)
            lbNumber->setColor(Color3B(85,240,255));
        trenchNumberBox->addChild(lbNumber);
        
        trenchNumberLocation += Vec2::RIGHT* (trenchNumberBox->getContentSize().width + 5);
    }
    
    /*
     bongDamage = 0;
     bongGold = 0;
     dfDamage = 0;
     dfAttackSpeed = 0;
     */
    std::vector<std::pair<std::string, double>> listEffect;
    
    switch (_type) {
        case E_DF_Trench_SettingType::CHARACTER:
        {
            listEffect = {
                std::make_pair(GAME_TEXT("t_ui_town_msg_2"), bongDamage),
                std::make_pair(GAME_TEXT("t_ui_defense_setting_character_total"), dfDamage),
                std::make_pair(GAME_TEXT("t_ui_defense_atk_spd"), dfAttackSpeed)
            };
        }
            break;
        case E_DF_Trench_SettingType::WEAPON:
        {
            listEffect = {
                std::make_pair(GAME_TEXT("t_ui_town_msg_2"), bongDamage),
                std::make_pair(GAME_TEXT("t_ui_defense_setting_weapon_total"), dfDamage),
                std::make_pair(GAME_TEXT("t_ui_defense_atk_spd"), dfAttackSpeed)
            };
        }break;
        default:
            break;
    }
//
    for( int i = 0; i < listEffect.size(); i++)
    {
        auto obj = listEffect[i];

        double posY = (lyInfo->getContentSize().height - 10) - 25 * i - 25 * i;

        auto lbTitle = Label::createWithTTF(obj.first, GAME_FONT, 26);
        lbTitle->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        lbTitle->setPosition(15, posY);
        lyInfo->addChild(lbTitle);

        double nEffect = obj.second;
        std::string strEffect = MafUtils::doubleToString(nEffect);
        
        if(nEffect >= 1000000)
            MafUtils::convertNumberToShort(strEffect);
        
        if(i == 0)
            strEffect += "%";
        if(i == 2 && _type == E_DF_Trench_SettingType::CHARACTER)
            strEffect += "%";
        
        auto lbEffect = Label::createWithTTF(strEffect, GAME_FONT, 26);
        lbEffect->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
        lbEffect->setPosition(lyInfo->getContentSize().width - 15, posY);
        lbEffect->setDimensions(lyInfo->getContentSize().width/2, lbEffect->getContentSize().height);
        lbEffect->setAlignment(cocos2d::TextHAlignment::RIGHT, cocos2d::TextVAlignment::TOP);
        lbEffect->setOverflow(cocos2d::Label::Overflow::SHRINK);
        lyInfo->addChild(lbEffect);
        {
            auto menu = MafNode::MafMenu::create();
            menu->setPosition(Vec2::ZERO);
            lbEffect->addChild(menu);

            auto area = Layer::create();
            area->setContentSize(lbEffect->getContentSize());

            auto btn = MafNode::MafMenuItemSprite::create(area, nullptr, nullptr, [=](Ref*){
                Vec2 posToast = lyInfo->getParent()->convertToWorldSpace(lyInfo->getPosition());
                posToast.y -= lyInfo->getContentSize().height / 2.f;

                std::string strEffect = MafUtils::doubleToString(nEffect);
                strEffect = MafUtils::addCommaToString(strEffect, 3, ',');
                strEffect += "%";
                PopupToast::show(strEffect, posToast.x, posToast.y);
            });
            btn->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            btn->setPosition(lbEffect->getContentSize()/2);
            menu->addChild(btn);
        }
    }
    
    auto sprInfoBox = ui::Scale9Sprite::create("Assets/ui/defense/Ui_name_box.png");
    sprInfoBox->setContentSize(Size(434,88));
    sprInfoBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    sprInfoBox->setPosition(lyBG->getContentSize().width/2, lyBG->getContentSize().height - lyBG->getContentSize().height - 10);
    lyBG->addChild(sprInfoBox);
    {
        std::string strTitle = GAME_TEXT("t_ui_revive_msg_2");
        
        if(isHave)
        {
            strTitle = MafUtils::format("{#FCDD1A:%s}\nLv.%d %s", GAME_TEXT(MafUtils::format("t_ui_spirit_grade_%d",grade)).c_str(), lv, name.c_str());
        }
        
        auto textObjectInfo = ui::CText::create(strTitle, GAME_FONT, 26);
        textObjectInfo->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        textObjectInfo->setPosition(sprInfoBox->getContentSize()/2);
        textObjectInfo->setTextAreaSize(Size(sprInfoBox->getContentSize().width - 5, sprInfoBox->getContentSize().height - 25));
        textObjectInfo->setTextVerticalAlignment(TextVAlignment::CENTER);
        textObjectInfo->setTextHorizontalAlignment(TextHAlignment::CENTER);
        textObjectInfo->setTextOverflow(Label::Overflow::SHRINK);
        sprInfoBox->addChild(textObjectInfo);
    }
    
    auto sprSmeltingBox = ui::Scale9Sprite::create("Assets/ui/defense/Ui_smelting_box.png");
    sprSmeltingBox->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    sprSmeltingBox->setPosition(Size(_lyTopUI->getContentSize() - Size(10)));
    _lyTopUI->addChild(sprSmeltingBox);
    {
        auto sprSmelting = Sprite::create("Assets/ui_common/icon_star1_3.png");
        sprSmelting->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        sprSmelting->setPosition(sprSmeltingBox->getContentSize().width/2 - 3, sprSmeltingBox->getContentSize().height/2);
        sprSmeltingBox->addChild(sprSmelting);
        
        auto lbSmelting = MafNode::createBmFont(MafUtils::format("%d", smelting), GAME_BM_FONT_AL, 25);
        lbSmelting->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        lbSmelting->setPosition(Vec2(sprSmeltingBox->getContentSize().width/2 + 2, sprSmeltingBox->getContentSize().height/2));
        if(smelting >= 10)
            lbSmelting->setPosition(Vec2(sprSmeltingBox->getContentSize().width/2 -2, sprSmeltingBox->getContentSize().height/2));
        lbSmelting->setColor(COLOR_TITLE);
        sprSmeltingBox->addChild(lbSmelting);
    }
}

void PopupDefenseObjectInfo::uiMiddle()
{
    _lyMiddleUI->removeAllChildren();
    
    auto sprBoxBG = ui::Scale9Sprite::create("Assets/ui_common/box_item.png");
    sprBoxBG->setContentSize(Size(_lyMiddleUI->getContentSize().width - 10, _lyMiddleUI->getContentSize().height));
    sprBoxBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    sprBoxBG->setPosition(_lyMiddleUI->getContentSize().width/2, 0);
    _lyMiddleUI->addChild(sprBoxBG);
    
    uiLoadLayer();
}

void PopupDefenseObjectInfo::uiBottom()
{
    _lyBottomUI->removeAllChildren();
    
    std::vector<PopupDefenseObjectInfo::E_TYPE> listTab = {
        PopupDefenseObjectInfo::E_TYPE::RANK_UP, PopupDefenseObjectInfo::E_TYPE::LEVEL_UP,
    PopupDefenseObjectInfo::E_TYPE::SMELTING
    };

    int size = (int)listTab.size();

    float width = _lyBottomUI->getContentSize().width / size - 10;
    float posX = _lyBottomUI->getContentSize().width/2 - width * (size/2) - 10 * (size/2);

    for(int i = 0; i < listTab.size(); ++i)
    {
        auto obj = listTab[i];

        std::string strPath = "Assets/ui_common/btn_tap_2_off.png";
        if(obj == _tabType)
            strPath = "Assets/ui_common/btn_tap_2_on.png";
        std::string strText = "";
        switch (obj) {
            case PopupDefenseObjectInfo::E_TYPE::RANK_UP: strText = GAME_TEXT("t_ui_promo"); break;
            case PopupDefenseObjectInfo::E_TYPE::LEVEL_UP: strText = GAME_TEXT("t_ui_level_up_1"); break;
            case PopupDefenseObjectInfo::E_TYPE::SMELTING: strText = GAME_TEXT("t_ui_smelting_1"); break;
            default:  break;
        }

        auto btnTab = ui::Button::create(strPath);
        btnTab->setScale9Enabled(true);
        btnTab->setContentSize(Size(width, 80));
        btnTab->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        btnTab->setPosition(Vec2(posX, _lyBottomUI->getContentSize().height/2));
        btnTab->addClickEventListener(CC_CALLBACK_1(PopupDefenseObjectInfo::onClickTab, this));
        btnTab->setZoomScale(0.05f);
        btnTab->setTag((int)obj);
        btnTab->setTitleText(strText);
        btnTab->setTitleAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
        btnTab->getTitleLabel()->setDimensions(btnTab->getContentSize().width, btnTab->getContentSize().height);
        btnTab->getTitleLabel()->enableBold();
        btnTab->setTitleFontSize(30);
        _lyBottomUI->addChild(btnTab);


        if(size % 2 == 0)
            btnTab->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);

        posX += width + 10;
    }
    
}

void PopupDefenseObjectInfo::uiReload()
{
    bool isHave = false;
    
    switch(_type)
    {
        case E_DF_Trench_SettingType::CHARACTER: isHave = _characterData->isHave(); break;
        case E_DF_Trench_SettingType::WEAPON: isHave = _weaponData->isHave(); break;
        default: break;
    }
    
    if(!isHave)
        _tabType = PopupDefenseObjectInfo::E_TYPE::RANK_UP;

    uiTop();
    uiLoadLayer();
    uiBottom();
}

void PopupDefenseObjectInfo::uiLoadLayer()
{
    if(_lyContainerNow != nullptr)
        _lyContainerNow->setVisible(false);

    switch(_tabType)
    {
        case PopupDefenseObjectInfo::E_TYPE::RANK_UP:
        {
            if(_lyRankUp == nullptr)
            {
                auto layer = UILayerDefenseObjectRankUp::create(_lyMiddleUI->getContentSize(), _type, _idx);
                layer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                layer->setPosition(_lyMiddleUI->getContentSize()/2);
                layer->setIgnoreAnchorPointForPosition(false);
                layer->_onHide += new MafFunction<void(void)>(CC_CALLBACK_0(PopupDefenseObjectInfo::hide, this), this);
                _lyMiddleUI->addChild(layer);
                _onEventChangeObject += new MafFunction<void(int)>(CC_CALLBACK_1(UILayerDefenseObjectRankUp::onChangeObject, layer), layer);
                _lyRankUp = layer;
            }
            _lyContainerNow = _lyRankUp;
        }break;
        case PopupDefenseObjectInfo::E_TYPE::SMELTING:
        {
            if(_lySmeltingUp == nullptr)
            {
                auto layer = UILayerDefenseObjectSmelting::create(_lyMiddleUI->getContentSize(),_type, _idx);
                layer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                layer->setPosition(_lyMiddleUI->getContentSize()/2);
                layer->_onHide += new MafFunction<void(void)>(CC_CALLBACK_0(PopupDefenseObjectInfo::hide, this), this);
                _lyMiddleUI->addChild(layer);
                _onEventChangeObject += new MafFunction<void(int)>(CC_CALLBACK_1(UILayerDefenseObjectSmelting::onChangeObject, layer), layer);
                _lySmeltingUp = layer;
            }
            _lyContainerNow = _lySmeltingUp;
        }break;
        case PopupDefenseObjectInfo::E_TYPE::LEVEL_UP:
        {
            if(_lyLevelUp == nullptr)
            {
                auto layer = UILayerDefenseObjectLevelup::create(_lyMiddleUI->getContentSize(), _type, _idx);
                layer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                layer->setPosition(_lyMiddleUI->getContentSize()/2);
                layer->setIgnoreAnchorPointForPosition(false);
                layer->_onHide += new MafFunction<void(void)>(CC_CALLBACK_0(PopupDefenseObjectInfo::hide, this), this);
                _lyMiddleUI->addChild(layer);
                _onEventChangeObject += new MafFunction<void(int)>(CC_CALLBACK_1(UILayerDefenseObjectLevelup::onChangeObject, layer), layer);
                _lyLevelUp = layer;
            }
            _lyContainerNow = _lyLevelUp;
        }break;
    }

    _lyContainerNow->setVisible(true);
    
    _onEventChangeObject.Invoke(_idx);
}

#pragma mark -utils
void PopupDefenseObjectInfo::onClickClose(Ref* sender)
{
    hide();
}

void PopupDefenseObjectInfo::onClickAnother(Ref* sender)
{
    auto node = (Node*)sender;
    
    int maxCount = 0;
    
    switch(_type)
    {
        case E_DF_Trench_SettingType::CHARACTER:
        {
            maxCount = DefenseManager::getInstance()->getCharacterDataCount();
        }break;
        case E_DF_Trench_SettingType::WEAPON:
        {
            maxCount = DefenseManager::getInstance()->getWeaponDataCount();
        }break;
        default: break;
    }
    
    if(node->getName().compare("NEXT") == 0)
    {
        _idx +=1;
        
        if(_idx > maxCount)
            _idx = 1;
    }
    else if(node->getName().compare("PREV") == 0)
    {
        _idx -=1;
        if(_idx <= 0)
            _idx = maxCount;
    }
    
    switch(_type)
    {
        case E_DF_Trench_SettingType::CHARACTER: _characterData = DefenseManager::getInstance()->getCharacterData(_idx); break;
        case E_DF_Trench_SettingType::WEAPON: _weaponData = DefenseManager::getInstance()->getWeaponData(_idx); break;
        default: break;
    }
        
    uiReload();
}

void PopupDefenseObjectInfo::onClickTab(Ref* sender)
{
    bool isHave = false;
    switch(_type)
    {
        case E_DF_Trench_SettingType::CHARACTER :
        {
            if(_characterData == nullptr)
                return;
            isHave = _characterData->isHave();
        }break;
        case E_DF_Trench_SettingType::WEAPON :
        {
            if(_weaponData == nullptr)
                return;
            isHave = _weaponData->isHave();
        }break;
        default: break;
    }
    
    //if(!isHave)
        //return;

    auto node = (Node*)sender;
    _tabType = (PopupDefenseObjectInfo::E_TYPE)node->getTag();

    uiReload();
}

#pragma mark -callback
void PopupDefenseObjectInfo::callbackServer(bool bResult, int nResult)
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
        
        hide();
        return;
    }
    uiTop();
    uiBottom();
}
