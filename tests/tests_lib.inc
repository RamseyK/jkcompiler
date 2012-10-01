#
# This is a shell library containing common testing logic
#
. ../tests_lib_shared_auto.inc


function test_diff()
{
    SOURCE_FILE=$1
    FLAGS=$2
    $BINARY $FLAGS < $SOURCE_FILE > $SOURCE_FILE.output 2>&1
    RETVAL=$?

    if [ $RETVAL == 139 ]; then
	# The compiler crashed
	echo FAILED
	exit 0
    fi
    cat $SOURCE_FILE.output

    # If the output does not match the expected, the test failed
    diff -u $SOURCE_FILE.expected $SOURCE_FILE.output || test_diff_failed
    echo PASSED
}


function test_diff_sem()
{
    SOURCE_FILE=$1
    FLAGS=$2

    test_diff $SOURCE_FILE "$FLAGS -exitsem"
}

function test_diff_ir()
{
    SOURCE_FILE=$1
    FLAGS=$2

    test_diff $SOURCE_FILE "$FLAGS -ir"
}


function test_diff_gen()
{
    SOURCE_FILE=$1
    FLAGS=$2
    rm out.c
    $BINARY $FLAGS < $SOURCE_FILE && gcc -g out.c && ./a.out > $SOURCE_FILE.output 2>&1
    RETVAL=$?

    if [ $RETVAL == 139 ]; then
	# The compiler crashed
	echo FAILED
	exit 0
    fi
    cat $SOURCE_FILE.output

    # If the output does not match the expected, the test failed
    diff -u $SOURCE_FILE.expected $SOURCE_FILE.output || test_diff_failed
    echo PASSED
}


function test_diff_failed()
{
    echo FAILED
    exit 0
}
