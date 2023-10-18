//
//  TextTyping.h
//
//

#ifndef TextTyping_h
#define TextTyping_h

#include <cocos2d.h>

NS_CC_BEGIN

class TextTyping : public cocos2d::ActionInterval
{
public:
    enum class E_TYPE{
        NUMBER,
        TIME_M_S,
    };
    
public:
    static TextTyping* create(float duration);
    
    virtual TextTyping* clone() const override;
    virtual TextTyping* reverse() const override;
    virtual void startWithTarget(cocos2d::Node *target) override;
    /**
     * @param time in seconds
     */
    virtual void update(float time) override;
    
CC_CONSTRUCTOR_ACCESS:
    TextTyping(void);
    virtual ~TextTyping(void);
    
    bool initWithDuration(float duration);
    
protected:
    int _length;
    
    int _idxStart;
    int _idxEnd;
};

NS_CC_END

#endif
