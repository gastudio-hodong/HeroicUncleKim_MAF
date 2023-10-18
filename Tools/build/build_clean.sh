PATH_CURRENT="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
PATH_ANDROID=$PATH_CURRENT/../../proj.android

# Path android
cd $PATH_ANDROID


# Build clean
./gradlew :commafiab:clean
./gradlew :libcocos2dx:clean
./gradlew :MyGame:clean
    
# delete .cxx
rm -rf ./app/.cxx
