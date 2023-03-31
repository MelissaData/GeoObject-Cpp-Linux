#include <iostream>
#include <cstring>
#include <cstdlib>
#include <list>

#include "mdGeo.h"
#include "GeoObject.h"
#include "DataContainer.h"

using namespace std;

// function declarations
void ParseArguments(string& license, string& testZip, string& dataPath, int argc, char** argv);
void RunAsConsole(string license, string testZip, string dataPath);
list<string> SplitResultCodes(string s, string delimiter);

int main(int argc, char* argv[])
{
	// Variables
	string license = "";
	string testZip = "";
	string dataPath = "";

	ParseArguments(license, testZip, dataPath, argc, argv);
	RunAsConsole(license, testZip, dataPath);

	return 0;
}

void ParseArguments(string& license, string& testZip, string& dataPath, int argc, char* argv[])
{
	for (int i = 1; i < argc; i++)
	{
		if (string(argv[i]) == "--license" || string(argv[i]) == "-l")
		{
			if (argv[i + 1] != NULL)
			{
				license = argv[i + 1];
			}
		}
		if (string(argv[i]) == "--zip" || string(argv[i]) == "-z")
		{
			if (argv[i + 1] != NULL)
			{
				testZip = argv[i + 1];
			}
		}
		if (string(argv[i]) == "--dataPath" || string(argv[i]) == "-d")
		{
			if (argv[i + 1] != NULL)
			{
				dataPath = argv[i + 1];
			}
		}
	}
}

void RunAsConsole(string license, string testZip, string dataPath)
{
	cout << "\n\n=========== WELCOME TO MELISSA GEOCODER OBJECT LINUX C++ ===========\n" << endl;
	
	GeoObject* geoObject = new GeoObject(license, dataPath);

	bool shouldContinueRunning = true;

	while (shouldContinueRunning)
	{
		DataContainer dataContainer = DataContainer();

		if (testZip.empty())
		{
			cout << "\nFill in each value to see the GeoCoder Object results" << endl;
			cout << "Zip: ";

			string input;
			getline(cin, input);

			strcpy(dataContainer.Zip, input.c_str());
		}
		else
		{
			strcpy(dataContainer.Zip, testZip.c_str());
		}

		// Print user input
		cout << "\n============================== INPUTS ==============================\n" << endl;
		cout << "\t                      Zip: " + string(dataContainer.Zip)                << endl;

		// Execute GeoCoder Object
		geoObject->ExecuteObjectAndResultCodes(dataContainer);

		// Print output
		cout << "\n============================== OUTPUT ==============================\n"                  << endl;
		cout << "\n\tGeoCoder Object Information:"                                                          << endl;
		cout << "\t                PlaceName: " + string(geoObject->mdGeoObj->GetPlaceName())               << endl;
		cout << "\t                   County: " + string(geoObject->mdGeoObj->GetCountyName())              << endl;
		cout << "\t    CountySubdivisionName: " + string(geoObject->mdGeoObj->GetCountySubdivisionName())   << endl;
		cout << "\t                 TimeZone: " + string(geoObject->mdGeoObj->GetTimeZone())                << endl;
		cout << "\t                 Latitude: " + string(geoObject->mdGeoObj->GetLatitude())                << endl;
		cout << "\t                Longitude: " + string(geoObject->mdGeoObj->GetLongitude())               << endl;
		cout << "\t             Result Codes: " + dataContainer.ResultCodes                                 << endl;

		list<string> rs = SplitResultCodes(dataContainer.ResultCodes, ",");
		list<string>::iterator it;

		for (it = rs.begin(); it != rs.end(); it++)
		{
			printf("        %s: %s", it->c_str(), geoObject->mdGeoObj->GetResultCodeDescription(it->c_str(), geoObject->mdGeoObj->ResultCodeDescriptionLong));
			cout << endl;
		}

		bool isValid = false;
		if (!testZip.empty()) 
		{
			isValid = true;
			shouldContinueRunning = false;
		}
		while (!isValid)
		{
			string testAnotherResponse;

			cout << "\nTest another zip? (Y/N)" << endl;
			getline(cin, testAnotherResponse);

			if (!testAnotherResponse.empty())
			{
				if (testAnotherResponse == "y" || testAnotherResponse == "Y")
				{
					isValid = true;
				}
				else if (testAnotherResponse == "n" || testAnotherResponse == "N")
				{
					isValid = true;
					shouldContinueRunning = false;
				}
				else
				{
					cout << "Invalid Response, please respond 'Y' or 'N'" << endl;
				}
			}
		}
	}
	cout << "\n=============== THANK YOU FOR USING MELISSA C++ OBJECT =============\n" << endl;
}

/// <summary>
/// This function splits the resultCodes based on a delimiter
/// </summary>
/// <param name="s">the resultCode string</param>
/// <param name="delimiter"the delimiter string></param>
/// <returns></returns>
list<string> SplitResultCodes(string s, string delimiter) {
	list<string> resultCodes;

	size_t pos = 0;
	string token;

	while ((pos = s.find(delimiter)) != string::npos) {
		token = s.substr(0, pos);
		resultCodes.push_back(token);
		s.erase(0, pos + delimiter.length());
	}

	// push back the last resultCode
	resultCodes.push_back(s);

	return resultCodes;
}
