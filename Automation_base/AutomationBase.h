#pragma once
#include <iostream>
#include <string>
#include <vector>


class AutomationBase
{
public:
	enum class AutomationType
	{
		FSA = 0, NPDA = 1
	};
	AutomationBase(AutomationType type);
	virtual bool accepts(const std::string& word, std::string& computation, bool shouldReturnComputation = false) const = 0;
	virtual int addState() = 0;
	virtual int addTransition(const std::vector<std::string>& args) = 0;
	virtual int makeStateFinal(size_t ind) = 0;

	virtual std::string getString() const = 0;
	virtual std::string getVisualizeString() const = 0;

	virtual AutomationBase* clone() const = 0;

	AutomationType getType() const;
private:
	AutomationType type;
};