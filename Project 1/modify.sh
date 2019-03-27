#!/bin/sh
Script=$(readlink -f "$0")
ScriptPath=$(dirname "$Script")
SED=""
MODE=""
RECURSION=0

# function for printing error messages to diagnostic output
Help()
{
	echo "Arg 1:
	[-h] - show help
	[-r] - change all of the names in folder and subfolders
	[-l] - change to lowercase
	[-h] - change to uppercase
	
Arg 2:
	[-l] - change to lowercase in folder and all subfolders
	[-h] - change to uppercase in folder and all subfolders
	<sed pattern> - change according to pattern
	Folder of file path if -r is not a first arg
Arg 3:
	Folder of file path if -r is a first arg"
exit 0
}



# if no arguments given
if test -z "$1"
then
cat<<EOT 1>&2

Arg 1:
	[-h] - show help
	[-r] - change all of the names in folder and subfolders
	[-l] - change to lowercase
	[-h] - change to uppercase
	
Arg 2:
	[-l] - change to lowercase in folder and all subfolders
	[-h] - change to uppercase in folder and all subfolders
	<sed pattern> - change according to pattern
	Folder of file path if -r is not a first arg
Arg 3:
	Folder of file path if -r is a first arg

EOT
exit 0
fi


# do with command line arguments
f=n
s=n
while test "x$1" != "x"
do
        case "$1" in
		-h) Help;;
		-l) MODE="l";;
		-u) MODE="u";;
		-r) RECURSION=1;;
		*) break;;
        esac
shift
done

if [ -n "$1" -a -z "$MODE" ]; then
	SED=$1
	MODE="s"
	shift
fi

if [[ $1 != ./* ]]; then
	cd
fi

while [ -e "$1" ]; do

cd "$ScriptPath"

echo "$1"
ls
	
if [ $RECURSION = 0 ]; then
	for i in "$1"
	do
		if [[ $i = ./* ]]; then
			cd "$ScriptPath"
		else
			cd
		fi
		cd ${i%/*}
		if [[ -f ${i##*/} ]]; then
			if [ $MODE = "l" ]; then
				rename 'y/A-Z/a-z/' ${i##*/}
			elif [ $MODE = "u" ]; then
				rename 'y/a-z/A-Z/' ${i##*/}
			elif [ $MODE = "s" ]; then
				rename "$SED" ${i##*/}
		fi
		else
			echo "NOT A FILE"
			exit 0
		fi
	done
fi

if [ $RECURSION = 1 ]; then
	for i in "$1"
	do
	if [[ $i != ./* ]]; then
		cd
	fi 
		for f in `find "$i" -name "*"`; do
		if [[ $i = ./* ]]; then
			cd "$ScriptPath"
		else
			cd
		fi
		cd ${f%/*}
		if [ -f ${f##*/} ]; then
			if [ $MODE = "l" ]; then
				rename 'y/A-Z/a-z/' ${f##*/}
			elif [ $MODE = "u" ]; then
				rename 'y/a-z/A-Z/' ${f##*/}
			elif [ $MODE = "s" ]; then
				rename "$SED" ${f##*/}
			fi
		fi
		done
	done
fi
ls
cd "$ScriptPath"
shift
if [[ $1 != ./* ]]; then
cd
fi
pwd
done
exit 1
