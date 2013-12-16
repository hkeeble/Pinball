#include <iostream>
#include "gameData.h"

using namespace std;

enum programFeatures
{
	none = 0,
	openAndView = 1,
	openAndCompare = 2,
	quit = 3
};

int main()
{
	int menuChoice = none;
	bool pRunning = true;

	string fileName1;
	string fileName2;
	GameData diff;
	GameData data1;
	GameData data2;

	cout << "Pinball Game Analyzer 1.0" << endl;

	while (pRunning)
	{
		while (menuChoice <= 0 || menuChoice > 3)
		{
			cout << "What do you wish to do?" << endl;
			cout << "1 - Open and View Game Data" << endl;
			cout << "2 - Open Two Sets of Data, and find difference between values." << endl;
			cout << "3 - Quit" << endl;
			cin >> menuChoice;
			cout << endl;
		}

		switch (menuChoice)
		{
		case openAndView:
			fileName1 = "";
			data1 = GameData();

			cout << "Enter name of data file: ";
			cin >> fileName1;
			cout << endl << "Reading data..." << endl;
			data1.ReadData(fileName1);

			if (data1.balls != nullptr)
				data1.OutputData();
			else
				cout << "Unable to read data from this file." << endl << endl;
			break;
		case openAndCompare:
			fileName1 = "";
			fileName2 = "";
			data1 = GameData();
			data2 = GameData();

			cout << "Enter name of data file 1:";
			cin >> fileName1;
			cout << "Enter name of data file 2:";
			cin >> fileName2;

			cout << "Reading data file 1..." << endl;
			data1.ReadData(fileName1);
			if (data1.balls == nullptr)
			{
				cout << "Unable to read data from this file." << endl << endl;
				break;
			}

			cout << "Reading data file 2..." << endl;
			data2.ReadData(fileName2);
			if (data2.balls == nullptr)
			{
				cout << "Unable to read data from this file." << endl << endl;
				break;
			}

			cout << "Calculating Differences..." << endl;
			cout << "The following values correspond to the differences between the two sets of data passed in." << endl;
			diff = data1.Difference(data2);
			diff.OutputData();
			break;

		case quit:
			pRunning = false;
			break;

		}

		if (pRunning)
		{
			cin.ignore();
			cin.clear();
			cout << "Press Enter To Continue." << endl << endl;
			cin.get();
		}

		menuChoice = none;
	}
}