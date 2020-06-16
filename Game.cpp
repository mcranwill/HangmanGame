#pragma once

#include <iostream>
#include <random>

#include "Game.h"

// Default Constructor
Game::Game()
{
    currentWord = GetRandomString();
    hiddenWord.insert(0, currentWord.size(), '_');
}

// Constructor requiring one parameter to choose from possible words
Game::Game(int wordChoice) :
    currentWord(possibleWords[wordChoice % possibleWords.size()])
{
    hiddenWord.insert(0, currentWord.size(), '_');
}

// Required utility function to avoid processing implicit newline characters from input via command line
void Game::ClearInputBuffer()
{
    char c = '\0';
    do
    {
        c = std::getchar();
    } while (c != '\n' && c != EOF);
}

// Example collected from https://en.cppreference.com/w/cpp/numeric/random/uniform_int_distribution
// to accomplish getting another string from possible words.
std::string Game::GetRandomString()
{
    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> distrib(0, possibleWords.size()-1);
    return possibleWords[distrib(gen)];
}

// Print current state of game using hiddenWord and usedGuesses to explain past choices
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

// Game Resets with a y or Y selection and cleans up object.
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

// Heavy lifting method to process guesses and reveal correct guesses and
// update the current state of the game.
bool Game::ProcessGuess()
{
    bool charInWord = false, continueGame = true;
    std::cout << "Enter your guess.\n";
    
    // Save first character entered by the user even if \n is the first char entered
    const char guessedLetter = std::tolower(std::getchar());
    ClearInputBuffer();

    // Turn all characters matching guessedLetter in hiddenWord to guessedLetter  
    for (size_t index = 0; index < currentWord.size(); ++index)
    {
        if (guessedLetter == currentWord[index])
        {
            charInWord = true;
            hiddenWord[index] = guessedLetter;
        }
        // If no match is found, then leave charInWord as false
    }

    if (charInWord && hiddenWord == currentWord)
    {
        // Word has been completely revealed and has no '_' left
        std::cout << "Congrats: You uncovered the word " << currentWord << '\n';
        continueGame = false;   // Current Game is over
    }
    else if (!charInWord)
    {
        // Guessed too many times, game over
        
        size_t tempIndex = wrongGuessesList.size();
        usedGuesses[tempIndex] = body[tempIndex];
        if (tempIndex == body.size() - 1)
        {
            std::cout << "Game Over!\n";
            PrintGallows();
            continueGame = false;
        }
        else
        {
            wrongGuessesList.push_back(guessedLetter);
            // As a courtesy, check if user is almost out of luck
            // since wrongGuessesList size has increased after the push_back
            if (wrongGuessesList.size() == 6)
                std::cout << "This is your last guess\n";
        }

    }
    
    return continueGame;
}
