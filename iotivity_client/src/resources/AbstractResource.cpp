
#include "AbstractResource.h"

#include "OCPlatform.h"
#include "OCApi.h"

//Constructor
AbstractResource::AbstractResource(string Uri, string Type) : resourceUri(Uri), resourceType(Type)
{
	//Create resource

	cout << "\n\n*******************************************\n";
	cout << "Creating Resource: " << Uri << ", Type: " << Type << endl;

	string resourceInterface = "core.rbpi.resources";
	uint8_t resourceFlag = OC_DISCOVERABLE | OC_OBSERVABLE;

	//Bind
	EntityHandler cb = bind(&AbstractResource::resourceEntityHandler, this, placeholders::_1);

	//Register resource
	OCStackResult result = OCPlatform::registerResource(resourceHandle, resourceUri, resourceType,
														resourceInterface, cb, resourceFlag);

	if (result != OC_STACK_OK)
		cerr << "Resource: Could not create " << Type << " resource" << endl;
	else
		cout << "Resource: Successfully created " << Type << " resource" << endl;

}

//Destructor
AbstractResource::~AbstractResource()
{
	//Destroy Resource
	cout << "Destroy Resource: ";
}

//Get resource representation
OCRepresentation AbstractResource::getResourceRepresentation()
{
	resourceRepresentation.setValue("test", 5.0);
    return resourceRepresentation;
}

void AbstractResource::handleGet(shared_ptr<OC::OCResourceResponse> Response)
{
	cout << "GET request" << endl;

	if (Response)
	{
		Response->setErrorCode(200);
		Response->setResponseResult(OC_EH_OK);
		Response->setResourceRepresentation(getResourceRepresentation());
	}
}

void AbstractResource::handlePut(shared_ptr<OC::OCResourceResponse> Response)
{
	cout << "PUT request" << endl;

}

void AbstractResource::handlePost(shared_ptr<OC::OCResourceResponse> Response)
{
	cout << "POST request" << endl;

}

void AbstractResource::handleDelete(shared_ptr<OC::OCResourceResponse> Response)
{
	cout << "DELETE request" << endl;

}

OCEntityHandlerResult AbstractResource::resourceEntityHandler(shared_ptr<OCResourceRequest> Request)
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
            	handleGet(Response);

            	if (OCPlatform::sendResponse(Response) == OC_STACK_OK)
            			{
            				result = OC_EH_OK;
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


