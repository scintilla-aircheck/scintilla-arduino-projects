#!/bin/sh
# Get a device's key from Azure.
# Requires the "iothub-explorer" node package
# sudo npm install iothub-explorer@latest
# Also uuid: sudo apt-get install uuid
# You'll need the IoT Hub's master key.
# Ask edg

CONNSTR=`cat ~/.azure_key`

iothub-explorer "${CONNSTR}" create `uuid` --connection-string --display=connection-string|grep connectionString|cut -d":" -f 2|tr -d ' '

