#include <iostream>
#include <string>
#include <fstream>
#include <unordered_map>
#include "QuestionReader.h"
#include "Account.h"
#include "Answer.h"
#include "Tree.h"

using namespace std;

vector<pair<int, int>> calculateScores(const string &filename)
{
    unordered_map<int, int> accountScores;
    ifstream file(filename);
    string line;

    if (file.is_open())
    {
        while (getline(file, line))
        {
            stringstream ss(line);
            string accountNoStr, question, scoreStr, roundStr;
            getline(ss, accountNoStr, '|');
            getline(ss, question, '|');
            getline(ss, scoreStr, '|');
            getline(ss, roundStr, '|');

            int accountNo = stoi(accountNoStr);
            int score = stoi(scoreStr);

            accountScores[accountNo] += score;
        }
        file.close();
    }
    else
    {
        cerr << "Unable to open file: " << filename << endl;
    }

    vector<pair<int, int>> scoresRanking;
    for (const auto &entry : accountScores)
    {
        scoresRanking.emplace_back(entry.first, entry.second);
    }

    sort(scoresRanking.begin(), scoresRanking.end(), [](const pair<int, int> &a, const pair<int, int> &b)
         { return b.second < a.second; });

    return scoresRanking;
}

DoublyLinkedList discardedDeck;

void discardedcard(int number, string question, string answer, int score);
bool selectDiscardedCard(const int &accountNo, DoublyLinkedList &scores, int &round);

void scoreboard();
// unanswered 10, 20, 30
// discarded 8, 16, 24

void placement();
void round1(DoublyLinkedList &scores, int accountNo, DoublyLinkedList &accounts);
void round2(DoublyLinkedList &scores, int accountNo, DoublyLinkedList &accounts);
void round3(DoublyLinkedList &scores, int accountNo, DoublyLinkedList &accounts);

void round(DoublyLinkedList &scores, int accountNo, int roundNum, DoublyLinkedList &accounts)
{
    int choice, choose;
    bool isAnswered = false;
    QuestionReader qr;
    string question;
    while (!isAnswered)
    {

        cout << "Enter your choice: ";
        cout << "1. See unanswered card, 2. See discarded card \n";
        cin >> choice;

        if (cin.fail())
        {
            cin.clear();               // clear the error flag
            cin.ignore(INT_MAX, '\n'); // discard invalid input
            cout << "Invalid input. Please enter a valid number." << endl;
            return;
        }

        switch (choice)
        {
        case 1:
            cout << "Here is your question" << endl;
            question = qr.readQuestion();
            cout << "1. Answer, 2. Discard\n";
            cin >> choose;
            if (cin.fail())
            {
                cin.clear();               // clear the error flag
                cin.ignore(INT_MAX, '\n'); // discard invalid input
                cout << "Invalid input. Please enter a valid number." << endl;
            }
            else if (choose == 1)
            {
                isAnswered = qr.answerQuestion(question, accountNo, scores, roundNum);
            }
            else if (choose == 2)
            {
                cout << "Discard success" << endl;
                discardedcard(qr.getRollnum(), qr.getCurrentQuestion(), qr.getCurrentAnswer(), qr.getCurrentScore());
                qr.rollnum = qr.generateRandomNumber();
            }
            else
            {
                cout << "Invalid choice" << endl;
            }
            break;

        case 2:
            if (discardedDeck.size > 0)
            {
                cout << "Below are the discarded cards:" << endl;
                isAnswered = selectDiscardedCard(accountNo, scores, roundNum);
            }
            else
            {
                cout << "No discarded cards." << endl;
            }

            break;

        default:
            cout << "Invalid choice" << endl;
        }
    }
}

void round1(DoublyLinkedList &scores, int accountNo, DoublyLinkedList &accounts)
{
    round(scores, accountNo, 1, accounts);
}

void round2(DoublyLinkedList &scores, int accountNo, DoublyLinkedList &accounts)
{
    round(scores, accountNo, 2, accounts);
}

void round3(DoublyLinkedList &scores, int accountNo, DoublyLinkedList &accounts)
{
    round(scores, accountNo, 3, accounts);
}

void discardedcard(int number, string question, string answer, int score)
{
    discardedDeck.pushDiscardedCard(number, question, answer, score);
}

bool selectDiscardedCard(const int &accountNo, DoublyLinkedList &scores, int &round)
{
    Node *baseNode = discardedDeck.getHead();
    DiscardedCardNode *current = dynamic_cast<DiscardedCardNode *>(baseNode);

    if (current)
    {
        // Display discarded cards with index numbers
        while (current)
        {
            cout << current->number << ". Question: " << current->question << ", Score: " << current->score << endl;
            current = dynamic_cast<DiscardedCardNode *>(current->next);
        }

        int choice;
        cout << "Enter the number of the discarded card you want to select or 0 to go back: ";
        cin >> choice;

        if (choice == 0)
        {
            return false; // Go back to the previous menu
        }
        else
        {
            DiscardedCardNode *selectedCard = discardedDeck.getDiscardedCardNodeByNumber(choice);

            if (selectedCard)
            {
                cout << "You selected discarded card: " << selectedCard->question << " with score: " << selectedCard->score << endl;

                // Ask for the answer
                string userAnswer;
                cout << "Enter your answer: ";
                cin.ignore(); // To handle the newline character left by previous input
                getline(cin, userAnswer);

                if (userAnswer == selectedCard->answer)
                {
                    cout << "Correct !!" << endl;
                    int score = static_cast<int>(selectedCard->score * 0.8); // Discarded card score is 80% of the original score
                    StorageManager::addScore(scores, accountNo, selectedCard->question, score, round);
                    discardedDeck.deleteNode(selectedCard);
                }
                else
                {
                    cout << "Wrong Answer!" << endl;
                    StorageManager::addScore(scores, accountNo, selectedCard->question, 0, round);
                }

                return true;
            }
            else
            {
                cout << "Invalid choice" << endl;
                return false;
            }
        }
    }
    return false;
}

int main() {
    StorageManager::resetScore();
    DoublyLinkedList scores;
    DoublyLinkedList accounts = StorageManager::readUsersFromCSV();

    cout << "Welcome to Trivia\n\n";
    vector<int> accountNoList = accounts.getAccountNoList();
    unsigned short int currentRound = 0;

    for (unsigned short int curRound = 0; curRound < 3; curRound++) {
        for (unsigned short int accIdx = 0; accIdx < accountNoList.size(); accIdx++) {
            AccountNode* account = dynamic_cast<AccountNode*>(accounts.getNodeByAccountNo(accountNoList[accIdx]));
            if (account) {
                int accountNo = account->accountNo;
                cout << "Starting round " << curRound + 1 <<" for : " << account->name << " (Account No: " << accountNo << ")\n";
                cout << "\nROUND " << curRound + 1 << "\n";
                if (curRound == 0)
                    round1(scores, accountNo, accounts);
                if (curRound == 1)
                    round2(scores, accountNo, accounts);
                if (curRound == 2)
                    round3(scores, accountNo, accounts);
                cout << "\nFinished round " << curRound + 1 <<" for : " << account->name << "\n";
            }
            else {
                cerr << "Account Info Retrieval Error" << endl;
            }
        }
        cout << "\n\nLeaderboard Ranking:" << endl;
        DoublyLinkedList scoreList = StorageManager::readScoresFromCSV();
        vector<AccountScore> scoresRanking = StorageManager::getScores(scoreList);
        StorageManager::printRankingChart(scoresRanking, accounts);
    }

    //Node *currentAccountNode = accounts.getHead();
    //while (currentAccountNode)
    //{
    //    AccountNode *account = dynamic_cast<AccountNode *>(currentAccountNode);
    //    if (account)
    //    {
    //        int accountNo = account->accountNo;
    //        cout << "Starting rounds for account: " << account->name << " (Account No: " << accountNo << ")\n";
    //        for (int i = 0; i < 3; ++i)
    //        {
    //            cout << "\nRound " << i + 1 << "\n";
    //            if (i == 0)
    //                round1(scores, accountNo, accounts);
    //            if (i == 1)
    //                round2(scores, accountNo, accounts);
    //            if (i == 2)
    //                round3(scores, accountNo, accounts);
    //        }
    //        cout << "Finished rounds for account: " << account->name << "\n";
    //    }
    //    currentAccountNode = currentAccountNode->next;
    //}

    vector<pair<int, int>> scoresTree = calculateScores(StorageManager::SCORE_FILE);

    BinaryTree tree;
    for (const auto &entry : scoresTree)
    {
        tree.insert(entry.first, entry.second);
    }

    cout << "\n\nHierarchical Tree:" << endl;
    tree.Dump(accounts);
    
    bool isExited = false;
    while (!isExited) {
        cout << "Menu:" << endl;
        cout << "1. Search Hierarchical Tree By Name" << endl;
        cout << "2. Sort Leaderboard" << endl;
        cout << "3. Search Leaderboard By Name" << endl;
        cout << "4. Exit" << endl;
        cout << "Enter your choice (1-4): ";

        int choice;
        if (!(cin >> choice)) {
            cout << "Invalid input." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        else if (choice < 1 || choice > 4) {
            cout << "Invalid choice." << endl;
        }
        else {
            switch (choice) {
            case 1: {
                // Search Hierarchical Tree By Name
                cout << "Enter the name to search in the tree: ";
                string name;
                cin.ignore(); // Clear newline left in the input buffer
                getline(cin, name);

                int accNo = accounts.getAccountNoByName(name);
                if (accNo > 0) {
                    // Ensure tree.searchByAccountNo is implemented
                    auto result = tree.searchByAccountNo(accNo);
                    if (result.first) {  // Assuming result is a pair<TreeNode*, int>
                        cout << "Found Account:\nName: " << name << "\nRank: " << result.second << "\n\n";
                    }
                    else {
                        cout << "Account found but not in the tree.\n\n";
                    }
                }
                else {
                    cout << "No account found with the name " << name << ".\n\n";
                }
                break;
            }
            case 2: {
                // Sort leaderboard
                cout << "Sorting Options:" << endl;
                cout << "1. By Rank Ascendingly" << endl;
                cout << "2. By Rank Descendingly" << endl;
                cout << "Enter your choice (1-2): ";

                int sortChoice;
                if (!(cin >> sortChoice)) {
                    cout << "Invalid input. Please enter a number (1-2)." << endl;
                    cin.clear(); // Clear error flags
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
                }
                else if (sortChoice < 1 || sortChoice > 2) {
                    cout << "Invalid choice. Please enter a number between 1 and 2." << endl;
                }
                else {
                    bool isDescending = sortChoice == 2;
                    // Ensure StorageManager functions are implemented
                    DoublyLinkedList scoreList = StorageManager::readScoresFromCSV();
                    vector<AccountScore> scoresRanking = StorageManager::getScores(scoreList);
                    StorageManager::printRankingChart(scoresRanking, accounts, isDescending);
                }
                break;
            }
            case 3: {
                // Search leaderboard by name
                cout << "Enter the name to search in the leaderboard: ";
                string targetName;
                cin.ignore(); // Clear newline left in the input buffer
                getline(cin, targetName);

                // Ensure StorageManager functions are implemented
                DoublyLinkedList scoreList = StorageManager::readScoresFromCSV();
                vector<AccountScore> scoresRanking = StorageManager::getScores(scoreList);
                StorageManager::printRankingChart(scoresRanking, accounts, false, targetName);
                break;
            }
            case 4: {
                isExited = true;
                break;
            }
            }
        }
    }

    cout << "Exiting the program.\n";

    return 0;
}

void credential();
// implement switch case to let users decide display or no then answer or no

void scoreboard();
// incrementing user score to scoreboard
// loop maybe to next user?

void credential();
// implement switch case to let users decide answer or no

void discardedcard();
// dump to discarded card if users choose to not answer
