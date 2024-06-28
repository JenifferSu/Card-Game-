#include <iomanip>
#include <regex>

struct AccountScore {
    int accountNo;
    int roundScores[3]; // Scores for rounds 1, 2, and 3
    string roundQuestion[3]; // Questions for rounds 1, 2, and 3
    int totalScore;

    // Default constructor
    AccountScore() : accountNo(0), totalScore(0) {
        roundScores[0] = 0;
        roundScores[1] = 0;
        roundScores[2] = 0;
        roundQuestion[0] = "";
        roundQuestion[1] = "";
        roundQuestion[2] = "";
    }

    // Parameterized constructor
    AccountScore(int no) : accountNo(no), totalScore(0) {
        roundScores[0] = 0;
        roundScores[1] = 0;
        roundScores[2] = 0;
        roundQuestion[0] = "";
        roundQuestion[1] = "";
        roundQuestion[2] = "";
    }
};

class StorageManager
{
private:


    static string truncateString(const string& str, int width) {

        trim(str);
        if (str.length() > width - 3) {
            return str.substr(0, width - 6) + "...";
        }
        return str;
    }

    static void trim(const string& s) {
        string result = s;
        result = regex_replace(result, regex("^\\s+ "), ""); // Trim leading whitespace
        result = regex_replace(result, regex("\\s+$ "), ""); // Trim trailing whitespace
    }

    static void printScores(const int(&scores)[3], int width) {
        for (int score : scores) {
            int padding = (width - to_string(score).length()) / 2;
            cout << setw(padding) << "" << score << setw(padding) << "";
        }
    }

    static void bubbleSort(vector<AccountScore>& scores, bool descending)
    {
        int n = scores.size();
        for (int i = 0; i < n - 1; i++) {
            for (int j = 0; j < n - i - 1; j++) {
                if (descending) {
                    if (scores[j].totalScore > scores[j + 1].totalScore) {
                        swap(scores[j], scores[j + 1]);
                    }
                }
                else {
                    if (scores[j].totalScore < scores[j + 1].totalScore) {
                        swap(scores[j], scores[j + 1]);
                    }
                }
            }
        }
    }

public:
    static string SCORE_FILE;
    static string ACCOUNT_FILE;

    // Struct to store scores for each round and total score


    static void writeListToCSV(DoublyLinkedList& list, string& filename)
    {
        ofstream file(filename);
        if (file.is_open())
        {
            Node* current = list.getHead();
            while (current)
            {
                if (ScoreNode* sNode = dynamic_cast<ScoreNode*>(current))
                {
                    file << sNode->accountNo << "|" << sNode->question << "|" << sNode->score << "|" << sNode->round << "\n";
                }
                else if (AccountNode* aNode = dynamic_cast<AccountNode*>(current))
                {
                    file << aNode->accountNo << "," << aNode->name << "," << aNode->scoreRound1 << "," << aNode->scoreRound2 << "," << aNode->scoreRound3 << "\n";
                }
                current = current->next;
            }
            file.close();
        }
        else
        {
            cerr << "Unable to open file: " << filename << endl;
        }
    }

    static DoublyLinkedList readListFromCSV(string& filename)
    {
        DoublyLinkedList list;
        ifstream file(filename);
        string line;
        if (file.is_open())
        {
            while (getline(file, line))
            {
                stringstream ss(line);
                string accountNo, type, numberStr, question, answer, scoreStr, roundStr, name, scoreRound1, scoreRound2, scoreRound3;
                if (filename == SCORE_FILE)
                {
                    getline(ss, accountNo, '|');
                    getline(ss, question, '|');
                    getline(ss, scoreStr, '|');
                    getline(ss, roundStr, '|');
                    list.pushScore(stoi(accountNo), question, answer, stoi(scoreStr), stoi(roundStr));
                }
                else if (filename == ACCOUNT_FILE)
                {
                    getline(ss, numberStr, ',');
                    getline(ss, name, ',');
                    getline(ss, scoreRound1, ',');
                    getline(ss, scoreRound2, ',');
                    getline(ss, scoreRound3, ',');

                    list.pushAccount(stoi(numberStr), name, stoi(scoreRound1), stoi(scoreRound2), stoi(scoreRound3));
                }
            }
            file.close();
        }
        else
        {
            cerr << "Unable to open file: " << filename << endl;
        }
        return list;
    }

    static DoublyLinkedList readScoresFromCSV()
    {
        return readListFromCSV(SCORE_FILE);
    }

    static void writeScoresToCSV(DoublyLinkedList& list)
    {
        writeListToCSV(list, SCORE_FILE);
    }

    static void addScore(DoublyLinkedList& list, int accNo, string question, int score, int round)
    {
        ScoreNode* newNode = new ScoreNode(accNo, question, score, round);
        list.pushScore(newNode);
        writeScoresToCSV(list);
    }

    static DoublyLinkedList readUsersFromCSV()
    {
        return readListFromCSV(ACCOUNT_FILE);
    }

    static void writeUsersToCSV(DoublyLinkedList& list)
    {
        writeListToCSV(list, ACCOUNT_FILE);
    }

    static vector<AccountScore> getScores(const DoublyLinkedList& list)
    {
        unordered_map<int, AccountScore> accountScores;

        Node* current = list.getHead();
        while (current)
        {
            if (ScoreNode* sNode = dynamic_cast<ScoreNode*>(current))
            {
                accountScores[sNode->accountNo].accountNo = sNode->accountNo;
                accountScores[sNode->accountNo].roundScores[sNode->round - 1] += sNode->score;
                accountScores[sNode->accountNo].roundQuestion[sNode->round - 1] = sNode->question;
                accountScores[sNode->accountNo].totalScore += sNode->score;
            }
            current = current->next;
        }

        vector<AccountScore> scores;
        for (const auto& entry : accountScores)
        {
            scores.push_back(entry.second);
        }

        sort(scores.begin(), scores.end(), [](const AccountScore& a, const AccountScore& b)
            { return b.totalScore < a.totalScore; });

        return scores;
    }

    static void printRankingChart(const vector<AccountScore>& scores, DoublyLinkedList& accounts, bool descending = false, const string& targetName="")
    {
        vector<AccountScore> sortedScores = scores;

        bubbleSort(sortedScores, descending);

        const int rankWidth = 6;
        const int nameWidth = 20;
        const int roundWidth = 30;
        const int totalWidth = 15;

        cout << "Ranking Chart:\n";
        cout << setw(rankWidth) << left << "Rank"
            << setw(nameWidth) << left << "Name"
            << setw(roundWidth) << left << "Round 1"
            << setw(roundWidth) << left << "Round 2"
            << setw(roundWidth) << left << "Round 3"
            << setw(totalWidth) << left << "Total Score" << "\n";

        int currentRank = descending ? 10 : 1;
        for (const auto& score : sortedScores) {
            AccountNode* account = dynamic_cast<AccountNode*>(accounts.getNodeByAccountNo(score.accountNo));
            if (account && (targetName.empty() || account->name == targetName)) {
                cout << setw(rankWidth) << left << currentRank
                    << setw(nameWidth) << left << account->name
                    << setw(roundWidth) << left << truncateString(score.roundQuestion[0], roundWidth)
                    << setw(roundWidth) << left << truncateString(score.roundQuestion[1], roundWidth)
                    << setw(roundWidth) << left << truncateString(score.roundQuestion[2], roundWidth)
                    << setw(totalWidth) << left << score.totalScore
                    << "\n";
                cout << setw(rankWidth) << left << ""
                    << setw(nameWidth) << left << "";
                printScores(score.roundScores, roundWidth);
                cout << setw(totalWidth) << left << ""
                    << "\n\n";
            }
            if (!descending) currentRank++;
            else currentRank--;
        }
    }

    static void resetScore() {
        ofstream ofs(SCORE_FILE, ofstream::trunc);
        ofs.close();
    }
};

string StorageManager::SCORE_FILE = "Score.csv";
string StorageManager::ACCOUNT_FILE = "Account.csv";
