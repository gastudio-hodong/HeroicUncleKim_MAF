//
//  PopupSpiritSummon.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/12/01.
//

#include "PopupSpiritSummon.h"

#include "Common/ConfigGameNormal.h"

#include "GameUI/UI/UICurrency.h"

#include "GameUIPopup/Other/CurrencyDetail/PopupCurrencyDetail.h"
#include "GameUIPopup/Other/CurrencyDetail/PopupCurrencyDetail.h"

#include "ManagerGame/NewRaidManager.h"
#include "ManagerGame/WeaponSpiritManager.h"
#include "ManagerGame/ItemsManager.h"

USING_NS_CC;

PopupSpiritSummon* PopupSpiritSummon::create()
{
    PopupSpiritSummon* pRet = new(std::nothrow) PopupSpiritSummon();
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

PopupSpiritSummon::PopupSpiritSummon(void):
_isMakeStart(false)
,_isClose(false)
,_isMakeEnd(false)
,_callback(nullptr)
,_lyBG(nullptr)
,_lyTopUI(nullptr)
,_lyMiddleUI(nullptr)
,_lyBottomUI(nullptr)
,_table(nullptr)
,_callbackTransferShop(nullptr)
{
    _vecReward.clear();
}

PopupSpiritSummon::~PopupSpiritSummon(void)
{
}

#pragma mark - table
void PopupSpiritSummon::tableCellTouched(extension::TableView* table, extension::TableViewCell* cell)
{
}

Size PopupSpiritSummon::tableCellSizeForIndex(extension::TableView *table, ssize_t idx)
{
    return Size(table->getContentSize().width, 130);
}

extension::TableViewCell* PopupSpiritSummon::tableCellAtIndex(extension::TableView *table, ssize_t idx)
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
    
    int maxGrade = WeaponSpiritManager::getInstance()->getSpiritMaxGrade();
    
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
        
        if(objItems->getIdx() == (int) E_ITEMS::SPIRIT_STON)
            strBoxPath = "Assets/ui_common/box_item_2_1.png";
        
        InfoWeaponSpirit* objSpirit = nullptr;
        int grade = 0;
        if(objItems->getIdx() == (int) E_ITEMS::WEAPON_SPIRIT)
            objSpirit = WeaponSpiritManager::getInstance()->getInfoWeaponSpirit(objItems->getTypeParams());
        
        if(objSpirit != nullptr)
        {
            grade = objSpirit->getGrade();
            strBoxPath = MafUtils::format("Assets/ui/spirit/box_icon_spirit_%02d.png", grade);
            
            //상위 3등급 후광 리소스 경로
            if(grade == 5)
                strBackFx = "Assets/ui_common/box_item_fx1_1.png";
            if(grade == 6)
                strBackFx = "Assets/ui_common/box_item_fx1_2.png";
            if(grade == 7)
                strBackFx = "Assets/ui_common/box_item_fx1_3.png";
            if(grade == 8)
                strBackFx = "Assets/ui_common/box_item_fx1_2.png";
        }
        
        auto sprBox = ui::Scale9Sprite::create(strBoxPath);
        sprBox->setContentSize(Size(82,82));
        sprBox->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        sprBox->setPosition(nPosX, nPosY);
        cell->addChild(sprBox, E_ZORER::UI);
        
        auto sprBoxEffect = ui::Scale9Sprite::create("Assets/ui_common/box_item_82_1.png");
        sprBoxEffect->setContentSize(sprBox->getContentSize());
        sprBoxEffect->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        sprBoxEffect->setPosition(sprBox->getContentSize()/2);
        sprBox->addChild(sprBoxEffect, E_ZORER::UI);
        
        /*
         뽑기 효과 1
         후광 효과
         최대 등급 - 2 이상 등급일 경우 활성화 하여 360도 회전
         */
        auto sprGradeFx = Sprite::create(strBackFx);
        sprGradeFx->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        sprGradeFx->setPosition(sprBox->getPosition() + Vec2::RIGHT * sprBox->getContentSize().width/2 + Vec2::DOWN * sprBox->getContentSize().height/2);
        cell->addChild(sprGradeFx, E_ZORER::UI - 1);
        sprGradeFx->setVisible(false);
        if(objSpirit != nullptr)
        {
            if(objSpirit->getGrade() >= 5)
            {
                auto action = RotateBy::create(2, 360);
                sprGradeFx->runAction(RepeatForever::create(action));
            }
        }
        /*
         뽑기 효과 2
         뽑기 2차 효과
         최대 등급 -1이상 등급에서 출력되는 2차 효과
         */
        auto sprGradeFx2 = Sprite::create("Assets/ui_common/box_item_82_2_1.png");
        sprGradeFx2->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        sprGradeFx2->setPosition(sprBox->getContentSize()/2);
        sprGradeFx2->setVisible(false);
        sprBox->addChild(sprGradeFx2);
        
        if(objSpirit != nullptr)
        {
            if(objSpirit->getGrade() == 6)
            {
                Animation *animation = Animation::create();
                animation->setDelayPerUnit(0.1);
                for(int i = 1; i <= 7; ++i)
                {
                    auto spriteAni = Sprite::create(MafUtils::format("Assets/ui_common/box_item_82_2_%d.png", i));
                    spriteAni->getTexture()->setTexParameters(texParams);
                    animation->addSpriteFrame(spriteAni->getSpriteFrame());
                }
                sprGradeFx2->runAction(RepeatForever::create(Animate::create(animation)));
            }
            else if(objSpirit->getGrade() == 7)
            {
                Animation *animation = Animation::create();
                animation->setDelayPerUnit(0.1);
                for(int i = 1; i <= 7; ++i)
                {
                    auto spriteAni = Sprite::create(MafUtils::format("Assets/ui_common/box_item_82_3_%d.png", i));
                    spriteAni->getTexture()->setTexParameters(texParams);
                    animation->addSpriteFrame(spriteAni->getSpriteFrame());
                }
                sprGradeFx2->runAction(RepeatForever::create(Animate::create(animation)));
            }
            else if(objSpirit->getGrade() == 8)
            {
                Animation *animation = Animation::create();
                animation->setDelayPerUnit(0.1);
                for(int i = 1; i <= 8; ++i)
                {
                    auto spriteAni = Sprite::create(MafUtils::format("Assets/ui_common/box_item_82_4_%d.png", i));
                    spriteAni->getTexture()->setTexParameters(texParams);
                    animation->addSpriteFrame(spriteAni->getSpriteFrame());
                }
                sprGradeFx2->runAction(RepeatForever::create(Animate::create(animation)));
            }
        }
        
        /*
        뽑기 효과 3
        파티클 효과 6.2.02 추가
        제일 높은 등급만 보여줌
        */
        auto summonVfx = ParticleSystemQuad::create("Assets/Particle/particle_spirit_summon_2.plist");
        summonVfx->setScale(0.6f);
        summonVfx->setPosition(sprBox->getContentSize()/2);
        summonVfx->setVisible(false);
        sprBox->addChild(summonVfx);
        
        sprBox->setVisible(false);
        
        if(auto sprIcon = Sprite::create(objItems->getPath()); sprIcon != nullptr)
        {
            sprIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            sprIcon->setPosition(sprBox->getContentSize()/2);
            sprBox->addChild(sprIcon);
        }
        
        if(objItems->getIdx() != (int)E_ITEMS::WEAPON_SPIRIT)
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
                /*
                 처음 뽑기 등장할 때 등급별 추가 연출 제어
                */
                if(grade >= 6)
                    sprGradeFx2->runAction(Show::create());
                
                if(grade >= 7)
                    summonVfx->runAction(Show::create());
                    
            });
            
            if(objSpirit != nullptr)
            {
                if(grade >= 5)
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
            
            if(objSpirit != nullptr)
            {
                /*
                 최초 뽑기 연출 이후 상위 3등급 뽑기 연출 on/off 제어
                 */
                if(grade >= 5)
                    sprGradeFx->setVisible(true);
                if(grade >= 6)
                    sprGradeFx2->setVisible(true);
                if(grade >= 7)
                    summonVfx->setVisible(true);
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

ssize_t PopupSpiritSummon::numberOfCellsInTableView(extension::TableView *table)
{
    size_t size = _vecReward.size()/5;
    if ( _vecReward.size() % 5 != 0 )
        size++;
    return size;
}


#pragma mark - init
bool PopupSpiritSummon::init()
{
    if ( !PopupBase::init() )
    {
        return false;
    }

    initVar();
    initUI();
    
    return true;
}

void PopupSpiritSummon::initVar()
{
    _spriteContainer->setVisible(false);
}

void PopupSpiritSummon::initUI()
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
    
    uiTop();
    uiMiddle();
    uiBottom();
}
#pragma mark - ui
void PopupSpiritSummon::uiTop()
{
    _lyTopUI->removeAllChildren();
    
    auto sprBox = Sprite::create("Assets/ui_common/base_text_1_4.png");
    sprBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    sprBox->setPosition(_lyTopUI->getContentSize()/2);
    _lyTopUI->addChild(sprBox);
    
    auto lbText = Label::createWithTTF(GAME_TEXT("t_ui_raidshop_spiritbox_bt"), GAME_FONT, 26);
    lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    lbText->setAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
    lbText->setDimensions(sprBox->getContentSize().width -10, sprBox->getContentSize().height - 5);
    lbText->setPosition(sprBox->getContentSize()/2);
    lbText->setColor(COLOR_TITLE);
    sprBox->addChild(lbText);
}

void PopupSpiritSummon::uiMiddle()
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

void PopupSpiritSummon::uiBottom()
{
    _lyBottomUI->removeAllChildren();
    
    int nSummonPrice = 10;
    int nSummonPrice11 = 100;
    int nSummonPrice33 = 300;
    
    if(auto setting = NewRaidManager::getInstance()->getRaidSetting("spirit_draw_price_1"))
        nSummonPrice = setting->getValueToInt();
    if(auto setting = NewRaidManager::getInstance()->getRaidSetting("spirit_draw_price_11"))
        nSummonPrice11 = setting->getValueToInt();
    if(auto setting = NewRaidManager::getInstance()->getRaidSetting("spirit_draw_price_33"))
        nSummonPrice33 = setting->getValueToInt();
    
    std::vector<E_ITEMS> list1;
    list1.push_back(E_ITEMS::NEW_RAID_DRAGON_BALL);
    list1.push_back(E_ITEMS::SPIRIT_STON);
    
    auto uiCurrency = UICurrency::create(list1, Size(_lyBottomUI->getContentSize().width - 120, 40));
    uiCurrency->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    uiCurrency->setPosition(Vec2(_lyBottomUI->getContentSize().width/2, _lyBottomUI->getContentSize().height - 30));
    _lyBottomUI->addChild(uiCurrency);
    
    std::vector<PopupSpiritSummon::E_TYPE> listTab;
    listTab.clear();
    
    if(NewRaidManager::getInstance()->getDragonBall() >= nSummonPrice33
       || WeaponSpiritManager::getInstance()->getSummonTicket_33() >= 1)
    {
        listTab.push_back(PopupSpiritSummon::E_TYPE::CLOSE);
        listTab.push_back(PopupSpiritSummon::E_TYPE::SUMMON_11);
        listTab.push_back(PopupSpiritSummon::E_TYPE::SUMMON_33);
    }
    else
    {
        listTab.push_back(PopupSpiritSummon::E_TYPE::CLOSE);
        listTab.push_back(PopupSpiritSummon::E_TYPE::SUMMON_1);
        listTab.push_back(PopupSpiritSummon::E_TYPE::SUMMON_11);
    }
    
    int size = (int)listTab.size();
    
    float width = (_lyTopUI->getContentSize().width - 100) / size - 10;
    float posX = _lyTopUI->getContentSize().width/2 - width * (size/2) - 10 * (size/2);
    
    for(int i = 0; i < listTab.size(); ++i)
    {
        auto obj = listTab[i];
        
        std::string strPath = "Assets/ui_common/btn_upgrade_bg.png";
        std::string strIconPath = "Assets/icon/icon_draball_1.png";
        std::string strText = "";
        int nPrice = 0;
        switch (obj) {
            case PopupSpiritSummon::E_TYPE::CLOSE: strText = GAME_TEXT("t_ui_confirm"); break;
            case PopupSpiritSummon::E_TYPE::SUMMON_1:
            {
                strText = GAME_TEXT("t_ui_raidshop_spiritbox_bt_draw1");
                nPrice = nSummonPrice;
            }break;
            case PopupSpiritSummon::E_TYPE::SUMMON_11:
            {
                strText = GAME_TEXT("t_ui_raidshop_spiritbox_bt_draw11");
                nPrice = nSummonPrice11;
            }break;
            case PopupSpiritSummon::E_TYPE::SUMMON_33:
            {
                strText = GAME_TEXT("t_ui_raidshop_spiritbox_bt_draw33");
                nPrice = nSummonPrice33;
            }break;
            default:  break;
        }
        
        auto btn = ui::Button::create(strPath);
        btn->setScale9Enabled(true);
        btn->setContentSize(Size(width, 90));
        btn->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        btn->setPosition(Vec2(posX, uiCurrency->getPosition().y - uiCurrency->getContentSize().height));
        btn->addClickEventListener(CC_CALLBACK_1(PopupSpiritSummon::onClick, this));
        btn->setZoomScale(0.05f);
        btn->setTag((int)obj);
        _lyBottomUI->addChild(btn);
        {
            auto lbText = Label::createWithTTF(strText, GAME_FONT, 26);
            lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            lbText->setPosition(btn->getContentSize().width/2, btn->getContentSize().height - btn->getContentSize().height/4);
            lbText->setDimensions(btn->getContentSize().width - 10, btn->getContentSize().height/2);
            lbText->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
            lbText->setOverflow(Label::Overflow::SHRINK);
            btn->addChild(lbText);
            
            if(obj != PopupSpiritSummon::E_TYPE::CLOSE)
            {
                auto item = ItemsManager::getInstance()->getInfoItems(E_ITEMS::NEW_RAID_DRAGON_BALL);
                switch(obj)
                {
                    case PopupSpiritSummon::E_TYPE::SUMMON_1:
                    {
                        if(WeaponSpiritManager::getInstance()->getSummonTicket_1() > 0)
                        {
                            item = ItemsManager::getInstance()->getInfoItems(E_ITEMS::SPIRIT_DRAW_TICKET_1);
                            nPrice = WeaponSpiritManager::getInstance()->getSummonTicket_1();
                        }
                    }break;
                    case PopupSpiritSummon::E_TYPE::SUMMON_11:
                    {
                        if(WeaponSpiritManager::getInstance()->getSummonTicket_11() > 0)
                        {
                            item = ItemsManager::getInstance()->getInfoItems(E_ITEMS::SPIRIT_DRAW_TICKET_11);
                            nPrice = WeaponSpiritManager::getInstance()->getSummonTicket_11();
                        }
                    }break;
                    case PopupSpiritSummon::E_TYPE::SUMMON_33:
                    {
                        if(WeaponSpiritManager::getInstance()->getSummonTicket_33() > 0)
                        {
                            item = ItemsManager::getInstance()->getInfoItems(E_ITEMS::SPIRIT_DRAW_TICKET_33);
                            nPrice = WeaponSpiritManager::getInstance()->getSummonTicket_33();
                        }
                    }break;
                }
                
                auto sprCurrencyIcon = Sprite::create(item->getPath());
                sprCurrencyIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                sprCurrencyIcon->setPosition(btn->getContentSize().width/2.6f, btn->getContentSize().height/4);
                btn->addChild(sprCurrencyIcon);
                
                auto lbCount = Label::createWithTTF(std::to_string((int)nPrice), GAME_FONT, 26);
                lbCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
                lbCount->setPosition(sprCurrencyIcon->getPosition() + Vec2::RIGHT * sprCurrencyIcon->getContentSize().width/2);
                lbCount->setDimensions(100, btn->getContentSize().height/2);
                lbCount->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
                lbCount->setOverflow(Label::Overflow::SHRINK);
                btn->addChild(lbCount);
            }
            else
            {
                lbText->setPosition(btn->getContentSize()/2);
            }
        }
        
        if(size % 2 == 0)
            btn->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        
        posX += width + 10;
    }
}

void PopupSpiritSummon::show()
{
    PopupBase::show();
    uiMiddle();
}

#pragma mark - utils
void PopupSpiritSummon::setCallback(const std::function<void(void)>& callback)
{
    _callback = callback;
}
void PopupSpiritSummon::setCallbackTransferShop(const std::function<void(void)>& callback)
{
    _callbackTransferShop = callback;
}

void PopupSpiritSummon::setReward(Vector<InfoItems*> vecReward)
{
    _vecReward = vecReward;
}

void PopupSpiritSummon::onClick(Ref* sender)
{
    if(WeaponSpiritManager::getInstance()->isServerDispatch())
        return;
    
    auto node = (Node*)sender;
    PopupSpiritSummon::E_TYPE eType = (PopupSpiritSummon::E_TYPE) node->getTag();
    
    if(eType == PopupSpiritSummon::E_TYPE::CLOSE)
    {
        hide();
    }
    if(eType == PopupSpiritSummon::E_TYPE::SUMMON_1)
    {
        auto obj11SummonSetting = NewRaidManager::getInstance()->getRaidSetting("spirit_draw_price_1");
        
        int nPrice = 0;
        bool isTicket = false;
        
        if(WeaponSpiritManager::getInstance()->getSummonTicket_1() >= 1)
            isTicket = true;
        
        if(obj11SummonSetting != nullptr)
            nPrice = obj11SummonSetting->getValueToInt();
        
        if(!isTicket && NewRaidManager::getInstance()->getDragonBall() < nPrice)
        {
//            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_raidshop_spiritbox_draw_pop_error_msg"));
//            popup->addButton(GAME_TEXT("t_ui_system_update_confirm"), true, nullptr);
//            popup->show();
            
            auto popup = PopupCurrencyDetail::create(E_ITEMS::NEW_RAID_DRAGON_BALL);
            popup->setCallbackTransferShop(CC_CALLBACK_0(PopupSpiritSummon::callbackTranferShop, this));
            popup->show();
            return;
        }
        if(!WeaponSpiritManager::getInstance()->isServerDispatch())
        {
            PopupLoading::show();
            WeaponSpiritManager::getInstance()->requestSpiritSummon(CC_CALLBACK_3(PopupSpiritSummon::callbackSummon, this), 1);
        }
    }
    if(eType == PopupSpiritSummon::E_TYPE::SUMMON_11)
    {
        auto obj11SummonSetting = NewRaidManager::getInstance()->getRaidSetting("spirit_draw_price_11");
        
        int nPrice = 0;
        bool isTicket = false;
        
        if(WeaponSpiritManager::getInstance()->getSummonTicket_11() >= 1)
            isTicket = true;
        
        if(obj11SummonSetting != nullptr)
            nPrice = obj11SummonSetting->getValueToInt();
        
        if(!isTicket && NewRaidManager::getInstance()->getDragonBall() < nPrice)
        {
//            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_raidshop_spiritbox_draw_pop_error_msg"));
//            popup->addButton(GAME_TEXT("t_ui_system_update_confirm"), true, nullptr);
//            popup->show();
            
            auto popup = PopupCurrencyDetail::create(E_ITEMS::NEW_RAID_DRAGON_BALL);
            popup->setCallbackTransferShop(CC_CALLBACK_0(PopupSpiritSummon::callbackTranferShop, this));
            popup->show();
            return;
        }
        if(!WeaponSpiritManager::getInstance()->isServerDispatch())
        {
            PopupLoading::show();
            WeaponSpiritManager::getInstance()->requestSpiritSummon(CC_CALLBACK_3(PopupSpiritSummon::callbackSummon, this), 11);
        }
    }
    else if(eType == PopupSpiritSummon::E_TYPE::SUMMON_33)
    {
        auto obj11SummonSetting = NewRaidManager::getInstance()->getRaidSetting("spirit_draw_price_33");
        
        int nPrice = 0;
        if(obj11SummonSetting != nullptr)
            nPrice = obj11SummonSetting->getValueToInt();
        
        if(NewRaidManager::getInstance()->getDragonBall() < nPrice)
        {
//            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_raidshop_spiritbox_draw_pop_error_msg"));
//            popup->addButton(GAME_TEXT("t_ui_system_update_confirm"), true, nullptr);
//            popup->show();
            
            auto popup = PopupCurrencyDetail::create(E_ITEMS::NEW_RAID_DRAGON_BALL);
            popup->setCallbackTransferShop(CC_CALLBACK_0(PopupSpiritSummon::callbackTranferShop, this));
            popup->show();
            return;
        }
        if(!WeaponSpiritManager::getInstance()->isServerDispatch())
        {
            PopupLoading::show();
            WeaponSpiritManager::getInstance()->requestSpiritSummon(CC_CALLBACK_3(PopupSpiritSummon::callbackSummon, this), 33);
        }
    }
}

#pragma mark -Callback
void PopupSpiritSummon::callbackSummon(bool bResult, int nResult, Vector<InfoItems*> listReward)
{
    PopupLoading::hide();
    
    if(nResult != 1)
    {
        std::string strMessageKey = "t_ui_error_30";
        
        if(nResult == 3)
            strMessageKey = "t_ui_error_31";
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT(strMessageKey));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        
        return;
    }
    
    _vecReward = listReward;
    RefreshManager::getInstance()->refreshUI(E_REFRESH::CURRENCY);
    RefreshManager::getInstance()->refreshUI(E_REFRESH::SPIRIT);
    if(_callback != nullptr)
        _callback();
    uiMiddle();
    uiBottom();
}

void PopupSpiritSummon::callbackTranferShop()
{
    if(_callbackTransferShop != nullptr)
        _callbackTransferShop();
    
    hide();
}
