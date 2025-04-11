#!/bin/sh -e

if [ ! -d "/greengrass/v2" ]; then
    echo "Greengrass not installed, extracting dependencies"

    # Extract the Installer
    if [ ! -d "/greengrass/GreengrassInstaller" ]; then
        unzip /tmp/greengrass-nucleus-latest.zip -d /greengrass/GreengrassInstaller
    else
        echo "GreengrassInstaller is already installed."
    fi

    if [ ! -d "/zulu11" ]; then
        tar x /tmp/azul-zulu-java11-jdk.tar.gz -C /
        # mv /zulu11.64.19-ca-jdk11.0.19-linux_aarch32hf/* /
        # echo "export PATH=/zulu11/bin:\$PATH" >> /etc/profile
        source /etc/profile
    else
        echo "Zulu JDK is already installed."
    fi
else
    echo "Greengrass installed, registering it with AWS IoT"
fi

# Source the required env variables
source ~/.bashrc

# Install Greengrass
. /customer/credentials.sh
java -Droot="/greengrass/v2" -jar -Djava.security.properties=/greengrass/conf/security/java.security /greengrass/GreengrassInstaller/lib/Greengrass.jar --aws-region $AWS_REGION --thing-name $DEVICE_NAME --thing-group-name $DEVICE_GROUP --component-default-user root:root --provision true --setup-system-service false --deploy-dev-tools false

# Cleanup
rm -rf /greengrass/GreengrassInstaller