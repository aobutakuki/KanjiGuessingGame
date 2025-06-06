#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <random>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <vector>
#include <chrono>
#include <thread>
#include <windows.h>

struct Entry {
    std::string word;
    std::string reading1;
    std::string reading2;
    bool parseSucess = true;
};

std::string readRandomWordFromFile(const std::string& filePath) {
    std::ifstream readFile(filePath);
    std::string word;
    std::vector<std::string> lines;

    if (!readFile.is_open()) {
        std::cerr << "Error: Failed to open file: " << filePath << std::endl;
        return "";
    }

    std::string currentLine;
    while (std::getline(readFile, currentLine)) {
        lines.push_back(currentLine);
    }
    readFile.close();

    if (lines.empty()) {
        std::cerr << "Error: File is empty or could not be read." << std::endl;
        return "";
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, lines.size() - 1);
    int randomIndex = dis(gen);

    return lines[randomIndex];
}

Entry parseWordInfo(const std::string& word) {
    Entry entry;
    std::stringstream ss(word);

    if (std::getline(ss, entry.word, ',')) {
        std::cout << "get word successful!!\n";
    }
    if (std::getline(ss, entry.reading1, ',')) {
        std::cout << "get reading1 successful\n";
    }
    if (std::getline(ss, entry.reading2)) {
        std::cout << "get reading2 successful\n";
    }
    return entry;
}

int main() {
    // Simple console setup - just set codepage
    system("chcp 65001 >nul");
    system("cls");


    bool gameOver = false;
    while (!gameOver) {
        int attemptCount = 0;
        static int wordCount = 0;
        std::string word = readRandomWordFromFile("kotoba/jouyou.csv");
        Entry currentWord = parseWordInfo(word);
        bool attempt = true;

        while (attempt) {
            system("cls");
            std::cout << u8"残り回数： " << (3 - attemptCount) << u8" | 正解問： " << wordCount << "\n\n";
            std::cout << u8"読み方入れてください　【 " << currentWord.word << u8" 】\n ?:";

            std::string answer;
            std::cin >> answer;

            if (answer == currentWord.reading1 || answer == currentWord.reading2) {
                std::cout << u8"おめでとうございます！";
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
                attempt = false;
                wordCount++;
                break;
            }
            else {
                attemptCount++;
                if (attemptCount >= 3) {
                    std::cout << u8"ゲームオーバー！\n";
                    std::this_thread::sleep_for(std::chrono::milliseconds(500));
                    gameOver = true;
                }
                else {
                    std::cout << u8"ブーブー不正解です！もう一回入れてください！";
                    std::this_thread::sleep_for(std::chrono::milliseconds(500));
                }
            }
        }
    }

    return 0;
}