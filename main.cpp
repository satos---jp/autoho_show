#include <Windows.h>
#include <iostream>
#include <cstdio>
#include <string>
#include <tlhelp32.h>

using namespace std;


//誕生日の。は、開始位置がランダムだった。

char buf[512];
char libpath[512]; // = "C:\\MinGW\\msys\\1.0\\home\\akihiro\\autoho\\autoho\\AIbase_all\\dlltest.dll";
char dllname[15] = "dlltest.dll";

int main(){
	
	{
		GetCurrentDirectory(MAX_PATH,libpath);
		int ls=strlen(libpath);
		libpath[ls] = '\\'; ls++; libpath[ls] = 0;
		
		int ldn = strlen(dllname);
		for(int i=0;i<=ldn;i++){
			libpath[i+ls] = dllname[i];
		}
		//puts(libpath);
		//return 0;
	}
	
	
	DWORD pid = 0;
	
	pid = 0;
	PROCESSENTRY32 pe;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	//printf("hSnap .. %X\n",hSnap);
	BOOL procuc = Process32First(hSnap, &pe);
	do{
		//printf("id .. %d name .. %s\n",pe.th32ProcessID,pe.szExeFile);
		//if(!strcmp("th095.exe", pe.szExeFile)){
		if(!strcmp("x.exe", pe.szExeFile)){
			pid = pe.th32ProcessID;
			break;
		}
		//cout << pe.szExeFile << endl;
	}while(Process32Next(hSnap, &pe));
	
	if(!pid){
		cout << "東方文花帖を起動してください" << endl;
		
		return 0;
	}
	
	sprintf(buf, "[*]pid: %d", pid);
	printf("%s\n",buf);

	HANDLE proc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	
	
	HMODULE hm = GetModuleHandle("kernel32");
	LPTHREAD_START_ROUTINE p_LoadLibrary = (LPTHREAD_START_ROUTINE)GetProcAddress(hm, "LoadLibraryA");
	printf("p_LoadLibrary:%X\n", p_LoadLibrary);	
	printf("%X\n",LPTHREAD_START_ROUTINE(LoadLibraryA));

	LPTHREAD_START_ROUTINE p_FreeLibrary = (LPTHREAD_START_ROUTINE)GetProcAddress(hm, "FreeLibrary");
	printf("p_FreeLibrary:%X\n", p_FreeLibrary);
	printf("%X\n",LPTHREAD_START_ROUTINE(FreeLibrary));
	

	
	int pathSize = strlen(libpath) + 1;
	LPVOID remoteLibPath = VirtualAllocEx(proc, NULL, pathSize, MEM_COMMIT, PAGE_READWRITE);
	printf("libpath: %p\n",remoteLibPath);
	WriteProcessMemory(proc, remoteLibPath, libpath, pathSize, NULL);
	

	char option[256];
	HANDLE hThread;
	
	while(1){
		scanf("%s", option);
		if(!strcmp(option, "load")){
			cout << "[*] Loading DLL" << endl;
			OutputDebugString("[*] Loading DLL");
			//MessageBox(NULL , TEXT("inject") ,TEXT("trytoinject") , MB_OK);
			hThread = CreateRemoteThread(proc, NULL, 0, p_LoadLibrary, remoteLibPath, 0, NULL);
			//hThread = CreateRemoteThread(proc, NULL, 0, LPTHREAD_START_ROUTINE(0x40733A), remoteLibPath, 0, NULL);
			if( !hThread )printf("miss\n");
			//hThread = CreateRemoteThread(proc, NULL, 0, LPTHREAD_START_ROUTINE(LoadLibraryA), remoteLibPath, 0, NULL);
			
			printf("%d\n",hThread);

		}else if(!strcmp(option, "unload")){
			//MessageBox(NULL,"unloading","unloading",MB_OK);
			cout << "[*] Unloading DLL" << endl;
			OutputDebugString("[*] Unloading DLL");
			DWORD exCode;
			GetExitCodeThread(hThread, &exCode);
			CreateRemoteThread(proc, NULL, 0, p_FreeLibrary, (LPDWORD)exCode, 0, NULL);
			//CreateRemoteThread(proc, NULL, 0, LPTHREAD_START_ROUTINE(FreeLibrary), (LPDWORD)exCode, 0, NULL);
			break;
		}
	}
	return 0;
}