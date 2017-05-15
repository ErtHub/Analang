#ifndef SCOPE_H
#define SCOPE_H

#include "Source.h"
#include <list>

enum TypKind{Frac, Bool, Str, Nothing, TYPKINDS };
extern const char *TK[TYPKINDS];

struct Range
{ int min, max;
  Range(int mi=0, int mx=1){ min=mi; max=mx; }
  friend int operator==(const Range &r1, const Range &r2)
  { return r1.min==r2.min && r1.max==r2.max; }

  friend ostream& operator<<(ostream& os, const Range& r)
  { return os<<'['<<r.min<<".."<<r.max<<']'; }
};

class TypRec	  // Klasa bazowa dla deskryptor�w typ�w
{
protected:
  TypKind kind;	// Rodzaj typu
  int count;		// Liczba "u�ytkownik�w" deskryptora
public:
  static int TypRecCount;

  TypRec(TypKind k): kind(k), count(0){ }
  virtual ~TypRec(){ }
  int Link(){ return ++count;}	// Do��czenie "u�ytkownika"
  int Delink(){ return --count;}// Od��czenie
  TypKind Kind()const { return kind; }

  virtual void Print() { cout<<TK[kind]<<'('<< count<<')'; }
  friend int Compatible(TypRec *t1,TypRec *t2);
};


// =================================================
// Definicje dotycz�ce reprezentacji identyfikator�w
//
enum IdKind		// Rodzaje ident.(mo�na ��czy� operatorem |)
{ VarId=1, ConstId=2, FuncId=4 };

struct IdRec   		// Deskryptor identyfikatora (BDS)
{ string name;	  // Nazwa
  unsigned kind;	// Rodzaj identyfikatora (wg IdKind)
  TypRec  *idtyp;	// Typ identyfikatora
  IdRec   *l, *r; // Lewe i prawe poddrzewa
  std::list<TypRec*> params;

  static int IdRecCount;

  IdRec(const string& nam, unsigned k, TypRec *it):
		name(nam), kind(k), idtyp(it)
  { if(idtyp) idtyp->Link();
	  l = r = 0;
  }
 ~IdRec()
  {
	  if(idtyp && idtyp->Delink()==0) { delete idtyp; }
	  if(l) delete l;
    if(r) delete r;
  }

  void Print();
  friend ostream& operator<<(ostream& os, const IdRec &ir);
};
//=============

class IdRecList	// Do przechowywania listy nazw zmiennych
{ struct IdNode
  { IdRec  *irp;
	  IdNode *next;

	  IdNode(IdRec *p): irp(p), next(0) { }
   ~IdNode()
    { if(next) delete next; }
  };

  IdNode *first, *last;

public:
  IdRecList() { first = last = 0; }
 ~IdRecList()
  { if(first) delete first; }
  void Append(IdRec *p);	    // Do��cz na ko�cu listy
  void AddTyp(TypRec *typptr);// Uzupe�nij typ zmiennych
};

// =======================================================
class Parser;  // Zapowied� definicji

class Scope    // Klasa opisuj�ca zakres bloku
{
  friend Parser;
  string scopename; // "GLOBAL", "PROGRAM" albo nazwa proc.
  Parser &p;        // Powi�zanie z parserem (obowi�zkowe)
  Scope *ext;	      // Zakres otaczaj�cy
  IdRec *root;	    // Korze� drzewa nazw lokalnych

public:
  Scope(const string& sn, Parser &parser, Scope *extscope);
 ~Scope();
  IdRec* Install(const string& name, unsigned kind, TypRec *trp);
  IdRec* Search (const string& name, unsigned kinds);
};
#endif
