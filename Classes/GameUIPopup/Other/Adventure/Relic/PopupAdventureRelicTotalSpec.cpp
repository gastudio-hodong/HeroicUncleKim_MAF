//
//  PopupAdventureRelicTotalSpec.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/07/12.
//
#include "PopupAdventureRelicTotalSpec.h"

#include "GameObject/InfoSetting.h"
#include "GameObject/InfoAccordionUI.h"

#include "GameUIPopup/Other/Adventure/Relic/UIAccordion/UIAccordionTableRelicSpec.h"

#include "ManagerGame/AdventureRelicManager.h"
#include "ManagerGame/AdventureManager.h"

USING_NS_CC;

PopupAdventureRelicTotalSpec* PopupAdventureRelicTotalSpec::create()
{
    PopupAdventureRelicTotalSpec* pRet = new(std::nothrow) PopupAdventureRelicTotalSpec();
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

PopupAdventureRelicTotalSpec::PopupAdventureRelicTotalSpec(void):
_lyTopUI(nullptr),
_lyTableUI(nullptr),
_node(nullptr),
_table(nullptr)
{
    _listType.clear();
}

PopupAdventureRelicTotalSpec::~PopupAdventureRelicTotalSpec(void)
{
    for(auto obj : _listType)
        obj.second.clear();
    
    _listType.clear();
}

#pragma mark -init
bool PopupAdventureRelicTotalSpec::init()
{
    if ( !PopupBase::init())
    {
        return false;
    }
    
    loadData();
    initVar();
    initUI();
    
    if(_node != nullptr)
    {
        auto n = _node->getChildByTag(1);
        if(n != nullptr)
        {
            n->setContentSize(Size(n->getContentSize().width, 800));
            _node->setContentSize(Size(_node->getContentSize().width, 810));
            _table->_updateContentSize();
        }
    }
    
    return true;
}

void PopupAdventureRelicTotalSpec::loadData()
{
    std::string strText = "";
    auto setting = AdventureManager::getInstance()->getSetting("relic_effect_dict_order");
    
    if(setting != nullptr)
        strText = setting->getValueToString();
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)strText.c_str());
    if( jsonParser.HasParseError() == true )
    {
        return;
    }
    
    _listType.clear();
    for ( int i = 0; i < jsonParser.Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser[i];
        
        std::string strCategori = jsonValue["categori"].GetString();
        auto arrayTypes = jsonValue["ability_types"].GetArray();
        Vector<InfoAccordionUI*> list;
        
        for(int j = 0; j < arrayTypes.Size(); ++j)
        {
            const rapidjson::Value& jsonTypeValue = arrayTypes[j];
            E_ADVENTURE_PROPERTY_TYPE nType = (E_ADVENTURE_PROPERTY_TYPE)jsonTypeValue["type"].GetInt();
            double dEffect = AdventureRelicManager::getInstance()->getRelicTotalEffect(nType);
            std::string strEffect = AdventureManager::getPropertyEffectText(nType, dEffect);
            bool isActive = AdventureRelicManager::getInstance()->isActivateRelicEffect(nType);
            
            auto info = InfoAccordionUI::create();
            info->setIdx(j+1);
            info->setTitleText(MafUtils::format("t_ui_advt_property_type_%d", (int)nType));
            info->addText("EFFECT", strEffect);
            info->addValueDouble("EFFECT", dEffect);
            info->setIsActive(isActive);
            list.pushBack(info);
        }
        
        _listType.push_back(std::make_pair(strCategori, list));
    }
    
}

void PopupAdventureRelicTotalSpec::initVar()
{
    _spriteContainer->setContentSize(Size(750, 1080));
}

void PopupAdventureRelicTotalSpec::initUI()
{
    // title
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_advt_relic_spec_title_1"), GAME_FONT, 28);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5) );
    _spriteContainer->addChild(labelTitle);
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupAdventureRelicTotalSpec::onClickClose, this));
    itemClose->setPosition( Vec2(_spriteContainer->getContentSize().width - itemClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5) );
    itemClose->setTouchRect( itemClose->getContentSize() * 2 );
    menu->addChild(itemClose);
    
    //650
    Size size = _spriteContainer->getContentSize();
    
    _lyTopUI = LayerColor::create();
    _lyTopUI->setContentSize( Size(size.width - 10, 110));
    _lyTopUI->setIgnoreAnchorPointForPosition(false);
    _lyTopUI->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _lyTopUI->setPosition(_spriteContainer->getContentSize().width/2, _spriteContainer->getContentSize().height - 75);
    _spriteContainer->addChild(_lyTopUI);
    
    _lyTableUI = LayerColor::create();
    _lyTableUI->setContentSize( Size(size.width - 10, _spriteContainer->getContentSize().height - 75 - _lyTopUI->getContentSize().height - 20));
    _lyTableUI->setIgnoreAnchorPointForPosition(false);
    _lyTableUI->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _lyTableUI->setPosition(_lyTopUI->getPosition() + Vec2::DOWN * _lyTopUI->getContentSize().height);
    _spriteContainer->addChild(_lyTableUI);
    
    uiTop();
    uiTable();
}

#pragma mark -ui
void PopupAdventureRelicTotalSpec::uiTop()
{
    _lyTopUI->removeAllChildren();
    
    auto sprBox = ui::Scale9Sprite::create("Assets/ui/adventure/adventure_relic_info_box.png", Rect::ZERO, Rect(5, 5, 6, 6));
    sprBox->setContentSize(Size(710, 100));
    sprBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    sprBox->setPosition(_lyTopUI->getContentSize()/2);
    _lyTopUI->addChild(sprBox);
    
    auto lbInfo = Label::createWithTTF(GAME_TEXT("t_ui_advt_relic_spec_info_1"), GAME_FONT, 26);
    lbInfo->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    lbInfo->setPosition(sprBox->getContentSize()/2);
    lbInfo->setDimensions(sprBox->getContentSize().width - 10,  sprBox->getContentSize().height - 10);
    lbInfo->setAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
    lbInfo->setOverflow(cocos2d::Label::Overflow::SHRINK);
    sprBox->addChild(lbInfo);
}

void PopupAdventureRelicTotalSpec::uiTable()
{
    _lyTableUI->removeAllChildren();
    
    auto layout = UIAccordionTableRelicSpec::create(_lyTableUI->getContentSize(), _listType);
    layout->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    layout->setPosition(_lyTableUI->getContentSize().width/2, _lyTableUI->getContentSize().height);
    _lyTableUI->addChild(layout);
}

#pragma mark -utils
void PopupAdventureRelicTotalSpec::onClickClose(Ref* sender)
{
    hide();
    
}
