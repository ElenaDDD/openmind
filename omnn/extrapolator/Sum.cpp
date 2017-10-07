//
// Created by Сергей Кривонос on 25.09.17.
//

#include "Sum.h"
#include "Product.h"
#include "Variable.h"

namespace omnn{
namespace extrapolator {
	Valuable Sum::operator -() const
	{
		Sum s;
		for (auto& a : members) 
			s.members.push_back(-a);
		return s;
	}

	void Sum::optimize()
	{
        if (members.size()==1) {
            Become(std::move(members.front()));
        }
        else
        {
            for (auto it = members.begin(); it != members.end();)
            {
                if (*it == 0)
                {
                    members.erase(it++);
                    continue;
                }
                auto s = cast(*it);
                if (s) {
                    for(auto& m : s->members)
                    {
                        members.push_back(std::move(m));
                    }
                    members.erase(it++);
                    continue;
                }
                auto t = it;
                for (auto it2 = ++t; it2 != members.end();)
                {
                    if (it2->OfSameType(*it)
                        && !Variable::cast(*it)
                        && !Product::cast(*it))
                    {
                        const_cast<Valuable&>(*it) += *it2;
                        members.erase(it2++);
                    }
                    else
                        ++it2;
                }
                
                ++it;
            }

        }
	}

	Valuable& Sum::operator +=(const Valuable& v)
	{
		auto i = cast(v);
		if (i) {
			for (auto& a : i->members) {
				*this += a;
			}
		}
		else
		{
            for (auto& a : members) {
                if(a.OfSameType(v))
                {
                    const_cast<Valuable&>(a) += v;
                    optimize();
                    return *this;
                }
            }
            // add new member
			members.push_back(v);
		}

		return *this;
	}

	Valuable& Sum::operator +=(int v)
	{
		return *this+=(Integer(v));
	}

	Valuable& Sum::operator *=(const Valuable& v)
	{
		auto f = cast(v);
		if (f)
		{
			Sum s;
			for (auto& a : members) {
				for (auto& b : f->members) {
					s.members.push_back(a*b);
				}
			}
            s.optimize();
			members = s.members;
		}
		else
        {
            for (auto& a : members) {
                a*=v;
            }
            optimize();
		}

		return *this;
	}

	Valuable& Sum::operator /=(const Valuable& v)
	{
        // todo: store valuables in Fraction
		auto i = cast(v);
		if (i)
		{
			Sum s;
			for (auto& a : members) {
				for (auto& b : i->members) {
					s.members.push_back(a/b);
				}
			}
			*this = s;
		}
		else
		{
			auto i = Integer::cast(v);
			if (i)
			{
				Sum s;
				for (auto& a : members) {
					s.members.push_back(a/(*i));

				}
				*this = s;
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

	Valuable& Sum::operator %=(const Valuable& v)
	{
		return base::operator %=(v);
	}

	Valuable& Sum::operator --()
	{
		return *this += -1;
	}

	Valuable& Sum::operator ++()
	{
		return *this += 1;
	}

	bool Sum::operator <(const Valuable& v) const
	{

		// not implemented comparison to this Valuable descent
		return base::operator <(v);
	}

	bool Sum::operator ==(const Valuable& v) const
	{
        auto s = cast(v);
        return s && members==s->members;
	}

	std::ostream& Sum::print(std::ostream& out) const
	{
        std::stringstream s;
        s << '(';
        constexpr char sep[] = " + ";
		for (auto& b : members) 
            s << b << sep;
        auto str = s.str();
        auto cstr = const_cast<char*>(str.c_str());
        cstr[str.size() - sizeof(sep) + 1] = 0;
        out << cstr << ')';
		return out;
	}

    /** fast linear equation formula deduction */
	Formula Sum::FormulaOfVa(const Variable& v) const
	{
        std::cout << *this;
		// TODO : make it workfor non-linear as well once Formula ready
        Valuable e(0);
        const Variable* cv = nullptr; // the var found
        const Product* svp = nullptr; // product with var
        
		for (auto& m : members)
		{
           
			auto p = Product::cast(m);
			if(p)
			{
				for(auto pm : *p)
				{
					auto vp = Variable::cast(pm);
					if(*vp==v)
					{
                        if(cv || svp)
                            throw "More then one variable occurence need Implement!";
						cv=vp;
                        svp = p;
                        break;;
					}
				}
			}
			else
			{
				auto vp = Variable::cast(m);
				if(vp && *vp==v)
				{
					cv=vp;
                    // TODO: e = *this - v;
				}
				else
				{
                    // TODO : other types?
                    
					e -= m;
				}
			}
		}
        
        if(cv)
        {
            if(svp)
            {
                Product o;
                for(auto a : *svp){
                    if(a!=v)
                        o*=a;
                }
                e /= o;
            }
            return Formula::DeclareFormula(v, e);
        }
        
        throw "No such variable";
	}

}}
