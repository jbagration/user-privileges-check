#include <napi.h>
#include <windows.h>
#include <lm.h>
#include <string>

#pragma comment(lib, "netapi32.lib")

std::wstring Utf8ToWstring(const std::string& str) {
    if (str.empty()) return std::wstring();
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
    std::wstring wstrTo(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
    return wstrTo;
}

std::string WstringToUtf8(const std::wstring& wstr) {
    if (wstr.empty()) return std::string();
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
    std::string strTo(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
    return strTo;
}

std::string GetAccountType(const std::wstring& userName) {
    LPLOCALGROUP_USERS_INFO_0 pBuf = nullptr;
    DWORD dwLevel = 0;
    DWORD dwFlags = LG_INCLUDE_INDIRECT;
    DWORD dwPrefMaxLen = MAX_PREFERRED_LENGTH;
    DWORD dwEntriesRead = 0;
    DWORD dwTotalEntries = 0;
    NET_API_STATUS nStatus;

    nStatus = NetUserGetLocalGroups(
        NULL,
        userName.c_str(),
        dwLevel,
        dwFlags,
        (LPBYTE*)&pBuf,
        dwPrefMaxLen,
        &dwEntriesRead,
        &dwTotalEntries
    );

    if (nStatus == NERR_Success) {
        for (DWORD i = 0; i < dwEntriesRead; i++) {
            if (wcscmp(pBuf[i].lgrui0_name, L"Administrators") == 0) {
                NetApiBufferFree(pBuf);
                return "Администратор";
            }
            else if (wcscmp(pBuf[i].lgrui0_name, L"Users") == 0) {
                NetApiBufferFree(pBuf);
                return "Пользователь";
            }
            else if (wcscmp(pBuf[i].lgrui0_name, L"Guests") == 0) {
                NetApiBufferFree(pBuf);
                return "Гость";
            }
        }
        NetApiBufferFree(pBuf);
        return "Пользователь";
    }

    return "Пользователь не найден";
}

Napi::String CheckUserPrivileges(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    if (info.Length() < 1 || !info[0].IsString()) {
        Napi::TypeError::New(env, "String expected").ThrowAsJavaScriptException();
    }

    std::string userName = info[0].As<Napi::String>().Utf8Value();
    std::wstring wideUserName = Utf8ToWstring(userName);
    std::string result = GetAccountType(wideUserName);

    return Napi::String::New(env, result);
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
    exports.Set(Napi::String::New(env, "checkUserPrivileges"),
        Napi::Function::New(env, CheckUserPrivileges));
    return exports;
}

NODE_API_MODULE(addon, Init)
