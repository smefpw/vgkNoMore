#pragma once
#include "utilities.h"

namespace service_controller
{
	bool find_manager();
	void close_manager();
	int get_vgk_status();
	bool config_vgk(bool disabled);
	int do_query_svc();
}