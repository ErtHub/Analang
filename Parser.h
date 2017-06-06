#ifndef PARSER_H
#define PARSER_H

#include "Scope.h"
#include "Scan.h"
#include "Util.h"
#include "Executable.h"
#include "Evaluable.h"
#include "Execution.h"
#include <memory>

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
  Execution& ex;
  Scope *scope;   // Modu³ obs³ugi semantycznej nazw

  SymType symbol; // Ostatnio pobrany atom z scanner'a
  bool can_parse;	// Jeœli synchronizacja OK
  bool error = false;
  
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
  void StmentBlock(const SymSet& fs, std::list<std::shared_ptr<Executable>>& blk);
  void VarDecl(const SymSet&  fs, std::shared_ptr<Executable>& decls);
  TypRec* Type(const SymSet&  fs);
  void FunDecl(const SymSet&  fs, int& funId, std::shared_ptr<Execution::FunctionPrototype>& prot);
  TypRec* FunCall(const SymSet& fs, std::shared_ptr<FunctionCall>& callComm);
  void Return(const SymSet& fs, std::shared_ptr<Executable>& ret);
  void Stment(const SymSet&  fs, std::shared_ptr<Executable>& st);
  void Cond(const SymSet&  fs, std::shared_ptr<Executable>& cond);
  void Loop(const SymSet&  fs, std::shared_ptr<Executable>& lp);
  void Assignment(const SymSet&  fs, std::shared_ptr<Executable>& assgn);
  void InputStment(const SymSet&  fs, std::shared_ptr<Executable>& inp);
  void PrintStment(const SymSet&  fs, std::shared_ptr<Executable>& prt);
  TypRec* Expr(const SymSet&  fs, std::shared_ptr<Evaluable>& xpr);
  TypRec* MedPrioExpr(const SymSet&  fs, std::shared_ptr<Evaluable>& xpr);
  TypRec* HiPrioExpr(const SymSet&  fs, std::shared_ptr<Evaluable>& xpr);
  TypRec* AtomExpr(const SymSet&  fs, std::shared_ptr<Evaluable>& xpr);
  TypRec* Variable(const SymSet&  fs, std::shared_ptr<Evaluable>& v);


public:
  Parser(Scan&, Execution&);
 ~Parser();
  bool NextExecutable();
  void SemanticError(int ecode);
};
#endif
