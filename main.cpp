#include <iostream>
#include <stack>

using std::cout;
using std::endl;
using std::string;

static string expression[9] = {"S->E", "E->E+T", "E->E-T", "E->T", "T->T*F", "T->T/F", "T->F", "F->(E)", "F->num"};

int step = 0;
std::stack<string> symbol;
std::stack<int> state;
std::stack<string> input;

int gotoSheet[16][3] = {{1,  2,  3},
                        {-1, -1, -1},
                        {-1, -1, -1},
                        {-1, -1, -1},
                        {10, 2,  3},
                        {-1, -1, -1},
                        {-1, 11, 3},
                        {-1, 12, 3},
                        {-1, -1, 13},
                        {-1, -1, 14},
                        {-1, -1, -1},
                        {-1, -1, -1},
                        {-1, -1, -1},
                        {-1, -1, -1},
                        {-1, -1, -1},
                        {-1, -1, -1}};

//横坐标：  *   /   (     )   num   +   -   $
string actionSheet[16][8] = {{"",   "",   "S4", "",    "S5", "",   "",   ""},//0
                             {"",   "",   "",   "",    "",   "S6", "S7", "ACC"},//1
                             {"S8", "S9", "",   "R3",  "",   "R3", "R3", "R3"},//2
                             {"R6", "R6", "",   "R6",  "",   "R6", "R6", "R6"},//3
                             {"",   "",   "S4", "",    "S5", "",   "",   ""},//4
                             {"R8", "R8", "R8", "R8",  "R8", "R8", "R8", "R8"},//5
                             {"",   "",   "S4", "",    "S5", "",   "",   ""},//6
                             {"",   "",   "S4", "",    "S5", "",   "",   ""},//7
                             {"",   "",   "S4", "",    "S5", "",   "",   ""},//8
                             {"",   "",   "S4", "",    "S5", "",   "",   ""},//9
                             {"",   "",   "",   "S15", "",   "S6", "S7", ""},//10
                             {"S8", "S9", "",   "R1",  "",   "R1", "R1", "R1"},//11
                             {"S8", "S9", "",   "R2",  "",   "R2", "R2", "R2"},//12
                             {"R4", "R4", "R4", "R4",  "",   "R4", "R4", "R4"},//13
                             {"R5", "R5", "R5", "R5",  "",   "R5", "R5", "R5"},//14
                             {"R7", "R7", "R7", "R7",  "R7", "R7", "R7", "R7"}//15
};//"E->E+T", "E->E-T", "E->T", "T->T*F", "T->T/F", "T->F", "F->(E)", "F->num"

void Reduce(int expression_) {
    switch (expression_) {
        case 1: {//E->E+T
            for (int i = 0; i < 3; i++) {
                symbol.pop();
                state.pop();
            }
            symbol.push("E");
            if (gotoSheet[state.top()][0] == -1)
                throw "Goto表中对应项为空";
            state.push(gotoSheet[state.top()][0]);

            break;
        }
        case 2: {//E->E-T
            for (int i = 0; i < 3; i++) {
                symbol.pop();
                state.pop();
            }
            symbol.push("E");
            if (gotoSheet[state.top()][0] == -1)
                throw "Goto表中对应项为空";
            state.push(gotoSheet[state.top()][0]);

            break;
        }
        case 3: {//E->T
            symbol.pop();
            state.pop();
            symbol.push("E");
            if (gotoSheet[state.top()][0] == -1)
                throw "Goto表中对应项为空";
            state.push(gotoSheet[state.top()][0]);

            break;
        }
        case 4: {//T->T*F
            for (int i = 0; i < 3; i++) {
                symbol.pop();
                state.pop();
            }
            symbol.push("T");
            if (gotoSheet[state.top()][1] == -1)
                throw "Goto表中对应项为空";
            state.push(gotoSheet[state.top()][1]);

            break;
        }
        case 5: {//T->T/F
            for (int i = 0; i < 3; i++) {
                symbol.pop();
                state.pop();
            }
            symbol.push("T");
            if (gotoSheet[state.top()][1] == -1)
                throw "Goto表中对应项为空";
            state.push(gotoSheet[state.top()][1]);

            break;
        }
        case 6: {//T->F
            symbol.pop();
            state.pop();
            if (gotoSheet[state.top()][1] == -1)
                throw "Goto表中对应项为空";
            symbol.push("T");
            state.push(gotoSheet[state.top()][1]);

            break;
        }
        case 7: {//F->(E)
            for (int i = 0; i < 3; i++) {
                symbol.pop();
                state.pop();
            }
            symbol.push("F");
            if (gotoSheet[state.top()][2] == -1)
                throw "Goto表中对应项为空";
            state.push(gotoSheet[state.top()][2]);

            break;
        }
        case 8: {//F->num
            symbol.pop();
            state.pop();
            if (gotoSheet[state.top()][2] == -1)
                throw "Goto表中对应项为空";
            symbol.push("F");
            state.push(gotoSheet[state.top()][2]);

            break;
        }

        default: {

        }
    }
}

void printSymbol() {
    std::stack<string> temp = symbol;
    int t = temp.size();
    for (int i = 0; i < t; i++) {
        cout << temp.top() << " ";
        temp.pop();
    }
}

void printState() {
    std::stack<int> temp = state;
    int t = temp.size();
    for (int i = 0; i < t; i++) {
        cout << temp.top() << " ";
        temp.pop();
    }
}

void printInput() {
    std::stack<string> temp = input;
    int t = temp.size();
    for (int i = 0; i < t; i++) {
        cout << temp.top() << " ";
        temp.pop();
    }
}

void output(int flag, string info) {
    if (flag == 0) {
        cout << step << "\t\tShift " << info << "\t\t\t" << "[";
        printSymbol();
        cout << "]\t\t\t" << "[ ";
        printState();
        cout << "]\t\t\t[ ";
        printInput();
        cout << "]" << endl;
    } else if (flag == 1) {
        cout << step << "\t\tReduce " << info << "\t\t\t" << "[";
        printSymbol();
        cout << "]\t\t\t" << "[ ";
        printState();
        cout << "]\t\t\t[ ";
        printInput();
        cout << "]" << endl;
    }
}

void run(string in) {
    //构造待分析终结符号栈
    std::stack<string> t;

    for (int i = 0; i < in.length(); i++) {
        if (in[i] == 'n') {
            t.push("num");
            i += 2;
        } else
            t.push(string{in[i]});
    }
    t.push("$");
    while (!t.empty()) {
        input.push(t.top());
        t.pop();
    }

    //准备开始工作

    state.push(0);
    symbol.push("$");
    cout << "步骤\t\t\t动作\t\t\t\t符号栈\t\t\t\t状态栈\t\t\t待扫描符号栈" << endl;
    while (!input.empty()) {
        step++;
        if (step == 10) {
            int foo = 0;//point
        }
        int nowState = state.top();
        if (input.top() == "*") {
            string nowAction = actionSheet[nowState][0];
            if (nowAction == "") {
                throw "Action表中对应动作为空";
            } else if (nowAction[0] == 'S') {

                string t = string{nowAction[1]} + string{nowAction[2]};
                state.push(atoi(t.c_str()));
                symbol.push(input.top());

                input.pop();
                output(0, t);
            } else if (nowAction[0] == 'R') {
                string t = string{nowAction[1]} + string{nowAction[2]};
                Reduce(atoi(t.c_str()));
                output(1, expression[atoi(t.c_str())]);
            }
        } else if (input.top() == "/") {
            string nowAction = actionSheet[nowState][1];
            if (nowAction == "") {
                throw "Action表中对应动作为空";
            } else if (nowAction[0] == 'S') {

                string t = string{nowAction[1]} + string{nowAction[2]};
                state.push(atoi(t.c_str()));
                symbol.push(input.top());

                input.pop();
                output(0, t);
            } else if (nowAction[0] == 'R') {
                string t = string{nowAction[1]} + string{nowAction[2]};
                Reduce(atoi(t.c_str()));
                output(1, expression[atoi(t.c_str())]);
            }
        }
            //    +   -   $
        else if (input.top() == "(") {
            string nowAction = actionSheet[nowState][2];
            if (nowAction == "") {
                throw "Action表中对应动作为空";
            } else if (nowAction[0] == 'S') {

                string t = string{nowAction[1]} + string{nowAction[2]};
                state.push(atoi(t.c_str()));
                symbol.push(input.top());

                input.pop();
                output(0, t);
            } else if (nowAction[0] == 'R') {
                string t = string{nowAction[1]} + string{nowAction[2]};
                Reduce(atoi(t.c_str()));
                output(1, expression[atoi(t.c_str())]);
            }
        } else if (input.top() == ")") {
            string nowAction = actionSheet[nowState][3];
            if (nowAction == "") {
                throw "Action表中对应动作为空";
            } else if (nowAction[0] == 'S') {

                string t = string{nowAction[1]} + string{nowAction[2]};
                state.push(atoi(t.c_str()));
                symbol.push(input.top());

                input.pop();
                output(0, t);
            } else if (nowAction[0] == 'R') {
                string t = string{nowAction[1]} + string{nowAction[2]};
                Reduce(atoi(t.c_str()));
                output(1, expression[atoi(t.c_str())]);
            }
        } else if (input.top() == "num") {
            string nowAction = actionSheet[nowState][4];
            if (nowAction == "") {
                throw "Action表中对应动作为空";
            } else if (nowAction[0] == 'S') {

                string t = string{nowAction[1]} + string{nowAction[2]};
                state.push(atoi(t.c_str()));
                symbol.push(input.top());
                output(0, t);
                input.pop();
            } else if (nowAction[0] == 'R') {
                string t = string{nowAction[1]} + string{nowAction[2]};
                Reduce(atoi(t.c_str()));
                output(1, expression[atoi(t.c_str())]);
            }
        } else if (input.top() == "+") {
            string nowAction = actionSheet[nowState][5];
            if (nowAction == "") {
                throw "Action表中对应动作为空";
            } else if (nowAction[0] == 'S') {

                string t = string{nowAction[1]} + string{nowAction[2]};
                state.push(atoi(t.c_str()));
                symbol.push(input.top());

                input.pop();
                output(0, t);
            } else if (nowAction[0] == 'R') {
                string t = string{nowAction[1]} + string{nowAction[2]};
                Reduce(atoi(t.c_str()));
                output(1, expression[atoi(t.c_str())]);
            }
        } else if (input.top() == "-") {
            string nowAction = actionSheet[nowState][6];
            if (nowAction == "") {
                throw "Action表中对应动作为空";
            } else if (nowAction[0] == 'S') {

                string t = string{nowAction[1]} + string{nowAction[2]};
                state.push(atoi(t.c_str()));
                symbol.push(input.top());

                input.pop();
                output(0, string{nowAction[1]});
            } else if (nowAction[0] == 'R') {
                string t = string{nowAction[1]} + string{nowAction[2]};
                Reduce(atoi(t.c_str()));
                output(1, expression[atoi(t.c_str())]);
            }
        } else if (input.top() == "$") {
            string nowAction = actionSheet[nowState][7];
            if (nowAction == "") {
                throw "Action表中对应动作为空";
            } else if (nowAction[0] == 'S') {

                string t = string{nowAction[1]} + string{nowAction[2]};
                state.push(atoi(t.c_str()));
                symbol.push(input.top());

                input.pop();
                output(0, string{nowAction[1]});
            } else if (nowAction[0] == 'R') {
                string t = string{nowAction[1]} + string{nowAction[2]};
                Reduce(atoi(t.c_str()));
                output(1, expression[atoi(t.c_str())]);
            } else if (nowAction == "ACC") {
                cout << step << "\t\t\tACC\t\t\t结束分析！该句子符合文法规范！！！！" << endl;
                input.pop();
            }
        }
    }

}

int main() {
        run("(num-num)+(num/num)+(num*num)");

    return 0;
}