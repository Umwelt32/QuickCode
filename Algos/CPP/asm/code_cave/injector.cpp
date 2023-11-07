#include <iostream>
#include <cstring>
#include <windows.h>

using namespace std;

bool dll_injector_inject(DWORD pid,std::string dll_path);
bool dll_injector_create(std::string path,DWORD &pid,char *arg=nullptr);
bool dll_injector_create_inject(std::string path,std::string dll_path,char *arg=nullptr);

int main(int argc, char **argv)
{
    bool v = dll_injector_create_inject(argv[1],argv[2],nullptr);
    printf(v?"injected!\n":"error!\n");
    return 0;
}

bool dll_injector_create_inject(std::string path,std::string dll_path,char *arg)
{
    DWORD pid=0;
    bool a1=dll_injector_create(path,pid,arg);
    bool a2=dll_injector_inject(pid,dll_path);
    return (a1&&a2);
}

bool dll_injector_create(std::string path,DWORD &pid,char *arg)
{
    bool result;
    STARTUPINFO startup_info;
    PROCESS_INFORMATION process_info;
    memset(&startup_info, 0, sizeof(STARTUPINFO));
    memset(&process_info, 0, sizeof(PROCESS_INFORMATION));
    result=CreateProcess(path.c_str(),arg,NULL,NULL,FALSE,0,NULL,NULL,&startup_info,&process_info)!=0;
    pid=process_info.dwProcessId;
    return result;
}

bool dll_injector_inject(DWORD pid,std::string dll_path)
{
    HANDLE HProc;
    LPVOID LibAddr, DllAdr;
    HProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    DllAdr = (LPVOID)VirtualAllocEx(HProc, NULL, dll_path.length(), MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);
    WriteProcessMemory(HProc, (LPVOID)DllAdr, dll_path.c_str(),dll_path.length(), NULL);
    LibAddr = (LPVOID)GetProcAddress(GetModuleHandle("kernel32.dll"), "LoadLibraryA");
    CreateRemoteThread(HProc, NULL, NULL, (LPTHREAD_START_ROUTINE)LibAddr, (LPVOID)DllAdr, NULL, NULL);
    CloseHandle(HProc);
    return ((HProc!=0)&&(DllAdr!=0));
}
