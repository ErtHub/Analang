#include <iostream>
#include <cstdlib>
#include "Scan.h"
#include "Source.h"

using namespace std;

int options = 0;

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
		case ifsy: {cout << "ifsy"; break; }
		case nothingsy: {cout << "nothingsy"; break; }
		case fracsy: {cout << "fracsy"; break; }
		case strsy: {cout << "strsy"; break; }
		case boolsy: {cout << "boolsy"; break; }
		case truesy: {cout << "truesy"; break; }
		case falsesy: {cout << "falsesy"; break; }
		case varsy: {cout << "varsy"; break; }
		case elsesy: {cout << "elsesy"; break; }
		case returnsy: {cout << "returnsy"; break; }
		case inputsy: {cout << "inputsy"; break; }
		case whilesy: {cout << "whilesy"; break; }
		case printsy: {cout << "printsy"; break; }
		case funcsy: {cout << "funcsy"; break; }
		case ident: {cout << "identsy"; break; }
		case fracconst: {cout << "fracconst"; break; }
		case strconst: {cout << "strconst"; break; }
		case mulop: {cout << "mulop"; break; }
		case addop: {cout << "addop"; break; }
		case subop: {cout << "subop"; break; }
		case orop: {cout << "orop"; break; }
		case andop: {cout << "andop"; break; }
		case divop: {cout << "divop"; break; }
		case endsy: {cout << "endsy"; break; }
		case notop: {cout << "notop"; break; }
		case beginsy: {cout << "beginsy"; break; }
		case ltop: {cout << "ltop"; break; }
		case gtop: {cout << "gtop"; break; }
		case eqop: {cout << "eqop"; break; }
		case rparent: {cout << "rparent"; break; }
		case lparent: {cout << "lparent"; break; }
		case comma: {cout << "comma"; break; }
		case period: {cout << "period"; break; }
		case colon: {cout << "colon"; break; }
		case becomes: {cout << "becomes"; break; }
		default: {cout << "Koniec pliku lub nieobs³ugiwany atom."; break; }
		}
		cout << endl;
	} while (symbol != others);
	system("pause");
	return 0;
}