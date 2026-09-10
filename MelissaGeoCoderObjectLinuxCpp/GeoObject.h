#include<iostream>
#include <string>

#include "mdGeo.h"
#include "DataContainer.h"

using namespace std;

#ifndef GEOOBJECT_H
#define GEOOBJECT_H

/**
 * Wrapper that owns a single Melissa GeoCoder Object instance and encapsulates the two
 * things every Melissa object needs: one-time setup (license + data files) and the
 * per-record processing sequence. Reuse one instance across many lookups; do NOT
 * re-initialize per lookup.
 */
class GeoObject
{
public:
	// Path to the GeoCoder Object data files.
	string dataFilePath;

	// The underlying Melissa GeoCoder Object instance.
	mdGeo* mdGeoObj = new mdGeo;

	/**
	 * Performs the mandatory one-time setup, in this required order:
	 *   1. SetLicenseString     - authorize the object.
	 *   2. SetPathTo*DataFiles  - tell it where each set of data files lives.
	 *   3. InitializeDataFiles  - load the data into memory.
	 *
	 * @param license  The Melissa license string used to authorize the object.
	 * @param dataPath Path to the folder containing the GeoCoder Object data files.
	 */
	GeoObject(string license, string dataPath);

	/**
	 * Runs the full GeoCoder Object processing sequence for one ZIP and captures its
	 * result codes. This is the canonical per-record call pattern to copy into your
	 * own application:
	 *   SetInputParameter -> FindGeo -> GetResults
	 *
	 * @param data The record to process. Its Zip is read as input, and ResultCodes is
	 *             populated with this run's result codes.
	 */
	void ExecuteObjectAndResultCodes(DataContainer& data);
};

#endif
