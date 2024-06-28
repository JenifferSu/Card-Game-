#include <string>
#include <iostream>

using namespace std;

class Node
{
public:
    Node *prev;
    Node *next;

    Node() : prev(nullptr), next(nullptr) {}
    virtual ~Node() {}
};

class QuestionNode : public Node
{
public:
    int accountNo;
    string question;
    string answer;
    int score;
    int round;

    QuestionNode(int num, string q, string ans, int s, int r)
        : accountNo(num), question(q), answer(ans), score(s), round(r) {}
};

class AccountNode : public Node
{
public:
    int accountNo;
    string name;
    int scoreRound1, scoreRound2, scoreRound3;

    AccountNode(int num, string n, int sr1, int sr2, int sr3)
        : accountNo(num), name(n), scoreRound1(sr1), scoreRound2(sr2), scoreRound3(sr3) {}
};

class DiscardedCardNode : public Node
{
public:
    int number;
    string question;
    string answer;
    int score;

    DiscardedCardNode(int num, string q, string ans, int s)
        : number(num), question(q), answer(ans), score(s) {}
};

class ScoreNode : public Node
{
public:
    int accountNo;
    string question;
    int score;
    int round;

    // Fixing the constructor parameters to match the class name
    ScoreNode(int accNo, string q, int s, int r)
        : accountNo(accNo), question(q), score(s), round(r) {}
};

class DoublyLinkedList
{
private:
    Node *head;
    Node *tail;

public:
    int size = 0;
    DoublyLinkedList() : head(nullptr), tail(nullptr) {}

    ~DoublyLinkedList()
    {
        while (head)
        {
            Node *temp = head;
            head = head->next;
            delete temp;
        }
    }

    Node *getHead() const
    {
        return head;
    }

    void pushScore(ScoreNode *newNode)
    {
        if (tail)
        {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
        else
        {
            head = tail = newNode;
        }
        size++;
    }

    void pushScore(int accountNo, string question, string answer, int score, int round)
    {
        ScoreNode *newNode = new ScoreNode(accountNo, question, score, round);
        if (tail)
        {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
        else
        {
            head = tail = newNode;
        }
        size++;
    }

    void pushQuestion(int accountNo, string question, string answer, int score, int round)
    {
        QuestionNode *newNode = new QuestionNode(accountNo, question, answer, score, round);
        if (tail)
        {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
        else
        {
            head = tail = newNode;
        }
        size++;
    }

    void pushAccount(int accountNo, string name, int scoreRound1, int scoreRound2, int scoreRound3)
    {
        AccountNode *newNode = new AccountNode(accountNo, name, scoreRound1, scoreRound2, scoreRound3);
        if (tail)
        {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
        else
        {
            head = tail = newNode;
        }
        size++;
    }

    void pushDiscardedCard(int number, string question, string answer, int score)
    {
        DiscardedCardNode *newNode = new DiscardedCardNode(number, question, answer, score);
        if (tail)
        {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
        else
        {
            head = tail = newNode;
        }
        size++;
    }

    void pop()
    {
        if (tail)
        {
            Node *temp = tail;
            tail = tail->prev;
            if (tail)
            {
                tail->next = nullptr;
            }
            else
            {
                head = nullptr;
            }
            delete temp;
            size--;
        }
    }

    void print()
    {
        cout << endl;
        Node *current = head;
        while (current)
        {
            if (ScoreNode *qNode = dynamic_cast<ScoreNode *>(current))
            {
                cout << "ScoreNode - Number: " << qNode->accountNo << ", Question: " << qNode->question << ", Score: " << qNode->score << ", Round: " << qNode->round << endl;
            }
            else if (AccountNode *aNode = dynamic_cast<AccountNode *>(current))
            {
                cout << "AccountNode - Number: " << aNode->accountNo << ", Name: " << aNode->name << ", ScoreRound1: " << aNode->scoreRound1 << ", ScoreRound2: " << aNode->scoreRound2 << ", ScoreRound3: " << aNode->scoreRound3 << endl;
            }
            current = current->next;
        }
        cout << endl;
    }

    Node *getNodeByAccountNo(int accountNo)
    {
        Node *current = head;
        while (current)
        {
            if (QuestionNode *qNode = dynamic_cast<QuestionNode *>(current))
            {
                if (qNode->accountNo == accountNo)
                {
                    return qNode;
                }
            }
            else if (AccountNode *aNode = dynamic_cast<AccountNode *>(current))
            {
                if (aNode->accountNo == accountNo)
                {
                    return aNode;
                }
            }
            current = current->next;
        }
        return nullptr;
    }

    // DiscardedCardNode *getNodeAtIndex(int index)
    //{
    //     int currentIndex = 0;
    //     DiscardedCardNode *current = dynamic_cast<DiscardedCardNode *>(getHead());

    //    while (current && currentIndex < index)
    //    {
    //        current = dynamic_cast<DiscardedCardNode *>(current->next);
    //        currentIndex++;
    //    }

    //    return current;
    //}
    DiscardedCardNode *getDiscardedCardNodeByNumber(int number)
    {
        DiscardedCardNode *current = dynamic_cast<DiscardedCardNode *>(getHead());

        while (current)
        {
            if (current->number == number)
            {
                return current;
            }
            current = dynamic_cast<DiscardedCardNode *>(current->next);
        }

        return nullptr; // Return nullptr if not found
    }

    AccountNode *searchAccountByName(const string &name)
    {
        Node *current = head;
        while (current)
        {
            if (AccountNode *aNode = dynamic_cast<AccountNode *>(current))
            {
                if (aNode->name == name)
                {
                    return aNode;
                }
            }
            current = current->next;
        }
        return nullptr;
    }

    int getNewAccountNumber()
    {
        int maxAccountNo = 0;
        Node *current = head;
        while (current)
        {
            if (AccountNode *aNode = dynamic_cast<AccountNode *>(current))
            {
                if (aNode->accountNo > maxAccountNo)
                {
                    maxAccountNo = aNode->accountNo;
                }
            }
            current = current->next;
        }
        return maxAccountNo + 1;
    }

    void deleteNode(Node *node)
    {
        if (!node)
            return;

        if (node->prev)
        {
            node->prev->next = node->next;
        }
        else
        {
            head = node->next;
        }

        if (node->next)
        {
            node->next->prev = node->prev;
        }
        else
        {
            tail = node->prev;
        }

        delete node;
    }

    vector<int> getAccountNoList()
    {
        vector<int> accountNoList;
        Node* current = head;
        while (current)
        {
            if (AccountNode* aNode = dynamic_cast<AccountNode*>(current))
            {
                accountNoList.push_back(aNode->accountNo);
            }
            current = current->next;
        }
        return accountNoList;
    }

    int getAccountNoByName(const string& name) {
        Node* current = head;
        while (current) {
            if (AccountNode* aNode = dynamic_cast<AccountNode*>(current)) {
                if (aNode->name == name) {
                    return aNode->accountNo;
                }
            }
            current = current->next;
        }
        return -1;
    }
};
