#include "Services.h"

/*
    Please ignore this.
    The code is disgusting.
*/

namespace service_controller
{
    TCHAR sz_svc_name[4] = "vgk";
    SC_HANDLE sch_sc_manager = nullptr;
    SC_HANDLE sch_service = nullptr;

    bool find_manager() 
    {
        sch_sc_manager = OpenSCManager(nullptr, nullptr, SC_MANAGER_ALL_ACCESS);

        if (!sch_sc_manager)
        {
            utilities::log_type(2);
            std::cerr << "OpenSCManager failed.\n";
            return false;
        }

        sch_service = OpenService(sch_sc_manager, sz_svc_name, SERVICE_ALL_ACCESS);

        if (!sch_service)
        {
            utilities::log_type(2);
            std::cerr << "OpenService failed.\n";
            CloseServiceHandle(sch_sc_manager);
            return false;
        }

        return true;
    }

    void close_manager()
    {
        CloseServiceHandle(sch_service);
        CloseServiceHandle(sch_sc_manager);
    }
	
	int get_vgk_status()
	{
		SERVICE_STATUS_PROCESS service_status;
		DWORD dw_bytes_needed = 0;

		if (!QueryServiceStatusEx(sch_service, SC_STATUS_PROCESS_INFO, (LPBYTE)&service_status, sizeof(SERVICE_STATUS_PROCESS), &dw_bytes_needed))
		{
			utilities::log_type(1);
			std::cerr << "QueryServiceStatusEx failed.\n";
			return -1;
		}

		return service_status.dwCurrentState;
	}

    int do_query_svc() //pPaste from Microsoft.
    {
        LPQUERY_SERVICE_CONFIG lpsc = nullptr;
        DWORD dwBytesNeeded, cbBufSize = 0, start_type = -1;

        if (!QueryServiceConfig(sch_service, nullptr, 0, &dwBytesNeeded))
        {
            if (ERROR_INSUFFICIENT_BUFFER == GetLastError())
            {
                cbBufSize = dwBytesNeeded;
                lpsc = (LPQUERY_SERVICE_CONFIG)LocalAlloc(LMEM_FIXED, dwBytesNeeded);
            }
            else
            {
                utilities::log_type(2);
                std::cerr << "QueryServiceConfig failed.\n";
            }
        }

        if (!QueryServiceConfig(sch_service, lpsc, cbBufSize, &dwBytesNeeded))
        {
            utilities::log_type(2);
            std::cerr << "QueryServiceConfig failed.\n";
        }

        if (lpsc != nullptr) 
        {
            start_type = lpsc->dwStartType;
            LocalFree(lpsc);
        }

        return start_type;
    }

	bool config_vgk(const bool disabled)
	{	
		if (!ChangeServiceConfig(sch_service, SERVICE_NO_CHANGE, disabled ? SERVICE_DISABLED : SERVICE_SYSTEM_START, SERVICE_NO_CHANGE,
			NULL, NULL, NULL, NULL, NULL, NULL, NULL))
		{
            utilities::log_type(2);
			std::cerr << "ChangeServiceConfig failed. This may be a result of other functions failing (if they did).\n";
			return false;
		}

		return true;
	}
}
