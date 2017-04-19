// Modu� Scan.cpp
// =================
// Defnicje funkcji sk�adowych klasy Scan, tabeli leksem�w AT
// i tabeli s��w kluczowych.
//
#include "Scan.h"

char *AT[MAXSYM+1] =	// Tabela Atom�w (dla komunikat�w o b��dach
{
  "if",    "nothing",     "frac",    "str",  "bool",
  "var",    "else", "true", "false", "input",
  "while",  "print", "func",
  "ident", "fracconst","strconst",

  "*", "+", "-", "||", "&&", "/", "}", "!", "{", "<", ">", "==", ")", "(",
  ",", ".",  ":", "=", "others"
};

Scan::KeyRec Scan::KT[NKEYS]= 	// Kolejno�� wg funkcji hash(keyword)
{
// Keyword		 Atom			hash(keyword)
//-------------------------------------------
  { "input",       inputsy  }, 	//  0
  { "func",     funcsy},  //  1
  { "print",        printsy   },  //  2
  { "frac",     fracsy},  //  3
  { "str",       strsy  },  //  4
  { "true", truesy },  //  5
  { "var",        varsy   },  //  6
  { "while",       whilesy  },  //  7
  { "else",        elsesy   },  //  8
  { "false",     falsesy},  //  9
  { "nothing",       nothingsy  },  //  10
  { "if",      ifsy },		//11
  {"bool",     boolsy }     //12
};

SymType Scan::NextSymbol()
{ 
  do
  {
    while(isspace(c)) Nextc();	// Pomi� znaki bia�e
    if(c==EOF) return others;
    if(c=='/')
	{
		atompos = src.GetPos(); // Mo�e by� divop albo komentarz
		Nextc();
		if (c == '*')
		{
			Nextc();
			while (true)
			{
				if (c == '*')
				{
					Nextc();
					if (c == '/')
					{
						Nextc();
						break;
					}
					else
						continue;
				}
				else if (c == EOF)
				{
					ScanError(UNTERMCOMM, "Niezako�czony komentarz");
					return others;
				}
				Nextc();
			}
		}
		else
			return divop;
	}
  } while(isspace(c) || c=='/');

  atompos=src.GetPos(); // B�dzie jaki� atom leksykalny

  //---Identyfikator lub s�owo kluczowe
  if(isalpha(c))
  { unsigned len, h;
    spell.clear();

    do
    { spell.push_back(c);
      Nextc();
    } while(isalnum(c));
    len = spell.size();
    h = hash(spell);
    if(KT[h].kw == spell)
      return KT[h].atom; 
    else return ident;
  }
  else

  //---Sta�a u�amkowa
  if(isdigit(c))
  {
	bool pointSeen = false;
	bool underscoreSeen = false;
	do
	{
		bool big = false; unsigned long long ul = 0;
		do
		{
			ul = ul * 10 + (c - '0');
			big = big || ul>INT_MAX;
			Nextc();
		} while (isdigit(c));
		if (big)
			ScanError(ICONST2BIG, "Przekroczony zakres sta�ej ca�kowitej");
		if (c == '.' && !pointSeen)
		{
			if (underscoreSeen)
				ScanError(FCONSTMALFORM, "Niepoprawny format sta�ej u�amkowej");
			else
			{
				pointSeen = true;
				fracconstant.wholes = ul;
				Nextc();
				if (!isdigit(c))
					ScanError(FCONSTMALFORM, "Niepoprawny format sta�ej u�amkowej");
			}
		}
		else if (c == '_' && !underscoreSeen)
		{
			underscoreSeen = true;
			fracconstant.nom = ul;
			Nextc();
			if (!isdigit(c))
				ScanError(FCONSTMALFORM, "Niepoprawny format sta�ej u�amkowej");
		}
		else
		{
			if (underscoreSeen)
			{
				if (!pointSeen)
					fracconstant.wholes = 0;
				fracconstant.denom = ul;
			}
			else if (!underscoreSeen && !pointSeen)
			{
				fracconstant.wholes = ul;
				fracconstant.nom = fracconstant.denom = 1;
			}
			else
				ScanError(FCONSTMALFORM, "Niepoprawny format sta�ej u�amkowej");
			return fracconst;
		}
		//intconstant = (int)ul;
	} while (isdigit(c));
  }
  else

  //---Pozosta�e atomy
  switch(c)
  {
    //----Sta�a znakowa
    case '"': Nextc();
			spell.clear();
			while (c != '"')
			{
				if (c == EOF)
				{
					ScanError(UNTERMSTRCONST, "Niezako�czony �a�cuch znakowy");
					return others;
				}
				spell.push_back(c);
				Nextc();
			}
			Nextc();
		    return strconst;
    //----Operatory 2 i 1 znakowe
	case '=': Nextc();
		    if (c=='=') { Nextc(); return eqop; }
		    return becomes;
	case '&': Nextc();
			if (c == '&') { Nextc(); return andop; }
			return others;
	case '|': Nextc();
			if (c == '|') { Nextc(); return orop; }
			return others;

    //----Operatory 1 znakowe
    case '+': Nextc(); return addop;
    case '-': Nextc(); return subop;
    case '*': Nextc(); return mulop;
    case '(': Nextc(); return lparent;
    case ')': Nextc(); return rparent;
    case ',': Nextc(); return comma;
	case '>': Nextc(); return gtop;
	case '<': Nextc(); return ltop;
	case '.': Nextc(); return period;
	case '!': Nextc(); return notop;
	case '{': Nextc(); return beginsy;
	case '}': Nextc(); return endsy;
	case ':': Nextc(); return colon;


    //----Nielegalne znaki
    default : Nextc(); return others;
  }
}
