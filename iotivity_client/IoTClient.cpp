//
// IOSM_Starter : Demo application for raspberry pi
//

#include <unistd.h>
#include <signal.h>

#include "IoTClient.h"

#include "src/resources/AbstractResource.h"

#include "OCPlatform.h"
#include "OCApi.h"


IoTClient::IoTClient()
{
	//Initialize client

	cout << "\n\n*******************************************\n";
	cout << "* IOTC for Linux\t\t\t*\n";
	cout << "*******************************************\n";
}

IoTClient::~IoTClient(){

}

void IoTClient::run()
{
	//Run client
	cout << "Starting up ...\n";

	//Start platform
    startPlatform();

    findResource();

    string host = "";
    string uri = "coap://224.0.1.187/oc/core/d";
    OCStackResult result = OCPlatform::getDeviceInfo(host,uri, deviceDiscoveryCallback);
    std::cout<< "Querying for device information... " <<std::endl;

}

void IoTClient::receivedDeviceInfo(const OCRepresentation& rep)
{
    std::cout << "\nDevice Information received ---->\n";

    std::string contentType;
    std::string dateOfManufacture;
    std::string deviceName;
    std::string deviceUUID;
    std::string firmwareVersion;
    std::string hostName;
    std::string manufacturerName;
    std::string manufacturerUrl;
    std::string modelNumber;
    std::string platformVersion;
    std::string supportUrl;
    std::string version;

    if(rep.getValue("ct", contentType))
    {
        std::cout << "Content Type: " << contentType << std::endl;
    }

    if(rep.getValue("mndt", dateOfManufacture))
    {
        std::cout << "Date of manufacture: " << dateOfManufacture << std::endl;
    }

    if(rep.getValue("dn", deviceName))
    {
        std::cout << "Device Name: " << deviceName << std::endl;
    }

    if(rep.getValue("di", deviceUUID))
    {
        std::cout << "Device UUID: " << deviceUUID << std::endl;
    }

    if(rep.getValue("mnfv", firmwareVersion))
    {
        std::cout << "Firmware Version: " << firmwareVersion << std::endl;
    }

    if(rep.getValue("hn", hostName))
    {
        std::cout << "Host Name: " << hostName << std::endl;
    }

    if(rep.getValue("mnmn", manufacturerName))
    {
        std::cout << "Manufacturer Name: " << manufacturerName << std::endl;
    }

    if(rep.getValue("mnml", manufacturerUrl))
    {
        std::cout << "Manufacturer Url: " << manufacturerUrl << std::endl;
    }

    if(rep.getValue("mnmo", modelNumber))
    {
        std::cout << "Model No. : " << modelNumber << std::endl;
    }

    if(rep.getValue("mnpv", platformVersion))
    {
        std::cout << "Platform Version: " << platformVersion << std::endl;
    }

    if(rep.getValue("mnsl", supportUrl))
    {
        std::cout << "Support URL: " << supportUrl << std::endl;
    }

    if(rep.getValue("icv", version))
    {
        std::cout << "Version: " << version << std::endl;
    }
}

void IoTClient::startPlatform()
{
	cout << "Starting OC-Platfrom...\n";

    uint16_t port = 0;

    std::string ipaddr = "0.0.0.0";

    cout << "Configure Platform...\n";
    m_platformConfig = make_shared<PlatformConfig>(ServiceType::InProc, ModeType::Client,
                  ipaddr, port, QualityOfService::LowQos);
    OCPlatform::Configure(*m_platformConfig);

    cout << "Port: " << (*m_platformConfig).port << endl;

    resourceDiscoveryCallback = bind(&IoTClient::discoveredResource, this, placeholders::_1);
    deviceDiscoveryCallback = bind(&IoTClient::receivedDeviceInfo, this, placeholders::_1);

}

void IoTClient::discoveredResource(shared_ptr<OCResource> Resource)
{
	cout << "Discovered resource......" << endl;
    try
    {
        if (Resource)
        {
            string resourceUri = Resource->uri();
            string hostAddress = Resource->host();

            cout << "\nFound Resource" << endl << "Resource Types:" << endl;
            for (auto& resourceTypes : Resource->getResourceTypes())
            {
                cout << "\t" << resourceTypes << endl;
            }

            cout << "Resource Interfaces: " << endl;
            for (auto& resourceInterfaces : Resource->getResourceInterfaces())
            {
                cout << "\t" << resourceInterfaces << endl;
            }
            cout << "Resource uri: " << resourceUri << endl;
            cout << "host: " << hostAddress << endl;

            /*if (resourceUri == TEMPERATURE_RESOURCE_ENDPOINT)
            {
                m_temperatureSensor = make_shared<TemperatureSensor>(Resource);
            }
            else if (resourceUri == LIGHT_RESOURCE_ENDPOINT)
            {
                m_ambientLightSensor = make_shared<AmbientLight>(Resource);
            }
            else if (resourceUri == LED_RESOURCE_ENDPOINT)
            {
                m_platformLED = make_shared<LED>(Resource);
            }*/
        }
    }
    catch (OCException& ex)
    {
        cerr << "Caught exception in discoveredResource: " << ex.reason() << endl;
    }
}

void IoTClient::findResource()
{

	cout << "Looking for resources......" << endl;

    string coap_multicast_discovery = string(OC_WELL_KNOWN_QUERY "?if=core.rbpi.resources");
    OCPlatform::findResource("", coap_multicast_discovery.c_str(), resourceDiscoveryCallback,
                             OC::QualityOfService::LowQos);
}

int quit = 0;

void handle_signal(int signal)
{
    quit = 1;
}

int main(int argc, char* argv[])
{
	//Set signal action for aborting programm on ctrl + c
    struct sigaction sa;
    sigfillset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = handle_signal;
    sigaction(SIGINT, &sa, NULL);

    //Create server
    IoTClient client;

    cout << "Press Ctrl-C to quit...." << endl;

    //Start client
    client.run();

    //Do nothing and wait...
    do
    {
    	//client.findResource();
        usleep(2000000);
    }
    while (quit != 1);

    return 0;
}
