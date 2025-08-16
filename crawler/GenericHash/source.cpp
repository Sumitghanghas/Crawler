// #include "../stringhandling.h"
//--------Node constructor--------//
template <typename v, typename k>
Node<v, k>::Node(v ke, k value)
{
    key = ke;
    data = value;
    next = nullptr;
}

//--------HashTable constructor--------//
template <typename v, typename k>
Hash<v, k>::Hash(int s)
{
    size = s;
    capacity = 0;
    table = new Node<v, k> *[size];
    for (int i = 0; i < size; ++i)
    {
        table[i] = nullptr;
    }
}


template <typename v, typename k>
void Hash<v, k>::insert(v key, k value)
{
    if (get(key) != nullptr) {
        return;  
    }

    int index = hashFunction(key);

    if ((capacity * 100) / size >= thrashold)
    {
        resize();
        index = hashFunction(key); 
    }

    Node<v, k> *newNode = new Node<v, k>(key, value);
    newNode->next = table[index];
    table[index] = newNode;

    capacity++;
}


//-----Remove the value--------//
template <typename v, typename k>
void Hash<v, k>::remove(v key, k value)
{
    int index = hashFunction(key);
    Node<v, k> *temp = table[index];
    Node<v, k> *prev = nullptr;

    while (temp != nullptr && temp->data != value)
    {
        prev = temp;
        temp = temp->next;
    }

    if (temp == nullptr)
    {
        return;
    }

    if (prev == nullptr)
    {
        table[index] = temp->next;
    }
    else
    {
        prev->next = temp->next;
    }

    delete temp;
    capacity--;
}

//--------Search the value--------//
template <typename v, typename k>
int Hash<v, k>::search(v key, k value)
{
    int index = hashFunction(key);
    Node<v, k> *temp = table[index];
    while (temp != nullptr && temp->data != value)
    {
        temp = temp->next;
    }
    if (temp != nullptr && temp->data == value)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

//--------Display the table--------//
template <typename v, typename k>
void Hash<v, k>::display()
{
    for (int i = 0; i < size; i++)
    {
        cout << "[" << i << "]: ";
        Node<v, k> *temp = table[i];
        while (temp != nullptr)
        {
            cout << "(" << temp->key << ", " << temp->data << ") -> ";
            temp = temp->next;
        }
        cout << "NULL" << endl;
    }
}

//--------HashTable Destructor--------//
template <typename v, typename k>
Hash<v, k>::~Hash()
{
    cout << "Decostructure call" << endl;
    for (int i = 0; i < size; i++)
    {
        Node<v, k> *current = table[i];
        while (current != nullptr)
        {
            Node<v, k> *prev = current;
            current = current->next;
            delete prev;
        }
    }
    delete[] table;
}

//------Resize function------//
template <typename v, typename k>
void Hash<v, k>::resize()
{
    int oldSize = size;
    size *= 2;
    Node<v, k> **oldTable = table;
    table = new Node<v, k> *[size];

    for (int i = 0; i < size; i++)
    {
        table[i] = nullptr;
    }

    capacity = 0;

    for (int i = 0; i < oldSize; i++)
    {
        Node<v, k> *current = oldTable[i];
        while (current)
        {
            insert(current->key, current->data);
            Node<v, k> *Delete = current;
            current = current->next;
            delete Delete;
        }
    }

    delete[] oldTable;
}
//------copy Constructor------//
template <typename v, typename k>
Hash<v, k>::Hash(const Hash<v, k> &other)
{
    size = other.size;
    capacity = other.capacity;
    thrashold = other.thrashold;

    table = new Node<v, k> *[size];
    for (int i = 0; i < size; i++)
    {
        table[i] = nullptr;
        Node<v, k> *otherCurrent = other.table[i];
        Node<v, k> **current = &table[i];

        while (otherCurrent)
        {
            *current = new Node<v, k>(otherCurrent->key, otherCurrent->data);
            current = &((*current)->next);
            otherCurrent = otherCurrent->next;
        }
    }
}

//------- operator overload------//
template <typename v, typename k>
Hash<v, k> &Hash<v, k>::operator=(const Hash<v, k> &other)
{
    if (this == &other)
    {
        return *this;
    }

    for (int i = 0; i < size; ++i)
    {
        Node<v, k> *current = table[i];
        while (current)
        {
            Node<v, k> *temp = current;
            current = current->next;
            delete temp;
        }
    }
    delete[] table;

    size = other.size;
    capacity = other.capacity;
    thrashold = other.thrashold;

    table = new Node<v, k> *[size];
    for (int i = 0; i < size; i++)
    {
        table[i] = nullptr;
        Node<v, k> *otherCurrent = other.table[i];
        Node<v, k> **Current = &table[i];

        while (otherCurrent)
        {
            *Current = new Node<v, k>(otherCurrent->key, otherCurrent->data);
            Current = &((*Current)->next);
            otherCurrent = otherCurrent->next;
        }
    }

    return *this;
}

template <typename v, typename k>
Node<v, k> *Hash<v, k>::get(v key)
{
    int index = hashFunction(key);
    Node<v, k> *temp = table[index];
    while (temp != nullptr)
    {
        if (strcampare(temp->key, key) == 0)
        {
            return temp;
        }
        temp = temp->next;
    }
    return nullptr;
}


template <typename v, typename k>
int Hash<v, k>::getSize()
{
    return size;
}

template <typename v, typename k>
Node<v, k> *Hash<v, k>::getNode(int index)
{
    if (index >= 0 && index < size)
    {
        return table[index];
    }
    return nullptr;
}
