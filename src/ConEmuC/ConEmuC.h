
#pragma once

#ifdef _DEBUG
	// ����������������� ��� ������ � ������� ���������� ������ Comspec
	#define PRINT_COMSPEC(f,a) //wprintf(f,a)
	#define DEBUGSTR(s) OutputDebugString(s)
#elif defined(__GNUC__)
	//  �����������������, ����� ����� ����� ������� �������� (conemuc.exe) �������� MessageBox, ����� ����������� ����������
	//  #define SHOW_STARTED_MSGBOX
	#define PRINT_COMSPEC(f,a) //wprintf(f,a)
	#define DEBUGSTR(s)
#else
	#define PRINT_COMSPEC(f,a)
	#define DEBUGSTR(s)
#endif

#define DEBUGLOG(s) //DEBUGSTR(s)
#define DEBUGLOGINPUT(s) //DEBUGSTR(s)
#define DEBUGLOGSIZE(s) DEBUGSTR(s)
#define DEBUGLOGLANG(s) //DEBUGSTR(s) //; Sleep(2000)


#define CSECTION_NON_RAISE

#include <Windows.h>
#include <WinCon.h>
#ifdef _DEBUG
#include <stdio.h>
#endif
#include <Shlwapi.h>
#include <Tlhelp32.h>
//#include <vector>
#include "..\common\common.hpp"
#include "..\common\ConEmuCheck.h"

#ifdef _DEBUG
extern wchar_t gszDbgModLabel[6];
#endif

#define START_MAX_PROCESSES 1000
#define CHECK_PROCESSES_TIMEOUT 500
#define CHECK_ANTIVIRUS_TIMEOUT 6*1000
#define CHECK_ROOTSTART_TIMEOUT 10*1000
#define CHECK_ROOTOK_TIMEOUT 10*1000
#define MAX_FORCEREFRESH_INTERVAL 1000
#define MAX_SYNCSETSIZE_WAIT 1000
#define GUI_PIPE_TIMEOUT 300
#define MAX_CONREAD_SIZE 30000 // � ������
#define RELOAD_INFO_TIMEOUT 500
#define REQSIZE_TIMEOUT 500
#define GUIREADY_TIMEOUT 10000

#define IMAGE_SUBSYSTEM_DOS_EXECUTABLE  255

#ifdef _DEBUG
//CRITICAL_ SECTION gcsHeap;
//#define MCHKHEAP { Enter CriticalSection(&gcsHeap); int MDEBUG_CHK=_CrtCheckMemory(); _ASSERTE(MDEBUG_CHK); LeaveCriticalSection(&gcsHeap); }
#define MCHKHEAP HeapValidate(ghHeap, 0, NULL);
//#define HEAP_LOGGING
#else
#define MCHKHEAP
#endif

//#ifndef _DEBUG
// �������� �����
#define FORCE_REDRAW_FIX
#define RELATIVE_TRANSMIT_DISABLE
//#else
//// ���������� �����
////#define FORCE_REDRAW_FIX
//#endif

#if !defined(CONSOLE_APPLICATION_16BIT)
#define CONSOLE_APPLICATION_16BIT       0x0001
#endif


#if defined(__GNUC__)
	//#include "assert.h"
	#ifndef _ASSERTE
		#define _ASSERTE(x)
	#endif
	#ifndef _ASSERT
		#define _ASSERT(x)
	#endif
#else
	#include <crtdbg.h>
#endif

#ifndef EVENT_CONSOLE_CARET
	#define EVENT_CONSOLE_CARET             0x4001
	#define EVENT_CONSOLE_UPDATE_REGION     0x4002
	#define EVENT_CONSOLE_UPDATE_SIMPLE     0x4003
	#define EVENT_CONSOLE_UPDATE_SCROLL     0x4004
	#define EVENT_CONSOLE_LAYOUT            0x4005
	#define EVENT_CONSOLE_START_APPLICATION 0x4006
	#define EVENT_CONSOLE_END_APPLICATION   0x4007
#endif

#define SafeCloseHandle(h) { if ((h)!=NULL) { HANDLE hh = (h); (h) = NULL; if (hh!=INVALID_HANDLE_VALUE) CloseHandle(hh); } }


DWORD WINAPI InstanceThread(LPVOID);
DWORD WINAPI ServerThread(LPVOID lpvParam);
DWORD WINAPI InputThread(LPVOID lpvParam);
DWORD WINAPI InputPipeThread(LPVOID lpvParam);
BOOL GetAnswerToRequest(CESERVER_REQ& in, CESERVER_REQ** out); 
DWORD WINAPI WinEventThread(LPVOID lpvParam);
void WINAPI WinEventProc(HWINEVENTHOOK hWinEventHook, DWORD event, HWND hwnd, LONG idObject, LONG idChild, DWORD dwEventThread, DWORD dwmsEventTime);
void CheckCursorPos();
//void SendConsoleChanges(CESERVER_REQ* pOut);
//CESERVER_REQ* CreateConsoleInfo(CESERVER_CHAR* pRgnOnly, BOOL bCharAttrBuff);
//BOOL ReloadConsoleInfo(CESERVER_CHAR* pChangedRgn=NULL); // ���������� TRUE � ������ ���������
//BOOL ReloadFullConsoleInfo(/*CESERVER_CHAR* pCharOnly=NULL*/); // � ��� ����� ������������ ����������
BOOL ReloadFullConsoleInfo(BOOL abForceSend);
DWORD WINAPI RefreshThread(LPVOID lpvParam); // ����, �������������� ���������� �������
//BOOL ReadConsoleData(CESERVER_CHAR* pCheck = NULL); //((LPRECT)1) ��� �������� LPRECT
void SetConsoleFontSizeTo(HWND inConWnd, int inSizeY, int inSizeX, wchar_t *asFontName);
int ServerInit(); // ������� ����������� ������� � ����
void ServerDone(int aiRc);
int ComspecInit();
void ComspecDone(int aiRc);
BOOL SetConsoleSize(USHORT BufferHeight, COORD crNewSize, SMALL_RECT rNewRect, LPCSTR asLabel = NULL);
void CreateLogSizeFile(int nLevel);
void LogSize(COORD* pcrSize, LPCSTR pszLabel);
BOOL WINAPI HandlerRoutine(DWORD dwCtrlType);
int GetProcessCount(DWORD *rpdwPID, UINT nMaxCount);
SHORT CorrectTopVisible(int nY);
BOOL CorrectVisibleRect(CONSOLE_SCREEN_BUFFER_INFO* pSbi);
WARNING("������ GetConsoleScreenBufferInfo ����� ������������ MyGetConsoleScreenBufferInfo!");
BOOL MyGetConsoleScreenBufferInfo(HANDLE ahConOut, PCONSOLE_SCREEN_BUFFER_INFO apsc);
//void EnlargeRegion(CESERVER_CHAR_HDR& rgn, const COORD crNew);
void CmdOutputStore();
void CmdOutputRestore();
LPVOID Alloc(size_t nCount, size_t nSize);
void Free(LPVOID ptr);
void CheckConEmuHwnd();
HWND FindConEmuByPID();
typedef BOOL (__stdcall *FGetConsoleKeyboardLayoutName)(wchar_t*);
extern FGetConsoleKeyboardLayoutName pfnGetConsoleKeyboardLayoutName;
void CheckKeyboardLayout();
int CALLBACK FontEnumProc(ENUMLOGFONTEX *lpelfe, NEWTEXTMETRICEX *lpntme, DWORD FontType, LPARAM lParam);
typedef DWORD (WINAPI* FGetConsoleProcessList)(LPDWORD lpdwProcessList, DWORD dwProcessCount);
extern FGetConsoleProcessList pfnGetConsoleProcessList;
//BOOL HookWinEvents(int abEnabled);
BOOL CheckProcessCount(BOOL abForce=FALSE);
BOOL IsNeedCmd(LPCWSTR asCmdLine, BOOL *rbNeedCutStartEndQuot);
BOOL FileExists(LPCWSTR asFile);
extern bool GetImageSubsystem(const wchar_t *FileName,DWORD& ImageSubsystem);
void SendStarted();
BOOL SendConsoleEvent(INPUT_RECORD* pr, UINT nCount);
typedef BOOL (WINAPI *FDebugActiveProcessStop)(DWORD dwProcessId);
extern FDebugActiveProcessStop pfnDebugActiveProcessStop;
typedef BOOL (WINAPI *FDebugSetProcessKillOnExit)(BOOL KillOnExit);
extern FDebugSetProcessKillOnExit pfnDebugSetProcessKillOnExit;
void ProcessDebugEvent();
BOOL IsUserAdmin();
#ifdef CRTPRINTF
void _wprintf(LPCWSTR asBuffer);
void _printf(LPCSTR asBuffer);
void _printf(LPCSTR asFormat, DWORD dwErr);
void _printf(LPCSTR asFormat, DWORD dwErr, LPCWSTR asAddLine);
void _printf(LPCSTR asFormat, DWORD dw1, DWORD dw2, LPCWSTR asAddLine=NULL);
#else
#define _printf printf
#define _wprintf wprintf
#endif
const wchar_t* PointToName(const wchar_t* asFullPath);


int ParseCommandLine(LPCWSTR asCmdLine, wchar_t** psNewCmd); // ������ ���������� ��������� ������
void Help();
void ExitWaitForKey(WORD vkKey, LPCWSTR asConfirm, BOOL abNewLine, BOOL abDontShowConsole);

int CreateMapHeader();
void CloseMapHeader();

void EmergencyShow();


/* Console Handles */
extern MConHandle ghConIn;
extern MConHandle ghConOut;


/*  Global  */
extern DWORD   gnSelfPID;
//HANDLE  ghConIn = NULL, ghConOut = NULL;
extern HWND    ghConWnd;
extern HWND    ghConEmuWnd; // Root! window
extern HANDLE  ghExitEvent;
extern HANDLE  ghFinalizeEvent;
extern BOOL    gbAlwaysConfirmExit, gbInShutdown, gbAutoDisableConfirmExit;
extern int     gbRootWasFoundInCon;
extern BOOL    gbAttachMode;
extern BOOL    gbForceHideConWnd;
extern DWORD   gdwMainThreadId;
//int       gnBufferHeight = 0;
extern wchar_t* gpszRunCmd;
extern DWORD   gnImageSubsystem;
//HANDLE  ghCtrlCEvent = NULL, ghCtrlBreakEvent = NULL;
extern HANDLE ghHeap; //HeapCreate(HEAP_GENERATE_EXCEPTIONS, nMinHeapSize, 0);
#ifdef _DEBUG
extern size_t gnHeapUsed, gnHeapMax;
extern HANDLE ghFarInExecuteEvent;
#endif

typedef enum tag_RunMode {
	RM_UNDEFINED = 0,
	RM_SERVER,
	RM_COMSPEC
} RunMode;

extern BOOL gbNoCreateProcess;
extern BOOL gbDebugProcess;
extern int  gnCmdUnicodeMode;
extern BOOL gbRootIsCmdExe;

#ifdef WIN64
#pragma message("ComEmuC compiled in X64 mode")
#define NTVDMACTIVE FALSE
#else
#pragma message("ComEmuC compiled in X86 mode")
#define NTVDMACTIVE (srv.bNtvdmActive)
#endif

typedef struct tag_SrvInfo {
	HANDLE hRootProcess;    DWORD dwRootProcess;  DWORD dwRootStartTime; BOOL bDebuggerActive;
	//
	HANDLE hServerThread;   DWORD dwServerThreadId;
	HANDLE hRefreshThread;  DWORD dwRefreshThread;
	HANDLE hWinEventThread; DWORD dwWinEventThread;
	HANDLE hInputThread;    DWORD dwInputThreadId;
	HANDLE hInputPipeThread;DWORD dwInputPipeThreadId; // Needed in Vista & administrator
	//
	//UINT nMsgHookEnableDisable;
	UINT nMaxFPS;
	//
	MSection *csProc;
	//CRITICAL_SECTION csConBuf;
	// ������ ��������� ��� �����, ����� ����������, ����� ������� ��� �� �����.
	// ��������, ��������� FAR, �� �������� Update, FAR �����������...
	//std::vector<DWORD> nProcesses;
	UINT nProcessCount, nMaxProcesses;
	DWORD* pnProcesses, *pnProcessesCopy, nProcessStartTick;
#ifndef WIN64
	BOOL bNtvdmActive; DWORD nNtvdmPID;
#endif
	BOOL bTelnetActive;
	//
	wchar_t szPipename[MAX_PATH], szInputname[MAX_PATH], szGuiPipeName[MAX_PATH];
	//
	HANDLE hFileMapping, hFileMappingData;
	CESERVER_REQ_CONINFO_HDR *pConsoleInfo;
	CHAR_INFO *pConsoleData; // Mapping
	CHAR_INFO *pConsoleDataCopy; // Local (Alloc)
	DWORD nConsoleDataSize;
	//
	HANDLE hConEmuGuiAttached;
	HWINEVENTHOOK /*hWinHook,*/ hWinHookStartEnd; //BOOL bWinHookAllow; int nWinHookMode;
	//BOOL bContentsChanged; // ������ ������ ���������� ������ ���� ������
	//wchar_t* psChars;
	//WORD* pnAttrs;
	//DWORD nBufCharCount;  // ������������ ������ (����� ���������� ������)
	//DWORD nOneBufferSize; // ������ ��� ������� � GUI (������� ������)
	//WORD* ptrLineCmp;
	//DWORD nLineCmpSize;
	DWORD dwCiRc; CONSOLE_CURSOR_INFO ci; // GetConsoleCursorInfo
	DWORD dwConsoleCP, dwConsoleOutputCP, dwConsoleMode;
	DWORD dwSbiRc; CONSOLE_SCREEN_BUFFER_INFO sbi; // MyGetConsoleScreenBufferInfo
	//USHORT nUsedHeight; // ������, ������������ � GUI - ������ ���� ���������� gcrBufferSize.Y
	SHORT nTopVisibleLine; // ��������� � GUI ����� ���� �������������. ���� -1 - ��� ����������, ���������� ������� ��������
	SHORT nVisibleHeight;  // �� ����, ������ ���� ����� (gcrBufferSize.Y). ��� ��������������� ���������� ����� psChars & pnAttrs
	DWORD nMainTimerElapse;
	BOOL  bConsoleActive;
	HANDLE hRefreshEvent; // ServerMode, ���������� �������, � ���� ���� ��������� - �������� � GUI
	HANDLE hDataSentEvent; // ����, ��� ��������� �������� � GUI
	// ����� ������� ������� ����� RefreshThread
	BOOL bRequestChangeSize;
	USHORT nReqSizeBufferHeight;
	COORD crReqSizeNewSize;
	SMALL_RECT rReqSizeNewRect;
	LPCSTR sReqSizeLabel;
	HANDLE hReqSizeChanged;
	//
	//HANDLE hChangingSize; // FALSE �� ����� ����� ������� �������
	//CRITICAL_ SECTION csChangeSize; DWORD ncsTChangeSize;
	//MSection cChangeSize;
	HANDLE hAllowInputEvent; BOOL bInSyncResize;
	//BOOL  bNeedFullReload;  // ����� ������ ���� �������
	//BOOL  bForceFullSend; // ���������� �������� ������ ���������� �������, � �� ������ ����������
	//BOOL  bRequestPostFullReload; // �� ����� ������ ��������� ������ - ����� ��������� ��������� ����!
	//DWORD nLastUpdateTick; // ��� FORCE_REDRAW_FIX
	DWORD nLastPacketID; // �� ������ ��� �������� � GUI
	// ���� �������� ������ ���� ������... (�� ���������� ��� �����)
	//BOOL bCharChangedSet; 
	//CESERVER_CHAR CharChanged; CRITICAL_SECTION csChar;

	// ����� ��� ������� � �������
	//DWORD nChangedBufferSize;
	//CESERVER_CHAR *pChangedBuffer;

	// ����������� Output ���������� cmd...
	//
	// Keyboard layout name
	wchar_t szKeybLayout[KL_NAMELENGTH+1];

	// Optional console font (may be specified in registry)
	wchar_t szConsoleFont[LF_FACESIZE];
	//wchar_t szConsoleFontFile[MAX_PATH]; -- �� ��������
	SHORT nConFontWidth, nConFontHeight;

	// ����� ���� ��������� ���������������� ����������
	DWORD dwLastUserTick;

	// ���� ����� ������������� ���� RefreshThread
	HANDLE hLockRefreshBegin, hLockRefreshReady;

	// Console Aliases
	wchar_t* pszAliases; DWORD nAliasesSize;
} SrvInfo;

extern SrvInfo srv;

#define USER_IDLE_TIMEOUT ((DWORD)1000)
#define CHECK_IDLE_TIMEOUT 250 /* 1000 / 4 */
#define USER_ACTIVITY ((gnBufferHeight == 0) || ((GetTickCount() - srv.dwLastUserTick) <= USER_IDLE_TIMEOUT))


#pragma pack(push, 1)
extern CESERVER_CONSAVE* gpStoredOutput;
#pragma pack(pop)

typedef struct tag_CmdInfo {
    DWORD dwFarPID;
	DWORD dwSrvPID;
    BOOL  bK;
    BOOL  bNonGuiMode; // ���� ������� �� � �������, ����������� � GUI. ����� ���� ��-�� ����, ��� �������� ��� COMSPEC
    CONSOLE_SCREEN_BUFFER_INFO sbi;
    BOOL  bNewConsole;
	DWORD nExitCode;
	wchar_t szComSpecName[32];
	wchar_t szSelfName[32];
	wchar_t *pszPreAliases;
	DWORD nPreAliasSize;
} CmdInfo;

extern CmdInfo cmd;

extern COORD gcrBufferSize;
extern BOOL  gbParmBufferSize;
extern SHORT gnBufferHeight;
extern wchar_t* gpszPrevConTitle;

extern HANDLE ghLogSize;
extern wchar_t* wpszLogSizeFile;


extern BOOL gbInRecreateRoot;

//#define CES_NTVDM 0x10 -- common.hpp
//DWORD dwActiveFlags = 0;

#define CERR_GETCOMMANDLINE 100
#define CERR_CARGUMENT 101
#define CERR_CMDEXENOTFOUND 102
#define CERR_NOTENOUGHMEM1 103
#define CERR_CREATESERVERTHREAD 104
#define CERR_CREATEPROCESS 105
#define CERR_WINEVENTTHREAD 106
#define CERR_CONINFAILED 107
#define CERR_GETCONSOLEWINDOW 108
#define CERR_EXITEVENT 109
#define CERR_GLOBALUPDATE 110
#define CERR_WINHOOKNOTCREATED 111
#define CERR_CREATEINPUTTHREAD 112
#define CERR_CONOUTFAILED 113
#define CERR_PROCESSTIMEOUT 114
#define CERR_REFRESHEVENT 115
#define CERR_CREATEREFRESHTHREAD 116
#define CERR_HELPREQUESTED 118
#define CERR_ATTACHFAILED 119
#define CERR_RUNNEWCONSOLE 121
#define CERR_CANTSTARTDEBUGGER 122
#define CERR_CREATEMAPPINGERR 123
#define CERR_MAPVIEWFILEERR 124
