#!/bin/bash

# Definition de quelques couleurs 
red='\e[0;31m'
redhl='\e[0;31;7m'
RED='\e[1;31m'
blue='\e[0;34m'
BLUE='\e[1;34m'
cyan='\e[0;36m'
CYAN='\e[1;36m'
green='\e[0;32m'
GREEN='\e[0;32m'
NC='\e[0m' # No Color

#Regarde si l'execution est bien en root (i.e. sudo)
if [ $USER != "root" ]
then
   echo -e $RED "Vous devez être root pour lancer ce progamme!" $NC
   exit 1
fi

#installer la toolchain de crosscompilation :
echo -en $RED "Installer la toolchain (compilateur, binutils, glibc ...) de cross-compilation CodeSourcery (y/n) ?" $NC
read TOOLCHAIN
if [ "$TOOLCHAIN" = "y" ]
then
   echo -e $BLUE "Installation de la toolchain ..." $NC
   cd /tmp
   wget http://www.codesourcery.com/sgpp/lite/arm/portal/package7853/public/arm-none-linux-gnueabi/arm-2010.09-50-arm-none-linux-gnueabi.bin
   chmod +x arm-2010.09-50-arm-none-linux-gnueabi.bin
   ./arm-2010.09-50-arm-none-linux-gnueabi.bin
else
   echo -e $BLUE "Pas d'installation de la toolchain" $NC
fi


echo -en $RED "Installer la librairie Libserial (y/n) ?" $NC
read LIBSERIAL


if [ "$LIBSERIAL" = "y" ]
then
   cd /tmp
   echo -e $CYAN " Récupération des sources " $NC
   wget http://freefr.dl.sourceforge.net/project/libserial/libserial/libserial-0.5.2/libserial-0.5.2.tar.gz
   tar xzvf libserial-0.5.2.tar.gz
   cd libserial-0.5.2/
   echo -e $CYAN "Configuration des sources" $NC
   DEFAULT_PATH=$HOME/CodeSourcery/Sourcery_G++_Lite/arm-none-linux-gnueabi/
   echo -n " Où est installé le cross-compilateur (default : "
   echo -en $green $DEFAULT_PATH
   echo -n " ) ?"
   read CROSS_COMPILER_PATH
   if [ CROSS_COMPILER_PATH -n ]
   then
   DEFAULT_PATH=CROSS_COMPILER_PATH
   fi
   ./configure CC=arm-none-linux-gnueabi-g++ --host=arm-none-linux-gnueabi --prefix=$DEFAULT_PATH
   echo -en $cyan "Modification des sources (il manque des #include à l'heure où j'ai fais ce script)"
   sed -i.bak '15i#include <string.h>' src/SerialStreamBuf.h
   sed -i.bak '23i#include <string.h>' src/SerialPort.h
   sed -i.bak '23i#include <cstdlib>' src/SerialPort.h
   sed -i.bak '15i#include <string.h>' src/PosixSignalDispatcher.h
   sed -i.bak '1i#include <cstdlib>' examples/read_port.cpp
   sed -i.bak '1i#include <cstdlib>' examples/write_port.cpp
   echo -e $CYAN "Compilation" $NC
   make
  echo -e $CYAN "Installation" $NC
   make $CYAN install
else
   echo -e $BLUE "Pas d'installation de la libserial" $NC
fi


echo -en $RED "Installer la librairie boost . Cette opération est longue . (y/n) ? " $NC
read BOOST
if [ "$BOOST" =  "y" ]
then
   echo -e $CYAN"Installation de boost (la totalité) ..." $NC
   echo -e $cyan "Dependance : bjam" $NC
   echo -n "Installation de bjam (Equivalent de make propre à boost. Nécéssaire à la compilation) (y/n) ?"
   read BJAM
   if [ "$BJAM" = "y" ]
   then
      echo "Installation de bjam"
      #installer bjam pour pouvoir compiler boost
      sudo apt-get install bjam
   else
      echo "Pas d'installation de bjam."
   fi
   #compiler boost
   echo -e $CYAN "Récupération des sources de boost v 1.44.0" $NC
   cd /tmp
   wget http://sourceforge.net/projects/boost/files/boost/1.44.0/boost_1_44_0.tar.gz
   echo -e $CYAN "Décompression" $NC
   tar xzvf boost_1_44_0.tar.gz
   cd boost_1_44_0/
   echo -e $CYAN"Configuration" $NC
   GPP_PATH=$HOME/CodeSourcery/Sourcery_G++_Lite/bin/arm-none-linux-gnueabi-g++
   GPP_VER=`${GPP_PATH} -v 2>&1 | tail -1 | awk '{print $3}'`
   echo "using gcc : ${GPP_VER} : ${GPP_PATH}  ; " > tools/build/v2/user-config.jam
   echo -e $CYAN "Compilation" $NC
   sudo bjam --toolset=gcc-${GPP_VER} --prefix=$HOME/CodeSourcery/Sourcery_G++_Lite/arm-none-linux-gnueabi/ install
else
   echo -e $BLUE "Pas d'installation de boost" $NC
fi

vlc=$(which ifconfig) 
if [ $? != 0 ]
then
   vlc --vout caca http://www.youtube.com/watch?v=EK2tWVj6lXw
   echo
   echo
   echo
   echo "*********************"
   echo "*ASCII RICKROLLED !!*"
   echo "*********************"
   echo
   echo
   echo
fi

