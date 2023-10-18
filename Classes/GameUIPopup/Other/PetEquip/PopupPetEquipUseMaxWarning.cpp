//
//  PopupPetEquipUseMaxWarning.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/05/24.
//
#include "PopupPetEquipUseMaxWarning.h"

USING_NS_CC;

PopupPetEquipUseMaxWarning* PopupPetEquipUseMaxWarning::create(int nUse, int nType)
{
    PopupPetEquipUseMaxWarning* pRet = new(std::nothrow) PopupPetEquipUseMaxWarning();
    if ( pRet && pRet->init(nUse, nType) )
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

PopupPetEquipUseMaxWarning::PopupPetEquipUseMaxWarning():
_callback(nullptr),

_nUse(0),
_nType(0),
_bCheck(false)
{
    
}

PopupPetEquipUseMaxWarning::~PopupPetEquipUseMaxWarning(void)
{
}

bool PopupPetEquipUseMaxWarning::init(int nUse, int nType)
{
    if ( !PopupBase::init() )
    {
        return false;
    }
    _nUse = nUse;
    _nType = nType;
    
    initVar();
    initUI();
    
    return true;
}

void PopupPetEquipUseMaxWarning::onEnter()
{
    PopupBase::onEnter();
}

void PopupPetEquipUseMaxWarning::initVar()
{
    float nWidth = 550;
    float nHeight = getContentSize().height;
    if ( nHeight > 400 )
        nHeight = 400;
    
    _spriteContainer->setContentSize(Size(nWidth, nHeight));
    
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu, E_ZORER::UI+1);
    
    auto lbTitle = Label::createWithTTF(GAME_TEXT("t_pet_equip_box_name_3"), GAME_FONT, 30);
    lbTitle->setColor(COLOR_TITLE);
    lbTitle->setPosition(Vec2(_spriteContainer->getContentSize().width/2, _spriteContainer->getContentSize().height - 40));
    _spriteContainer->addChild(lbTitle);
}

void PopupPetEquipUseMaxWarning::initUI()
{
    auto lbContent = Label::createWithTTF(GAME_TEXT("t_pet_equip_warn_limit_1"), GAME_FONT, 24);
    lbContent->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    lbContent->setDimensions(_spriteContainer->getContentSize().width - 10, 90);
    lbContent->setAlignment(TextHAlignment::CENTER, TextVAlignment::TOP);
    lbContent->setOverflow(Label::Overflow::SHRINK);
    lbContent->setPosition(Vec2(_spriteContainer->getContentSize().width/2, _spriteContainer->getContentSize().height - 90));
    _spriteContainer->addChild(lbContent);
    
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu, E_ZORER::UI+1);
    
    auto sprClose = ui::Scale9Sprite::create("Assets/ui_common/btn_1_1_on.png");
    sprClose->setContentSize(Size(160, 80));
    
    auto itemClose = MafNode::MafMenuItemSprite::create(sprClose, nullptr, CC_CALLBACK_1(PopupPetEquipUseMaxWarning::onClick, this));
    itemClose->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    itemClose->setPosition(Vec2(80, 30));
    itemClose->setName("CLOSE");
    menu->addChild(itemClose);
    {
        auto lbBoxUse = Label::createWithTTF(GAME_TEXT("t_ui_system_update_confirm"), GAME_FONT, 24);
        lbBoxUse->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        lbBoxUse->setPosition(itemClose->getContentSize()/2);
        lbBoxUse->setDimensions(itemClose->getContentSize().width - 10, itemClose->getContentSize().height - 5);
        lbBoxUse->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        lbBoxUse->setOverflow(Label::Overflow::SHRINK);
        itemClose->addChild(lbBoxUse);
    }
    
    auto sprUse = ui::Scale9Sprite::create("Assets/ui_common/btn_1_1_on.png");
    sprUse->setContentSize(Size(160, 80));
    
    auto itemBoxUse = MafNode::MafMenuItemSprite::create(sprUse, nullptr, CC_CALLBACK_1(PopupPetEquipUseMaxWarning::onClick, this));
    itemBoxUse->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    itemBoxUse->setPosition(Vec2(_spriteContainer->getContentSize().width - 80, 30));
    itemBoxUse->setName("USE");
    menu->addChild(itemBoxUse);
    {
        auto lbBoxUse = Label::createWithTTF(GAME_TEXT("t_ui_checkdata_btn_1"), GAME_FONT, 24);
        lbBoxUse->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        lbBoxUse->setPosition(itemBoxUse->getContentSize()/2);
        lbBoxUse->setDimensions(itemBoxUse->getContentSize().width - 10, itemBoxUse->getContentSize().height - 5);
        lbBoxUse->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        lbBoxUse->setOverflow(Label::Overflow::SHRINK);
        itemBoxUse->addChild(lbBoxUse);
    }
    
    auto itemCheckBox = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui/costume/menu_btn_list_5.png"), nullptr, CC_CALLBACK_1(PopupPetEquipUseMaxWarning::onClick, this));
    itemCheckBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    itemCheckBox->setPosition(Vec2(_spriteContainer->getContentSize().width / 2 - 80, _spriteContainer->getContentSize().height/2 - 30));
    itemCheckBox->setName("CHECK_BOX");
    itemCheckBox->setScale(2);
    itemCheckBox->setTouchRect(itemCheckBox->getContentSize() * itemCheckBox->getScale());
    menu->addChild(itemCheckBox);
    {
        auto lbBoxUse = Label::createWithTTF(GAME_TEXT("t_ui_remind_week"), GAME_FONT, 24);
        lbBoxUse->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        lbBoxUse->setPosition(itemCheckBox->getPosition() + Vec2::RIGHT * (itemCheckBox->getContentSize().width * itemCheckBox->getScale()));
        lbBoxUse->setDimensions(150 , lbBoxUse->getContentSize().height - 5);
        lbBoxUse->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
        lbBoxUse->setOverflow(Label::Overflow::SHRINK);
        _spriteContainer->addChild(lbBoxUse);
    }
    
}

void PopupPetEquipUseMaxWarning::onClick(cocos2d::Ref* sender)
{
    auto item = (Node*)sender;
    
    if(item->getName().compare("CLOSE") == 0)
    {
        onHide();
    }
    else if(item->getName().compare("USE") == 0)
    {
        if(_callback != nullptr)
            _callback(_nUse, _nType);
        
        onHide();
    }
    else if(item->getName().compare("CHECK_BOX") == 0)
    {
        _bCheck = !_bCheck;
        
        MafNode::MafMenuItemSprite* itemCheck = (MafNode::MafMenuItemSprite*)item;
        
        if(_bCheck == true)
            itemCheck->setNormalImage(Sprite::create("Assets/ui/costume/menu_btn_list_4.png"));
        else
            itemCheck->setNormalImage(Sprite::create("Assets/ui/costume/menu_btn_list_5.png"));
            
    }
}

void PopupPetEquipUseMaxWarning::onHide()
{
    
    if(_bCheck == true)
    {
        //7일
        auto saveTime = UtilsDate::getInstance()->getTime() + 604800;
        
        std::string strSec = MafUtils::format("%ld", saveTime);
        
        UserDefault::getInstance()->setStringForKey("PET_EQUIP_MAX_WARNING", strSec);
    }
    
    hide();
}
void PopupPetEquipUseMaxWarning::setCallback(std::function<void(int, int)> pCallback)
{
    _callback = pCallback;
}
