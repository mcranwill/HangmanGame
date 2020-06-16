#pragma once

#include <array>
#include <string>
#include <vector>

class Game
{
public:
    Game();
    Game(int wordChoice);

    bool ProcessReset();
    void PrintGallows();
    bool ProcessGuess();

private:
    // Utility Functions
    void ClearInputBuffer();
    std::string GetRandomString();
    
    const std::string body{ "O\\|/|/\\" };
    std::string currentWord;
    std::string hiddenWord;
    std::string usedGuesses{ std::string().insert(0,body.length(), '\0') };
    std::vector<char> wrongGuessesList;

    const std::array<const std::string, 5> possibleWords{ {"mountain", "algorithmic", "labor", "statistics", "headphones"} };
};