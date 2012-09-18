#!/bin/sh


if [ $# -le 1 ]; then
    echo "usage: $0 <sed_replace_regex> [files_list]"
    exit -1
fi

SED_REPLACE_REGEX=$1
shift
FILES="$@"


TEMP_DIR=./

common_replace_multiline_sed()
{
    FILENAME=$1
    MULTILINE_REPLACE_SED=$2

    echo "$MULTILINE_REPLACE_SED" > $TEMP_DIR/multiline_replace.$$
    cat $FILENAME | sed -f $TEMP_DIR/multiline_replace.$$ > $TEMP_DIR/multiline_replace.output.$$

    cat $TEMP_DIR/multiline_replace.output.$$ > $FILENAME
    rm $TEMP_DIR/multiline_replace.output.$$
    rm $TEMP_DIR/multiline_replace.$$
}


echo regex is $SED_REPLACE_REGEX
for file in $FILES; do
    echo $file
    common_replace_multiline_sed $file "$SED_REPLACE_REGEX"
done
