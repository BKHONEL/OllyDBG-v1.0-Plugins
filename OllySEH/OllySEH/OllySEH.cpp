#include "OllySEH.h"

BOOL WINAPI DllEntryPoint(HINSTANCE hi,DWORD dwReason,LPVOID lpReserved) {
	if (dwReason == DLL_PROCESS_ATTACH)
		hdllinst = hi;
	return 1;
}

extc t_menu * __cdecl ODBG2_Pluginmenu(wchar_t *type)
{
	if (wcscmp(type,PWM_MAIN) == 0)
		return mainmenu;
	return NULL;
}

extc int __cdecl ODBG2_Pluginquery(int ollydbgversion,ulong *features,wchar_t pluginname[SHORTNAME],wchar_t pluginversion[SHORTNAME])
{
	if (ollydbgversion < 201)
		return 0;
	StrcopyW(pluginname,SHORTNAME,PLUGINNAME);
	StrcopyW(pluginversion,SHORTNAME,VERSION);
	return PLUGIN_VERSION;
}

extc int __cdecl ODBG2_Plugininit(void)
{
	if (Createsorteddata(&OSEHTable.sorted,sizeof(OSEHTable),1,NULL,NULL,NULL) != 0)
		return -1;

	StrcopyW(OSEHTable.name,SHORTNAME,PLUGINNAME);
	OSEHTable.mode = TABLE_SAVEALL;
	OSEHTable.bar.visible = 1;
	OSEHTable.bar.name[0] = L"SEH Mode";
	OSEHTable.bar.expl[0] = L"";
	OSEHTable.bar.mode[0] = BAR_FLAT;
	OSEHTable.bar.defdx[0] = 15;
	OSEHTable.bar.name[1] = L"Base";
	OSEHTable.bar.expl[1] = L"";
	OSEHTable.bar.mode[1] = BAR_FLAT;
	OSEHTable.bar.defdx[1] = 30;
	OSEHTable.bar.name[2] = L"Module Name";
	OSEHTable.bar.expl[2] = L"";
	OSEHTable.bar.mode[2] = BAR_FLAT;
	OSEHTable.bar.defdx[2] = TEXTLEN;
	OSEHTable.bar.nbar = 3;
	OSEHTable.custommode = 0;
	OSEHTable.customdata = NULL;
	OSEHTable.updatefunc = NULL;
	OSEHTable.tabfunc = (TABFUNC*)osehTableTabFunc;
	OSEHTable.drawfunc = (DRAWFUNC*)osehDrawCallstackTable;
	OSEHTable.tableselfunc = NULL;
	OSEHTable.menu = NULL;

	return 0;
}

extc void __cdecl ODBG2_Pluginreset(void)
{
	Deletesorteddatarange(&OSEHTable.sorted,0,0xFFFFFFFF);
}

extc void __cdecl ODBG2_Plugindestroy(void) 
{
	Destroysorteddata(&OSEHTable.sorted);
}

int osehUpdateCallstackTable(t_table *pt,wchar_t *name,ulong index,int mode) 
{
	t_run runState = run;
	int iIndexCounter = NULL;

	Deletesorteddatarange(&OSEHTable.sorted,0,0xFFFFFFFF);

	const t_sorted *sortedModules = &module.sorted;
	t_module *pModule = (t_module*)sortedModules->data;

	for(int i = 0; i < sortedModules->n; i++,pModule++)
	{
		t_OSEHTable newData;
		newData.index = iIndexCounter++;
		newData.size = 1;
		newData.type = 0;
		newData.dwStateSEH = CheckDLLForSafeSEH(pModule);
		newData.dwBaseAddr = pModule->base;
		wcscpy(newData.wDLLPath,pModule->path);
		Addsorteddata(&OSEHTable.sorted,&newData);
	}

	return MENU_REDRAW;
}

int osehShowCallstackTable(t_table *pt,wchar_t *name,ulong index,int mode)
{
	if (mode == MENU_VERIFY)
		return MENU_NORMAL;
	else if (mode == MENU_EXECUTE)
	{
		if (OSEHTable.hw == NULL)
			Createtablewindow(&OSEHTable,0,OSEHTable.bar.nbar,NULL,L"ICO_P",PLUGINNAME);
		else
			Activatetablewindow(&OSEHTable);

		return osehUpdateCallstackTable(pt,name,index,mode);
	}
	return MENU_ABSENT;
}

int osehDrawCallstackTable(wchar_t *s,uchar *mask,int *select,t_table *pt,t_drawheader *ph,int column,void *cache) 
{
	DWORD	dwStrLen = NULL;
	t_OSEHTable *pmark = (t_OSEHTable*)ph;

	switch (column)
	{
	case 0:	// SEHMode
		{
			PTCHAR wDllPath;
			switch(pmark->dwStateSEH)
			{
			case 0:
				*select |= DRAW_GRAY;
				wDllPath = L"/safeSEH NO";
				break;
			case 1:
				wDllPath = L"/safeSEH YES";
				break;
			case 2:
				wDllPath = L"no SEH";
				break;
			default:
				wDllPath = L"ERROR";
				break;
			}
			dwStrLen = Swprintf(s,L"%s",wDllPath);
		}
		break;
	case 1: // DLL BaseAddr
		dwStrLen = Simpleaddress(s,pmark->dwBaseAddr,mask,select);
		break;
	case 2: // DLLPath
		dwStrLen = Swprintf(s,L"%s",pmark->wDLLPath);
		break;
	default:
		break;
	}
	return dwStrLen;
}

long osehTableTabFunc(t_table *pt,HWND hw,UINT msg,WPARAM wp,LPARAM lp)
{
	t_OSEHTable *pTableData;
	switch (msg)
	{
	case WM_USER_DBLCLK:
		pTableData = (t_OSEHTable*)Getsortedbyselection(&pt->sorted,pt->sorted.selected);
		if (pTableData != NULL)
			Setcpu(0,pTableData->dwBaseAddr,0,0,0,CPU_ASMHIST|CPU_ASMCENTER|CPU_ASMFOCUS);
		return 1;
	default: 
		break;
	}
	return 0;
}

DWORD CheckDLLForSafeSEH(t_module *pModule)
{
	LPVOID lpPEHeaderMem = malloc(pModule->memhdrsize);
	if(lpPEHeaderMem == NULL)
		return -1;

	if(!Readmemory(lpPEHeaderMem,pModule->base,pModule->memhdrsize,MM_SILENT))
	{
		free(lpPEHeaderMem);
		return -1;
	}

	PIMAGE_DOS_HEADER pIDH = (PIMAGE_DOS_HEADER)lpPEHeaderMem;
	PIMAGE_NT_HEADERS pINH = (PIMAGE_NT_HEADERS)((DWORD)lpPEHeaderMem + pIDH->e_lfanew);

	if(pIDH->e_magic != IMAGE_DOS_SIGNATURE || pINH->Signature != IMAGE_NT_SIGNATURE)
	{
		free(lpPEHeaderMem);
		return -1;
	}

	if(pINH->OptionalHeader.DllCharacteristics & IMAGE_DLLCHARACTERISTICS_NO_SEH)
	{
		free(lpPEHeaderMem);
		return 2;
	}
	else
	{
		PIMAGE_DATA_DIRECTORY pIDEC = (PIMAGE_DATA_DIRECTORY)&pINH->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG];
		PIMAGE_LOAD_CONFIG_DIRECTORY pILCD = (PIMAGE_LOAD_CONFIG_DIRECTORY)malloc(sizeof(IMAGE_LOAD_CONFIG_DIRECTORY));

		if(pIDEC->Size > 0 && Readmemory(pILCD,pModule->base + pIDEC->VirtualAddress,sizeof(IMAGE_LOAD_CONFIG_DIRECTORY),MM_SILENT))
		{
			if(pILCD->SEHandlerTable)
			{
				free(lpPEHeaderMem);
				free(pILCD);
				return 1;
			}
		}

		free(pILCD);
		free(lpPEHeaderMem);
		return 0;
	}
}