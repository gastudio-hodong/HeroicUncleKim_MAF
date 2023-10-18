//
//  UILayoutTown.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/12/03.
//

#include "UILayoutTown.h"

#include "GameObject/InfoTownReward.h"

#include "GameUI/UI/UICurrency.h"

#include "GameUIPopup/Other/CurrencyDetail/PopupCurrencyDetail.h"

#include "ManagerGame/ItemsMoneyManager.h"
#include "ManagerGame/TownManager.h"
#include "ManagerGame/StoryManager.h"
#include "ManagerGame/DialogManager.h"

USING_NS_CC;

UILayoutTown* UILayoutTown::create()
{
    UILayoutTown* pRet = new(std::nothrow) UILayoutTown();
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

UILayoutTown::UILayoutTown():
_ePlace(E_PLACE::EMPTY),
_eBuildingUpgrade(E_TOWN::TOWN_NONE),

_bTownPos(false),

_uiContainerTown(nullptr),
_uiContainerUpgrade(nullptr),

_uiScrollTown(nullptr),

_uiSatelliteLast(nullptr)
{
    
}

bool UILayoutTown::init()
{
    if ( !ui::Layout::init() )
    {
        return false;
    }
    
    
    //
    SoundManager::SoundPreload(SOUND_ROCKET_START);
    
    //
    initVar();
    initUI();
    
    return true;
}


void UILayoutTown::onEnter()
{
    ui::Layout::onEnter();
    
    uiContainer();
    uiTown();
    uiUpgrade();
    
    // story
    StoryManager::getInstance()->showStory((int)E_COMIC::TOWN_JOIN);
    
    //
    PopupLoading::show();
    TownManager::getInstance()->requestInfo(CC_CALLBACK_1(UILayoutTown::callbackInfo, this));
}

#pragma mark - init
void UILayoutTown::initVar()
{
    //
    _ePlace = E_PLACE::DG_NORMAL;
}

void UILayoutTown::initUI()
{
    // size
    Size size = Size(750, 1150);

    setContentSize(size);
    setAnchorPoint(Vec2::ZERO);
}

#pragma mark - ui
void UILayoutTown::uiContainer()
{
    Size size = getContentSize();
    size.width = 728;
    
    //
    _uiContainerUpgrade = ui::Layout::create();
    _uiContainerUpgrade->setContentSize( Size(size.width, 505) );
    _uiContainerUpgrade->setIgnoreAnchorPointForPosition(false);
    _uiContainerUpgrade->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _uiContainerUpgrade->setPosition( Vec2(getContentSize().width / 2, 0) );
    _uiContainerUpgrade->setClippingEnabled(true);
    addChild(_uiContainerUpgrade);
    
    _uiContainerTown = ui::Layout::create();
    _uiContainerTown->setContentSize( Size(size.width, size.height - _uiContainerUpgrade->getContentSize().height) );
    _uiContainerTown->setIgnoreAnchorPointForPosition(false);
    _uiContainerTown->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _uiContainerTown->setPosition( Vec2(getContentSize().width / 2, getContentSize().height) );
    addChild(_uiContainerTown);
}

void UILayoutTown::uiTown()
{
    _uiContainerTown->removeAllChildren();
    
    auto layout = _uiContainerTown;
    auto size = layout->getContentSize();
    
    //
    _uiScrollTown = ui::ScrollView::create();
    _uiScrollTown->setContentSize(size);
    _uiScrollTown->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _uiScrollTown->setPosition(Vec2(size.width / 2, 0));
    _uiScrollTown->setDirection(ui::ScrollView::Direction::VERTICAL);
    _uiScrollTown->setScrollBarPositionFromCorner(Vec2(4, 4));
    _uiScrollTown->setScrollBarColor(Color3B::YELLOW);
    _uiScrollTown->setBounceEnabled(true);
    _uiScrollTown->setInertiaScrollEnabled(false);
    _uiScrollTown->addEventListener([&](Ref* ref, ui::ScrollView::EventType e) {
        
        auto pos = _uiScrollTown->getInnerContainerPosition();
        if ( pos.y > 0 )
        {
            pos.y = 0;
        }
        _uiScrollTown->setInnerContainerPosition(pos);
    });
    layout->addChild(_uiScrollTown);
    {
        auto uiLayoutTownBG = ui::Layout::create();
        uiLayoutTownBG->setContentSize(size);
        uiLayoutTownBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        uiLayoutTownBG->setPosition(Vec2(size.width / 2, 0));
        uiLayoutTownBG->setName("UI_CONTAINER_TOWN_BG");
        _uiScrollTown->addChild(uiLayoutTownBG);
        
        auto uiLayoutTownLand = ui::Layout::create();
        uiLayoutTownLand->setContentSize(Size(1470, size.height));
        uiLayoutTownLand->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        uiLayoutTownLand->setPosition(Vec2(0, 0));
        uiLayoutTownLand->setName("UI_CONTAINER_TOWN_GROUND");
        _uiScrollTown->addChild(uiLayoutTownLand);
        
        auto uiContainerAniBack = ui::Layout::create();
        uiContainerAniBack->setContentSize(Size(1470, size.height));
        uiContainerAniBack->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        uiContainerAniBack->setPosition(Vec2(0, 0));
        uiContainerAniBack->setName("UI_CONTAINER_ANI_BACK");
        _uiScrollTown->addChild(uiContainerAniBack);
        
        auto uiLayoutBuilding = ui::Layout::create();
        uiLayoutBuilding->setContentSize(Size(1470, size.height));
        uiLayoutBuilding->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        uiLayoutBuilding->setPosition(Vec2(0, 0));
        uiLayoutBuilding->setName("UI_CONTAINER_BUILDING");
        _uiScrollTown->addChild(uiLayoutBuilding);
        
        auto uiLayoutSatelliteLaunch = ui::Layout::create();
        uiLayoutSatelliteLaunch->setContentSize(Size(1470, size.height));
        uiLayoutSatelliteLaunch->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        uiLayoutSatelliteLaunch->setPosition(Vec2(0, 0));
        uiLayoutSatelliteLaunch->setName("UI_CONTAINER_SATELLITE_LAUNCH");
        _uiScrollTown->addChild(uiLayoutSatelliteLaunch);
        
        auto uiLayoutSatellite = ui::Layout::create();
        uiLayoutSatellite->setContentSize(size);
        uiLayoutSatellite->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        uiLayoutSatellite->setPosition(Vec2(0, 0));
        uiLayoutSatellite->setName("UI_CONTAINER_SATELLITE");
        _uiScrollTown->addChild(uiLayoutSatellite);
        
        auto uiContainerAniFront = ui::Layout::create();
        uiContainerAniFront->setContentSize(Size(1470, size.height));
        uiContainerAniFront->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        uiContainerAniFront->setPosition(Vec2(0, 0));
        uiContainerAniFront->setName("UI_CONTAINER_ANI_FRONT");
        _uiScrollTown->addChild(uiContainerAniFront);
    }
    
    
    auto uButtonArrowL = ui::Button::create("Assets/ui/tower/tower_arrow_left.png");
    uButtonArrowL->setPosition(Vec2(50, 50));
    uButtonArrowL->addClickEventListener(CC_CALLBACK_1(UILayoutTown::onClickTownArrow, this));
    uButtonArrowL->setName("UI_ARROW_L");
    uButtonArrowL->setTag(1);
    layout->addChild(uButtonArrowL);
    
    auto uButtonArrowR = ui::Button::create("Assets/ui/tower/tower_arrow_right.png");
    uButtonArrowR->setPosition(Vec2(size.width - 50, 50));
    uButtonArrowR->addClickEventListener(CC_CALLBACK_1(UILayoutTown::onClickTownArrow, this));
    uButtonArrowR->setName("UI_ARROW_R");
    uButtonArrowR->setTag(2);
    layout->addChild(uButtonArrowR);
    
    
    // draw
    drawTownBG();
    drawTownGround();
    drawTownAniBack();
    drawTownAniFront();
    drawBuilding();
    drawSatelliteLaunch();
    drawSatellite();
    
    drawTown();
    
    //
    onMoveBuilding();
}

void UILayoutTown::uiUpgrade()
{
    _uiContainerUpgrade->removeAllChildren();
    
    auto layout = _uiContainerUpgrade;
    
    auto size = layout->getContentSize();
    
    // 봉파트, 봉빌라, 봉멘션, 봉빌딩
    {
        E_TOWN listBuilding[] = {E_TOWN::BUILDING_2, E_TOWN::BUILDING_3, E_TOWN::BUILDING_4, E_TOWN::BUILDING_5};
        
        int count = sizeof(listBuilding) / sizeof(E_TOWN);
        double padding = 5;
        
        double width = (size.width - padding * (count - 1)) / count;
        double height = 230;
        
        double posX = 0;
        double posY = layout->getContentSize().height;
        for ( int i = 0; i < sizeof(listBuilding) / sizeof(E_TOWN); i++ )
        {
            E_TOWN eType = listBuilding[i];
            
            auto uiLayout = ui::Layout::create();
            uiLayout->setContentSize(Size(width, height));
            uiLayout->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
            uiLayout->setPosition(Vec2(posX, posY));
            uiLayout->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
            uiLayout->setBackGroundColor(Color3B(29, 41, 51));
            uiLayout->setLayoutType(ui::Layout::Type::VERTICAL);
            uiLayout->setTag((int)eType);
            layout->addChild(uiLayout);
            
            posX += uiLayout->getContentSize().width + padding;
        }
    }
    
    // 봉식 타워
    {
        double width = size.width;
        double height = 135;
        
        double posX = 0;
        double posY = layout->getContentSize().height - 235;
        
        auto uiLayout = ui::Layout::create();
        uiLayout->setContentSize(Size(width, height));
        uiLayout->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        uiLayout->setPosition(Vec2(posX, posY));
        uiLayout->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
        uiLayout->setBackGroundColor(Color3B(29, 41, 51));
        uiLayout->setTag((int)E_TOWN::BUILDING_1);
        layout->addChild(uiLayout);
    }
    
    // 인공 위성
    {
        double width = size.width;
        double height = 135;
        
        double posX = 0;
        double posY = layout->getContentSize().height - 235 - 135;
        
        auto uiLayout = ui::Layout::create();
        uiLayout->setContentSize(Size(width, height));
        uiLayout->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        uiLayout->setPosition(Vec2(posX, posY));
        uiLayout->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
        uiLayout->setBackGroundColor(Color3B(29, 41, 51));
        uiLayout->setTag((int)E_TOWN::SATELLITE);
        layout->addChild(uiLayout);
    }
    
    // draw
    drawUpgrade();
}


#pragma mark - draw
void UILayoutTown::drawTownBG()
{
    auto uiContainer = _uiScrollTown->getInnerContainer()->getChildByName<ui::Layout*>("UI_CONTAINER_TOWN_BG");
    uiContainer->removeAllChildren();
    
    Size size = uiContainer->getContentSize();
    
    double posX = uiContainer->getContentSize().width / 2;
    double posY = 0;
    double scale = size.width / 182;
    
    //
    auto uiBG = ui::ImageView::create("Assets/ui/tower/tower_bg_1_1.png");
    uiBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    uiBG->setPosition(Vec2(posX, posY));
    uiBG->setScale(scale);
    ((ui::Scale9Sprite*)uiBG->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
    uiContainer->addChild(uiBG);
    {
        auto uiImgLight1 = Sprite::create("Assets/ui/tower/tower_bg_1_2.png");
        uiImgLight1->setPosition(Vec2(uiBG->getContentSize().width / 2, uiBG->getContentSize().height / 2));
        uiImgLight1->setOpacity(0);
        uiBG->addChild(uiImgLight1);
        {
            auto action1 = FadeIn::create(1.0);
            auto action2 = FadeOut::create(1.0);
            auto action3 = DelayTime::create(2.0);
            auto seq = Sequence::create(action1, action2, action3, NULL);
            uiImgLight1->runAction(RepeatForever::create(seq));
        }

        auto uiImgLight2 = Sprite::create("Assets/ui/tower/tower_bg_1_3.png");
        uiImgLight2->setPosition(Vec2(uiBG->getContentSize().width / 2, uiBG->getContentSize().height / 2));
        uiImgLight2->setOpacity(0);
        uiBG->addChild(uiImgLight2);
        {
            auto action1 = DelayTime::create(2.0);
            auto action2 = FadeIn::create(1.0);
            auto action3 = FadeOut::create(1.0);
            auto seq = Sequence::create(action1, action2, action3, NULL);
            uiImgLight2->runAction(RepeatForever::create(seq));
        }
    }
    
    posY += uiBG->getBoundingBox().size.height;
    
    //
    for ( int i = 0; i < 3; i++ )
    {
        auto uiLoopBG = ui::ImageView::create("Assets/ui/tower/tower_bg_2_1.png");
        uiLoopBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        uiLoopBG->setPosition(Vec2(posX, posY));
        uiLoopBG->setScale(scale);
        ((ui::Scale9Sprite*)uiLoopBG->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
        uiContainer->addChild(uiLoopBG);
        {
            auto uiImgLight1 = Sprite::create("Assets/ui/tower/tower_bg_2_2.png");
            uiImgLight1->setPosition(Vec2(uiLoopBG->getContentSize().width / 2, uiLoopBG->getContentSize().height / 2));
            uiImgLight1->setOpacity(0);
            uiLoopBG->addChild(uiImgLight1);
            {
                auto action1 = FadeIn::create(1.0);
                auto action2 = FadeOut::create(1.0);
                auto action3 = DelayTime::create(2.0);
                auto seq = Sequence::create(action1, action2, action3, NULL);
                uiImgLight1->runAction(RepeatForever::create(seq));
            }

            auto uiImgLight2 = Sprite::create("Assets/ui/tower/tower_bg_2_3.png");
            uiImgLight2->setPosition(Vec2(uiLoopBG->getContentSize().width / 2, uiLoopBG->getContentSize().height / 2));
            uiImgLight2->setOpacity(0);
            uiLoopBG->addChild(uiImgLight2);
            {
                auto action1 = DelayTime::create(2.0);
                auto action2 = FadeIn::create(1.0);
                auto action3 = FadeOut::create(1.0);
                auto seq = Sequence::create(action1, action2, action3, NULL);
                uiImgLight2->runAction(RepeatForever::create(seq));
            }
        }
        
        posY += uiLoopBG->getBoundingBox().size.height;
    }
}

void UILayoutTown::drawTownGround()
{
    auto uiContainer = _uiScrollTown->getInnerContainer()->getChildByName<ui::Layout*>("UI_CONTAINER_TOWN_GROUND");
    uiContainer->removeAllChildren();
    
    Size size = uiContainer->getContentSize();
    
    auto uiTownBG = ui::ImageView::create("Assets/ui/tower/town_bg.png");
    uiTownBG->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    uiTownBG->setPosition(Vec2(0, 0));
    uiTownBG->setScale(2);
    ((ui::Scale9Sprite*)uiTownBG->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
    uiContainer->addChild(uiTownBG);
    
    //
    auto uiCloud1 = ui::ImageView::create("Assets/ui/tower/tower_obj_0_1.png");
    uiCloud1->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    uiCloud1->setPosition(Vec2(-150, uiTownBG->getContentSize().height - 47));
    uiCloud1->setLocalZOrder(-1);
    uiCloud1->setScale(2);
    ((ui::Scale9Sprite*)uiCloud1->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
    uiTownBG->addChild(uiCloud1);
    {
        auto action1_1 = MoveBy::create(20.0, Vec2(20, 0));
        auto action1_2 = MoveBy::create(10.0, Vec2(-10, 0));
        auto action1_3 = MoveBy::create(20.0, Vec2(20, 0));
        auto action1_4 = MoveBy::create(30.0, Vec2(-30, 0));
        auto seq1 = Sequence::create(action1_1, action1_2, action1_3, action1_4, NULL);
        
        auto action2_1 = ScaleBy::create(50, 1.01);
        auto action2_2 = ScaleBy::create(100, 0.99);
        auto action2_3 = ScaleBy::create(50, 1.0);
        auto seq2 = Sequence::create(action2_1, action2_2, action2_3, NULL);
        
        uiCloud1->runAction(RepeatForever::create(seq1));
        uiCloud1->runAction(RepeatForever::create(seq2));
    }
    
    //
    auto uiCloud2 = ui::ImageView::create("Assets/ui/tower/tower_obj_0_2.png");
    uiCloud2->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    uiCloud2->setPosition(Vec2(uiTownBG->getContentSize().width / 2, uiTownBG->getContentSize().height - 47));
    uiCloud2->setLocalZOrder(-1);
    uiCloud2->setScale(2);
    ((ui::Scale9Sprite*)uiCloud2->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
    uiTownBG->addChild(uiCloud2);
    {
        auto action2_1 = ScaleBy::create(10, 1.1);
        auto action2_2 = ScaleBy::create(20, 0.9);
        auto action2_3 = ScaleBy::create(10, 1.0);
        auto seq2 = Sequence::create(action2_1, action2_2, action2_3, NULL);
        
        uiCloud2->runAction(RepeatForever::create(seq2));
    }
    
    //
    auto uiCloud3 = ui::ImageView::create("Assets/ui/tower/tower_obj_0_3.png");
    uiCloud3->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    uiCloud3->setPosition(Vec2(uiTownBG->getContentSize().width, uiTownBG->getContentSize().height - 47));
    uiCloud3->setLocalZOrder(-1);
    uiCloud3->setScale(2);
    ((ui::Scale9Sprite*)uiCloud3->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
    uiTownBG->addChild(uiCloud3);
    {
        auto action1_1 = MoveBy::create(25.0, Vec2(5, 0));
        auto action1_2 = MoveBy::create(22.5, Vec2(-2, 0));
        auto action1_3 = MoveBy::create(25.0, Vec2(5, 0));
        auto action1_4 = MoveBy::create(27.5, Vec2(-7, 0));
        auto seq1 = Sequence::create(action1_1, action1_2, action1_3, action1_4, NULL);
        
        auto action2_1 = ScaleBy::create(5, 1.1);
        auto action2_2 = ScaleBy::create(10, 0.9);
        auto action2_3 = ScaleBy::create(5, 1.0);
        auto seq2 = Sequence::create(action2_1, action2_2, action2_3, NULL);
        
        uiCloud3->runAction(RepeatForever::create(seq1));
        uiCloud3->runAction(RepeatForever::create(seq2));
    }
    
    // 애니메이션 : 물줄기
    {
        auto animation = Animation::create();
        animation->setDelayPerUnit(0.3);
        for ( int i = 1; i <= 4; i++ )
        {
            auto spriteAni = Sprite::create(MafUtils::format("Assets/ui/tower/town_bg_1_%d.png",i).c_str());
            spriteAni->getTexture()->setTexParameters(texParams);
            animation->addSpriteFrame(spriteAni->getSpriteFrame());
        }
        
        auto uiAni = ui::ImageView::create("Assets/ui/tower/town_bg_1_1.png");
        uiAni->setAnchorPoint(Vec2::ZERO);
        uiAni->setPosition(Vec2(0, 0));
        uiAni->getVirtualRenderer()->stopAllActions();
        uiAni->getVirtualRenderer()->runAction(RepeatForever::create(Animate::create(animation)));
        uiTownBG->addChild(uiAni, 1000);
    }
    
    // 애니메이션 : 고래
    {
        auto animation = Animation::create();
        animation->setDelayPerUnit(0.2);
        for ( int i = 1; i <= 9; i++ )
        {
            auto spriteAni = Sprite::create(MafUtils::format("Assets/ui/tower/tower_whale_1_%d.png",i).c_str());
            spriteAni->getTexture()->setTexParameters(texParams);
            animation->addSpriteFrame(spriteAni->getSpriteFrame());
        }
        
        auto uiAni = ui::ImageView::create("Assets/ui/tower/tower_whale_1_1.png");
        uiAni->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
        uiAni->setPosition(Vec2(735*2, 278*2));
        uiAni->setScale(2);
        uiAni->setOpacity(0);
        uiContainer->addChild(uiAni, 1000);
        
        auto action1 = DelayTime::create(random(5, 10));
        auto action2 = FadeIn::create(0);
        auto action3 = Animate::create(animation);
        auto action4 = FadeOut::create(0);
        auto seq = Sequence::create(action1, action2, action3, action4, NULL);
        uiAni->getVirtualRenderer()->stopAllActions();
        uiAni->getVirtualRenderer()->runAction(RepeatForever::create(seq));
    }
}

void UILayoutTown::drawBuilding()
{
    auto uiContainer = _uiScrollTown->getInnerContainer()->getChildByName<ui::Layout*>("UI_CONTAINER_BUILDING");
    uiContainer->removeAllChildren();
    
    Size size = uiContainer->getContentSize();
    
    // 빌딩 위치
    Vec2 listBuildingPos[] = {
        Vec2(188, 81),
        Vec2(40, 95),
        Vec2(110, 143),
        Vec2(260, 138),
        Vec2(320, 95),
    };
    
    // 빌딩 층별 간격
    double listBuildingFloorSpacing[] = {
        164,
        80,
        84,
        136,
        108,
    };
    
    
    std::vector<E_TOWN> listTown;
    listTown.push_back(E_TOWN::BUILDING_1);
    listTown.push_back(E_TOWN::BUILDING_2);
    listTown.push_back(E_TOWN::BUILDING_3);
    listTown.push_back(E_TOWN::BUILDING_4);
    listTown.push_back(E_TOWN::BUILDING_5);
    
    //
    for ( int i = 0; i < listTown.size(); i++ )
    {
        E_TOWN eType = listTown.at(i);
        
        std::string pathFloor = TownManager::getInstance()->getBuildingPathFloor(eType);
        std::string pathTop = TownManager::getInstance()->getBuildingPathFloorTop(eType);
        std::string pathBottom = TownManager::getInstance()->getBuildingPathFloorBottom(eType);
        
        int level = TownManager::getInstance()->getTownLevel(eType);
        int levelMax = TownManager::getInstance()->getTownLevelMax(eType);
        if ( level > levelMax )
        {
            level = levelMax;
        }
        
        int floor = level / 20;
        
        double posX = listBuildingPos[i].x * 2;
        double posY = listBuildingPos[i].y * 2;
        double spacing = listBuildingFloorSpacing[i];
        int zorder = 1000 - listBuildingPos[i].y;
        
        //
        auto uiImgBottom = ui::ImageView::create(pathBottom);
        uiImgBottom->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        uiImgBottom->setPosition(Vec2(posX, posY));
        uiImgBottom->setScale(2);
        uiImgBottom->setLocalZOrder(zorder);
        ((ui::Scale9Sprite*)uiImgBottom->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
        uiContainer->addChild(uiImgBottom);
        
        posY += uiImgBottom->getBoundingBox().size.height - spacing;
        
        //
        for ( int j = 0; j < floor; j++ )
        {
            auto uiImgFloor = ui::ImageView::create(pathFloor);
            uiImgFloor->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
            uiImgFloor->setPosition(Vec2(posX, posY));
            uiImgFloor->setScale(2);
            uiImgFloor->setLocalZOrder(zorder);
            ((ui::Scale9Sprite*)uiImgFloor->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
            uiContainer->addChild(uiImgFloor);
            
            posY += uiImgFloor->getBoundingBox().size.height - spacing;
        }
        
        //
        auto uiImgTop = ui::ImageView::create(pathTop);
        uiImgTop->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        uiImgTop->setPosition(Vec2(posX, posY));
        uiImgTop->setScale(2);
        uiImgTop->setLocalZOrder(zorder);
        ((ui::Scale9Sprite*)uiImgTop->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
        uiContainer->addChild(uiImgTop);
        
        posY += uiImgTop->getBoundingBox().size.height;
        
        // 봉식 타워 보상
        if ( eType == E_TOWN::BUILDING_1 )
        {
            bool isReward = drawBuildingReward(posX, posY);
            if ( isReward == true )
            {
                posY += 300;
            }
        }
        
        //
        if ( size.height < posY)
        {
            size.height = posY;
        }
    }
    
    //
    uiContainer->setContentSize(size);
}

bool UILayoutTown::drawBuildingReward(double posX, double posY)
{
    auto uiContainer = _uiScrollTown->getInnerContainer()->getChildByName<ui::Layout*>("UI_CONTAINER_BUILDING");
    
    auto objTownReward = TownManager::getInstance()->getObjectTownRewardLast(1);
    if ( objTownReward == nullptr )
    {
        return false;
    }
    
    auto objItem = objTownReward->getItems();
    if ( objItem == nullptr )
    {
        return false;
    }
    
    auto uiLayoutReward = ui::Layout::create();
    uiLayoutReward->setContentSize(Size(164, 280));
    uiLayoutReward->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    uiLayoutReward->setPosition(Vec2(posX, posY));
    uiLayoutReward->setTouchEnabled(true);
    uiLayoutReward->addClickEventListener(CC_CALLBACK_1(UILayoutTown::onClickBuildingReward, this));
    uiContainer->addChild(uiLayoutReward, 1000);
    {
        auto uiBG = ui::ImageView::create("Assets/ui/tower/tower_gift_01.png");
        uiBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        uiBG->setPosition(Vec2(uiLayoutReward->getContentSize().width / 2, 0));
        uiBG->setScale(4);
        ((ui::Scale9Sprite*)uiBG->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
        uiLayoutReward->addChild(uiBG);
        
        // 아이템
        auto uiItem = ui::ImageView::create(objItem->getPath());
        uiItem->setPosition(Vec2(uiLayoutReward->getContentSize().width / 2 - 1, 70));
        uiLayoutReward->addChild(uiItem);
        {
            uiItem->setScale(58 / uiItem->getContentSize().width);
        }
        
        auto uiCount = ui::CText::create(objItem->getCount(), GAME_FONT, 16);
        uiCount->setPosition(Vec2(uiLayoutReward->getContentSize().width / 2 - 1, 45));
        uiCount->enableOutline(Color4B::BLACK, 2);
        uiLayoutReward->addChild(uiCount);
        
        // 획득 층수
        auto uiFloor = ui::CText::create(GAME_TEXTFORMAT("t_ui_town_floor", objTownReward->getCondition()), GAME_FONT, 19);
        uiFloor->setPosition(Vec2(uiLayoutReward->getContentSize().width / 2 - 1, 12));
        uiFloor->enableOutline(Color4B::BLACK, 2);
        uiLayoutReward->addChild(uiFloor);
    }
    
    // action
    auto action1 = MoveBy::create(1.0, Vec2(0, 10));
    auto action2 = MoveBy::create(1.0, Vec2(0, -10));
    auto seq = Sequence::create(action1, action2, NULL);
    uiLayoutReward->runAction(RepeatForever::create(seq));
    
    return true;
}

void UILayoutTown::drawSatelliteLaunch()
{
    auto uiContainer = _uiScrollTown->getInnerContainer()->getChildByName<ui::Layout*>("UI_CONTAINER_SATELLITE_LAUNCH");
    uiContainer->removeAllChildren();
    
    Size size = uiContainer->getContentSize();
    
    //
    auto uiTownBG = ui::ImageView::create("Assets/ui/tower/satellite_launch_pedestal.png");
    uiTownBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    uiTownBG->setPosition(Vec2(570*2, 80*2));
    uiTownBG->setScale(2);
    ((ui::Scale9Sprite*)uiTownBG->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
    uiContainer->addChild(uiTownBG);
    
    //
    int stepSatellite = TownManager::getInstance()->getSatelliteStep();
    
    int level = TownManager::getInstance()->getTownLevel(E_TOWN::SATELLITE);
    int levelMax = TownManager::getInstance()->getTownLevelMax(E_TOWN::SATELLITE);
    int levelStep = level - stepSatellite * 100;
    if ( level >= levelMax )
    {
        auto uiTownBG = ui::ImageView::create("Assets/ui/tower/satellite_launch_pedestal_continue.png");
        uiTownBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        uiTownBG->setPosition(Vec2(585*2, 105*2));
        uiTownBG->setScale(2);
        ((ui::Scale9Sprite*)uiTownBG->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
        uiContainer->addChild(uiTownBG);
    }
    else if ( levelStep >= 1 )
    {
        int stepShuttle = levelStep / 10;
        
        auto uiImgShuttle = ui::ImageView::create(MafUtils::format("Assets/ui/tower/satellite_%02d.png", stepShuttle));
        uiImgShuttle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        uiImgShuttle->setPosition(Vec2(570*2, 80*2));
        uiImgShuttle->setScale(2);
        ((ui::Scale9Sprite*)uiImgShuttle->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
        uiContainer->addChild(uiImgShuttle);
        
        if ( stepShuttle < 10 )
        {
            // 애니메이션 : 1
            {
                auto animation = Animation::create();
                animation->setDelayPerUnit(0.1);
                for ( int i = 1; i <= 4; i++ )
                {
                    auto spriteAni = Sprite::create(MafUtils::format("Assets/ui/tower/town_mole_1_%d.png",i).c_str());
                    spriteAni->getTexture()->setTexParameters(texParams);
                    animation->addSpriteFrame(spriteAni->getSpriteFrame());
                }
                
                auto uiAni = ui::ImageView::create("Assets/ui/tower/town_mole_1_1.png");
                uiAni->setPosition(Vec2(522*2, 153*2));
                uiAni->setScale(2);
                uiAni->getVirtualRenderer()->stopAllActions();
                uiAni->getVirtualRenderer()->runAction(RepeatForever::create(Animate::create(animation)));
                uiContainer->addChild(uiAni, 1000);
            }
            
            // 애니메이션 : 2
            if ( stepShuttle >= 2 )
            {
                auto animation = Animation::create();
                animation->setDelayPerUnit(0.1);
                for ( int i = 1; i <= 4; i++ )
                {
                    auto spriteAni = Sprite::create(MafUtils::format("Assets/ui/tower/town_mole_2_%d.png",i).c_str());
                    spriteAni->getTexture()->setTexParameters(texParams);
                    animation->addSpriteFrame(spriteAni->getSpriteFrame());
                }
                
                auto uiAni = ui::ImageView::create("Assets/ui/tower/town_mole_2_1.png");
                uiAni->setPosition(Vec2(596*2, 105*2));
                uiAni->setScale(2);
                uiAni->getVirtualRenderer()->stopAllActions();
                uiAni->getVirtualRenderer()->runAction(RepeatForever::create(Animate::create(animation)));
                uiContainer->addChild(uiAni, 1000);
            }
            
            // 애니메이션 : 3
            if ( stepShuttle >= 8 )
            {
                auto animation = Animation::create();
                animation->setDelayPerUnit(0.1);
                for ( int i = 1; i <= 4; i++ )
                {
                    auto spriteAni = Sprite::create(MafUtils::format("Assets/ui/tower/town_mole_3_%d.png",i).c_str());
                    spriteAni->getTexture()->setTexParameters(texParams);
                    animation->addSpriteFrame(spriteAni->getSpriteFrame());
                }
                
                auto uiAni = ui::ImageView::create("Assets/ui/tower/town_mole_3_1.png");
                uiAni->setPosition(Vec2(574*2, 200*2));
                uiAni->setScale(2);
                uiAni->getVirtualRenderer()->stopAllActions();
                uiAni->getVirtualRenderer()->runAction(RepeatForever::create(Animate::create(animation)));
                uiContainer->addChild(uiAni, 1000);
            }
            
            // 애니메이션 : 4-1
            if ( stepShuttle >= 4 )
            {
                auto animation = Animation::create();
                animation->setDelayPerUnit(0.1);
                for ( int i = 1; i <= 4; i++ )
                {
                    auto spriteAni = Sprite::create(MafUtils::format("Assets/ui/tower/town_mole_4_%d.png",i).c_str());
                    spriteAni->getTexture()->setTexParameters(texParams);
                    animation->addSpriteFrame(spriteAni->getSpriteFrame());
                }
                
                auto uiAni = ui::ImageView::create("Assets/ui/tower/town_mole_4_1.png");
                uiAni->setPosition(Vec2(630*2, 120*2));
                uiAni->setScale(2);
                uiAni->getVirtualRenderer()->stopAllActions();
                uiAni->getVirtualRenderer()->runAction(RepeatForever::create(Animate::create(animation)));
                uiContainer->addChild(uiAni, 1000);
            }
            
            // 애니메이션 : 4-2
            if ( stepShuttle >= 6 )
            {
                auto animation = Animation::create();
                animation->setDelayPerUnit(0.1);
                for ( int i = 1; i <= 4; i++ )
                {
                    auto spriteAni = Sprite::create(MafUtils::format("Assets/ui/tower/town_mole_4_%d.png",i).c_str());
                    spriteAni->getTexture()->setTexParameters(texParams);
                    animation->addSpriteFrame(spriteAni->getSpriteFrame());
                }
                
                auto uiAni = ui::ImageView::create("Assets/ui/tower/town_mole_4_1.png");
                uiAni->setPosition(Vec2(520*2, 242*2));
                uiAni->setScale(2);
                uiAni->setFlippedX(true);
                uiAni->getVirtualRenderer()->stopAllActions();
                uiAni->getVirtualRenderer()->runAction(RepeatForever::create(Animate::create(animation)));
                uiContainer->addChild(uiAni, 1000);
            }
            
            // 애니메이션 : 5
            if ( stepShuttle >= 7 )
            {
                auto animation = Animation::create();
                animation->setDelayPerUnit(0.1);
                for ( int i = 1; i <= 4; i++ )
                {
                    auto spriteAni = Sprite::create(MafUtils::format("Assets/ui/tower/town_mole_5_%d.png",i).c_str());
                    spriteAni->getTexture()->setTexParameters(texParams);
                    animation->addSpriteFrame(spriteAni->getSpriteFrame());
                }
                
                auto uiAni = ui::ImageView::create("Assets/ui/tower/town_mole_5_1.png");
                uiAni->setPosition(Vec2(610*2, 217*2));
                uiAni->setScale(2);
                uiAni->getVirtualRenderer()->stopAllActions();
                uiAni->getVirtualRenderer()->runAction(RepeatForever::create(Animate::create(animation)));
                uiContainer->addChild(uiAni, 1000);
            }
        }
        else
        {
            auto uiBtnPush = ui::Button::create("Assets/ui/tower/satellite_push.png", "Assets/ui/tower/satellite_push_down.png");
            uiBtnPush->setPosition(Vec2(550*2, 90*2));
            uiBtnPush->setScale(2);
            uiBtnPush->addClickEventListener(CC_CALLBACK_1(UILayoutTown::onClickSatelliteReward, this));
            ((ui::Scale9Sprite*)uiBtnPush->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
            uiContainer->addChild(uiBtnPush);
            
            // 애니메이션 : 6
            {
                auto animation = Animation::create();
                animation->setDelayPerUnit(0.1);
                for ( int i = 1; i <= 4; i++ )
                {
                    auto spriteAni = Sprite::create(MafUtils::format("Assets/ui/tower/town_mole_6_%d.png",i).c_str());
                    spriteAni->getTexture()->setTexParameters(texParams);
                    animation->addSpriteFrame(spriteAni->getSpriteFrame());
                }
                
                auto uiAni = ui::ImageView::create("Assets/ui/tower/town_mole_6_1.png");
                uiAni->setPosition(Vec2(590*2, 104*2));
                uiAni->setScale(2);
                uiAni->getVirtualRenderer()->stopAllActions();
                uiAni->getVirtualRenderer()->runAction(RepeatForever::create(Animate::create(animation)));
                uiContainer->addChild(uiAni, 1000);
            }
            
            // 애니메이션 : 7
            {
                auto animation = Animation::create();
                animation->setDelayPerUnit(0.1);
                for ( int i = 1; i <= 4; i++ )
                {
                    auto spriteAni = Sprite::create(MafUtils::format("Assets/ui/tower/town_mole_7_%d.png",i).c_str());
                    spriteAni->getTexture()->setTexParameters(texParams);
                    animation->addSpriteFrame(spriteAni->getSpriteFrame());
                }
                
                auto uiAni = ui::ImageView::create("Assets/ui/tower/town_mole_7_1.png");
                uiAni->setPosition(Vec2(497*2, 89*2));
                uiAni->setScale(2);
                uiAni->getVirtualRenderer()->stopAllActions();
                uiAni->getVirtualRenderer()->runAction(RepeatForever::create(Animate::create(animation)));
                uiContainer->addChild(uiAni, 1000);
            }
            
            //
            {
                auto uiAni = ui::ImageView::create("Assets/ui/tower/push_finger.png");
                uiAni->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
                uiAni->setPosition(Vec2(535*2, 100*2));
                uiAni->setScale(2);
                uiContainer->addChild(uiAni, 1000);
                
                // action
                auto action1 = MoveBy::create(0.5, Vec2(0, 15));
                auto action2 = MoveBy::create(0.5, Vec2(0, -15));
                auto seq = Sequence::create(action1, action2, NULL);
                uiAni->runAction(RepeatForever::create(seq));
            }
        }
        
        // progress
        auto uiLevelBG = ui::ImageView::create("Assets/ui/tower/satellite_launch_ui_gauge_bg.png");
        uiLevelBG->setPosition(Vec2(570*2, 40*2));
        uiLevelBG->setScale(2);
        ((ui::Scale9Sprite*)uiLevelBG->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
        uiContainer->addChild(uiLevelBG);
        {
            int stepSatellite = TownManager::getInstance()->getSatelliteStep();
            int levelBarMin = stepSatellite * 100 + 1;
            int levelBarMax = levelBarMin + 99;
            double percent = level - levelBarMin + 1;
            
            auto uiImgText = ui::ImageView::create("Assets/ui/tower/satellite_launch_ui_level.png");
            uiImgText->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
            uiImgText->setPosition(Vec2(0, uiLevelBG->getContentSize().height));
            ((ui::Scale9Sprite*)uiImgText->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
            uiLevelBG->addChild(uiImgText);
            
            auto uiLevelBar = ui::LoadingBar::create("Assets/ui/tower/satellite_launch_ui_gauge.png");
            uiLevelBar->setPosition(Vec2(uiLevelBG->getContentSize().width / 2, uiLevelBG->getContentSize().height / 2));
            uiLevelBar->setPercent(percent);
            ((ui::Scale9Sprite*)uiLevelBar->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
            uiLevelBG->addChild(uiLevelBar);
            
            auto uiTextLevelMin = ui::CText::create(MafUtils::format("Lv %d", levelBarMin), GAME_FONT, 14);
            uiTextLevelMin->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
            uiTextLevelMin->setPosition(Vec2(uiLevelBG->getPositionX() - uiLevelBG->getBoundingBox().size.width / 2, uiLevelBG->getPositionY() - uiLevelBG->getBoundingBox().size.height / 2 + 5));
            uiTextLevelMin->enableOutline(Color4B::BLACK, 2);
            uiContainer->addChild(uiTextLevelMin);
            
            auto uiTextLevelMax = ui::CText::create(MafUtils::format("Lv %d", levelBarMax), GAME_FONT, 14);
            uiTextLevelMax->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
            uiTextLevelMax->setPosition(Vec2(uiLevelBG->getPositionX() + uiLevelBG->getBoundingBox().size.width / 2, uiLevelBG->getPositionY() - uiLevelBG->getBoundingBox().size.height / 2 + 5));
            uiTextLevelMax->enableOutline(Color4B::BLACK, 2);
            uiContainer->addChild(uiTextLevelMax);
            
            //
            drawSatelliteReward(uiLevelBG->getPositionX() + uiLevelBG->getBoundingBox().size.width / 2, uiLevelBG->getPositionY() + uiLevelBG->getBoundingBox().size.height / 2);
        }
    }
    
    
    
}

void UILayoutTown::drawSatellite()
{
    auto uiContainerBuilding = _uiScrollTown->getInnerContainer()->getChildByName<ui::Layout*>("UI_CONTAINER_BUILDING");
    auto uiContainer = _uiScrollTown->getInnerContainer()->getChildByName<ui::Layout*>("UI_CONTAINER_SATELLITE");
    uiContainer->removeAllChildren();
    
    Size size = uiContainer->getContentSize();
    size.height = 0;
    
    int satelliteStep = TownManager::getInstance()->getSatelliteStep();
    
    //
    double spacing = 325;
    double posX = 0;
    double posY = 200;
    for ( int i = 0; i < satelliteStep; i++ )
    {
        int idx = i % 5 + 1;
        
        auto uiLayout = ui::Layout::create();
        uiLayout->setContentSize(Size(size.width, spacing));
        uiLayout->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        uiLayout->setPosition(Vec2(posX, posY));
        uiContainer->addChild(uiLayout);
        
        posY += uiLayout->getContentSize().height;
        
        // 애니메이션 :
        {
            auto animation = Animation::create();
            animation->setDelayPerUnit(0.1);
            for ( int j = 0; j <= 7; j++ )
            {
                auto spriteAni = Sprite::create(MafUtils::format("Assets/ui/tower/space_satellite%d_%02d.png", idx, j).c_str());
                spriteAni->getTexture()->setTexParameters(texParams);
                animation->addSpriteFrame(spriteAni->getSpriteFrame());
            }
            
            auto uiAni = ui::ImageView::create(MafUtils::format("Assets/ui/tower/space_satellite%d_00.png", idx));
            uiAni->setScale(1.5);
            uiAni->setTouchEnabled(true);
            uiAni->addClickEventListener([=](Ref* sender){
            
                auto uiAni = (ui::ImageView*)sender;
                
                Vec2 posToast = uiAni->getParent()->convertToWorldSpace(uiAni->getPosition());
                PopupToast::showImmediately(GAME_TEXTFORMAT("t_ui_bong_town_rokcet_info", uiAni->getTag() * 100), posToast.x, posToast.y);
            });
            uiAni->getVirtualRenderer()->stopAllActions();
            uiAni->getVirtualRenderer()->runAction(RepeatForever::create(Animate::create(animation)));
            uiAni->setTag(i+1);
            uiLayout->addChild(uiAni);
            if ( i % 2 == 0 )
            {
                uiAni->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
                uiAni->setPosition(Vec2(50, uiLayout->getContentSize().height / 2));
            }
            else
            {
                uiAni->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
                uiAni->setPosition(Vec2(uiLayout->getContentSize().width - 50, uiLayout->getContentSize().height / 2));
            }
            
            // action
            auto action1 = MoveBy::create(random(0.5, 1.5), Vec2(0, 10));
            auto action2 = MoveBy::create(random(0.5, 1.5), Vec2(0, -10));
            auto seq = Sequence::create(action1, action2, NULL);
            uiAni->runAction(RepeatForever::create(seq));
            
            //
            _uiSatelliteLast = uiAni;
        }
 
        //
        if ( size.height < posY )
        {
            size.height = posY;
        }
    }
    
    //
    uiContainer->setContentSize(size);
    uiContainer->setPosition(Vec2(0, uiContainerBuilding->getContentSize().height));
}

bool UILayoutTown::drawSatelliteReward(double posX, double posY)
{
    auto uiContainer = _uiScrollTown->getInnerContainer()->getChildByName<ui::Layout*>("UI_CONTAINER_SATELLITE_LAUNCH");
    
    int stepSatellite = TownManager::getInstance()->getSatelliteStep();
    int levelCondition = stepSatellite * 100 + 100;
    
    auto objTownReward = TownManager::getInstance()->getObjectTownReward(2, levelCondition);
    if ( objTownReward == nullptr )
    {
        return false;
    }
    
    auto objItem = objTownReward->getItems();
    if ( objItem == nullptr )
    {
        return false;
    }
    
    auto uiLayoutReward = ui::Layout::create();
    uiLayoutReward->setContentSize(Size(34*2, 41*2));
    uiLayoutReward->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    uiLayoutReward->setPosition(Vec2(posX, posY));
    uiContainer->addChild(uiLayoutReward, 1000);
    {
        auto uiBG = ui::ImageView::create("Assets/ui/tower/satellite_launch_ui_reward_box.png");
        uiBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        uiBG->setPosition(Vec2(uiLayoutReward->getContentSize().width / 2, 0));
        uiBG->setScale(2);
        ((ui::Scale9Sprite*)uiBG->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
        uiLayoutReward->addChild(uiBG);
        
        // 아이템
        auto uiItem = ui::ImageView::create(objItem->getPath());
        uiItem->setPosition(Vec2(uiLayoutReward->getContentSize().width / 2 - 1, 50));
        uiLayoutReward->addChild(uiItem);
        {
            uiItem->setScale(58 / uiItem->getContentSize().width);
        }
        
        auto uiCount = ui::CText::create(objItem->getCount(), GAME_FONT, 16);
        uiCount->setPosition(Vec2(uiLayoutReward->getContentSize().width / 2 - 1, 25));
        uiCount->enableOutline(Color4B::BLACK, 2);
        uiLayoutReward->addChild(uiCount);
    }
    
    return true;
}

void UILayoutTown::drawTownAniBack()
{
    auto uiContainer = _uiScrollTown->getInnerContainer()->getChildByName<ui::Layout*>("UI_CONTAINER_ANI_BACK");
    uiContainer->removeAllChildren();
    
    Size size = uiContainer->getContentSize();
    
    // 애니메이션 1
    {
        auto animation = Animation::create();
        animation->setDelayPerUnit(0.1);
        for ( int i = 1; i <= 7; i++ )
        {
            auto spriteAni = Sprite::create(MafUtils::format("Assets/ui/tower/tower_obj_6_%d.png",i).c_str());
            spriteAni->getTexture()->setTexParameters(texParams);
            animation->addSpriteFrame(spriteAni->getSpriteFrame());
        }
        
        auto uiAni = ui::ImageView::create("Assets/ui/tower/tower_obj_6_1.png");
        uiAni->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
        uiAni->setPosition(Vec2(size.width, 2424));
        uiAni->setScale(4);
        uiContainer->addChild(uiAni, 1000);
        
        auto action1 = DelayTime::create(random(1, 5));
        auto action2 = Animate::create(animation);
        auto action3 = DelayTime::create(random(10, 15));
        auto seq = Sequence::create(action1, action2, action3, NULL);
        uiAni->getVirtualRenderer()->stopAllActions();
        uiAni->getVirtualRenderer()->runAction(RepeatForever::create(seq));
    }
    
    // 애니메이션 2
    {
        auto animation = Animation::create();
        animation->setDelayPerUnit(0.1);
        for ( int i = 1; i <= 9; i++ )
        {
            auto spriteAni = Sprite::create(MafUtils::format("Assets/ui/tower/tower_obj_5_%d.png",i).c_str());
            spriteAni->getTexture()->setTexParameters(texParams);
            animation->addSpriteFrame(spriteAni->getSpriteFrame());
        }
        
        auto uiAni = ui::ImageView::create("Assets/ui/tower/tower_obj_5_9.png");
        uiAni->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        uiAni->setPosition(Vec2(0, 2200));
        uiAni->setScale(4);
        uiContainer->addChild(uiAni, 1000);
        
        auto action1 = DelayTime::create(random(5, 10));
        auto action2 = Animate::create(animation);
        auto action3 = DelayTime::create(random(5, 10));
        auto seq = Sequence::create(action1, action2, action3,  NULL);
        uiAni->getVirtualRenderer()->stopAllActions();
        uiAni->getVirtualRenderer()->runAction(RepeatForever::create(seq));
    }
}

void UILayoutTown::drawTownAniFront()
{
    auto uiContainer = _uiScrollTown->getInnerContainer()->getChildByName<ui::Layout*>("UI_CONTAINER_ANI_FRONT");
    uiContainer->removeAllChildren();
    
    Size size = uiContainer->getContentSize();
    
    // 애니메이션 : 하트
    {
        auto animation = Animation::create();
        animation->setDelayPerUnit(0.1);
        for ( int i = 0; i <= 7; i++ )
        {
            auto spriteAni = Sprite::create(MafUtils::format("Assets/ui/tower/tower_front_obj_2_%d.png",i).c_str());
            spriteAni->getTexture()->setTexParameters(texParams);
            animation->addSpriteFrame(spriteAni->getSpriteFrame());
        }
        
        auto uiAni = ui::ImageView::create("Assets/ui/tower/tower_front_obj_2_0.png");
        uiAni->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        uiAni->setPosition(Vec2(120, 130));
        uiAni->setScale(2);
        uiAni->getVirtualRenderer()->stopAllActions();
        uiAni->getVirtualRenderer()->runAction(RepeatForever::create(Animate::create(animation)));
        uiContainer->addChild(uiAni, 1000);
    }
    
    // 애니메이션 : 정원
    {
        auto animation = Animation::create();
        animation->setDelayPerUnit(0.1);
        for ( int i = 0; i <= 3; i++ )
        {
            auto spriteAni = Sprite::create(MafUtils::format("Assets/ui/tower/tower_front_obj_1_%d.png",i).c_str());
            spriteAni->getTexture()->setTexParameters(texParams);
            animation->addSpriteFrame(spriteAni->getSpriteFrame());
        }
        
        auto uiAni = ui::ImageView::create("Assets/ui/tower/tower_front_obj_1_0.png");
        uiAni->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        uiAni->setPosition(Vec2(530, 100));
        uiAni->setScale(2);
        uiAni->getVirtualRenderer()->stopAllActions();
        uiAni->getVirtualRenderer()->runAction(RepeatForever::create(Animate::create(animation)));
        uiContainer->addChild(uiAni, 1000);
    }
    
    // 애니메이션 : 봉식
    {
        auto uiAni1 = ui::ImageView::create("Assets/ui/tower/tower_front_obj_4.png");
        uiAni1->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        uiAni1->setPosition(Vec2(390, 120));
        uiAni1->setScale(2);
        ((ui::Scale9Sprite*)uiAni1->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
        uiContainer->addChild(uiAni1, 1000);
        
        auto uiAni2 = ui::ImageView::create("Assets/ui/tower/tower_front_obj_4.png");
        uiAni2->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        uiAni2->setPosition(Vec2(480*2, 190*2));
        uiAni2->setScale(2);
        ((ui::Scale9Sprite*)uiAni2->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
        uiContainer->addChild(uiAni2, 1000);
    }
    
    // 애니메이션 : 펫 1
    {
        auto uiAni1 = ui::ImageView::create("Assets/ui/tower/tower_front_obj_5.png");
        uiAni1->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        uiAni1->setPosition(Vec2(340, 120));
        uiAni1->setScale(2);
        ((ui::Scale9Sprite*)uiAni1->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
        uiContainer->addChild(uiAni1, 1000);
        
        auto uiAni2 = ui::ImageView::create("Assets/ui/tower/tower_front_obj_5.png");
        uiAni2->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        uiAni2->setPosition(Vec2(455*2, 190*2));
        uiAni2->setScale(2);
        ((ui::Scale9Sprite*)uiAni2->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
        uiContainer->addChild(uiAni2, 1000);
    }
    
    // 애니메이션 : 펫 2
    {
        auto uiAni1 = ui::ImageView::create("Assets/ui/tower/tower_front_obj_6.png");
        uiAni1->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        uiAni1->setPosition(Vec2(550, 40));
        uiAni1->setScale(2);
        ((ui::Scale9Sprite*)uiAni1->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
        uiContainer->addChild(uiAni1, 1000);
        
        auto uiAni2 = ui::ImageView::create("Assets/ui/tower/tower_front_obj_6.png");
        uiAni2->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        uiAni2->setPosition(Vec2(440*2, 190*2));
        uiAni2->setScale(2);
        ((ui::Scale9Sprite*)uiAni2->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
        uiContainer->addChild(uiAni2, 1000);
    }
    
    // 애니메이션 : 야자수
    {
        auto uiAni = ui::ImageView::create("Assets/ui/tower/tower_front_obj_3.png");
        uiAni->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        uiAni->setPosition(Vec2(184, 50));
        uiAni->setScale(2);
        ((ui::Scale9Sprite*)uiAni->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
        uiContainer->addChild(uiAni, 1000);
    }
    
    
    // 애니메이션 : 큰풍선
    {
        auto animation = Animation::create();
        animation->setDelayPerUnit(0.2);
        for ( int i = 1; i <= 4; i++ )
        {
            auto spriteAni = Sprite::create(MafUtils::format("Assets/ui/tower/tower_obj_2_%d.png",i).c_str());
            spriteAni->getTexture()->setTexParameters(texParams);
            animation->addSpriteFrame(spriteAni->getSpriteFrame());
        }
        
        auto uiAni = ui::ImageView::create("Assets/ui/tower/tower_obj_2_1.png");
        uiAni->setPosition(Vec2(660, 600));
        uiAni->setScale(4);
        uiAni->getVirtualRenderer()->stopAllActions();
        uiAni->getVirtualRenderer()->runAction(RepeatForever::create(Animate::create(animation)));
        uiContainer->addChild(uiAni, 1000);
    }
    
    // 애니메이션 : 비행기
    {
        auto animation = Animation::create();
        animation->setDelayPerUnit(0.1);
        for ( int i = 1; i <= 3; i++ )
        {
            auto spriteAni = Sprite::create(MafUtils::format("Assets/ui/tower/tower_obj_3_%d.png",i).c_str());
            spriteAni->getTexture()->setTexParameters(texParams);
            animation->addSpriteFrame(spriteAni->getSpriteFrame());
        }
        
        auto uiAni = ui::ImageView::create("Assets/ui/tower/tower_obj_3_1.png");
        uiAni->setPosition(Vec2(uiContainer->getContentSize().width + 100, 800));
        uiAni->setScale(4);
        uiAni->getVirtualRenderer()->stopAllActions();
        uiAni->getVirtualRenderer()->runAction(RepeatForever::create(Animate::create(animation)));
        uiContainer->addChild(uiAni, 1000);
        
        auto action1 = Place::create(Vec2(uiContainer->getContentSize().width + 150, 800));
        auto action2 = DelayTime::create(random(1, 20));
        auto action3 = MoveTo::create(10.0, Vec2(-150, 1000));
        auto action4 = DelayTime::create(20.0);
        auto seq = Sequence::create(action1, action2, action3, action4, NULL);
        uiAni->runAction(RepeatForever::create(seq));
    }
    
    // 애니메이션 : 우주선
    {
        auto animation = Animation::create();
        animation->setDelayPerUnit(0.1);
        for ( int i = 1; i <= 4; i++ )
        {
            auto spriteAni = Sprite::create(MafUtils::format("Assets/ui/tower/tower_obj_4_%d.png",i).c_str());
            spriteAni->getTexture()->setTexParameters(texParams);
            animation->addSpriteFrame(spriteAni->getSpriteFrame());
        }
        
        auto uiAni = ui::ImageView::create("Assets/ui/tower/tower_obj_4_1.png");
        uiAni->setPosition(Vec2(80, 1080));
        uiAni->setScale(4);
        uiAni->setOpacity(0);
        uiAni->getVirtualRenderer()->stopAllActions();
        uiAni->getVirtualRenderer()->runAction(RepeatForever::create(Animate::create(animation)));
        uiContainer->addChild(uiAni, 1000);
        
        ccBezierConfig config;
        config.controlPoint_1 = Point(150, 1450); //제어점 설정
        config.controlPoint_2 = Point(400, 1650); //제어점 설정
        config.endPosition = Point(580, 1550); //도착점 설정
            
        //BezierTo::create(액션시간 , 설정)
        auto action_Move = BezierTo::create(5.0f, config);   //베지어 액션 생성
        
        auto action1_1 = Place::create(Vec2(80, 1080));
        auto action1_2 = EaseInOut::create(action_Move, 10.0);
        auto action1_3 = DelayTime::create(20.0);
        auto seq1 = Sequence::create(action1_1, action1_2, action1_3, NULL);
        
        auto action2_1 = FadeIn::create(5.0);
        auto action2_2 = DelayTime::create(2.0);
        auto action2_3 = FadeOut::create(3.0);
        auto action2_4 = DelayTime::create(15.0);
        auto seq2 = Sequence::create(action2_1, action2_2, action2_3, action2_4, NULL);
        
        auto spawn = Spawn::create(seq1, seq2, NULL);
        uiAni->runAction(RepeatForever::create(spawn));
    }
}

void UILayoutTown::drawTown()
{
    auto layout = _uiContainerTown;
    
    auto uButtonArrowL = layout->getChildByName<ui::Button*>("UI_ARROW_L");
    auto uButtonArrowR = layout->getChildByName<ui::Button*>("UI_ARROW_R");
    
    int nSatelliteTutorial = TownManager::getInstance()->getSatelliteTutorial();
    if ( nSatelliteTutorial <= 0 )
    {
        uButtonArrowL->setVisible(false);
        uButtonArrowR->setVisible(false);
    }
    else
    {
        if ( _bTownPos == false )
        {
            uButtonArrowL->setVisible(false);
            uButtonArrowR->setVisible(true);
        }
        else
        {
            uButtonArrowL->setVisible(true);
            uButtonArrowR->setVisible(false);
        }
    }
    
    
    //
    auto uiContainerBuilding = _uiScrollTown->getInnerContainer()->getChildByName<ui::Layout*>("UI_CONTAINER_BUILDING");
    auto uiContainerSatellite = _uiScrollTown->getInnerContainer()->getChildByName<ui::Layout*>("UI_CONTAINER_SATELLITE");
    
    auto pos = _uiScrollTown->getInnerContainerPosition();
    auto size = _uiScrollTown->getInnerContainerSize();
    size.height = uiContainerBuilding->getContentSize().height + uiContainerSatellite->getContentSize().height;
    _uiScrollTown->setInnerContainerSize(size);
    _uiScrollTown->setInnerContainerPosition(pos);
}

void UILayoutTown::drawUpgrade()
{
    auto layout = _uiContainerUpgrade;

    for ( auto& child : layout->getChildren() )
    {
        E_TOWN eType = (E_TOWN)child->getTag();
        
        if ( eType == E_TOWN::BUILDING_1 )
        {
            drawCellUpgradeTower((ui::Widget*)child, eType);
        }
        else if ( eType == E_TOWN::SATELLITE )
        {
            drawCellUpgradeSatellite((ui::Widget*)child, eType);
        }
        else
        {
            drawCellUpgrade((ui::Widget*)child, eType);
        }
    }
}

void UILayoutTown::drawCellUpgrade(cocos2d::ui::Widget* childview,  E_TOWN eType)
{
    childview->removeAllChildren();
    
    Size size = childview->getContentSize();
    
    //
    int level = TownManager::getInstance()->getTownLevel(eType);
    int levelMax = TownManager::getInstance()->getTownLevelMax(eType);
    double value = TownManager::getInstance()->getTownEffect(eType, level);
    
    std::string name = TownManager::getInstance()->getTownName(eType);
    std::string floor = GAME_TEXTFORMAT("t_ui_town_floor", level);
    std::string desc = TownManager::getInstance()->getTownExplain(eType);
    
    std::string effect = MafUtils::doubleToString(value);
    MafUtils::convertNumberToShort(effect);
    effect = MafUtils::format("%s%%", effect.c_str());
    
    
    std::string currentPrice = TownManager::getInstance()->getBuildingProgress(eType);
    std::string price = TownManager::getInstance()->getTownPriceCoin(eType, level);
    std::string priceAdd = price;
    priceAdd.pop_back();
    priceAdd.pop_back();
    
    std::string priceTotal = priceAdd;
    
    int percent = 0;
    for ( int i = 0; i < 100; i++ )
    {
        if ( !MafUtils::compareStr(currentPrice, priceTotal) )
        {
            break;
        }
        
        priceTotal = MafUtils::bigAddNum(priceTotal, priceAdd);
        percent++;
    }
    
    // name
    auto uiTextName = ui::CText::create(name, GAME_FONT, 24);
    uiTextName->setTextColor(Color4B(COLOR_COIN));
    uiTextName->setTextAreaSize(Size(size.width, uiTextName->getContentSize().height));
    uiTextName->setTextOverflow(Label::Overflow::SHRINK);
    uiTextName->setTextHorizontalAlignment(TextHAlignment::CENTER);
    childview->addChild(uiTextName);
    
    // floor
    auto uiTextFloor = ui::CText::create(floor, GAME_FONT, 18);
    uiTextFloor->setTextAreaSize(Size(size.width, uiTextFloor->getContentSize().height));
    uiTextFloor->setTextOverflow(Label::Overflow::SHRINK);
    uiTextFloor->setTextHorizontalAlignment(TextHAlignment::CENTER);
    childview->addChild(uiTextFloor);
    
    // desc
    auto uiTextDesc = ui::CText::create(desc, GAME_FONT, 18);
    uiTextDesc->setTextAreaSize(Size(size.width, uiTextDesc->getContentSize().height));
    uiTextDesc->setTextOverflow(Label::Overflow::SHRINK);
    uiTextDesc->setTextHorizontalAlignment(TextHAlignment::CENTER);
    childview->addChild(uiTextDesc);
    
    // effect
    auto uiTextEffect = ui::CText::create(effect, GAME_FONT, 18);
    uiTextEffect->setTextAreaSize(Size(size.width, uiTextEffect->getContentSize().height));
    uiTextEffect->setTextOverflow(Label::Overflow::SHRINK);
    uiTextEffect->setTextHorizontalAlignment(TextHAlignment::CENTER);
    childview->addChild(uiTextEffect);

    //
    auto uiBarSavingBG = ui::ImageView::create("Assets/ui/tower/tower_bar1.png");
    childview->addChild(uiBarSavingBG);
    {
        auto params = ui::LinearLayoutParameter::create();
        params->setGravity(ui::LinearLayoutParameter::LinearGravity::CENTER_HORIZONTAL);
        params->setMargin(ui::Margin(0, 10, 0, 0));
        uiBarSavingBG->setLayoutParameter(params);
        
        auto uiBarSaving = ui::LoadingBar::create("Assets/ui/tower/tower_bar2.png");
        uiBarSaving->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        uiBarSaving->setPosition(Vec2(0, 0));
        uiBarSaving->setPercent(percent);
        uiBarSavingBG->addChild(uiBarSaving);
        
        if ( level < levelMax )
        {
            std::string priceShort = price;
            priceShort = MafUtils::convertNumberToShort(priceShort);
            auto uiTextPrice = ui::CText::create(priceShort, GAME_FONT, 20);
            uiTextPrice->setPosition(Vec2(uiBarSavingBG->getContentSize().width / 2, uiBarSavingBG->getContentSize().height / 2));
            uiBarSavingBG->addChild(uiTextPrice);
            
            auto uiImgPriceIcon = ui::ImageView::create("Assets/icon/menu_coin.png");
            uiImgPriceIcon->setPosition(Vec2(uiTextPrice->getPositionX() - uiTextPrice->getContentSize().width / 2 - 10, uiTextPrice->getPositionY()));
            uiImgPriceIcon->setScale(0.8);
            uiBarSavingBG->addChild(uiImgPriceIcon);
        }
    }
    
    auto uiBtnUpgrade = ui::Button::create("Assets/ui_common/tower_btn.png", "", "Assets/ui_common/tower_btn_max.png");
    uiBtnUpgrade->addClickEventListener(CC_CALLBACK_1(UILayoutTown::onClickBuildingUpgradeAsk, this));
    uiBtnUpgrade->setZoomScale(0.05f);
    uiBtnUpgrade->setTag((int)eType);
    childview->addChild(uiBtnUpgrade);
    {
        auto params = ui::LinearLayoutParameter::create();
        params->setGravity(ui::LinearLayoutParameter::LinearGravity::CENTER_HORIZONTAL);
        params->setMargin(ui::Margin(0, 15, 0, 0));
        uiBtnUpgrade->setLayoutParameter(params);
        
        auto uiText = ui::CText::create(GAME_TEXT("t_ui_town_investment"), GAME_FONT, 24);
        uiText->setPosition(Vec2(uiBtnUpgrade->getContentSize().width / 2, uiBtnUpgrade->getContentSize().height / 2));
        uiBtnUpgrade->addChild(uiText);
    
        if ( level >= levelMax )
        {
            uiText->setString("MAX");
            uiText->setTextColor(Color4B(COLOR_COIN));
            uiBtnUpgrade->setEnabled(false);
        }
    }
}

void UILayoutTown::drawCellUpgradeTower(cocos2d::ui::Widget* childview,  E_TOWN eType)
{
    childview->removeAllChildren();
    
    Size size = childview->getContentSize();
    
    //
    int level = TownManager::getInstance()->getTownLevel(eType);
    int levelMax = TownManager::getInstance()->getTownLevelMax(eType);
    double value = TownManager::getInstance()->getTownEffect(eType, level);
    double valueUp = TownManager::getInstance()->getTownEffect(eType, level + 1) - value;
    
    std::string name = TownManager::getInstance()->getTownName(eType);
    std::string floor = GAME_TEXTFORMAT("t_ui_town_floor", level);
    std::string desc = TownManager::getInstance()->getTownExplain(eType);
    
    std::string effect = MafUtils::doubleToString(value);
    MafUtils::convertNumberToShort(effect);
    effect = MafUtils::format("%s%%", effect.c_str());
    
    std::string effectPlus = MafUtils::doubleToString(valueUp);
    MafUtils::convertNumberToShort(effectPlus);
    effectPlus = MafUtils::format("+ %s%%", effectPlus.c_str());
    
    std::string price = TownManager::getInstance()->getTownPriceCoin(eType, level);
    std::string priceGem = MafUtils::toString(TownManager::getInstance()->getTownPriceGem(eType, level));
    
    
    // icon
    auto uiImgBox = ui::ImageView::create("Assets/ui_common/box_quest.png");
    uiImgBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    uiImgBox->setPosition(Vec2(3, size.height / 2));
    childview->addChild(uiImgBox);
    {
        auto uiImgIcon = ui::ImageView::create("Assets/ui/tower/tower_icon.png");
        uiImgIcon->setPosition(Vec2(uiImgBox->getContentSize().width / 2, uiImgBox->getContentSize().height / 2));
        uiImgBox->addChild(uiImgIcon);
    }
    
    // info
    auto uiLayoutInfo = ui::Layout::create();
    uiLayoutInfo->setContentSize(Size(420, 116));
    uiLayoutInfo->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    uiLayoutInfo->setPosition(Vec2(uiImgBox->getPositionX() + uiImgBox->getContentSize().width + 5, uiImgBox->getPositionY()));
    uiLayoutInfo->setLayoutType(ui::Layout::Type::VERTICAL);
    childview->addChild(uiLayoutInfo);
    {
        // name
        auto uiTextName = ui::CText::create(name, GAME_FONT, 24);
        uiTextName->setTextColor(Color4B(COLOR_COIN));
        uiTextName->setTextAreaSize(Size(uiLayoutInfo->getContentSize().width, uiTextName->getContentSize().height));
        uiTextName->setTextOverflow(Label::Overflow::SHRINK);
        uiTextName->setTextHorizontalAlignment(TextHAlignment::LEFT);
        uiLayoutInfo->addChild(uiTextName);
        
        // floor
        auto uiTextFloor = ui::CText::create(floor, GAME_FONT, 18);
        uiTextFloor->setTextAreaSize(Size(uiLayoutInfo->getContentSize().width, uiTextFloor->getContentSize().height));
        uiTextFloor->setTextOverflow(Label::Overflow::SHRINK);
        uiTextFloor->setTextHorizontalAlignment(TextHAlignment::LEFT);
        uiLayoutInfo->addChild(uiTextFloor);
        
        // desc
        auto uiTextDesc = ui::CText::create(desc, GAME_FONT, 18);
        uiTextDesc->setTextAreaSize(Size(uiLayoutInfo->getContentSize().width, uiTextDesc->getContentSize().height));
        uiTextDesc->setTextOverflow(Label::Overflow::SHRINK);
        uiTextDesc->setTextHorizontalAlignment(TextHAlignment::LEFT);
        uiLayoutInfo->addChild(uiTextDesc);
        
        // effect
        auto uiTextEffect = ui::CText::create(effect, GAME_FONT, 18);
        uiTextEffect->setTextAreaSize(Size(uiLayoutInfo->getContentSize().width, uiTextEffect->getContentSize().height));
        uiTextEffect->setTextOverflow(Label::Overflow::SHRINK);
        uiTextEffect->setTextHorizontalAlignment(TextHAlignment::LEFT);
        uiLayoutInfo->addChild(uiTextEffect);
    }
    
    //
    auto uiBtnUpgrade = ui::Button::create("Assets/ui_common/btn_upgrade_bg.png", "", "Assets/ui_common/btn_upgrade_max.png");
    uiBtnUpgrade->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    uiBtnUpgrade->setPosition(Vec2(size.width - 3, size.height / 2));
    uiBtnUpgrade->addClickEventListener(CC_CALLBACK_1(UILayoutTown::onClickBuildingUpgradeAsk, this));
    uiBtnUpgrade->setZoomScale(0.05f);
    uiBtnUpgrade->setTag((int)eType);
    childview->addChild(uiBtnUpgrade);
    if ( level >= levelMax )
    {
        auto uiText = ui::CText::create("MAX", GAME_FONT, 24);
        uiText->setPosition(Vec2(uiBtnUpgrade->getContentSize().width / 2, uiBtnUpgrade->getContentSize().height / 2));
        uiText->setTextColor(Color4B(COLOR_COIN));
        uiBtnUpgrade->addChild(uiText);
        
        uiBtnUpgrade->setEnabled(false);
    }
    else
    {
        std::string priceShort = price;
        priceShort = MafUtils::convertNumberToShort(priceShort);
        auto uiTextGold = ui::CText::create(priceShort, GAME_FONT, 22);
        uiTextGold->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        uiTextGold->setPosition(Vec2(uiBtnUpgrade->getContentSize().width / 2, uiBtnUpgrade->getContentSize().height * 0.5));
        uiBtnUpgrade->addChild(uiTextGold);
        {
            auto uiImgIcon = ui::ImageView::create("Assets/icon/menu_coin.png");
            uiImgIcon->setPosition(Vec2(-40, uiTextGold->getContentSize().height / 2));
            uiTextGold->addChild(uiImgIcon);
        }
        
        auto uiTextGem = ui::CText::create(priceGem, GAME_FONT, 22);
        uiTextGem->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        uiTextGem->setPosition(Vec2(uiBtnUpgrade->getContentSize().width / 2, uiBtnUpgrade->getContentSize().height * 0.25));
        uiBtnUpgrade->addChild(uiTextGem);
        {
            auto uiImgIcon = ui::ImageView::create("Assets/icon/icon_jewel.png");
            uiImgIcon->setPosition(Vec2(-40, uiTextGold->getContentSize().height / 2));
            uiTextGem->addChild(uiImgIcon);
        }
        
        auto uiTextValuePlus = ui::CText::create(effectPlus, GAME_FONT, 26);
        uiTextValuePlus->setPosition(Vec2(uiBtnUpgrade->getContentSize().width / 2, uiBtnUpgrade->getContentSize().height * 0.75));
        uiBtnUpgrade->addChild(uiTextValuePlus);
        
        //
        std::string currencyGold = ItemsMoneyManager::getInstance()->getGold();
        std::string currencyGem = MafUtils::toString(ItemsMoneyManager::getInstance()->getGem());
        
        if ( MafUtils::compareStr(currencyGold, price) == false || MafUtils::compareStr(currencyGem, priceGem) == false )
        {
            uiBtnUpgrade->loadTextureNormal("Assets/ui_common/btn_upgrade_max.png");
        }
    }
}

void UILayoutTown::drawCellUpgradeSatellite(cocos2d::ui::Widget* childview,  E_TOWN eType)
{
    childview->removeAllChildren();
    
    Size size = childview->getContentSize();
    
    //
    int satelliteTutorial = TownManager::getInstance()->getSatelliteTutorial();
    
    int levelBuilding = TownManager::getInstance()->getTownLevel(E_TOWN::BUILDING_1);
    int levelBuildingMax = TownManager::getInstance()->getTownLevelMax(E_TOWN::BUILDING_1);
    if ( levelBuilding < levelBuildingMax )
    {
        auto nodeContents = UtilsWidget::getInstance()->createIconText("Assets/ui/tower/tower_mole_icon.png", GAME_TEXT("t_ui_bong_town_rokcet_error_1"), size.width, 24);
        nodeContents->setPosition(Vec2(size.width / 2, size.height / 2));
        childview->addChild(nodeContents);
    }
    else if ( satelliteTutorial <= 0 )
    {// 오픈 버튼 스토리
        auto uiBtnOpen = ui::Button::create("Assets/ui/tower/tower_bottom_btn.png");
        uiBtnOpen->setScale9Enabled(true);
        uiBtnOpen->setContentSize(Size(704, 124));
        uiBtnOpen->setPosition(Vec2(size.width / 2, size.height / 2));
        uiBtnOpen->addClickEventListener(CC_CALLBACK_1(UILayoutTown::onClickSatelliteOpen, this));
        childview->addChild(uiBtnOpen);
        {
            auto nodeContents = UtilsWidget::getInstance()->createIconText("Assets/ui/tower/satellite_icon.png", GAME_TEXT("t_ui_bong_town_rokcet_start_bt"), size.width, 24);
            nodeContents->setPosition(Vec2(size.width / 2, size.height / 2));
            uiBtnOpen->addChild(nodeContents);
        }
        
    }
    else
    {// 오픈 satelliteTutorial >= 1
        int level = TownManager::getInstance()->getTownLevel(eType);
        int levelMax = TownManager::getInstance()->getTownLevelMax(eType);
        double value = TownManager::getInstance()->getTownEffect(eType, level);
        double valueUp = TownManager::getInstance()->getTownEffect(eType, level + 1) - value;
        
        std::string name = TownManager::getInstance()->getTownName(eType);
        std::string floor = MafUtils::format("Lv.%d", level);
        std::string desc = TownManager::getInstance()->getTownExplain(eType);
        std::string pathIcon = TownManager::getInstance()->getTownPathIcon(eType);
        
        std::string effect = MafUtils::doubleToString(value);
        MafUtils::convertNumberToShort(effect);
        effect = MafUtils::format("%s%%", effect.c_str());
        
        std::string effectPlus = MafUtils::doubleToString(valueUp);
        MafUtils::convertNumberToShort(effectPlus);
        effectPlus = MafUtils::format("+ %s%%", effectPlus.c_str());
        
        std::string price = TownManager::getInstance()->getTownPriceCoin(eType, level);
        std::string priceGem = MafUtils::toString(TownManager::getInstance()->getTownPriceGem(eType, level));
        
        
        // icon
        auto uiImgBox = ui::ImageView::create("Assets/ui_common/box_quest.png");
        uiImgBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        uiImgBox->setPosition(Vec2(3, size.height / 2));
        childview->addChild(uiImgBox);
        {
            auto uiImgIcon = ui::ImageView::create(pathIcon);
            uiImgIcon->setPosition(Vec2(uiImgBox->getContentSize().width / 2, uiImgBox->getContentSize().height / 2));
            uiImgBox->addChild(uiImgIcon);
        }
        
        // info
        auto uiLayoutInfo = ui::Layout::create();
        uiLayoutInfo->setContentSize(Size(420, 116));
        uiLayoutInfo->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        uiLayoutInfo->setPosition(Vec2(uiImgBox->getPositionX() + uiImgBox->getContentSize().width + 5, uiImgBox->getPositionY()));
        uiLayoutInfo->setLayoutType(ui::Layout::Type::VERTICAL);
        childview->addChild(uiLayoutInfo);
        {
            // name
            auto uiTextName = ui::CText::create(name, GAME_FONT, 24);
            uiTextName->setTextColor(Color4B(COLOR_COIN));
            uiTextName->setTextAreaSize(Size(uiLayoutInfo->getContentSize().width, uiTextName->getContentSize().height));
            uiTextName->setTextOverflow(Label::Overflow::SHRINK);
            uiTextName->setTextHorizontalAlignment(TextHAlignment::LEFT);
            uiLayoutInfo->addChild(uiTextName);
            
            // floor
            auto uiTextFloor = ui::CText::create(floor, GAME_FONT, 18);
            uiTextFloor->setTextAreaSize(Size(uiLayoutInfo->getContentSize().width, uiTextFloor->getContentSize().height));
            uiTextFloor->setTextOverflow(Label::Overflow::SHRINK);
            uiTextFloor->setTextHorizontalAlignment(TextHAlignment::LEFT);
            uiLayoutInfo->addChild(uiTextFloor);
            
            // desc
            auto uiTextDesc = ui::CText::create(desc, GAME_FONT, 18);
            uiTextDesc->setTextAreaSize(Size(uiLayoutInfo->getContentSize().width, uiTextDesc->getContentSize().height));
            uiTextDesc->setTextOverflow(Label::Overflow::SHRINK);
            uiTextDesc->setTextHorizontalAlignment(TextHAlignment::LEFT);
            uiLayoutInfo->addChild(uiTextDesc);
            
            // effect
            auto uiTextEffect = ui::CText::create(effect, GAME_FONT, 18);
            uiTextEffect->setTextAreaSize(Size(uiLayoutInfo->getContentSize().width, uiTextEffect->getContentSize().height));
            uiTextEffect->setTextOverflow(Label::Overflow::SHRINK);
            uiTextEffect->setTextHorizontalAlignment(TextHAlignment::LEFT);
            uiLayoutInfo->addChild(uiTextEffect);
        }
        
        //
        auto uiBtnUpgrade = ui::Button::create("Assets/ui_common/btn_upgrade_bg.png", "", "Assets/ui_common/btn_upgrade_max.png");
        uiBtnUpgrade->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        uiBtnUpgrade->setPosition(Vec2(size.width - 3, size.height / 2));
        uiBtnUpgrade->addClickEventListener(CC_CALLBACK_1(UILayoutTown::onClickBuildingUpgradeAsk, this));
        uiBtnUpgrade->setZoomScale(0.05f);
        uiBtnUpgrade->setTag((int)eType);
        childview->addChild(uiBtnUpgrade);
        if ( level >= levelMax )
        {
            auto uiText = ui::CText::create("MAX", GAME_FONT, 24);
            uiText->setPosition(Vec2(uiBtnUpgrade->getContentSize().width / 2, uiBtnUpgrade->getContentSize().height / 2));
            uiText->setTextColor(Color4B(COLOR_COIN));
            uiBtnUpgrade->addChild(uiText);
            
            uiBtnUpgrade->setEnabled(false);
        }
        else
        {
            std::string priceShort = price;
            priceShort = MafUtils::convertNumberToShort(priceShort);
            auto uiTextGold = ui::CText::create(priceShort, GAME_FONT, 22);
            uiTextGold->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            uiTextGold->setPosition(Vec2(uiBtnUpgrade->getContentSize().width / 2, uiBtnUpgrade->getContentSize().height * 0.5));
            uiBtnUpgrade->addChild(uiTextGold);
            {
                auto uiImgIcon = ui::ImageView::create("Assets/icon/menu_coin.png");
                uiImgIcon->setPosition(Vec2(-40, uiTextGold->getContentSize().height / 2));
                uiTextGold->addChild(uiImgIcon);
            }
            
            auto uiTextGem = ui::CText::create(priceGem, GAME_FONT, 22);
            uiTextGem->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            uiTextGem->setPosition(Vec2(uiBtnUpgrade->getContentSize().width / 2, uiBtnUpgrade->getContentSize().height * 0.25));
            uiBtnUpgrade->addChild(uiTextGem);
            {
                auto uiImgIcon = ui::ImageView::create("Assets/icon/icon_jewel.png");
                uiImgIcon->setPosition(Vec2(-40, uiTextGold->getContentSize().height / 2));
                uiTextGem->addChild(uiImgIcon);
            }
            
            auto uiTextValuePlus = ui::CText::create(effectPlus, GAME_FONT, 26);
            uiTextValuePlus->setPosition(Vec2(uiBtnUpgrade->getContentSize().width / 2, uiBtnUpgrade->getContentSize().height * 0.75));
            uiBtnUpgrade->addChild(uiTextValuePlus);
            
            //
            std::string currencyGold = ItemsMoneyManager::getInstance()->getGold();
            std::string currencyGem = MafUtils::toString(ItemsMoneyManager::getInstance()->getGem());
            
            if ( MafUtils::compareStr(currencyGold, price) == false || MafUtils::compareStr(currencyGem, priceGem) == false )
            {
                uiBtnUpgrade->loadTextureNormal("Assets/ui_common/btn_upgrade_max.png");
            }
        }
    }
}

#pragma mark - set, get

#pragma mark - callback
void UILayoutTown::callbackInfo(bool bResult)
{
    //
    PopupLoading::hide();
    
    if ( bResult == false )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_30"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        
        auto parent = dynamic_cast<PopupBase*>(this->getWidgetParent());
        if ( parent != nullptr )
        {
            parent->hide();
        }
        return;
    }
    
    // draw
    drawBuilding();
    drawSatelliteLaunch();
    drawSatellite();
    
    drawTown();
}

void UILayoutTown::callbackRewardBuilding(bool bResult, int nResult)
{
    //
    PopupLoading::hide();
    
    if ( bResult == false )
    {
        std::string strMessage = GAME_TEXT("t_ui_error_30");
        if ( nResult == 3 )
        {
            strMessage = GAME_TEXT("t_ui_growth_pack_reward_msg");
        }
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), strMessage);
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, [=](Ref* sender){
            
            PopupLoading::show();
            TownManager::getInstance()->requestInfo(CC_CALLBACK_1(UILayoutTown::callbackInfo, this));
            
        });
        popup->show();
        return;
    }
    
    // draw
    drawBuilding();
    drawSatelliteLaunch();
    drawSatellite();
    
    drawTown();
}

void UILayoutTown::callbackRewardSatellite(bool bResult, int nResult)
{
    //
    PopupLoading::hide();
    
    if ( bResult == false )
    {
        std::string strMessage = GAME_TEXT("t_ui_error_30");
        if ( nResult == 3 )
        {
            strMessage = GAME_TEXT("t_ui_growth_pack_reward_msg");
        }
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), strMessage);
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, [=](Ref* sender){
            
            PopupLoading::show();
            TownManager::getInstance()->requestInfo(CC_CALLBACK_1(UILayoutTown::callbackInfo, this));
            
        });
        popup->show();
        
        if ( nResult == 3 )
        {
            int satelliteStep = TownManager::getInstance()->getSatelliteStep();
            TownManager::getInstance()->setSatelliteStep(satelliteStep + 1);
            TownManager::getInstance()->saveData();
        }
        return;
    }
    
    //
    int satelliteStep = TownManager::getInstance()->getSatelliteStep();
    TownManager::getInstance()->setSatelliteStep(satelliteStep + 1);
    TownManager::getInstance()->saveData();
    
    // draw
    drawBuilding();
    drawSatelliteLaunch();
    drawSatellite();
    
    drawTown();
    
    // ani
    auto uiContainer = _uiScrollTown->getInnerContainer()->getChildByName<ui::Layout*>("UI_CONTAINER_SATELLITE_LAUNCH");
    if ( uiContainer != nullptr )
    {
        //
        auto animation = Animation::create();
        animation->setDelayPerUnit(0.1);
        for ( int i = 0; i <= 11; i++ )
        {
            auto spriteAni = Sprite::create(MafUtils::format("Assets/ui/tower/satellite_launch_%02d.png",i).c_str());
            spriteAni->getTexture()->setTexParameters(texParams);
            animation->addSpriteFrame(spriteAni->getSpriteFrame());
        }
        
        auto uiAni = ui::ImageView::create("Assets/ui/tower/satellite_launch_00.png");
        uiAni->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        uiAni->setPosition(Vec2(570*2, 35*2));
        uiAni->setScale(2);
        ((ui::Scale9Sprite*)uiAni->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
        uiContainer->addChild(uiAni, 1000);
        
        
        PopupLoadingLock::show();
        auto action0 = DelayTime::create(1.5);
        auto action1 = CallFunc::create([=](){
            SoundManager::SoundEffectStart(SOUND_ROCKET_START);
        });
        auto action2 = Animate::create(animation);
        auto action3 = FadeOut::create(0);
        auto action4 = DelayTime::create(0.5);
        auto action5 = CallFunc::create([=](){
        
            PopupLoadingLock::hide();
            _uiScrollTown->scrollToTop(1, false);
            
            if ( _uiSatelliteLast != nullptr )
            {
                int idx = _uiSatelliteLast->getTag() % 5 + 1;
                
                
                auto animation = Animation::create();
                animation->setDelayPerUnit(0.1);
                for ( int j = 0; j <= 4; j++ )
                {
                    auto spriteAni = Sprite::create(MafUtils::format("Assets/ui/tower/space_satellite%d_in%02d.png", idx, j));
                    spriteAni->getTexture()->setTexParameters(texParams);
                    animation->addSpriteFrame(spriteAni->getSpriteFrame());
                }
                
                {
                    auto action0 = DelayTime::create(1.0);
                    auto action1 = Animate::create(animation);
                    auto action2 = CallFuncN::create([=](Node* sender){
                
                        auto animation = Animation::create();
                        animation->setDelayPerUnit(0.1);
                        for ( int j = 0; j <= 7; j++ )
                        {
                            auto spriteAni = Sprite::create(MafUtils::format("Assets/ui/tower/space_satellite%d_%02d.png", idx, j));
                            spriteAni->getTexture()->setTexParameters(texParams);
                            animation->addSpriteFrame(spriteAni->getSpriteFrame());
                        }
                        sender->runAction(RepeatForever::create(Animate::create(animation)));
                    });
                    auto seq = Sequence::create(action0, action1, action2, NULL);
                    
                    _uiSatelliteLast->loadTexture("Assets/ui/tower/space_satellite_in_blank.png");
                    _uiSatelliteLast->getVirtualRenderer()->stopAllActions();
                    _uiSatelliteLast->getVirtualRenderer()->runAction(seq);
                }
                {
                    auto action1 = MoveBy::create(random(0.5, 1.5), Vec2(0, 10));
                    auto action2 = MoveBy::create(random(0.5, 1.5), Vec2(0, -10));
                    auto seq = Sequence::create(action1, action2, NULL);
                    _uiSatelliteLast->stopAllActions();
                    _uiSatelliteLast->runAction(RepeatForever::create(seq));
                }
            }
            
            
        });
        auto seq = Sequence::create(action0, action1, action2, action3, action4, action5, RemoveSelf::create(), NULL);
        uiAni->getVirtualRenderer()->stopAllActions();
        uiAni->getVirtualRenderer()->runAction(seq);
        
        //
        _uiScrollTown->getInnerContainer()->runAction(Shake::create(1.5, 4));
    }
    
    
}

#pragma mark - click
void UILayoutTown::onClickTownArrow(cocos2d::Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_BUY_GOLD);
    
    auto uiContainerAniBack = _uiScrollTown->getInnerContainer()->getChildByName<ui::Layout*>("UI_CONTAINER_ANI_BACK");
    auto uiContainerAniFront = _uiScrollTown->getInnerContainer()->getChildByName<ui::Layout*>("UI_CONTAINER_ANI_FRONT");
    auto uiLayoutTownLand = _uiScrollTown->getInnerContainer()->getChildByName<ui::Layout*>("UI_CONTAINER_TOWN_GROUND");
    auto uiLayoutBuilding = _uiScrollTown->getInnerContainer()->getChildByName<ui::Layout*>("UI_CONTAINER_BUILDING");
    auto uiLayoutSatelliteLaunch = _uiScrollTown->getInnerContainer()->getChildByName<ui::Layout*>("UI_CONTAINER_SATELLITE_LAUNCH");
    
    auto uiBtn = dynamic_cast<ui::Button*>(sender);
    if ( uiBtn->getTag() == 1 )
    {
        _bTownPos = false;
        
        
        auto action = MoveTo::create(0.5, Vec2(0, 0));
        uiContainerAniBack->runAction(action->clone());
        uiContainerAniFront->runAction(action->clone());
        uiLayoutTownLand->runAction(action->clone());
        uiLayoutBuilding->runAction(action->clone());
        uiLayoutSatelliteLaunch->runAction(action->clone());
    }
    else
    {
        _bTownPos = true;
        
        auto sizeWidthScroll = _uiScrollTown->getContentSize().width;
        auto sizeWidthContainer = uiLayoutBuilding->getContentSize().width;
        auto sizeDiff = sizeWidthContainer - sizeWidthScroll;
        
        auto action = MoveTo::create(0.5, Vec2(-sizeDiff, 0));
        uiContainerAniBack->runAction(action->clone());
        uiContainerAniFront->runAction(action->clone());
        uiLayoutTownLand->runAction(action->clone());
        uiLayoutBuilding->runAction(action->clone());
        uiLayoutSatelliteLaunch->runAction(action->clone());
    }
    
    // draw
    drawTown();
}

void UILayoutTown::onClickBuildingUpgradeAsk(Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_BUY_GOLD);
    
    auto uiBtn = dynamic_cast<ui::Button*>(sender);

    E_TOWN eType = (E_TOWN)uiBtn->getTag();
    _eBuildingUpgrade = eType;
    
    int level = TownManager::getInstance()->getTownLevel(eType);
    
    if ( eType == E_TOWN::BUILDING_1 )
    {
        std::string price = TownManager::getInstance()->getTownPriceCoin(eType, level);
        std::string priceGem = MafUtils::toString(TownManager::getInstance()->getTownPriceGem(eType, level));
        
        std::string currencyGold = ItemsMoneyManager::getInstance()->getGold();
        std::string currencyGem = MafUtils::toString(ItemsMoneyManager::getInstance()->getGem());

        if ( MafUtils::compareStr(currencyGold, price) == false || MafUtils::compareStr(currencyGem, priceGem) == false )
        {
            if ( MafUtils::compareStr(currencyGold, price) == false )
            {
                auto popup = PopupCurrencyDetail::create(E_ITEMS::GOLD);
                popup->show();
            }
            else if ( MafUtils::compareStr(currencyGem, priceGem) == false )
            {
                auto popup = PopupCurrencyDetail::create(E_ITEMS::GEM);
                popup->show();
            }
        }
        else
        {
            onClickBuildingUpgrade(sender);
        }
    }
    else if ( eType == E_TOWN::SATELLITE )
    {
        std::string price = TownManager::getInstance()->getTownPriceCoin(eType, level);
        std::string priceGem = MafUtils::toString(TownManager::getInstance()->getTownPriceGem(eType, level));
        
        std::string currencyGold = ItemsMoneyManager::getInstance()->getGold();
        std::string currencyGem = MafUtils::toString(ItemsMoneyManager::getInstance()->getGem());
        
        int level  = TownManager::getInstance()->getTownLevel(eType);
        int satelliteStep = TownManager::getInstance()->getSatelliteStep();

        if ( satelliteStep < level / 100 )
        {
            PopupToast::show(GAME_TEXT("t_ui_bong_town_rokcet_error_2"));
            
            //
            onMoveSatellite();
        }
        else if ( MafUtils::compareStr(currencyGold, price) == false || MafUtils::compareStr(currencyGem, priceGem) == false )
        {
            if ( MafUtils::compareStr(currencyGold, price) == false )
            {
                auto popup = PopupCurrencyDetail::create(E_ITEMS::GOLD);
                popup->show();
            }
            else if ( MafUtils::compareStr(currencyGem, priceGem) == false )
            {
                auto popup = PopupCurrencyDetail::create(E_ITEMS::GEM);
                popup->show();
            }
        }
        else
        {
            onClickBuildingUpgrade(sender);
        }
    }
    else
    {
        std::string floor = GAME_TEXTFORMAT("t_ui_town_floor", level);
        
        std::string currentPrice = TownManager::getInstance()->getBuildingProgress(eType);
        std::string price = TownManager::getInstance()->getTownPriceCoin(eType, level);
        std::string priceAdd = price;
        priceAdd.pop_back();
        priceAdd.pop_back();
        
        std::string priceTotal = priceAdd;
        
        int percent = 0;
        for ( int i = 0; i < 100; i++ )
        {
            if ( !MafUtils::compareStr(currentPrice, priceTotal) )
            {
                break;
            }
            
            priceTotal = MafUtils::bigAddNum(priceTotal, priceAdd);
            percent++;
        }
        
        //int
        int expectLevel = TownManager::getInstance()->getBuildingUpgradeExpectLevel(eType, ItemsMoneyManager::getInstance()->getGold());
        int expectPercent = TownManager::getInstance()->getBuildingUpgradeExpectPercent(eType, ItemsMoneyManager::getInstance()->getGold());
        if ( level == expectLevel && percent == expectPercent )
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_bong_town_upgrade_error"));
            popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
            popup->show();
            return;
        }
        
        //
        std::string strMessage = GAME_TEXT("t_ui_msg_buy");
        strMessage = strMessage + "\n" +  floor + " " + StringUtils::toString(percent) + "% -> ";
        strMessage = strMessage + GAME_TEXTFORMAT("t_ui_town_floor", expectLevel) + " " + StringUtils::toString(expectPercent) + "%";
        
        auto popup = PopupDefault::create(TownManager::getInstance()->getTownName(eType), strMessage);
        popup->addButton(GAME_TEXT("t_ui_no"), true, nullptr);
        popup->addButton(GAME_TEXT("t_ui_yes"), false, CC_CALLBACK_1(UILayoutTown::onClickBuildingUpgrade, this));
        popup->show();
        
    }
}


void UILayoutTown::onClickBuildingUpgrade(Ref* sender)
{
    E_TOWN eType = _eBuildingUpgrade;
    
    int level = TownManager::getInstance()->getTownLevel(eType);
    int levelMax = TownManager::getInstance()->getTownLevelMax(eType);
    if ( level >= levelMax )
    {
        return;
    }
    
    if ( eType == E_TOWN::BUILDING_1 )
    {
        int result = TownManager::getInstance()->onTownUpgradeTower();
        if ( result != 0 )
        {
            return;
        }
    }
    else if ( eType == E_TOWN::SATELLITE )
    {
        int result = TownManager::getInstance()->onTownUpgradeSatellite();
        if ( result != 0 )
        {
            return;
        }
    }
    else
    {
        int result = TownManager::getInstance()->onTownUpgrade(eType, ItemsMoneyManager::getInstance()->getGold());
        if ( result != 0 )
        {
            return;
        }
    }
    
    // draw
    drawTownBG();
    drawBuilding();
    drawSatelliteLaunch();
    drawSatellite();
    drawUpgrade();
    
    drawTown();
    
    //
    if ( eType == E_TOWN::SATELLITE )
    {
        onMoveSatellite();
    }
}

void UILayoutTown::onClickBuildingReward(cocos2d::Ref* sender)
{
    auto uiButton = (ui::Button*)sender;
    
    auto objTownReward = TownManager::getInstance()->getObjectTownRewardLast(1);
    if ( objTownReward == nullptr )
    {
        return;
    }
    
    int level = TownManager::getInstance()->getTownLevel(E_TOWN::BUILDING_1);
    if ( objTownReward->getCondition() > level )
    {
        Vec2 posToast = uiButton->getParent()->convertToWorldSpace(uiButton->getPosition());
        posToast.y += 10;
            
        PopupToast::showImmediately(GAME_TEXTFORMAT("t_ui_bong_town_reward_condition", objTownReward->getCondition()), posToast.x, posToast.y);
        return;
    }
    
    PopupLoading::show();
    TownManager::getInstance()->requestReward(objTownReward->getType(), objTownReward->getCondition(), CC_CALLBACK_2(UILayoutTown::callbackRewardBuilding, this));
}

void UILayoutTown::onClickSatelliteOpen(cocos2d::Ref* sender)
{
    auto uiButton = (ui::Button*)sender;
    
    int levelBuilding = TownManager::getInstance()->getTownLevel(E_TOWN::BUILDING_1);
    int levelBuildingMax = TownManager::getInstance()->getTownLevelMax(E_TOWN::BUILDING_1);
    if ( levelBuilding < levelBuildingMax )
    {
        return;
    }
    
    //
    TownManager::getInstance()->setSatelliteTutorial(1);
    TownManager::getInstance()->saveData();
    
    // 시나리오 연출
    if(DialogManager::getInstance()->isViewDialog("BONG_TOWN_EXPANSION") == 0)
    {
        DialogManager::getInstance()->showDialog("BONG_TOWN_EXPANSION");
    }
    
    // draw
    drawUpgrade();
    drawTown();
    
    // move
    onMoveSatellite();
}

void UILayoutTown::onClickSatelliteReward(cocos2d::Ref* sender)
{
    auto uiButton = (ui::Button*)sender;
    
    int stepSatellite = TownManager::getInstance()->getSatelliteStep();
    int levelCondition = stepSatellite * 100 + 100;
    
    auto objTownReward = TownManager::getInstance()->getObjectTownReward(2, levelCondition);
    if ( objTownReward == nullptr )
    {
        return;
    }
    
    int level = TownManager::getInstance()->getTownLevel(E_TOWN::SATELLITE);
    if ( objTownReward->getCondition() > level )
    {
        return;
    }
    
    PopupLoading::show();
    TownManager::getInstance()->requestReward(objTownReward->getType(), objTownReward->getCondition(), CC_CALLBACK_2(UILayoutTown::callbackRewardSatellite, this));
}


#pragma mark - event
void UILayoutTown::onMoveBuilding()
{
    auto uiContainerAniBack = _uiScrollTown->getInnerContainer()->getChildByName<ui::Layout*>("UI_CONTAINER_ANI_BACK");
    auto uiContainerAniFront = _uiScrollTown->getInnerContainer()->getChildByName<ui::Layout*>("UI_CONTAINER_ANI_FRONT");
    auto uiLayoutTownLand = _uiScrollTown->getInnerContainer()->getChildByName<ui::Layout*>("UI_CONTAINER_TOWN_GROUND");
    auto uiLayoutBuilding = _uiScrollTown->getInnerContainer()->getChildByName<ui::Layout*>("UI_CONTAINER_BUILDING");
    auto uiLayoutSatelliteLaunch = _uiScrollTown->getInnerContainer()->getChildByName<ui::Layout*>("UI_CONTAINER_SATELLITE_LAUNCH");
    

    {
        uiContainerAniBack->setPositionX(0);
        uiContainerAniFront->setPositionX(0);
        uiLayoutTownLand->setPositionX(0);
        uiLayoutBuilding->setPositionX(0);
        uiLayoutSatelliteLaunch->setPositionX(0);
        
        auto pos = _uiScrollTown->getInnerContainerPosition();
        auto size = _uiScrollTown->getInnerContainerSize();
        pos.y = (size.height - _uiScrollTown->getContentSize().height) * -1;
        _uiScrollTown->stopAutoScroll();
        _uiScrollTown->setInnerContainerPosition(pos);
    }
    
    //
    _bTownPos = false;
    drawTown();
}

void UILayoutTown::onMoveSatellite()
{
    auto uiContainerAniBack = _uiScrollTown->getInnerContainer()->getChildByName<ui::Layout*>("UI_CONTAINER_ANI_BACK");
    auto uiContainerAniFront = _uiScrollTown->getInnerContainer()->getChildByName<ui::Layout*>("UI_CONTAINER_ANI_FRONT");
    auto uiLayoutTownLand = _uiScrollTown->getInnerContainer()->getChildByName<ui::Layout*>("UI_CONTAINER_TOWN_GROUND");
    auto uiLayoutBuilding = _uiScrollTown->getInnerContainer()->getChildByName<ui::Layout*>("UI_CONTAINER_BUILDING");
    auto uiLayoutSatelliteLaunch = _uiScrollTown->getInnerContainer()->getChildByName<ui::Layout*>("UI_CONTAINER_SATELLITE_LAUNCH");
    
    {
        auto sizeWidthScroll = _uiScrollTown->getContentSize().width;
        auto sizeWidthContainer = uiLayoutBuilding->getContentSize().width;
        auto sizeDiff = sizeWidthContainer - sizeWidthScroll;
        
        uiContainerAniBack->setPositionX(-sizeDiff);
        uiContainerAniFront->setPositionX(-sizeDiff);
        uiLayoutTownLand->setPositionX(-sizeDiff);
        uiLayoutBuilding->setPositionX(-sizeDiff);
        uiLayoutSatelliteLaunch->setPositionX(-sizeDiff);
        
        //
        auto pos = _uiScrollTown->getInnerContainerPosition();
        pos.y = 0;
        _uiScrollTown->stopAutoScroll();
        _uiScrollTown->setInnerContainerPosition(pos);
    }
    
    //
    _bTownPos = true;
    drawTown();
}
