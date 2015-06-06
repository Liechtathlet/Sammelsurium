//
// IOSM_Starter : Demo application for raspberry pi
//

#include <unistd.h>
#include <signal.h>

#include "IoTServer.h"

#include "src/resources/AbstractResource.h"

#include "OCPlatform.h"
#include "OCApi.h"


IoTServer::IoTServer()
{
	//Initialize server

	cout << "\n\n*******************************************\n";
	cout << "* IOSM for Raspberry-PI\t\t\t*\n";
	cout << "*******************************************\n";
}

IoTServer::~IoTServer(){
	delete(test_res);
}

void IoTServer::run()
{
	//Run server
	cout << "Starting up ...\n";

	//Start platform
    startPlatform();

    //Register device
    OCStackResult result = SetDeviceInfo(contentType, dateOfManufacture, deviceName,
            deviceUUID, firmwareVersion, hostName, manufacturerName, manufacturerUrl,
            modelNumber, platformVersion, supportUrl, version);

    if(result != OC_STACK_OK)
    {
        std::cout << "Device Registration failed\n";
    }

    result = OCPlatform::registerDeviceInfo(deviceInfo);

    if(result != OC_STACK_OK)
    {
        std::cout << "Device Registration failed\n";
    }

    DeleteDeviceInfo();

    //Register Ressources
    setupResources();
}

void IoTServer::startPlatform()
{
	cout << "Starting OC-Platfrom...\n";

    uint16_t port = 0;

    std::string ipaddr = "0.0.0.0";

    cout << "Configure Platform...\n";
    m_platformConfig = make_shared<PlatformConfig>(ServiceType::InProc, ModeType::Server,
                  ipaddr, port, QualityOfService::LowQos);
    OCPlatform::Configure(*m_platformConfig);

    cout << "Port: " << (*m_platformConfig).port << endl;

}

void IoTServer::setupResources()
{
	cout << "Registrating resources ...\n";
	cout << "---------------------- ...\n\n";

	//Resource xyz
	cout << "Ressource: xyz \n";
	EntityHandler cb1 = bind(&IoTServer::testEntityHandler, this, placeholders::_1);
	createResource("/test", "Test.Test", cb1,m_testRes);

	test_res = new AbstractResource("/Blubb","Blubber.Blubb");
	//IoTObserverCb tempObsCb = bind(&IoTServer::temperatureObserverLoop, this);
	//m_temperatureObserverLoop = make_shared<IoTObserver>(tempObsCb);


}

void IoTServer::createResource(string Uri, string Type, EntityHandler Cb, OCResourceHandle& Handle)
{
	//Creat resource

	cerr << "Create Resource:" << Uri << ", Type: " << Type << endl;

    string resourceUri = Uri;
    string resourceType = Type;
    string resourceInterface = DEFAULT_INTERFACE;
    uint8_t resourceFlag = OC_DISCOVERABLE | OC_OBSERVABLE;

    //Register resource
    OCStackResult result = OCPlatform::registerResource(Handle, resourceUri, resourceType,
                                                        resourceInterface, Cb, resourceFlag);

    if (result != OC_STACK_OK)
        cerr << "Resource: Could not create " << Type << " resource" << endl;
    else
        cout << "Resource: Successfully created " << Type << " resource" << endl;
}

void IoTServer::DeleteDeviceInfo()
{
    delete[] deviceInfo.contentType;
    delete[] deviceInfo.dateOfManufacture;
    delete[] deviceInfo.deviceName;
    delete[] deviceInfo.deviceUUID;
    delete[] deviceInfo.firmwareVersion;
    delete[] deviceInfo.hostName;
    delete[] deviceInfo.manufacturerName;
    delete[] deviceInfo.manufacturerUrl;
    delete[] deviceInfo.modelNumber;
    delete[] deviceInfo.platformVersion;
    delete[] deviceInfo.supportUrl;
    delete[] deviceInfo.version;
}

void IoTServer::DuplicateString(char ** targetString, std::string sourceString)
{
    *targetString = new char[sourceString.length() + 1];
    strncpy(*targetString, sourceString.c_str(), (sourceString.length() + 1));
}

OCStackResult IoTServer::SetDeviceInfo(std::string contentType, std::string dateOfManufacture,
                std::string deviceName, std::string deviceUUID, std::string firmwareVersion,
                std::string hostName, std::string manufacturerName, std::string manufacturerUrl,
                std::string modelNumber, std::string platformVersion, std::string supportUrl,
                std::string version)
{
    if(manufacturerName.length() > MAX_MANUFACTURER_NAME_LENGTH)
    {
        return OC_STACK_INVALID_PARAM;

    }

    if(manufacturerUrl.length() > MAX_MANUFACTURER_URL_LENGTH)
    {
        return OC_STACK_INVALID_PARAM;

    }

    try
    {
        DuplicateString(&deviceInfo.contentType, contentType);
        DuplicateString(&deviceInfo.dateOfManufacture, dateOfManufacture);
        DuplicateString(&deviceInfo.deviceName, deviceName);
        DuplicateString(&deviceInfo.deviceUUID, deviceUUID);
        DuplicateString(&deviceInfo.firmwareVersion, firmwareVersion);
        DuplicateString(&deviceInfo.hostName, hostName);
        DuplicateString(&deviceInfo.manufacturerName, manufacturerName);
        DuplicateString(&deviceInfo.manufacturerUrl, manufacturerUrl);
        DuplicateString(&deviceInfo.modelNumber, modelNumber);
        DuplicateString(&deviceInfo.platformVersion, platformVersion);
        DuplicateString(&deviceInfo.supportUrl, supportUrl);
        DuplicateString(&deviceInfo.version, version);
    }catch(exception &e)
    {
        std::cout<<"String Copy failed!!\n";
        return OC_STACK_ERROR;
    }

    return OC_STACK_OK;
}

OCEntityHandlerResult IoTServer::testEntityHandler(shared_ptr<OCResourceRequest> Request)
{
	//Set Default result
    OCEntityHandlerResult result = OC_EH_ERROR;

    //Check if it is a request
    if (Request)
    {
    	//Get reqeuest information
        string requestType = Request->getRequestType();
        int requestFlag = Request->getRequestHandlerFlag();
        if (requestFlag & RequestHandlerFlag::RequestFlag)
        {
        	//create response
            auto Response = std::make_shared<OC::OCResourceResponse>();
            Response->setRequestHandle(Request->getRequestHandle());
            Response->setResourceHandle(Request->getResourceHandle());

            //Handle get
            if (requestType == "GET")
            {
                cout << "GET request for test reading" << endl;
                if (Response)
                {
                    Response->setErrorCode(200);
                    Response->setResponseResult(OC_EH_OK);
                    Response->setResourceRepresentation(getTestRepresentation());
                    if (OCPlatform::sendResponse(Response) == OC_STACK_OK)
                    {
                        result = OC_EH_OK;
                    }
                }
            }
            else
            {
                Response->setResponseResult(OC_EH_ERROR);
                OCPlatform::sendResponse(Response);
                cerr << "Unsupported request type" << endl;
                return result;
            }
        }
        /*if (requestFlag & RequestHandlerFlag::ObserverFlag)
        {
            ObservationInfo observationInfo = Request->getObservationInfo();
            if (ObserveAction::ObserveRegister == observationInfo.action)
            {
                cout << "Starting observer for temperature sensor" << endl;
                m_temperatureObservers.push_back(observationInfo.obsId);
                m_temperatureObserverLoop->start();
            }
            else if (ObserveAction::ObserveUnregister == observationInfo.action)
            {
                m_temperatureObservers.erase(
                        remove(m_temperatureObservers.begin(), m_temperatureObservers.end(),
                               observationInfo.obsId),
                        m_temperatureObservers.end());
            }
        }*/
    }
    return result;
}

OCRepresentation IoTServer::getTestRepresentation()
{
    m_testRepresentation.setValue("test", 5.0);
    return m_testRepresentation;
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
    IoTServer server;

    cout << "Press Ctrl-C to quit...." << endl;

    //Start server
    server.run();

    //Do nothing and wait...
    do
    {
        usleep(2000000);
    }
    while (quit != 1);

    return 0;
}
