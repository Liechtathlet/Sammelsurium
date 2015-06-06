/*
 * AbstractResource.h
 *
 *  Created on: Jun 4, 2015
 *      Author: developer
 */

#ifndef ABSTRACTRESOURCE_H_
#define ABSTRACTRESOURCE_H_

#include <string>
#include <cstdlib>
#include "OCPlatform.h"
#include "OCApi.h"

using namespace OC;
using namespace std;

class AbstractResource
{
	shared_ptr<PlatformConfig> platformConfig;

	OCRepresentation resourceRepresentation;
	OCResourceHandle resourceHandle;

	string resourceUri;
	string resourceType;

    std::vector<std::string> resourceTypes;
    std::vector<std::string> resourceInterfaces;

public:
	AbstractResource(string Uri, string Type);
	virtual ~AbstractResource();

	virtual OCRepresentation getResourceRepresentation();
	virtual void handleGet(shared_ptr<OC::OCResourceResponse> Response);
	virtual void handlePost(shared_ptr<OC::OCResourceResponse> Response);
	virtual void handleDelete(shared_ptr<OC::OCResourceResponse> Response);
	virtual void handlePut(shared_ptr<OC::OCResourceResponse> Response);

protected:
    OCEntityHandlerResult resourceEntityHandler(shared_ptr<OCResourceRequest> Request);

};


#endif /* ABSTRACTRESOURCE_H_ */
