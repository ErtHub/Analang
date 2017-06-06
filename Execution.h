#ifndef EXECUTION_H
#define EXECUTION_H

#include <list>
#include <map>
#include <stack>
#include <memory>
#include "Executable.h"
#include "Evaluable.h"
#include <string>
#include <iostream>
#include "Value.h"
#include "Fraction.h"

class Executable;
class Value;

class Execution
{
public:
	class FunctionPrototype
	{
	private:
		std::list<std::shared_ptr<Executable>> functionBody;
		std::list<int> paramIds;
	public:
		std::list<int> getParamIds() { return paramIds; };//------
		std::list<std::shared_ptr<Executable>> getBody() { return functionBody; };
		FunctionPrototype(std::list<std::shared_ptr<Executable>> b, std::list<int> p) : functionBody(b), paramIds(p) {};
	};

	void run();
	void addExecutable(std::shared_ptr<Executable> toAdd);
	void addFunction(int funIdent, std::shared_ptr<FunctionPrototype> funDef);
	//void startFunctionBuild();
	//void finishFunctionBuild();
	void addVariable(int varIdent, std::shared_ptr<Value> initialValue);
	void materialize(std::list<std::shared_ptr<Executable>> comms);
	int assignValue(int varIdent, std::shared_ptr<Value> val);
	std::shared_ptr<Value> getValue(int varIdent);
	std::shared_ptr<Value> retrieveReturnValue();
	int callFunction(int funIdent, std::list<std::shared_ptr<Value>> params, std::shared_ptr<Value> defRet);
	void returnFromFunction(std::shared_ptr<Value> withValue);
	void cut();
	Execution() { commands.push(std::list<std::shared_ptr<Executable>>()); };

private:
	std::stack<std::list<std::shared_ptr<Executable>>> commands;
	//std::stack<std::list<std::shared_ptr<Executable>>::iterator> currComm;
	std::stack<std::map<int, std::shared_ptr<Value>>> callStackVariables;

	std::map<int, std::shared_ptr<Value>> globalVariables;
	std::shared_ptr<Value> returnValue;
	//std::list<std::shared_ptr<Executable>> functionBuild;
	//std::list<std::shared_ptr<Executable>>* executableAdditionContext;

	

	std::map<int, std::shared_ptr<FunctionPrototype>> definedFunctions;
};

class Evaluable
{
public:
	virtual std::shared_ptr<Value> evaluate(Execution& ex) = 0;
	virtual ~Evaluable() {};
};
/*class Value : public Evaluable
{
public:
virtual void print() = 0;
virtual int input() = 0;
~Value() {};
std::shared_ptr<Value> evaluate(Execution& ex) { return std::shared_ptr<Value>(this); };
};*/

class BinaryOp : public Evaluable
{
public:
	BinaryOp(std::shared_ptr<Evaluable> l, std::shared_ptr<Evaluable> r) : left(l), right(r) {};
	~BinaryOp() {};
protected:
	std::shared_ptr<Evaluable> left;
	std::shared_ptr<Evaluable> right;
};

class LesserThan : public BinaryOp
{
public:
	LesserThan(std::shared_ptr<Evaluable> l, std::shared_ptr<Evaluable> r) : BinaryOp(l, r) {};
	std::shared_ptr<Value> evaluate(Execution& ex);
	~LesserThan() {};
};

class GreaterThan : public BinaryOp
{
public:
	GreaterThan(std::shared_ptr<Evaluable> l, std::shared_ptr<Evaluable> r) : BinaryOp(l, r) {};
	std::shared_ptr<Value> evaluate(Execution& ex);
	~GreaterThan() {};
};

class Equal : public BinaryOp
{
public:
	Equal(std::shared_ptr<Evaluable> l, std::shared_ptr<Evaluable> r) : BinaryOp(l, r) {};
	std::shared_ptr<Value> evaluate(Execution& ex);
	~Equal() {};
};

class ArithNegation : public Evaluable
{
public:
	ArithNegation(std::shared_ptr<Evaluable> e) : xpr(e) {};
	std::shared_ptr<Value> evaluate(Execution& ex);
	~ArithNegation() {};
private:
	std::shared_ptr<Evaluable> xpr;
};

class AddOp : public BinaryOp
{
public:
	AddOp(std::shared_ptr<Evaluable> l, std::shared_ptr<Evaluable> r) : BinaryOp(l, r) {};
	std::shared_ptr<Value> evaluate(Execution& ex);
	~AddOp() {};
};

class SubOp : public BinaryOp
{
public:
	SubOp(std::shared_ptr<Evaluable> l, std::shared_ptr<Evaluable> r) : BinaryOp(l, r) {};
	std::shared_ptr<Value> evaluate(Execution& ex);
	~SubOp() {};
};

class OrOp : public BinaryOp
{
public:
	OrOp(std::shared_ptr<Evaluable> l, std::shared_ptr<Evaluable> r) : BinaryOp(l, r) {};
	std::shared_ptr<Value> evaluate(Execution& ex);
	~OrOp() {};
};

class MulOp : public BinaryOp
{
public:
	MulOp(std::shared_ptr<Evaluable> l, std::shared_ptr<Evaluable> r) : BinaryOp(l, r) {};
	std::shared_ptr<Value> evaluate(Execution& ex);
	~MulOp() {};
};

class DivOp : public BinaryOp
{
public:
	DivOp(std::shared_ptr<Evaluable> l, std::shared_ptr<Evaluable> r) : BinaryOp(l, r) {};
	std::shared_ptr<Value> evaluate(Execution& ex);
	~DivOp() {};
};

class AndOp : public BinaryOp
{
public:
	AndOp(std::shared_ptr<Evaluable> l, std::shared_ptr<Evaluable> r) : BinaryOp(l, r) {};
	std::shared_ptr<Value> evaluate(Execution& ex);
	~AndOp() {};
};

class Negation : public Evaluable
{
public:
	Negation(std::shared_ptr<Evaluable> e) : exprToNegate(e) {};
	std::shared_ptr<Value> evaluate(Execution& ex);
	~Negation() {};
private:
	std::shared_ptr<Evaluable> exprToNegate;
};

class LexicalLesser : public BinaryOp
{
public:
	LexicalLesser(std::shared_ptr<Evaluable> l, std::shared_ptr<Evaluable> r) : BinaryOp(l, r) {};
	std::shared_ptr<Value> evaluate(Execution& ex);
	~LexicalLesser() {};
};

class LexicalGreater : public BinaryOp
{
public:
	LexicalGreater(std::shared_ptr<Evaluable> l, std::shared_ptr<Evaluable> r) : BinaryOp(l, r) {};
	std::shared_ptr<Value> evaluate(Execution& ex);
	~LexicalGreater() {};
};

class LexicalEqual : public BinaryOp
{
public:
	LexicalEqual(std::shared_ptr<Evaluable> l, std::shared_ptr<Evaluable> r) : BinaryOp(l, r) {};
	std::shared_ptr<Value> evaluate(Execution& ex);
	~LexicalEqual() {};
};

class VariableEx : public Evaluable
{
public:
	VariableEx(int i) : varIdent(i) {};
	std::shared_ptr<Value> evaluate(Execution& ex);
	~VariableEx() {};
	int getIdent() { return varIdent; };
private:
	int varIdent;
};

class Value : public Evaluable
{
public:
	virtual void print() = 0;
	virtual int input() = 0;
	~Value() {};
	//std::shared_ptr<Value> evaluate(Execution& ex) { return std::shared_ptr<Value>(this); };
};

class NothingVal : public Value
{
public:
	virtual void print() { std::cout << std::endl << "Analang: There's nothing here!" << std::endl; };
	virtual int input() { std::cout << std::endl << "Analang: I'd receive user input right now, but there's nothing to scan to here!" << std::endl; return -1; };
	~NothingVal() {};
	NothingVal() {};
	std::shared_ptr<Value> evaluate(Execution& ex) { return std::shared_ptr<Value>(new NothingVal()); };
};

class FractionVal : public Value
{
public:
	virtual void print();
	virtual int input();
	~FractionVal() {};
	std::shared_ptr<Value> isLesserThan(std::shared_ptr<FractionVal> another);
	std::shared_ptr<Value> isGreaterThan(std::shared_ptr<FractionVal> another);
	std::shared_ptr<Value> isEqual(std::shared_ptr<FractionVal> another);
	std::shared_ptr<Value> opp();
	std::shared_ptr<Value> plus(std::shared_ptr<FractionVal> another);
	std::shared_ptr<Value> minus(std::shared_ptr<FractionVal> another);
	std::shared_ptr<Value> times(std::shared_ptr<FractionVal> another);
	std::shared_ptr<Value> divBy(std::shared_ptr<FractionVal> another);
	FractionVal(Fraction c) : core(c) {};
	std::shared_ptr<Value> evaluate(Execution& ex) { return std::shared_ptr<Value>(new FractionVal(core)); };
private:
	Fraction core;
};

class StringVal : public Value
{
public:
	virtual void print() { std::cout << core; };
	virtual int input() { std::cin >> core; std::cin.clear(); std::cin.sync(); return 0; };
	~StringVal() {};
	std::shared_ptr<Value> isLesserThan(std::shared_ptr<StringVal> another);
	std::shared_ptr<Value> isGreaterThan(std::shared_ptr<StringVal> another);
	std::shared_ptr<Value> isEqual(std::shared_ptr<StringVal> another);
	StringVal(std::string c) : core(c) {};
	std::shared_ptr<Value> evaluate(Execution& ex) { return std::shared_ptr<Value>(new StringVal(core)); };
private:
	std::string core;
};

class Boolean : public Value
{
public:
	virtual void print() { std::cout << core; };
	virtual int input() {
		std::string vessel; std::cin >> vessel;
		std::cin.clear(); std::cin.sync();
		if (vessel == "0")
			core = false;
		else
			core = true;
		return 0;
	};
	~Boolean() {};
	std::shared_ptr<Value> isEqual(std::shared_ptr<Boolean> another);
	std::shared_ptr<Value> and (std::shared_ptr<Boolean> another);
	std::shared_ptr<Value> or (std::shared_ptr<Boolean> another);
	std::shared_ptr<Value> not();
	bool isTrue() { return core; };
	Boolean(bool c) : core(c) {};
	std::shared_ptr<Value> evaluate(Execution& ex) { return std::shared_ptr<Value>(new Boolean(core)); };
private:
	bool core;
};

class Executable
{
public:
	virtual void execute(Execution& ex) = 0;
	virtual ~Executable() {};
};


class FunctionCall : public Executable, public Evaluable
{
public:
	void execute(Execution& ex);
	std::shared_ptr<Value> evaluate(Execution& ex);
	~FunctionCall() {};
	FunctionCall(int i, std::list<std::shared_ptr<Evaluable>> p, std::shared_ptr<Value> v) : funIdent(i), params(p), defaultReturnValue(v) {};
private:
	int funIdent;
	std::list<std::shared_ptr<Evaluable>> params;
	std::shared_ptr<Value> defaultReturnValue;
};

class AssignmentEx : public Executable
{
public:
	void execute(Execution& ex);
	~AssignmentEx() {};
	AssignmentEx(int i, std::shared_ptr<Evaluable> v) : varIdent(i), valueToAssign(v) {};
private:
	int varIdent;
	std::shared_ptr<Evaluable> valueToAssign;
};

class ReturnEx : public Executable
{
public:
	void execute(Execution& ex);
	~ReturnEx() {};
	ReturnEx(std::shared_ptr<Evaluable> v) : toReturn(v) {};
private:
	std::shared_ptr<Evaluable> toReturn;
};

class Conditional : public Executable
{
public:
	void execute(Execution& ex);
	~Conditional() {};
	Conditional(std::shared_ptr<Evaluable> c, std::list<std::shared_ptr<Executable>> t, std::list<std::shared_ptr<Executable>> f) : condition(c), toDoIfTrue(t), toDoIfFalse(f) {};
private:
	std::shared_ptr<Evaluable> condition;
	std::list<std::shared_ptr<Executable>> toDoIfTrue;
	std::list<std::shared_ptr<Executable>> toDoIfFalse;
};

class Looped : public Executable
{
public:
	void execute(Execution& ex);
	~Looped() {};
	Looped(std::shared_ptr<Evaluable> c, std::list<std::shared_ptr<Executable>> r) : loopCondition(c), toRepeat(r) {};
private:
	std::shared_ptr<Evaluable> loopCondition;
	std::list<std::shared_ptr<Executable>> toRepeat;
};

class PrintEx : public Executable
{
public:
	void execute(Execution& ex);
	~PrintEx() {};
	PrintEx(std::list<std::shared_ptr<Evaluable>> d) : data(d) {};
private:
	std::list<std::shared_ptr<Evaluable>> data;
};

class InputEx : public Executable
{
public:
	void execute(Execution& ex);
	~InputEx() {};
	InputEx(std::list<int> v) : vars(v) {};
private:
	std::list<int> vars;
};

class VarDeclaration : public Executable
{
public:
	void execute(Execution& ex);
	~VarDeclaration() {};
	VarDeclaration(std::list<int> i, std::shared_ptr<Value> v) : varIdent(i), initValue(v) {};
private:
	std::list<int> varIdent;
	std::shared_ptr<Value> initValue;
};

#endif //EXECUTION_H