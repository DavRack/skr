#!/bin/sh

install(){
    gcc src/core/main.c -o skr

    sudo mkdir -p /usr/share/skr 
    sudo cp -r src/parser /usr/share/skr

    sudo cp skr /usr/bin/
}
uninstall(){
    sudo rm -rf /usr/share/skr
    sudo rm /usr/bin/skr
}
usage(){
    echo usage:
    echo " "
    echo "      ./install.sh      installs skr" 
    echo "      ./install.sh -u   uninstalls skr"
}

if [ "$#" = "0" ];then
    echo installing skr
    install

elif [ "$1" = "-u" ];then
    echo uninstalling skr
    uninstall

else
    usage
fi
