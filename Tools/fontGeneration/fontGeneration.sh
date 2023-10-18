

function draw_progress_bar()
{
    PROGRESS_BAR_WIDTH=50  # progress bar length in characters

    # Arguments: current value, max value, unit of measurement (optional)
    local __value=$1
    local __max=$2
    local __unit=${3:-""}  # if unit is not supplied, do not display it

    # Calculate percentage
    if (( $__max < 1 )); then __max=1; fi  # anti zero division protection
    local __percentage=$(( 100 - ($__max*100 - $__value*100) / $__max ))

    # Rescale the bar according to the progress bar width
    local __num_bar=$(( $__percentage * $PROGRESS_BAR_WIDTH / 100 ))

    # Draw progress bar
    printf "["
    for b in $(seq 1 $__num_bar); do printf "#"; done
    for s in $(seq 1 $(( $PROGRESS_BAR_WIDTH - $__num_bar ))); do printf " "; done
    printf "] $__percentage%% ($__value / $__max $__unit)\r"
}

function getDuplicateletters()
{
    strText=$1
    strFileName=$2

    strFileContents=$(<"$strFileName")
    nFileLangth=${#strFileContents}

    for ((nPos=0; nPos<$nFileLangth; nPos++)) do
        strWord="${strFileContents:$nPos:1}"
        if [[ "$strText" != *"$strWord"* ]]; then
            strText=$strText$strWord
        fi

        #progress
        draw_progress_bar $nPos $nFileLangth "bytes"
    done

    retrun_string=$strText
}

retrun_string=""
function main()
{
    list=(
        "text_language_other.txt"
        "text_language_ko.txt"
        "text_language_jp.txt"
        "text_language_cn.txt"
        "text_language_tw.txt"
        "text_game.txt"
    )
    listGame="text_game.txt"

    #============================================
    # text game Duplicate
    #============================================
    printf "Start : Game Duplicate letters\n"

    # text game duplicate
    strAll=""
    getDuplicateletters "$strAll" "$listGame"
    strAll=$retrun_string

    # text game file save
    echo $strAll > $listGame

    printf "\n"
    printf "End : Game Duplicate letters\n"


    #============================================
    # text all Duplicate
    #============================================
    strAll=""
    for strFileName in ${list[@]}; do
        getDuplicateletters "$strAll" "$strFileName"
        strAll=$retrun_string

        printf "\n"
        printf "End : FileName(%s)\n" $strFileName
    done

    #file save
    echo $strAll > ./text_all.txt


    #============================================
    # Generation font
    #============================================
    pyftsubset "NotoSansCJKjp-Medium.otf" \
    --output-file="./NotoSansCJKjp-Medium_small.otf" \
    --text-file="text_all.txt" \
    --layout-features='*' \
    --glyph-names \
    --symbol-cmap \
    --legacy-cmap \
    --notdef-glyph \
    --notdef-outline \
    --recommended-glyphs \
    --name-legacy \
    --drop-tables= \
    --name-IDs='*' \
    --name-languages='*'
}

#
main
