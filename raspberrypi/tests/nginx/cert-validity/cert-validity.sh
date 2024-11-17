#!/bin/bash

#intermediate cert
openssl verify \
        -CAfile /root/ca/certs/ca.cert.pem   \
        /root/ca/intermediate/certs/intermediate.cert.pem

#client cert - this is an example assuming the 
#client cert was signed by the intermediate cert
openssl verify \
       -CAfile /root/ca/intermediate/certs/ca-chain.cert.pem \
       /root/ca/intermediate/certs/client.cert.pem

#server cert
openssl verify \
       -CAfile /root/ca/intermediate/certs/ca-chain.cert.pem \
       /etc/ssl/certs/intermediate/server.cert.pem
