#ifndef UTIL_H
#define UTIL_H

#include <cstdarg>		// Zmienna lista argumentow
#include "Scan.h"
using namespace std;

int const EOS=-1;			    // Oznacznik koñca listy elementów

class SymSet
{ enum
  { size  = MAXSYM,		// Elementy zbioru: 0..(size-1)
	ubits = (sizeof(unsigned)*CHAR_BIT),// Bity w unsigned
	nwords= ((size-1)/ubits+1)			    // Liczba s³ów
  };

  unsigned s[nwords];	// Bitowa reprezentacja zbioru

public:
  SymSet();       		    // Zbiór pusty
  SymSet(int e);		      // Zbiór 1-elementowy
  SymSet(int, int,...);	  // Zbiór wieloelementowy

  SymSet operator+(const SymSet &t)const; // s + t
  bool   has(int e)const;	// Czy e nale¿y do *this
  friend ostream& operator<< (ostream& os, const SymSet& t);
};

// =========================================================
// Klasa Trace do œledzenia wywo³añ funkcji rozbioru.
//
// Deklaruj¹c obiekt lokalny klasy Trace w funkcji
// rozbioru:
//   Trace t("NazwaFunkcji", SymbolSet);
// otrzymamy komunikat o wywo³aniu tej funkcji i sekwencji
// powrotu. Sk³adowe statyczne kontroluj¹ postaæ komunikatów
// wynikaj¹c¹ z poziomu wywo³ania funkcji. Elementy zbioru
// SymbolSet s¹ wyprowadzane je¿eli sk³adowa statyczna
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

