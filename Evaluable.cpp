#include "Evaluable.h"

using namespace std;

/*shared_ptr<Value> LesserThan::evaluate(Execution& ex)
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
	return static_pointer_cast<Boolean>(left->evaluate(ex))->or(static_pointer_cast<Boolean>(right->evaluate(ex)));
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
	return static_pointer_cast<Boolean>(left->evaluate(ex))->and(static_pointer_cast<Boolean>(right->evaluate(ex)));
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
}*/