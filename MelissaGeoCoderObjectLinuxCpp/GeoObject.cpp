#include "GeoObject.h"

GeoObject::GeoObject(string license, string dataPath)
{
	// Set license string and set path to data files
	mdGeoObj->SetLicenseString(license.c_str());
	dataFilePath = dataPath;

	// Point the object at the GeoCoder data files (US GeoCode, Canada, and GeoPoint).
	mdGeoObj->SetPathToGeoCodeDataFiles(dataFilePath.c_str());
	mdGeoObj->SetPathToGeoCanadaDataFiles(dataFilePath.c_str());
	mdGeoObj->SetPathToGeoPointDataFiles(dataFilePath.c_str());

	// Load the data files. The returned ProgramStatus reports whether initialization succeeded.
	// If you see a different date than expected, check your license string and either download the new data files
	// or use the Melissa Updater program to update your data files.
	mdGeo::ProgramStatus pStatus = mdGeoObj->InitializeDataFiles();

	// If an issue occurred, please investigate the common causes.
	// Common causes: an invalid/expired license, or missing/wrong-path data files.
	if (pStatus != mdGeo::ProgramStatus::ErrorNone)
	{
		cout << "Failed to Initialize Object." << endl;
		cout << pStatus << endl;
		return;
	}

	// Diagnostic information, handy for confirming the object loaded the data you expect:

	// Build date of the data files
	cout << "                    DataBase Date: " + string(mdGeoObj->GetDatabaseDate()) << endl;

	// When the license stops working
	cout << "                  Expiration Date: " + string(mdGeoObj->GetLicenseExpirationDate()) << endl;

	/**
	 * This number should match with the file properties of the Melissa Object binary file.
	 * If TEST appears with the build number, there may be a license key issue.
	 */
	cout << "                   Object Version: " + string(mdGeoObj->GetBuildNumber()) << endl;
}

// This will call the lookup function to process the input zip as well as generate the result codes
void GeoObject::ExecuteObjectAndResultCodes(DataContainer& data)
{
	// Supply the ZIP to look up
	mdGeoObj->SetInputParameter("Zip", data.Zip);

	// Look up the geographic data for that ZIP
	mdGeoObj->FindGeo();

	// Collect the result codes for this run
	// ResultsCodes explain any issues GeoCoder Object has with the object.
	// List of result codes for GeoCoder Object
	// https://docs.melissa.com/on-premise-api/geocoder-object/result-codes.html
	data.ResultCodes = mdGeoObj->GetResults();
}
