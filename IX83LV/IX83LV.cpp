// IX83LV.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"


// IX83Lib.cpp : ¶¨Òå DLL Ó¦ÓÃ³ÌÐòµÄµ¼³öº¯Êý¡£
//


#include <Windows.h>
#include <direct.h>
//#include "IX83Lib.h"
//#include "IX83Opt.h"
#include <string>
#include <tchar.h>
#include "mdk_if.h"
//#define _DEBUG_
#define	GT_MDK_PORT_MANAGER	"msl_pm.dll"
#ifdef IX83LIB
#define IX83LIB extern "C" _declspec(dllimport) 
#else
#define IX83LIB extern "C" _declspec(dllexport) 
#endif

//#define GetCurrentDir _getcwd
//#define	GT_MDK_PORT_MANAGER	"msl_pm.dll"

using namespace std;

typedef int(*GetMetrixcs)(int);
//typedef void(*pcloseCommand)();

//IX83Opt ix;

HMODULE						m_hMod;
fn_MSL_PM_GetInterfaceInfo	pfn_GetInterfaceInfo;
fn_MSL_PM_Initialize		pfn_InifInterface;
fn_MSL_PM_EnumInterface		pfn_EnumInterface;
fn_MSL_PM_OpenInterface		pfn_OpenInterface;
fn_MSL_PM_CloseInterface	pfn_CloseInterface;
fn_MSL_PM_SendCommand		pfn_SendCommand;
fn_MSL_PM_RegisterCallback	pfn_RegisterCallback;


void stringToWstring(std::wstring& , std::string );

IX83LIB unsigned long openPort()
{

	pfn_GetInterfaceInfo = NULL;
	pfn_InifInterface = NULL;
	pfn_EnumInterface = NULL;
	pfn_OpenInterface = NULL;
	pfn_CloseInterface = NULL;
	pfn_SendCommand = NULL;
	pfn_RegisterCallback = NULL;

	//-----------------------------------------------------
	// get function address
	m_hMod = LoadLibrary(_T(GT_MDK_PORT_MANAGER));
	if (m_hMod == NULL) return false;

	//-----------------------------------------------------
	// get function address
	pfn_InifInterface = (fn_MSL_PM_Initialize)GetProcAddress(m_hMod, "MSL_PM_Initialize");
	pfn_EnumInterface = (fn_MSL_PM_EnumInterface)GetProcAddress(m_hMod, "MSL_PM_EnumInterface");
	pfn_GetInterfaceInfo = (fn_MSL_PM_GetInterfaceInfo)GetProcAddress(m_hMod, "MSL_PM_GetInterfaceInfo");
	pfn_OpenInterface = (fn_MSL_PM_OpenInterface)GetProcAddress(m_hMod, "MSL_PM_OpenInterface");
	pfn_CloseInterface = (fn_MSL_PM_CloseInterface)GetProcAddress(m_hMod, "MSL_PM_CloseInterface");
	pfn_SendCommand = (fn_MSL_PM_SendCommand)GetProcAddress(m_hMod, "MSL_PM_SendCommand");
	pfn_RegisterCallback = (fn_MSL_PM_RegisterCallback)GetProcAddress(m_hMod, "MSL_PM_RegisterCallback");

	int ret = 0;
	void* ifData = NULL;

	
	ret = (*pfn_InifInterface)();

	pfn_GetInterfaceInfo(0, &ifData);

	return (unsigned long)ifData;
	//return (unsigned long)pfn_InifInterface;

	Sleep(1000);
	//cout << "InifInterface status: " + ret << endl;
	if (pfn_EnumInterface == NULL)	return -1;
	int count = (*pfn_EnumInterface)();
	//cout << "Interface Count:" << count << endl;
	//cout << "Interface Address:" << endl;
	
	if (count == 0)return -2;
	
	for (int i = 0; i < count; i++) {
		if (pfn_GetInterfaceInfo != NULL) {
			if ((*pfn_GetInterfaceInfo)(i, &ifData)) {
				Sleep(1000);
				//cout << "\t" << hex << (unsigned long)(ifData) << endl;
			}
		}
	}
	//address = ifData;
	
	bool rc = (*pfn_OpenInterface)(ifData);
	//return (unsigned long)ifData;
	
	
}



IX83LIB void sendCommand(unsigned long address, std::string command)
{

	MDK_MSL_CMD cmd;
	void*		result;
	bool		rc;
	memset(&cmd, 0x00, sizeof(MDK_MSL_CMD));
	command += ("\r\n");
	size_t len = command.length();
	wstring szDst;
	stringToWstring(szDst, command);
#ifdef	_UNICODE
	size_t	conv;
	wcstombs_s(&conv, (char *)cmd.m_Cmd, (size_t)MAX_COMMAND_SIZE, szDst.c_str(), _TRUNCATE);
#else
	memcpy(cmd.m_Cmd, (LPCSTR)s, len + 1);
#endif
	////////////////////////////////////////////////////////////
	// set parameters for command
	cmd.m_CmdSize = len;
	cmd.m_Callback = NULL;
	cmd.m_Context = NULL;		// this pointer passed by pv
	cmd.m_Timeout = 10000;	// (ms)
	cmd.m_Sync = FALSE;

	cmd.m_Command = TRUE;		// TRUE: Command , FALSE: it means QUERY form ('?').

								/////////////////////////////////////////////////////////
								//	Send command
	if (pfn_SendCommand != NULL && address != NULL) {
		rc = (*pfn_SendCommand)((void*)address, &cmd);
	}
}

IX83LIB int closeCommand(unsigned long address) {

	bool rc = false;
	if (address != 0) {
		if (pfn_CloseInterface == NULL)	return -1;
		sendCommand(address,"L 0,0");
		Sleep(1000);
		bool rc = (*pfn_CloseInterface)((void*)address);
	}
	return rc;
}

IX83LIB int add(int a, int b) {
	return a + b;
}



IX83LIB int user32Test(int index)
{
	HMODULE hDLL = LoadLibrary(_T("user32.dll"));
	GetMetrixcs getMetrixcs = (GetMetrixcs)GetProcAddress(hDLL, "GetSystemMetrics");
	int num = getMetrixcs(index);
	return num;

}




void stringToWstring(std::wstring& szDst, std::string str)
{
	std::string temp = str;
	int len = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)temp.c_str(), -1, NULL, 0);
	wchar_t * wszUtf8 = new wchar_t[len + 1];
	memset(wszUtf8, 0, len * 2 + 2);
	MultiByteToWideChar(CP_ACP, 0, (LPCSTR)temp.c_str(), -1, (LPWSTR)wszUtf8, len);
	szDst = wszUtf8;
	std::wstring r = wszUtf8;
	delete[] wszUtf8;
}