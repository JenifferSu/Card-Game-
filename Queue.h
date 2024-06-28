template <typename T>
class QueueNode
{
public:
    T data;
    QueueNode* next;

    QueueNode(T value) : data(value), next(nullptr) {}
};

template <typename T>
class Queue
{
private:
    QueueNode<T>* front;
    QueueNode<T>* rear;

public:
    Queue() : front(nullptr), rear(nullptr) {}

    ~Queue()
    {
        while (!isEmpty())
        {
            dequeue();
        }
    }


    // FIFO
    // First In First Out
    void enqueue(T value)
    {
        QueueNode<T>* newNode = new QueueNode<T>(value);
        if (isEmpty())
        {
            front = rear = newNode;
        }
        else
        {
            rear->next = newNode;
            rear = newNode;
        }
    }

    void dequeue()
    {
        if (isEmpty())
        {
            std::cerr << "Queue is empty. Cannot dequeue.\n";
            return;
        }
        QueueNode<T>* temp = front;
        front = front->next;
        delete temp;
        if (front == nullptr)
        {
            rear = nullptr; // Queue is empty now
        }
    }

    T peek()
    {
        if (isEmpty())
        {
            std::cerr << "Queue is empty. Cannot peek.\n";
            return T();
        }
        return front->data;
    }

    bool isEmpty()
    {
        return front == nullptr;
    }
};
