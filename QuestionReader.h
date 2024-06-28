#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <random>
#include <algorithm>

#include "DoublyLinkedList.h" // seperated from DiscardedCard.h
#include "StorageManager.h"

using namespace std;

class QuestionReader
{
public:
    QuestionReader()
        : rollnum(generateRandomNumber()), currentScore(0) {}

    int generateRandomNumber()
    {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> distribution(1, 100); // Adjust to actual range
        return distribution(gen);
    }

    string readQuestion()
    {
        ifstream fin("File.csv");
        if (!fin.is_open())
        {
            cerr << "Unable to open file." << endl;
            return "";
        }

        string line, roll, question, answer, score;
        while (getline(fin, line))
        {
            stringstream ss(line);
            getline(ss, roll, '|');
            try
            {
                int rollNumber = stoi(roll);

                if (rollNumber == rollnum)
                {
                    getline(ss, question, '|');
                    getline(ss, answer, '|');
                    getline(ss, score, '|');

                    currentQuestion = question;
                    currentAnswer = answer;
                    currentScore = stoi(score);

                    cout << "Question: " << question << "\n";
                    return question;
                }
            }
            catch (const invalid_argument& e)
            {
                cerr << "Invalid argument: " << e.what() << endl;
                cerr << "Error parsing roll number: " << roll << endl;
            }
        }

        cerr << "Record not found for roll number: " << rollnum << endl;
        currentQuestion = "";
        currentAnswer = "";
        currentScore = 0;
        return "";
    }

    bool answerQuestion(string &question, int accountNo, DoublyLinkedList &scores, int round) const
    {
        ifstream fin("File.csv");
        if (!fin.is_open())
        {
            cerr << "Unable to open file." << endl;
            return false;
        }

        string line, roll, questionFromFile, answer, score;
        while (getline(fin, line))
        {
            stringstream ss(line);
            getline(ss, roll, '|');
            int rollNumber = stoi(roll);

            if (rollNumber == rollnum)
            {
                getline(ss, questionFromFile, '|');
                getline(ss, answer, '|');
                getline(ss, score, '|');

                cout << "Enter Correct Answer (True/False): ";
                string userAnswer;
                cin >> userAnswer;

                if (userAnswer == answer)
                {
                    cout << "Congratulations, you are correct!" << endl;
                    StorageManager::addScore(scores, accountNo, questionFromFile, stoi(score), round);
                }
                else
                {
                    cout << "Skill issue" << endl;
                    cout << "The Correct Answer is: " << answer << endl;
                    StorageManager::addScore(scores, accountNo, questionFromFile, 0, round);
                }
                return true;
            }
        }

        cerr << "Record not found\n";
        return false;
    }

    int getRollnum() const
    {
        return rollnum;
    }

    string getCurrentQuestion() const
    {
        return currentQuestion;
    }

    string getCurrentAnswer() const
    {
        return currentAnswer;
    }

    int getCurrentScore() const
    {
        return currentScore;
    }

public:
        int rollnum;

private:
    string currentQuestion;
    int currentScore;
    string currentAnswer;
};
