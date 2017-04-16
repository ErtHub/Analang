// Modu³ SOURCE.H
// ===============
// Definicja klasy Source.
//
#ifndef SOURCE_H
#define SOURCE_H

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <cstdlib>
using namespace std;

enum Options{ NOLIST  = 1,	// NOLIST ==> tylko wiersze z b³êdami
			        SHOWIDS = 2,	// Drukuj tabele symboli
			};

extern int options;	// Definicja w funkcji main() 

struct TextPos	// Pomocnicza klasa: pozycja w pliku tekstowym
{ int   ln;     // Numer wiersza (od 1)
  int   cn;     // Numer znaku w wierszu (od 1)
  TextPos(int l=0, int c=0): ln(l), cn(c) {}
};

class Source
{
  int etotal, einline;// Liczniki b³êdów: globalny, w wierszu

  const string fn;    // Nazwa pliku
  ifstream istr;
  string Line;        // Bufor bie¿¹cego wiersza

  TextPos tpos;     	// Bie¿¹ca pozycja w pliku

  void PrntLine()		  // Drukowanie bie¿¹cego wiersza
  { cout<<setw(5)<<tpos.ln<<' '<<Line; }

  bool  NextLine();		// Nastêpny wiersz do bufora

public:
  Source(const string& file);
 ~Source();
  void  Error(int ec,const TextPos&tp,const char*mt="",const char*at="");
  int   NextChar();
  const TextPos& GetPos()const { return tpos; }
};
#endif
