#include <bits/stdc++.h>
#define MAX_INSTRUCTION_LENGTH 1024
#define CAPACITY 500
#define ASSIGN "assign"
#define PRINT "print"
#define BEGIN "begin"
#define END "end"

using namespace std;

class Map
{
private:
    pair<string, int> *arr;
    int size;
    int capacity;

public:
    Map(int);
    int hash(string);
    bool search(string);
    bool insert(string, int);
    int getValue(string);
};

Map ::Map(int cap = CAPACITY) : capacity(cap)
{

    size = 0;
    arr = new pair<string, int>[capacity];
    for (int i = 0; i < capacity; i++)
        arr[i] = {"", INT_MIN};
}

int Map ::hash(string key)
{
    int hashValue = 0;
    int factor = 3;
    for (auto ch : key)
    {
        hashValue = (hashValue + ch * factor) % capacity;
        factor *= 3;
    }
    return hashValue;
}

bool Map ::search(string key)
{
    int h = hash(key);
    int i = h;

    while (arr[i].first != "")
    {
        if (arr[i].first == key)
            return true;
        i = (i + 1) % capacity;
        if (i == h)
            return false;
    }
    return false;
}

bool Map::insert(string key, int val)
{
    if (size == capacity)
        return false;

    int i = hash(key);

    while (arr[i].first != "" && arr[i].first != key)
        i = (i + 1) % capacity;

    if (arr[i].first == key)
    {
        cout << "'" << key << "' already exists" << endl;
        return false;
    }
    else
    {
        arr[i] = {key, val};
        size++;
        return true;
    }
}

int Map ::getValue(string key)
{
    int h = hash(key);
    return arr[h].second;
}

typedef struct Scope
{
    Map symbolTable;
    Scope *enclosingScope;
} Scope;

Scope *createScope()
{
    Scope *newScope = new Scope;
    newScope->enclosingScope = nullptr;
    return newScope;
}

string fetchVariableName(char *line, char *instruction)
{
    char *p = strstr(line, instruction);
    p = p + strlen(instruction) + 1;

    string name = "";
    while (*p != '\0' && *p != ' ')
        name.push_back(*(p++));

    return name;
}

int fetchVariableValue(char *line)
{
    int i = strlen(line) - 1;
    while (line[i] != ' ')
        i--;
    return stoi(line + i);
}

int main()
{
    FILE *sourceFile = fopen("sourceProgram.txt", "r");
    char line[MAX_INSTRUCTION_LENGTH];
    Scope *currScope = NULL;

    while (fgets(line, sizeof(line), sourceFile))
    {
        line[strlen(line) - 1] = '\0';

        if (strstr(line, BEGIN))
        {
            Scope *newScope = createScope();
            newScope->enclosingScope = currScope;
            currScope = newScope;
        }
        else if (strstr(line, ASSIGN))
            currScope->symbolTable.insert(fetchVariableName(line, (char *)ASSIGN), fetchVariableValue(line));
        else if (strstr(line, PRINT))
        {
            string varName = fetchVariableName(line, (char *)PRINT);
            Scope *scope = currScope;

            while (scope && scope->symbolTable.search(varName) == false)
                scope = scope->enclosingScope;

            if (scope)
                cout << varName << " = " << scope->symbolTable.getValue(varName) << endl;
            else
            {
                cout << "error: "
                     << "'" << varName << "'"
                     << " was not declared in this scope" << endl;
            }
        }
        else
        {
            Scope *deadScope = currScope;
            currScope = currScope->enclosingScope;
            delete deadScope;
        }
    }
    fclose(sourceFile);
    return 0;
}
