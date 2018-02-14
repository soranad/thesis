echo "start ODL controller"
JAVA_MAX_MEM=10G JAVA_MAX_PERM_MEM=1G ./ODL/nitrogen-ODL-0.7.1/bin/karaf

echo "start ONOS controller"
./ONOS/onos-1.12.0/bin/onos-service start