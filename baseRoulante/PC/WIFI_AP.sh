#!/bin/bash

# Configuration du point d'accès

#Infos générales
INTERFACE="wlan3"
SSID="Test"
CHANNEL="1"
HW_MODE="g"
DRIVER="nl80211"
BEACON_INT="100"


#Infos du WPA
WPA_PASSPHRASE="passphrase"
WPA="2"
WPA_KEY_MGMT="WPA-PSK"

# IP & mask du sous-réseau créé sur l'interface wlan
SUBNET="192.168.6.0/24" 
IP="192.168.6.1"
MASK="255.255.255.0"

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

#Mode Debug Dhcp ?
DBG="-d"
#DBG=""     #Ligne à décommenter pour désactiver le debug du serveur dhcpd

#Regarde si l'execution est bien en root (i.e. sudo)
if [ $USER != "root" ]
then
   echo -e $RED"Vous devez être root pour lancer ce progamme!"$NC
   exit 1
fi

#Verifie si tous les modules sont bien installes
ifconfig=$(which ifconfig) 
if [ $? != 0 ]
then
    echo -e $RED"Erreur Fatale: Un problème est survenue: Impossible de trouver la commande ifconfig!"$NC
    exit 1
fi

hostapd=$(which hostapd)
if [ $? != 0 ]
then
    echo -e $RED"Erreur Fatale: Vous devez installer hostapd!"$NC
    exit 1
fi

dhcpd=$(which dhcpd)
if [ $? != 0 ]
then
    echo -e $RED"Erreur Fatale: Vous devez installer dhcpd!"$NC
    exit 1
fi

echo -e $BLUE "Démarrage et configuration de l'interface wifi $INTERFACE..."$NC
sudo ifconfig $INTERFACE down
sleep 0.5
sudo ifconfig $INTERFACE $IP netmask $MASK

echo -e $BLUE "Création profil hostapd temporaire" $NC

OPTIONS="interface=$INTERFACE\n\
ssid=$SSID\n\
channel=$CHANNEL\n\
hw_mode=$HW_MODE\n\
driver=$DRIVER\n\
beacon_int=$BEACON_INT\n\
wpa_passphrase=$WPA_PASSPHRASE\n\
wpa_key_mgmt=$WPA_KEY_MGMT"


echo -e $green $OPTIONS $NC
echo -e $OPTIONS > /tmp/AP.hostapd
echo -e $BLUE "Démarrage du démon hostapd" $NC
sudo hostapd -B /tmp/AP.hostapd

echo -e $BLUE "Démarrage de dhcpd" $NC
   sudo /etc/init.d/isc-dhcp-server start
sleep 1
