#include "hardwareinfo.h"
#include <QtGlobal>
#ifdef 	Q_OS_WIN32

#include <wbemidl.h>
#include <Wbemcli.h>
#pragma comment(lib,"Wbemuuid.lib")
#include <objbase.h>
#include <comdef.h>
#include <tchar.h>
#include <strsafe.h>
//#include "debug_new.h"
#pragma comment(lib, "User32.lib")
#pragma comment(lib, "ole32.lib")

#ifndef PRODUCT_PROFESSIONAL
#define PRODUCT_PROFESSIONAL  0x00000030
#endif
#ifndef VER_SUITE_WH_SERVER
#define VER_SUITE_WH_SERVER  0x00008000
#endif
#define BUFSIZE 1024

typedef void (WINAPI *PGNSI)(LPSYSTEM_INFO);
typedef BOOL (WINAPI *PGPI)(DWORD, DWORD, DWORD, DWORD, PDWORD);
typedef int (WINAPI *PGetSystemDefaultLocaleName)(LPWSTR lpLocaleName, int cchLocaleName);
typedef int (WINAPI *PGetUserDefaultLocaleName)(LPWSTR lpLocaleName, int cchLocaleName);



void GetLocaleNameLCID(LCID lcid, char* out_name);

char* TrimSimpleChart(char* s)
{
    if (!s || !strcmp(s, ""))
    {
        return s;
    }

    int slen = strlen(s);
    char* ret = new char[slen + 1];
    if (!ret)
    {
        return 0;
    }

    char* head = ret;
    memset(ret, 0, slen + 1);
    while(*s!=0)
    {
        if (*s == ' ' || *s == '\t')
        {
            *s++;
            continue;
        }
        else
        {
            *ret++ = *s++;
        }
    }
    return head;
}

BOOL GetOSDisplayStr( LPSTR pszOS)
{
    OSVERSIONINFOEXA osvi;
    SYSTEM_INFO si;
    PGNSI pGNSI;
    PGPI pGPI;
    BOOL bOsVersionInfoEx;
    DWORD dwType;

    ZeroMemory(&si, sizeof(SYSTEM_INFO));
    ZeroMemory(&osvi, sizeof(OSVERSIONINFOEXA));

    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEXA);

    if( !(bOsVersionInfoEx = GetVersionExA ((OSVERSIONINFOA *) &osvi)) )
        return 1;

    // Call GetNativeSystemInfo if supported or GetSystemInfo otherwise.

    pGNSI = (PGNSI) GetProcAddress(
        GetModuleHandleA("kernel32.dll"),
        "GetNativeSystemInfo");
    if(NULL != pGNSI)
        pGNSI(&si);
    else GetSystemInfo(&si);

    if ( VER_PLATFORM_WIN32_NT==osvi.dwPlatformId &&
        osvi.dwMajorVersion > 4 )
    {
        StringCchCopyA(pszOS, BUFSIZE, "Microsoft ");

        // Test for the specific product.

        if ( osvi.dwMajorVersion == 6 )
        {
            if( osvi.dwMinorVersion == 0 )
            {
                if( osvi.wProductType == VER_NT_WORKSTATION )
                    strcat_s(pszOS, BUFSIZE, "Windows Vista ");
                else strcat_s(pszOS, BUFSIZE, "Windows Server 2008 ");
            }

            if ( osvi.dwMinorVersion == 1 )
            {
                if( osvi.wProductType == VER_NT_WORKSTATION )
                    strcat_s(pszOS, BUFSIZE, "Windows 7 ");
                else strcat_s(pszOS, BUFSIZE, "Windows Server 2008 R2 ");
            }

            if ( osvi.dwMinorVersion == 2 )
            {
                if( osvi.wProductType == VER_NT_WORKSTATION )
                    strcat_s(pszOS, BUFSIZE, "Windows 8 ");
                else strcat_s(pszOS, BUFSIZE, "Windows 8 ");
            }

            pGPI = (PGPI) GetProcAddress(
                GetModuleHandleA("kernel32.dll"),
                "GetProductInfo");

            pGPI( osvi.dwMajorVersion, osvi.dwMinorVersion, 0, 0, &dwType);

            switch( dwType )
            {
            case PRODUCT_ULTIMATE:
                strcat_s(pszOS, BUFSIZE, "Ultimate Edition");
                break;
#ifdef PRODUCT_PROFESSIONAL  //win 6.0SDK没有这个定义
            case PRODUCT_PROFESSIONAL:
                strcat_s(pszOS, BUFSIZE, "Professional");
                break;
#endif
            case PRODUCT_HOME_PREMIUM:
                strcat_s(pszOS, BUFSIZE, "Home Premium Edition");
                break;
            case PRODUCT_HOME_BASIC:
                strcat_s(pszOS, BUFSIZE, "Home Basic Edition");
                break;
            case PRODUCT_ENTERPRISE:
                strcat_s(pszOS, BUFSIZE, "Enterprise Edition");
                break;
            case PRODUCT_BUSINESS:
                strcat_s(pszOS, BUFSIZE, "Business Edition");
                break;
            case PRODUCT_STARTER:
                strcat_s(pszOS, BUFSIZE, "Starter Edition");
                break;
            case PRODUCT_CLUSTER_SERVER:
                strcat_s(pszOS, BUFSIZE, "Cluster Server Edition");
                break;
            case PRODUCT_DATACENTER_SERVER:
                strcat_s(pszOS, BUFSIZE, "Datacenter Edition");
                break;
            case PRODUCT_DATACENTER_SERVER_CORE:
                strcat_s(pszOS, BUFSIZE, "Datacenter Edition (core installation)");
                break;
            case PRODUCT_ENTERPRISE_SERVER:
                strcat_s(pszOS, BUFSIZE, "Enterprise Edition");
                break;
            case PRODUCT_ENTERPRISE_SERVER_CORE:
                strcat_s(pszOS, BUFSIZE, "Enterprise Edition (core installation)");
                break;
            case PRODUCT_ENTERPRISE_SERVER_IA64:
                strcat_s(pszOS, BUFSIZE, "Enterprise Edition for Itanium-based Systems");
                break;
            case PRODUCT_SMALLBUSINESS_SERVER:
                strcat_s(pszOS, BUFSIZE, "Small Business Server");
                break;
            case PRODUCT_SMALLBUSINESS_SERVER_PREMIUM:
                strcat_s(pszOS, BUFSIZE, "Small Business Server Premium Edition");
                break;
            case PRODUCT_STANDARD_SERVER:
                strcat_s(pszOS, BUFSIZE, "Standard Edition");
                break;
            case PRODUCT_STANDARD_SERVER_CORE:
                strcat_s(pszOS, BUFSIZE, "Standard Edition (core installation)");
                break;
            case PRODUCT_WEB_SERVER:
                strcat_s(pszOS, BUFSIZE, "Web Server Edition");
                break;
            case 72:
                strcat_s(pszOS, BUFSIZE, "Enterprise");
                break;
            default:
                {
                    char sztmp[10] = {0};
                    itoa(dwType, sztmp, 10);
                    strcat_s(pszOS, BUFSIZE, sztmp);
                }
                break;
            }
        }

        if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2 )
        {
            if( GetSystemMetrics(SM_SERVERR2) )
                strcat_s(pszOS, BUFSIZE,  "Windows Server 2003 R2, ");
            else if ( osvi.wSuiteMask & VER_SUITE_STORAGE_SERVER )
                strcat_s(pszOS, BUFSIZE,  "Windows Storage Server 2003");
#ifdef VER_SUITE_WH_SERVER  //win 6.0SDK没有这个定义
            else if ( osvi.wSuiteMask & VER_SUITE_WH_SERVER )
                strcat_s(pszOS, BUFSIZE,  "Windows Home Server");
#endif
            else if( osvi.wProductType == VER_NT_WORKSTATION &&
                si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_AMD64)
            {
                strcat_s(pszOS, BUFSIZE,  "Windows XP Professional x64 Edition");
            }
            else strcat_s(pszOS, BUFSIZE, "Windows Server 2003, ");

            // Test for the server type.
            if ( osvi.wProductType != VER_NT_WORKSTATION )
            {
                if ( si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_IA64 )
                {
                    if( osvi.wSuiteMask & VER_SUITE_DATACENTER )
                        strcat_s(pszOS, BUFSIZE, "Datacenter Edition for Itanium-based Systems");
                    else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
                        strcat_s(pszOS, BUFSIZE,  "Enterprise Edition for Itanium-based Systems");
                }

                else if ( si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_AMD64 )
                {
                    if( osvi.wSuiteMask & VER_SUITE_DATACENTER )
                        strcat_s(pszOS, BUFSIZE,  "Datacenter x64 Edition" );
                    else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
                        strcat_s(pszOS, BUFSIZE,  "Enterprise x64 Edition" );
                    else strcat_s(pszOS, BUFSIZE,  "Standard x64 Edition");
                }

                else
                {
                    if ( osvi.wSuiteMask & VER_SUITE_COMPUTE_SERVER )
                        strcat_s(pszOS, BUFSIZE,  "Compute Cluster Edition" );
                    else if( osvi.wSuiteMask & VER_SUITE_DATACENTER )
                        strcat_s(pszOS, BUFSIZE,  "Datacenter Edition" );
                    else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
                        strcat_s(pszOS, BUFSIZE,  "Enterprise Edition" );
                    else if ( osvi.wSuiteMask & VER_SUITE_BLADE )
                        strcat_s(pszOS, BUFSIZE,  "Web Edition" );
                    else strcat_s(pszOS, BUFSIZE,  "Standard Edition" );
                }
            }
        }

        if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1 )
        {
            strcat_s(pszOS, BUFSIZE, "Windows XP ");
            if( osvi.wSuiteMask & VER_SUITE_PERSONAL )
                strcat_s(pszOS, BUFSIZE,  "Home Edition" );
            else strcat_s(pszOS, BUFSIZE,  "Professional" );
        }

        if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0 )
        {
            strcat_s(pszOS, BUFSIZE, "Windows 2000 ");

            if ( osvi.wProductType == VER_NT_WORKSTATION )
            {
                strcat_s(pszOS, BUFSIZE,  "Professional" );
            }
            else
            {
                if( osvi.wSuiteMask & VER_SUITE_DATACENTER )
                    strcat_s(pszOS, BUFSIZE,  "Datacenter Server" );
                else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
                    strcat_s(pszOS, BUFSIZE,  "Advanced Server" );
                else strcat_s(pszOS, BUFSIZE,  "Server" );
            }
        }

        // Include service pack (if any) and build number.

        if( /*_tcslen*/strlen(osvi.szCSDVersion) > 0 )
        {
            strcat_s(pszOS, BUFSIZE, " " );
            strcat_s(pszOS, BUFSIZE, osvi.szCSDVersion);
        }

        char buf[80];

        StringCchPrintfA( buf, 80, " (build %d)", osvi.dwBuildNumber);
        strcat_s(pszOS, BUFSIZE, buf);

        if ( si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_AMD64 )
            strcat_s(pszOS, BUFSIZE,  ", 64-bit");
        else if (si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_INTEL )
            strcat_s(pszOS, BUFSIZE, ", 32-bit");
        else
        {
            strcat_s(pszOS, BUFSIZE, ", unknow-bit");
        }

        if ( osvi.dwMajorVersion >= 6 )
        {
            wchar_t lcname[50] = {0};
            // 获取文本格式, 如果修改了文本格式, 那么通过API是获取不到原来操作系统是什么语言的
            PGetUserDefaultLocaleName pUser = (PGetUserDefaultLocaleName)GetProcAddress(GetModuleHandleA("kernel32.dll"),
                "GetUserDefaultLocaleName");
            if (pUser)
            {
                memset(lcname, 0, sizeof(lcname));
                pUser(lcname, sizeof(lcname));
                size_t len = wcslen(lcname) + 1;
                size_t converted = 0;
                char *CStr =(char*)malloc(len*sizeof(char));
                wcstombs_s(&converted, CStr, len, lcname, _TRUNCATE);
                strcat_s(pszOS, BUFSIZE,  ", " );
                strcat_s(pszOS, BUFSIZE, CStr);
                delete[] CStr;
            }

            // 获取区域语言, 很多用户会改区域语言, 但是不会修改文本格式
            PGetSystemDefaultLocaleName pSys = (PGetSystemDefaultLocaleName)GetProcAddress(GetModuleHandle(TEXT("kernel32.dll")),
                "GetSystemDefaultLocaleName");
            if (pSys)
            {
                memset(lcname, 0, sizeof(lcname));
                pSys(lcname, sizeof(lcname));
                size_t len = wcslen(lcname) + 1;
                size_t converted = 0;
                char *CStr = (char*)malloc(len*sizeof(char));
                wcstombs_s(&converted, CStr, len, lcname, _TRUNCATE);
                strcat_s(pszOS, BUFSIZE, ", ");
                strcat_s(pszOS, BUFSIZE, CStr);
                delete[] CStr;
            }
        }
        else
        {
            char lcname[50] = {0};
            LCID  userLCID = GetUserDefaultLCID();
            GetLocaleNameLCID(userLCID, lcname);
            strcat_s(pszOS, BUFSIZE, ", ");
            strcat_s(pszOS, BUFSIZE, lcname);

            LCID  sysLCID = GetSystemDefaultLCID();
            GetLocaleNameLCID(sysLCID, lcname);
            strcat_s(pszOS, BUFSIZE, ", ");
            strcat_s(pszOS, BUFSIZE, lcname);
        }
        //MessageBoxA(0, pszOS, "tip", 0);
        return TRUE;
    }
    else
    {
        strcpy_s(pszOS, BUFSIZE, "Do not know this version of Windows.");
        return FALSE;
    }
}

void GetLocaleNameLCID(LCID lcid, char* out_name)
{
    const char *prim=NULL, *subl=NULL;
    switch(lcid)
    {
    case 0x0c09:
        prim = "en";subl="AU";break;
    case 0x2809:
        prim = "en";subl="BZ";break;
    case 0x1009:
        prim = "en";subl="CA";break;
    case 0x2409:
        prim = "en";subl="029";break;
    case 0x4009:
        prim = "en";subl="IN";break;
    case 0x1809:
        prim = "en";subl="IE";break;
    case 0x2009:
        prim = "en";subl="JM";break;
    case 0x4409:
        prim = "en";subl="MY";break;
    case 0x1409:
        prim = "en";subl="NZ";break;
    case 0x3409:
        prim = "en";subl="PH";break;
    case 0x4809:
        prim = "en";subl="SG";break;
    case 0x1c09:
        prim = "en";subl="ZA";break;
    case 0x2c09:
        prim = "en";subl="TT";break;
    case 0x0809:
        prim = "en";subl="GB";break;
    case 0x0409:
        prim = "en";subl="US";break;
    case 0x3009:
        prim = "en";subl="ZW";break;

    case 0x0411:  //Japanese (Japan)
        prim = "ja";subl="JP";break;

    case 0x2c0a:// Spanish (Argentina) es-AR Latn 1252
        prim = "es";subl="AR";break;
    case 0x400a:// Spanish (Bolivia) es-BO Latn 1252
        prim = "es";subl="BO";break;
    case 0x340a:// Spanish (Chile) es-CL Latn 1252
        prim = "es";subl="CL";break;
    case 0x240a:// Spanish (Colombia) es-CO Latn 1252
        prim = "es";subl="CO";break;
    case 0x140a:// Spanish (Costa Rica) es-CR Latn 1252
        prim = "es";subl="CR";break;
    case 0x1c0a:// Spanish (Dominican Republic) es-DO Latn 1252
        prim = "es";subl="DO";break;
    case 0x300a:// Spanish (Ecuador) es-EC Latn 1252
        prim = "es";subl="EC";break;
    case 0x440a:// Spanish (El Salvador) es-SV Latn 1252
        prim = "es";subl="SV";break;
    case 0x100a:// Spanish (Guatemala) es-GT Latn 1252
        prim = "es";subl="GT";break;
    case 0x480a:// Spanish (Honduras) es-HN Latn 1252
        prim = "es";subl="HN";break;
    case 0x080a:// Spanish (Mexico) es-MX Latn 1252
        prim = "es";subl="MX";break;
    case 0x4c0a:// Spanish (Nicaragua) es-NI Latn 1252
        prim = "es";subl="NI";break;
    case 0x180a:// Spanish (Panama) es-PA Latn 1252
        prim = "es";subl="PA";break;
    case 0x3c0a:// Spanish (Paraguay) es-PY Latn 1252
        prim = "es";subl="PY";break;
    case 0x280a:// Spanish (Peru) es-PE Latn 1252
        prim = "es";subl="PE";break;
    case 0x500a:// Spanish (Puerto Rico) es-PR Latn 1252
        prim = "es";subl="PR";break;
    case 0x0c0a:// Spanish (Spain) es-ES Latn 1252
        prim = "es";subl="ES";break;
    case 0x040a:// Spanish (Spain, Traditional Sort) es-ES_tradnl Latn 1252
        prim = "es";subl="ES_tradnl";break;
    case 0x540a:// Windows Vista and later: Spanish (United States) es-US
        prim = "es";subl="US";break;
    case 0x380a:// Spanish (Uruguay) es-UY Latn 1252
        prim = "es";subl="UY";break;
    case 0x200a:// Spanish (Venezuela) es-VE Latn 1252
        prim = "es";subl="VE";break;

    case 0x080c:// French (Belgium) fr-BE Latn 1252
        prim = "fr";subl="BE";break;
    case 0x0c0c:// French (Canada) fr-CA Latn 1252
        prim = "fr";subl="CA";break;
    case 0x040c:// French (France) fr-FR Latn 1252
        prim = "fr";subl="FR";break;
    case 0x140c:// French (Luxembourg) fr-LU Latn 1252
        prim = "fr";subl="LU";break;
    case 0x180c:// French (Monaco) fr-MC Latn 1252
        prim = "fr";subl="MC";break;
    case 0x100c:// French (Switzerland) fr-CH Latn 1252
        prim = "fr";subl="CH";break;

    case 0x0410://  Italian (Italy) it-IT Latn 1252
        prim = "it";subl="IT";break;
    case 0x0810://  Italian (Switzerland) it-CH Latn 1252
        prim = "it";subl="CH";break;

    case 0x082e:// Windows Vista and later: Lower Sorbian (Germany) dsb-DE
        prim = "dsb";subl="DE";break;

    case 0x0c04://  Chinese (Hong Kong SAR, PRC) zh-HK Hant 950
        prim = "zh";subl="HK";break;
    case 0x1404://  Windows 98/Me, Windows XP and later: Chinese (Macao SAR) zh-MO Hant 950
        prim = "zh";subl="MO";break;
    case 0x0804://  Chinese (PRC) zh-CN Hans 936
        prim = "zh";subl="CN";break;
    case 0x1004://  Chinese (Singapore) zh-SG Hans 936
        prim = "zh";subl="SG";break;
    case 0x0404://  Chinese (Taiwan) zh-TW Hant 950
        prim = "zh";subl="TW";break;

    case 0x0419:// Windows Vista and later: Lower Sorbian (Germany) dsb-DE
        prim = "ru";subl="RU";break;

    case 0x0416:// Portuguese (Brazil) pt-BR Latn 1252
        prim = "pt";subl="BR";break;
    case 0x0816:// Portuguese (Portugal) pt-PT Latn 1252
        prim = "pt";subl="PT";break;

    case 0x0412://Korean (Korea) ko-KR
        prim = "ko";subl="KR";break;

    case 0x0813:// Dutch (Belgium) nl-BE Latn 1252
        prim = "nl";subl="BE";break;
    case 0x0413:// Dutch (Netherlands) nl-NL Latn 1252
        prim = "nl";subl="NL";break;

    case 0x0406:// Danish (Denmark) da-DK Latn 1252
        prim = "da";subl="DK";break;

    case 0x040b:// Finnish (Finland) fi-FI Latn 1252
        prim = "fi";subl="FI";break;

    case 0x0414://  Norwegian (Bokm?l, Norway) nb-NO Latn 1252
        prim = "nb";subl="NO";break;
    case 0x0814://  Norwegian (Nynorsk, Norway) nn-NO Latn 1252
        prim = "nn";subl="NO";break;

    case 0x081d:// Swedish (Finland) sv-FI Latn 1252
        prim = "sv";subl="FI";break;
    case 0x041d:// Swedish (Sweden) sv-SE Latn 1252
        prim = "sv";subl="SE";break;

    case 0x0415:// Polish (Poland) pl-PL Latn 1250
        prim = "pl";subl="PL";break;

    case 0x0408:// Greek (Greece) el-GR Grek 1253
        prim = "el";subl="GR";break;

    case 0x041f:// Turkish (Turkey) tr-TR Latn 1254
        prim = "tr";subl="TR";break;

    case 0x040e:// Hungarian (Hungary) hu-HU Latn 1250
        prim = "hu";subl="HU";break;

    default:
        prim = "en"; subl="";break;
    }

    if (out_name)
    {
        sprintf(out_name, "%s-%s", prim, subl);
    }
}


#endif

HardwareInfo::HardwareInfo()
{
    MakeHardwareInfoString(false);
}

QString HardwareInfo::getCpuID() const
{
    return QString(qbCpuID);
}

QString HardwareInfo::getBiosID() const
{
    return QString(qbBiosID);
}

QString HardwareInfo::getDiskID() const
{
    return QString(qbDiskID);
}

QString HardwareInfo::getMacAdd() const
{
    return QString(qbMacAdd);
}

bool HardwareInfo::MakeHardwareInfoString(bool bThread)
{
#ifdef 	Q_OS_WIN32
    char szInfoBuf[BUFSIZE];
    HRESULT hres;
    char szOSVer[BUFSIZE];

    /// 操作系统版本号
    GetOSDisplayStr(szOSVer);
    qbOSVer = szOSVer;

    //步骤1：不是必须的，COM只须也只能初始化一次
    //每线程需要调一次.QT主线程不用,但创建出来的需要
    //fix by hongcs 2018.11.23 子线程才需要初始化
    if (bThread)
    {
        hres = CoInitializeEx(0, COINIT_MULTITHREADED );
        //hres = CoInitialize(0);
        if (FAILED(hres))  //出错点:在QT线程中,出现过初始化就死的情况.不能调用CoInitializeEx
        {       //在新线程中需要正常调用CoInitializeEx
            return false; //初始化COM异常：注意，COM只须也只能初始化一次
        }
    }
    //步骤2：不是必须的，COM只须也只能设置一次
    //Set general COM security levels
    hres = CoInitializeSecurity(
        NULL,
        -1, // COM authentication
        NULL, // Authentication services
        NULL, // Reserved
        RPC_C_AUTHN_LEVEL_PKT, // Default authentication
        RPC_C_IMP_LEVEL_IMPERSONATE, // Default Impersonation
        NULL, // Authentication info
        EOAC_NONE, // Additional capabilities
        NULL // Reserved
        );

    if (FAILED(hres) && RPC_E_TOO_LATE!=hres)
    {
        CoUninitialize();
        return false; // Program has failed.
    }

    //以上不是必须的，若已有“::COMInit();”，则要跳过
    //步骤3： Obtain the initial locator to WMI
    IWbemLocator *pLoc = NULL;
    hres = CoCreateInstance(
        CLSID_WbemLocator,
        0,
        CLSCTX_INPROC_SERVER,
        IID_IWbemLocator, (LPVOID *) &pLoc);

    if (FAILED(hres))
    {
        CoUninitialize();
        return false;//Failed to create IWbemLocator object
    }

    //步骤4：Connect to WMI through the IWbemLocator::ConnectServer method
    IWbemServices *pSvc = NULL;
    hres = pLoc->ConnectServer(
        _bstr_t(L"ROOT\\CIMV2"), // Object path of WMI namespace
        NULL, // User name. NULL = current user
        NULL, // User password. NULL = current
        0, // Locale. NULL indicates current
        NULL, // Security flags.
        0, // Authority (e.g. Kerberos)
        0, // Context object
        &pSvc // pointer to IWbemServices proxy
        );

    if (FAILED(hres))
    {
        pLoc->Release();
        CoUninitialize();
        return false; // Program has failed.
    }
    // 步骤5： Set security levels on the proxy
    hres = CoSetProxyBlanket(
        pSvc, // Indicates the proxy to set
        RPC_C_AUTHN_WINNT, // RPC_C_AUTHN_xxx
        RPC_C_AUTHZ_NONE, // RPC_C_AUTHZ_xxx
        NULL, // Server principal name
        RPC_C_AUTHN_LEVEL_CALL, // RPC_C_AUTHN_LEVEL_xxx
        RPC_C_IMP_LEVEL_IMPERSONATE, // RPC_C_IMP_LEVEL_xxx
        NULL, // client identity
        EOAC_NONE // proxy capabilities
        );

    if (FAILED(hres))
    {
        pSvc->Release();
        pLoc->Release();
        CoUninitialize();
        return false;
    }

    // 步骤6：Use the IWbemServices pointer to make requests of WMI ----
    IEnumWbemClassObject* pEnumerator = NULL;
    //计算CPUID
    hres = pSvc->ExecQuery(
        bstr_t("WQL"),
        bstr_t("SELECT * FROM Win32_Processor"),//Win32_OperatingSystem
        WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
        NULL,
        &pEnumerator);

    if (FAILED(hres))
    {
        pSvc->Release();
        pLoc->Release();
        CoUninitialize();
        return false;
    }
    // 步骤7：Get the data from the query
    IWbemClassObject *pclsObj;
    ULONG uReturn = 0;
    while (pEnumerator)
    {
        HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1,
            &pclsObj, &uReturn);

        if(0 == uReturn)
        {
            break;
        }
        VARIANT vtProp;
        VariantInit(&vtProp);
        hr = pclsObj->Get(L"ProcessorId", 0, &vtProp, 0, 0);
        if (SUCCEEDED(hr))
        {

            if (vtProp.vt == VT_NULL)
                strcpy(szInfoBuf, "null_cpu");
            else
            {
                char* lpsz = _com_util::ConvertBSTRToString(vtProp.bstrVal);
                strcpy(szInfoBuf, lpsz);
                delete[] lpsz;
                SysFreeString(vtProp.bstrVal);
            }
            qbCpuID = szInfoBuf;
        }
    }


    /*
    //主板
    hres = pSvc->ExecQuery(
        bstr_t("WQL"),
        bstr_t("SELECT * FROM Win32_BaseBoard"),//Win32_OperatingSystem
        WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
        NULL,
        &pEnumerator);

    if (FAILED(hres))
    {
        pSvc->Release();
        pLoc->Release();
        CoUninitialize();
        return false;
    }
    // 步骤7：Get the data from the query
    //IWbemClassObject *pclsObj;
    //ULONG
    uReturn = 0;
    while (pEnumerator)
    {
        HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1,
            &pclsObj, &uReturn);

        if(0 == uReturn)
        {
            break;
        }
        VARIANT vtProp;
        VariantInit(&vtProp);
        hr = pclsObj->Get(L"Manufacturer", 0, &vtProp, 0, 0);
        strProcessID+=_com_util::ConvertBSTRToString(vtProp.bstrVal);//strProcessID：类级变量

        hr = pclsObj->Get(L"Product", 0, &vtProp, 0, 0);
        strProcessID+=_com_util::ConvertBSTRToString(vtProp.bstrVal);//strProcessID：类级变量

        hr = pclsObj->Get(L"SerialNumber", 0, &vtProp, 0, 0);
        strProcessID+=_com_util::ConvertBSTRToString(vtProp.bstrVal);//strProcessID：类级变量
    }
    */



    //BIOS


    hres = pSvc->ExecQuery(
        bstr_t("WQL"),
        bstr_t("SELECT * FROM Win32_BIOS"),//Win32_OperatingSystem
        WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
        NULL,
        &pEnumerator);

    if (FAILED(hres))
    {
        pSvc->Release();
        pLoc->Release();
        CoUninitialize();
        return false;
    }
    // 步骤7：Get the data from the query
    //IWbemClassObject *pclsObj;
    //ULONG
    uReturn = 0;
    while (pEnumerator)
    {
        HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1,
            &pclsObj, &uReturn);

        if(0 == uReturn)
        {
            break;
        }
        VARIANT vtProp;
        VariantInit(&vtProp);
        hr = pclsObj->Get(L"SerialNumber", 0, &vtProp, 0, 0);
        if (SUCCEEDED(hr))
        {
            if (vtProp.vt == VT_NULL)
                strcpy(szInfoBuf, "null_d_sn");
            else
            {
                char* lpsz = _com_util::ConvertBSTRToString(vtProp.bstrVal);
                strcpy(szInfoBuf, lpsz);
                delete[] lpsz;
                SysFreeString(vtProp.bstrVal);
            }
            qbBiosID = szInfoBuf;
        }
    }



    //MAC
    hres = pSvc->ExecQuery(
        bstr_t("WQL"),
        bstr_t("SELECT MACAddress FROM Win32_NetworkAdapter WHERE ((MACAddress Is Not NULL) AND (Manufacturer <> 'Microsoft'))"),//Win32_OperatingSystem
        WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
        NULL,
        &pEnumerator);

    if (FAILED(hres))
    {
        pSvc->Release();
        pLoc->Release();
        CoUninitialize();
        return false;
    }
    // 步骤7：Get the data from the query
    //IWbemClassObject *pclsObj;
    //ULONG
    uReturn = 0;
    //while (pEnumerator)
    {
        HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1,
            &pclsObj, &uReturn);

        if(0 == uReturn)
        {
            //break;
        }
        VARIANT vtProp;
        VariantInit(&vtProp);
        hr = pclsObj->Get(L"MACAddress", 0, &vtProp, 0, 0);
        if (SUCCEEDED(hr))
        {
            if (vtProp.vt == VT_NULL)
                strcpy(szInfoBuf, "null_mac");
            else
            {
                char* lpsz = _com_util::ConvertBSTRToString(vtProp.bstrVal);
                strcpy(szInfoBuf, lpsz);
                delete[] lpsz;
                SysFreeString(vtProp.bstrVal);
            }
            qbMacAdd = szInfoBuf;
        }
    }



    //计算硬盘系列号
    hres = pSvc->ExecQuery(
        bstr_t("WQL"),
        bstr_t("SELECT * FROM Win32_DiskDrive"),
        WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
        NULL,
        &pEnumerator);

    if (FAILED(hres))
    {
        pSvc->Release();
        pLoc->Release();
        CoUninitialize();
        return false;
    }
    while (pEnumerator)
    {
        HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1,
            &pclsObj, &uReturn);

        if(0 == uReturn)
        {
            break;
        }

        VARIANT vtProp;
        VariantInit(&vtProp);
        // 多个硬盘, 就检查硬盘类型是USB还是IDE还是SCSI
        // IDE\\DISK   USBSTOR\\DISK
        hr = pclsObj->Get(L"PNPDeviceID", 0, &vtProp, 0, 0);
        if (SUCCEEDED(hr))
        {
            char* lpsz = _com_util::ConvertBSTRToString(vtProp.bstrVal);
            if (strstr(lpsz, "IDE\\DISK") || strstr(lpsz, "SCSI\\DISK"))
            {
                // 如果是硬盘, 而不是U盘
                // 删除操作放到最后做, 因为当下面所有方法获取值都失效后, 需要记录此硬盘型号, 方便日后买一块回来继续调试^_^
                //delete[] lpsz;
                SysFreeString(vtProp.bstrVal);

                hr = pclsObj->Get(L"Signature", 0, &vtProp, 0, 0);
                if (SUCCEEDED(hr))
                {
                    szInfoBuf[0]=0;
                    if (vtProp.vt != VT_NULL)
                    {
                        unsigned int iValue = vtProp.uintVal;
                        char szTmp[100] = {0};
                        ultoa(iValue, szTmp, 10);
                        if (strcmp(szInfoBuf, ""))
                        {
                            strcat(szInfoBuf, "|");
                        }
                        strcat(szInfoBuf, szTmp);
                    }
                    else
                    {
                        // 获取Signature为空, 就获取SerialNumber
                        hr = pclsObj->Get(L"SerialNumber", 0, &vtProp, 0, 0);
                        if (SUCCEEDED(hr))
                        {
                            if (vtProp.vt != VT_NULL)
                            {
                                char* lpsz = _com_util::ConvertBSTRToString(vtProp.bstrVal);
                                char* trim = TrimSimpleChart(lpsz);
                                if (trim)
                                {
                                    if (strcmp(szInfoBuf, ""))
                                    {
                                        strcat(szInfoBuf, "|");
                                    }
                                    strcat(szInfoBuf, trim);
                                    delete[] trim;
                                    delete[] lpsz;
                                    SysFreeString(vtProp.bstrVal);
                                }
                                else
                                {
                                    if (strcmp(szInfoBuf, ""))
                                    {
                                        strcat(szInfoBuf, "|");
                                    }
                                    strcat(szInfoBuf, lpsz);
                                }
                            }
                            else
                            {
                                // 以上获取方法都失效了, 先记录此硬盘型号, 方便日后买一块回来继续调试^_^
                                if (strcmp(szInfoBuf, ""))
                                {
                                    strcat(szInfoBuf, "|");
                                }
                                strcat(szInfoBuf, lpsz);
                            }
                        }
                    }
                    qbDiskID = szInfoBuf;
                }

                // 删除操作放到最后做, 因为当下面所有方法获取值都失效后, 需要记录此硬盘型号, 方便日后买一块回来继续调试^_^
                delete[] lpsz;
            }
        }
    }

    pSvc->Release();
    pLoc->Release();
    pEnumerator->Release();
    pclsObj->Release();
    //CoUninitialize();
#endif
//    return QByteArray("")+"cpuid="+qbCpuID+"&biosid="+qbBiosID+"&diskid="
//            +qbDiskID+"&macaddr="+qbMacAdd/*+"&osver="+qbOSVer*/;
    return true;
}
