#include "Fraction.h"

using namespace std;

unsigned int gcd(unsigned int u, unsigned int v)
{
	int shift;

	/* GCD(0,v) == v; GCD(u,0) == u, GCD(0,0) == 0 */
	if (u == 0) return v;
	if (v == 0) return u;

	/* Let shift := lg K, where K is the greatest power of 2
	dividing both u and v. */
	for (shift = 0; ((u | v) & 1) == 0; ++shift) {
		u >>= 1;
		v >>= 1;
	}

	while ((u & 1) == 0)
		u >>= 1;

	/* From here on, u is always odd. */
	do {
		/* remove all factors of 2 in v -- they are not common */
		/*   note: v is not zero, so while will terminate */
		while ((v & 1) == 0)  /* Loop X */
			v >>= 1;

		/* Now u and v are both odd. Swap if necessary so u <= v,
		then set v = v - u (which is even). For bignums, the
		swapping is just pointer movement, and the subtraction
		can be done in-place. */
		if (u > v) {
			unsigned int t = v; v = u; u = t;
		}  // Swap u and v.
		v = v - u;                       // Here v >= u.
	} while (v != 0);

	/* restore common factors of 2 */
	return u << shift;
}

Fraction parseFrac(string source)
{
	bool big = false, pointSeen = false, underscoreSeen = false, shouldBeNegative = false;
	unsigned wholes = 0, nom = 0, denom = 1;
	unsigned long long ul = 0;
	shouldBeNegative = source.at(0) == '-';
	if (source.at(0) == '+' || source.at(0) == '-')
		source.erase(source.begin(), source.begin() + 1);
	for (char& c : source)
	{
		if (isdigit(c))
		{
			ul = ul * 10 + (c - '0');
			big = big || ul > INT_MAX;
			if (big) return Fraction(0, 0, 0, shouldBeNegative);
		}
		else if (c == '.')
		{
			if (pointSeen || underscoreSeen) return Fraction(0, 0, 0, shouldBeNegative);
			else
			{
				wholes = ul;
				ul = 0;
				pointSeen = true;
			}
		}
		else if (c == '_')
		{
			if (underscoreSeen) return Fraction(0, 0, 0, shouldBeNegative);
			else
			{
				nom = ul;
				ul = 0;
				underscoreSeen = true;
			}
		}
		else
			return Fraction(0, 0, 0, shouldBeNegative);
	}
	if (underscoreSeen)
		denom = ul;
	else if (!pointSeen && !underscoreSeen)
		wholes = ul;
	else
		return Fraction(0, 0, 0, shouldBeNegative);
	Fraction toReturn(wholes, nom, denom, shouldBeNegative);
	toReturn.normalize();
	return toReturn;
}

bool Fraction::lesser(Fraction another)
{
	if (isNegative && !another.isNegative)
		return true;
	else if (!isNegative && another.isNegative)
		return false;

	unsigned long myNom = wholes * denom + nom;
	unsigned long anotherNom = another.wholes * another.denom + another.nom;

	myNom *= another.denom;
	anotherNom *= denom;

	if (isNegative && another.isNegative)
		return myNom > anotherNom;
	else if (!isNegative && !another.isNegative)
		return myNom < anotherNom;
}

bool Fraction::greater(Fraction another)
{
	if (isNegative && !another.isNegative)
		return false;
	else if (!isNegative && another.isNegative)
		return true;

	unsigned long myNom = wholes * denom + nom;
	unsigned long anotherNom = another.wholes * another.denom + another.nom;

	myNom *= another.denom;
	anotherNom *= denom;

	if (isNegative && another.isNegative)
		return myNom < anotherNom;
	else if (!isNegative && !another.isNegative)
		return myNom > anotherNom;
}

bool Fraction::equals(Fraction another)
{
	if (isNegative != another.isNegative)
		return false;

	unsigned long myNom = wholes * denom + nom;
	unsigned long anotherNom = another.wholes * another.denom + another.nom;

	myNom *= another.denom;
	anotherNom *= denom;

	return myNom == anotherNom;
}

Fraction Fraction::add(Fraction another)
{
	long myNom = wholes * denom + nom;
	long anotherNom = another.wholes * another.denom + another.nom;
	
	myNom *= another.denom;
	anotherNom *= denom;
	if (isNegative)
		myNom = -myNom;
	if (another.isNegative)
		anotherNom = -anotherNom;

	long result = myNom + anotherNom;
	bool shouldBeNegative = false;
	if (result < 0)
	{
		shouldBeNegative = true;
		result = -result;
	}

	Fraction toReturn(0, result, denom * another.denom, shouldBeNegative);
	toReturn.normalize();
	return toReturn;
}

Fraction Fraction::sub(Fraction another)
{
	long myNom = wholes * denom + nom;
	long anotherNom = another.wholes * another.denom + another.nom;

	myNom *= another.denom;
	anotherNom *= denom;
	if (isNegative)
		myNom = -myNom;
	if (another.isNegative)
		anotherNom = -anotherNom;

	long result = myNom - anotherNom;
	bool shouldBeNegative = false;
	if (result < 0)
	{
		shouldBeNegative = true;
		result = -result;
	}

	Fraction toReturn(0, result, denom * another.denom, shouldBeNegative);
	toReturn.normalize();
	return toReturn;
}

Fraction Fraction::mul(Fraction another)
{
	unsigned long myNom = wholes * denom + nom;
	unsigned long anotherNom = another.wholes * another.denom + another.nom;

	Fraction toReturn(0, myNom * anotherNom, denom * another.denom, isNegative != another.isNegative);
	return toReturn;
}

Fraction Fraction::div(Fraction another)
{
	unsigned long myNom = wholes * denom + nom;
	unsigned long anotherNom = another.wholes * another.denom + another.nom;

	Fraction toReturn(0, myNom * another.denom, denom * another.nom, isNegative != another.isNegative);
	return toReturn;
}

void Fraction::normalize()
{
	if (denom != 0)
	{
		while (nom >= denom)
		{
			nom -= denom;
			++wholes;
		}
	}
	unsigned divisor = gcd(nom, denom);
	if (divisor != 0)
	{
		nom /= divisor;
		denom /= divisor;
	}
}