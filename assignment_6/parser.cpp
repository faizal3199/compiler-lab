#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <ctype.h>
#include "terminalProperty.cpp"

#define vector_string vector<string>
extern char* yytext;

using namespace std;

extern void yylex();
extern FILE* yyin;

// State	ACTION	                                GOTO
//         add	sub	mul	div	num	lparn	rparn	$	E'	E	T	F
string parseTable[34][13] = {
    {"0", " ", "s5", " ", " ", "s4", "s6", " ", " ", " ", "1", "2", "3"},
    {"1", "s7", "s8", " ", " ", " ", " ", " ", "acc", " ", " ", " ", " "},
    {"2", "r3", "r3", "s9", "s10", " ", " ", " ", "r3", " ", " ", " ", " "},
    {"3", "r6", "r6", "r6", "r6", " ", " ", " ", "r6", " ", " ", " ", " "},
    {"4", "r7", "r7", "r7", "r7", " ", " ", " ", "r7", " ", " ", " ", " "},
    {"5", " ", "s5", " ", " ", "s4", "s6", " ", " ", " ", " ", " ", "11"},
    {"6", " ", "s16", " ", " ", "s15", "s17", " ", " ", " ", "12", "13", "14"},
    {"7", " ", "s5", " ", " ", "s4", "s6", " ", " ", " ", " ", "18", "3"},
    {"8", " ", "s5", " ", " ", "s4", "s6", " ", " ", " ", " ", "19", "3"},
    {"9", " ", "s5", " ", " ", "s4", "s6", " ", " ", " ", " ", " ", "20"},
    {"10", " ", "s5", " ", " ", "s4", "s6", " ", " ", " ", " ", " ", "21"},
    {"11", "r8", "r8", "r8", "r8", " ", " ", " ", "r8", " ", " ", " ", " "},
    {"12", "s23", "s24", " ", " ", " ", " ", "s22", " ", " ", " ", " ", " "},
    {"13", "r3", "r3", "s25", "s26", " ", " ", "r3", " ", " ", " ", " ", " "},
    {"14", "r6", "r6", "r6", "r6", " ", " ", "r6", " ", " ", " ", " ", " "},
    {"15", "r7", "r7", "r7", "r7", " ", " ", "r7", " ", " ", " ", " ", " "},
    {"16", " ", "s16", " ", " ", "s15", "s17", " ", " ", " ", " ", " ", "27"},
    {"17", " ", "s16", " ", " ", "s15", "s17", " ", " ", " ", "28", "13", "14"},
    {"18", "r1", "r1", "s9", "s10", " ", " ", " ", "r1", " ", " ", " ", " "},
    {"19", "r2", "r2", "s9", "s10", " ", " ", " ", "r2", " ", " ", " ", " "},
    {"20", "r4", "r4", "r4", "r4", " ", " ", " ", "r4", " ", " ", " ", " "},
    {"21", "r5", "r5", "r5", "r5", " ", " ", " ", "r5", " ", " ", " ", " "},
    {"22", "r9", "r9", "r9", "r9", " ", " ", " ", "r9", " ", " ", " ", " "},
    {"23", " ", "s16", " ", " ", "s15", "s17", " ", " ", " ", " ", "29", "14"},
    {"24", " ", "s16", " ", " ", "s15", "s17", " ", " ", " ", " ", "30", "14"},
    {"25", " ", "s16", " ", " ", "s15", "s17", " ", " ", " ", " ", " ", "31"},
    {"26", " ", "s16", " ", " ", "s15", "s17", " ", " ", " ", " ", " ", "32"},
    {"27", "r8", "r8", "r8", "r8", " ", " ", "r8", " ", " ", " ", " ", " "},
    {"28", "s23", "s24", " ", " ", " ", " ", "s33", " ", " ", " ", " ", " "},
    {"29", "r1", "r1", "s25", "s26", " ", " ", "r1", " ", " ", " ", " ", " "},
    {"30", "r2", "r2", "s25", "s26", " ", " ", "r2", " ", " ", " ", " ", " "},
    {"31", "r4", "r4", "r4", "r4", " ", " ", "r4", " ", " ", " ", " ", " "},
    {"32", "r5", "r5", "r5", "r5", " ", " ", "r5", " ", " ", " ", " ", " "},
    {"33", "r9", "r9", "r9", "r9", " ", " ", "r9", " ", " ", " ", " ", " "}
};


map<int, vector<int>> reducingRules;
map<int, Variable* (*)(vector<Variable*> &)> callbackPointers;
vector<int> stateStack;
vector<pair<int, Variable*>> symbolsStack;
map<string, int> numToken;

bool debugStatus = (getenv("DEBUG")?true:false);

void dumpVector(vector<int> &x){
    for(int i=0;i<x.size();i++){
        printf("%d ",x.at(i));
    }
    printf("\n");
}
void dumpVector(vector<pair<int, Variable*>> &x){
    for(int i=0;i<x.size();i++){
        printf("%d ",x.at(i).first);
    }
    printf("\n");
}

void debugLog(char* x){
    if(debugStatus){
        printf("[+] %s\n",x);
    }
}

void handleInput(char*);

bool checkAllLower(string token){
    for(int i=0;i<token.length();i++){
        if(!islower(token.at(i))){
            return false;
        }
    }
    return true;
}

void generateNumsFromRules(map<int, vector<string>> &rules){
    debugLog("Generating nums for token in rules");
    int index = 1;

    // Insert all terminals
    for(const auto &pair: rules){
        for(int i=0;i<pair.second.size();i++){
            if (pair.second.at(i) == ""){
                continue;
            }
            if(checkAllLower(pair.second.at(i))){
                auto temp = numToken.insert({pair.second.at(i), index});

                if(temp.second){
                    string tmp = "Inserting `"+pair.second.at(i) + "` at index: " + to_string(index);
                    debugLog((char *)tmp.c_str());
                    index++;
                }
            }
        }
    }

    numToken.insert({"$", index});
    index++;

    // Insert all non terminals
    // Consider only the left term of the production rule
    for(const auto &pair: rules){
        auto temp = numToken.insert({pair.second.at(0), index});

        if(temp.second){
            string tmp = "Inserting `"+pair.second.at(0) + "` at index: " + to_string(index);
            debugLog((char *)tmp.c_str());
            index++;
        }
    }

    numToken.insert({"", index});
    debugLog("Nums generated for tokens");
}

int getTokenNum(char* inputToken){
    string token(inputToken);

    auto index = numToken.find(inputToken);
    if (index != numToken.end()){
        return index->second;
    }
    
    printf("Invalid Token: `%s`\n",inputToken);
    exit(1);
}

void reduceState(){
    if(debugStatus){
        printf("[+] stateStack: ");
        dumpVector(stateStack);

        printf("[+] symbolsStack: ");
        dumpVector(symbolsStack);
    }

    int nextState = atoi(parseTable[stateStack.back()][symbolsStack.back().first].c_str());
    stateStack.push_back(nextState);
}

void applyReduceRule(int ruleNumber, char* value){
    debugLog("Starting reduction");
    // Copy vector in case assignment is by address
    vector<int> currRule(reducingRules[ruleNumber]);
    debugLog("Vector copy created");

    int reduceCount = currRule.size() - 1;

    if(debugStatus){
        printf("[+] Popping %d states and input.\n", reduceCount);
    }

    vector<Variable*> argumentHolder;
    while(reduceCount--){
        int ruleTop, lastState;
        pair<int, Variable*> symbolTop;

        // Pop states
        lastState = stateStack.back();
        stateStack.pop_back();

        // Pop symbols and match with reduction rule        
        symbolTop = symbolsStack.back();
        argumentHolder.push_back(symbolTop.second);
        symbolsStack.pop_back();

        ruleTop = currRule.back();
        currRule.pop_back();

        if(ruleTop == getTokenNum("")){ // Nullable symbol
            // Put the symbol back
            // And append nullable symbol at top of symbols stack
            // ^ done after while rule
            symbolsStack.push_back(symbolTop);
            argumentHolder.pop_back();
            // Also put the state back
            stateStack.push_back(lastState);
            break;
        }
        if(symbolTop.first != ruleTop){
            printf("Error in reducing.");
            printf("Error occured at token: `%s`\n", yytext);
            exit(1);
        }
    }
    debugLog("Popping done");

    reverse(argumentHolder.begin(), argumentHolder.end());
    symbolsStack.push_back(make_pair(currRule[0], callbackPointers[ruleNumber](argumentHolder)));
    debugLog("Reduced symbol inserted");
    

    reduceState();
    debugLog("States updated");

    // Call hanldeInpupt again with same value. Because yylex() will shift to next input
    // And in reducing input is not consumed;
    // NOTE: Possibility of infinite loop . Check here if TLE occurs
    debugLog("Calling handleInput again");
    handleInput(value);
}

void changeState(int tokenNum, int newState){
    stateStack.push_back(newState);
    Variable* temp = new Variable();
    
    temp->add<string>("text", string(yytext));

    symbolsStack.push_back(make_pair(tokenNum, temp));
}

void handleInput(char* value){// At index 1
    debugLog("Handle input called with :");
    debugLog(value);

    int currState = stateStack.back();
    int tokenNum = getTokenNum(value);
    string temp = parseTable[currState][tokenNum];
    char* nextAction = (char *)temp.c_str();

    debugLog("Next Action:");
    debugLog(nextAction);

    if(nextAction[0] == 'r'){
        debugLog("Reducing...");
        nextAction++;
        int reductionRule = atoi(nextAction);
        
        if(debugStatus){
            printf("[+] Reducing using rule %d.\n", reductionRule);
        }
        
        applyReduceRule(reductionRule, value); // Pass 'value' for passback
    }
    else if(nextAction[0] == 's'){
        debugLog("Changing state");
        nextAction++;
        changeState(tokenNum, atoi(nextAction));
    }
    else if(string(nextAction) == "acc"){
        printf("Accepted\n");
        printf("Value: %d\n", symbolsStack.back().second->get<int>("val"));
        exit(0);
    }
    else{
        printf("Error: No action found\n");
        printf("Error occured at token: `%s`\n", yytext);
        exit(1);
    }

    debugLog("handleInput done!");
}

void convertRulesToNums(map<int, vector<string>> &rules){
    debugLog("Converting rules...");
    for(const auto &pair: rules){
        vector<int> tmp;
        for(int i=0;i<pair.second.size();i++)
            tmp.push_back(getTokenNum((char*)pair.second.at(i).c_str()));
        
        reducingRules[pair.first] = vector<int>(tmp);
    }
    debugLog("Rules converted");
}

// Reducing callbacks
Variable* callback_r0(vector<Variable*> &x){
    Variable* retVal = new Variable();
    
    retVal->add<int>("val", x[0]->get<int>("val"));

    return retVal;
}

Variable* callback_r1(vector<Variable*> &x){
    Variable* retVal = new Variable();
   
    int temp = x[0]->get<int>("val") + x[2]->get<int>("val");
    retVal->add<int>("val", temp);

    return retVal;
}

Variable* callback_r2(vector<Variable*> &x){
    Variable* retVal = new Variable();
    
    int temp = x[0]->get<int>("val") - x[2]->get<int>("val");
    retVal->add<int>("val", temp);

    return retVal;
}

Variable* callback_r4(vector<Variable*> &x){
    Variable* retVal = new Variable();
    
    int temp = x[0]->get<int>("val") * x[2]->get<int>("val");
    retVal->add<int>("val", temp);

    return retVal;
}

Variable* callback_r5(vector<Variable*> &x){
    Variable* retVal = new Variable();
    
    int temp = x[0]->get<int>("val") / x[2]->get<int>("val");
    retVal->add<int>("val", temp);

    return retVal;
}

Variable* callback_r7(vector<Variable*> &x){
    Variable* retVal = new Variable();

    int val = atoi(x[0]->get<string>("text").c_str());
    retVal->add<int>("val", val);
    
    return retVal;
}

Variable* callback_r8(vector<Variable*> &x){
    Variable* retVal = new Variable();

    int temp = x[1]->get<int>("val");
    retVal->add<int>("val", -temp);

    return retVal;
}

Variable* callback_r9(vector<Variable*> &x){
    Variable* retVal = new Variable();
    
    int temp = x[1]->get<int>("val");
    retVal->add<int>("val", temp);

    return retVal;
}

void prepareRules(){
    debugLog("Preparing rules");

    Variable* (*callback_proto)(vector<Variable*> &);

    map<int, vector_string> tempReducingRules;
    // E' -> E
    tempReducingRules[0] = vector_string{"E'", "E"};
    callback_proto = &callback_r0;
    callbackPointers.insert({0, callback_proto});

    // E -> E add T
    tempReducingRules[1] = vector_string{"E", "E", "add", "T"};
    callback_proto = &callback_r1;
    callbackPointers.insert({1, callback_proto});

    // E -> E sub T
    tempReducingRules[2] = vector_string{"E", "E", "sub", "T"};
    callback_proto = &callback_r2;
    callbackPointers.insert({2, callback_proto});

    // E -> T
    tempReducingRules[3] = vector_string{"E", "T"};
    callback_proto = &callback_r0;
    callbackPointers.insert({3, callback_proto});

    // T -> T mul F
    tempReducingRules[4] = vector_string{"T", "T", "mul", "F"};
    callback_proto = &callback_r4;
    callbackPointers.insert({4, callback_proto});

    // T -> T div F
    tempReducingRules[5] = vector_string{"T", "T", "div", "F"};
    callback_proto = &callback_r5;
    callbackPointers.insert({5, callback_proto});

    // T -> F
    tempReducingRules[6] = vector_string{"T", "F"};
    callback_proto = &callback_r0;
    callbackPointers.insert({6, callback_proto});

    // F -> num
    tempReducingRules[7] = vector_string{"F", "num"};
    callback_proto = &callback_r7;
    callbackPointers.insert({7, callback_proto});

    // F -> sub F
    tempReducingRules[8] = vector_string{"F", "sub", "F"};
    callback_proto = &callback_r8;
    callbackPointers.insert({8, callback_proto});

    // F -> lparn E rparn
    tempReducingRules[9] = vector_string{"F", "lparn", "E", "rparn"};
    callback_proto = &callback_r9;
    callbackPointers.insert({9, callback_proto});

    generateNumsFromRules(tempReducingRules);
    convertRulesToNums(tempReducingRules);
    debugLog("Rules prepared");
}

/* The trick is to define yywrap in lex file itself */
// int yywrap(){
//     return 1;
// }

int main(int argc, char** argv) {
    if(argc != 2){
        printf("Usage: %s <file-path>\n",argv[0]);
        exit(1);
    }

    prepareRules();
    stateStack.push_back(0); // Push intial state;

    debugLog("Opening file");
    yyin = fopen(argv[1], "r");
    debugLog("File opened");

    debugLog("Parsing started");
    yylex();
    debugLog("Parsing done");
}
