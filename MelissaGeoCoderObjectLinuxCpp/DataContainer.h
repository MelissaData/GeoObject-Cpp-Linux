#include<iostream>

using namespace std;

#ifndef DATACONTAINER_H
#define DATACONTAINER_H

/**
 * Data holder for a single record: carries the input ZIP in and the result codes out.
 */
class DataContainer
{
public:
	// Input: the ZIP code to look up.
	char Zip[20] = "";

	// Output: comma-separated result codes from GetResults().
	string ResultCodes;

	DataContainer() {};
};

#endif