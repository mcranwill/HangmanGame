#pragma once

#include <iostream>
#include <random>

#include "Game.h"

// Default Constructor
Game::Game() : currentWord(GetRandomString())
{
    hiddenWord.insert(0, currentWord.size(), '_');
}

Game::Game(int wordChoice) :
    currentWord(possibleWords[wordChoice % possibleWords.size()])
{
    hiddenWord.insert(0, currentWord.size(), '_');
}

void Game::ClearInputBuffer()
{
    char c = '\0';
    do
    {
        c = std::getchar();
    } while (c != '\n' && c != EOF);
}

std::string Game::GetRandomString()
{
    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> distrib(0, possibleWords.size()-1);
    return possibleWords[distrib(gen)];
}

void Game::PrintGallows()
{
    std::cout << "___________\n";
    std::cout << "|          |\n";
    std::cout << "|          " << usedGuesses[0] << "\n";
    std::cout << "|         " << usedGuesses[1] << usedGuesses[2] << usedGuesses[3] << "\n";
    std::cout << "|          " << usedGuesses[4] << "\n";
    std::cout << "|         " << usedGuesses[5] << " " << usedGuesses[6] << "\n";
    std::cout << "|          \n";

    std::cout << '\n' << "  " << hiddenWord << '\n';
    std::cout << "Incorrect Guessed Letters\n";
    for (auto c : wrongGuessesList) { std::cout << c; }
    std::cout << '\n';
}

bool Game::ProcessReset()
{
    bool shouldReset = false;
    std::cout << "Enter y or Y if you would like to play again.\n";

    const char letter = std::tolower(std::getchar());
    ClearInputBuffer();

    if (letter == 'y')
    {
        // Perform reset steps
        shouldReset = true;
        currentWord = GetRandomString();
        hiddenWord.erase();
        hiddenWord.insert(0, currentWord.size(), '_');
        wrongGuessesList.clear();
        usedGuesses.insert(0, body.length(), '\0');
    }

    return shouldReset;
}

bool Game::ProcessGuess()
{
    bool charInWord = false, continueGame = true;
    std::cout << "Enter your guess.\n";

    const char guessedLetter = std::tolower(std::getchar());
    ClearInputBuffer();

    // Turn all characters matching guessedLetter in hiddenWord over
    for (size_t index = 0; index < currentWord.size(); ++index)
    {
        if (guessedLetter == currentWord[index])
        {
            charInWord = true;
            hiddenWord[index] = guessedLetter;
        }
    }

    if (!charInWord && wrongGuessesList.size() >= 6) {
        // 
        std::cout << "Game Over!\n";
        int tempIndex = wrongGuessesList.size();
        usedGuesses[tempIndex] = body[tempIndex];
        PrintGallows();
        continueGame = false;
    }
    else if (charInWord && hiddenWord == currentWord)
    {
        std::cout << "Congrats: You uncovered the word " << currentWord << '\n';
        continueGame = false;   // Current Game is over
    }
    else if (!charInWord)
    {
        int tempIndex = wrongGuessesList.size();
        wrongGuessesList.push_back(guessedLetter);
        usedGuesses[tempIndex] = body[tempIndex];
        if (wrongGuessesList.size() == 6)
            std::cout << "This is your last guess\n";
    }
    
    return continueGame;
}
