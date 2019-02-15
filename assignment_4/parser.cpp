#include <iostream>
#include <string>
#include <vector>
#include <map>

#define vector_string vector<string>

using namespace std;

extern void yylex();
extern FILE* yyin;

//          	ACTION	                                                    GOTO
// State while	begin	end	lparn	rop	rparn	id	num	semi	aop	mop	$	W'	W	C	I	S	S'	P	E
string parseTable[30][21] = {
    {"0", "s2", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "1", " ", " ", " ", " ", " ", " "},
    {"1", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "acc", " ", " ", " ", " ", " ", " ", " ", " "},
    {"2", " ", " ", " ", "s4", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "3", " ", " ", " ", " ", " "},
    {"3", " ", "s5", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
    {"4", " ", " ", " ", " ", " ", " ", "s7", "s8", " ", " ", " ", " ", " ", " ", " ", "6", " ", " ", " ", " "},
    {"5", " ", " ", "r7", " ", " ", " ", "s12", " ", "r8", " ", " ", " ", " ", " ", " ", " ", "9", "10", "11", " "},
    {"6", " ", " ", " ", " ", "s13", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
    {"7", " ", " ", " ", " ", "r3", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
    {"8", " ", " ", " ", " ", "r4", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
    {"9", " ", " ", "s14", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
    {"10", " ", " ", "r5", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
    {"11", " ", " ", " ", " ", " ", " ", " ", " ", "s15", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
    {"12", " ", " ", " ", " ", " ", " ", " ", " ", " ", "s16", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
    {"13", " ", " ", " ", " ", " ", " ", "s18", "s19", " ", " ", " ", " ", " ", " ", " ", "17", " ", " ", " ", " "},
    {"14", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "r1", " ", " ", " ", " ", " ", " ", " ", " "},
    {"15", " ", " ", "r7", " ", " ", " ", "s12", " ", "r8", " ", " ", " ", " ", " ", " ", " ", " ", "20", "11", " "},
    {"16", " ", " ", " ", " ", " ", " ", "s23", "s24", " ", " ", " ", " ", " ", " ", " ", "22", " ", " ", " ", "21"},
    {"17", " ", " ", " ", " ", " ", "s25", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
    {"18", " ", " ", " ", " ", " ", "r3", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
    {"19", " ", " ", " ", " ", " ", "r4", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
    {"20", " ", " ", "r6", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
    {"21", " ", " ", " ", " ", " ", " ", " ", " ", "r9", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
    {"22", " ", " ", " ", " ", " ", " ", " ", " ", "r11", " ", "s26", " ", " ", " ", " ", " ", " ", " ", " ", " "},
    {"23", " ", " ", " ", " ", " ", " ", " ", " ", "r3", " ", "r3", " ", " ", " ", " ", " ", " ", " ", " ", " "},
    {"24", " ", " ", " ", " ", " ", " ", " ", " ", "r4", " ", "r4", " ", " ", " ", " ", " ", " ", " ", " ", " "},
    {"25", " ", "r2", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
    {"26", " ", " ", " ", " ", " ", " ", "s28", "s29", " ", " ", " ", " ", " ", " ", " ", "27", " ", " ", " ", " "},
    {"27", " ", " ", " ", " ", " ", " ", " ", " ", "r10", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
    {"28", " ", " ", " ", " ", " ", " ", " ", " ", "r3", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
    {"29", " ", " ", " ", " ", " ", " ", " ", " ", "r4", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "}
};

map<int, vector<int>> reducingRules;
vector<int> stateStack;
vector<int> symbolsStack;

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

int getTokenNum(char* inputToken){
    string token(inputToken);

    if(token == "while")
        return 1;
    if(token == "begin")
        return 2;
    if(token == "end")
        return 3;
    if(token == "lparn")
        return 4;
    if(token == "rop")
        return 5;
    if(token == "rparn")
        return 6;
    if(token == "id")
        return 7;
    if(token == "num")
        return 8;
    if(token == "semi")
        return 9;
    if(token == "aop")
        return 10;
    if(token == "mop")
        return 11;
    if(token == "$")
        return 12;
    if(token == (char*)"W'")
        return 13;
    if(token == "W")
        return 14;
    if(token == "C")
        return 15;
    if(token == "I")
        return 16;
    if(token == "S")
        return 17;
    if(token == "S'")
        return 18;
    if(token == "P")
        return 19;
    if(token == "E")
        return 20;
    if(token == "")
        return 100;
    
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
        exit(1);
    }

    debugLog("handleInput done!");
}

map<int, vector<int>> convertRulesToNums(map<int, vector<string>> &rules){
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
    debugLog("[+] Preparing rules");

    map<int, vector_string> tempReducingRules;
    // W' -> W
    tempReducingRules[0] = vector_string{"W'", "W"};
    // W -> while C begin S end
    tempReducingRules[1]  = vector_string{"W", "while", "C", "begin", "S", "end"};
    // C -> lparn I rop I rparn
    tempReducingRules[2] = vector_string{"C", "lparn", "I", "rop", "I", "rparn"};
    // I -> id
    tempReducingRules[3] = vector_string{"I", "id"};
    // I -> num
    tempReducingRules[4] = vector_string{"I", "num"};
    // S -> S'
    tempReducingRules[5] = vector_string{"S", "S'"};
    // S' -> P semi S'
    tempReducingRules[6] = vector_string{"S'", "P", "semi", "S'"};
    // S' -> ''
    tempReducingRules[7] = vector_string{"S'", ""};
    // P -> ''
    tempReducingRules[8] = vector_string{"P", ""};
    // P -> id aop E
    tempReducingRules[9] = vector_string{"P", "id", "aop", "E"};
    // E -> I mop I
    tempReducingRules[10] = vector_string{"E", "I", "mop", "I"};
    // E -> I
    tempReducingRules[11] = vector_string{"E", "I"};

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