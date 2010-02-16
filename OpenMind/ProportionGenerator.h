#pragma once
#include <string>
#include <iostream>
#include "GeneralGoalGenerator.h"


class ProportionGenerator :
	public GeneralGoalGenerator<ProportionGenerator>
{
	typedef GeneralGoalGenerator<ProportionGenerator> base_t; 
public:
	ProportionGenerator(string_t::pointer name);
	Goal::ptr_t GenerateGoal();

	//Goal::ptr_t GenerateGoal()

};
