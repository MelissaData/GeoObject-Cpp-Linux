/**
 * @file Main.cpp
 * @brief GeoCoder Object enables you to access geographic data using your ZIP Code and optional
 * Plus4. This allows you to obtain latitude and longitude geographic coordinates, census
 * tract and block numbers, as well as county name and FIPS numbers.
 *
 * High-level flow of this sample:
 *   1. SETUP     - create an mdGeo instance, hand it the license string and the
 *                  paths to the data files, then InitializeDataFiles() (one time).
 *   2. INPUT     - supply a ZIP via SetInputParameter("Zip", ...).
 *   3. PROCESS   - FindGeo() looks up the geographic data for that ZIP.
 *   4. READ      - pull the results back out with the Get* getters
 *                  (GetLatitude, GetLongitude, GetCountyName, GetPlaceName, ...).
 *   5. INTERPRET - GetResults() returns comma-separated result codes describing
 *                  what the object did/found; each code has a human description.
 *
 * The pieces of this sample map onto that flow:
 *   - main / ParseArguments / RunAsConsole : console harness (argument parsing + the interactive loop).
 *   - GeoObject       : thin wrapper around mdGeo that owns setup + the call sequence.
 *   - DataContainer   : plain holder for one record's input and output.
 *
 * Where mdGeo comes from:
 *   There is no generated wrapper source for C++. mdGeo.h and
 *   mdEnums.h declare the API, and libmdGeo.so carries the implementation. The
 *   accompanying MelissaGeoCoderObjectLinuxCpp.sh script downloads all three on every run.
 *
 * Reference:
 *   Quickstart    : https://docs.melissa.com/on-premise-api/geocoder-object/geocoder-object-quickstart.html
 *   Release notes : https://releasenotes.melissa.com/on-premise-api/geocoder-object/
 *   Result codes  : https://docs.melissa.com/on-premise-api/geocoder-object/result-codes.html
 */

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

/**
 * Entry point. Reads the optional command-line arguments, then hands control to
 * RunAsConsole, which performs the actual GeoCoder Object setup and processing.
 *
 * @param argc The count of command-line arguments.
 * @param argv The raw command-line arguments.
 */
int main(int argc, char* argv[])
{
	// Populated by ParseArguments below.
	string license = "";
	string testZip = "";
	string dataPath = "";

	ParseArguments(license, testZip, dataPath, argc, argv);
	RunAsConsole(license, testZip, dataPath);

	return 0;
}

/**
 * Reads the supported command-line options into the reference parameters.
 *
 * Recognized flags (each followed by its value, e.g. "--zip 92688"):
 *   --license / -l   : the Melissa license string
 *   --dataPath / -d  : path to the GeoCoder Object data files
 *   --zip / -z       : a ZIP code to test in one-shot mode
 *
 * @param license  Receives the Melissa license string.
 * @param testZip  Receives the ZIP code to test in one-shot mode.
 * @param dataPath Receives the path to the GeoCoder Object data files.
 * @param argc     The count of command-line arguments to parse.
 * @param argv     The raw command-line arguments to parse.
 */
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

/**
 * Sets up the GeoCoder Object once, then drives the input -> process -> output cycle.
 *
 * In interactive mode (no --zip) it loops, asking for a new ZIP each pass until the
 * user answers "N". In one-shot mode (--zip supplied) it runs a single pass
 * and exits.
 *
 * @param license  The Melissa license string used to initialize the object.
 * @param testZip  A ZIP code to process in one-shot mode; if empty, the program prompts interactively.
 * @param dataPath Path to the GeoCoder Object data files.
 */
void RunAsConsole(string license, string testZip, string dataPath)
{
	cout << "\n\n=========== WELCOME TO MELISSA GEOCODER OBJECT LINUX C++ ===========\n" << endl;
	
	// Construct the wrapper. This is where the object is licensed, pointed at the
	// data files, and initialized (see the GeoObject constructor below).
	GeoObject* geoObject = new GeoObject(license, dataPath);

	bool shouldContinueRunning = true;

	while (shouldContinueRunning)
	{
		// Holder for this pass's input and result codes.
		DataContainer dataContainer = DataContainer();

		if (testZip.empty())
		{
			// Interactive mode: prompt the user for a ZIP code.
			cout << "\nFill in each value to see the GeoCoder Object results" << endl;
			cout << "Zip: ";

			string input;
			getline(cin, input);

			strcpy(dataContainer.Zip, input.c_str());
		}
		else
		{
			// One-shot mode: use the ZIP passed on the command line.
			strcpy(dataContainer.Zip, testZip.c_str());
		}

		// Print user input
		cout << "\n============================== INPUTS ==============================\n" << endl;
		cout << "\t                      Zip: " + string(dataContainer.Zip)                << endl;

		// Execute GeoCoder Object
		// Runs the FindGeo lookup and stores the result codes on dataContainer
		geoObject->ExecuteObjectAndResultCodes(dataContainer);

		// Print output
		// Each Get* getter below returns one component the object produced for the most
		// recently processed ZIP. These read directly from the mdGeo instance, which
		// still holds the results from the Execute call above.
		cout << "\n============================== OUTPUT ==============================\n"                  << endl;
		cout << "\n\tGeoCoder Object Information:"                                                          << endl;
		cout << "\t                PlaceName: " + string(geoObject->mdGeoObj->GetPlaceName())               << endl;
		cout << "\t                   County: " + string(geoObject->mdGeoObj->GetCountyName())              << endl;
		cout << "\t    CountySubdivisionName: " + string(geoObject->mdGeoObj->GetCountySubdivisionName())   << endl;
		cout << "\t                 TimeZone: " + string(geoObject->mdGeoObj->GetTimeZone())                << endl;
		cout << "\t                 Latitude: " + string(geoObject->mdGeoObj->GetLatitude())                << endl;
		cout << "\t                Longitude: " + string(geoObject->mdGeoObj->GetLongitude())               << endl;
		cout << "\t             Result Codes: " + dataContainer.ResultCodes                                 << endl;

		// Result codes come back as a single comma-separated string (e.g. "GS05,GS06").
		// Split it and ask the object for a readable description of each code.
		// ResultCodeDescriptionLong requests the long-form text; a short form is also
		// available via ResultCodeDescriptionShort.
		list<string> rs = SplitResultCodes(dataContainer.ResultCodes, ",");
		list<string>::iterator it;

		for (it = rs.begin(); it != rs.end(); it++)
		{
			printf("        %s: %s", it->c_str(), geoObject->mdGeoObj->GetResultCodeDescription(it->c_str(), geoObject->mdGeoObj->ResultCodeDescriptionLong));
			cout << endl;
		}

		bool isValid = false;

		// In one-shot mode there is nothing more to do after a single pass: mark the
		// input handled and stop the outer loop.
		if (!testZip.empty()) 
		{
			isValid = true;
			shouldContinueRunning = false;
		}

		// Interactive mode: ask whether to process another ZIP. Keep prompting until
		// we get a valid Y/N. "N" ends the program; "Y" falls through to another pass.
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

/**
 * Splits the comma-separated result-code string into individual codes.
 *
 * @param s         The result-code string (e.g. "GS05,GS06").
 * @param delimiter The delimiter string to split on.
 * @return A list holding each individual result code.
 */
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
