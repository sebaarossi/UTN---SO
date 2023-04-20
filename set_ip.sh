#!/bin/bash

#Exits if error
set -e 

# usage() { echo "Usage: $0 -e <env>" 1>&2; exit 1; }
# 
# while getopts ":e:" o; do
#     case "${o}" in
#         e)
#             e=${OPTARG}
#              ((e == "development" || e == "production")) || usage
#             ENV=$e
#             ;;
#         *)
#             usage
#             ;;
#     esac
# done
# shift $((OPTIND-1))
# 
# if [ -z "${ENV}" ]; then
#     usage
# fi

IP_KERNEL=$1
IP_CPU=$2
IP_MEMORIA=$3

echo "SET NEW IP" 

# sed -i '' s/PATRON QUE QUIERO ENCONTRAR/PATRON QUE QUIERO REEMPLAZAR/g FILE

#CAMBIO IPS

sed -i "s/IP_KERNEL=127.0.0.1/IP_KERNEL=${IP_KERNEL}/g"   ./consola/cfg/*.config


sed -i "s/IP_CPU=127.0.0.1/IP_CPU=${IP_CPU}/g" ./kernel/cfg/*.config
sed -i "s/IP_MEMORIA=127.0.0.1/IP_MEMORIA=${IP_MEMORIA}/g" ./kernel/cfg/*.config
sed -i "s/IP_KERNEL=127.0.0.1/IP_KERNEL=${IP_KERNEL}/g" ./kernel/cfg/*.config

sed -i "s/IP_MEMORIA=127.0.0.1/IP_MEMORIA=${IP_MEMORIA}/g" ./CPU/cfg/*.config
sed -i "s/IP_KERNEL=127.0.0.1/IP_KERNEL=${IP_KERNEL}/g" ./CPU/cfg/*.config
sed -i "s/IP_CPU=127.0.0.1/IP_CPU=${IP_CPU}/g" ./CPU/cfg/*.config


sed -i "s/IP_MEMORIA=127.0.0.1/IP_MEMORIA=${IP_MEMORIA}/g" ./memoria/cfg/*.config

echo 'Finalizacion OK'
