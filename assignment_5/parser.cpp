#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <ctype.h>

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
vector<int> stateStack;
vector<int> symbolsStack;
map<string, int> numToken;

bool debugStatus = (getenv("DEBUG")?true:false);

void dumpVector(vector<int> &x){
    for(int i=0;i<x.size();i++){
        printf("%d ",x.at(i));
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

    int nextState = atoi(parseTable[stateStack.back()][symbolsStack.back()].c_str());
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

    while(reduceCount--){
        int symbolTop, ruleTop, lastState;
        // Pop states
        lastState = stateStack.back();
        stateStack.pop_back();

        // Pop symbols and match with reduction rule        
        symbolTop = symbolsStack.back();
        symbolsStack.pop_back();

        ruleTop = currRule.back();
        currRule.pop_back();

        if(ruleTop == getTokenNum("")){ // Nullable symbol
            // Put the symbol back
            // And append nullable symbol at top of symbols stack
            // ^ done after while rule
            symbolsStack.push_back(symbolTop);
            // Also put the state back
            stateStack.push_back(lastState);
            break;
        }
        if(symbolTop != ruleTop){
            printf("Error in reducing.");
            printf("Error occured at token: `%s`\n", yytext);
            exit(1);
        }
    }
    debugLog("Popping done");

    symbolsStack.push_back(currRule[0]);
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
    symbolsStack.push_back(tokenNum);
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

void prepareRules(){
    debugLog("Preparing rules");

    map<int, vector_string> tempReducingRules;
    // E' -> E
    tempReducingRules[0] = vector_string{"E'", "E"};
    // E -> E add T
    tempReducingRules[1] = vector_string{"E", "E", "add", "T"};
    // E -> E sub T
    tempReducingRules[2] = vector_string{"E", "E", "sub", "T"};
    // E -> T
    tempReducingRules[3] = vector_string{"E", "T"};
    // T -> T mul F
    tempReducingRules[4] = vector_string{"T", "T", "mul", "F"};
    // T -> T div F
    tempReducingRules[5] = vector_string{"T", "T", "div", "F"};
    // T -> F
    tempReducingRules[6] = vector_string{"T", "F"};
    // F -> num
    tempReducingRules[7] = vector_string{"F", "num"};
    // F -> sub F
    tempReducingRules[8] = vector_string{"F", "sub", "F"};
    // F -> lparn E rparn
    tempReducingRules[9] = vector_string{"F", "lparn", "E", "rparn"};

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