// Modu³ SCAN.H
// ===============
// Definicja klasy Scan i typu SymType.
//
#ifndef SCAN_H
#define SCAN_H

#include <climits>
#include <cctype>
#include "Source.h"

enum SymType	// Atomy leksykalne Analangu
{ ifsy, nothingsy, fracsy, strsy, boolsy,
  varsy,    elsesy,  returnsy, truesy, falsesy, inputsy,
  whilesy,  printsy, funcsy,	// Keywords

  NKEYS,   MAXKEY=funcsy,

  ident,   fracconst, strconst, mulop,   addop,   subop,
  orop, andop, divop, endsy, notop, beginsy,
  ltop,    gtop,    eqop,
  rparent, lparent, comma,
  period,  colon,   becomes,  others, MAXSYM=others+1
};

enum ScanErrors{ CARCONSTWRONG=1, ICONST2BIG, FCONSTMALFORM, UNTERMCOMM, UNTERMSTRCONST };

class Scan
{
  struct KeyRec   // Pomocniczy deskryptor s³owa kluczowego
  { string kw;		// S³owo kluczowe
    SymType atom; // Odpowiadaj¹cy mu atom
  };
  static KeyRec KT[NKEYS];	// Tabela s³ów kluczowych

  int     c;			    // Aktualny znak z src
  TextPos atompos;	  // Pozycja atomu w tekscie
  struct frac
  {
	  int wholes;
	  int nom;
	  int denom;
  } fracconstant;
  //int     intconstant;// Wartož ostatniej sta³ej liczbowej
  string  spell;      // Ostatnio wczytany ident

  void Nextc() { c=src.NextChar(); }  // Pobieranie znaków
  
  unsigned hash(const string& s)
  { 
	  size_t len = s.size();
	  unsigned short h;

	  if (s == "return") return 2;
	  if (s == "true") return 3;
	  if (s == "print") return 8;// Korekta kolizji
	  h = (s[0] >> 1) * s[1] * 1834;
	  if (len >= 3) h += s[len - 1];
	  return h%NKEYS;
  }

public:
  Source& src;

  Scan(Source &s):src(s) { Nextc(); }
  SymType NextSymbol();		// Zwraca nastêpny atom leksykalny
  //int IntConst() { return intconstant; }
  struct frac FracConst() { return fracconstant; }
  const string& Spell() { return spell; }
  void ScanError(int ecode, const char *mtxt="", const char *atxt="")
  { src.Error(ecode, atompos, mtxt, atxt); }
};

#endif
