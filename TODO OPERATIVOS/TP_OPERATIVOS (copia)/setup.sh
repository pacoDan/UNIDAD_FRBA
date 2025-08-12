#!/bin/bash
# SisOp 2C2018
# Instalador de ElGranTP
# elepethread

ORIG_PATH=$(pwd)
TP_PATH=$ORIG_PATH"/tp-2018-2c-elepethread"

function mostrar() {
  echo "$1"
  echo "$2"
}

function avance() {
  if [ $CONTAME -lt 100 ]
  then
    printf "\r\e[37;41m \e[?25l ${1}...        (${CONTAME} of 100) \e[0m"
    CONTAME=$(($CONTAME + 20))
    sleep .5
  fi
}

function bajar(){
  mostrar "" "Ingrese sus credenciales de GIT:"
  read -p " GIT user name: " GUSER;
  read -p " GIT user email: " GEMAIL;
  if [ -z "$GUSER" ]
  then
    GUSER="$USER"
  fi
  if [ -z "$GEMAIL" ]
  then
    GEMAIL="$MAIL"
  fi
  git config --global user.name $GUSER
  git config --global user.email $GEMAIL
  git clone "https://${USER}@github.com/sisoputnfrba/tp-2018-2c-elepethread.git"
  git clone "https://${USER}@github.com/sisoputnfrba/so-commons-library.git"
  mostrar "Todos los repositorios han sido clonados."
}

function instalarLibs(){
  mostrar
  cd $ORIG_PATH"/so-commons-library"
  sudo make
  sudo make install
  cd $ORIG_PATH

  mostrar
  cd $ORIG_PATH"/custom_library"
  sudo make
  sudo make install
  #export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:~/git/tp-2018-2c-QUANTUM/qcommons/Qcommons
  #echo "export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/your/custom/path/" >> ~/.bashrc
  cd $ORIG_PATH

  mostrar "Todas las librerias han sido instaladas."
}

function instalar {
  echo ""
  echo " .:: El sistema sera compilado ::."
  echo ""
  declare -i CONTAME=19

  cd $TP_PATH"/cpu"  
  sudo make clean
  sudo make
  cd ..
  avance "cpu compilado satisfactoriamente"

  cd $TP_PATH"/dam"
  sudo make clean
  sudo make
  cd ..
  avance "dam compilado satisfactoriamente"

  cd $TP_PATH"/fm9"
  sudo make clean
  sudo make
  cd ..
  avance "fm9 compilado satisfactoriamente"

  cd $TP_PATH"/mdj"
  sudo make clean
  sudo make
  cd ..
  avance "mdj compilado satisfactoriamente"

  cd $TP_PATH"/safa"
  sudo make clean
  sudo make
  cd ..
  avance "safa compilado satisfactoriamente"

  mostrar

  mostrar "Todos los modulos han sido compilados."
  cd $TP_PATH
  sudo chmod -R a+rwX *
  cd ..
  chown -R utnso tp-2018-2c-elepethread
}

function configurar {
  echo ""
  echo " .:: Complete los campos para el setup y luego instalar ::."
  echo " Los campos son opcionales. Dejar cualquier campo vacio para mantener la configuracion default."
  echo ""
  read -p " IP_SAFA: " SAFA;
  read -p " PUERTO_ESCUCHA_CONEXIONES_SAFA: " SAFA_P;

  read -p " IP_DIEGO: " DIEGO;
  read -p " PUERTO_ESCUCHA_CONEXIONES_DIEGO: " DIEGO_P;

  read -p " IP_MDJ: " MDJ;
  read -p " PUERTO_ESCUCHA_CONEXIONES_MDJ: " MDJ_P;

  read -p " IP_FM9: " FM9;
  read -p " PUERTO_ESCUCHA_CONEXIONES_FM9: " FM9_P;

  declare -i CONTAME=19
  mostrar
  ####################################### CPU #######################################
  avance "Creando archivo de configuracion para CPU"
  EL_FILE=$TP_PATH"/cpu/cpu.config"
  agregarCoso "IP_SAFA" $EL_FILE $SAFA
  agregarCoso "PUERTO_SAFA" $EL_FILE $SAFA_P
  agregarCoso "IP_DIEGO" $EL_FILE $DIEGO
  agregarCoso "PUERTO_DIEGO" $EL_FILE $DIEGO_P
  ####################################### DAM #######################################
  avance "Creando archivo de configuracion para DAM"
  EL_FILE=$TP_PATH"/dam/dam.config"
  agregarCoso "IP_SAFA" $EL_FILE $SAFA
  agregarCoso "PUERTO_SAFA" $EL_FILE $SAFA_P
  agregarCoso "IP_MDJ" $EL_FILE $MDJ
  agregarCoso "PUERTO_MDJ" $EL_FILE $MDJ_P
  agregarCoso "IP_FM9" $EL_FILE $FM9
  agregarCoso "PUERTO_FM9" $EL_FILE $FM9_P
  ####################################### FM9 #######################################
  avance "Creando archivo de configuracion para FM9"
  EL_FILE=$TP_PATH"/fm9/fm9.config"
  agregarCoso "PUERTO" $EL_FILE $FM9_P
  ####################################### MDJ #######################################
  avance "Creando archivo de configuracion para MDJ"
  EL_FILE=$TP_PATH"/mdj/mdj.config"
  agregarCoso "PUERTO" $EL_FILE $MDJ_P
  ####################################### SAFA #######################################
  avance "Creando archivo de configuracion para SAFA"
  EL_FILE=$TP_PATH"/safa/safa.config"
  agregarCoso "PUERTO" $EL_FILE $SAFA_P
  #######################################################################################
  mostrar
  mostrar "Todos los archivos han sido configurados."
}

function borrar(){
  mostrar "" " .:: Se iniciara el proceso de borrado ::."
  echo ""
  read -p "Estas seguro que quieres desinstalar? [Y/N] " BLEH
  case $BLEH in
    [Yy]* ) otrave;;
    [Nn]* ) echo "Se ha cancelado la operacion";;
    * ) echo "No se ha eliminado ningun componente.";;
  esac
}

function otrave(){
  mostrar "" "Realmente, esto eliminara todo."
  read -p "Estas seguro? [Y/N] " RESP
  case $RESP in
  [Yy]* ) mostrar "Eliminando...."
        sudo rm -rf $TP_PATH
            sudo rm -rf $ORIG_PATH"/so-commons-library"
        mostrar "Se ha eliminado!";;
  [Nn]* ) echo "Se deshizo la operacion";;
  * ) echo "No se realizaron cambios";;
  esac
}

function agregarCoso(){
    if [ -n "$3" ]
    then
      sed -i "/${1}/ c\\${1}=${3}" $2
    fi
  }

function forzarRoot(){
  if [[ $(id -un) != "root" ]]
  then
   echo "Si no eres root, no puede instalar el software."
   sudo $0
   exit
  fi
}

clear
forzarRoot
echo ""
printf " \e[31;4;1m -== SisOp 2C2018 - ElGranTP ==- \e[0m"
mostrar
options=("Descargar - Clona los repositorios" \
    "Instalar commons & custom_library - Setup de las commons & custom_library" \
    "Setup - Crea los archivos de configuracion" \
    "Instalar ElGranTP - Compila el sistema" \
    "Borrar - Elimina el sistema ElGranTP" "Quit")
PS3="Seleccione una opcion: "
select opt in "${options[@]}"; do 
    case "$REPLY" in
    1 ) bajar;;
    2 ) instalarLibs;;   
    3 ) configurar;;
    4 ) instalar;;
    5 ) borrar;;
    6 ) echo "#elepethread - SisterCall"; break;;
    *) echo "Opcion invalida. Intenta con otra."; continue;;
    esac
done