#include "AutomationBase.h"

AutomationBase::AutomationBase(AutomationType type) : type(type) {}

AutomationBase::AutomationType AutomationBase::getType() const
{
	return type;
}