#pragma once
#include <string>
#include <memory>

class BaseAction{
public:
	typedef std::unique_ptr<BaseAction> Ptr;
public:
	BaseAction();
	virtual ~BaseAction();

	void execute();
	//if returns empty string, this action is executeable
	//returning any string message is meant for the error message
	//i.e, reason why the action is not executeable
	virtual std::string isActionExecuteable() const = 0;
private:
	virtual void executeAction() = 0;
private:

};

