#include <iostream>
#include <fstream>
#include "quiz_class.h"
using namespace std;

quiz::quiz() {
    SCORE = 0;
    OpenFiles();
}



/**************************************************************************************** basic **********/
string quiz::GetExactLine(int n) {
    ifstream file;
    file.open(SRC, ios::in);
    if (!file.is_open()) {
        cout << "Plik SOURCE.txt jest bledny.\n";
        exit(0);
    }
    string line;
    int counter = 0;
    while (getline(file, line)) {
        counter++;
        if (counter == n) {
          file.close();
          return line;
        } 
    }
    file.close();
    return "";
}

string RawString(string s) {
    string res = "";
    for (int i=0; i<s.size(); i++){
        if (s[i]!=' ' && s[i]!='\n' && s[i]!='\t') res += s[i];
    }
    return res;
}

bool quiz::ActiveLine(string line) {
    return !((line[0] == '/' && line[1] == '/') || line.size()==0 || line[0] == '\n'); 
}

pair<string, string> quiz::ToQs(string s) {
    pair<string, string> res = {"", ""};

    bool answer = false;
    for (int i=0; i<s.size(); i++) {
        if (i > 0 && s[i] == '/' && s[i-1] == '/') break;
        else if (s[i] == '-' && !answer) answer = true;
        else if (!answer) res.first += s[i];
        else if (answer)  res.second += s[i];
    }

    if (res.second == "" || res.first == "" || RawString(res.second).size() == 0) return {"", ""};
    return res;
}

void SetToFalse(bool* start, int size) {
    for(bool* i = start; i<start+size; i++) *i = false;
}




/*************************************************************************************** for * main ******/
bool quiz::CorrectInput(string s) {
    s += ' ';
    string sraw = RawString(s);
    if (sraw.size() == 0) return false;
    if (sraw[0] == 'w' && sraw.size() == 1) return true;

    bool* used = new bool[Q+1];
    SetToFalse(used, Q+1);
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
        else nr += sraw[i];
    } 
    delete[] used;
    return true;
}

vector<int> quiz::BegParts() {
    ifstream file;
    vector<int> parts = MakeParts();
    vector<int> res;
    res.assign(parts.size()-2, -1);
    file.open(SRC, ios::in);
    if (!file.is_open()) {
        cout << "Plik SOURCE.txt jest bledny.\n";
        exit(0);
    }

    // c -- na ktorym pytaniu jestes teraz
    // cc -- na ktorym pytaniu jestes w partii
    // ind -- numer partii
    int c=0, cc=0, ind=0;
    string line;
    while(getline(file, line)) {
        if (ActiveLine(line)) {
            c++;
            if (cc==0) {
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




/**************************************************************************************** main ***********/
void quiz::OpenFiles() {
    string line;
    ifstream srcfile, file;

    srcfile.open("SOURCE.txt", ios::in);
    if (!srcfile.is_open()) {
        cout << "Plik SOURCE.txt nie istnieje.\n";
        exit(0);
    }
    getline(srcfile, SRC);
    
    file.open(SRC, ios::in);
    if (!file.is_open()) {
        cout << "Plik SOURCE.txt jest bledny.\n";
        exit(0);
    }

    Q = 0;
    while (getline(file, line)) {
        if (ActiveLine(line)) Q++;
    }

    srcfile.close();
    file.close();
}

vector<int> quiz::MakeParts() {
    vector<int> res;

    if (Q <= 15) res.assign(2+1, Q);
    else {
        // number of parts
        const int p = Q/10 + 1;
        // minimal and maximal part size
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

void quiz::AskParts() {
    vector<int> parts = MakeParts();
    cout << "Quiz zostal podzielony na " << parts.size() - 2 << " czesci po " << parts[0];

    if(parts[0] != parts[1]) cout << "-" << parts[1];

    cout << " pytan. \nKtore z czesci chcesz dzisiaj przecwiczyc? Jezeli chcesz przecwiczyc "
            "wszystkie z nich, wpisz 'w'.\n";

    string input;
    getline(cin, input);

    while (!CorrectInput(input)) {
        cout << "Wpisz 'w' lub oddzielone od siebie spacja czesci quizu.\n";
        getline(cin, input);
    }

    BuildQue(input);
}

void quiz::BuildQue(string input) {
    vector<int> parts = MakeParts();
    vector<int> beg_parts = BegParts();
    input += ' ';
   
    string nr = "";
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

