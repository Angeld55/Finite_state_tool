#include <iostream>
#include <vector>
#include <string>

using namespace std;

#include "GUI\AFL_main_form.h"

using namespace System;
using namespace System::Windows::Forms;

int WinMain() 
{	
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	AFL_forms::AFL_main_form form;
	Application::Run(%form);
	return 0;
}
 
