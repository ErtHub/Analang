#ifndef EVALUABLE_H
#define EVALUABLE_H

/*#include <string>
#include <memory>
#include <iostream>
#include "Execution.h"
#include "Value.h"
#include "Fraction.h"

class Value;
class Execution;

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
};/

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
	std::shared_ptr<Value> evaluate(Execution& ex) { return std::shared_ptr<Value>(this); };
};

class NothingVal : public Value
{
public:
	virtual void print() { std::cout << std::endl << "Analang: There's nothing here!" << std::endl; };
	virtual int input() { std::cout << std::endl << "Analang: I'd receive user input right now, but there's nothing to scan to here!" << std::endl; return -1; };
	~NothingVal() {};
	NothingVal() {};
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
private:
	bool core;
};*/

#endif //EVALUABLE_H