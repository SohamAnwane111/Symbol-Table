#include <bits/stdc++.h>
#include <stdio.h>
#define MAX_INSTRUCTION_LENGTH 1024
#define ASSIGN "assign"
#define PRINT "print"
#define BEGIN "begin"
#define END "end"

using namespace std;

typedef struct Scope
{
    unordered_map<string, int> symbolTable;
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
        {
            string varName = fetchVariableName(line, (char *)ASSIGN);
            currScope->symbolTable[varName] = fetchVariableValue(line);
        }
        else if (strstr(line, PRINT))
        {
            string varName = fetchVariableName(line, (char *)PRINT);
            Scope *scope = currScope;

            while (scope && scope->symbolTable.count(varName) == 0)
                scope = scope->enclosingScope;

            if (scope)
                cout << varName << " = " << scope->symbolTable[varName] << endl;
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