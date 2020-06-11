#include <iostream>
#include "Definitions.h"

using namespace std;

#define VCS_FAIL 0

void ShowDialog()
{
	HANDLE Handler;
	DWORD ErrCodes;
	WORD NodeId;

	Handler = VCS_OpenDeviceDlg(&ErrCodes);
}

int main()
{
	cout << "Maxon Motor Demo Project" << endl;
	
	HANDLE Handler;
	DWORD ErrorCode;
	WORD NodeId = 0;

	DWORD Baudrate = 19200;
	DWORD TimeOutMs = 500;

	char* DeviceName = (char*) "EPOS4";
	char* ProtocolStackName = (char*) "MAXON SERIAL V2";
	char* InterfaceName = (char*) "USB";
	char* PortName = (char*) "USB0";
	bool bAbsolute = false;
	bool bImmediately = true;

	Handler = VCS_OpenDevice(DeviceName, ProtocolStackName, InterfaceName, PortName, &ErrorCode);
	VCS_SetProtocolStackSettings(Handler, Baudrate, TimeOutMs, &ErrorCode);
	VCS_SetOperationMode(Handler, NodeId, OMD_PROFILE_POSITION_MODE, &ErrorCode);
	VCS_SetPositionProfile(Handler, NodeId, 100, 50, 50, &ErrorCode);

	// Recommended) Use VCS_SetState function
	VCS_SetEnableState(Handler, NodeId, &ErrorCode);
	
	VCS_MoveToPosition(Handler, NodeId, 1000, bAbsolute, bImmediately, &ErrorCode);

	long CurrentPosition;
	if (VCS_GetPositionIs(Handler, NodeId, &CurrentPosition, &ErrorCode) == VCS_FAIL)
	{
		cout << "Error was detected in Get Position Command" << endl;
	}
	cout << "[Before]Position : " << CurrentPosition << endl;

	if (VCS_WaitForTargetReached(Handler, NodeId, 5000, &ErrorCode) == VCS_FAIL)
	{
		cout << "Can't reach to target" << endl;
	}

	if (VCS_GetPositionIs(Handler, NodeId, &CurrentPosition, &ErrorCode) == VCS_FAIL)
	{
		cout << "Error was detected in Get Position Command" << endl;
	}
	cout << "[After]Position : " << CurrentPosition << endl;

	VCS_SetDisableState(Handler, NodeId, &ErrorCode);
	if (VCS_CloseDevice(Handler, &ErrorCode) == VCS_FAIL)
	{
		cout << "Error was detected in Close Command" << endl;
		cout << ErrorCode << endl;
	}

	return 0;
}