#!/bin/bash

echo "=========================================="
echo "=                                        ="
echo "=        Self-Signed Certificate         ="
echo "=               Generator                ="
echo "=                                        ="
echo "=========================================="
echo ""

function generate_CA () {
   echo -e "\n===== Generating CA certs ====="
   openssl req -x509 -nodes -sha256 -newkey rsa:2048 -subj "$1"  -days 365 -keyout ca.key -out ca.crt
   echo "==================================================="
}

function generate_server () {
   echo -e "\n===== Generating server certs ====="
   openssl req -nodes -sha256 -new -subj "$1" -keyout server.key -out server.csr
   openssl x509 -req -sha256 -in server.csr -CA ca.crt -CAkey ca.key -CAcreateserial -out server.crt -days 365
   echo "==================================================="
}

function generate_client () {
   echo -e "\n===== Generating client certs ====="
   openssl req -new -nodes -sha256 -subj "$1" -out client.csr -keyout client.key
   openssl x509 -req -sha256 -in client.csr -CA ca.crt -CAkey ca.key -CAcreateserial -out client.crt -days 365
   echo "==================================================="
}


read -p "Generate server certs [Y/N]:  " GenerateServerCert
while [[ "$GenerateServerCert" != "Y" && "$GenerateServerCert" != "N" ]]
do
       echo -e "\nInvalid choice please choose Y or N\n"
       read -p "Generate server certs [Y/N]:  " GenerateServerCert
done

read -p "Generate client cert [Y/N]:  " GenerateClientCert
while [[ "$GenerateClientCert" != "Y" && "$GenerateClientCert" != "N" ]]
do
       echo -e "\nInvalid choice please choose Y or N\n"
       read -p "Generate client cert [Y/N]:  " GenerateClientCert
done

read -p "Enter server IP/Domain: " IP
#IP="192.168.1.29"
SUBJECT_CA="/C=ES/ST=Spain/L=Spain/O=MyHome/OU=CA/CN=$IP"
SUBJECT_SERVER="/C=ES/ST=Spain/L=Spain/O=MyHome/OU=Server/CN=$IP"
SUBJECT_CLIENT="/C=ES/ST=Spain/L=Spain/O=MyHome/OU=Client/CN=$IP"


if [[ "$GenerateServerCert" == "Y" ]]
then
       generate_CA $SUBJECT_CA
       generate_server $SUBJECT_SERVER
fi

if [[ "$GenerateClientCert" == "Y" ]]
then
       generate_client $SUBJECT_CLIENT
fi