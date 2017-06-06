#ifndef VALUE_H
#define VALUE_H

/*#include "Evaluable.h"
#include "Fraction.h"
#include <iostream>

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
	virtual int input() { std::string vessel; std::cin >> vessel;
	std::cin.clear(); std::cin.sync();
	if (vessel == "0")
		core = false;
	else
		core = true;
	return 0; };
	~Boolean() {};
	std::shared_ptr<Value> isEqual(std::shared_ptr<Boolean> another);
	std::shared_ptr<Value> and(std::shared_ptr<Boolean> another);
	std::shared_ptr<Value> or(std::shared_ptr<Boolean> another);
	std::shared_ptr<Value> not();
	bool isTrue() { return core; };
	Boolean(bool c) : core(c) {};
private:
	bool core;
};*/

#endif //VALUE_H