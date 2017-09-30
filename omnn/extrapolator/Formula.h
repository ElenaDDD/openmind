//
// Created by Сергей Кривонос on 30.09.17.
//

#pragma once
#include "Variable.h"

namespace omnn {
namespace extrapolator {


class Formula
        : public ValuableDescendantContract<Formula>
{
    using base = ValuableDescendantContract<Formula>;
    friend base;
    Variable v;
    Valuable e;
    Formula(const Variable&, const Valuable&);
    Formula(Valuable&& ex) : e(std::move(ex)){}
    Formula(int i) : e(i){}
public:
    using base::base;
    static Formula DeduceFormula(const Valuable& e, const Variable& v);
    static Formula DeclareFormula(const Variable& v, const Valuable& e);
};
}}