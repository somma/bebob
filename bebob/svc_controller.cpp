/**----------------------------------------------------------------------------
 * svc_controller.cpp
 *-----------------------------------------------------------------------------
 * 
 *-----------------------------------------------------------------------------
 * All rights reserved by Noh,Yonghwan (fixbrain@gmail.com, unsorted@msn.com)
 *-----------------------------------------------------------------------------
 * 2014:8:19 14:28 created
**---------------------------------------------------------------------------*/
#include "stdafx.h"
#include "scm_context.h"

#include "..\share\fc_drv_ioctl.h"
#include "..\share\fc_drv_share.h"


void show_usage(_In_ const wchar_t* argv0);

/**
 * @brief	
 * @param	
 * @see		
 * @remarks	
 * @code		
 * @endcode	
 * @return	
**/
int _tmain(int argc, _TCHAR* argv[])
{
	if (argc!= 2) 
	{
		show_usage(argv[0]);
		return -1;
	}

	// check driver file exists
	wchar_t* driver_path = argv[1];
	if (true != is_file_existsW(driver_path))
	{
		log_err L"file does not exists. file = %s", driver_path log_end
		return -1;
	}

	// install service and start service for our driver.
	scm_context scm(driver_path, _service_name, _service_name_display, true);
	if (true != scm.install_driver())
	{
		log_err L"scm.install_drvier() failed." log_end
		return -1;
	}
	log_info L"service = [%s] installed successfully.", _service_name_display log_end

	if (true != scm.start_driver())
	{
		log_err L"scm.start_driver() failed." log_end
		return -1;
	}
	log_info L"service = [%s] started successfully.", _service_name_display log_end

	// wait for user's input
	log_msg L">> press any [enter] to terminate..." log_end
	getchar();

	// uninstall and stop service
	scm.stop_driver();
	scm.uninstall_driver();

	return 0;
}


/**
 * @brief	show usage
 * @param	
 * @see		
 * @remarks	
 * @code		
 * @endcode	
 * @return	
**/
void show_usage(_In_ const wchar_t* argv0)
{
	_ASSERTE(NULL != argv0);
	if (NULL == argv0) return;

	log_info 
		L"\n\n======================================================\n"\
		L"==                                                  ==\n"\
		L"== bebob                                            ==\n"\
		L"== driver controller by somma(fixbrain@gmail.com)   ==\n"\
		L"======================================================\n"\
		L"usage : %s [driver_path]\n"\
		L"usage : %s d:\\some_dir\\kdrv.sys\n", 		
		argv0, argv0	
	log_end
}