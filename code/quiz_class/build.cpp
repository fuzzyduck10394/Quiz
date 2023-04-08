#include <iostream>
#include <fstream>
#include "quiz_class.h"
using namespace std;

// basic
string RawString(string);               // removes blank chars: {' ', '\n', '\t'}
string GetExactLine(int);               // getline() but for the exact line in file
bool ActiveLine(string);                // checks if the line is not a comment and is not empty
void SetToFalse(bool*, int);            // sets dynamic bool array to false
                                
// for main functions
void OpenFiles();                       // checks if source.txt and answers are able to open 
bool CorrectInput(string);              // checks if parts on input are typed correctly
pair<string, string> ToQs(string );     // divides a line into a question and it's answer
vector<int> MakeParts();                // returns size for the parts; [0] element is the size of minimum part size and [1] is a maximal size 
vector<int> BegParts();                 // returns a vector of numbers of lines in the file which are beginnings of the parts

// class
// void quiz::AskParts();               // gets from the user parts of the quiz (public) 
// void quiz::BuildQue(string);         // writes the vector<pair<string, string>qs using MakeParts() and BegParts() functions (private)

// variables
int Q = 0;                              // number of active lines in the answer file
string SRC;                             // source.txt's content
                                        


/************************************************************************************** constructor *******/
quiz::quiz() {
    SCORE = 0;
    OpenFiles();
}


/**************************************************************************************** basic ***********/
/* string RawString(string s) {
    string res = "";
    for (int i=0; i<s.size(); i++) {
        if (s[i]!=' ' && s[i]!='\n' && s[i]!='\t') res += s[i];
    }
    return res;
} */

string GetExactLine(int n) {
    ifstream file;
    file.open(SRC, ios::in);
    if (!file.is_open()) {
        cout << "Plik source.txt jest błędny.\n";
        exit(0);
    }

    string line;
    int line_counter = 0;
    while (getline(file, line)) {
        line_counter++;
        if (line_counter == n) {
            file.close();
            return line;
        } 
    }

    file.close();
    return "";
}

bool ActiveLine(string line) {
    return !((line[0] == '/' && line[1] == '/') || line.size() == 0 || line[0] == '\n'); 
}

void SetToFalse(bool* start, int size) {
    for (bool* i = start; i < start + size; i++) *i = false;
}


/***************************************************************************************** for main *******/
void OpenFiles() {
    string line;
    ifstream srcfile, file;

    srcfile.open("source.txt", ios::in);
    if (!srcfile.is_open()) {
        cout << "Plik source.txt nie istnieje.\n";
        exit(0);
    }
    getline(srcfile, SRC);
    
    file.open(SRC, ios::in);
    if (!file.is_open()) {
        cout << "Plik source.txt jest błędny.\n";
        exit(0);
    }

    while (getline(file, line)) {
        if (ActiveLine(line)) Q++;
    }

    srcfile.close();
    file.close();
}

bool CorrectInput(string s) {
    string sraw = RawString(s);
    if (sraw.size() == 0) return false;
    if (sraw[0] == 'w' && sraw.size() == 1) return true;

    bool* used = new bool[Q+1];
    SetToFalse(used, Q+1);
    s += ' '; // triggers end of a number at the end of input
    string nr = "";
    for (int i=0; i<s.size(); i++) {
        if (s[i] == ' ') {
            if (nr == "" || (nr.size() == 1 && (int)nr[0] == 0)) continue;
            if (stoi(nr) > Q || used[stoi(nr)]) {
                delete[] used;
                return false;
            }
            used[stoi(nr)] = true;
            nr = "";
        }
        else if (s[i] < '1' || s[i] > '9') {
            delete[] used;
            return false;
        }
        else nr += s[i];
    } 
    delete[] used;
    return true;
}

pair<string, string> ToQs(string s) {
    pair<string, string> res = {"", ""};

    bool answer = false;
    for (int i=0; i<s.size(); i++) {
        if (s[i] == '/' && s[i+1] == '/') break;
        else if (s[i] == '-' && !answer) answer = true;
        else if (!answer) res.first += s[i];
        else if (answer)  res.second += s[i];
    }

    if (res.second == "" || res.first == "" || RawString(res.second).size() == 0) return {"", ""};
    return res;
}

vector<int> MakeParts() {
    vector<int> res;
    
    if (Q <= 15) res.assign(2+1, Q);
    else {
        // number of parts
        const int p = Q/10 + 1;
        // minimal and maximal part sizes
        const int mini = Q/p;
        int maks = mini;

        res.assign(p+2, mini);

        int c = 0, ind = 2;
        while (p * mini + c < Q) {
            res[ind]++;
            c++; 

            if (ind == 2) maks++;
            ind++;
            if (ind > res.size()-1) ind = 2;
        }

        res[0] = mini;
        res[1] = maks;
    }
    
    // DEBUG
    // cout << "\nMakeParts: ";
    // for (auto i:res) cout << i << ' ';
    // cout << endl;
    
    return res;
}

vector<int> BegParts() {
    vector<int> parts = MakeParts();
    vector<int> res;
    res.assign(parts.size()-2, -1);

    ifstream file;
    file.open(SRC, ios::in);
    if (!file.is_open()) {
        cout << "Plik source.txt jest błędny.\n";
        exit(0);
    }

    // c -- na ktorym pytaniu jestes teraz
    // cc -- na ktorym pytaniu jestes w partii
    // ind -- numer partii
    int c = 0, cc = 0, ind = 0;
    string line;
    while (getline(file, line)) {
        if (ActiveLine(line)) {
            c++;
            if (cc == 0) {
                res[ind] = c;
            }
            cc++;
            if (cc == parts[ind+2]) {
                cc = 0;
                ind++;
            }
        }
    }

    res.push_back(c+1);

    // DEBUG
    // cout << "BegParts:" << endl;
    // for (auto i:res) cout << '\t' << i << "\n";

    file.close();
    return res;
}


/**************************************************************************************** class ***********/
void quiz::AskParts() {
    vector<int> parts = MakeParts();
    string input = "1";
    const int P_SIZE = parts.size()-2;

    if (P_SIZE == 1) {
        const int Q_SIZE = parts[0];
        cout << "Quiz nie został podzielony na części. "; 
        if (Q_SIZE == 1) cout << "Jest 1 pytanie.\n";
        else if (Q_SIZE > 1 && Q_SIZE <= 4) cout << "Są " << Q_SIZE << " pytania.\n";
        else if (Q_SIZE >= 5 && Q_SIZE <= 21) cout << "Jest " << Q_SIZE << " pytań.\n";
        // SĄ 22 pytania, lecz wielkość jednej partii to maksymalnie 15 pytań

        cout << "Zaczynamy?\n";
        string cinget;
        getline(cin, cinget);
    }
    else {
        cout << "Quiz został podzielony na " << P_SIZE << " części po " << parts[0];
        if(parts[0] != parts[1]) cout << "-" << parts[1];
        cout << " pytań. \nKtóre z części chcesz dzisiaj przećwiczyć? Jeśli chcesz przećwiczyć "
                "wszystkie z nich, wpisz 'w'.\n";

        getline(cin, input);
        while (!CorrectInput(input)) {
            cout << "Wpisz 'w' lub oddzielone od siebie spacją części quizu.\n";
            getline(cin, input);
        }
    }
    BuildQue(input);
}

void quiz::BuildQue(string input) {
    vector<int> parts = MakeParts();
    vector<int> beg_parts = BegParts();

    string nr = "";
    input += ' ';
    for (int i=0; i<input.size(); i++) {
        if (input[i] == ' ') {
            if (nr == "") continue;
            
            const int begin_p = beg_parts[stoi(nr)-1];
            const int end_p = beg_parts[stoi(nr)];

            for (int i=begin_p; i<end_p; i++) {
                if (ActiveLine(GetExactLine(i))) {
                    pair<string, string> to_qs = ToQs(GetExactLine(i));
                    if (to_qs.first != "") qs.push_back(to_qs);
                }
            }
            nr = "";
        }
        else nr += input[i];
    }
    // DEBUG
    // for (auto i:qs) cout << i.first << '\n' << i.second << '\n' << '\n';
}

