#ifndef IOTSERVER_H
#define IOTSERVER_H

#include <string>
#include <cstdlib>
#include "OCPlatform.h"
#include "OCApi.h"
#include "src/resources/AbstractResource.h"

using namespace OC;
using namespace std;

class IoTServer
{
	shared_ptr<PlatformConfig> m_platformConfig;

	OCRepresentation m_testRepresentation;
	OCResourceHandle m_testRes;

	AbstractResource * test_res;

	OCDeviceInfo deviceInfo;

	std::string contentType = "myContentType";
	std::string dateOfManufacture = "myDateOfManufacture";
	std::string deviceName = "myDeviceName";
	std::string deviceUUID = "myDeviceUUID";
	std::string firmwareVersion = "myFirmwareVersion";
	std::string hostName = "myHostName";
	std::string manufacturerName = "myManufacturerNa";
	std::string manufacturerUrl = "myManufacturerUrl";
	std::string modelNumber = "myModelNumber";
	std::string platformVersion = "myPlatformVersion";
	std::string supportUrl = "mySupportUrl";
	std::string version = "myVersion";

	void DeleteDeviceInfo();
	void DuplicateString(char ** targetString, std::string sourceString);
	OCStackResult SetDeviceInfo(std::string contentType, std::string dateOfManufacture,
	                std::string deviceName, std::string deviceUUID, std::string firmwareVersion,
	                std::string hostName, std::string manufacturerName, std::string manufacturerUrl,
	                std::string modelNumber, std::string platformVersion, std::string supportUrl,
	                std::string version);
public:
	IoTServer();
	~IoTServer();
    void init();
    void run();
    void setupResources();
    void createResource(string Uri, string Type, EntityHandler Cb, OCResourceHandle& Handle);

protected:
    void startPlatform();
    OCEntityHandlerResult testEntityHandler(shared_ptr<OCResourceRequest> Request);
    OCRepresentation getTestRepresentation();
};


#endif // IOTSERVER_H
