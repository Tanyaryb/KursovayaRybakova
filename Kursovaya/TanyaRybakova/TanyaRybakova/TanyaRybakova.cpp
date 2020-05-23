#include <iostream>
#include "Flow.h"
#include <string>
#include <iostream>

using namespace std;

void printMaxFlow(string &file)
{
	Flow flow(file);

	cout << "MaxFlow: " << flow.getMaxFlow() << endl;
}

int main()
{

	for (int i = 1; i < 8; ++i)
	{
		string fileName = "INPUT" + to_string(i) + ".txt";

		printMaxFlow(fileName);
	}

	return 0;
}