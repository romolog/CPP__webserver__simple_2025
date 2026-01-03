#! /bin/bash

if [[ -z "$1" ]] || ( [[ "$1" != "-DNDEBUG=ON" ]] && [[ "$1" != "-DNDEBUG=OFF" ]] ); then
    echo -e "Usage: $0  <arg: -DNDEBUG=ON / -DNDEBUG=OFF>"
    exit 1
fi


### LOG FILES ######################################################################

LOG_="./tests/test.log"
BUFF_="./tests/buff.log"
BUFF_SERV_="./tests/buff_serv.log"
VAR_BUFF_=./tests/var_buff.log
touch $LOG_;
touch $BUFF_;
touch $BUFF_SERV_;
touch $VAR_BUFF_;

### UTILITY VARIABLES ######################################################################

BLK_BOLD_="\e[1;30m";
RED_="\033[0;31m";
RED_BOLD_="\033[1;31m";
GRN_="\033[0;92m";
BRN_BOLD_="\e[1;33m";
YEL_="\e[0;93m";
YEL_BOLD_="\e[1;93m";
MAG_="\e[0;35m";
CYN_="\e[0;36m";
WHT_BOLD_="\e[1;37m";

BG_BLK_="\e[40m";
BG_GRY_="\e[100m";
BG_GRN_="\e[42m";

ULN_="\033[4m";

DFT_="\033[0m";

ERR_="$RED_BOLD_\tERROR: $RED_" 
ERR_END_="$DFT_\n";
BUILD_="build";
PROG_="./$BUILD_/webserv";

VALGRIND_FLAGS_=" --leak-check=full --show-leak-kinds=all --track-fds=all --trace-children=yes";

### BUILD ######################################################################
echo -e "\n$WHT_BOLD_$BG_GRY_\t CMAKE WEBSERVER $DFT_\n";

((cmake -B $BUILD_  $1 && cmake --build $BUILD_ ) || (echo -e "$ERR_ cmake failed $ERR_END_" && false)) || exit 1;

### TEST CONFIG ######################################################################
echo -e "\n\t$WHT_BOLD_$BG_GRY_ TEST CONFIG: $DFT_\n";


echo $(date) >$LOG_;
echo -e "--------------------------------\n" >>$LOG_;
echo $(date) >$BUFF_;
echo -e "--------------------------------\n" >>$BUFF_;


### TEST FUNCTION ######################################################################
NUM_=0;
call_test_conf()
{
	NUM_=$(($NUM_ + 1));
	TEST_="$DFT_$NUM_:$DFT_ $TEST_DESCR_\n\t$MAG_$CALL_$DFT_";

	{ $CALL_; } >& $BUFF_;

	SEG_FAULT_=$(cat $BUFF_ | grep fault);
	if [[ "$SEG_FAULT_" == *"fault"* ]]; then
		echo -en $YEL_ 
		echo -e $TEST_
		echo -e $RED_ "SEG FAULT\n" $DFT_
		exit 1
	fi

	TARGET_=Error;
	RES_=$($CALL_ 2>&1 | grep $TARGET_);
	LEAK_=$(valgrind --leak-check=full $CALL_ 2>&1 | grep "in use at exit");
	FDS_=$(valgrind --track-fds=yes $CALL_ 2>&1 | grep "FILE DESCRIPTORS:");

	echo -e "\nTEST #" $NUM_ "\n\t"$TEST_DESCR_ >> $LOG_;
	echo -en "\t\tprog output:\n\t\t\t" >> $LOG_;
	echo $RES_ >> $LOG_;
	echo -en "\t\tprog exit code: " >> $LOG_;
	$CALL_ 2>/dev/null && echo $? >> $LOG_ || echo $? >> $LOG_;

	echo -e $TEST_;

	echo -en $YEL_ "\tOutput:\t"$DFT_;
	if [[ "$RES_" = *"Error"* ]]; then
		echo -e $GRN_ "OK" $DFT_ 
	else
		echo -e $RED_ Error $DFT_
	fi

	echo -en $YEL_ "\tFD:\t"$DFT_;
	if [[ "$FDS_" == *"FILE DESCRIPTORS: 3 open (3 std) at exit." ]]; then
		echo -e $GRN_ "OK" $DFT_
	elif [[ "$FDS_" = *"FILE DESCRIPTORS: 2 open (2 std) at exit." ]]; then
		echo -e $GRN_ "OK" $DFT_
	elif [[ "$FDS_" = *"FILE DESCRIPTORS: 1 open (1 std) at exit." ]]; then
		echo -e $GRN_ "OK" $DFT_	 
	else
		echo -e $RED_ "Error: " $DFT_ $FDS_ 
	fi

	echo -en $YEL_ "\tLeaks:\t"$DFT_;
	if [[ "$LEAK_" = *"in use at exit: 0 bytes in 0 blocks"* ]]; then
		echo -e $GRN_ "NONE" $DFT_	
	else
		echo -e $RED_ "Error: " $DFT_ $LEAK_
	fi
}



### TEST ######################################################################
#	test wrong number of args
CALL_="$PROG_ ./tests/test.conf ./tests/test.conf";
TEST_DESCR_="Two valid configuration files are passed: ";
call_test_conf;

### TEST ######################################################################
#	test invalid configuration files
TTL_=0
while IFS= read -r CONF_FILE; do
	CALL_="$PROG_ $CONF_FILE";
	TEST_DESCR_="Invalid configuration file is passed:";
	call_test_conf;
	TTL_=$((TTL_+1))
done < <(find "./tests/confs_invalid" -type f -name "*.conf")

echo -e $MAG_"\n\tTotal invalid confs tested: $TTL_\n" $DFT_;

### TEST SERVER ######################################################################
echo -e "\n\t$WHT_BOLD_$BG_GRY_ TEST SERVER RUN: $DFT_\n";

### TEST ######################################################################
#	test one line configuration file

CALL_="$PROG_ ./tests/confs_valid/one_line.conf";
TEST_DESCR_="VISUAL CHECK: click a link below and check one line configuration file supports a website (duration: 20 sec)";

NUM_=$(($NUM_ + 1));
TEST_="$DFT_$NUM_:$DFT_ $TEST_DESCR_\n\t$MAG_$CALL_$DFT_\n\n\thttp://127.0.0.1:2442$DFT_\n";

echo -e $TEST_;

2>/dev/null 1>/dev/null $CALL_ & echo $! > ./tests/webserv.pid;
PID_=$(cat ./tests/webserv.pid);
sleep 20;
kill $PID_;


### TEST ######################################################################
#	test multiple websites visually in browser

CALL_="$PROG_";
TEST_DESCR_="VISUAL CHECK: click links below and check if 2 websites are available (duration: 30 sec)";

NUM_=$(($NUM_ + 1));
TEST_="$DFT_$NUM_:$DFT_ $TEST_DESCR_\n\t$MAG_$CALL_$DFT_\n\n\thttp://127.0.0.1:4242\n\thttp://127.0.0.1:4343$DFT_\n";

echo -e $TEST_;

2>/dev/null 1>/dev/null $CALL_ & echo $! > ./tests/webserv.pid;
PID_=$(cat ./tests/webserv.pid);
sleep 30;
kill $PID_;

### TEST FUNCTION ######################################################################

call_test_fds_leaks()
{
	LEAK_=$(cat $BUFF_SERV_ | grep "in use at exit");
	echo -en $YEL_ "\tLeaks:\t\t"$DFT_;
	if [[ "$LEAK_" = *"in use at exit: 0 bytes in 0 blocks"* ]]; then
		echo -e $GRN_ "NONE" $DFT_	
	else
		echo -e $RED_ "Error: " $DFT_ $LEAK_
	fi

	FDS_=$(cat $BUFF_SERV_ | grep "FILE DESCRIPTORS:");
	echo -en $YEL_"\tFDS:\t\t"$DFT_;
	if [[ "$FDS_" == *"FILE DESCRIPTORS: 3 open (3 std) at exit." ]]; then
		echo -e $GRN_ "OK" $DFT_
	elif [[ "$FDS_" = *"FILE DESCRIPTORS: 2 open (2 std) at exit." ]]; then
		echo -e $GRN_ "OK" $DFT_
	elif [[ "$FDS_" = *"FILE DESCRIPTORS: 1 open (1 std) at exit." ]]; then
		echo -e $GRN_ "OK" $DFT_	 
	else
		echo -e $RED_ "Error: " $DFT_ $FDS_ 
	fi
}

### TEST FUNCTION ######################################################################
call_test_get()
{
	CALL_="valgrind $VALGRIND_FLAGS_ $PROG_";
	2>$BUFF_SERV_ 1>/dev/null $CALL_ & echo $! > ./tests/webserv.pid;
	PID_=$(cat ./tests/webserv.pid);
	sleep 3;

	NUM_=$(($NUM_ + 1));
	TEST_="$DFT_$NUM_:$DFT_ $TEST_DESCR_\n\t$MAG_$CURL_$DFT_";

	{ $CURL_; } >$BUFF_ 2>$BUFF_;

	IFS=' ';
	<$BUFF_ read -a WORDS_;

	STATUS_CODE_="${WORDS_[1]}";
	CONTENT_=$(grep -i '>>|' $BUFF_);

	echo -e $TEST_;

	echo -en $YEL_ "\tStatus code:\t"$DFT_;
	if [ "1" -lt "${#WORDS_[@]}" ] && [ "$STATUS_CODE_" == $EXPECTED_CODE_ ]; then
		echo -e $GRN_ "OK\t$STATUS_CODE_" $DFT_
	else
		echo -e $RED_ "Error:\t" $DFT_ $STATUS_CODE_
	fi

	echo -en $YEL_"\tContent:\t"$DFT_;
	if [[ -n "$CONTENT_" ]] && [[ -n "$EXPECTED_CONTENT_" ]] && [[ "$CONTENT_" == *"$EXPECTED_CONTENT_"* ]]; then
		echo -e $GRN_ "OK\t$CONTENT_" $DFT_
	elif [[ -z "$CONTENT_" ]] && [[ -z "$EXPECTED_CONTENT_" ]]; then
		echo -e $GRN_ "OK" $DFT_
	else
		echo -e $RED_ "Error:\t" $DFT_ $CONTENT_
	fi

	kill $PID_;
	sleep 3;
	call_test_fds_leaks;

}

### TEST GET ######################################################################
#	test by curl

TARGET_="/";
CURL_="curl -isX GET http://127.0.0.1:2442$TARGET_";
TEST_DESCR_="GET $TARGET_";
EXPECTED_CODE_=200;
EXPECTED_CONTENT_="DEFAULT ROOT PAGE";
call_test_get;

TARGET_="/request_to_be_routed/";
CURL_="curl -isX GET http://127.0.0.1:2442$TARGET_";
TEST_DESCR_="GET $TARGET_ -- should be routed to the default page inside ./routed_response/ folder";
EXPECTED_CODE_=200;
EXPECTED_CONTENT_="ROUTED RESPONSE";
call_test_get;

TARGET_="/non_existing_dir/";
CURL_="curl -isX GET http://127.0.0.1:2442$TARGET_";
TEST_DESCR_="GET $TARGET_";
EXPECTED_CODE_=404;
EXPECTED_CONTENT_="NOT FOUND";
call_test_get;

TARGET_="/special_files/non_existing_file";
CURL_="curl -isX GET http://127.0.0.1:2442$TARGET_";
TEST_DESCR_="GET $TARGET_";
EXPECTED_CODE_=404;
EXPECTED_CONTENT_="NOT FOUND";
call_test_get;

TARGET_="/special_files/dir";
CURL_="curl -isX GET http://127.0.0.1:2442$TARGET_";
TEST_DESCR_="GET $TARGET_ -- existing directory is requested, without autoindex or default index file";
EXPECTED_CODE_=404;
EXPECTED_CONTENT_="NOT FOUND";
call_test_get;

TARGET_="/special_files/no_permission_file";
chmod 000 ./tests$TARGET_;
CURL_="curl -isX GET http://127.0.0.1:2442$TARGET_";
TEST_DESCR_="GET $TARGET_";
EXPECTED_CODE_=403;
EXPECTED_CONTENT_="FORBIDDEN";
call_test_get;

TARGET_="/special_files/empty_file";
rm -rf ./tests$TARGET_;
touch ./tests$TARGET_;
CURL_="curl -isX GET http://127.0.0.1:2442$TARGET_";
TEST_DESCR_="GET $TARGET_";
EXPECTED_CODE_=200;
EXPECTED_CONTENT_="";
call_test_get;


### TEST FUNCTION ######################################################################
call_test_post()
{
	CALL_="valgrind $VALGRIND_FLAGS_ $PROG_";
	2>$BUFF_SERV_ 1>/dev/null $CALL_ & echo $! > ./tests/webserv.pid;
	PID_=$(cat ./tests/webserv.pid);
	sleep 3;

	NUM_=$(($NUM_ + 1));
	TEST_="$DFT_$NUM_:$DFT_ $TEST_DESCR_\n\t$MAG_$CURL_$DFT_";

	{ $CURL_; } >& $BUFF_;

	IFS=' ';
	cat <$BUFF_ | grep HTTP >$VAR_BUFF_;
	<$VAR_BUFF_ read -a WORDS_;
	STATUS_CODE_="${WORDS_[1]}";

	TARGET_FILE_=./tests$TARGET_
	TARGET_FILE_EXIST_="EXIST";
	if [ ! -f "$TARGET_FILE_" ]; then
		TARGET_FILE_EXIST_="NOT_EXIST";
	fi

	CONTENT_=$(grep -i '>>|' $TARGET_FILE_ 2>/dev/null);

	echo -e $TEST_;

	echo -en $YEL_ "\tStatus code:\t"$DFT_;
	if [ "1" -lt "${#WORDS_[@]}" ] && [ "$STATUS_CODE_" == $EXPECTED_CODE_ ]; then
		echo -e $GRN_ "OK\t$STATUS_CODE_" $DFT_
	else
		echo -e $RED_ "Error:\t"$STATUS_CODE_$DFT_
	fi

	echo -en $YEL_"\tContent:\t"$DFT_;
	if [[ -n "$CONTENT_" ]] && [[ -n "$EXPECTED_CONTENT_" ]] && [[ "$CONTENT_" == *"$EXPECTED_CONTENT_"* ]]; then
		echo -e $GRN_ "OK\t$CONTENT_" $DFT_
	elif [[ -z "$CONTENT_" ]] && [[ -z "$EXPECTED_CONTENT_" ]]; then
		echo -e $GRN_ "OK\tEXPECTED EMPTY" $DFT_
	else
		echo -e $RED_ "Error:\t" $DFT_ $CONTENT_
	fi

	echo -en $YEL_"\tFile:\t\t"$DFT_;
	if [[ "$TARGET_FILE_EXIST_" == "$EXPECTED_EXIST_" ]]; then
		echo -e $GRN_ "OK\t"$EXPECTED_EXIST_ $DFT_
	else
		echo -e $RED_ "Error\t"$DFT_$TARGET_FILE_EXIST_
	fi

	kill $PID_;
	sleep 3;
	call_test_fds_leaks;
}

### TEST POST ######################################################################
#	test by curl

TARGET_="/post/new_file";
rm -rf ./tests$TARGET_;
POST_="-d >>|POSTED|<<";
CURL_="curl -isX POST http://127.0.0.1:2442$TARGET_ $POST_";
TEST_DESCR_="POST to $TARGET_";
EXPECTED_CODE_=200;
EXPECTED_EXIST_="EXIST";
EXPECTED_CONTENT_=">>|POSTED|<<";
call_test_post;

TARGET_="/post/append_existing_file";
rm -rf ./tests$TARGET_;
touch ./tests$TARGET_;
echo -n "APPEND_" >./tests$TARGET_;
POST_="-d >>|POSTED|<<";
CURL_="curl -isX POST http://127.0.0.1:2442$TARGET_ $POST_";
TEST_DESCR_="POST to $TARGET_";
EXPECTED_CONTENT_="APPEND_>>|POSTED|<<";
EXPECTED_CODE_=200;
EXPECTED_EXIST_="EXIST";
call_test_post;

TARGET_="/post/dir";
POST_="-d >>|POSTED|<<";
CURL_="curl -isX POST http://127.0.0.1:2442$TARGET_ $POST_";
TEST_DESCR_="POST to $TARGET_ which is a dir";
EXPECTED_CODE_=500;
EXPECTED_EXIST_="NOT_EXIST";
EXPECTED_CONTENT_="";
call_test_post;

TARGET_="/post/permission_denied_file";
touch ./tests/post/permission_denied_file;
chmod 000 ./tests$TARGET_;
POST_="-d >>|POSTED|<<";
CURL_="curl -isX POST http://127.0.0.1:2442$TARGET_ $POST_";
TEST_DESCR_="POST to $TARGET_";
EXPECTED_CODE_=500;
EXPECTED_EXIST_="EXIST";
EXPECTED_CONTENT_="";
call_test_post;

TARGET_="/post/upload_file";
rm -rf ./tests$TARGET_;
touch ./tests/post/source_for_upload_file;
echo ">>|CHECK_UPLOAD|<<" >./tests/post/source_for_upload_file;
CURL_="curl -isX POST http://127.0.0.1:2442$TARGET_ -F file=@./tests/post/source_for_upload_file ";
TEST_DESCR_="POST to $TARGET_";
EXPECTED_CODE_=200;
EXPECTED_EXIST_="EXIST";
EXPECTED_CONTENT_=">>|CHECK_UPLOAD|<<";
call_test_post;

TARGET_="/upload_denied/upload_file";
rm -rf ./tests$TARGET_;
POST_="-d >>|DENIED_UPLOAD|<<";
CURL_="curl -isX POST http://127.0.0.1:2442$TARGET_ -F file=@./tests/upload_denied/source_for_upload_file";
TEST_DESCR_="POST to $TARGET_";
EXPECTED_CODE_=403;
EXPECTED_EXIST_="NOT_EXIST";
EXPECTED_CONTENT_="";
call_test_post;

### TEST DELETE ######################################################################
#	test by curl

TARGET_="/delete/delete_file";
touch ./tests$TARGET_;
CURL_="curl -isX DELETE http://127.0.0.1:2442$TARGET_";
TEST_DESCR_="DELETE $TARGET_";
EXPECTED_CODE_=200;
EXPECTED_EXIST_="NOT_EXIST";
EXPECTED_CONTENT_="";
call_test_post;

TARGET_="/delete/non_existing_file";
rm -rf ./tests$TARGET_;
CURL_="curl -isX DELETE http://127.0.0.1:2442$TARGET_";
TEST_DESCR_="DELETE $TARGET_";
EXPECTED_CODE_=500;
EXPECTED_EXIST_="NOT_EXIST";
EXPECTED_CONTENT_="";
call_test_post;

TARGET_="/delete/dir";
CURL_="curl -isX DELETE http://127.0.0.1:2442$TARGET_";
TEST_DESCR_="DELETE $TARGET_";
EXPECTED_CODE_=500;
EXPECTED_EXIST_="NOT_EXIST";
EXPECTED_CONTENT_="";
call_test_post;

TARGET_="/delete/permission_denied/remain_file";
chmod 777 ./tests/delete/permission_denied/;
touch ./tests$TARGET_;
chmod 444 ./tests/delete/permission_denied/;
CURL_="curl -isX DELETE http://127.0.0.1:2442$TARGET_";
TEST_DESCR_="DELETE $TARGET_";
EXPECTED_CODE_=500;
EXPECTED_EXIST_="NOT_EXIST";
EXPECTED_CONTENT_="";
call_test_post;

TARGET_="/delete_denied/remain_file";
touch ./tests$TARGET_;
CURL_="curl -isX DELETE http://127.0.0.1:2442$TARGET_";
TEST_DESCR_="DELETE $TARGET_";
EXPECTED_CODE_=405;
EXPECTED_EXIST_="EXIST";
EXPECTED_CONTENT_="";
call_test_post;


### TEST FUNCTION ######################################################################
call_test_cgi()
{
	CALL_="valgrind $VALGRIND_FLAGS_ $PROG_";
	2>$BUFF_SERV_ 1>/dev/null $CALL_ & echo $! > ./tests/webserv.pid;
	PID_=$(cat ./tests/webserv.pid);
	sleep 3;

	NUM_=$(($NUM_ + 1));
	TEST_="$DFT_$NUM_:$DFT_ $TEST_DESCR_\n\t$MAG_$CURL_$DFT_";
	echo -e $TEST_;

	{ $CURL_; } >& $BUFF_;

	IFS=' ';
	cat <$BUFF_ | grep -i HTTP >$VAR_BUFF_;
	<$VAR_BUFF_ read -a WORDS_;
	STATUS_CODE_="${WORDS_[1]}";

	CONTENT_=$(cat <$BUFF_ | grep -i '>>|' 2>/dev/null);

	echo -en $YEL_ "\tStatus code:\t"$DFT_;
	if [ "1" -lt "${#WORDS_[@]}" ] && [ "$STATUS_CODE_" == $EXPECTED_CODE_ ]; then
		echo -e $GRN_ "OK\t$STATUS_CODE_" $DFT_
	else
		echo -e $RED_ "Error:\t"$STATUS_CODE_$DFT_
	fi

	echo -en $YEL_"\tContent:\t"$DFT_;
	if [[ -n "$CONTENT_" ]] && [[ -n "$EXPECTED_CONTENT_" ]] && [[ "$CONTENT_" == *"$EXPECTED_CONTENT_"* ]]; then
		echo -e $GRN_ "OK\t$CONTENT_" $DFT_
	elif [[ -z "$CONTENT_" ]] && [[ -z "$EXPECTED_CONTENT_" ]]; then
		echo -e $GRN_ "OK\tEXPECTED EMPTY" $DFT_
	else
		echo -e $RED_ "Error:\t" $DFT_ $CONTENT_
	fi

	kill $PID_;
	sleep 3;
	call_test_fds_leaks;

}

### TEST CGI ######################################################################
#	test by curl

TARGET_="/cgi/to_upper/script_capitalize.cgi";
chmod 744 ./tests$TARGET_;
CURL_="curl -isX POST http://127.0.0.1:2442$TARGET_ -d '>>|abc_123|<<'";
TEST_DESCR_="CGI POST $TARGET_";
EXPECTED_CODE_=200;
EXPECTED_CONTENT_="ABC_123";
call_test_cgi;

TARGET_="/cgi/permission_denied_script_capitalize.cgi";
chmod 000 ./tests$TARGET_;
CURL_="curl -isX POST http://127.0.0.1:2442$TARGET_ -d '>>|abc_123|<<'";
TEST_DESCR_="CGI POST $TARGET_";
EXPECTED_CODE_=500;
EXPECTED_CONTENT_="ERROR";
call_test_cgi;

TARGET_="/cgi/non_existing.cgi";
CURL_="curl -isX GET http://127.0.0.1:2442$TARGET_";
TEST_DESCR_="CGI GET $TARGET_";
EXPECTED_CODE_=500;
EXPECTED_CONTENT_="ERROR";
call_test_cgi;

TARGET_="/cgi/dir.cgi";
CURL_="curl -isX GET http://127.0.0.1:2442$TARGET_";
TEST_DESCR_="CGI GET $TARGET_";
EXPECTED_CODE_=500;
EXPECTED_CONTENT_="ERROR";
call_test_cgi;

TARGET_="/cgi/get_post/script_get_post.cgi";
CURL_="curl -isX GET http://127.0.0.1:2442$TARGET_";
TEST_DESCR_="CGI GET $TARGET_";
EXPECTED_CODE_=200;
EXPECTED_CONTENT_="GET";
call_test_cgi;

TARGET_="/cgi/get_post/script_get_post.cgi";
CURL_="curl -isX POST http://127.0.0.1:2442$TARGET_";
TEST_DESCR_="CGI POST $TARGET_";
EXPECTED_CODE_=200;
EXPECTED_CONTENT_="POST";
call_test_cgi;

TARGET_="/cgi/infinite_loop/script_infinite_loop.cgi";
CURL_="curl -isX GET http://127.0.0.1:2442$TARGET_";
TEST_DESCR_="CGI GET $TARGET_";
EXPECTED_CODE_=408;
EXPECTED_CONTENT_="TIMEOUT";
call_test_cgi;


### CHMOD ######################################################################
# change permissions back to allow copy project
TARGET_="/special_files/no_permission_file";
chmod 644 ./tests$TARGET_;

TARGET_="/post/permission_denied_file";
chmod 644 ./tests$TARGET_;

chmod 744 ./tests/delete/permission_denied/;

TARGET_="/cgi/permission_denied_script_capitalize.cgi";
chmod 744 ./tests$TARGET_;

### END ######################################################################

echo -e "\n\t$DFT_$BLK_BOLD_$BG_GRN_ The END $DFT_\n"