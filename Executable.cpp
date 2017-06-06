#include "Executable.h"
#include "Execution.h"

using namespace std;

/*void FunctionCall::execute(Execution& ex)
{
	list<shared_ptr<Value>> paramValues;
	for (auto& i : params)
		paramValues.push_back(i->evaluate(ex));
	ex.callFunction(funIdent, paramValues, defaultReturnValue);
}

shared_ptr<Value> FunctionCall::evaluate(Execution& ex)
{
	execute(ex);
	return ex.retrieveReturnValue();
}

void AssignmentEx::execute(Execution& ex)
{
	ex.assignValue(varIdent, valueToAssign->evaluate(ex));
}

void ReturnEx::execute(Execution& ex)
{
	ex.returnFromFunction(toReturn->evaluate(ex));
}

void Conditional::execute(Execution& ex)
{
	if (static_pointer_cast<Boolean>(condition->evaluate(ex))->isTrue())
		ex.materialize(toDoIfTrue);
	else
		ex.materialize(toDoIfFalse);
}

void Looped::execute(Execution& ex)
{
	if (static_pointer_cast<Boolean>(loopCondition->evaluate(ex))->isTrue())
	{
		toRepeat.push_back(shared_ptr<Executable>(new Looped(loopCondition, toRepeat)));
		ex.materialize(toRepeat);
	}
}

void PrintEx::execute(Execution& ex)
{
	for (auto& i : data)
		i->evaluate(ex)->print();
}

void InputEx::execute(Execution& ex)
{
	for (auto& i : vars)
		ex.getValue(i)->input();
}

void VarDeclaration::execute(Execution& ex)
{
	for(auto& i : varIdent)
		ex.addVariable(i, initValue);
}*/