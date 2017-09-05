#pragma once
#include "../StateEventsIdentifier.h"

class BaseStateEventCommand{
public:
	BaseStateEventCommand();
	virtual ~BaseStateEventCommand();

	//StateEventsIdentifier::ID getCreatorStateEventIdent() const;
private:
	//creator of this command state event
	StateEventsIdentifier::ID mCreatorStateEventIdent;
};

