#include <iostream>
#include <Windows.h>
#include <chrono>
#include <thread>
#include <Shellapi.h>

bool IsRunAsAdministrator() {
    BOOL fIsRunAsAdmin = FALSE;
    DWORD dwError = ERROR_SUCCESS;
    PSID pAdministratorsGroup = NULL;

    // Allocate and initialize a SID of the administrators group.
    SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;
    if (AllocateAndInitializeSid(&NtAuthority, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS,
        0, 0, 0, 0, 0, 0, &pAdministratorsGroup))
    {
        // Check whether the process is run as administrator.
        if (!CheckTokenMembership(NULL, pAdministratorsGroup, &fIsRunAsAdmin))
        {
            dwError = GetLastError();
        }

        // Free the SID.
        FreeSid(pAdministratorsGroup);
    }

    return fIsRunAsAdmin != 0;
}

void RestartAsAdministrator() {
    SHELLEXECUTEINFO sei = { sizeof(sei) };
    sei.lpVerb = L"runas";
    sei.lpFile = GetCommandLine(); // Use the command line that started the program
    sei.nShow = SW_NORMAL;

    if (!ShellExecuteEx(&sei)) {
        std::cerr << "Failed to restart as administrator." << std::endl;
    }
}

int main() {
    if (!IsRunAsAdministrator()) {
        std::cerr << "Detected that the program is not run as administrator. Please run the program as an administrator." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(3));
        // 继续执行程序...
    }

    std::cout << " 说明:按住“-”键以高速旋转，该程序使用“mouse_event”模拟鼠标移动，没有注入内存，理论不会封号，如果不放心，可以使用云原神. \n Written by Morax";

    while (true) {
        if (GetKeyState(VK_OEM_MINUS) < 0) {
            mouse_event(MOUSEEVENTF_MOVE, 800, 0, 0, 0);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    return 0;
}
