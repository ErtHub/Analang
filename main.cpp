#include <iostream>
#include <cstdlib>
#include "Scan.h"
#include "Source.h"
#include "Parser.h"

using namespace std;

int Trace::call_level = 0;
int Trace::trace_on = 0;
int Trace::show_symbols = 0;

int options = 0;

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		cout << "Niewystarczajace argumenty wykonania!" << endl;
		return -1;
	}
	string filename(argv[1]);
	//std::cout << "Nazwa pliku:" << endl;
	//cin >> filename;
	Source src(filename);
	Scan scn(src);
	Execution e;
	Parser par(scn, e);
	Synchronize::p = &par;
	while (par.NextExecutable());
	e.run();
	system("pause");
	return 0;
}