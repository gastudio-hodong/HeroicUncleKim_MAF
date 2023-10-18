//
//  UIAccordionLayoutCostumeSkin.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/07/13.
//

#include "UIAccordionLayoutCostumeSkin.h"

#include "ManagerGame/CostumeSkinManager.h"

USING_NS_CC;

UIAccordionLayoutCostumeSkin* UIAccordionLayoutCostumeSkin::create(const Vector<InfoAccordionUI*> listInfoAccodion, bool isActive/* = false*/)
{
    UIAccordionLayoutCostumeSkin* pRet = new(std::nothrow) UIAccordionLayoutCostumeSkin();
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

UIAccordionLayoutCostumeSkin::UIAccordionLayoutCostumeSkin(void)
{
    
}

UIAccordionLayoutCostumeSkin::~UIAccordionLayoutCostumeSkin(void)
{
    
}

#pragma mark - ui
void UIAccordionLayoutCostumeSkin::uiContainer()
{
    _uiContainer->removeAllChildren();
    
    if(!_isActive)
    {
        _uiContainer->setContentSize(Size::ZERO);
        updateLayout();
        return;
    }
    
    int skinEquip = CostumeSkinManager::getInstance()->getCostumeSkinEquip();
    int skinEquipDummy = CostumeSkinManager::getInstance()->getCostumeSkinEquipDummy();
    
    int64_t count = _listInfoAccodion.size() / 3;
    if ( _listInfoAccodion.size() % 3 != 0 )
    {
        count++;
    }
    
    //
    Size sizeContainer;
    sizeContainer.width = 700;
    sizeContainer.height = (249 + 10) * count;
    
    Size sizeLayout;
    sizeLayout.width = 201 * 3 + 15 * 2;
    sizeLayout.height = 249;
    
    //
    _uiContainer->setContentSize(sizeContainer);
    
    //
    double posLayoutX = sizeContainer.width / 2;
    double posLayoutY = sizeContainer.height;
    for ( int idx = 0; idx < count; idx++ )
    {
        ssize_t nIdxStart = idx * 3;
        ssize_t nIdxEnd = nIdxStart + 3;
        if ( nIdxEnd > _listInfoAccodion.size() )
        {
            nIdxEnd = _listInfoAccodion.size();
        }
        
        //
        auto uiLayout = ui::Layout::create();
        uiLayout->setContentSize(Size(sizeLayout.width, sizeLayout.height));
        uiLayout->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        uiLayout->setPosition(Vec2(posLayoutX, posLayoutY));
        _uiContainer->addChild(uiLayout);
        
        //
        for ( int i = 0; nIdxStart + i < nIdxEnd; i++ )
        {
            int nIdxReal = (int)nIdxStart + i;
            
            auto objAccodion = _listInfoAccodion.at(nIdxReal);
            if ( objAccodion == nullptr )
            {
                continue;
            }
            
            auto objSkin = CostumeSkinManager::getInstance()->getInfoSkin(objAccodion->getIdx());
            if ( objSkin == nullptr )
            {
                continue;
            }
            
            bool bBuy = CostumeSkinManager::getInstance()->isCostumeSkinBuy(objSkin->getIdx());
            
            double posX = 201 * i + 15 * i;
            double posY = uiLayout->getContentSize().height;
            
            auto uiBtnBG = ui::Button::create("Assets/ui/costume/skin_box1_1.png");
            uiBtnBG->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
            uiBtnBG->setPosition(Vec2(posX, posY));
            uiBtnBG->addClickEventListener(CC_CALLBACK_1(UIAccordionLayoutCostumeSkin::onClickSkin, this));
            uiBtnBG->setSwallowTouches(false);
            uiBtnBG->setTag(nIdxReal);
            uiLayout->addChild(uiBtnBG);
            if ( bBuy == false )
            {
                uiBtnBG->loadTextureNormal("Assets/ui/costume/skin_box1_3.png");
            }
            
            // icon
            auto uiIcon = ui::ImageView::create(MafUtils::format("Assets/character_hero/skin/skin_%02d_walk_1.png", objSkin->getIdx()));
            uiIcon->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
            uiIcon->setPosition(Vec2(100.5 + 165 + 15, 33));
            uiBtnBG->addChild(uiIcon);
            if ( bBuy == false )
            {
                ((ui::Scale9Sprite*)uiIcon->getVirtualRenderer())->setState(ui::Scale9Sprite::State::GRAY);
            }
            
            // name
            auto uiName = ui::CText::create(objSkin->getName(), GAME_FONT, 20);
            uiName->setPosition(Vec2(uiBtnBG->getContentSize().width / 2, 24));
            uiName->setTextColor(Color4B(225, 230, 235, 255));
            uiName->setTextOverflow(Label::Overflow::SHRINK);
            uiName->setTextAreaSize(Size(uiBtnBG->getContentSize().width - 10, uiName->getContentSize().height));
            uiName->setTextVerticalAlignment(TextVAlignment::CENTER);
            uiName->setTextHorizontalAlignment(TextHAlignment::CENTER);
            uiBtnBG->addChild(uiName);
            if ( bBuy == false )
            {
                uiName->setTextColor(Color4B::GRAY);
            }
            
            // equip label
            if ( objSkin->getIdx() == skinEquip )
            {
                auto uiLabel = ui::ImageView::create("Assets/ui/costume/costume_wear_mark.png");
                uiLabel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
                uiLabel->setPosition(Vec2(0, uiBtnBG->getContentSize().height));
                uiBtnBG->addChild(uiLabel);
            }
            
            // effect select
            if ( objSkin->getIdx() == skinEquipDummy )
            {
                auto animation = Animation::create();
                animation->setDelayPerUnit(0.2);
                for ( int i = 0; i <= 2; i++ )
                {
                    auto spriteAni = Sprite::create(MafUtils::format("Assets/ui/costume/skin_box_select%02d.png",i).c_str());
                    spriteAni->getTexture()->setTexParameters(texParams);
                    animation->addSpriteFrame(spriteAni->getSpriteFrame());
                }
                
                auto uiAni = ui::ImageView::create("Assets/ui/costume/skin_box_select00.png");
                uiAni->setPosition(Vec2(uiBtnBG->getContentSize().width / 2, uiBtnBG->getContentSize().height / 2));
                uiAni->setScale(3);
                uiAni->getVirtualRenderer()->stopAllActions();
                uiAni->getVirtualRenderer()->runAction(RepeatForever::create(Animate::create(animation)));
                uiBtnBG->addChild(uiAni);
            }
            
        }
        
        //
        posLayoutY -= (uiLayout->getContentSize().height + 10);
    }

    //
    updateLayout();
}

void UIAccordionLayoutCostumeSkin::onClickSkin(cocos2d::Ref* sender)
{
    auto uiButton = (ui::Button*)sender;
    
    if ( uiButton->getTouchMovePosition().equals(Vec2::ZERO) == false )
    {
        if ( uiButton->getTouchBeganPosition().distance(uiButton->getTouchMovePosition()) > 20 )
        {
            return;
        }
    }
    
    auto idx = uiButton->getTag();
    auto objAccodion = _listInfoAccodion.at(idx);
    auto objSkin = CostumeSkinManager::getInstance()->getInfoSkin(objAccodion->getIdx());
    if ( objSkin == nullptr )
    {
        return;
    }
    
    //
    CostumeSkinManager::getInstance()->setCostumeSkinEquipDummy(objSkin->getIdx());
    
    //
    RefreshManager::getInstance()->refreshUI(E_REFRESH::COSTUME_DUMMY);
}
