#include "Finate_state_automation\FiniteStateAutomation.h"
#include "Non-deterministic_pushdown_automaton\NPDA.h"
#include "Environment\Environment.h"

#include  "CommandShell\Dispatcher\CommandDispatcher.h"
#include  "GUI\AFL.h"

using namespace System;
using namespace System::Windows::Forms;

[STAThreadAttribute]
int main() 
{	
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	AFL::AFL form;
	Application::Run(%form);

	CommandDispatcher d;
	cout << d.dispatch("dsfds test");
	return 0;
}
 
