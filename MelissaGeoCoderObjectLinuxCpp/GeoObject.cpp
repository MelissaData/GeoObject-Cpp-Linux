#include "GeoObject.h"

GeoObject::GeoObject(string license, string dataPath)
{
	// Set license string and set path to data files (.dat, etc)
	mdGeoObj->SetLicenseString(license.c_str());
	dataFilePath = dataPath;

	// Set data paths for objects
	mdGeoObj->SetPathToGeoCodeDataFiles(dataFilePath.c_str());
	mdGeoObj->SetPathToGeoCanadaDataFiles(dataFilePath.c_str());
	mdGeoObj->SetPathToGeoPointDataFiles(dataFilePath.c_str());

	// If you see a different date than expected, check your license string and either download the new data files or use the Melissa Updater program to update your data files.  
	mdGeo::ProgramStatus pStatus = mdGeoObj->InitializeDataFiles();

	if (pStatus != mdGeo::ProgramStatus::ErrorNone)
	{
		cout << "Failed to Initialize Object." << endl;
		cout << pStatus << endl;
		return;
	}

	cout << "                    DataBase Date: " + string(mdGeoObj->GetDatabaseDate()) << endl;
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
	mdGeoObj->SetInputParameter("Zip", data.Zip);

	mdGeoObj->FindGeo();
	data.ResultCodes = mdGeoObj->GetResults();

	// ResultsCodes explain any issues GeoCoder Object has with the object.
	// List of result codes for GeoCoder Object
	// https://wiki.melissadata.com/?title=Result_Code_Details#GeoCoder_Object
}
