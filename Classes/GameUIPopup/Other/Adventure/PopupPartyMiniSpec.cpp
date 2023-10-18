//
//  PopupPartyMiniSpec.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/07/06.
//

#include "PopupPartyMiniSpec.h"

#include "GameUIPopup/Other/Adventure/Layer/UILayerAdventurePartyMiniSpec.h"

USING_NS_CC;

PopupPartyMiniSpec* PopupPartyMiniSpec::create(const int nParty)
{
    PopupPartyMiniSpec* pRet = new(std::nothrow) PopupPartyMiniSpec();
    if ( pRet && pRet->init(nParty) )
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

PopupPartyMiniSpec::PopupPartyMiniSpec(void) :
_nParty(0)
,_layerContainerContents(nullptr)
{
    
}

PopupPartyMiniSpec::~PopupPartyMiniSpec(void)
{
    
}

bool PopupPartyMiniSpec::init(const int nParty)
{
    if ( !PopupBase::init() )
    {
        return false;
    }
    _nParty = nParty;
    // init
    initVar();
    initUi();
    
    return true;
}

#pragma mark - init
void PopupPartyMiniSpec::initVar()
{

}

void PopupPartyMiniSpec::initUi()
{
    Size size = Size(740, 550);
    
    //
    _spriteContainer->setContentSize( size );
    
    _layerContainerContents = Layer::create();
    _layerContainerContents->setContentSize(_spriteContainer->getContentSize() - Size(10, 75));
    _layerContainerContents->setIgnoreAnchorPointForPosition(false);
    _layerContainerContents->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP);
    _layerContainerContents->setPosition(_spriteContainer->getContentSize().width/2, _spriteContainer->getContentSize().height - 75);
    _spriteContainer->addChild(_layerContainerContents);
    
    // title
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_adventure_party_spec_1"), GAME_FONT, 28);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5) );
    _spriteContainer->addChild(labelTitle);
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupPartyMiniSpec::onClickClose, this));
    itemClose->setPosition( Vec2(_spriteContainer->getContentSize().width - itemClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5) );
    itemClose->setTouchRect( itemClose->getContentSize() * 2 );
    menu->addChild(itemClose);
    
    uiContents();
}

#pragma mark - ui
void PopupPartyMiniSpec::uiContents()
{
    //
    _layerContainerContents->removeAllChildren();
    
    auto uiSpec = UILayerAdventurePartyMiniSpec::create(_layerContainerContents->getContentSize());
    uiSpec->setAdventureID(_nParty);
    uiSpec->setIsPopup(true);
    uiSpec->initUi();
    _layerContainerContents->addChild(uiSpec);
}

#pragma mark - click
void PopupPartyMiniSpec::onClickClose(Ref* sender)
{
    hide();
}
