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
while test "x$1" != "x"							#checking the first argument, and the second argument if recursion is selected
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

if [ -n "$1" -a -z "$MODE" ]; then					#setting the SED expression and setting the mode to s (SED), if the mode is not 
	SED=$1								#already selected
	MODE="s"
	shift
fi

if [[ $1 != ./* ]]; then						#if the given imput directory is not in ./* format, change the directory to
	cd								#home directory to avoid the confilct with directories
fi

while [ -e "$1" ]; do							#check if given file exists

cd "$ScriptPath"
	
if [ $RECURSION = 0 ]; then
	for i in "$1"
	do
		if [[ $i = ./* ]]; then					#changing directories to avoid confilct
			cd "$ScriptPath"				#if the dir is in the ./*, change dir to the script dir
		else
			cd						#if not, change to base directory
		fi
		cd ${i%/*}						#change dir to dir of given file; ${i%/*} cuts the given file name, but leaves 										the dir 
		if [[ -e ${i##*/} ]]; then				#check if the given file exists; ${i##*/} cuts everything but the 										filename
			if [ $MODE = "l" ]; then
				rename 'y/A-Z/a-z/' ${i##*/}		#changes the letters to lowercase
			elif [ $MODE = "u" ]; then
				rename 'y/a-z/A-Z/' ${i##*/}		#changes the letters to uppercase
			elif [ $MODE = "s" ]; then
				rename "$SED" ${i##*/}			#changes the name according to the sed expression
		fi
		else
			echo "NOT A FILE"				#if given file is not a file, print an error
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
		for f in `find "$i" -name "*"`; do			#creates the list of all of the subdirectories of a given file, and goes one by 									one
		if [[ $i = ./* ]]; then
			cd "$ScriptPath"
		else
			cd
		fi
		cd ${f%/*}						#change dir to dir of given file; ${f%/*} cuts the given file name, but leaves 										the dir 
		if [ -f ${f##*/} ]; then				#check if the given file is indeed a file; ${f##*/} cuts everything but the 										filename
			if [ $MODE = "l" ]; then
				rename 'y/A-Z/a-z/' ${f##*/}		#changes the letters to lowercase
			elif [ $MODE = "u" ]; then
				rename 'y/a-z/A-Z/' ${f##*/}		#changes the letters to uppercase
			elif [ $MODE = "s" ]; then
				rename "$SED" ${f##*/}			#changes the name according to the sed expression			
			fi
		fi
		done
		echo "Done"
		if [[ $i != ./* ]]; then
		cd
		fi 
		for f in `find "$i" -name "*" | tac`; do		#creates the list of all of the subdirectories of a given file, and 										goes one by one, "| tac" reverses the list
		if [[ $i = ./* ]]; then
			cd "$ScriptPath"
		else
			cd
		fi
		cd ${f%/*}						#check if the given file is indeed a file; ${f##*/} cuts everything but the 										filename
		if [ -d ${f##*/} ]; then				#check if the given file is a directory; ${f##*/} cuts everything but the 										filename
			if [ $MODE = "l" ]; then
				rename 'y/A-Z/a-z/' ${f##*/}		#changes the letters to lowercase
			elif [ $MODE = "u" ]; then
				rename 'y/a-z/A-Z/' ${f##*/}		#changes the letters to uppercase
			elif [ $MODE = "s" ]; then
				rename "$SED" ${f##*/}			#changes the name according to the sed expression
			fi
		fi
		done
	done
fi
cd "$ScriptPath"
shift
if [[ $1 != ./* ]]; then						#if the next file is not in format ./*, change the dir to the home dir, to avoid
cd									#conflict
fi
done
exit 1
