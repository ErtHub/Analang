#ifndef FRACTION_H
#define FRACTION_H

#include <string>
#include <cctype>
#include <climits>

class Fraction
{
private:
	unsigned wholes = 0;
	unsigned nom = 0;
	unsigned denom = 1;
	bool isNegative;
public:
	void normalize();
	Fraction negate() { return Fraction(wholes, nom, denom, !isNegative); };
	bool lesser(Fraction another);
	bool greater(Fraction another);
	bool equals(Fraction another);
	Fraction add(Fraction another);
	Fraction sub(Fraction another);
	Fraction mul(Fraction another);
	Fraction div(Fraction another);
	unsigned getWholes() { return wholes; };
	unsigned getNom() { return nom; };
	unsigned getDenom() { return denom; };
	bool getNegative() { return isNegative; };
	Fraction(unsigned w, unsigned n, unsigned d, bool neg) : wholes(w), nom(n), denom(d), isNegative(neg) {};
	Fraction() : wholes(0), nom(0), denom(1), isNegative(false) {};
};

Fraction parseFrac(std::string source);

#endif //FRACTION_H