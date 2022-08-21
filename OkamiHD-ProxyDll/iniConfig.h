#pragma once
#include "ini.h"

#define CONFIG_FILE "Chalice.ini"

std::wstring chainLoadDLL = L"";

const char* defaultFile =
";Chalice by Church Guard\n\n"
"; To load another dll that requires to be dinput8.dll, simply rename the\n"
"; other dll file, place it the same folder as this and enter its name below\n"
"; Depending on what the other dll does this may not work, but usually does\n"
"; example: dll = wexdust.dll\n"
"[CHAINLOAD]\n"
"dll=\n";
//https://stackoverflow.com/questions/6691555/converting-narrow-string-to-wide-string
inline std::wstring convert_s2ws(const std::string& as)
{
    // deal with trivial case of empty string
    if (as.empty())    return std::wstring();

    // determine required length of new string
    size_t reqLength = ::MultiByteToWideChar(CP_UTF8, 0, as.c_str(), (int)as.length(), 0, 0);

    // construct new string of required length
    std::wstring ret(reqLength, L'\0');

    // convert old string to new string
    ::MultiByteToWideChar(CP_UTF8, 0, as.c_str(), (int)as.length(), &ret[0], (int)ret.length());

    // return new string ( compiler should optimize this away )
    return ret;
}

void loadINIConfig() {
    std::ifstream f(CONFIG_FILE);
    if (!f.good()) {
        std::ofstream out(CONFIG_FILE);
        out << defaultFile << std::endl;
        out.close();
    }
    std::ifstream g(CONFIG_FILE);
    if (g.good()) {
        mINI::INIFile cfgFile(CONFIG_FILE);
        mINI::INIStructure ini;
        if (cfgFile.read(ini)) {

            chainLoadDLL = convert_s2ws(ini.get("CHAINLOAD").get("dll"));
        }
        cfgFile.~INIFile();
        ini.clear();
    }
}