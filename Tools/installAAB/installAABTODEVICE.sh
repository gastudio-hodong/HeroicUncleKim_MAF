#!/bin/sh
# One step wrapper to install aab to connected device, it downloads bundletool if its missing and signs the app with android debug certifiate.
#
# Usage: installFromAAB install-aab <local-aab-path>

BASENAME="${1##*/}"
STORE_FILE=../../proj.android/app/maf.keystore
STORE_PASS="pass:tksfla0712"
KEY_ALIAS="maf_lite"
KEY_PASS="pass:tksfla0712"

BUNDLETOOL_PATH=~/.android/bundletool-all-0.10.3.jar
BUNDLETOOL='java -jar $BUNDLETOOL_PATH'
BUNDLETOOL_DOWNLOAD_URL='https://github.com/google/bundletool/releases/download/0.10.3/bundletool-all-0.10.3.jar'

TMP_APKS_PATH="./tmp/$BASENAME.apks"
AAB_PATH=$1

function die () {
    echo
    echo "$*"
    echo
    exit 1
}

function error () {
    printf "\e[01;31m $1 \e[0m"
}

function usage {
    echo "Usage install-aab <local-aab-path>"    
    echo "\n"
    echo "eg. install-aab ~/Downloads/YourApp-Version.aab"
}

if [[ $1 == "" ]]; then
    usage
    die
fi

# Determine the Java command to use to start the JVM.
if [ -n "$JAVA_HOME" ] ; then
    if [ -x "$JAVA_HOME/jre/sh/java" ] ; then
        # IBM's JDK on AIX uses strange locations for the executables
        JAVACMD="$JAVA_HOME/jre/sh/java"
    else
        JAVACMD="$JAVA_HOME/bin/java"
    fi
    if [ ! -x "$JAVACMD" ] ; then
        die "ERROR: JAVA_HOME is set to an invalid directory: $JAVA_HOME

Please set the JAVA_HOME variable in your environment to match the
location of your Java installation."
    fi
else
    JAVACMD="java"
    which java >/dev/null 2>&1 || die "ERROR: JAVA_HOME is not set and no 'java' command could be found in your PATH.

Please set the JAVA_HOME variable in your environment to match the
location of your Java installation."
fi

if [ ! -f $BUNDLETOOL_PATH ]; then    
    if hash curl 2>/dev/null; then
        echo "Trying to find latest bundletool version"
        BUNDLETOOL_DOWNLOAD_URL=$(curl -s --connect-timeout 30 https://api.github.com/repos/google/bundletool/releases/latest | grep browser_download_ur | cut -d '"' -f 4)

        echo "Downloading bundletool from $BUNDLETOOL_DOWNLOAD_URL"

        if curl -# --connect-timeout 30 -L $BUNDLETOOL_DOWNLOAD_URL --output "$BUNDLETOOL_PATH.part" ; then 
            mv "$BUNDLETOOL_PATH.part" $BUNDLETOOL_PATH
        else
            error "Unable to download bundletool, download it from 
                    $BUNDLETOOL_PATH and store it in $BUNDLETOOL_PATH"
            die
        fi                      
    else
        error "Unable to download bundletool, download it from 
                    $BUNDLETOOL_PATH and store it in $BUNDLETOOL_PATH"
        die
    fi
fi

if [ ! -f $STORE_FILE ]; then
    echo "Creating keystore $STORE_FILE"
    if hash keytool 2>/dev/null; then            
        keytool -genkey -v -keystore $STORE_FILE -storepass $STORE_PASS -alias $KEY_ALIAS \
            -keypass $KEY_PASS -keyalg RSA -keysize 2048 -validity 10000 \
            -dname "C=US, O=Android, CN=Android Debug"
    else
        error "Unable to create $STORE_FILE"
        exit 0;
    fi
fi

echo "Creating $TMP_APKS_PATH from $AAB_PATH"
$JAVACMD -jar $BUNDLETOOL_PATH build-apks --bundle=$AAB_PATH --ks=$STORE_FILE --ks-pass=$STORE_PASS --ks-key-alias=$KEY_ALIAS \
    --key-pass=$KEY_PASS --output=$TMP_APKS_PATH --connected-device --overwrite && \

echo "Installing $TMP_APKS_PATH" && \
$JAVACMD -jar $BUNDLETOOL_PATH install-apks --apks=$TMP_APKS_PATH
