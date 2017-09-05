#include "BaseAction.h"


BaseAction::BaseAction()
{
}


BaseAction::~BaseAction()
{
}


void BaseAction::execute()
{
	if (isActionExecuteable() != std::string(""))
		return;

	executeAction();
}