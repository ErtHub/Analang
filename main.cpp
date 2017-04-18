#include <iostream>
#include <cstdlib>
#include "Scan.h"

using namespace std;

int main()
{
	string filename;
	cout << "Nazwa pliku:" << endl;
	cin >> filename;
	Source src(filename);
	Scan scn(src);
	SymType symbol;
	//cout << "This program is a stub. Don't expect it to do anything." << endl;
	do
	{
		symbol = scn.NextSymbol();
		switch (symbol)
		{
		case ifsy: cout << "ifsy";
			case nothingsy: cout << "nothingsy";
			case fracsy: cout << "fracsy";
			case strsy: cout << "strsy";
			case boolsy: cout << "boolsy";
			case varsy: cout << "varsy";
			case elsesy: cout << "elsesy";
			case inputsy: cout << "inputsy";
			case whilesy: cout << "whilesy";
			case printsy: cout << "printsy";
			case funcsy: cout << "funcsy";
			case ident: cout << "identsy";
			case fracconst: cout << "fracconst";
			case strconst: cout << "strconst";
			case mulop: cout << "mulop";
			case addop: cout << "addop";
			case subop: cout << "subop";
			case orop: cout << "orop";
			case andop: cout << "andop";
			case divop: cout << "divop";
			case endsy: cout << "endsy";
			case notop: cout << "notop";
			case beginsy: cout << "beginsy";
			case ltop: cout << "ltop";
			case gtop: cout << "gtop";
			case eqop: cout << "eqop";
			case rparent: cout << "rparent";
			case lparent: cout << "lparent";
			case comma: cout << "comma";
			case period: cout << "period";
			case colon: cout << "colon";
			default: cout << "Koniec pliku lub nieobs³ugiwany atom.";
		}
		cout << endl;
	} while (symbol != others);
	system("pause");
	return 0;
}