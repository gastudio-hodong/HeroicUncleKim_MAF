//
//  PopupAdventureRelicCraft.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/05/11.
//

#include "PopupAdventureRelicCraft.h"

#include "Common/ConfigGameNormal.h"

#include "GameObject/Adventure/InfoAdventureRelic.h"

#include "GameUI/UI/UICurrency.h"

#include "GameUIPopup/Other/CurrencyDetail/PopupCurrencyDetail.h"
#include "GameUIPopup/Other/CurrencyDetail/PopupCurrencyDetail.h"

#include "ManagerGame/ItemsManager.h"
#include "ManagerGame/AdventureManager.h"
#include "ManagerGame/AdventureRelicManager.h"

#pragma mark - popup

PopupAdventureRelicCraft* PopupAdventureRelicCraft::create()
{
    PopupAdventureRelicCraft* pRet = new(std::nothrow) PopupAdventureRelicCraft();
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

PopupAdventureRelicCraft::PopupAdventureRelicCraft(void):
_isMakeStart(false)
,_isClose(false)
,_isMakeEnd(false)
,_infoCore(nullptr)
,_callback(nullptr)
,_lyBG(nullptr)
,_lyTopUI(nullptr)
,_lyMiddleUI(nullptr)
,_lyBottomUI(nullptr)
,_table(nullptr)
,_callbackTransferShop(nullptr)
,_callbackTutorial(nullptr)
{
    _vecReward.clear();
}

PopupAdventureRelicCraft::~PopupAdventureRelicCraft(void)
{
}

#pragma mark - table
void PopupAdventureRelicCraft::tableCellTouched(extension::TableView* table, extension::TableViewCell* cell)
{
    
}

Size PopupAdventureRelicCraft::tableCellSizeForIndex(extension::TableView *table, ssize_t idx)
{
    return Size(table->getContentSize().width, 130);
}

extension::TableViewCell* PopupAdventureRelicCraft::tableCellAtIndex(extension::TableView *table, ssize_t idx)
{
    auto cell = table->dequeueCell();
    if ( cell == nullptr )
    {
        cell = new extension::TableViewCell();
        cell->autorelease();
    }
    if(idx == 0 && _isMakeStart == false)
        _isMakeStart = true;
    
    cell->removeAllChildren();
    
    
    // size
    Size size = tableCellSizeForIndex(table, idx);
    cell->setContentSize(size);
    
    // obj
    ssize_t nIdxStart = idx * 5;
    ssize_t nIdxEnd = nIdxStart + 5;
    
    if ( nIdxEnd > _vecReward.size() )
    {
        nIdxEnd = _vecReward.size();
    }
    
    for ( int i = 0; nIdxStart + i < nIdxEnd; i++ )
    {
        int nIdx = (int)nIdxStart + i;
        
        auto objItems = _vecReward.at(nIdx);
        if ( objItems == nullptr )
        {
            continue;
        }
        
        double nPosX = 95 + 82 * i + 35 * i;
        double nPosY = cell->getContentSize().height - 20;
        std::string strBoxPath = "Assets/ui_common/box_item.png";
        std::string strBackFx = "Assets/ui_common/box_item_fx1_1.png";
        
        InfoAdventureRelic* objRelic = nullptr;
        int grade = 0;
        if(objItems->getIdx() == (int) E_ITEMS::ADVENTURE_RELIC)
            objRelic = AdventureRelicManager::getInstance()->getInfoRelic(objItems->getTypeParams());
        
        if(objRelic != nullptr)
        {
            grade = objRelic->getGrade();
            strBoxPath = MafUtils::format("Assets/ui/adventure/adventure_box_%02d.png", grade);
            
            if(grade == 4)
                strBackFx = "Assets/ui_common/box_item_fx1_1.png";
            if(grade == 5)
                strBackFx = "Assets/ui_common/box_item_fx1_2.png";
        }
        
        auto sprBox = ui::Scale9Sprite::create(strBoxPath);
        sprBox->setContentSize(Size(82,82));
        sprBox->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        sprBox->setPosition(nPosX, nPosY);
        cell->addChild(sprBox, E_ZORER::UI);
        
        auto sprIconAreaCore = Sprite::create(MafUtils::format("Assets/icon/icon_core_%02d.png", objRelic->getArea()));
        sprIconAreaCore->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        sprIconAreaCore->setScale(0.4f);
        sprIconAreaCore->setPosition(5, sprBox->getContentSize().height - 5);
        sprBox->addChild(sprIconAreaCore);
        
        auto sprBoxEffect = ui::Scale9Sprite::create("Assets/ui_common/box_item_82_1.png");
        sprBoxEffect->setContentSize(sprBox->getContentSize());
        sprBoxEffect->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        sprBoxEffect->setPosition(sprBox->getContentSize()/2);
        sprBox->addChild(sprBoxEffect, E_ZORER::UI);
        
        auto sprGradeFx = Sprite::create(strBackFx);
        sprGradeFx->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        sprGradeFx->setPosition(sprBox->getPosition() + Vec2::RIGHT * sprBox->getContentSize().width/2 + Vec2::DOWN * sprBox->getContentSize().height/2);
        cell->addChild(sprGradeFx, E_ZORER::UI - 1);
        sprGradeFx->setVisible(false);
        {
            if(objRelic != nullptr)
            {
                if(objRelic->getGrade() >= 4)
                {
                    auto action = RotateBy::create(2, 360);
                    sprGradeFx->runAction(RepeatForever::create(action));
                }
            }
        }
        auto sprGradeFx2 = Sprite::create("Assets/ui_common/box_item_82_2_1.png");
        sprGradeFx2->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        sprGradeFx2->setPosition(sprBox->getContentSize()/2);
        sprBox->addChild(sprGradeFx2);
        sprGradeFx2->setVisible(false);
        {
            if(objRelic != nullptr)
            {
                if(objRelic->getGrade() == 5)
                {
                    Animation *animation = Animation::create();
                    animation->setDelayPerUnit(0.1);
                    for(int i = 0; i < 7; ++i)
                    {
                        auto spriteAni = Sprite::create(MafUtils::format("Assets/ui_common/box_item_82_2_%d.png", i+1));
                        spriteAni->getTexture()->setTexParameters(texParams);
                        animation->addSpriteFrame(spriteAni->getSpriteFrame());

                    }
                    sprGradeFx2->runAction(RepeatForever::create(Animate::create(animation)));
                }
            }
        }
        
        
        sprBox->setVisible(false);
        
        auto sprIcon = Sprite::create(objItems->getPath());
        sprIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        sprIcon->setPosition(sprBox->getContentSize()/2);
        sprBox->addChild(sprIcon);
        
        if(objItems->getIdx() != (int)E_ITEMS::ADVENTURE_RELIC)
        {
            auto lbCount = Label::createWithTTF(objItems->getCount(), GAME_FONT, 22);
            lbCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
            lbCount->setPosition(sprBox->getContentSize().width/2, 0);
            lbCount->setDimensions(sprBox->getContentSize().width, sprBox->getContentSize().height);
            lbCount->setOverflow(cocos2d::Label::Overflow::SHRINK);
            lbCount->setAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::BOTTOM);
            
            sprBox->addChild(lbCount);
        }
        
        if(_isMakeEnd == false && _isMakeStart == true)
        {
            float time = (float)idx / 10 + i * 0.02f;
            
            if(idx >= 7)
                time = (float)(idx-7) / 10 + i * 0.02f;
            
            auto action1 = DelayTime::create(time);
            auto action2 = Show::create();
            auto action3 = CallFunc::create([=](){
                
                float y = _table->getContentOffset().y - size.height;
                
                if(y <= 0)
                    y = 0;
                _table->setContentOffset(Vec2(_table->getContentOffset().x, y), true);
            });
            if(nIdx >= 29 && nIdxStart + i + 1 >= nIdxEnd)
            {
                auto seq = Sequence::create(action1, action2, action3, nullptr);
                sprBox->runAction(seq);
            }
            else
            {
                auto seq = Sequence::create(action1, action2, nullptr);
                sprBox->runAction(seq);
            }
            
            auto actionEffect1 = DelayTime::create(time);
            auto actionEffect2 = FadeOut::create(0.2f);
            auto actionEffect3 = CallFunc::create([=](){
                sprGradeFx->runAction(Show::create());
                
                if(grade >= 5)
                    sprGradeFx2->runAction(Show::create());
                    
            });
            
            if(objRelic != nullptr)
            {
                if(grade >= 4)
                {
                    sprBoxEffect->runAction(Sequence::create(actionEffect1, Spawn::create(actionEffect2, actionEffect3, nullptr), nullptr));
                }
                else
                    sprBoxEffect->runAction(Sequence::create(actionEffect1, actionEffect2, nullptr));
            }
            else
            {
                sprBoxEffect->runAction(Sequence::create(actionEffect1, actionEffect2, nullptr));
            }
            
        }
        
        if(_isMakeEnd == true)
        {
            sprBox->setVisible(true);
            sprBoxEffect->setVisible(false);
            
            if(objRelic != nullptr)
            {
                if(grade >= 4)
                {
                    sprGradeFx->setVisible(true);
                    
                    if(grade >= 5)
                        sprGradeFx2->setVisible(true);
                }
                else
                    sprGradeFx->setVisible(false);
            }
        }
        
    }
    
    size_t maxIdx = _vecReward.size()/5;
    if ( _vecReward.size() % 5 != 0 )
        maxIdx++;

    if(idx == maxIdx - 1 && _isMakeStart == true)
        _isMakeEnd = true;
    
    return cell;
}

ssize_t PopupAdventureRelicCraft::numberOfCellsInTableView(extension::TableView *table)
{
    size_t size = _vecReward.size()/5;
    if ( _vecReward.size() % 5 != 0 )
        size++;
    return size;
}


#pragma mark - init
bool PopupAdventureRelicCraft::init()
{
    if ( !PopupBase::init() )
    {
        return false;
    }

    initVar();
    initUI();
    _bHideBackKey = false;
    if(AdventureManager::getInstance()->getTutorialStep() < 7)
        RefreshManager::getInstance()->addUI(E_REFRESH::ADVENTURE_TUTORIAL_COMPLETE, this);
    return true;
}

void PopupAdventureRelicCraft::initVar()
{
    _spriteContainer->setVisible(false);
}

void PopupAdventureRelicCraft::initUI()
{
    _lyBG = LayerColor::create(Color4B(0,0,0,0));
    _lyBG->setContentSize(getContentSize());
    _lyBG->setIgnoreAnchorPointForPosition(false);
    _lyBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _lyBG->setPosition(getContentSize()/2);
    _lyBG->setOpacity(165);
    addChild(_lyBG);
    
    _lyTopUI = LayerColor::create();
    _lyTopUI->setContentSize(Size(getContentSize().width - 10, 150));
    _lyTopUI->setIgnoreAnchorPointForPosition(false);
    _lyTopUI->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _lyTopUI->setPosition(getContentSize().width / 2, getContentSize().height);
    addChild(_lyTopUI);
    
    _lyMiddleUI = LayerColor::create();
    _lyMiddleUI->setContentSize( Size(getContentSize().width - 10, 800));
    _lyMiddleUI->setIgnoreAnchorPointForPosition(false);
    _lyMiddleUI->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _lyMiddleUI->setPosition( _lyTopUI->getPosition() + Vec2::DOWN * _lyTopUI->getContentSize().height);
    addChild(_lyMiddleUI);
    
    _lyBottomUI = LayerColor::create();
    _lyBottomUI->setContentSize( Size(getContentSize().width - 10, getContentSize().height - _lyTopUI->getContentSize().height - _lyMiddleUI->getContentSize().height - 10));
    _lyBottomUI->setIgnoreAnchorPointForPosition(false);
    _lyBottomUI->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _lyBottomUI->setPosition( _lyMiddleUI->getPosition() + Vec2::DOWN * _lyMiddleUI->getContentSize().height);
    addChild(_lyBottomUI);
    

}
#pragma mark - ui
void PopupAdventureRelicCraft::uiTop()
{
    _lyTopUI->removeAllChildren();
    
    auto sprBox = Sprite::create("Assets/ui_common/base_text_1_4.png");
    sprBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    sprBox->setPosition(_lyTopUI->getContentSize()/2);
    _lyTopUI->addChild(sprBox);
    
    auto lbText = Label::createWithTTF(GAME_TEXT("t_ui_advt_info_title_8"), GAME_FONT, 26);
    lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    lbText->setAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
    lbText->setDimensions(sprBox->getContentSize().width -10, sprBox->getContentSize().height - 5);
    lbText->setPosition(sprBox->getContentSize()/2);
    lbText->setColor(COLOR_TITLE);
    sprBox->addChild(lbText);
}

void PopupAdventureRelicCraft::uiMiddle()
{
    _lyMiddleUI->removeAllChildren();
    
    _table = extension::TableView::create(this, Size(_lyMiddleUI->getContentSize().width, _lyMiddleUI->getContentSize().height - 10));
    _table->setIgnoreAnchorPointForPosition(false);
    _table->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _table->setPosition(_lyMiddleUI->getContentSize().width / 2, 0);
    _table->setDirection(extension::TableView::Direction::VERTICAL);
    _table->setVerticalFillOrder(extension::TableView::VerticalFillOrder::TOP_DOWN);
    _table->setDelegate(this);
    _table->setBounceable(false);
    _lyMiddleUI->addChild(_table);
    
    
    Vec2 pos = _table->getContentOffset();
    _table->setContentOffset(Vec2(pos.x, _table->getContentSize().height));
    
    _isMakeStart = false;
    _isMakeEnd = false;
    _table->reloadData();
}

void PopupAdventureRelicCraft::uiBottom()
{
    _lyBottomUI->removeAllChildren();
    
    bool isFriendCraft = false;
    if(_infoCore->getIdx() == 7)
        isFriendCraft = true;
    
    Vector<InfoItems*> list1;
    auto objItemsCore = ItemsManager::getInstance()->getInfoItems(E_ITEMS::ADVENTURE_CORE);
    objItemsCore->setTypeParams(_infoCore->getIdx());
    list1.pushBack(objItemsCore);
    
    auto uiCurrency = UICurrency::create(list1, Size(_lyBottomUI->getContentSize().width - 120, 40));
    uiCurrency->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    uiCurrency->setPosition(Vec2(_lyBottomUI->getContentSize().width/2, _lyBottomUI->getContentSize().height - 30));
    _lyBottomUI->addChild(uiCurrency);
    
    std::vector<int> listBtn;
    listBtn.clear();
    if(!isFriendCraft)
    {
        const int nPrice = AdventureRelicManager::getCraftPrice(_infoCore->getIdx(), 1);
        const int nAllCount = _infoCore->getCount() / nPrice >= 30 ? 30 : _infoCore->getCount() / nPrice;
        
        listBtn.push_back(1);
        listBtn.push_back(10);
        listBtn.push_back(nAllCount);
    }
    
    int size = (int)listBtn.size();
    
    float width = (_lyTopUI->getContentSize().width - 100) / size - 10;
    float posX = _lyTopUI->getContentSize().width/2 - width * (size/2) - 10 * (size/2);
    
    for(int i = 0; i < listBtn.size(); ++i)
    {
        int cnt = listBtn[i];
        
        auto btnCraft = ui::Button::create("Assets/ui_common/btn_1_1_on.png");
        btnCraft->setScale9Enabled(true);
        btnCraft->setContentSize(Size(190,107));
        btnCraft->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        btnCraft->setPosition(Vec2(posX, uiCurrency->getPosition().y - uiCurrency->getContentSize().height));
        btnCraft->addClickEventListener(CC_CALLBACK_1(PopupAdventureRelicCraft::onClickCraft, this));
        btnCraft->setZoomScale(0.05f);
        btnCraft->setTag(cnt);
        _lyBottomUI->addChild(btnCraft);
        {
            auto lbText = Label::createWithTTF(GAME_TEXT("t_ui_advt_relic_crafting_bt") + MafUtils::format("x%d", cnt), GAME_FONT, 26);
            lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            lbText->setPosition(btnCraft->getContentSize().width/2, btnCraft->getContentSize().height - btnCraft->getContentSize().height/4);
            lbText->setDimensions(btnCraft->getContentSize().width - 10, btnCraft->getContentSize().height/2);
            lbText->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
            lbText->setOverflow(Label::Overflow::SHRINK);
            btnCraft->addChild(lbText);
            
            auto sprCurrencyIcon = Sprite::create(MafUtils::format("Assets/icon/icon_core_%02d.png",_infoCore->getIdx()));
            sprCurrencyIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            sprCurrencyIcon->setPosition(btnCraft->getContentSize().width/2.6f, btnCraft->getContentSize().height/4);
            btnCraft->addChild(sprCurrencyIcon);
            
            auto lbCount = Label::createWithTTF(std::to_string(AdventureRelicManager::getCraftPrice(_infoCore->getIdx(), cnt)), GAME_FONT, 26);
            lbCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            lbCount->setPosition(sprCurrencyIcon->getPosition() + Vec2::RIGHT * sprCurrencyIcon->getContentSize().width/2);
            lbCount->setDimensions(100, btnCraft->getContentSize().height/2);
            lbCount->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
            lbCount->setOverflow(Label::Overflow::SHRINK);
            btnCraft->addChild(lbCount);
        }
        
        if(size % 2 == 0)
            btnCraft->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        
        posX += width + 10;
    }
    
    auto btnClose = ui::Button::create("Assets/ui_common/btn_1_1_on.png");
    btnClose->setScale9Enabled(true);
    btnClose->setContentSize(Size(300,107));
    btnClose->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    btnClose->setPosition(Vec2(_lyBottomUI->getContentSize().width/2, 50));
    btnClose->addClickEventListener(CC_CALLBACK_1(PopupAdventureRelicCraft::onClickClose, this));
    btnClose->setZoomScale(0.05f);
    _lyBottomUI->addChild(btnClose);
    {
        auto lbText = Label::createWithTTF(GAME_TEXT("t_ui_close"), GAME_FONT, 26);
        lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        lbText->setPosition(btnClose->getContentSize()/2);
        lbText->setDimensions(btnClose->getContentSize().width - 10, btnClose->getContentSize().height/2);
        lbText->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        lbText->setOverflow(Label::Overflow::SHRINK);
        btnClose->addChild(lbText);
    }
}

#pragma mark - Override
void PopupAdventureRelicCraft::refresh(E_REFRESH eType, cocos2d::ValueMap listData)
{
    if(eType == E_REFRESH::ADVENTURE_TUTORIAL_COMPLETE)
        this->setLocalZOrder(ZORDER_SCENE_POPUP);
}
void PopupAdventureRelicCraft::show()
{
    PopupBase::show();
    
    if(AdventureManager::getInstance()->getTutorialStep() < 7)
        this->setLocalZOrder(ZORDER_SCENE_FORCE_TUTORIAL);
    uiTop();
    uiMiddle();
    uiBottom();
}

#pragma mark - utils
void PopupAdventureRelicCraft::setCallback(const std::function<void(void)>& callback)
{
    _callback = callback;
}
void PopupAdventureRelicCraft::setCallbackTutorial(const std::function<void(void)>& callback)
{
    _callbackTutorial = callback;
}
void PopupAdventureRelicCraft::setCallbackTransferShop(const std::function<void(void)>& callback)
{
    _callbackTransferShop = callback;
}

void PopupAdventureRelicCraft::setReward(Vector<InfoItems*> vecReward)
{
    _vecReward = vecReward;
}
void PopupAdventureRelicCraft::setCore(InfoAdventureCore* infoCore)
{
    _infoCore = infoCore;
}

void PopupAdventureRelicCraft::onClickClose(Ref* sender)
{
    if(_callbackTutorial != nullptr)
        _callbackTutorial();
    hide();
}

void PopupAdventureRelicCraft::onClickCraft(Ref* sender)
{
    auto node = (Node*)sender;
    
    if(node == nullptr)
        return;
    
    const int nPrice = AdventureRelicManager::getCraftPrice(_infoCore->getIdx(), node->getTag());
    
    if(_infoCore->getCount() < nPrice || nPrice <= 0)
    {
        auto popup = PopupCurrencyDetail::create(E_ITEMS::ADVENTURE_CORE, PopupCurrencyDetail::E_TYPE::NONE, _infoCore->getIdx());
        popup->setCallbackTransferShop(CC_CALLBACK_0(PopupAdventureRelicCraft::callbackTranferShop, this));
        popup->show();
        return;
    }
    
    PopupLoading::show();
    AdventureRelicManager::getInstance()->requestCraftRelic(CC_CALLBACK_3(PopupAdventureRelicCraft::callbackSummon, this), _infoCore->getIdx(), node->getTag());
}
#pragma mark -Callback
void PopupAdventureRelicCraft::callbackSummon(bool bResult, int nResult, Vector<InfoItems*> listReward)
{
    PopupLoading::hide();
    
    if(nResult != 1)
    {
        std::string strMessageKey = "t_ui_error_30";
        
        if(nResult == 7)
            strMessageKey = "t_ui_advt_error_2";
        else if(nResult == 8)
            strMessageKey = "t_ui_advt_error_3";
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT(strMessageKey));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        if(AdventureManager::getInstance()->getTutorialStep() < 7)
            popup->setLocalZOrder(ZORDER_SCENE_FORCE_TUTORIAL);
        
        if(nResult == 7 || nResult == 8)
        {
            ValueMap listData;
            listData["inspection_code"] = Value(nResult);
            RefreshManager::getInstance()->refreshUI(E_REFRESH::ADVENTURE, listData);
        }
        
        hide();
        return;
    }
    
    _vecReward = listReward;
    RefreshManager::getInstance()->refreshUI(E_REFRESH::CURRENCY);
    
    if(AdventureManager::getInstance()->getTutorialStep() > 6)
        RefreshManager::getInstance()->refreshUI(E_REFRESH::ADVENTURE);
    
    if(_callback != nullptr)
        _callback();
    uiMiddle();
    uiBottom();
}

void PopupAdventureRelicCraft::callbackTranferShop()
{
    if(_callbackTransferShop != nullptr && AdventureManager::getInstance()->getTutorialStep() > 3)
        _callbackTransferShop();
    if(_callbackTutorial != nullptr)
        _callbackTutorial();
    hide();
}
