#ifndef EXECUTABLE_H
#define EXECUTABLE_H

/*#include "Execution.h"
#include "Evaluable.h"

class Execution;
class Evaluable;
//shared_ptr<Value> Evaluable::evaluate(Execution& ex);
class Value;

class Executable
{
public:
	virtual void execute(Execution& ex) = 0;
	virtual ~Executable() {};
};


class FunctionCall : public Executable, Evaluable
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
};*/

#endif //EXECUTABLE_H