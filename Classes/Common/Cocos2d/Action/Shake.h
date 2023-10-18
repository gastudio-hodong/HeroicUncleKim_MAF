//
//  Shake.h
//
//

#ifndef Shake_h
#define Shake_h

#include <cocos2d.h>

NS_CC_BEGIN

class Shake : public ActionInterval
{
public:
    static Shake* create(float duration, float strength );
    static Shake* create(float duration, float strength_x, float strength_y );
    
    virtual Shake* clone() const override;
    virtual Shake* reverse() const override;
    virtual void startWithTarget(cocos2d::Node *target) override;
    virtual void stop() override;
    /**
     * @param time in seconds
     */
    virtual void update(float time) override;
    
CC_CONSTRUCTOR_ACCESS:
    Shake(void);
    virtual ~Shake(void);
    
    bool initWithDuration(float duration, float strength_x, float strength_y );
    
protected:
    float _initial_x, _initial_y;
    float _strength_x, _strength_y;
};

NS_CC_END

#endif
