#include <windows.h>
#include <DbgHelp.h>

#include "OllyPluginHeader.h"

#define PLUGINNAME     L"OllySEH"			// Unique plugin name
#define VERSION        L"1.0"				// Plugin version

HINSTANCE        hdllinst;					// Instance of plugin DLL

int osehShowCallstackTable(t_table *pt,wchar_t *name,ulong index,int mode);
int osehUpdateCallstackTable(t_table *pt,wchar_t *name,ulong index,int mode);
int osehDrawCallstackTable(wchar_t *s,uchar *mask,int *select,t_table *pt,t_drawheader *ph,int column,void *cache);

long osehTableTabFunc(t_table *pt,HWND hw,UINT msg,WPARAM wp,LPARAM lp);

DWORD CheckDLLForSafeSEH(t_module *pModule);

static t_menu mainmenu[] = {
	{ L"View saveSEH state",L"Shows if a module was compiled with /saveSEH",K_NONE,osehShowCallstackTable,NULL,0 },{NULL,NULL,K_NONE,NULL,NULL,0 }
};

typedef struct t_OSEHTable
{
	ulong       index;
	ulong       size;
	ulong       type; 

	DWORD		dwStateSEH;
	DWORD		dwBaseAddr;
	wchar_t		wDLLPath[MAX_PATH];			
} t_OSEHTable;

static t_table   OSEHTable;             // Bookmark table