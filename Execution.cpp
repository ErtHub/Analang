#include "Execution.h"

using namespace std;

void Execution::addExecutable(shared_ptr<Executable> toAdd)
{
	commands.top().push_back(toAdd);
}

/*void Execution::startFunctionBuild()
{
	executableAdditionContext = &functionBuild;
}*/

int Execution::assignValue(int varIdent, shared_ptr<Value> val)
{
	map<int, shared_ptr<Value>>::iterator i;
	if ((callStackVariables.empty()) || ((i = callStackVariables.top().find(varIdent)) == callStackVariables.top().end()))
		if ((i = globalVariables.find(varIdent)) == globalVariables.end())
			return -1;
	//*i = pair<string, shared_ptr<Value>>(varIdent, val);
	i->second = val;
	return 0;
}

shared_ptr<Value> Execution::getValue(int varIdent)
{
	map<int, shared_ptr<Value>>::iterator i;
	if ((callStackVariables.empty()) || ((i = callStackVariables.top().find(varIdent)) == callStackVariables.top().end()))
		if ((i = globalVariables.find(varIdent)) == globalVariables.end())
			return shared_ptr<Value>(new NothingVal());
	return i->second;
}

int Execution::callFunction(int funIdent, list<shared_ptr<Value>> params, shared_ptr<Value> defRet)
{
	map<int, shared_ptr<FunctionPrototype>>::iterator i;
	if ((i = definedFunctions.find(funIdent)) == definedFunctions.end())
		return -1;
	list<int> paramIds = i->second->getParamIds();
	commands.push(i->second->getBody());
	//currComm.push(commands.top().begin());
	list<int>::iterator ipn = paramIds.begin();
	list<shared_ptr<Value>>::iterator ip = params.begin();
	map<int, shared_ptr<Value>> localVariables;
	while (ipn != paramIds.end() && ip != params.end())
	{
		localVariables.insert(pair<int, shared_ptr<Value>>(*ipn, *ip));
		++ipn;
		++ip;
	}
	callStackVariables.push(localVariables);
	returnValue = defRet;
	run();
	commands.pop();
	callStackVariables.pop();
	return 0;
}

void Execution::addVariable(int varIdent, shared_ptr<Value> initialValue)
{
	//cout << "addVariable" << endl;
	map<int, shared_ptr<Value>>::iterator i;
	if (!callStackVariables.empty())
	{
		//cout << "addVariableIf" << endl;
		if ((i = callStackVariables.top().find(varIdent)) == callStackVariables.top().end())
			callStackVariables.top().insert(pair<int, shared_ptr<Value>>(varIdent, initialValue));
		else
			i->second = initialValue;
	}
	else
	{
		//cout << "addVariableElse" << endl;
		if ((i = globalVariables.find(varIdent)) == globalVariables.end())
			globalVariables.insert(pair<int, shared_ptr<Value>>(varIdent, initialValue));
		else
		{
			//cout << "guilty?" << endl;
			i->second = initialValue;
		}
	}
}

void Execution::materialize(list<shared_ptr<Executable>> comms)
{
	commands.top().insert(++(commands.top().begin()), comms.begin(), comms.end());
}

shared_ptr<Value> Execution::retrieveReturnValue()
{
	shared_ptr<Value> toReturn = returnValue;
	returnValue = shared_ptr<Value>(new NothingVal());
	return toReturn;
}

void Execution::returnFromFunction(shared_ptr<Value> withValue)
{
	returnValue = withValue;
	//currComm.pop();
	//commands.pop();
	//callStackVariables.pop();
	cut();
}

void Execution::addFunction(int funIdent, shared_ptr<FunctionPrototype> funDef)
{
	definedFunctions.insert(pair<int, shared_ptr<FunctionPrototype>>(funIdent, funDef));
}

void Execution::run()
{
	cout << "run" << endl;
	while (!commands.top().empty())
	{
		//list<shared_ptr<Executable>>::iterator i;
		/*if ((i = currComm.top()) == commands.top().end())
			returnFromFunction(shared_ptr<Value>(new Nothing()));
		else
		{
			(*i)->execute(*this);
			++i;
		}*/
		commands.top().front()->execute(*this);
		commands.top().pop_front();
	}
}

void Execution::cut()
{
	commands.top().erase(++(commands.top().begin()), commands.top().end());
}

shared_ptr<Value> LesserThan::evaluate(Execution& ex)
{
	return static_pointer_cast<FractionVal>(left->evaluate(ex))->isLesserThan(static_pointer_cast<FractionVal>(right->evaluate(ex)));
}

shared_ptr<Value> GreaterThan::evaluate(Execution& ex)
{
	return static_pointer_cast<FractionVal>(left->evaluate(ex))->isGreaterThan(static_pointer_cast<FractionVal>(right->evaluate(ex)));
}

shared_ptr<Value> Equal::evaluate(Execution& ex)
{
	return static_pointer_cast<FractionVal>(left->evaluate(ex))->isEqual(static_pointer_cast<FractionVal>(right->evaluate(ex)));
}

shared_ptr<Value> ArithNegation::evaluate(Execution& ex)
{
	return static_pointer_cast<FractionVal>(xpr->evaluate(ex))->opp();
}

shared_ptr<Value> AddOp::evaluate(Execution& ex)
{
	return static_pointer_cast<FractionVal>(left->evaluate(ex))->plus(static_pointer_cast<FractionVal>(right->evaluate(ex)));
}

shared_ptr<Value> SubOp::evaluate(Execution& ex)
{
	return static_pointer_cast<FractionVal>(left->evaluate(ex))->minus(static_pointer_cast<FractionVal>(right->evaluate(ex)));
}

shared_ptr<Value> OrOp::evaluate(Execution& ex)
{
	return static_pointer_cast<Boolean>(left->evaluate(ex))-> or (static_pointer_cast<Boolean>(right->evaluate(ex)));
}

shared_ptr<Value> MulOp::evaluate(Execution& ex)
{
	return static_pointer_cast<FractionVal>(left->evaluate(ex))->times(static_pointer_cast<FractionVal>(right->evaluate(ex)));
}

shared_ptr<Value> DivOp::evaluate(Execution& ex)
{
	return static_pointer_cast<FractionVal>(left->evaluate(ex))->divBy(static_pointer_cast<FractionVal>(right->evaluate(ex)));
}

shared_ptr<Value> AndOp::evaluate(Execution& ex)
{
	return static_pointer_cast<Boolean>(left->evaluate(ex))-> and (static_pointer_cast<Boolean>(right->evaluate(ex)));
}

shared_ptr<Value> Negation::evaluate(Execution& ex)
{
	return static_pointer_cast<Boolean>(exprToNegate->evaluate(ex))->not();
}

shared_ptr<Value> LexicalLesser::evaluate(Execution& ex)
{
	return static_pointer_cast<StringVal>(left->evaluate(ex))->isLesserThan(static_pointer_cast<StringVal>(right->evaluate(ex)));
}

shared_ptr<Value> LexicalGreater::evaluate(Execution& ex)
{
	return static_pointer_cast<StringVal>(left->evaluate(ex))->isGreaterThan(static_pointer_cast<StringVal>(right->evaluate(ex)));
}

shared_ptr<Value> LexicalEqual::evaluate(Execution& ex)
{
	return static_pointer_cast<StringVal>(left->evaluate(ex))->isEqual(static_pointer_cast<StringVal>(right->evaluate(ex)));
}

shared_ptr<Value> VariableEx::evaluate(Execution& ex)
{
	return ex.getValue(varIdent);
}

shared_ptr<Value> FractionVal::isLesserThan(shared_ptr<FractionVal> another)
{
	return shared_ptr<Value>(new Boolean(core.lesser(another->core)));
}

shared_ptr<Value> FractionVal::isGreaterThan(shared_ptr<FractionVal> another)
{
	return shared_ptr<Value>(new Boolean(core.greater(another->core)));
}

shared_ptr<Value> FractionVal::isEqual(shared_ptr<FractionVal> another)
{
	return shared_ptr<Value>(new Boolean(core.equals(another->core)));
}

shared_ptr<Value> FractionVal::opp()
{
	return shared_ptr<Value>(new FractionVal(core.negate()));
}

shared_ptr<Value> FractionVal::plus(shared_ptr<FractionVal> another)
{
	return shared_ptr<Value>(new FractionVal(core.add(another->core)));
}

shared_ptr<Value> FractionVal::minus(shared_ptr<FractionVal> another)
{
	return shared_ptr<Value>(new FractionVal(core.sub(another->core)));
}

shared_ptr<Value> FractionVal::times(shared_ptr<FractionVal> another)
{
	return shared_ptr<Value>(new FractionVal(core.mul(another->core)));
}

shared_ptr<Value> FractionVal::divBy(shared_ptr<FractionVal> another)
{
	return shared_ptr<Value>(new FractionVal(core.div(another->core)));
}

shared_ptr<Value> StringVal::isLesserThan(shared_ptr<StringVal> another)
{
	return shared_ptr<Value>(new Boolean(core.compare(another->core) < 0));
}

shared_ptr<Value> StringVal::isGreaterThan(shared_ptr<StringVal> another)
{
	return shared_ptr<Value>(new Boolean(core.compare(another->core) > 0));
}

shared_ptr<Value> StringVal::isEqual(shared_ptr<StringVal> another)
{
	return shared_ptr<Value>(new Boolean(core == another->core));
}

shared_ptr<Value> Boolean::isEqual(shared_ptr<Boolean> another)
{
	return shared_ptr<Value>(new Boolean(core == another->core));
}

shared_ptr<Value> Boolean:: and (shared_ptr<Boolean> another)
{
	return shared_ptr<Value>(new Boolean(core && another->core));
}

shared_ptr<Value> Boolean:: or (shared_ptr<Boolean> another)
{
	return shared_ptr<Value>(new Boolean(core || another->core));
}

shared_ptr<Value> Boolean::not()
{
	return shared_ptr<Value>(new Boolean(!core));
}

void FractionVal::print()
{
	unsigned d = core.getDenom();
	if (d == 0)
		cout << "NaN";
	else
		cout << core.getWholes() << "." << core.getNom() << "_" << d;
}

int FractionVal::input()
{
	string vessel;
	cin >> vessel;
	core = parseFrac(vessel);
	return 0;
}

void FunctionCall::execute(Execution& ex)
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
	cout << endl;
}

void InputEx::execute(Execution& ex)
{
	for (auto& i : vars)
		ex.getValue(i)->input();
}

void VarDeclaration::execute(Execution& ex)
{
	for (auto& i : varIdent)
		ex.addVariable(i, initValue);
}