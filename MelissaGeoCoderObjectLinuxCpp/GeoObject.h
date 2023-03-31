#include<iostream>
#include <string>

#include "mdGeo.h"
#include "DataContainer.h"

using namespace std;

#ifndef GEOOBJECT_H
#define GEOOBJECT_H

class GeoObject
{
public:
	// Path to GeoCoder Object data files (.dat, etc)
	string dataFilePath;

	// Create instance of Melissa GeoCoder Object
	mdGeo* mdGeoObj = new mdGeo;

	GeoObject(string license, string dataPath);

	// This will call the lookup function to process the input zip as well as generate the result codes
	void ExecuteObjectAndResultCodes(DataContainer& data);
};

#endif
