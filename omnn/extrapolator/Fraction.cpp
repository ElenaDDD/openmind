//
// Created by Сергей Кривонос on 01.09.17.
//
#include "Fraction.h"
#include "Integer.h"
#include "Sum.h"
#include "Product.h"

namespace std{
    template<>
    struct is_signed<boost::multiprecision::cpp_int> {
        bool value = true;
    };
}
namespace omnn{
namespace extrapolator {
	Fraction::Fraction(const Integer& n)
		:numerator(n), denominator(1)
	{
		
	}
    
	Valuable Fraction::operator -() const
    {
        return Fraction(-numerator, denominator);
    }

    void Fraction::optimize()
    {
        numerator.optimize();
        denominator.optimize();
        auto n = Integer::cast(numerator);
        auto dn = Integer::cast(denominator);
        if (n)
        {
            if (*n == 0)
            {
                if (dn && *dn == 0)
                    throw "NaN";
                Become(0);
                return;
            }
            if (dn)
            {
                if (*dn == 1)
                {
                    Become(Integer(*n));
                    return;
                }
                Integer::base_int d = boost::gcd(
                    static_cast<Integer::base_int>(*n),
                    static_cast<Integer::base_int>(*dn));
                numerator /= Integer(d);
                denominator /= Integer(d);
            }
        }
        else
        {
            // TODO : products
        }
    }
    
    Valuable& Fraction::operator +=(const Valuable& v)
    {
        auto i = cast(v);
        if (i){
			auto f = *i;
            if(denominator != f.denominator) {
                f.numerator *= denominator;
                f.denominator *= denominator;
                numerator *= i->denominator;
                denominator *= i->denominator;
            }

            numerator += f.numerator;
            optimize();
        }
        else
        {
            auto i = Integer::cast(v);
            if(i)
            {
                *this += Fraction(*i);
            }
            else
            {
                return Become(Sum(*this, v));
            }
        }
        return *this;
    }

    Valuable& Fraction::operator +=(int v)
    {
		auto f = cast(v);
		if (f)
		{
			numerator += v*denominator;
		}
		else
		{
			auto i = Integer::cast(v);
			if (i)
			{
				numerator += (decltype(numerator))(*i);
			}
			else
			{
				// try other type
				// no type matched
				base::operator +=(v);
			}
		}
		optimize();
		return *this;
    }

    Valuable& Fraction::operator *=(const Valuable& v)
    {
        auto f = cast(v);
        if (f)
        {
            numerator *= f->numerator;
            denominator *= f->denominator;
        }
        else{
            auto i = Integer::cast(v);
            if (i)
            {
                numerator *= (decltype(numerator))(*i);
            }
            else
            {
                return Become(Product(*this, v));
            }
        }
		
		optimize();
        return *this;
    }

    Valuable& Fraction::operator /=(const Valuable& v)
    {
        auto i = cast(v);
        if (i)
		{
			numerator *= i->denominator;
			denominator *= i->numerator;
		}
        else
        {
				auto i = Integer::cast(v);
				if (i)
				{
					denominator *= (decltype(denominator))(*i);
				}
				else
				{
					// try other type
					// no type matched
					base::operator /=(v);
				}
			
        }
		optimize();
		return *this;
    }

    Valuable& Fraction::operator %=(const Valuable& v)
    {
		return base::operator %=(v);
    }

    Valuable& Fraction::operator --()
    {
        return *this+=-1;
    }

    Valuable& Fraction::operator ++()
    {
        return *this+=1;
    }

    bool Fraction::operator <(const Valuable& v) const
    {
        auto i = cast(v);
		if (i)
		{
			auto f = *i;
			auto l = *this;
			if (l.denominator != f.denominator) {
				f.numerator *= l.denominator;
				l.numerator *= i->denominator;
			}
			return l.numerator < f.numerator;//arbitrary < i->arbitrary;
		}
            
        else
        {
			auto i = Integer::cast(v);
			if (i)
			{
				auto f = (decltype(numerator))(*i);
				auto l = *this;
				return l.numerator < f*denominator;
			}
			else
			{
				// try other type
				// no type matched
				
			}
        }

        // not implemented comparison to this Valuable descent
        return base::operator <(v);
    }

    bool Fraction::operator ==(const Valuable& v) const
    {
        auto i = cast(v);
		if (i)
		{
			auto f = *i;
			auto l = *this;
			if (l.denominator != f.denominator) {
				f.numerator *= l.denominator;
				l.numerator *= i->denominator;
			}
			return l.numerator == f.numerator;//arbitrary < i->arbitrary;
		}
        else
        {
			auto i = Integer::cast(v);
			if (i)
			{
				auto f = (decltype(numerator))(*i);
				auto l = *this;
				return l.numerator == f*denominator;
			}
			else
			{
				// try other type
				// no type matched

			}
        }
        // no type matched
        return base::operator ==(v);
    }

    std::ostream& Fraction::print(std::ostream& out) const
    {
        auto n = Integer::cast(numerator);
        auto dn = Integer::cast(denominator);
        if(!n)
            out << '(' << numerator << ')';
        else
            out << numerator;
        out << '/';
        if(!dn)
            out << '(' << denominator << ')';
        else
            out << denominator;
        return out;
    }

    const omnn::extrapolator::Fraction::base_int& Fraction::getDenominator() const
    {
        return denominator;
    }

    const omnn::extrapolator::Fraction::base_int& Fraction::getNumerator() const
    {
        return numerator;
    }

    omnn::extrapolator::Fraction Fraction::Reciprocal() const
    {
        return Fraction(denominator, numerator);
    }

    const Variable* Fraction::FindVa() const
    {
        auto va = denominator.FindVa();
        return va ? va : numerator.FindVa();
    }

    void Fraction::Eval(const Variable& va, const Valuable& v)
    {
        numerator.Eval(va, v);
        denominator.Eval(va, v);
    }
}}
