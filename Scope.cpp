#include "Scope.h"
#include "Parser.h"

const char *TK[TYPKINDS]={"Frac","Bool","Str","Nothing"};

int stricmp(const char *s1, const char*s2)
{  
  if(!s1) return -1;
  if(!s2) return 1;
  for(; toupper(*s1) == toupper(*s2); s1++, s2++)
    if(*s1=='\0') return 0;
  return toupper(*s1) - toupper(*s2);
}
  
//=======================================================
Scope::Scope(const string& snam, Parser &parser, Scope *extscope):
			 scopename(snam), p(parser), ext(extscope), root(0)
{ }
//=============
Scope::~Scope()
{ if(root)
  { if(options & SHOWIDS)
	  { cout<<"======== Nazwy zdefiniowane w zakresie: "
            <<scopename<<'\n';
	    root->Print();
	    cout<<"========\n";
	  }
	  delete root;
  }
}
//====================================
IdRec* Scope::Install(const string& name,
                      unsigned   kind,
                      TypRec    *trp)
// Instaluje identyfikator name z podanymi atrybutami
// w tabeli symboli bie��cego zakresu (wskazanie root).
//
{ int eq, left;
  IdRec *irp, *lastirp;
  IdRec *irpnew=new IdRec(name, kind, trp);

  if(root==0) return root=irpnew;

  irp = root;
  while(irp)
  { lastirp = irp;
	  //eq = stricmp(name, irp->name);// A...Z == a...z
    eq = name.compare(irp->name);// A...Z == a...z
	  if(eq<0) irp = irp->l, left = 1; else
	  if(eq>0) irp = irp->r, left = 0; else
	  { p.SemanticError(0);			// Definicja homonimu
	    irp = irp->r; left = 0;		// Instaluj jednak
	  }
  }
  return left? (lastirp->l = irpnew): (lastirp->r = irpnew);
}
//============================================
IdRec* Scope::Search(const string& name,
                     unsigned   expectedkinds)
{
// Szukanie nazwy 'name' w tabelach poczynaj�c od bie��cego
// zakresu. Parametr 'expectedkinds' okre�la dopuszczalne
// rodzaje identyfikator�w (podzbi�r {VarId, ConstId, TypId,
// ProcId}). Szukanie prowadzone jest zgodnie z regu��
// zakresu dla struktury blokowej - od zakresu bie��cego na
// zewn�trz w zakresach otaczaj�cych. Znalezienie nazwy
// niezgodnego rodzaju albo brak nazwy powoduj� komunikat
// b��du; w przypadku braku nazwy, opr�cz wys�ania
// komunikatu, instaluje si� t� nazw� z najbardziej
// prawdopodobnym rodzajem (p. kolejno�� w zbiorze IdKind).
// Je�eli nazw� znaleziono, ale ma ona b��dny rodzaj,
// to zwracane jest wskazanie na pseudo-deskryptor ...Dummy.
// Pozwala to korzysta� z funkcji Search() jednorodnie,
// niezale�nie od wyniku szukania.
//
  int ok=1, eq, best;
  IdRec *irp;
  Scope *sp=this;

  for(best=1; (expectedkinds&best)==0; best<<=1);

  // best="najwa�nejszy" rodzaj

  do
  { irp = sp->root;
	while(irp)
	{ 
    //eq = stricmp(name, irp->name);// A...Z == a...z
    eq = name.compare(irp->name);// A...Z == a...z
	  if(eq<0) irp = irp->l; else
	  if(eq>0) irp = irp->r; else
	  { ok = expectedkinds & irp->kind;// Oczekiwany rodzaj?
		if(ok) return irp;			   // Znaleziony
		irp = irp->r;
	  }
	}
	if(!ok)
	{ p.SemanticError(1);	// B��dny rodzaj identyfikatora

      // Zwr�� wskazanie na deskryptor "pozoruj�cy"

	  switch(best)
	  { case 1: return p.VarDummy;
		case 2: return p.ConstDummy;
		case 4: return p.FuncDummy;
		default:cout<<__FILE__ <<__LINE__<<": "<<best
                    <<" (FATAL INTERNAL ERROR)\n";
				exit(1);
	  }
	}
	sp = sp->ext;
  } while(sp);

  p.SemanticError(2);		// Brak definicji identyfikatora
  return Install(name, best, NULL);
}
//====================================
int Compatible(TypRec *t1, TypRec *t2)  // Zgodno�� typ�w
{ 
  if(t1==t2 || t1==0 || t2==0) return 1;

  else return 0;
}
//=================
void IdRec::Print()
{ if(l) l->Print();
  cout<< *this;
  if(r) r->Print();
}
//===============================================
ostream& operator<<(ostream& os, const IdRec &ir)

{ os<<setw(10)<<ir.name<<": ";
  if(ir.idtyp) ir.idtyp->Print(); else cout<<"NULL";
  return os<<'\n';
}
//=========================================
// Funkcje sk�adowe klasy IdRecList
//
void IdRecList::Append(IdRec *p)   // Do��cz na ko�cu listy
{ IdNode *inp = new IdNode(p);

  if(first) last->next = inp; else first = inp;
  last = inp;
}
//====================================
void IdRecList::AddTyp(TypRec *typptr)
//
// Uzupe�nij informacje o typie zmiennych
//
{ IdNode *ptr=first;
  while(ptr && typptr)  // typptr==0: nieznany typ
  { ptr->irp->idtyp = typptr;
    typptr->Link();
	ptr = ptr->next;
  }
}

