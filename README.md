## GetDiscordToken
 Print Your Discord Token

## How to get discord token
```cpp
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
```

![result](result.png)