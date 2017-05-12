// MODUL MP2_PARS.CPP
// ==================
// Parser z akcjami semantycznymi (bez generacji kodu)

/**
DO ZROBIENIA:
Next Executable CHECK
Stment CHECK
FunDecl CHECK
FunCall CHECK
Assignment CHECK
Return CHECK
Cond CHECK
VarDecl CHECK
Loop CHECK
Expr CHECK
MedPrioExpr CHECK
Comparison CHECK
HiPrioExpr CHECK
AtomExpr CHECK
Negation CHECK
IfWhileBlock (StmentBlock?) CHECK
InputStment CHECK
PrintStment CHECK
Type CHECK
Variable CHECK
*/

#include "Parser.h"

//
// Funkcje prywatne parsera
//
//==================
void Parser::Nexts()		// Pobranie nastêpnego symbolu
{ 
	symbol=scn.NextSymbol();
}
//===============================
void Parser::accept(SymType atom)
{ if(symbol==atom) Nexts(); else
	  SyntaxError(atom);
}
//================================
void Parser::SyntaxError(int atom)	// Oczekiwany atom
{ scn.ScanError(FirstSyntaxError+atom,
			 "Spodziewany symbol: ",
			  AT[atom]);
}
//=================================
void Parser::SyntaxError1(int atom)	// Nieoczekiwany atom
{ scn.ScanError(FirstSyntaxError+atom,
			 "Nieoczekiwany symbol: ",
			  AT[atom]);
}
//=============================
void Parser::OpenScope(const string& sn)
{
  scope = new Scope(sn, *this, scope);
}
//=======================
void Parser::CloseScope()
{ Scope *oldscope = scope;
  scope = scope->ext;
  delete oldscope;
}
//
// Konstruktor, destruktor i funkcje rozbioru Parsera
//
//===================================================
//Parser::Parser(const char *fn): Scan(fn), scope(0)
Parser::Parser(Scan& sc): scn(sc), scope(0)
// Otwiera "GLOBAL" Scope
{
  Synchronize::p = this;    // Zwi¹zanie z klas¹ Synchronize

  ststart  = SymSet(inputsy,  printsy,
                    ifsy,    whilesy, ident, returnsy, varsy, EOS);
  /*stseq    = SymSet(beginsy, inputsy,  printsy,
                    ifsy,    whilesy, endsy,EOS);
  stiter   = SymSet(beginsy, inputsy,  printsy,
                    ifsy,    whilesy, EOS);*/
  factstart= SymSet(ident,   fracconst,strconst, truesy, falsesy,
                    notop,   lparent, EOS);
  mulops   = SymSet(mulop,   divop,   andop, EOS);
  factiter = factstart + mulops;
  signs    = SymSet(addop,subop,EOS);
  addops   = SymSet(addop,subop,orop, EOS);
  relops   = SymSet(eqop, ltop, gtop, EOS);
  types = SymSet(fracsy, boolsy, strsy, nothingsy, EOS);

//================================================
// Utwórz deskryptory pozorne dla Scope::Search()
//

  VarDummy = new IdRec("@@VDUMMY", VarId, 0),
  ConstDummy = new IdRec("@@CDUMMY", ConstId, 0),
  FuncDummy = new IdRec("@@FDUMMY", FuncId, 0);

  // Utwórz zasiêg globalny nazw
  OpenScope("GLOBAL");

  // Deskryptory typów wbudowanych
  fractyp  = new TypRec(Frac);
  booltyp = new TypRec(Bool);
  strtyp = new TypRec(Str);
  nothingtyp = new TypRec(Nothing);
  expectedreturn = nothingtyp;

  // Instaluj nazwy predefiniowane
  /*scope->Install("frac", TypId,   fractyp);
  scope->Install("str",    TypId,   strtyp);
  scope->Install("bool", TypId,   booltyp);
  scope->Install("false",   ConstId, booltyp);
  scope->Install("true",    ConstId, booltyp);*/
  //scope->Install("PROGRAM", ProgId,  proctyp);

  Nexts();          // Pobranie 1-go atomu
}
//==============
Parser::~Parser()
{
  CloseScope();     // Zamknij zakres globalny "GLOBAL"

  delete VarDummy;
  delete ConstDummy;
  delete FuncDummy;
}
//========================
/*void Parser::Program(void)
{ Trace t("Program",-1);
  IdRec *irp;

  // Program   	= "program" ident ';' Block '.' ;

  accept(progsy);
  if(symbol==ident)
    irp = new IdRec(scn.Spell(), ProgId, NULL);
  else
    irp = new IdRec("PROGRAM???", ProgId, NULL);

  accept(ident);
  accept(semicolon);
  Block(period, irp);

  delete irp;
}*/

bool Parser::NextExecutable()
{
	Trace("NextExecutable", -1);
	if (ststart.has(symbol))
	{
		Stment(ststart + SymSet(funcsy, others, EOS));
		return true;
	}
	else if (symbol == funcsy)
	{
		FunDecl(ststart + SymSet(funcsy, others, EOS));
		return true;
	}
	return false;

}
//=========================================================
/*void Parser::Block(const SymSet& fs,const IdRec *irp)
{ Trace x("Block", fs);
  Synchronize s(SymSet(varsy,procsy, beginsy,EOS), fs);

// Block = VarPart ProcPart CompStmt ;

  if(!can_parse) return;

  OpenScope(irp->name);

  VarPart(SymSet(procsy, beginsy, EOS));
  ProcPart(beginsy);
  CompStmt(fs);

  CloseScope();
}*/

void Parser::StmentBlock(const SymSet& fs)
{
	Trace x("StmentBlock", fs);
	Synchronize s(beginsy, fs);

	if (!can_parse) return;

	accept(beginsy);
	while (ststart.has(symbol))
	{
		Stment(fs + ststart + endsy);
	}
	accept(endsy);
}
//============================================
/*void Parser::VarPart(const SymSet&  fs)
{ Trace x("VarPart", fs);
  Synchronize s(fs+varsy, fs);
  if(!can_parse) return;

// VarPart = empty | "var" VarDecl ';' { VarDecl ';' } ;

  if(symbol==varsy)
  { accept(varsy);
	  do
	  { VarDecl(fs + semicolon);
	    accept(semicolon);
	  } while (symbol==ident);
  }
}*/
//============================================
/*void Parser::VarDecl(const SymSet&  fs)
{ Trace x("VarDec", fs);
  Synchronize s(SymSet(ident, comma, colon, EOS),
                fs);
  if(!can_parse) return;

// VarDecl = ident { ',' ident } ':' Type ;

  IdRecList irl;

  irl.Append(scope->Install(scn.Spell(), VarId, NULL));
  accept(ident);
  while(symbol==comma)
  { accept(comma);
	  irl.Append(scope->Install(scn.Spell(), VarId, NULL));
	  accept(ident);
  }
  accept(colon);
  irl.AddTyp(Type(fs));
}*/

void Parser::VarDecl(const SymSet&  fs)
{
	Trace x("VarDecl", fs);
	Synchronize s(varsy, fs);
	if (!can_parse) return;

	// VarDecl = ident { ',' ident } ':' Type ;

	IdRecList irl;

	accept(varsy);
	accept(colon);

	TypRec* typ;
	if ((typ = Type(fs + ident)) == nothingtyp)
		SemanticError(3);

	irl.Append(scope->Install(scn.Spell(), VarId, NULL));
	accept(ident);
	while (symbol == comma)
	{
		accept(comma);
		irl.Append(scope->Install(scn.Spell(), VarId, NULL));
		accept(ident);
	}
	irl.AddTyp(typ);
}
//============================================
/*TypRec* Parser::Type(const SymSet&  fs)
{ Trace x("Type", fs);
  Synchronize s(SymSet(ident,arraysy,EOS), fs);
  if(!can_parse) return 0;

// Type	= SimpType | ArrayType ;

  ArrTyp *atp;
  if(symbol==ident) return SimpleType(fs);
//  else

  // ArrayType = "array" '[' IndexRange ']' "of" SimpType ; 
  { Range r;
	  TypRec *etyp;
	  accept(arraysy);
	  accept(lbracket);
	  r = IndexRange(fs + SymSet(rbracket, ofsy, EOS));
	  accept(rbracket);
	  accept(ofsy);
	  etyp = SimpleType(fs);
	  atp = new ArrTyp(etyp, r);
  }
  return atp;
}*/

TypRec* Parser::Type(const SymSet&  fs)
{
	Trace x("Type", fs);
	Synchronize s(types, fs);
	if (!can_parse) return 0;

	// Type	= SimpType | ArrayType ;

	//ArrTyp *atp;
	//if (symbol == ident) return SimpleType(fs);
	//  else

	// ArrayType = "array" '[' IndexRange ']' "of" SimpType ; 
	/*{ Range r;
	TypRec *etyp;
	accept(arraysy);
	accept(lbracket);
	r = IndexRange(fs + SymSet(rbracket, ofsy, EOS));
	accept(rbracket);
	accept(ofsy);
	etyp = SimpleType(fs);
	atp = new ArrTyp(etyp, r);
	}
	return atp;*/

	switch (symbol)
	{
	case fracsy: accept(fracsy); return fractyp;
	case boolsy: accept(boolsy); return booltyp;
	case strsy: accept(strsy); return strtyp;
	case nothingsy: accept(nothingsy); return nothingtyp;
	}
}
//=================================================
/*TypRec* Parser::SimpleType(const SymSet& fs)
{ Trace x("SimType", fs);
  Synchronize s(ident, fs);
  if(!can_parse) return 0;

// SimpType	= TypeIdent ;

  IdRec *irp = scope->Search(scn.Spell(), TypId);
  accept(ident);
  return (irp)? irp->idtyp : NULL;
}*/
//=================================================
/*Range Parser::IndexRange (const SymSet & fs)
{ Trace x("IndRange", fs);
  Synchronize s(SymSet(intconst, thru,EOS), fs);
  if(!can_parse) return Range(0,0);

// <IndexRange = intconst '..' intconst ;

  int min=0, max=0;

  accept(intconst); min = scn.IntConst();
  accept(thru);
  accept(intconst); max = scn.IntConst();
  if(max<min)
  { SemanticError(3);		// Bledny zakres indeksu
    max=min;
  }
  return Range(min, max);
}*/
//=============================================
/*void Parser::ProcPart(const SymSet&  fs)
{ Trace x("ProcPart", fs);
  Synchronize s(fs+procsy, fs);
  if(!can_parse) return;

// ProcPart	= { ProcDecl ';' } ;

  while(symbol==procsy)
  { ProcDecl(fs + SymSet(semicolon,procsy,EOS));
    accept (semicolon);
  }
}*/
//=============================================
/*void Parser::ProcDecl(const SymSet&  fs)
{ Trace x("ProcDec", fs);
  Synchronize s(fs + procsy, fs);
  if(!can_parse) return;

// ProcDecl  	= "procedure" ident ';' Block ;

  string pname="???";
  IdRec *procidptr;

  accept(procsy);
  if(symbol==ident) pname = scn.Spell();
  procidptr = scope->Install(pname, ProcId, proctyp);
  accept(ident);
  accept(semicolon);
  Block(fs, procidptr);
}*/

void Parser::FunDecl(const SymSet&  fs)
{
	Trace x("FunDecl", fs);
	Synchronize s(fs + funcsy, fs);
	if (!can_parse) return;

	// ProcDecl  	= "procedure" ident ';' Block ;

	string fname = "???";
	string paramname = "???";
	IdRec *procidptr;

	accept(funcsy);
	accept(colon);
	TypRec* typ = Type(fs + ident);
	expectedreturn = typ;

	if (symbol == ident) fname = scn.Spell();
	procidptr = scope->Install(fname, FuncId, typ);
	accept(ident);
	accept(lparent);

	OpenScope(fname);

	if (types.has(symbol))
	{
		TypRec* ptyp = Type(fs + ident);
		paramname = scn.Spell();
		accept(ident);
		procidptr->params.push_back(scope->Install(paramname, VarId, ptyp)->idtyp);

		while(symbol==comma)
		{
			accept(comma);
			ptyp = Type(fs + ident);
			paramname = scn.Spell();
			accept(ident);
			procidptr->params.push_back(scope->Install(paramname, VarId, ptyp)->idtyp);
		}

	}
	accept(rparent);

	StmentBlock(fs);

	CloseScope();
	expectedreturn = nothingtyp;
}

TypRec* Parser::FunCall(const SymSet& fs)
{
	Trace x("FunCall", fs);
	Synchronize s(ident, fs);

	if (!can_parse) return NULL;
	IdRec *funid = scope->Search(scn.Spell(), FuncId);
	std::list<TypRec*> ptyps;
	accept(ident);
	accept(lparent);
	if (symbol != rparent)
	{
		ptyps.push_back(Expr(fs + SymSet(comma, rparent, EOS)));
		while (symbol == comma)
		{
			accept(comma);
			ptyps.push_back(Expr(fs + SymSet(comma, rparent, EOS)));
		}
	}
	if (ptyps != funid->params)
		SemanticError(5);
	accept(rparent);
}

void Parser::Return(const SymSet& fs)
{
	Trace x("Return", fs);
	Synchronize s(returnsy, fs);
	if (expectedreturn == nothingtyp)
		SemanticError(14);
	accept(returnsy);
	TypRec* etyp = Expr(fs);
	if (!Compatible(etyp, expectedreturn))
		SemanticError(15);
}
//=========================================
/*void Parser::Stmt(const SymSet&  fs)
{ Trace x("Stmt", fs);
  Synchronize s(ststart, fs);
  if(!can_parse) return;

// Stmt = CompStmt  | IfStmt   | WhileStmt | ReadStmt> | 
//        WriteStmt | ProcStmt | Assignment ;

  switch(symbol)
  { IdRec *irp;   // Do rozró¿nienia procedury/podstawienia

	  case beginsy: CompStmt(fs);	 break;
	  case ifsy:	  IfStmt(fs);	   break;
	  case whilesy: WhileStmt(fs); break;
	  case readsy:  ReadStmt(fs);  break;
	  case writesy: WriteStmt(fs); break;

	  case ident :
    // Mo¿e byæ wywo³anie procedury albo przypisanie.
    // Rozró¿nienie: na podstawie atrybutów identyfikatora.

          irp=scope->Search (scn.Spell(), (VarId | ProcId));
          if(irp->kind == VarId)
            Assignment(fs);
          else

      // ProcStmt> = ProcIdent ;
			Nexts();    // Procedura bezparametrowa
  }
}*/

void Parser::Stment(const SymSet&  fs)
{
	Trace x("Stment", fs);
	Synchronize s(ststart, fs);
	if (!can_parse) return;

	// Stmt = CompStmt  | IfStmt   | WhileStmt | ReadStmt> | 
	//        WriteStmt | ProcStmt | Assignment ;

	switch (symbol)
	{
		IdRec *irp;   // Do rozró¿nienia procedury/podstawienia

	case ifsy:	  Cond(fs);	   break;
	case whilesy: Loop(fs); break;
	case inputsy:  InputStment(fs);  break;
	case printsy: PrintStment(fs); break;
	case returnsy: Return(fs); break;
	case varsy: VarDecl(fs); break;

	case ident:
		// Mo¿e byæ wywo³anie procedury albo przypisanie.
		// Rozró¿nienie: na podstawie atrybutów identyfikatora.

		irp = scope->Search(scn.Spell(), (VarId | FuncId));
		if (irp->kind == VarId)
			Assignment(fs);
		else
			FunCall(fs);
	}
}
//=============================================
/*void Parser::CompStmt(const SymSet&  fs)
{ Trace x("CompStmt", fs);
  Synchronize s(beginsy, fs);
  if(!can_parse) return;

// CompStment	= "begin" Stment { ';' Stment } "end" ;
// Uwaga: nie ma instrukcji pustych.

  accept(beginsy);
  Stmt(stseq+fs);
  while(stiter.has(symbol))
  {
	  accept(semicolon);
	  Stmt(stseq+fs);
  }
  accept(endsy);
}*/
//===========================================
/*void Parser::IfStmt(const SymSet&  fs)
{ // Synchronizacja w procedurze Stmt()
  Trace t("IfStmt", -1);
  TypRec *etyp;

// IfStement = "if" Expr "then" Stment |
//		         "if" Expr "then" Stment "else" Stment ;

  accept(ifsy);
  etyp = Expression(fs+SymSet(thensy, elsesy, EOS));
  if(!Compatible(etyp, booltyp)) SemanticError(12);
  accept(thensy);
  Stmt(fs + elsesy);
  if (symbol == elsesy)
  { accept(elsesy);
	Stmt(fs);
  }
}*/

void Parser::Cond(const SymSet&  fs)
{ // Synchronizacja w procedurze Stmt()
	Trace t("Cond", -1);
	TypRec *etyp;

	// IfStement = "if" Expr "then" Stment |
	//		         "if" Expr "then" Stment "else" Stment ;

	accept(ifsy);
	accept(lparent);
	etyp = Expr(fs + rparent);
	if (!Compatible(etyp, booltyp)) SemanticError(12);
	accept(rparent);
	OpenScope("CondIf");
	StmentBlock(fs + elsesy);
	CloseScope();
	if (symbol == elsesy)
	{
		accept(elsesy);
		OpenScope("CondElse");
		StmentBlock(fs);
		CloseScope();
	}
}
//==============================================
/*void Parser::WhileStmt(const SymSet&  fs)
{ // Synchronizacja w procedurze Stmt()
  Trace t("WhileStmt", -1);
  TypRec *etyp;

// WhileStment	= "while" Expr "do" Stment ;

  accept(whilesy);
  etyp = Expression(fs + dosy);
  if(!Compatible(etyp, booltyp)) SemanticError(12);
  accept(dosy);
  Stmt(fs);
}*/

void Parser::Loop(const SymSet&  fs)
{ // Synchronizacja w procedurze Stmt()
	Trace t("Loop", -1);
	TypRec *etyp;

	// WhileStment	= "while" Expr "do" Stment ;

	accept(whilesy);
	accept(lparent);
	etyp = Expr(fs + rparent);
	if (!Compatible(etyp, booltyp)) SemanticError(12);
	accept(rparent);
	OpenScope("Loop");
	StmentBlock(fs);
	CloseScope();
}
//===============================================
/*void Parser::Assignment(const SymSet&  fs)
{ // Synchronizacja w procedurze Stmt()
  Trace t("Assgn", -1);
  TypRec *vtyp, *etyp;

// Assignment	= Var ':=' Expr ;

  vtyp = Variable(fs + becomes);
  accept(becomes);
  etyp = Expression(fs);
  if(!Compatible(vtyp, etyp))
	SemanticError(9);		// Zienna i wyra¿enie niezgodne
}*/

void Parser::Assignment(const SymSet&  fs)
{ // Synchronizacja w procedurze Stmt()
	Trace t("Assignment", -1);
	TypRec *vtyp, *etyp;

	// Assignment	= Var ':=' Expr ;

	vtyp = Variable(fs + becomes);
	accept(becomes);
	etyp = Expr(fs);
	if (!Compatible(vtyp, etyp))
		SemanticError(9);		// Zienna i wyra¿enie niezgodne
}
//=============================================
/*void Parser::ReadStmt(const SymSet&  fs)
{ // Synchronizacja w procedurze Stmt()
  Trace t("ReadSt", -1);

// ReadStment	= "read" '('InputVar { ',' InputVar } ')' ;

  accept(readsy);
  accept(lparent);
  InputVar(fs);
  while (symbol==comma)
  { accept(comma);
	InputVar(fs);
  }
  accept(rparent);
}*/

void Parser::InputStment(const SymSet&  fs)
{ // Synchronizacja w procedurze Stmt()
	Trace t("InputStment", -1);

	// ReadStment	= "read" '('InputVar { ',' InputVar } ')' ;

	accept(inputsy);
	accept(lparent);
	if (Variable(fs + SymSet(comma, rparent, EOS)) == nothingtyp)
		SemanticError(3);//TODO: dodac mozliwa kontrole bledow w przypadku nothing
	while (symbol == comma)
	{
		accept(comma);
		if (Variable(fs + SymSet(comma, rparent, EOS)) == nothingtyp)
			SemanticError(3);//ibidem
	}
	accept(rparent);
}
//================================================
/*TypRec* Parser::InputVar(const SymSet&  fs)
{ Trace t("InVar", -1);
  TypRec *vtyp;

// InputVar = Variable ;

  vtyp = Variable(fs + SymSet(comma,rparent,EOS));
  if(!(Compatible(vtyp, fractyp)||Compatible(vtyp, strtyp)))
	SemanticError(10);
  return vtyp;
}*/
//==============================================
/*void Parser::WriteStmt(const SymSet&  fs)
{ // Synchronizacja w procedurze Stmt()
  Trace t("WriteSt", -1);

// WriteStment	= "write" '('OutValue {',' OutValue } ')' ;

  accept(writesy);
  accept(lparent);
  OutputValue(fs);
  while (symbol==comma)
  { accept(comma);
	OutputValue(fs);
  }
  accept(rparent);
}*/

void Parser::PrintStment(const SymSet&  fs)
{ // Synchronizacja w procedurze Stmt()
	Trace t("PrintStment", -1);

	// WriteStment	= "write" '('OutValue {',' OutValue } ')' ;

	accept(printsy);
	accept(lparent);
	if (Expr(fs + SymSet(comma, rparent, EOS)) == nothingtyp)
		SemanticError(4);//TODO: rowniez kontrola przeciwko typowi nothing
	while (symbol == comma)
	{
		accept(comma);
		if (Expr(fs + SymSet(comma, rparent, EOS)) == nothingtyp)
			SemanticError(4);//ibidem
	}
	accept(rparent);
}
//===================================================
/*TypRec* Parser::OutputValue(const SymSet&  fs)
{ Trace t("OutVal", -1);
  TypRec *etyp;

// OutputValue = Expression ;

  etyp = Expression(fs + SymSet(comma,rparent,EOS));
  if(!(Compatible(etyp,fractyp)||Compatible(etyp, strtyp)))
	SemanticError(10);
  return etyp;
}*/
//==================================================
/*TypRec* Parser::Expression(const SymSet&  fs)
{ // Synchronizacja odroczona do SimpleExpr()
  Trace t("Expr", -1);
  TypRec *et, *et1;

// Expr	= SimpExpr | SimpExpr RelOp SimpExpr ;


  et = SimpleExpr(fs + relops);
  if(relops.has(symbol))
  { et1 = et;
	Nexts();
	et = SimpleExpr(fs);
	if(!((Compatible(et1,fractyp) &&Compatible(et,fractyp)) ||
		 (Compatible(et1,strtyp)&&Compatible(et,strtyp))))
	  SemanticError(8);
	et = booltyp;
  }
  return et;
}*/

TypRec* Parser::Expr(const SymSet&  fs)
{ // Synchronizacja odroczona do SimpleExpr()
	Trace t("Expr", -1);
	TypRec *et, *et1;

	// Expr	= SimpExpr | SimpExpr RelOp SimpExpr ;


	et = MedPrioExpr(fs + relops);
	if (relops.has(symbol))
	{
		et1 = et;
		Nexts();
		et = MedPrioExpr(fs);
		if (!((Compatible(et1, fractyp) && Compatible(et, fractyp)) ||
			(Compatible(et1, strtyp) && Compatible(et, strtyp))))
			SemanticError(8);
		et = booltyp;
	}
	return et;
}
//==================================================
/*TypRec* Parser::SimpleExpr(const SymSet&  fs)
{ Trace x("SimExp", fs);
  Synchronize s(factstart+signs, fs);
  TypRec *termtyp, *exptyp;
  int     signum=0;
  SymType op;

// SimpExpr	= Sign Term { AddOp Term } ;

  if(!can_parse) return 0;

  if(signs.has(symbol)){ Nexts(); signum = 1; }
  exptyp = Term(fs + addops);
  if(signum && !Compatible(exptyp, fractyp))
	SemanticError(7);

  while(addops.has(symbol))
  { termtyp = exptyp;
	  op = symbol;
	  Nexts();
	  exptyp = Term(fs+addops);
	  switch(op)
	  { case plus:
	    case minus: if(!Compatible(termtyp, fractyp) ||
					           !Compatible(exptyp,  fractyp))
					          SemanticError(7);
				          exptyp = fractyp;
				          break;
	    case orop: if(!Compatible(termtyp, booltyp) ||
					          !Compatible(exptyp,  booltyp))
					         SemanticError(6);
				         exptyp = booltyp;
				         break;
	  }
  }
  return exptyp;
}*/

TypRec* Parser::MedPrioExpr(const SymSet&  fs)
{
	Trace x("MedPrioExpr", fs);
	Synchronize s(factstart + signs, fs);
	TypRec *termtyp, *exptyp;
	int     signum = 0;
	SymType op;

	// SimpExpr	= Sign Term { AddOp Term } ;

	if (!can_parse) return 0;

	if (signs.has(symbol)) { Nexts(); signum = 1; }
	exptyp = HiPrioExpr(fs + addops);
	if (signum && !Compatible(exptyp, fractyp))
		SemanticError(7);

	while (addops.has(symbol))
	{
		termtyp = exptyp;
		op = symbol;
		Nexts();
		exptyp = HiPrioExpr(fs + addops);
		switch (op)
		{
		case addop:
		case subop: if (!Compatible(termtyp, fractyp) ||
			!Compatible(exptyp, fractyp))
			SemanticError(7);
			exptyp = fractyp;
			break;
		case orop: if (!Compatible(termtyp, booltyp) ||
			!Compatible(exptyp, booltyp))
			SemanticError(6);
			exptyp = booltyp;
			break;
		}
	}
	return exptyp;
}
//============================================
/*TypRec* Parser::Term(const SymSet&  fs)
{ // Synchronizacja odroczona do Factor()
  Trace t("Term", -1);
  TypRec *facttyp, *exptyp;
  SymType op;			// Nastepny operator multiplikatywny

// Term	= Factor { MultOp Factor } ;

  exptyp = Factor(fs + factiter);

  while(factiter.has(symbol))
  { facttyp = exptyp;
	  op = symbol;
	  if(mulops.has(symbol)) Nexts();
	    else SyntaxError(times);
	  exptyp = Factor(fs + factiter);
	  if(mulops.has(op))
	  switch(op)
	  { case times:
	    case divop: if(!Compatible(facttyp, fractyp) ||
			  		         !Compatible(exptyp, fractyp))
				  	        SemanticError(7);
				          exptyp = fractyp;
				          break;
	    case andop: if(!Compatible(facttyp, booltyp) ||
			  		         !Compatible(exptyp, booltyp))
				  	        SemanticError(6);
				          exptyp = booltyp;
				          break;
	  }
	  else exptyp = 0;
  }
  return exptyp;
}*/

TypRec* Parser::HiPrioExpr(const SymSet&  fs)
{ // Synchronizacja odroczona do Factor()
	Trace t("HiPrioExpr", -1);
	TypRec *facttyp, *exptyp;
	SymType op;			// Nastepny operator multiplikatywny

						// Term	= Factor { MultOp Factor } ;

	exptyp = AtomExpr(fs + factiter);

	while (factiter.has(symbol))
	{
		facttyp = exptyp;
		op = symbol;
		if (mulops.has(symbol)) Nexts();
		else SyntaxError(mulop);
		exptyp = AtomExpr(fs + factiter);
		if (mulops.has(op))
			switch (op)
			{
			case mulop:
			case divop: if (!Compatible(facttyp, fractyp) ||
				!Compatible(exptyp, fractyp))
				SemanticError(7);
				exptyp = fractyp;
				break;
			case andop: if (!Compatible(facttyp, booltyp) ||
				!Compatible(exptyp, booltyp))
				SemanticError(6);
				exptyp = booltyp;
				break;
			}
		else exptyp = 0;
	}
	return exptyp;
}
//==============================================
/*TypRec* Parser::Factor(const SymSet&  fs)
{ Trace x("Factor", fs);
  Synchronize s(factstart, fs);
  if(!can_parse) return 0;

// Factor	= Var | Constant | '('Expr')' |"not" Factor ;

  IdRec  *irp;
  TypRec *trp;

  switch(symbol)
  { case ident :irp=scope->Search(scn.Spell(), VarId | ConstId);
				if(irp->kind==ConstId)
				{ accept(ident);
				  return irp->idtyp;
				}
				else return Variable(fs);

	  case intconst: accept(intconst);  return fractyp;
	  case charconst:accept(charconst); return strtyp;
	  case lparent:  accept(lparent);
				   trp = Expression(fs+rparent);
				   accept(rparent);
				   return trp;
	  case notop:    accept(notop);
				   trp = Factor(fs);
				   if(!Compatible(trp, booltyp))
                     SemanticError(6);
				   return booltyp;
	  default:	   return 0;

  }
}*/

TypRec* Parser::AtomExpr(const SymSet&  fs)
{
	Trace x("AtomExpr", fs);
	Synchronize s(factstart, fs);
	if (!can_parse) return 0;

	// Factor	= Var | Constant | '('Expr')' |"not" Factor ;

	IdRec  *irp;
	TypRec *trp;

	switch (symbol)
	{
	case ident:irp = scope->Search(scn.Spell(), VarId | ConstId | FuncId);
		if (irp->kind == ConstId)
		{
			accept(ident);
			return irp->idtyp;
		}
		else if (irp->kind == FuncId)
		{
			return FunCall(fs);
		}
		else return Variable(fs);

	case fracconst: accept(fracconst);  return fractyp;
	case strconst:accept(strconst); return strtyp;
	case truesy: case falsesy: return booltyp;
	case lparent:  accept(lparent);
		trp = Expr(fs + rparent);
		accept(rparent);
		return trp;
	case notop:    accept(notop);
		trp = AtomExpr(fs);
		if (!Compatible(trp, booltyp))
			SemanticError(6);
		return booltyp;
	default:	   return 0;

	}
}
//================================================
/*TypRec* Parser::Variable(const SymSet&  fs)
{ Trace x("Variable", fs);
  Synchronize s(ident, fs);
  if(!can_parse) return NULL;

// Var = SimpVar | IndexedVar ;

  TypRec *vartyp, *indextyp;

  vartyp = (scope->Search(scn.Spell(), VarId))->idtyp;
  accept(ident);
  if(symbol==lbracket)
  { if(vartyp && vartyp->Kind() != Arr)
	  { SemanticError(4);
	    vartyp = 0;
	  }
	  accept(lbracket);
	  indextyp = Expression(fs + rbracket);
	  if(!Compatible(indextyp, fractyp))
	    SemanticError(5);
	  if(vartyp) vartyp = ((ArrTyp*)vartyp)->ElementTyp();
	  accept(rbracket);
  }
  return vartyp;
}*/

TypRec* Parser::Variable(const SymSet&  fs)
{
	Trace x("Variable", fs);
	Synchronize s(ident, fs);
	if (!can_parse) return NULL;

	// Var = SimpVar | IndexedVar ;

	TypRec *vartyp;

	vartyp = (scope->Search(scn.Spell(), VarId))->idtyp;
	accept(ident);
	/*if (symbol == lbracket)
	{
		if (vartyp && vartyp->Kind() != Arr)
		{
			SemanticError(4);
			vartyp = 0;
		}
		accept(lbracket);
		indextyp = Expression(fs + rbracket);
		if (!Compatible(indextyp, fractyp))
			SemanticError(5);
		if (vartyp) vartyp = ((ArrTyp*)vartyp)->ElementTyp();
		accept(rbracket);
	}*/
	return vartyp;
}
//===================================
void Parser::SemanticError(int ecode)
{
  static char* SemErr[] =
  { "Definicja homonimu",							              // 0
	  "B³êdny rodzaj identyfikatora",					        // 1
	  "Brak definicji identyfikatora",				        // 2
	  "Zmienna nie moze byc typu 'nothing'",						            // 3
	  "Wyrazenie nie moze byc typu 'nothing'",		        // 4
	  "Lista argumentow nie pasuje do funkcji",					          // 5
	  "Operand musi byæ 'bool'",    				        // 6
	  "Operandy musz¹ byæ 'frac'",					        // 7
	  "Operandy musz¹ byæ 'str' albo 'frac'",     // 8
	  "Niezgodne typy zmiennej i wyra¿enia",		      // 9
	  "Zmienna musi byæ typu 'str' albo 'frac'",  // 10
	  "Wyra¿enie musi byæ typu 'str' albo 'frac'",// 11
	  "Wyra¿enie musi byæ typu 'bool'",        // 12
	  "Wartosc zwracana ma niepoprawny typ",    //13
	  "Nie oczekiwano zwrocenia zadnej wartosci w tym zakresie",  //14
	  "Wartosc zwracana ma niepoprawny typ"    //15
  };

  scn.ScanError(FirstSemanticError + ecode, SemErr[ecode]);
}
//================================================
// Sk³adowa statyczna i funkcje  klasy Synchronize
//

Parser* Synchronize::p=0;

Synchronize::Synchronize(const SymSet& sset,
                         const SymSet& fset): f(fset)
{
  if(!sset.has(p->symbol))
  { p->SyntaxError1(p->symbol);	// Nieoczekiwany atom
	  skipto(sset+f);
  }
  p->can_parse = sset.has(p->symbol);
}
//=========================
Synchronize::~Synchronize()
{
  if(!f.has(p->symbol))
  { p->SyntaxError1(p->symbol);   // Nieoczekiwany atom
	  skipto(f);
  }
}
//========================================
void Synchronize::skipto(const SymSet &ss)
{ while(!ss.has(p->symbol)) p->Nexts();
}
