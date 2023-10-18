//
//  LayerEventLuckyBagCurrentLvInfo.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/12/21.
//

#include "LayerEventLuckyBagCurrentLvInfo.h"

#include "GameObject/InfoItems.h"

#include "ManagerEvent/EventLuckyBagManager.h"

USING_NS_CC;

LayerEventLuckyBagCurrentLvInfo* LayerEventLuckyBagCurrentLvInfo::create(cocos2d::Size size)
{
    LayerEventLuckyBagCurrentLvInfo* pRet = new(std::nothrow) LayerEventLuckyBagCurrentLvInfo();
    if ( pRet && pRet->init(size) )
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

LayerEventLuckyBagCurrentLvInfo::LayerEventLuckyBagCurrentLvInfo(void) :
_bInitUI(false),

_bResult(false),

_uiContentsReward(nullptr)
{
}

LayerEventLuckyBagCurrentLvInfo::~LayerEventLuckyBagCurrentLvInfo(void)
{
}

bool LayerEventLuckyBagCurrentLvInfo::init(cocos2d::Size size)
{
    if ( !ui::Layout::init() )
    {
        return false;
    }
    
    //
    setContentSize(size);
    setIgnoreAnchorPointForPosition(false);
    setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    setClippingEnabled(true);
    setTouchEnabled(true);
    
    // init
    initVar();
    initUi();
    
    return true;
}

void LayerEventLuckyBagCurrentLvInfo::onEnter()
{
    ui::Layout::onEnter();
    
    if ( _bInitUI == false )
    {
        _bInitUI = true;
        
        uiContainer();
        uiReward();
    }
    
    //
    drawReward();
}

void LayerEventLuckyBagCurrentLvInfo::onSizeChanged()
{
    ui::Layout::onSizeChanged();
    
    if ( _bInitUI == true )
    {
        drawReward();
    }
}

#pragma mark - init
void LayerEventLuckyBagCurrentLvInfo::initVar()
{
    
}

void LayerEventLuckyBagCurrentLvInfo::initUi()
{
    
}

#pragma mark - ui
void LayerEventLuckyBagCurrentLvInfo::uiContainer()
{
    auto size = getContentSize();
    
    // ui default
    _uiContentsReward = ui::Layout::create();
    _uiContentsReward->setContentSize(size);
    _uiContentsReward->setAnchorPoint(Vec2::ZERO);
    _uiContentsReward->setPosition(Vec2::ZERO);
    addChild(_uiContentsReward);
}

void LayerEventLuckyBagCurrentLvInfo::uiReward()
{
    auto layout = _uiContentsReward;
    auto size = layout->getContentSize();
    
    auto uiRewardBox = ui::ImageView::create("Assets/ui/event/event_new23_box_1.png");
    uiRewardBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    uiRewardBox->setPosition(Vec2(size.width/2, size.height));
    layout->addChild(uiRewardBox);
    
    auto layoutRewardArea = ui::Layout::create();
    layoutRewardArea->setContentSize(uiRewardBox->getContentSize() - Size(0, 47));
    layoutRewardArea->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    layoutRewardArea->setPosition(Vec2(uiRewardBox->getContentSize().width/2, 0));
    layoutRewardArea->setName("UI_REWARD_AREA");
    uiRewardBox->addChild(layoutRewardArea);
    
    auto uiTitle = ui::CText::create("", GAME_FONT, 26);
    uiTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    uiTitle->setPosition(Vec2(uiRewardBox->getContentSize().width/2, uiRewardBox->getContentSize().height));
    uiTitle->setTextAreaSize(Size(size.width - 50, uiTitle->getContentSize().height));
    uiTitle->setTextOverflow(Label::Overflow::SHRINK);
    uiTitle->setTextVerticalAlignment(TextVAlignment::CENTER);
    uiTitle->setTextHorizontalAlignment(TextHAlignment::CENTER);
    uiTitle->setName("UI_TITLE");
    uiRewardBox->addChild(uiTitle);
}

#pragma mark - ui : draw
void LayerEventLuckyBagCurrentLvInfo::drawReward()
{
    auto layout = _uiContentsReward;
    auto list = EventLuckyBagManager::getInstance()->getListEventLuckyBagReward();
    
    if(auto uiTitle = (ui::CText*)utils::findChild(layout, "UI_TITLE"); uiTitle != nullptr)
    {
        if(EventLuckyBagManager::getInstance()->getCurrentLevel() == EventLuckyBagManager::getInstance()->getMaxLevel())
        {
            //
            uiTitle->setString("");
        }
        else
        {
            std::string tmp = "LV." + std::to_string(EventLuckyBagManager::getInstance()->getCurrentLevel() + 1);
            std::string text = GAME_TEXTFORMAT("t_ui_event_enhance_draw_msg_4", tmp.c_str());
            uiTitle->setString(text);
        }
    }
    
    if(auto layoutArea = (ui::Layout*)utils::findChild(layout, "UI_REWARD_AREA"); layoutArea != nullptr)
    {
        layoutArea->removeAllChildren();
        
        int nListHalfSize = list.size()/2;
        if(list.size() != 0 && list.size() % 2 != 0)
            nListHalfSize++;
        
        float width = 62;
        float posX = layoutArea->getContentSize().width/2 - width * (nListHalfSize/2) - 10 * (nListHalfSize/2);
        
        for(int i = 0; i < nListHalfSize; ++i)
        {
            auto obj = list.at(i);
            
            std::string strPath = "Assets/ui/adventure/btn_adventure_menu_1_1.png";
            
            auto objItem = obj->getItem();
            if(objItem == nullptr)
                continue;
            
            auto uiRewardBox = ui::ImageView::create("Assets/ui_common/box_item.png");
            uiRewardBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
            uiRewardBox->setPosition(Vec2(posX, layoutArea->getContentSize().height/2 + 5));
            uiRewardBox->setScale9Enabled(true);
            uiRewardBox->setContentSize(Size(width, width));
            layoutArea->addChild(uiRewardBox);
            
            if (auto uiItemIcon = ui::ImageView::create(objItem->getPath()); uiItemIcon != nullptr )
            {
                uiItemIcon->setPosition(Vec2(uiRewardBox->getContentSize().width / 2, uiRewardBox->getContentSize().height / 2 + 10));
                ((ui::Scale9Sprite*)uiItemIcon->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
                uiRewardBox->addChild(uiItemIcon);
                
                std::string strItemsCount = "";
                
                if((E_ITEMS)objItem->getIdx() != E_ITEMS::COSTUME_SKIN &&
                   (E_ITEMS)objItem->getIdx() != E_ITEMS::WEAPON_SPIRIT)
                {
                    strItemsCount = "x" + objItem->getCount();
                    if (objItem->isString())
                        MafUtils::convertNumberToShort(strItemsCount);
                }
                if((E_ITEMS)objItem->getIdx() == E_ITEMS::WEAPON_SPIRIT)
                {
                    uiItemIcon->setScale(0.7f);
                    uiItemIcon->setPosition(uiRewardBox->getContentSize()/2);
                }

                auto uiCount = ui::CText::create(strItemsCount, GAME_FONT, 20);
                uiCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
                uiCount->setPosition(Vec2(uiRewardBox->getContentSize().width / 2, 5));
                uiCount->setTextAreaSize(Size(uiRewardBox->getContentSize().width - 10, uiCount->getContentSize().height));
                uiCount->setTextOverflow(Label::Overflow::SHRINK);
                uiCount->setTextVerticalAlignment(TextVAlignment::BOTTOM);
                uiCount->setTextHorizontalAlignment(TextHAlignment::CENTER);
                uiRewardBox->addChild(uiCount);
            }
            
            if(nListHalfSize % 2 == 0)
                uiRewardBox->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
            
            posX += width + 10;
        }
        
        posX = layoutArea->getContentSize().width/2 - width * ((list.size() - nListHalfSize) /2) - 10 * ((list.size() - nListHalfSize) /2);
        
        for(int i = nListHalfSize; i < list.size(); ++i)
        {
            auto obj = list.at(i);
            
            std::string strPath = "Assets/ui/adventure/btn_adventure_menu_1_1.png";
            
            auto objItem = obj->getItem();
            if(objItem == nullptr)
                continue;
            
            auto uiRewardBox = ui::ImageView::create("Assets/ui_common/box_item.png");
            uiRewardBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
            uiRewardBox->setPosition(Vec2(posX, layoutArea->getContentSize().height/2 - 5));
            uiRewardBox->setScale9Enabled(true);
            uiRewardBox->setContentSize(Size(width, width));
            layoutArea->addChild(uiRewardBox);
            
            if (auto uiItemIcon = ui::ImageView::create(objItem->getPath()); uiItemIcon != nullptr )
            {
                uiItemIcon->setPosition(Vec2(uiRewardBox->getContentSize().width / 2, uiRewardBox->getContentSize().height / 2 + 10));
                ((ui::Scale9Sprite*)uiItemIcon->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
                uiRewardBox->addChild(uiItemIcon);
                
                std::string strItemsCount = "";
                
                if((E_ITEMS)objItem->getIdx() != E_ITEMS::COSTUME_SKIN &&
                   (E_ITEMS)objItem->getIdx() != E_ITEMS::WEAPON_SPIRIT)
                {
                    strItemsCount = "x" + objItem->getCount();
                    if (objItem->isString())
                        MafUtils::convertNumberToShort(strItemsCount);
                }
                if((E_ITEMS)objItem->getIdx() == E_ITEMS::WEAPON_SPIRIT)
                {
                    uiItemIcon->setScale(0.7f);
                    uiItemIcon->setPosition(uiRewardBox->getContentSize()/2);
                }
                    
                auto uiCount = ui::CText::create(strItemsCount, GAME_FONT, 20);
                uiCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
                uiCount->setPosition(Vec2(uiRewardBox->getContentSize().width / 2, 5));
                uiCount->setTextAreaSize(Size(uiRewardBox->getContentSize().width - 10, uiCount->getContentSize().height));
                uiCount->setTextOverflow(Label::Overflow::SHRINK);
                uiCount->setTextVerticalAlignment(TextVAlignment::BOTTOM);
                uiCount->setTextHorizontalAlignment(TextHAlignment::CENTER);
                uiRewardBox->addChild(uiCount);
            }
            
            if((list.size() - nListHalfSize) % 2 == 0)
                uiRewardBox->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
            
            posX += width + 10;
        }
    }
    
}

#pragma mark - ui : set, get

#pragma mark - ui : click

#pragma mark - callback

#pragma mark - schedule

#pragma mark - game
void LayerEventLuckyBagCurrentLvInfo::onDrawInfo()
{
    drawReward();
}
