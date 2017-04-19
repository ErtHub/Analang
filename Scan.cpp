// Modu³ Scan.cpp
// =================
// Defnicje funkcji sk³adowych klasy Scan, tabeli leksemów AT
// i tabeli s³ów kluczowych.
//
#include "Scan.h"

char *AT[MAXSYM+1] =	// Tabela Atomów (dla komunikatów o b³êdach
{
  "if",    "nothing",     "frac",    "str",  "bool",
  "var",    "else",  "input",
  "while",  "print", "func",
  "ident", "fracconst","strconst",

  "*", "+", "-", "||", "&&", "/", "}", "!", "{", "<", ">", "==", ")", "(",
  ",", ".",  ":", "=", "others"
};

Scan::KeyRec Scan::KT[NKEYS]= 	// Kolejnoœæ wg funkcji hash(keyword)
{
// Keyword		 Atom			hash(keyword)
//-------------------------------------------
  { "var",       varsy  }, 	//  0
  { "frac",     fracsy},  //  1
  { "if",        ifsy   },  //  2
  { "str",     strsy},  //  3
  { "else",       elsesy  },  //  4
  { "nothing", nothingsy },  //  5
  { "while",        whilesy   },  //  6
  { "func",       funcsy  },  //  7
  { "print",        printsy   },  //  8
  { "input",     inputsy},  //  9
  { "bool",       boolsy  },  //  10
};

SymType Scan::NextSymbol()
{ 
  do
  {
    while(isspace(c)) Nextc();	// Pomiñ znaki bia³e
    if(c==EOF) return others;
    if(c=='/')
	{
		atompos = src.GetPos(); // Mo¿e byæ divop albo komentarz
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
				Nextc();
			}
		}
		else
			return divop;
	}
  } while(isspace(c) || c=='/');

  atompos=src.GetPos(); // Bêdzie jakiœ atom leksykalny

  //---Identyfikator lub s³owo kluczowe
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

  //---Sta³a u³amkowa
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
			ScanError(ICONST2BIG, "Przekroczony zakres sta³ej ca³kowitej");
		if (c == '.' && !pointSeen)
		{
			if (underscoreSeen)
				ScanError(FCONSTMALFORM, "Niepoprawny format sta³ej u³amkowej");
			else
			{
				pointSeen = true;
				fracconstant.wholes = ul;
				Nextc();
				if (!isdigit(c))
					ScanError(FCONSTMALFORM, "Niepoprawny format sta³ej u³amkowej");
			}
		}
		else if (c == '_' && !underscoreSeen)
		{
			underscoreSeen = true;
			fracconstant.nom = ul;
			Nextc();
			if (!isdigit(c))
				ScanError(FCONSTMALFORM, "Niepoprawny format sta³ej u³amkowej");
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
				ScanError(FCONSTMALFORM, "Niepoprawny format sta³ej u³amkowej");
			return fracconst;
		}
		//intconstant = (int)ul;
	} while (isdigit(c));
  }
  else

  //---Pozosta³e atomy
  switch(c)
  {
    //----Sta³a znakowa
    case '"': Nextc();
			spell.clear();
			while (c != '"')
			{
				spell.push_back(c);
				Nextc();
			}
			Nextc();
		    /*if(c=='"')
		    { Nextc();
		      if(c!='"') ScanError(CARCONSTWRONG, "B³êdna sta³a znakowa");
		    }
		    intconstant=c;
		    Nextc();
		    if(c!='"') ScanError(CARCONSTWRONG, "B³êdna sta³a znakowa");
		    else Nextc();*/
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
