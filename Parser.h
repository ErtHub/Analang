#ifndef PARSER_H
#define PARSER_H

#include "Scope.h"
#include "Scan.h"
#include "Util.h"

extern char *AT[];	// Atom Tab - dla komunikatów o b³êdach

class Parser;

class Synchronize   // Do synchronizacji parsera
{ friend Parser;
  const SymSet &f;
  void skipto(const SymSet &ss);

public:
  static Parser *p;
  Synchronize(const SymSet& s,const SymSet& f);
 ~Synchronize();
};

class Parser
{ 
  friend Scope;
  friend Synchronize;
  enum{ FirstSyntaxError=10, FirstSemanticError=60 };

  Scan& scn;  
  Scope *scope;   // Modu³ obs³ugi semantycznej nazw

  SymType symbol; // Ostatnio pobrany atom z scanner'a
  bool can_parse;	// Jeœli synchronizacja OK
  
  // Podzbiory atomów
  SymSet ststart,   stseq,    stiter;
  SymSet factstart, factiter, mulops;
  SymSet addops,    signs,    relops, types;

  TypRec *fractyp, 	// Inicjowane przez Parser::Parser()
		     *booltyp,
		     *strtyp,
         *nothingtyp,
		*expectedreturn;

  // Deskryptory pozorne - wykorzystywane w funkcji Search()
  // w przypadku odwo³ania do niezdefiniowanej nazwy

  IdRec *VarDummy, *ConstDummy,	*FuncDummy;

  void Nexts();				         // Nastêpny symbol
  void accept(SymType atom);
  void SyntaxError(int atom);	 // Oczekiwany atom
  void SyntaxError1(int atom); // Nieoczekiwany atom

  void OpenScope(const string& sn);
  void CloseScope();

  // FUNKCJE ROZBIORU
  void StmentBlock(const SymSet& fs);
  void VarDecl(const SymSet&  fs);
  TypRec* Type(const SymSet&  fs);
  void FunDecl(const SymSet&  fs);
  TypRec* FunCall(const SymSet& fs);
  void Return(const SymSet& fs);
  void Stment(const SymSet&  fs);
  void Cond(const SymSet&  fs);
  void Loop(const SymSet&  fs);
  void Assignment(const SymSet&  fs);
  void InputStment(const SymSet&  fs);
  void PrintStment(const SymSet&  fs);
  TypRec* Expr(const SymSet&  fs);
  TypRec* MedPrioExpr(const SymSet&  fs);
  TypRec* HiPrioExpr(const SymSet&  fs);
  TypRec* AtomExpr(const SymSet&  fs);
  TypRec* Variable(const SymSet&  fs);


public:
  Parser(Scan&);
 ~Parser();
  bool NextExecutable();
  void SemanticError(int ecode);
};
#endif
