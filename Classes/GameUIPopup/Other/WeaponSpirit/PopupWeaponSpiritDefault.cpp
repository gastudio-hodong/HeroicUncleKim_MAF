//
//  PopupWeaponSpiritDefault.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/11/23.
//

#include "PopupWeaponSpiritDefault.h"

#include "Common/ConfigGameNormal.h"

#include "GameUIPopup/Other/WeaponSpirit/Layer/UILayerSpiritRankUp.h"
#include "GameUIPopup/Other/WeaponSpirit/Layer/UILayerSpiritLevelup.h"
#include "GameUIPopup/Other/WeaponSpirit/Layer/UILayerSpiritSmelting.h"
#include "GameUIPopup/Other/WeaponSpirit/PopupSpiritBuffInfo.h"

#include "ManagerGame/WeaponSpiritManager.h"
#include "ManagerGame/NewRaidManager.h"

PopupWeaponSpiritDefault* PopupWeaponSpiritDefault::create(InfoWeaponSpirit* infoSpirit)
{
    PopupWeaponSpiritDefault* pRet = new(std::nothrow) PopupWeaponSpiritDefault();
    if ( pRet && pRet->init(infoSpirit) )
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

PopupWeaponSpiritDefault::PopupWeaponSpiritDefault(void):
_eType(PopupWeaponSpiritDefault::E_TYPE::RANK_UP)
,_infoSpirit(nullptr)
,_lyTopUI(nullptr)
,_lyMiddleUI(nullptr)
,_lyBottomUI(nullptr)
,_lbSpiritInfo(nullptr)
,_lyContainerNow(nullptr)
,_lyContainerRankUp(nullptr)
,_lyContainerSmelting(nullptr)
,_lyContainerLevelUp(nullptr)
{
}

PopupWeaponSpiritDefault::~PopupWeaponSpiritDefault(void)
{
}

#pragma mark -init
bool PopupWeaponSpiritDefault::init(InfoWeaponSpirit* infoSpirit)
{
    if ( !PopupBase::init())
    {
        return false;
    }
    
    _infoSpirit = infoSpirit;
    
    if(!WeaponSpiritManager::getInstance()->isFirstSummon(_infoSpirit->getIdx()-1) && _infoSpirit->isHas())
    {
        WeaponSpiritManager::getInstance()->setFirstSummon(_infoSpirit->getIdx()-1);
    }
    
    initVar();
    initUI();
    
    return true;
}

void PopupWeaponSpiritDefault::initVar()
{
    _spriteContainer->setContentSize(Size(712, 1268));
    _spriteContainer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _spriteContainer->setPosition(getContentSize().width / 2, 0);
}

void PopupWeaponSpiritDefault::initUI()
{
    // title
    
    auto lbTitle = Label::createWithTTF(GAME_TEXT("t_ui_weapon_spirit_1"), GAME_FONT, 28);
    lbTitle->setColor(COLOR_TITLE);
    lbTitle->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5) );
    _spriteContainer->addChild(lbTitle);
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupWeaponSpiritDefault::onClickClose, this));
    itemClose->setPosition( Vec2(_spriteContainer->getContentSize().width - itemClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5) );
    itemClose->setTouchRect( itemClose->getContentSize() * 2 );
    menu->addChild(itemClose);
    
    
    _lyTopUI = LayerColor::create();
    _lyTopUI->setContentSize( Size(_spriteContainer->getContentSize().width - 10, 550));
    _lyTopUI->setIgnoreAnchorPointForPosition(false);
    _lyTopUI->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _lyTopUI->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75));
    _spriteContainer->addChild(_lyTopUI);
    
    _lyMiddleUI = LayerColor::create();
    _lyMiddleUI->setContentSize( Size(_spriteContainer->getContentSize().width - 10, 470));
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
void PopupWeaponSpiritDefault::uiTop()
{
    _lyTopUI->removeAllChildren();
    
    auto sprBg = Sprite::create("Assets/ui/spirit/spirit_bg1_1.png");
    sprBg->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    sprBg->setPosition(_lyTopUI->getContentSize().width/2, _lyTopUI->getContentSize().height);
    _lyTopUI->addChild(sprBg);
    {
        auto sprEffect = Sprite::create("Assets/ui/spirit/spirit_bg1_2.png");
        sprEffect->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        sprEffect->setPosition(sprBg->getContentSize()/2);
        sprBg->addChild(sprEffect);
        
        auto fadeOut = FadeOut::create(1.f);
        auto fadeIn = FadeIn::create(1.f);
        auto seq = Sequence::create(fadeOut,fadeIn, NULL);
        sprEffect->runAction(RepeatForever::create(seq));
    }
    
    auto sprSpirit = Sprite::create(_infoSpirit->getResourcePath());
    sprSpirit->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    sprSpirit->setPosition(_lyTopUI->getContentSize().width/2, _lyTopUI->getContentSize().height + 20);
    sprSpirit->setScale(2);
    sprSpirit->getTexture()->setTexParameters(texParams);
    _lyTopUI->addChild(sprSpirit);
    {
        auto action1 = MoveBy::create(1.f,Vec2(0, 15));
        auto action2 = action1->reverse();
        auto seq = Sequence::create(action1,action2, NULL);
        sprSpirit->runAction(RepeatForever::create(seq));
        
        if(!_infoSpirit->isHas())
            sprSpirit->setColor(Color3B(15, 15, 15));
    }
    
    auto btnNext = ui::Button::create("Assets/ui_common/arrow_R06.png");
    btnNext->setScale(1.3f);
    btnNext->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    btnNext->setPosition(Vec2(sprBg->getContentSize().width - 50, sprBg->getContentSize().height/2));
    btnNext->setZoomScale(0.05f);
    btnNext->setName("NEXT");
    btnNext->addClickEventListener(CC_CALLBACK_1(PopupWeaponSpiritDefault::onClickAnotherSpirit, this));
    sprBg->addChild(btnNext);
    
    auto btnPrev = ui::Button::create("Assets/ui_common/arrow_L06.png");
    btnPrev->setScale(1.3f);
    btnPrev->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    btnPrev->setPosition(Vec2(50, sprBg->getContentSize().height/2));
    btnPrev->setZoomScale(0.05f);
    btnPrev->setName("PREV");
    btnPrev->addClickEventListener(CC_CALLBACK_1(PopupWeaponSpiritDefault::onClickAnotherSpirit, this));
    sprBg->addChild(btnPrev);
    
    
    std::vector<std::pair<std::string, double>> listEffect = {
        std::make_pair(GAME_TEXT("t_ui_town_msg_2"), _infoSpirit->getEffectDmg()),
        std::make_pair(GAME_TEXT("t_ui_town_msg_5"), _infoSpirit->getEffectGold()),
        std::make_pair(GAME_TEXT("t_ui_town_msg_3"), _infoSpirit->getEffectKey())
    };
    
    for( int i = 0; i < listEffect.size(); i++)
    {
        auto obj = listEffect[i];
        
        double posY = (_lyTopUI->getContentSize().height - sprBg->getContentSize().height - 25) - 25 * i - 25 * i;
        
        auto lbTitle = Label::createWithTTF(obj.first, GAME_FONT, 26);
        lbTitle->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        lbTitle->setPosition(15, posY);
        _lyTopUI->addChild(lbTitle);
        
        double nEffect = obj.second;
        std::string strEffect = MafUtils::doubleToString(nEffect);
        MafUtils::convertNumberToShort(strEffect);
        
        auto lbEffect = Label::createWithTTF(strEffect + "%", GAME_FONT, 26);
        lbEffect->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
        lbEffect->setPosition(_lyTopUI->getContentSize().width - 15, posY);
        lbEffect->setDimensions(_lyTopUI->getContentSize().width/2, lbEffect->getContentSize().height);
        lbEffect->setAlignment(cocos2d::TextHAlignment::RIGHT, cocos2d::TextVAlignment::TOP);
        lbEffect->setOverflow(cocos2d::Label::Overflow::SHRINK);
        _lyTopUI->addChild(lbEffect);
        {
            auto menu = MafNode::MafMenu::create();
            menu->setPosition(Vec2::ZERO);
            lbEffect->addChild(menu);
            
            auto area = Layer::create();
            area->setContentSize(lbEffect->getContentSize());
            
            auto btn = MafNode::MafMenuItemSprite::create(area, nullptr, nullptr, [=](Ref*){
                Vec2 posToast = _lyTopUI->getParent()->convertToWorldSpace(_lyTopUI->getPosition());
                posToast.y -= _lyTopUI->getContentSize().height / 2.f;
                
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
    
    std::string strEquip = GAME_TEXT("t_ui_costume_equip");
    std::string strEquipPath = "Assets/ui/spirit/btn_spirit_mount_off.png";
    
    if(_infoSpirit->isWear())
    {
        strEquip = GAME_TEXT("t_ui_costume_equip_now");
        strEquipPath = "Assets/ui/spirit/btn_spirit_mount_on.png";
    }
    auto btnEquip = ui::Button::create(strEquipPath);
    btnEquip->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    btnEquip->setPosition(Vec2(_lyTopUI->getContentSize().width - 10, _lyTopUI->getContentSize().height - 5));
    btnEquip->addClickEventListener(CC_CALLBACK_1(PopupWeaponSpiritDefault::onClickEquip, this));
    btnEquip->setZoomScale(0.05f);
    _lyTopUI->addChild(btnEquip);
    {
        auto lbText = Label::createWithTTF(strEquip, GAME_FONT, 24);
        lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        lbText->setPosition(btnEquip->getContentSize().width/2, btnEquip->getContentSize().height - 23);
        lbText->setDimensions(btnEquip->getContentSize().width - 5, btnEquip->getContentSize().height/2);
        lbText->setAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
        lbText->setOverflow(cocos2d::Label::Overflow::SHRINK);
        btnEquip->addChild(lbText);
    }
    if(!_infoSpirit->isHas())
        btnEquip->setEnabled(false);
    
    auto sprInfoBox = ui::Scale9Sprite::create("Assets/ui_common/base_text_1_5.png");
    sprInfoBox->setContentSize(Size(434,88));
    sprInfoBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    sprInfoBox->setPosition(_lyTopUI->getContentSize().width/2, _lyTopUI->getContentSize().height - sprBg->getContentSize().height);
    _lyTopUI->addChild(sprInfoBox);
    {
        std::string strTitle = GAME_TEXT("t_ui_revive_msg_2");
        
        if(_infoSpirit->isHas())
        {
            strTitle = MafUtils::format("%s\nLv.%d %s", GAME_TEXT(MafUtils::format("t_ui_spirit_grade_%d",_infoSpirit->getGrade())).c_str(), _infoSpirit->getCurrentLv(), _infoSpirit->getText().c_str());
        }
        
        _lbSpiritInfo = Label::createWithTTF(strTitle, GAME_FONT, 28);
        _lbSpiritInfo->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        _lbSpiritInfo->setPosition(sprInfoBox->getContentSize()/2);
        _lbSpiritInfo->setDimensions(sprInfoBox->getContentSize().width - 5, sprInfoBox->getContentSize().height - 10);
        _lbSpiritInfo->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        _lbSpiritInfo->setOverflow(Label::Overflow::SHRINK);
        sprInfoBox->addChild(_lbSpiritInfo);
    }
    
    auto sprSmeltingBox = ui::Scale9Sprite::create("Assets/ui_common/btn_2_6_on.png");
    sprSmeltingBox->setContentSize(Size(100, 55));
    sprSmeltingBox->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    sprSmeltingBox->setPosition(sprInfoBox->getPosition() + Vec2::RIGHT * (sprInfoBox->getContentSize().width/2 - 30) + Vec2::UP * sprInfoBox->getContentSize().height);
    _lyTopUI->addChild(sprSmeltingBox);
    {
        auto sprIcon = Sprite::create("Assets/ui_common/icon_star1_2.png");
        sprIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        sprIcon->setPosition(sprSmeltingBox->getContentSize().width/4, sprSmeltingBox->getContentSize().height/2);
        sprSmeltingBox->addChild(sprIcon);
        
        auto lbCount = Label::createWithTTF(MafUtils::format("%02d",_infoSpirit->getCurrentSmelting()), GAME_FONT, 25);
        lbCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        lbCount->setPosition(sprIcon->getPosition() + Vec2::RIGHT * (sprIcon->getContentSize().width + 3));
        lbCount->setDimensions(sprSmeltingBox->getContentSize().width - 5 - sprIcon->getContentSize().width - 8, sprSmeltingBox->getContentSize().height);
        lbCount->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
        lbCount->setOverflow(Label::Overflow::SHRINK);
        lbCount->enableBold();
        lbCount->setColor(COLOR_COIN);
        sprSmeltingBox->addChild(lbCount);
    }
    
    int contractLv = WeaponSpiritManager::getInstance()->getContractLv();
    int count = contractLv;
    int maxCount = _infoSpirit->getGrade() - 1;
    switch(_infoSpirit->getGrade())
    {
        case 1 : count = 0; break;
        case 2 :
        {
            if(count >= 1)
                count = 1;
        }break;
        case 3 :
        {
            if(count >= 2)
                count = 2;
        }break;
        case 4 :
        {
            if(count >= 3)
                count = 3;
        }break;
        case 5 :
        {
            if(count >= 4)
                count = 4;
        }break;
        case 6 :
        {
            if(count >= 5)
                count = 5;
        }break;
        default:
        {
            if(maxCount >= 5)
                maxCount = 5;
                
            if(count >= 5)
                count = 5;
        }
    }
    
    if(!_infoSpirit->isHas())
        count = 0;
    
    Vec2 pos = Vec2(5, _lyTopUI->getContentSize().height - 5);
    for(int i = 0; i < maxCount; ++i)
    {
        std::string strPath = "Assets/ui/spirit/spirit_contract_1_0.png";
        if(i + 1 <= count)
        {
            strPath = MafUtils::format("Assets/ui/spirit/spirit_contract_1_%d.png", i + 1);
        }
        auto sprContractSton = Sprite::create(strPath);
        sprContractSton->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        sprContractSton->getTexture()->setTexParameters(texParams);
        sprContractSton->setScale(0.6f);
        sprContractSton->setPosition(pos);
        _lyTopUI->addChild(sprContractSton);
        
        if(i + 1 <= count)
        {
            auto sprFx = Sprite::create("Assets/ui/spirit/spirit_contract_2_0.png");
            sprFx->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            sprFx->setPosition(sprContractSton->getContentSize()/2);
            sprContractSton->addChild(sprFx);
        }
        pos += Vec2::RIGHT * (sprContractSton->getContentSize().width * sprContractSton->getScale() + 5);
    }
    
    if(_infoSpirit->getGrade() >=6 && _infoSpirit->isHas())
    {
        auto menu = MafNode::MafMenu::create();
        menu->setAnchorPoint(Vec2::ZERO);
        menu->setPosition(Vec2::ZERO);
        _lyTopUI->addChild(menu);
        
        auto sprBuff = ui::Scale9Sprite::create("Assets/ui_common/btn_2_2_on.png");
        sprBuff->setContentSize(Size(126, 46));
        
        auto btnBuff = MafNode::MafMenuItemSprite::create(sprBuff, nullptr, nullptr, CC_CALLBACK_1(PopupWeaponSpiritDefault::onClickBuff, this));
        btnBuff->setContentSize(Size(126, 46));
        btnBuff->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
        btnBuff->setPosition(Vec2(_lyTopUI->getContentSize().width - 20, _lyTopUI->getContentSize().height - sprBg->getContentSize().height - 5));
        menu->addChild(btnBuff);
        {
            auto lbText = Label::createWithTTF(GAME_TEXT("t_ui_spirit_buff_bt"), GAME_FONT, 26);
            lbText->setAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
            lbText->setDimensions(btnBuff->getContentSize().width, btnBuff->getContentSize().height);
            lbText->setOverflow(cocos2d::Label::Overflow::SHRINK);
            lbText->setPosition(btnBuff->getContentSize()/2);
            btnBuff->addChild(lbText);
            
            auto objSettingMaxLv = NewRaidManager::getInstance()->getRaidSetting("spirit_max_lv");
            int nMaxLv = 99;
            
            if(objSettingMaxLv != nullptr)
                nMaxLv = objSettingMaxLv->getValueToInt();
            
            if(_infoSpirit->isWear() &&
               _infoSpirit->getGrade() >= 6 &&
               _infoSpirit->getCurrentLv() >= nMaxLv &&
               _infoSpirit->getCurrentSmelting() >= 1)
            {
                auto sprBuff = (ui::Scale9Sprite*)btnBuff->getNormalImage();
                sprBuff->setTexture("Assets/ui/spirit/btn_spirit_buf_on_01.png");
                btnBuff->setPosition(btnBuff->getPosition() + Vec2::DOWN * 10 + Vec2::LEFT * 10);
                
                lbText->setPosition(lbText->getPosition() + Vec2::UP * 10 + Vec2::RIGHT * 10);
                
                auto anim = Animation::create();
                anim->setDelayPerUnit(0.1f);

                for(int i = 0; i < 12; ++i)
                {
                    std::string sprPath = MafUtils::format("Assets/ui/spirit/btn_spirit_buf_on_%02d.png", i+1);
                    anim->addSpriteFrameWithFile(sprPath);
                }
                auto animate = Animate::create(anim);
                auto repeat = RepeatForever::create(animate);
                btnBuff->getNormalImage()->runAction(repeat);
                
            }
        }
        
        
    }
}

void PopupWeaponSpiritDefault::uiMiddle()
{
    _lyMiddleUI->removeAllChildren();
    
    auto sprBoxBG = ui::Scale9Sprite::create("Assets/ui_common/box_item.png");
    sprBoxBG->setContentSize(Size(_lyMiddleUI->getContentSize().width - 10, _lyMiddleUI->getContentSize().height));
    sprBoxBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    sprBoxBG->setPosition(_lyMiddleUI->getContentSize().width/2, 0);
    _lyMiddleUI->addChild(sprBoxBG);
    
    uiLoadLayer();
}

void PopupWeaponSpiritDefault::uiBottom()
{
    _lyBottomUI->removeAllChildren();
    
    std::vector<PopupWeaponSpiritDefault::E_TYPE> listTab = {
        PopupWeaponSpiritDefault::E_TYPE::RANK_UP, PopupWeaponSpiritDefault::E_TYPE::LEVEL_UP,
    };
    
    
    auto objSmeltingSetting = NewRaidManager::getInstance()->getRaidSetting("spirit_smelting");
    
    if(objSmeltingSetting->getValueToBool())
        listTab.push_back(PopupWeaponSpiritDefault::E_TYPE::SMELTING);
    
    int size = (int)listTab.size();
    
    float width = _lyBottomUI->getContentSize().width / size - 10;
    float posX = _lyBottomUI->getContentSize().width/2 - width * (size/2) - 10 * (size/2);
    
    for(int i = 0; i < listTab.size(); ++i)
    {
        auto obj = listTab[i];
        
        std::string strPath = "Assets/ui_common/btn_tap_2_off.png";
        if(obj == _eType)
            strPath = "Assets/ui_common/btn_tap_2_on.png";
        std::string strText = "";
        switch (obj) {
            case PopupWeaponSpiritDefault::E_TYPE::RANK_UP: strText = GAME_TEXT("t_ui_spirit_rank_up_1"); break;
            case PopupWeaponSpiritDefault::E_TYPE::LEVEL_UP: strText = GAME_TEXT("t_ui_level_up_1"); break;
            case PopupWeaponSpiritDefault::E_TYPE::SMELTING: strText = GAME_TEXT("t_ui_smelting_1"); break;
            default:  break;
        }
        
        auto btnTab = ui::Button::create(strPath);
        btnTab->setScale9Enabled(true);
        btnTab->setContentSize(Size(width, 80));
        btnTab->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        btnTab->setPosition(Vec2(posX, _lyBottomUI->getContentSize().height/2));
        btnTab->addClickEventListener(CC_CALLBACK_1(PopupWeaponSpiritDefault::onClickTab, this));
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

void PopupWeaponSpiritDefault::uiReload()
{
    std::string strTitle = "???";
    
    if(_infoSpirit->isHas())
        strTitle = _infoSpirit->getText() + MafUtils::format("(Lv.%d)", _infoSpirit->getCurrentLv());
    _lbSpiritInfo->setString(strTitle);
    
    uiTop();
    
    if(!_infoSpirit->isHas())
        _eType = PopupWeaponSpiritDefault::E_TYPE::RANK_UP;
    
    uiLoadLayer();
    uiBottom();
}

void PopupWeaponSpiritDefault::uiLoadLayer()
{
    if(_lyContainerNow != nullptr)
        _lyContainerNow->setVisible(false);
    
    switch(_eType)
    {
        case PopupWeaponSpiritDefault::E_TYPE::RANK_UP:
        {
            if(_lyContainerRankUp == nullptr)
            {
                auto layer = UILayerSpiritRankUp::create(_lyMiddleUI->getContentSize(), _infoSpirit);
                layer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                layer->setPosition(_lyMiddleUI->getContentSize()/2);
                layer->setCallbackRedraw(CC_CALLBACK_0(PopupWeaponSpiritDefault::uiReload, this));
                _lyMiddleUI->addChild(layer);
                
                _lyContainerRankUp = layer;
            }
            _lyContainerNow = _lyContainerRankUp;
        }break;
        case PopupWeaponSpiritDefault::E_TYPE::SMELTING:
        {
            if(_lyContainerSmelting == nullptr)
            {
                auto layer = UILayerSpiritSmelting::create(_lyMiddleUI->getContentSize(), _infoSpirit);
                layer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                layer->setPosition(_lyMiddleUI->getContentSize()/2);
                layer->setCallbackRedraw(CC_CALLBACK_0(PopupWeaponSpiritDefault::uiReload, this));
                _lyMiddleUI->addChild(layer);
                
                _lyContainerSmelting = layer;
            }
            _lyContainerNow = _lyContainerSmelting;
        }break;
        case PopupWeaponSpiritDefault::E_TYPE::LEVEL_UP:
        {
            if(_lyContainerLevelUp == nullptr)
            {
                auto layer = UILayerSpiritLevelup::create(_lyMiddleUI->getContentSize(), _infoSpirit);
                layer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                layer->setPosition(_lyMiddleUI->getContentSize()/2);
                layer->setCallbackRedraw(CC_CALLBACK_0(PopupWeaponSpiritDefault::uiReload, this));
                _lyMiddleUI->addChild(layer);
                
                _lyContainerLevelUp = layer;
            }
            _lyContainerNow = _lyContainerLevelUp;
        }break;
    }
    
    _lyContainerNow->setVisible(true);
    _lyContainerNow->refreshUI(_infoSpirit);
}

#pragma mark -utils
void PopupWeaponSpiritDefault::onClickClose(Ref* sender)
{
    hide();
}

void PopupWeaponSpiritDefault::onClickAnotherSpirit(Ref* sender)
{
    auto node = (Node*)sender;
    
    if(node->getName().compare("NEXT") == 0)
    {
        auto tmpSpirit = WeaponSpiritManager::getInstance()->getInfoWeaponSpirit(_infoSpirit->getIdx() + 1);
        
        if(tmpSpirit != nullptr)
        {
            _infoSpirit = tmpSpirit;
            if(!WeaponSpiritManager::getInstance()->isFirstSummon(_infoSpirit->getIdx()-1) && _infoSpirit->isHas())
            {
                WeaponSpiritManager::getInstance()->setFirstSummon(_infoSpirit->getIdx()-1);
            }
        }
    }
    else if(node->getName().compare("PREV") == 0)
    {
        auto tmpSpirit = WeaponSpiritManager::getInstance()->getInfoWeaponSpirit(_infoSpirit->getIdx() - 1);
        
        if(tmpSpirit != nullptr)
        {
            _infoSpirit = tmpSpirit;
            if(!WeaponSpiritManager::getInstance()->isFirstSummon(_infoSpirit->getIdx()-1) && _infoSpirit->isHas())
            {
                WeaponSpiritManager::getInstance()->setFirstSummon(_infoSpirit->getIdx()-1);
            }
        }
    }
    
    uiReload();
}

void PopupWeaponSpiritDefault::onClickEquip(Ref* sender)
{
    if(_infoSpirit->isWear())
        return;
    
    if(!WeaponSpiritManager::getInstance()->isServerDispatch())
    {
        PopupLoading::show();
        
        WeaponSpiritManager::getInstance()->requestSpiritWear(CC_CALLBACK_2(PopupWeaponSpiritDefault::callbackEquip, this), _infoSpirit->getIdx());
    }
}

void PopupWeaponSpiritDefault::onClickTab(Ref* sender)
{
    if(!_infoSpirit->isHas())
        return;
    
    auto node = (Node*)sender;
    _eType = (PopupWeaponSpiritDefault::E_TYPE)node->getTag();
    
    uiReload();
}

void PopupWeaponSpiritDefault::onClickBuff(Ref* sender)
{
    auto popup = PopupSpiritBuffInfo::create(_infoSpirit->getIdx());
    popup->show();
}

#pragma mark -callback
void PopupWeaponSpiritDefault::callbackEquip(bool bResult, int nResult)
{
    PopupLoading::hide();
    if ( bResult == false )
    {
        std::string strMessageKey = "t_ui_error_30";
        
        if( nResult == 0 )
        {

        }
        else if( nResult == 2 )
        {
            
        }
        else if( nResult == 3 )
        {
            
        }
        else if( nResult == 4 )
        {
            
        }
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT(strMessageKey));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    uiReload();
    RefreshManager::getInstance()->refreshUI(E_REFRESH::SPIRIT);
}
