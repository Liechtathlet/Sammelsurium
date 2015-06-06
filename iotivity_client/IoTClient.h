#ifndef IOTSERVER_H
#define IOTSERVER_H

#include <string>
#include <cstdlib>
#include "OCPlatform.h"
#include "OCApi.h"

using namespace OC;
using namespace std;

class IoTClient
{
	shared_ptr<PlatformConfig> m_platformConfig;

	FindCallback resourceDiscoveryCallback;

	FindDeviceCallback deviceDiscoveryCallback;

	void discoveredResource(shared_ptr<OCResource>);

	void receivedDeviceInfo(const OCRepresentation& rep);
public:
	IoTClient();
	~IoTClient();
    void init();
    void run();
    void findResource();

protected:
    void startPlatform();
};


#endif // IOTSERVER_H
