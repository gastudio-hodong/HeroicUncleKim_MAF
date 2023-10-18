APP_STL := c++_static

APP_CPPFLAGS := -frtti -DCC_ENABLE_CHIPMUNK_INTEGRATION=1 -std=c++11 -fsigned-char -Wno-extern-c-compat
APP_LDFLAGS := -latomic

ifeq ($(NDK_DEBUG),1)
  APP_ABI := armeabi-v7a
else
  APP_ABI := armeabi-v7a arm64-v8a
endif
APP_SHORT_COMMANDS := true


ifeq ($(NDK_DEBUG),1)
  APP_CPPFLAGS += -DCOCOS2D_DEBUG=1
  APP_OPTIM := debug
else
  APP_CPPFLAGS += -DCOCOS2D_DEBUG=0
  APP_CPPFLAGS += -DNDEBUG
  APP_OPTIM := release
endif
