#!/bin/sh -e

if [ ! -d "/opt/aws/greengrass/v2" ]; then
    echo "Greengrass not installed, extracting dependencies"

    # Extract the Installer
    if [ ! -d "/opt/GreengrassInstaller" ]; then
        unzip /opt/greengrass-nucleus-latest.zip -d /opt/GreengrassInstaller
    else
        echo "GreengrassInstaller is already installed."
    fi

    if [ ! -d "/opt/aws" ]; then
        tar -xvf /opt/azul-zulu-java11-jdk.tar.gz -C /opt/
        mv /opt/zulu11.64.19-ca-jdk11.0.19-linux_aarch32hf/ /opt/aws/
    else
        echo "Zulu JDK is already installed."
    fi
else
    echo "Greengrass installed, registering it with AWS IoT"
fi

# Source the required env variables
source ~/.bashrc

# install greengrass
. /customer/credentials.sh
java -Droot="/opt/aws/greengrass/v2" -jar -Djava.security.properties=/opt/aws/conf/security/java.security /opt/GreengrassInstaller/lib/Greengrass.jar --aws-region $AWS_REGION --thing-name $DEVICE_NAME --thing-group-name $DEVICE_GROUP --component-default-user root:root --provision true --setup-system-service false --deploy-dev-tools false

# cleanup
rm -rf /opt/GreengrassInstaller