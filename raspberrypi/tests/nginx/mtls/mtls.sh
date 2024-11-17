#!/bin/bash

curl -v -s https://192.168.1.175:443/ \
     --cacert /home/hayden/ca-chain.cert.pem \
     --key /home/hayden/client.key.pem    \
     --cert /home/hayden/client.cert.pem
