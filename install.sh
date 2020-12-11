#!/bin/sh

install(){

    sudo mkdir -p /usr/share/skr 
    sudo cp -r src/parser /usr/share/skr

    sudo cp skrkid /usr/share/skr

    sudo cp skrcore /usr/share/skr
    sudo cp skr /usr/bin/
}
build(){
    gcc src/core/main.c -o skrcore
    gcc src/kid/skrkid.c -o skrkid
}
uninstall(){
    sudo rm -rf /usr/share/skr
    sudo rm /usr/bin/skr
}
usage(){
    echo usage:
    echo " "
    echo "      ./install.sh      builds and installs skr" 
    echo "      ./install.sh -b   builds skr" 
    echo "      ./install.sh -i   installs skr" 
    echo "      ./install.sh -u   uninstalls skr"
}

if [ "$#" = "0" ];then
    echo installing skr
    build
    install
elif [ "$1" = "-b" ];then
    echo building skr
    build
elif [ "$1" = "-i" ];then
    echo building skr
    install

elif [ "$1" = "-u" ];then
    echo uninstalling skr
    uninstall

else
    usage
fi
