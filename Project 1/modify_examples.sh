#!/bin/sh

text()
{
    echo "The goal of the modify.sh is to change the names of the files, according to the given sed expression/regex.
    The simplest use of this method are presented as:

    modify -u ./file    - This simple commend will turn all of the lowercase characters in the file names 'file' to uppercase characters
    modify -l ./Dir/*   - This command will modify all of the files inside the 'Dir' folder, by changing their uppercase letters, to lowercase
    modify -h           - This command will display all of the possible arguments for the modify method
    
    Some more advanced uses:

    modify -u ./file Desktop/Dir/*    - This command will not only change all of the letters in 'file' but all of the files inside the 'Dir' folder
    located on the desktop as well
    modify -r -l Desktop/Dir          - This method, in contrary to other examples, accepts not a file path, but a folder path, and proceeds to 
    change all of the files in given folder to lowercase letters, as well as all of its subdirectories
    modify <sed_expression> ./file    - This command will change the 'file' name according to the given sed expression
    
    Some incoorect uses:
    modify ./file -u     - Will not execute because of the wrong order of arguments
    modify ./file        - Will not execute because of lack of the mode argument
    modify -u            - Will not execute beausee of lack of file directory
    modify -u -r ./Dir   - Will not execute because of wrong order of the first two arguments"
}

if test -z "$1"
then
text()
fi

while test "x$1" != "x"
do
        case "$1" in
                *) text()
        esac
        shift
done
exit 1


