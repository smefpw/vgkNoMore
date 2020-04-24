#include "Services.h"


int main()
{
	SetConsoleTitle("vgkNoMore v0.1.0-BETA | smef.pw");

	utilities::log_type(0);
	std::cout << "Made with " << termcolor::red << "<3" << termcolor::white << " by smef.pw.\n";
	
	if (!utilities::is_process_elevated())
	{
		MessageBox(nullptr, "Process requires elevation.", "vgkNoMore", MB_ICONERROR | MB_OK);
		return -1;
	}

	utilities::elevate_privileges();

	if (!service_controller::find_manager())
	{
		MessageBox(nullptr, "A fatal error occured and the program cannot continue.", "vgkNoMore", MB_ICONERROR | MB_OK);
		return -1;
	}
	
    if (service_controller::do_query_svc() == SERVICE_SYSTEM_START && service_controller::get_vgk_status() == SERVICE_RUNNING)
    {
		utilities::log_type(0);
		std::cout << "Vanguard is currently running on this system, and is set to autostart.\n";

		utilities::log_type(0);
		std::cout << "Attempting to disable autostart for Vanguard.\n";

		if (service_controller::config_vgk(true))
		{
			utilities::log_type(0);
			std::cout << "Autostart has been set to disabled state for Vanguard.\n";
		} 
		else
		{
			utilities::log_type(2);
			std::cout << "Failed to change Vanguard autostart state to disabled.\n";
		}
    } 
	else if (service_controller::do_query_svc() == SERVICE_DISABLED && service_controller::get_vgk_status() == SERVICE_STOPPED)
    {
		utilities::log_type(0);
		std::cout << "Vanguard is not running and is not set to autostart.\n";

		utilities::log_type(0);
		std::cout << "Attempting to enable autostart for Vanguard.\n";

		if (service_controller::config_vgk(false))
		{
			utilities::log_type(0);
			std::cout << "Autostart has been set to system controlled state for Vanguard.\n";
		}
		else
		{
			utilities::log_type(2);
			std::cout << "Failed to change Vanguard autostart state to system controlled.\n";
		}
	} 
	else if (service_controller::do_query_svc() == SERVICE_DISABLED && service_controller::get_vgk_status() == SERVICE_RUNNING)
	{
		utilities::log_type(0);
		std::cout << "Vanguard is running and set to not autostart.\n";

		utilities::log_type(0);
		std::cout << "Attempting to enable autostart for Vanguard.\n";

		if (service_controller::config_vgk(false))
		{
			utilities::log_type(0);
			std::cout << "Autostart has been set to system controlled state for Vanguard.\n";
		}
		else
		{
			utilities::log_type(2);
			std::cout << "Failed to change Vanguard autostart state to system controlled.\n";
		}
	}

	if ((service_controller::get_vgk_status() == SERVICE_RUNNING && service_controller::do_query_svc() == SERVICE_DISABLED) || 
		service_controller::get_vgk_status() == SERVICE_STOPPED && service_controller::do_query_svc() == SERVICE_SYSTEM_START)
	{
		const int msg_box = MessageBox(nullptr, "A system restart is required to append changes made to Vanguard service.\nDo you wish to restart now?.", "vgkNoMore", MB_ICONERROR | MB_YESNO);
		if (msg_box == IDYES)
		{
			service_controller::close_manager();
			if (!InitiateSystemShutdownA(nullptr, (LPSTR)"Vanguard service cannot be killed.", 1, TRUE, TRUE))
			{
				utilities::log_type(1);
				std::cout << "Restart request failed. Please restart manually. Press enter to close.\n";
				std::cin.get();
			}
			return 0;
		}
		
		utilities::log_type(0);
		std::cout << "Changes to Vanguard won't take place until next restart.\n";
	} 
	else
	{
		utilities::log_type(0);
		std::cout << "No restart required, no changes were made since the original session.\n";
	}

	service_controller::close_manager();
	
	utilities::log_type(0);
	std::cout << "Press enter to close app.\n";
	std::cin.get();
	return 0;
}