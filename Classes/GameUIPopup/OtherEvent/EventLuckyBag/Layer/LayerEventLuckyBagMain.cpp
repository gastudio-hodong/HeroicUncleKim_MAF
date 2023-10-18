//
//  LayerEventLuckyBagMyReward.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/12/21.
//

#include "LayerEventLuckyBagMain.h"

#include "Common/ConfigGameNormal.h"

#include "GameObject/InfoItems.h"

#include "GameUIPopup/Other/PurchaseCash/PopupCash.h"
#include "GameUIPopup/OtherEvent/EventVote/PopupEventVoting.h"
#include "GameUIPopup/OtherEvent/EventLuckyBag/PopupLuckyBagDrawEffect.h"

#include "ManagerEvent/EventLuckyBagManager.h"
#include "ManagerGame/ItemsManager.h"


USING_NS_CC;

LayerEventLuckyBagMain* LayerEventLuckyBagMain::create(cocos2d::Size size)
{
    LayerEventLuckyBagMain* pRet = new(std::nothrow) LayerEventLuckyBagMain();
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

LayerEventLuckyBagMain::LayerEventLuckyBagMain(void) :
_bInitUI(false)
,_bResult(false)
,_nNowDrawRewardLv(0)
,_uiContentsMyReward(nullptr)
,_uiContentsNotics(nullptr)
,_uiContentsButton(nullptr)
{
}

LayerEventLuckyBagMain::~LayerEventLuckyBagMain(void)
{
    EventLuckyBagManager::getInstance()->unSubDelegateProcess(this);
}

bool LayerEventLuckyBagMain::init(cocos2d::Size size)
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
    
    EventLuckyBagManager::getInstance()->subDelegateProcess(new MafFunction<void(bool, int)>(CC_CALLBACK_2(LayerEventLuckyBagMain::callbackProgress, this), this));
    
    return true;
}

void LayerEventLuckyBagMain::onEnter()
{
    ui::Layout::onEnter();
    
    if ( _bInitUI == false )
    {
        _bInitUI = true;
        
        uiContainer();
        uiNotics();
        uiMyReward();
        uiButtton();
    }
    
    //
    drawMyReward();
    drawButtton();
}

void LayerEventLuckyBagMain::onSizeChanged()
{
    ui::Layout::onSizeChanged();
    
    if ( _bInitUI == true )
    {
        drawMyReward();
        drawButtton();
    }
}

#pragma mark - init
void LayerEventLuckyBagMain::initVar()
{
    
}

void LayerEventLuckyBagMain::initUi()
{
    
}

#pragma mark - ui
void LayerEventLuckyBagMain::uiContainer()
{
    auto size = getContentSize();
    
    // ui default
    _uiContentsNotics = ui::Layout::create();
    _uiContentsNotics->setContentSize(Size(size.width, 100));
    _uiContentsNotics->setIgnoreAnchorPointForPosition(false);
    _uiContentsNotics->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _uiContentsNotics->setPosition(Vec2(size.width / 2, size.height));
    _uiContentsNotics->setClippingEnabled(false);
    addChild(_uiContentsNotics);
    
    _uiContentsMyReward = ui::Layout::create();
    _uiContentsMyReward->setContentSize(Size(size.width, size.height - _uiContentsNotics->getContentSize().height - 150));
    _uiContentsMyReward->setIgnoreAnchorPointForPosition(false);
    _uiContentsMyReward->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _uiContentsMyReward->setPosition(_uiContentsNotics->getPosition() + Vec2::DOWN * _uiContentsNotics->getContentSize().height);
    _uiContentsMyReward->setClippingEnabled(false);
    addChild(_uiContentsMyReward);
    
    _uiContentsButton = ui::Layout::create();
    _uiContentsButton->setContentSize(Size(size.width, 150));
    _uiContentsButton->setIgnoreAnchorPointForPosition(false);
    _uiContentsButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _uiContentsButton->setPosition(_uiContentsMyReward->getPosition() + Vec2::DOWN * _uiContentsMyReward->getContentSize().height);
    _uiContentsButton->setClippingEnabled(false);
    addChild(_uiContentsButton);
}

void LayerEventLuckyBagMain::uiNotics()
{
    auto layout = _uiContentsNotics;
    auto size = layout->getContentSize();
    
    auto uiTitle = ui::CText::create(GAME_TEXT("t_ui_event_enhance_draw_msg_1"), GAME_FONT, 30);
    uiTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    uiTitle->setPosition(size/2);
    uiTitle->setTextAreaSize(Size(size.width - 50, uiTitle->getContentSize().height));
    uiTitle->setTextOverflow(Label::Overflow::SHRINK);
    uiTitle->setTextVerticalAlignment(TextVAlignment::CENTER);
    uiTitle->setTextHorizontalAlignment(TextHAlignment::CENTER);
    uiTitle->setColor(Color3B(128,128,128));
    layout->addChild(uiTitle);
}
void LayerEventLuckyBagMain::uiMyReward()
{
    auto layout = _uiContentsMyReward;
    auto size = layout->getContentSize();
    
    auto uiBox = ui::ImageView::create("Assets/ui_common/box_04.png");
    uiBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    uiBox->setPosition(size/2);
    uiBox->setScale9Enabled(true);
    uiBox->setContentSize(Size(674, 354));
    layout->addChild(uiBox);
    
    //타이틀
    auto uiTitleBox = ui::ImageView::create("Assets/ui/costume/costume_name_box.png");
    uiTitleBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    uiTitleBox->setPosition(Vec2(uiBox->getContentSize().width/2, uiBox->getContentSize().height));
    uiBox->addChild(uiTitleBox);
    {
        auto uiTitle = ui::CText::create(GAME_TEXT("t_ui_event_enhance_draw_msg_2"), GAME_FONT, 24);
        uiTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        uiTitle->setPosition(Vec2(uiTitleBox->getContentSize()/2));
        uiTitle->setTextAreaSize(Size(uiTitleBox->getContentSize().width - 40, uiTitle->getContentSize().height));
        uiTitle->setTextOverflow(Label::Overflow::SHRINK);
        uiTitle->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiTitle->setTextHorizontalAlignment(TextHAlignment::CENTER);
        uiTitleBox->addChild(uiTitle);
    }
    
    //리워드
    auto layoutRewardArea = ui::Layout::create();
    layoutRewardArea->setContentSize(uiBox->getContentSize() - Size(0, 50));
    layoutRewardArea->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    layoutRewardArea->setPosition(Vec2(uiBox->getContentSize().width/2, 0));
    layoutRewardArea->setName("UI_REWARD_AREA");
    uiBox->addChild(layoutRewardArea);
    
}

void LayerEventLuckyBagMain::uiButtton()
{
    auto layout = _uiContentsButton;
    auto size = layout->getContentSize();
    
    auto uiButtons = ui::Layout::create();
    uiButtons->setIgnoreAnchorPointForPosition(false);
    uiButtons->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    uiButtons->setPosition(Vec2(size.width / 2, size.height / 2));
    uiButtons->setLayoutType(ui::Layout::Type::HORIZONTAL);
    uiButtons->setName("UI_BUTTONS");
    layout->addChild(uiButtons);
    {
        //Stop
        auto uiBtnStop = ui::Button::create("Assets/ui_common/btn_1_1_on.png");
        uiBtnStop->setScale9Enabled(true);
        uiBtnStop->setContentSize(Size(210, 100));
        uiBtnStop->setName("UI_BUTTON_STOP");
        uiBtnStop->addClickEventListener(CC_CALLBACK_1(LayerEventLuckyBagMain::onClickStop, this));
        uiButtons->addChild(uiBtnStop);
        {
            auto uiText = ui::CText::create(GAME_TEXT("t_ui_event_enhance_draw_bt_stop"), GAME_FONT, 24);
            uiText->setPosition(uiBtnStop->getContentSize()/2);
            uiText->setTextAreaSize(Size(uiBtnStop->getContentSize().width - 10, uiText->getContentSize().height));
            uiText->setTextOverflow(Label::Overflow::SHRINK);
            uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
            uiText->setTextHorizontalAlignment(TextHAlignment::CENTER);
            uiText->setName("TEXT");
            uiBtnStop->addChild(uiText);
        }
        
        //Go
        auto uiBtnGo = ui::Button::create("Assets/ui_common/btn_1_1_on.png");
        uiBtnGo->setScale9Enabled(true);
        uiBtnGo->setContentSize(Size(210, 100));
        uiBtnGo->addClickEventListener(CC_CALLBACK_1(LayerEventLuckyBagMain::onClickGo, this));
        uiBtnGo->setName("UI_BUTTON_GO");
        uiButtons->addChild(uiBtnGo);
        {
            auto params = ui::LinearLayoutParameter::create();
            params->setMargin(ui::Margin(10, 0, 0, 0));
            uiBtnGo->setLayoutParameter(params);
        }
        
        //
        auto uiBtnCount = ui::Button::create("Assets/ui_common/btn_1_1_on.png");
        uiBtnCount->setScale9Enabled(true);
        uiBtnCount->setContentSize(Size(210, 100));
        uiBtnCount->addClickEventListener(CC_CALLBACK_1(LayerEventLuckyBagMain::onClickCount, this));
        uiBtnCount->setName("UI_BUTTON_COUNT");
        uiButtons->addChild(uiBtnCount);
        {
            auto params = ui::LinearLayoutParameter::create();
            params->setMargin(ui::Margin(10, 0, 0, 0));
            uiBtnCount->setLayoutParameter(params);

            auto uiText = ui::CText::create(GAME_TEXTFORMAT("t_ui_event_enhance_draw_bt_draw_up", EventLuckyBagManager::getInstance()->getCountMax()), GAME_FONT, 24);
            uiText->setPosition(uiBtnCount->getContentSize()/2);
            uiText->setTextAreaSize(Size(uiBtnCount->getContentSize().width - 10, uiText->getContentSize().height));
            uiText->setTextOverflow(Label::Overflow::SHRINK);
            uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
            uiText->setTextHorizontalAlignment(TextHAlignment::CENTER);
            uiBtnCount->addChild(uiText);
        }
    }
}

#pragma mark - ui : draw
void LayerEventLuckyBagMain::drawMyReward()
{
    auto layout = _uiContentsMyReward;
    if(auto layoutArea = (ui::Layout*)utils::findChild(layout, "UI_REWARD_AREA"); layoutArea != nullptr)
    {
        layoutArea->removeAllChildren();
        drawReward(layoutArea, 0, 5, Vec2::ANCHOR_MIDDLE_BOTTOM);
        
        drawReward(layoutArea, 5, 10, Vec2::ANCHOR_MIDDLE_TOP);
    }
}
void LayerEventLuckyBagMain::drawButtton()
{
    auto layout = _uiContentsButton;
    
    auto uiButtons = utils::findChild<ui::Layout*>(layout, "UI_BUTTONS");
    
    //고 버튼
    if(auto uiBtnGo = (ui::Button*)utils::findChild(uiButtons, "UI_BUTTON_GO"); uiBtnGo != nullptr)
    {
        uiBtnGo->removeAllChildren();
        uiBtnGo->setEnabled(true);
        
        //첫 뽑기
        if(EventLuckyBagManager::getInstance()->getCurrentLevel() == 0)
        {
            auto objItem = ItemsManager::getInstance()->getInfoItems(E_ITEMS::EVENT_LUCKEY_BACK);
            
            auto uiVoteCoin = ui::ImageView::create(objItem->getPath());
            uiVoteCoin->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            uiVoteCoin->setPosition(Vec2(uiBtnGo->getContentSize().width/2 - uiVoteCoin->getContentSize().width, uiBtnGo->getContentSize().height/2 + 25));
            uiBtnGo->addChild(uiVoteCoin);
            
            auto uiCount = ui::CText::create(MafUtils::format("%d", EventLuckyBagManager::getInstance()->getCountNow()), GAME_FONT, 24);
            uiCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            uiCount->setPosition(uiVoteCoin->getPosition() + Vec2::RIGHT * uiVoteCoin->getContentSize().width);
            uiCount->setTextOverflow(Label::Overflow::SHRINK);
            uiCount->setTextVerticalAlignment(TextVAlignment::CENTER);
            uiCount->setTextHorizontalAlignment(TextHAlignment::LEFT);
            uiBtnGo->addChild(uiCount);
            
            auto uiText = ui::CText::create(GAME_TEXT("t_ui_event_enhance_draw_bt_draw_1"), GAME_FONT, 24);
            uiText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
            uiText->setPosition(Vec2(uiBtnGo->getContentSize().width/2, uiBtnGo->getContentSize().height/2));
            uiText->setTextAreaSize(Size(uiBtnGo->getContentSize().width - 10, uiText->getContentSize().height));
            uiText->setTextOverflow(Label::Overflow::SHRINK);
            uiText->setTextVerticalAlignment(TextVAlignment::TOP);
            uiText->setTextHorizontalAlignment(TextHAlignment::CENTER);
            uiBtnGo->addChild(uiText);
        }
        //마지막 레벨 달성시
        else if(EventLuckyBagManager::getInstance()->getCurrentLevel() == EventLuckyBagManager::getInstance()->getMaxLevel())
        {
            uiBtnGo->setEnabled(false);
            auto uiText = ui::CText::create(GAME_TEXT("t_ui_event_enhance_draw_bt_draw_3"), GAME_FONT, 24);
            uiText->setPosition(Vec2(uiBtnGo->getContentSize().width/2, uiBtnGo->getContentSize().height/2));
            uiText->setTextAreaSize(Size(uiBtnGo->getContentSize().width - 10, uiText->getContentSize().height));
            uiText->setTextOverflow(Label::Overflow::SHRINK);
            uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
            uiText->setTextHorizontalAlignment(TextHAlignment::CENTER);
            uiBtnGo->addChild(uiText);
        }
        //일반적인 레벨일 떄
        else
        {
            auto uiCount = ui::CText::create(GAME_TEXT("t_ui_event_enhance_draw_bt_draw_2"), GAME_FONT, 24);
            uiCount->setPosition(Vec2(uiBtnGo->getContentSize().width / 2, uiBtnGo->getContentSize().height / 2));
            uiCount->setTextAreaSize(Size(uiBtnGo->getContentSize().width - 10, uiCount->getContentSize().height));
            uiCount->setTextOverflow(Label::Overflow::SHRINK);
            uiCount->setTextVerticalAlignment(TextVAlignment::CENTER);
            uiCount->setTextHorizontalAlignment(TextHAlignment::CENTER);
            uiBtnGo->addChild(uiCount);
        }
    }
    
    //스탑 버튼
    if(auto uiBtnStop = (ui::Button*)utils::findChild(uiButtons, "UI_BUTTON_STOP"); uiBtnStop != nullptr)
    {
        uiBtnStop->setEnabled(true);
        
        if(EventLuckyBagManager::getInstance()->getCurrentLevel() == 0 ||
           EventLuckyBagManager::getInstance()->getCurrentLevel() == 1)
        {
            //1번도 뽑기 안했을 때 막기
            if(EventLuckyBagManager::getInstance()->getCurrentLevel() == 0)
                uiBtnStop->setEnabled(false);
            
            if(auto uiText = (ui::CText*)utils::findChild(uiBtnStop, "TEXT"); uiText != nullptr)
            {
                uiText->setTextAreaSize(Size::ZERO);
                uiText->setString(GAME_TEXT("t_ui_event_enhance_draw_bt_stop"));
                uiText->setTextAreaSize(Size(uiBtnStop->getContentSize().width - 10, uiText->getContentSize().height));
            }
        }
        //마지막 레벨 달성시
        else if(EventLuckyBagManager::getInstance()->getCurrentLevel() == EventLuckyBagManager::getInstance()->getMaxLevel())
        {
            if(auto uiText = (ui::CText*)utils::findChild(uiBtnStop, "TEXT"); uiText != nullptr)
            {
                uiText->setTextAreaSize(Size::ZERO);
                uiText->setString(GAME_TEXT("t_ui_event_enhance_draw_bt_stop_2"));
                uiText->setTextAreaSize(Size(uiBtnStop->getContentSize().width - 10, uiText->getContentSize().height));
            }
        }
        //일반적인 상황
        else
        {
            if(auto uiText = (ui::CText*)utils::findChild(uiBtnStop, "TEXT"); uiText != nullptr)
            {
                uiText->setTextAreaSize(Size::ZERO);
                uiText->setString(GAME_TEXT("t_ui_event_enhance_draw_bt_stop_1"));
                uiText->setTextAreaSize(Size(uiBtnStop->getContentSize().width - 10, uiText->getContentSize().height));
            }
        }
    }
    
    //
    if(auto uiBtnCount = (ui::Button*)utils::findChild(uiButtons, "UI_BUTTON_COUNT"); uiBtnCount != nullptr)
    {
        if ( EventLuckyBagManager::getInstance()->getCurrentLevel() > 0 )
            uiBtnCount->setEnabled(false);
        else
            uiBtnCount->setEnabled(true);
    }
    
    //
    auto sizeButtons = UtilsWidget::getInstance()->getLayoutAccumulatedSize(uiButtons);
    uiButtons->setContentSize(sizeButtons);
    uiButtons->requestDoLayout();
    
}
void LayerEventLuckyBagMain::drawReward(ui::Layout* layout, int nStartIdx, int nLastIdx, Vec2 anchor)
{
    
    int nListHalfSize = 5;
    
    float width = 80;
    float posX = layout->getContentSize().width/2 - width * (nListHalfSize/2) - 44 * (nListHalfSize/2);
    float posY = layout->getContentSize().height/2;
    
    if(anchor == Vec2::ANCHOR_MIDDLE_BOTTOM)
        posY +=60;
    else
        posY -=20;
        
    
    for(int i = nStartIdx; i < nLastIdx; ++i)
    {
        auto objItem = EventLuckyBagManager::getInstance()->getMyRewardByLevel(i);
        
        std::string strProgressPath = "Assets/ui/event/event_step_box_1_1.png";
        std::string strLevelPath = "Assets/ui/event/event_lv_1_2.png";
        std::string strPath = "Assets/ui/event/event_reward_1_2.png";
        if ( i == 9 )
        {
            strPath = "Assets/ui/event/event_reward_2_1.png";
        }
        
        //해당 레벨 뽑았을 경우
        if(objItem != nullptr)
        {
            //방금 뽑았을 경우
            if( _nNowDrawRewardLv == i+1)
                strPath = "Assets/ui/event/event_reward_1_0.png";
            else
                strPath = "Assets/ui/event/event_reward_1_1.png";
            
            strProgressPath = "Assets/ui/event/event_step_box_1_2.png";
            strLevelPath = "Assets/ui/event/event_lv_1_1.png";
        }
        
        if(i + 1 == EventLuckyBagManager::getInstance()->getCurrentLevel()+1)
        {
            strPath = "Assets/ui/event/event_reward_1_4.png";
            
            if ( i == 9 )
            {
                strPath = "Assets/ui/event/event_reward_2_2.png";
            }
        }
            
        
        auto uiRewardBox = ui::ImageView::create(strPath);
        uiRewardBox->setAnchorPoint(anchor);
        uiRewardBox->setPosition(Vec2(posX, posY));
        layout->addChild(uiRewardBox);
        
        //첫 보상 확정 보상 표기
        if(i == 0)
        {
            auto uiText = ui::CText::create(GAME_TEXT("t_ui_event_enhance_draw_msg_5"), GAME_FONT, 20);
            uiText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
            uiText->setPosition(Vec2(uiRewardBox->getContentSize().width/2, uiRewardBox->getContentSize().height + 2));
            uiText->setTextAreaSize(Size(uiRewardBox->getContentSize().width, uiText->getContentSize().height));
            uiText->setTextOverflow(Label::Overflow::SHRINK);
            uiText->setTextVerticalAlignment(TextVAlignment::BOTTOM);
            uiText->setTextHorizontalAlignment(TextHAlignment::CENTER);
            uiText->setColor(Color3B(132,243,19));
            uiRewardBox->addChild(uiText);
        }
        
        auto uiLevelBox = ui::ImageView::create(strLevelPath);
        uiLevelBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        if(anchor == Vec2::ANCHOR_MIDDLE_BOTTOM)
            uiLevelBox->setPosition(uiRewardBox->getPosition() + Vec2::DOWN * 10);
        else
            uiLevelBox->setPosition(uiRewardBox->getPosition() + Vec2::DOWN * (uiRewardBox->getContentSize().height  + 10));
        layout->addChild(uiLevelBox);
        {
            auto uiText = ui::CText::create(MafUtils::format("LV.%d", i+1), GAME_FONT_AL, 20);
            uiText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            uiText->setPosition(uiLevelBox->getContentSize()/2);
            uiText->setTextAreaSize(Size(uiLevelBox->getContentSize().width - 10, uiText->getContentSize().height));
            uiText->setTextOverflow(Label::Overflow::SHRINK);
            uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
            uiText->setTextHorizontalAlignment(TextHAlignment::CENTER);
            uiLevelBox->addChild(uiText);
        }
        
        //프로그래스바
        if(i < nLastIdx - 1)
        {
            auto uiProgress = ui::ImageView::create(strProgressPath);
            if(anchor == Vec2::ANCHOR_MIDDLE_BOTTOM)
            {
                uiProgress->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
                uiProgress->setPosition(uiRewardBox->getPosition() + Vec2::UP * uiRewardBox->getContentSize().height/2 + Vec2::RIGHT * uiRewardBox->getContentSize().width/2);
            }
            else
            {
                uiProgress->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
                uiProgress->setPosition(uiRewardBox->getPosition() + Vec2::DOWN * uiRewardBox->getContentSize().height/2 + Vec2::RIGHT * uiRewardBox->getContentSize().width/2);
            }
                
            layout->addChild(uiProgress);
        }
        
        //첫 상품 확정 보상 이미지
        if(objItem != nullptr && i == 0)
        {
            auto uiLabel = ui::ImageView::create("Assets/ui/event/event_reward_1_3.png");
            uiLabel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
            uiLabel->setPosition(Vec2(0, uiRewardBox->getContentSize().height));
            uiRewardBox->addChild(uiLabel);
        }
        
        if(objItem != nullptr)
        {
            if (auto uiItemIcon = ui::ImageView::create(objItem->getPath()); uiItemIcon != nullptr )
            {
                uiItemIcon->setPosition(Vec2(uiRewardBox->getContentSize().width / 2, uiRewardBox->getContentSize().height / 2 + 10));
                ((ui::Scale9Sprite*)uiItemIcon->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
                uiRewardBox->addChild(uiItemIcon);
                
                std::string strItemsCount = "";
                
                if((E_ITEMS)objItem->getIdx() != E_ITEMS::COSTUME_SKIN)
                {
                    strItemsCount = "x" + objItem->getCount();
                    if (objItem->isString())
                        MafUtils::convertNumberToShort(strItemsCount);
                }
                
                auto uiCount = ui::CText::create(strItemsCount, GAME_FONT, 20);
                uiCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
                uiCount->setPosition(Vec2(uiRewardBox->getContentSize().width / 2, 10));
                uiCount->setTextAreaSize(Size(uiRewardBox->getContentSize().width - 10, uiCount->getContentSize().height));
                uiCount->setTextOverflow(Label::Overflow::SHRINK);
                uiCount->setTextVerticalAlignment(TextVAlignment::BOTTOM);
                uiCount->setTextHorizontalAlignment(TextHAlignment::CENTER);
                uiRewardBox->addChild(uiCount);
            }
        }
        
        if(nListHalfSize % 2 == 0)
        {
            if(anchor == Vec2::ANCHOR_MIDDLE_BOTTOM)
                uiRewardBox->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
            else
                uiRewardBox->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
                
        }
        
        if ( objItem != nullptr && EventLuckyBagManager::getInstance()->getCountNow() > 1 )
        {
            auto uiMulti = ui::ImageView::create(MafUtils::format("Assets/ui_common/icon_mul_%02d.png", EventLuckyBagManager::getInstance()->getCountNow()));
            uiMulti->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
            uiMulti->setPosition(Vec2(uiRewardBox->getContentSize().width, uiRewardBox->getContentSize().height));
            uiRewardBox->addChild(uiMulti);
        }
        
        posX += width + 44;
    }
}

#pragma mark - ui : set, get

#pragma mark - ui : click
void LayerEventLuckyBagMain::onClickStop(Ref* const sender)
{
    if(EventLuckyBagManager::getInstance()->getCurrentLevel() < 1)
        return;
    
    int nCount = EventLuckyBagManager::getInstance()->getCountNow();
    EventLuckyBagManager::getInstance()->requestProceeLuckyBag(true, 2, nCount);
}
void LayerEventLuckyBagMain::onClickGo(Ref* const sender)
{
    int nCount = EventLuckyBagManager::getInstance()->getCountNow();
    if(EventLuckyBagManager::getInstance()->getLuckyBagCount() < nCount && EventLuckyBagManager::getInstance()->getCurrentLevel() == 0)
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_31"));
        popup->addButton(GAME_TEXT("t_ui_close"), true, nullptr);
        popup->addButton(GAME_TEXT("t_ui_purchase_bonus_msg_4"), false, [](Ref*){
            auto popup = PopupCash::create();
            popup->show();
        });
        popup->show();
        return;
    }
    
    if(EventLuckyBagManager::getInstance()->getCurrentLevel() == 0 || !EventLuckyBagManager::getInstance()->isEffect())
    {
        EventLuckyBagManager::getInstance()->requestProceeLuckyBag(true, 1, nCount);
    }
    else
    {
        auto popup = PopupLuckyBagDrawEffect::create(PopupLuckyBagDrawEffect::E_TYPE::TRY);
        popup->show();
    }
}
void LayerEventLuckyBagMain::onClickCount(Ref* const sender)
{
    int countNow = EventLuckyBagManager::getInstance()->getCountNow();
    int countMax = EventLuckyBagManager::getInstance()->getCountMax();
    
    countNow++;
    countNow = countNow > countMax ? 1 : countNow;
    
    //
    EventLuckyBagManager::getInstance()->setCountNow(countNow);
    
    //
    drawButtton();
}
#pragma mark - callback
void LayerEventLuckyBagMain::callbackProgress(const bool bResult, const int nResult)
{
    if(bResult == false)
        return;
    
    _nNowDrawRewardLv = EventLuckyBagManager::getInstance()->getNowDrawRewardLv();
    EventLuckyBagManager::getInstance()->setNowDrawRewardLv(0);
}
#pragma mark - schedule

#pragma mark - game
void LayerEventLuckyBagMain::onDrawInfo()
{
    drawMyReward();
    drawButtton();
}
