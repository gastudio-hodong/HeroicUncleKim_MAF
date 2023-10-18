//
//  PopupSpiritBuffInfo.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/01/25.
//

#include "PopupSpiritBuffInfo.h"

#include "GameUIPopup/Other/WeaponSpirit/Layer/UILayerSpiritRankUp.h"
#include "GameUIPopup/Other/WeaponSpirit/Layer/UILayerSpiritLevelup.h"
#include "GameUIPopup/Other/WeaponSpirit/Layer/UILayerSpiritSmelting.h"

#include "ManagerGame/WeaponSpiritManager.h"
#include "ManagerGame/NewRaidManager.h"

PopupSpiritBuffInfo* PopupSpiritBuffInfo::create(int nIdx)
{
    PopupSpiritBuffInfo* pRet = new(std::nothrow) PopupSpiritBuffInfo();
    if ( pRet && pRet->init(nIdx) )
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

PopupSpiritBuffInfo::PopupSpiritBuffInfo(void):
_lyTopUI(nullptr)
,_lyBottomUI(nullptr)
{
}

PopupSpiritBuffInfo::~PopupSpiritBuffInfo(void)
{
}

#pragma mark -init
bool PopupSpiritBuffInfo::init(int nIdx)
{
    if ( !PopupBase::init())
    {
        return false;
    }
    _nIdx = nIdx;
    
    initVar();
    initUI();
    
    return true;
}

void PopupSpiritBuffInfo::initVar()
{
    _spriteContainer->setContentSize(Size(552, 800));
}

void PopupSpiritBuffInfo::initUI()
{
    // title
    
    auto lbTitle = Label::createWithTTF(GAME_TEXT("t_ui_spirit_buff_info_title"), GAME_FONT, 28);
    lbTitle->setColor(COLOR_TITLE);
    lbTitle->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5) );
    _spriteContainer->addChild(lbTitle);
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupSpiritBuffInfo::onClickClose, this));
    itemClose->setPosition( Vec2(_spriteContainer->getContentSize().width - itemClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5) );
    itemClose->setTouchRect( itemClose->getContentSize() * 2 );
    menu->addChild(itemClose);
    
    
    _lyTopUI = LayerColor::create();
    _lyTopUI->setContentSize( Size(_spriteContainer->getContentSize().width - 10, 250));
    _lyTopUI->setIgnoreAnchorPointForPosition(false);
    _lyTopUI->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _lyTopUI->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75));
    _spriteContainer->addChild(_lyTopUI);
    
    _lyBottomUI = LayerColor::create();
    _lyBottomUI->setContentSize( Size(_spriteContainer->getContentSize().width - 10, _spriteContainer->getContentSize().height - 85 - _lyTopUI->getContentSize().height));
    _lyBottomUI->setIgnoreAnchorPointForPosition(false);
    _lyBottomUI->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _lyBottomUI->setPosition( _lyTopUI->getPosition() + Vec2::DOWN * _lyTopUI->getContentSize().height);
    _spriteContainer->addChild(_lyBottomUI);
    
    uiTop();
    uiBottom();
}

#pragma mark -ui
void PopupSpiritBuffInfo::uiTop()
{
    _lyTopUI->removeAllChildren();
    
    auto objBuff = WeaponSpiritManager::getInstance()->getInfoWeaponSpiritBuff(_nIdx);
    
    if(objBuff == nullptr)
        return;

    auto fxSpiritBuff = ParticleSystemQuad::create("Assets/Particle/particle_spirit_buff_2.plist");
    fxSpiritBuff->setScale(1.f);
    fxSpiritBuff->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    fxSpiritBuff->setPosition(_lyTopUI->getContentSize().width/2, 80);
    fxSpiritBuff->setStartColor(Color4F(objBuff->getEffectColor()));
    _lyTopUI->addChild(fxSpiritBuff);
    
    auto spriteSpiritBuffFx = Sprite::create(MafUtils::format("Assets/ui/spirit/buf_spirit_%04d.png", objBuff->getIdx()));
    spriteSpiritBuffFx->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    spriteSpiritBuffFx->getTexture()->setTexParameters(texParams);
    spriteSpiritBuffFx->setScale(2.f);
    spriteSpiritBuffFx->setPosition(_lyTopUI->getContentSize().width/2, 100);
    _lyTopUI->addChild(spriteSpiritBuffFx);
}

void PopupSpiritBuffInfo::uiBottom()
{
    _lyBottomUI->removeAllChildren();
    
    auto objBuff = WeaponSpiritManager::getInstance()->getInfoWeaponSpiritBuff(_nIdx);
    
    if(objBuff == nullptr)
        return;
    
    auto lbBuffInfo_1 = Label::createWithTTF(GAME_TEXTFORMAT("t_ui_spirit_buff_info_1", objBuff->getCoolTime(), objBuff->getDurationTime()), GAME_FONT, 24);
    lbBuffInfo_1->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    lbBuffInfo_1->setPosition(_lyBottomUI->getContentSize().width/2, _lyBottomUI->getContentSize().height - 30);
    lbBuffInfo_1->setAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::TOP);
    lbBuffInfo_1->setDimensions(_lyBottomUI->getContentSize().width - 10, lbBuffInfo_1->getContentSize().height);
    lbBuffInfo_1->setColor(COLOR_TITLE);
    _lyBottomUI->addChild(lbBuffInfo_1);
    
    std::string strEffect = GAME_TEXTFORMAT("t_arti_d2_option_1", (float)objBuff->getIncreases() * 100.f);
    
    auto lbBuffInfo_2 = Label::createWithTTF(GAME_TEXTFORMAT("t_ui_spirit_buff_info_2", strEffect.c_str()), GAME_FONT, 24);
    lbBuffInfo_2->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    lbBuffInfo_2->setPosition(lbBuffInfo_1->getPosition() + Vec2::DOWN * (lbBuffInfo_1->getContentSize().height + 10));
    lbBuffInfo_2->setAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::TOP);
    lbBuffInfo_2->setDimensions(_lyBottomUI->getContentSize().width - 10, lbBuffInfo_2->getContentSize().height);
    _lyBottomUI->addChild(lbBuffInfo_2);
    
    auto sprCondirionBox = ui::Scale9Sprite::create("Assets/ui_common/base_box_1_8.png");
    sprCondirionBox->setContentSize(Size(510, 82));
    sprCondirionBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    sprCondirionBox->setPosition(_lyBottomUI->getContentSize().width/2, 150);
    _lyBottomUI->addChild(sprCondirionBox);
    {
        auto objSetting = NewRaidManager::getInstance()->getRaidSetting("spirit_buff_condition_lv");
        int nLvCondition = 99;
        if(objSetting != nullptr)
            nLvCondition = objSetting->getValueToInt();
        
        std::vector<std::pair<std::string, std::string>> list = {
            std::make_pair("t_ui_level_1", std::to_string(nLvCondition)),
            std::make_pair("t_ui_smelting_1", std::to_string(1)),
            std::make_pair("t_ui_costume_equip", "O")
        };
        
        float width = sprCondirionBox->getContentSize().width / list.size();
        float posX = sprCondirionBox->getContentSize().width/2 - width * (list.size()/2) * (list.size()/2);
        for(int i = 0; i < list.size(); ++i)
        {
            auto obj = list[i];
            
            Vec2 pos = Vec2(posX, sprCondirionBox->getContentSize().height - sprCondirionBox->getContentSize().height/4);
            
            auto lbConditionUp = Label::createWithTTF(GAME_TEXT(obj.first), GAME_FONT, 24);
            lbConditionUp->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            lbConditionUp->setPosition(pos);
            lbConditionUp->setAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
            lbConditionUp->setDimensions(sprCondirionBox->getContentSize().width/4, lbConditionUp->getContentSize().height);
            sprCondirionBox->addChild(lbConditionUp);
            
            pos = Vec2(posX, sprCondirionBox->getContentSize().height/4);
            
            auto lbConditionDown = Label::createWithTTF(obj.second, GAME_FONT, 24);
            lbConditionDown->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            lbConditionDown->setPosition(pos);
            lbConditionDown->setAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
            lbConditionDown->setDimensions(sprCondirionBox->getContentSize().width/4, lbConditionDown->getContentSize().height);
            sprCondirionBox->addChild(lbConditionDown);
            
            if(list.size() % 2 == 0)
            {
                lbConditionUp->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
                lbConditionDown->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            }
            
            posX += width + 10;
        }
        
    }
    auto lbCondition = Label::createWithTTF(GAME_TEXT("t_ui_invocation_condition"), GAME_FONT, 24);
    lbCondition->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    lbCondition->setPosition(sprCondirionBox->getPosition() + Vec2::LEFT *  sprCondirionBox->getContentSize().width/2 + Vec2::UP * (sprCondirionBox->getContentSize().height + 5));
    _lyBottomUI->addChild(lbCondition);
    
    auto btnClose = ui::Button::create("Assets/ui_common/btn_ok.png");
    btnClose->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    btnClose->setPosition(Vec2(_lyBottomUI->getContentSize().width/2, 15));
    btnClose->setZoomScale(0.05f);
    btnClose->addClickEventListener(CC_CALLBACK_1(PopupSpiritBuffInfo::onClickClose, this));
    btnClose->setTitleText(GAME_TEXT("t_ui_confirm"));
    btnClose->setTitleAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
    btnClose->getTitleLabel()->setDimensions(btnClose->getContentSize().width - 10, btnClose->getContentSize().height - 10);
    btnClose->getTitleLabel()->enableBold();
    btnClose->setTitleFontSize(30);
    _lyBottomUI->addChild(btnClose);
    
}

#pragma mark - click
void PopupSpiritBuffInfo::onClickClose(Ref* sender)
{
    hide();
}
