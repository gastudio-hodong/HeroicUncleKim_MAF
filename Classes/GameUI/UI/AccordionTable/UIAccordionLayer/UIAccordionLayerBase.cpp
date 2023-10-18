//
//  UIAccordionLayer.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/07/13.
//

#include "UIAccordionLayerBase.h"

#include "GameUI/UI/AccordionTable/UIAccordionItem/UIAccordionItemBase.h"

USING_NS_CC;

UIAccordionLayerBase* UIAccordionLayerBase::create(const Vector<InfoAccordionUI*> listInfoAccodion, bool isActive/* = false*/)
{
    UIAccordionLayerBase* pRet = new(std::nothrow) UIAccordionLayerBase();
    if ( pRet && pRet->init(listInfoAccodion, isActive) )
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

UIAccordionLayerBase::UIAccordionLayerBase(void):
_isActive(false)
,_strBtnText("")
,_strBtnPath("Assets/ui/adventure/adventure_relic_info_listname_box1.png")
,_btnSize(Size::ZERO)
,_btnCapInsets(Rect::ZERO)
,_uiButton(nullptr)
,_lbTitle(nullptr)
,_uiContainer(nullptr)
,_btnAccordion(nullptr)
,_callbackRefresh(nullptr)
{
    _listInfoAccodion.clear();
}

UIAccordionLayerBase::~UIAccordionLayerBase(void)
{
    _listInfoAccodion.clear();
}

bool UIAccordionLayerBase::init(const Vector<InfoAccordionUI*> listInfoAccodion, bool isActive/* = false*/)
{
    if ( !LayerColor::init() )
    {
        return false;
    }
    
    //
    setIgnoreAnchorPointForPosition(false);
    _listInfoAccodion = listInfoAccodion;
    _isActive = isActive;
    
    // init
    initVar();
    initUi();
    
    return true;
}
 
#pragma mark - init
void UIAccordionLayerBase::initVar()
{
    
}

void UIAccordionLayerBase::initUi()
{
    _uiButton = Layer::create();
    _uiButton->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP);
    _uiButton->setIgnoreAnchorPointForPosition(false);
    addChild(_uiButton);
    
    _uiContainer = Layer::create();
    _uiContainer->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP);
    _uiContainer->setIgnoreAnchorPointForPosition(false);
    addChild(_uiContainer);
    
    uiButton();
    uiContainer();
}

#pragma mark - ui
void UIAccordionLayerBase::uiButton()
{
    _uiButton->removeAllChildren();
    
    if(_strBtnText.compare("") == 0)
        _strBtnText = "title";
    if(_btnSize.equals(Size::ZERO))
        _btnSize = Size(710,46);
    if(_btnCapInsets.equals(Rect::ZERO))
        _btnCapInsets = Rect(6,6,35,35);
    
    _btnAccordion = ui::Button::create(_strBtnPath);
    _btnAccordion->setScale9Enabled(true);
    _btnAccordion->setContentSize(_btnSize);
    _btnAccordion->getRendererNormal()->setCapInsets(_btnCapInsets);
    _btnAccordion->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _btnAccordion->addClickEventListener(CC_CALLBACK_1(UIAccordionLayerBase::onClickAccordion, this));
    _uiButton->addChild(_btnAccordion);
    
    auto lbTitle = Label::createWithTTF(_strBtnText, GAME_FONT, 24);
    lbTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    lbTitle->setPosition(15, _btnAccordion->getContentSize().height / 2);
    lbTitle->setDimensions(_btnAccordion->getContentSize().width / 2, _btnAccordion->getContentSize().height - 10);
    lbTitle->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
    lbTitle->setOverflow(Label::Overflow::SHRINK);
    _btnAccordion->addChild(lbTitle);
    
    std::string strArrowPath = "Assets/ui/adventure/adventure_relic_info_list_downarrow.png";
    if(_isActive)
        strArrowPath = "Assets/ui/adventure/adventure_relic_info_list_uparrow.png";
    
    auto sprIcon = Sprite::create(strArrowPath);
    sprIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    sprIcon->setPosition(_btnAccordion->getContentSize().width - 15, _btnAccordion->getContentSize().height/2);
    _btnAccordion->addChild(sprIcon);
    
    _uiButton->setContentSize(_btnAccordion->getContentSize() + Size(0, 10));
    _btnAccordion->setPosition(_uiButton->getContentSize()/2);
    
    updateLayout();
}

void UIAccordionLayerBase::uiContainer()
{
    _uiContainer->removeAllChildren();
    
    if(!_isActive)
    {
        _uiContainer->setContentSize(Size::ZERO);
        updateLayout();
        return;
    }
    
    float subItemHeight = 42;
    float subItemInterval = 5;
    
    Size size = Size(710, subItemHeight * _listInfoAccodion.size() + subItemInterval * _listInfoAccodion.size());
    _uiContainer->setContentSize(size);
    Vec2 pos = Vec2(size.width/2, size.height);
    for(int i = 0; i < _listInfoAccodion.size(); ++i)
    {
        auto obj = _listInfoAccodion.at(i);
        auto uiItem = UIAccordionItemBase::create(Size(710, 42), obj);
        uiItem->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        uiItem->setPosition(pos);
        _uiContainer->addChild(uiItem);
        
        pos -= Vec2(0, subItemHeight + subItemInterval);
    }
    
    updateLayout();
}

void UIAccordionLayerBase::updateLayout()
{
    Size size = _uiButton->getContentSize() + _uiContainer->getContentSize();
    
    setContentSize(size);
    
    _uiButton->setPosition(size.width/2, size.height);
    _uiContainer->setPosition(_uiButton->getPosition() + Vec2::DOWN * _uiButton->getContentSize().height);
}

#pragma mark - utils
void UIAccordionLayerBase::onClickAccordion(Ref* const sender)
{
    _isActive = !_isActive;
    
    uiButton();
    uiContainer();
    
    if(_callbackRefresh != nullptr)
        _callbackRefresh();
}

void UIAccordionLayerBase::setButtonText(const std::string strText)
{
    _strBtnText = strText;
    
    uiButton();
}

void UIAccordionLayerBase::setButtonImage(const std::string strPath, const Size size, const Rect rect)
{
    _strBtnPath = strPath;
    _btnSize = size;
    _btnCapInsets = rect;
    
    uiButton();
}

void UIAccordionLayerBase::setCallbackRefresh(const std::function<void(void)>& callback)
{
    _callbackRefresh = callback;
}

void UIAccordionLayerBase::reloadData()
{
    uiContainer();
}

void UIAccordionLayerBase::setList(const Vector<InfoAccordionUI*> listInfoAccodion)
{
    _listInfoAccodion.clear();
    _listInfoAccodion = listInfoAccodion;
}
