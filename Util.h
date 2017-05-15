#ifndef UTIL_H
#define UTIL_H

#include <cstdarg>		// Zmienna lista argumentow
#include "Scan.h"
using namespace std;

int const EOS=-1;			    // Oznacznik ko�ca listy element�w

class SymSet
{ enum
  { size  = MAXSYM,		// Elementy zbioru: 0..(size-1)
	ubits = (sizeof(unsigned)*CHAR_BIT),// Bity w unsigned
	nwords= ((size-1)/ubits+1)			    // Liczba s��w
  };

  unsigned s[nwords];	// Bitowa reprezentacja zbioru

public:
  SymSet();       		    // Zbi�r pusty
  SymSet(int e);		      // Zbi�r 1-elementowy
  SymSet(int, int,...);	  // Zbi�r wieloelementowy

  SymSet operator+(const SymSet &t)const; // s + t
  bool   has(int e)const;	// Czy e nale�y do *this
  friend ostream& operator<< (ostream& os, const SymSet& t);
};

// =========================================================
// Klasa Trace do �ledzenia wywo�a� funkcji rozbioru.
//
// Deklaruj�c obiekt lokalny klasy Trace w funkcji
// rozbioru:
//   Trace t("NazwaFunkcji", SymbolSet);
// otrzymamy komunikat o wywo�aniu tej funkcji i sekwencji
// powrotu. Sk�adowe statyczne kontroluj� posta� komunikat�w
// wynikaj�c� z poziomu wywo�ania funkcji. Elementy zbioru
// SymbolSet s� wyprowadzane je�eli sk�adowa statyczna
// show_symbols (odpowiada jej opcja kompilatora '/a')
// jest ustawiona.
//
class Trace
{ static int call_level;

public:
  Trace(const char *text, const SymSet& s);
 ~Trace();
  static void prefix();
  static int trace_on;
  static int show_symbols;
};
#endif

