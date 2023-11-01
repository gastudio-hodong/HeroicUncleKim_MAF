PATH_CURRENT="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
PATH_ANDROID=$PATH_CURRENT/../../proj.android

#
TARGET=RELEASE #DEV or RELEASE
BUNDLE=YES # YES or NO

# Path android
cd $PATH_ANDROID


# Build Output apk
if [ $TARGET = DEV ]
then
	./gradlew :MyGame:assembleDebug
elif [ $TARGET = RELEASE ]
then
	./gradlew :MyGame:assembleRelease
    if [ $BUNDLE = YES ]
    then
        ./gradlew :MyGame:bundleRelease
        ./gradlew :MyGame:uploadCrashlyticsSymbolFileRelease
    fi
fi


# Build Output File Move
if [ ! -d $PATH_ANDROID/../bin/ ]
then
    mkdir $PATH_ANDROID/../bin/
fi

if [ $TARGET = DEV ]
then
    # apk
    if [ -f $PATH_ANDROID/../bin/MyGame-debug.apk ]; then
        rm $PATH_ANDROID/../bin/MyGame-debug.apk
    fi
    
    cd $PATH_ANDROID/app/build/outputs/apk/debug/
    cp -rf MyGame-debug.apk $PATH_ANDROID/../bin/MyGame-debug.apk
elif [ $TARGET = RELEASE ]
then
    # apk
    if [ -f $PATH_ANDROID/../bin/MyGame-release.apk ]; then
        rm $PATH_ANDROID/../bin/MyGame-release.apk
    fi
    cd $PATH_ANDROID/app/build/outputs/apk/release/
    cp -rf MyGame-release.apk $PATH_ANDROID/../bin/MyGame-release.apk
    
    # bundle
    if [ $BUNDLE = YES ]
    then
        if [ -f $PATH_ANDROID/../bin/MyGame-release.aab ]; then
            rm $PATH_ANDROID/../bin/MyGame-release.aab
        fi
        
        cd $PATH_ANDROID/app/build/outputs/bundle/release/
        cp -rf MyGame-release.aab $PATH_ANDROID/../bin/MyGame-release.aab
    fi
fi
