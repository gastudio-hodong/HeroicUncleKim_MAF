//
//  MafNode.h
//  HexagonSnake
//
//  Created by SanHeo on 2014. 9. 12..
//
//

#ifndef __HexagonSnake__MafNode__
#define __HexagonSnake__MafNode__

#include "cocos2d.h"
#include "extensions/cocos-ext.h"

namespace MafNode {
    
    static void setNewScale(cocos2d::Node &lb, float fontSize);
    

    //_lbTime = Label::createWithBMFont(GAME_BM_FONT_AL, "0");
    static cocos2d::Label* createBmFont(const std::string& text,const std::string& bmfontFilePath,float fontSize){
        //font default 28으로 제작.
        cocos2d::Label *lb = cocos2d::Label::createWithBMFont(bmfontFilePath, text);
        setNewScale(*lb,fontSize);
//        lb->setScale(newSize);
        
        return lb;
    }
    
    static void setNewScale(cocos2d::Node &lb,float fontSize){
    
        float size = 28;
        float newSize = fontSize/size;
        lb.setScale(newSize);
    }
    
    
    class MafMenu;
    class MafMenuItem;
    class MafMenuItemSprite;
    
    class MafMenu : public cocos2d::Menu
    {
    public:

        static MafMenu* create();
        virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
        virtual void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
        virtual void onTouchCancelled(cocos2d::Touch* touch, cocos2d::Event* event);
        virtual void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
        
    protected:
        bool initWithArray(const cocos2d::Vector<MafMenuItem*>& arrayOfItems);
        MafMenuItem* getItemForTouch(cocos2d::Touch * touch);
       
        /** item activate lock */
        static int64_t _activate;
        cocos2d::Vec2 _touchesStart;
        cocos2d::Vec2 _touchesEnd;
        
        cocos2d::EventListenerTouchOneByOne* _touchListener;
    };
    
    class MafMenuItem : public cocos2d::MenuItem
    {
    public:
        MafMenuItem();
        static MafMenuItem* create();
        
        void setTouchRect(cocos2d::Size size);
        cocos2d::Rect getTouchRect();
    protected:
        cocos2d::Size _touchSize;
    };
    
    
    
    class MafMenuItemSprite : public MafMenuItem
    {
    public:

        /** creates a menu item with a normal and selected image with a callable object */
        static MafMenuItemSprite * create(cocos2d::Node* normalSprite, cocos2d::Node* selectedSprite, const cocos2d::ccMenuCallback& callback);
        static MafMenuItemSprite * create(cocos2d::Node* normalSprite, cocos2d::Node* selectedSprite, cocos2d::Node *disabledSprite,const cocos2d::ccMenuCallback& callback);
        /** Gets the image used when the item is not selected */
        inline Node* getNormalImage() const { return _normalImage; };
        /** Sets the image used when the item is not selected */
        void setNormalImage(Node* image);
        /** Gets the image used when the item is selected */
        inline Node* getSelectedImage() const { return _selectedImage; };
        /** Sets the image used when the item is selected */
        void setSelectedImage(Node* image);
        /** Gets the image used when the item is disabled */
        inline Node* getDisabledImage() const { return _disabledImage; };
        /** Sets the image used when the item is disabled */
        void setDisabledImage(Node* image);
        
        /**
         @since v0.99.5
         */
        virtual void selected();
        virtual void unselected();
        virtual void setEnabled(bool bEnabled);
        
        //long click
        void setLongClick(bool bLong);
        void scheduleLongSelected(float dt);
        
    CC_CONSTRUCTOR_ACCESS:
        MafMenuItemSprite()
        :_normalImage(nullptr)
        ,_selectedImage(nullptr)
        ,_disabledImage(nullptr)
        
        ,_bLongClick(false)
        ,_bLongSchedule(false)
        ,_nLongTime(0.05)
        ,_nLongTimeMax(0.05)
        {}
        
        /** initializes a menu item with a normal, selected  and disabled image with a callable object */
        bool initWithNormalSprite(cocos2d::Node* normalSprite, cocos2d::Node* selectedSprite, cocos2d::Node* disabledSprite, const cocos2d::ccMenuCallback& callback);
        
    protected:
        virtual void updateImagesVisibility();
        
        /** the image used when the item is not selected */
        cocos2d::Node* _normalImage;
        /** the image used when the item is selected */
        cocos2d::Node* _selectedImage;
        /** the image used when the item is disabled */
        cocos2d::Node* _disabledImage;
        
        bool _bLongClick;
        bool _bLongSchedule;
        double _nLongTime;
        double _nLongTimeMax;
        
    private:
        CC_DISALLOW_COPY_AND_ASSIGN(MafMenuItemSprite);
    };
    
    
    class CC_DLL MafMenuItemLabel : public MafMenuItem
    {
    public:
        /** creates a MenuItemLabel with a Label and a callback */
        static MafMenuItemLabel * create(Node*label, const cocos2d::ccMenuCallback& callback);
        
        /** creates a MenuItemLabel with a Label. Target and selector will be nil */
        static MafMenuItemLabel* create(Node *label);
        
        /** sets a new string to the inner label */
        void setString(const std::string& label);
        
        /** Gets the color that will be used to disable the item */
        inline const cocos2d::Color3B& getDisabledColor() const { return _disabledColor; };
        
        /** Sets the color that will be used to disable the item */
        inline void setDisabledColor(const cocos2d::Color3B& color) { _disabledColor = color; };
        
        /** Gets the label that is rendered. */
        inline cocos2d::Node* getLabel() const { return _label; };
        
        /** Sets the label that is rendered. */
        void setLabel(cocos2d::Node* node);
        
        // Overrides
        virtual void activate() override;
        virtual void selected() override;
        virtual void unselected() override;
        virtual void setEnabled(bool enabled) override;
        
    CC_CONSTRUCTOR_ACCESS:
        /**
         * @js ctor
         */
        MafMenuItemLabel()
        : _originalScale(0.0)
        , _label(nullptr)
        {}
        /**
         * @js NA
         * @lua NA
         */
        virtual ~MafMenuItemLabel();
        
        /** initializes a MenuItemLabel with a Label, target and selector */
        bool initWithLabel(cocos2d::Node* label, const cocos2d::ccMenuCallback& callback);
        
        /** initializes a MenuItemLabel with a Label, target and selector */
        CC_DEPRECATED_ATTRIBUTE bool initWithLabel(cocos2d::Node* label, cocos2d::Ref* target, cocos2d::SEL_MenuHandler selector);
        
    protected:
        cocos2d::Color3B    _colorBackup;
        float      _originalScale;
        
        /** the color that will be used to disable the item */
        cocos2d::Color3B _disabledColor;
        /** Label that is rendered. It can be any Node that implements the LabelProtocol */
        cocos2d::Node* _label;
        
    private:
        CC_DISALLOW_COPY_AND_ASSIGN(MafMenuItemLabel);
    };
    
}










#endif /* defined(__HexagonSnake__MafNode__) */
