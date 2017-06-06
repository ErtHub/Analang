#include "Parser.h"

using namespace std;

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






Parser::Parser(Scan& sc, Execution& e): scn(sc), ex(e), scope(0)
// Otwiera "GLOBAL" Scope
{
  Synchronize::p = this;    // Zwi¹zanie z klas¹ Synchronize

  ststart  = SymSet(inputsy,  printsy,
                    ifsy,    whilesy, ident, returnsy, varsy, EOS);
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

  VarDummy = new IdRec("@@VDUMMY", VarId, 0, 0),
  ConstDummy = new IdRec("@@CDUMMY", ConstId, 0, 0),
  FuncDummy = new IdRec("@@FDUMMY", FuncId, 0, 0);

  // Utwórz zasiêg globalny nazw
  OpenScope("GLOBAL");

  // Deskryptory typów wbudowanych
  fractyp  = new TypRec(Frac);
  booltyp = new TypRec(Bool);
  strtyp = new TypRec(Str);
  nothingtyp = new TypRec(Nothing);
  expectedreturn = nothingtyp;

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
bool Parser::NextExecutable()
{
	Trace("NextExecutable", -1);
	shared_ptr<Executable> nextComm;
	shared_ptr<Execution::FunctionPrototype> nextFun;
	int nextFunId;
	if (ststart.has(symbol))
	{
		Stment(ststart + SymSet(funcsy, others, EOS), nextComm);
		if (!error)
			ex.addExecutable(nextComm);
		else
			error = false;
		return true;
	}
	else if (symbol == funcsy)
	{
		FunDecl(ststart + SymSet(funcsy, others, EOS), nextFunId, nextFun);
		if (!error)
			ex.addFunction(nextFunId, nextFun);
		else
			error = false;
		return true;
	}
	return false;

}
//=========================================================
void Parser::StmentBlock(const SymSet& fs, list<shared_ptr<Executable>>& blk)
{
	Trace x("StmentBlock", fs);
	Synchronize s(beginsy, fs);

	shared_ptr<Executable> nextComm;

	if (!can_parse) return;

	accept(beginsy);
	while (ststart.has(symbol))
	{
		Stment(fs + ststart + endsy, nextComm);
		if (!error)
			blk.push_back(nextComm);
		else
			error = false;
	}
	accept(endsy);
}
//============================================
void Parser::VarDecl(const SymSet&  fs, shared_ptr<Executable>& decls)
{
	Trace x("VarDecl", fs);
	Synchronize s(varsy, fs);
	if (!can_parse) return;

	IdRecList irl;

	accept(varsy);
	accept(colon);

	TypRec* typ;
	shared_ptr<Value> initVal;
	if ((typ = Type(fs + ident)) == nothingtyp)
	{
		SemanticError(3);
		//error = false;
		//return;
		initVal = shared_ptr<Value>(new NothingVal());
	}
	else if (typ == booltyp)
		initVal = shared_ptr<Value>(new Boolean(true));
	else if (typ == fractyp)
		initVal = shared_ptr<Value>(new FractionVal(Fraction()));
	else if (typ == strtyp)
		initVal = shared_ptr<Value>(new StringVal(""));

	bool nothingError = error;
	error = false;

	list<int> ids;
	IdRec* desc = scope->Install(scn.Spell(), VarId, NULL);
	irl.Append(desc);
	if (!error)
		ids.push_back(desc->uniqueId);
	else
		error = false;
	accept(ident);

	while (symbol == comma)
	{
		accept(comma);
		desc = scope->Install(scn.Spell(), VarId, NULL);
		irl.Append(desc);
		if (!error)
			ids.push_back(desc->uniqueId);
		else
			error = false;
		accept(ident);

	}
	irl.AddTyp(typ);
	decls = shared_ptr<Executable>(new VarDeclaration(ids, initVal));

	error = nothingError;
}
//============================================
TypRec* Parser::Type(const SymSet&  fs)
{
	Trace x("Type", fs);
	Synchronize s(types, fs);
	if (!can_parse) return 0;
	switch (symbol)
	{
	case fracsy: accept(fracsy); return fractyp;
	case boolsy: accept(boolsy); return booltyp;
	case strsy: accept(strsy); return strtyp;
	case nothingsy: default: accept(nothingsy); return nothingtyp;
	}
}
//=================================================
void Parser::FunDecl(const SymSet&  fs, int& funId, shared_ptr<Execution::FunctionPrototype>& prot)
{
	Trace x("FunDecl", fs);
	Synchronize s(fs + funcsy, fs);
	if (!can_parse) return;

	string fname = "???";
	string paramname = "???";
	IdRec *procidptr;
	IdRec *paramidptr;

	list<shared_ptr<Executable>> body;
	list<int> paramList;

	accept(funcsy);
	accept(colon);
	TypRec* typ = Type(fs + ident);
	expectedreturn = typ;

	if (symbol == ident) fname = scn.Spell();
	procidptr = scope->Install(fname, FuncId, typ);

	bool homonymError = error;
	error = false;

	accept(ident);
	accept(lparent);

	OpenScope(fname);

	if (types.has(symbol))
	{
		TypRec* ptyp = Type(fs + ident);
		if(symbol == ident) paramname = scn.Spell();
		paramidptr = scope->Install(paramname, VarId, ptyp);
		if (!error)
		{
			procidptr->params.push_back(paramidptr->idtyp);
			paramList.push_back(paramidptr->uniqueId);
		}
		else
			error = false;
		paramname = "???";
		accept(ident);
		

		while(symbol==comma)
		{
			accept(comma);
			ptyp = Type(fs + ident);
			if (symbol == ident) paramname = scn.Spell();
			paramidptr = scope->Install(paramname, VarId, ptyp);
			if (!error)
			{
				procidptr->params.push_back(paramidptr->idtyp);
				paramList.push_back(paramidptr->uniqueId);
			}
			else
				error = false;
			paramname = "???";
			accept(ident);
		}

	}
	accept(rparent);

	StmentBlock(fs, body);

	funId = procidptr->uniqueId;
	prot = shared_ptr<Execution::FunctionPrototype>(new Execution::FunctionPrototype(body, paramList));

	CloseScope();
	expectedreturn = nothingtyp;
	error = homonymError;
}

TypRec* Parser::FunCall(const SymSet& fs, shared_ptr<FunctionCall>& callComm)
{
	Trace x("FunCall", fs);
	Synchronize s(ident, fs);

	string funName = scn.Spell();
	if (!can_parse) return NULL;
	IdRec *funid = scope->Search(funName, FuncId);
	list<TypRec*> ptyps;
	list<shared_ptr<Evaluable>> callParams;
	shared_ptr<Evaluable> currParam;
	TypRec* paramTyp;
	accept(ident);
	accept(lparent);
	if (symbol != rparent)
	{
		paramTyp = (Expr(fs + SymSet(comma, rparent, EOS), currParam));
		ptyps.push_back(paramTyp);
		callParams.push_back(currParam);
		while (symbol == comma)
		{
			accept(comma);
			paramTyp = (Expr(fs + SymSet(comma, rparent, EOS), currParam));
			ptyps.push_back(paramTyp);
			callParams.push_back(currParam);
		}
	}
	else if (ptyps != funid->params)
		SemanticError(5);
	shared_ptr<Value> defaultRet;
	if (funid->idtyp == nothingtyp)
		defaultRet = shared_ptr<Value>(new NothingVal());
	else if (funid->idtyp == booltyp)
		defaultRet = shared_ptr<Value>(new Boolean(true));
	else if (funid->idtyp == fractyp)
		defaultRet = shared_ptr<Value>(new FractionVal(Fraction()));
	else if (funid->idtyp == strtyp)
		defaultRet = shared_ptr<Value>(new StringVal(""));
	callComm = shared_ptr<FunctionCall>(new FunctionCall(funid->uniqueId, callParams, defaultRet));
	accept(rparent);
	return funid->idtyp;
}

void Parser::Return(const SymSet& fs, shared_ptr<Executable>& ret)
{
	Trace x("Return", fs);
	Synchronize s(returnsy, fs);
	if (expectedreturn == nothingtyp)
		SemanticError(14);
	accept(returnsy);
	shared_ptr<Evaluable> val;
	TypRec* etyp = Expr(fs, val);
	if (!Compatible(etyp, expectedreturn))
		SemanticError(15);
	ret = shared_ptr<Executable>(new ReturnEx(val));
}
//=========================================
void Parser::Stment(const SymSet&  fs, shared_ptr<Executable>& st)
{
	Trace x("Stment", fs);
	Synchronize s(ststart, fs);
	if (!can_parse) return;

	switch (symbol)
	{
		IdRec *irp;

	case ifsy:	  Cond(fs, st);	   break;
	case whilesy: Loop(fs, st); break;
	case inputsy:  InputStment(fs, st);  break;
	case printsy: PrintStment(fs, st); break;
	case returnsy: Return(fs, st); break;
	case varsy: VarDecl(fs, st); break;

	case ident:
		// Mo¿e byæ wywo³anie procedury albo przypisanie.
		// Rozró¿nienie: na podstawie atrybutów identyfikatora.

		irp = scope->Search(scn.Spell(), (VarId | FuncId));
		if (irp->kind == VarId)
			Assignment(fs, st);
		else
		{
			shared_ptr<FunctionCall> c;
			FunCall(fs, c);
			st = static_pointer_cast<Executable>(c);
		}
	}
}
//=============================================
void Parser::Cond(const SymSet&  fs, shared_ptr<Executable>& cond)
{
	Trace t("Cond", -1);
	TypRec *etyp;

	shared_ptr<Evaluable> condExpr;
	list<shared_ptr<Executable>> onTrue;
	list<shared_ptr<Executable>> onFalse;

	accept(ifsy);
	accept(lparent);
	etyp = Expr(fs + rparent, condExpr);
	if (!Compatible(etyp, booltyp)) SemanticError(12);
	bool condError = error;
	error = false;
	accept(rparent);
	OpenScope("CondIf");
	StmentBlock(fs + elsesy, onTrue);
	CloseScope();
	if (symbol == elsesy)
	{
		accept(elsesy);
		OpenScope("CondElse");
		StmentBlock(fs, onFalse);
		CloseScope();
	}
	cond = shared_ptr<Executable>(new Conditional(condExpr, onTrue, onFalse));
	error = condError;
}
//==============================================
void Parser::Loop(const SymSet&  fs, shared_ptr<Executable>& lp)
{
	Trace t("Loop", -1);
	TypRec *etyp;

	shared_ptr<Evaluable> condExpr;
	list<shared_ptr<Executable>> onLoop;

	accept(whilesy);
	accept(lparent);
	etyp = Expr(fs + rparent, condExpr);
	if (!Compatible(etyp, booltyp)) SemanticError(12);
	bool loopError = error;
	error = false;
	accept(rparent);
	OpenScope("Loop");
	StmentBlock(fs, onLoop);
	CloseScope();
	lp = shared_ptr<Executable>(new Looped(condExpr, onLoop));
	error = loopError;
}
//===============================================
void Parser::Assignment(const SymSet&  fs, shared_ptr<Executable>& assgn)
{
	Trace t("Assignment", -1);
	TypRec *vtyp, *etyp;

	shared_ptr<Evaluable> var;
	shared_ptr<Evaluable> val;

	vtyp = Variable(fs + becomes, var);
	accept(becomes);
	etyp = Expr(fs, val);
	if (!Compatible(vtyp, etyp))
		SemanticError(9);		// Zmienna i wyra¿enie niezgodne
	assgn = shared_ptr<Executable>(new AssignmentEx(static_pointer_cast<VariableEx>(var)->getIdent(), val));
}
//=============================================
void Parser::InputStment(const SymSet&  fs, shared_ptr<Executable>& inp)
{
	Trace t("InputStment", -1);
	shared_ptr<Evaluable> var;
	list<int> varIds;
	accept(inputsy);
	accept(lparent);
	if (Variable(fs + SymSet(comma, rparent, EOS), var) == nothingtyp)
		SemanticError(3);
	varIds.push_back(static_pointer_cast<VariableEx>(var)->getIdent());
	while (symbol == comma)
	{
		accept(comma);
		if (Variable(fs + SymSet(comma, rparent, EOS), var) == nothingtyp)
			SemanticError(3);
		varIds.push_back(static_pointer_cast<VariableEx>(var)->getIdent());
	}
	accept(rparent);
	inp = shared_ptr<Executable>(new InputEx(varIds));
}
//================================================
void Parser::PrintStment(const SymSet&  fs, shared_ptr<Executable>& prt)
{ // Synchronizacja w procedurze Stmt()
	Trace t("PrintStment", -1);

	shared_ptr<Evaluable> eval;
	list<shared_ptr<Evaluable>> toPrint;

	accept(printsy);
	accept(lparent);
	if (Expr(fs + SymSet(comma, rparent, EOS), eval) == nothingtyp)
		SemanticError(4);
	toPrint.push_back(eval);
	while (symbol == comma)
	{
		accept(comma);
		if (Expr(fs + SymSet(comma, rparent, EOS), eval) == nothingtyp)
			SemanticError(4);
		toPrint.push_back(eval);
	}
	accept(rparent);
	prt = shared_ptr<Executable>(new PrintEx(toPrint));
}
//===================================================
TypRec* Parser::Expr(const SymSet&  fs, shared_ptr<Evaluable>& xpr)
{
	Trace t("Expr", -1);
	TypRec *et, *et1;

	et = MedPrioExpr(fs + relops, xpr);
	if (relops.has(symbol))
	{
		SymType compKind = symbol;
		shared_ptr<Evaluable> otherXpr;
		et1 = et;
		Nexts();
		et = MedPrioExpr(fs, otherXpr);
		//if (!((Compatible(et1, fractyp) && Compatible(et, fractyp)) ||
			//(Compatible(et1, strtyp) && Compatible(et, strtyp))))//TODO: Do something about this!
			//SemanticError(8);
		if (Compatible(et1, fractyp) && Compatible(et, fractyp))
		{
			if (compKind == ltop)
				xpr = shared_ptr<Evaluable>(new LesserThan(xpr, otherXpr));
			else if (compKind == gtop)
				xpr = shared_ptr<Evaluable>(new GreaterThan(xpr, otherXpr));
			else if (compKind == eqop)
				xpr = shared_ptr<Evaluable>(new Equal(xpr, otherXpr));
		}
		else if (Compatible(et1, strtyp) && Compatible(et, strtyp))
		{
			if (compKind == ltop)
				xpr = shared_ptr<Evaluable>(new LexicalLesser(xpr, otherXpr));
			else if (compKind == gtop)
				xpr = shared_ptr<Evaluable>(new LexicalGreater(xpr, otherXpr));
			else if (compKind == eqop)
				xpr = shared_ptr<Evaluable>(new LexicalEqual(xpr, otherXpr));
		}
		else
			SemanticError(8);
		et = booltyp;
	}
	return et;
}
//==================================================
TypRec* Parser::MedPrioExpr(const SymSet&  fs, shared_ptr<Evaluable>& xpr)
{
	Trace x("MedPrioExpr", fs);
	Synchronize s(factstart + signs, fs);
	TypRec *termtyp, *exptyp;
	int     signum = 0;
	SymType op;
	SymType sign;

	if (!can_parse) return 0;

	if (signs.has(symbol)) { sign = symbol; Nexts(); signum = 1; }
	exptyp = HiPrioExpr(fs + addops, xpr);
	//if (signum && !Compatible(exptyp, fractyp))
		//SemanticError(7);
	if(signum)
	{
		if (!Compatible(exptyp, fractyp))
			SemanticError(7);
		if (sign == subop)
			xpr = shared_ptr<Evaluable>(new ArithNegation(xpr));
	}

	while (addops.has(symbol))
	{
		shared_ptr<Evaluable> otherXpr;
		termtyp = exptyp;
		op = symbol;
		Nexts();
		exptyp = HiPrioExpr(fs + addops, otherXpr);
		switch (op)
		{
		case addop: if (!Compatible(termtyp, fractyp) ||
			!Compatible(exptyp, fractyp))
			SemanticError(7);
			xpr = shared_ptr<Evaluable>(new AddOp(xpr, otherXpr));
			exptyp = fractyp;
			break;
		case subop: if (!Compatible(termtyp, fractyp) ||
			!Compatible(exptyp, fractyp))
			SemanticError(7);
			xpr = shared_ptr<Evaluable>(new SubOp(xpr, otherXpr));
			exptyp = fractyp;
			break;
		case orop: if (!Compatible(termtyp, booltyp) ||
			!Compatible(exptyp, booltyp))
			SemanticError(6);
			xpr = shared_ptr<Evaluable>(new OrOp(xpr, otherXpr));
			exptyp = booltyp;
			break;
		}
	}
	return exptyp;
}
//============================================
TypRec* Parser::HiPrioExpr(const SymSet&  fs, shared_ptr<Evaluable>& xpr)
{
	Trace t("HiPrioExpr", -1);
	TypRec *facttyp, *exptyp;
	SymType op;

	exptyp = AtomExpr(fs + factiter, xpr);

	while (mulops.has(symbol))
	{
		shared_ptr<Evaluable> otherXpr;
		facttyp = exptyp;
		op = symbol;
		Nexts();
		//if (mulops.has(symbol)) Nexts();
		//else SyntaxError(mulop);
		exptyp = AtomExpr(fs + factiter, otherXpr);
		if (mulops.has(op))
			switch (op)
			{
			case mulop: if (!Compatible(facttyp, fractyp) ||
				!Compatible(exptyp, fractyp))
				SemanticError(7);
				xpr = shared_ptr<Evaluable>(new MulOp(xpr, otherXpr));
				exptyp = fractyp;
				break;
			case divop: if (!Compatible(facttyp, fractyp) ||
				!Compatible(exptyp, fractyp))
				SemanticError(7);
				xpr = shared_ptr<Evaluable>(new DivOp(xpr, otherXpr));
				exptyp = fractyp;
				break;
			case andop: if (!Compatible(facttyp, booltyp) ||
				!Compatible(exptyp, booltyp))
				SemanticError(6);
				xpr = shared_ptr<Evaluable>(new AndOp(xpr, otherXpr));
				exptyp = booltyp;
				break;
			}
		else exptyp = 0;
	}
	return exptyp;
}
//==============================================
TypRec* Parser::AtomExpr(const SymSet&  fs, shared_ptr<Evaluable>& xpr)
{
	Trace x("AtomExpr", fs);
	Synchronize s(factstart, fs);
	if (!can_parse) return 0;

	IdRec  *irp;
	TypRec *trp;
	TypRec *toReturn;

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
			shared_ptr<FunctionCall> c;
			toReturn = FunCall(fs, c);
			xpr = static_pointer_cast<Evaluable>(c);
			return toReturn;
		}
		else return Variable(fs, xpr);

	case fracconst:
		xpr = shared_ptr<Evaluable>(new FractionVal(scn.FracConst()));
		accept(fracconst);
		return fractyp;
	case strconst:
		xpr = shared_ptr<Evaluable>(new StringVal(scn.Spell()));
		accept(strconst);
		return strtyp;
	case truesy:
		xpr = shared_ptr<Evaluable>(new Boolean(true));
		return booltyp;
	case falsesy:
		xpr = shared_ptr<Evaluable>(new Boolean(false));
		return booltyp;
	case lparent:  accept(lparent);
		trp = Expr(fs + rparent, xpr);
		accept(rparent);
		return trp;
	case notop:    accept(notop);
		trp = AtomExpr(fs, xpr);
		if (!Compatible(trp, booltyp))
			SemanticError(6);
		xpr = shared_ptr<Evaluable>(new Negation(xpr));
		return booltyp;
	default:	   return 0;

	}
}
//================================================
TypRec* Parser::Variable(const SymSet&  fs, shared_ptr<Evaluable>& v)
{
	Trace x("Variable", fs);
	Synchronize s(ident, fs);
	if (!can_parse) return NULL;

	TypRec *vartyp;
	IdRec* desc;

	desc = scope->Search(scn.Spell(), VarId);
	vartyp = desc->idtyp;
	v = shared_ptr<Evaluable>(new VariableEx(desc->uniqueId));
	accept(ident);
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

  error = true;

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
