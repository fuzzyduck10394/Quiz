#include <iostream>
#include <random>
#include <time.h>
#include <stack>
#include "quiz_class.h"
using namespace std;

// basic
string RawString(string);               // removes blank chars: {' ', '\n', '\t'}
bool IsLetter(char);                    // tells if a char is a letter
bool TheSameChar(char, char);           // tells if chars are the same including capital letters

void SetToFalse(bool*, int);            // in build.cpp; set a dynamic bool array to false

// for main functions
short tnInput(string);                  // called when all of the questions were answered correctly in the first turn; returns -1 if the input is not correct, 1 if the user agreed or 0 if the user disagreed
void AssignPq(int);                     // assignes pq with i*{i, 0} for i in {0, arg}

// main


// class
// private:
//      void quiz::MixQs();             // mixes qs's elements using rand()
//      bool quiz::AskQuestion(int);    // clears screen, asks question and returns if answer is correct
//      void quiz::Round();             // typical quiz's round
// public:
//      void quiz::StartQuiz();         // asks all of the questions in random order

// variables
struct comp {
    // returns true if pair<int, int> p1 should be found before pair<int, int> p2
    bool operator()(const pair<int, unsigned int>& p1,
                    const pair<int, unsigned int>& p2) const
    {
        return p1.second > p2.second;
        
    }
};
// sorts from the smallest to the biggest value by the second variable; allows duplications
priority_queue<pair<int, unsigned int>, vector<pair<int, unsigned int>>, comp> pq;



/**************************************************************************************** basic **********/
string RawString(string s) {
    string res = "";
    for (int i=0; i<s.size(); i++){
        if (s[i] != ' ' && s[i] != '\n' && s[i] != '\t') res += s[i];
    }
    return res;
}

bool IsLetter(char a) {
    return ((a >= 'a' && a <= 'z') || (a >= 'A' && a <= 'Z'));
}

bool TheSameChar(char a, char b) {
    if (IsLetter(a) && IsLetter(b)) return (a == b || int(a)+32 == int(b) || int(a)-32 == int(b));
    else return (a == b);
}


/*************************************************************************************** for  main ******/
short tnInput(string input) {
    input = RawString(input);
    if (input == "tak" || input == "t") return 1;
    else if (input == "nie" || input == "n") return 0;
    else return -1;
}

void AssignPq(int size) {
    for (int i=0; i<size; i++) pq.push({i, 0});
}

void PrintPq(priority_queue<pair<int, unsigned int>, vector<pair<int, unsigned int>>, comp> pq) {
    cout << "\n----" << pq.size() << "\n";
    while(!pq.empty()) {
        cout << pq.top().first +1<< ' ' << pq.top().second << '\n';
        pq.pop();
    }
    cout << "----\n";
    cin.get();
}

/**************************************************************************************** main ***********/



/**************************************************************************************** class ***********/
/*** private ***/
void quiz::MixQs() {
    srand(time(NULL));
    for (int i=0; i<qs.size()-1; i++) {
        int r = rand() % (qs.size()-i) + i;
        swap(qs[i], qs[r]);
    }
}

bool quiz::AskQuestion(int ind) {
    system("clear");
    cout << "SCORE: " << SCORE << '\n';

    cout << qs[ind].first << '\n';
    string input;
    getline(cin, input);

    input = RawString(input);
    string correct = RawString(qs[ind].second);

    if (input.size() != correct.size()) return false;

    for (int i=0; i<input.size(); i++) {
        if (!TheSameChar(input[i], correct[i])) return false;
    }

    return true;
}

void quiz::Round() {
    const int QS_SIZE = qs.size();              // const qs.size()
    const int F = int(0.2 * (double)QS_SIZE);   // a question can't be asked if it was asked in F (20% of number of questions) rounds
    // DEBUG
    // cout << "F: " << F << '\n';
    // cin.get();

    queue<int> lastf;                           // lastf queue
    bool in_lastf[QS_SIZE];                     // tells if element is present in the lastf queue
    SetToFalse(in_lastf, QS_SIZE);

    stack<pair<int, int>> st;                   // stack used to find pq.top() which wasn't asked in last F rounds
    pair<int, int> curr;                        // a temporary pq.top()
    unsigned /*long long*/ int priority_v = 1;                // is added to a correctly answered question
                                                
                                                

    while (1) {
        // shouldn't be triggered
        if (pq.empty()) {
            cout << "Błąd kolejki.\n";
            exit(0);
        }

        // start - chooses a question from pq
        while (in_lastf[pq.top().first]) {
            st.push(pq.top());
            pq.pop();
        }
        curr = pq.top();        
        pq.pop();

        // asking question and update it's atributes
        bool correct = AskQuestion(curr.first);

        if (lastf.size() >= F) {
            in_lastf[lastf.front()] = false;
            lastf.pop();
        }
        in_lastf[curr.first] = true;
        lastf.push(curr.first);
        
        
        // checking if an answer is correct and updating the question's priority in pq
        if (correct) {
            curr.second += priority_v;
            SCORE++;
            priority_v++;
        }
        else {
            // TODO: maybe some updates for a priority of a question which isn't asked correctly?...
            SCORE--;
    
            cout << qs[curr.first].second << '\n';
            string cinget;
            getline(cin, cinget);
        }

        // end - adding a changed element to the pq back again
        pq.push(curr);
        while (!st.empty()) {
            pq.push(st.top());
            st.pop();
        }
        
        // DEBUG
        // PrintPq(pq);
    }
}



/*** public ***/
void quiz::StartQuiz() {
    MixQs();

    for (int i=0; i<qs.size(); i++) {
        bool correct = AskQuestion(i);
        if (correct) {
            SCORE++;
            if (SCORE == qs.size()) {
                cout << "\nBrawo! Odpowiedziałeś dobrze na wszystkie pytania za pierwszym razem.\n"
                     << "Czy chcesz kontynuować? (t/n)\n";

                string input;
                getline(cin, input);
                while (tnInput(input) == -1) {
                    cout << "Wpisz 'tak' lub 'nie'.\n";
                    getline(cin, input);
                }

                if (tnInput(input) == 1) {
                    AssignPq(qs.size());
                    break;
                }
                else if (tnInput(input) == 0) exit(0);
            }
        }
        else {
            SCORE--;
            pq.push({i, 0}); 
            cout << qs[i].second << '\n';
            string cinget;
            getline(cin, cinget);
        }
    }

    Round();
}

