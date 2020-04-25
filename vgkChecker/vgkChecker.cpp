#include <iostream>
#include "../vgkNoMore/Services.h"

int main()
{
	SetConsoleTitle("vgkChecker v1.0-RELEASE | smef.pw");

	if (!utilities::is_process_elevated())
	{
		MessageBox(nullptr, "Process requires elevation.", "vgkChecker", MB_ICONERROR | MB_OK);
		return -1;
	}

	if (!service_controller::find_manager())
	{
		MessageBox(nullptr, "A fatal error occured and the program cannot continue.", "vgkChecker", MB_ICONERROR | MB_OK);
		return -1;
	}

	const int query = service_controller::do_query_svc();

	if (service_controller::get_vgk_status() == SERVICE_RUNNING)
	{
		utilities::log_type(0);
		std::cout << "Vanguard driver is running" << (query == SERVICE_DISABLED ? ", and is set to not auto-start.\n" : ", and is set to auto-start.\n");
	}
	else
	{
		utilities::log_type(0);
		std::cout << "Vanguard driver is not running" << (query == SERVICE_DISABLED ? ", and is set to not auto-start.\n" : ", and is set to auto-start.\n");
	}

	service_controller::close_manager();
	utilities::log_type(0);
	std::cout << "Press enter to close app.\n";
	std::cin.get();
	return 0;
}