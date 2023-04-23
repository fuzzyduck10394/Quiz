#include <iostream>
#include <random>
#include <time.h>
#include <stack>
#include "quiz_class.h"
using namespace std;

struct comp;

// basic
bool IsYes(string);                     // basic.cpp
bool IsNo(string);                      // basic.cpp
bool tnInput();                         // basic.cpp
bool IsLetter(char);                    // basic.cpp
bool TheSameChar(char, char, bool);     // basic.cpp
string RawString(string);               // basic.cpp
void SetToFalse(bool*, int);            // basic.cpp
bool HasNr(string);                     // basic.cpp
void CheckExit(string, string, bool);   // basic.cpp
                                        
// for main functions
void AssignPq(int);                     // assignes pq with i*{i, 0} for i in {0, arg}
void PrintPq(priority_queue<pair<int,unsigned int>,vector<pair<int, unsigned int>>,comp>); // debug - prints priority queue's elements 

// class
// private:
//      void quiz::MixQs();                     // mixes qs's elements using rand()
//      string quiz::AskQuestion(int);          // clears screen, asks question and returns an answer
//      bool CheckAnswer(string, string);       // checks if the answer is correct
//      void quiz::Round();                     // an infinity loop
// public:
//      void quiz::StartQuiz();                 // asks all of the questions in random order and then triggers Round()



// global variables
struct comp {
    // returns true if pair<int, int> p1 should be found before pair<int, int> p2
    bool operator()(const pair<int, unsigned int>& p1,
                    const pair<int, unsigned int>& p2) const
    { return p1.second > p2.second; }
};
// sorts from the smallest to the biggest value by the second variable; allows duplications
priority_queue<pair<int, unsigned int>, vector<pair<int, unsigned int>>, comp> pq;



/***************************************************************************************** for main *******/

void AssignPq(int size) {for (int i=0; i<size; i++) pq.push({i, 0});}

// DEBUG function
void PrintPq(priority_queue<pair<int, unsigned int>, vector<pair<int, unsigned int>>, comp> pq) {
    cout << "\n----" << pq.size() << "\n";
    while(!pq.empty()) {
        cout << pq.top().first +1<< ' ' << pq.top().second << '\n';
        pq.pop();
    }
    cout << "----\n";
    cin.get();
}

/**************************************************************************************** class ***********/

/*** private ***/
void quiz::MixQs() {
    srand(time(NULL));
    for (int i=0; i<qs.size()-1; i++) {
        int r = rand() % (qs.size()-i) + i;
        swap(qs[i], qs[r]);
    }
}
                        // answer, correct
bool quiz::CheckAnswer(string a, string c) {
    if (BLANK == false) {
        a = RawString(a);
        c = RawString(c);
    }

    // don't able to do typos when answering the date
    const bool typos_allowed = (TYPOS == true && !HasNr(a) && a.size() > 5);

    int mistakes = 0, mistakes_max = 1;

    // bad length - something missing or something is too much
    if (!typos_allowed && a.size() != c.size()) return false;
    else if (a.size() + mistakes_max < c.size() || a.size() - mistakes_max > c.size()) return false;

    for (int i=0; i<a.size(); i++) {
        if (!TheSameChar(a[i], c[i], CAPIT)) {
            if (typos_allowed) {
                if (i>0 && TheSameChar(a[i-1], c[i], CAPIT)) continue;
                else if (i<a.size()-1 && TheSameChar(a[i+1], c[i], CAPIT)) continue;
                else mistakes++;
            }
            else return false;
        }

        if (mistakes > mistakes_max) return false;
    }

    return true;
}

string quiz::AskQuestion(int ind) {
    system("clear");
    cout << "SCORE: " << SCORE << '\n';

    string question = qs[ind].first;
    cout << question << '\n';

    string input;
    getline(cin, input);
    CheckExit(input, qs[ind].second, BLANK);

    return input;
}

void quiz::Round() {
    const int QS_SIZE = qs.size();                      
    const int F = max(int(0.2 * (double)QS_SIZE), 1);   // a question can't be asked if it was asked in F (20% of number of questions) rounds

    queue<int> lastf;                                   
    bool in_lastf[QS_SIZE];                             
    SetToFalse(in_lastf, QS_SIZE);

    stack<pair<int, int>> st;                           // stack used to find pq.top() which wasn't asked in last F rounds
    pair<int, int> curr;                                
    unsigned /*long long*/ int priority_v = 1;          
                                                
    while (1) {
        // shouldn't be triggered
        if (pq.empty()) {
            cout << "Błąd kolejki.\n";
            exit(0);
        }

        while (in_lastf[pq.top().first]) {
            st.push(pq.top());
            pq.pop();
        }
        curr = pq.top();        
        pq.pop();

        string answer = AskQuestion(curr.first);
        bool correct = CheckAnswer(answer, qs[curr.first].second);

        if (lastf.size() >= F) {
            in_lastf[lastf.front()] = false;
            lastf.pop();
        }
        in_lastf[curr.first] = true;
        lastf.push(curr.first);
        
        if (correct) {
            SCORE++;
            curr.second += priority_v++;
        }
        else {
            // TODO: maybe some updates for a priority of a question which isn't asked correctly?...
            SCORE--;
            
            string correct_answer = qs[curr.first].second;
            cout << correct_answer << '\n';
            string cinget;
            getline(cin, cinget);
            
            // '.' tells that the answer was correct; undo
            if (cinget.size() == 1 && cinget[0] == '.') {
                SCORE++;
                curr.second += priority_v++;
            }
        }

        pq.push(curr);
        while (!st.empty()) {
            pq.push(st.top());
            st.pop();
        }
        
        // PrintPq(pq);
    }
}


/*** public ***/
void quiz::StartQuiz() {
    MixQs();

    for (int i=0; i<qs.size(); i++) {
        string answer = AskQuestion(i);
        bool correct = CheckAnswer(answer, qs[i].second);
        if (correct) {
            SCORE++;
            if (SCORE == qs.size()) {
                cout << "\nBrawo! Odpowiedziałeś dobrze na wszystkie pytania za pierwszym razem.\n"
                        "Czy chcesz kontynuować? (t/n)\n";

                bool tn_answer = tnInput();
                if (tn_answer) {
                    AssignPq(qs.size());
                    break;
                }
                else exit(0);
            }
        }
        else {
            SCORE--;
            pq.push({i, 0}); 

            string correct_answer = qs[i].second;
            cout << correct_answer << '\n';
            string cinget;
            getline(cin, cinget);

            // '.' tells that the answer was correct; undo
            if (cinget.size() == 1 && cinget[0] == '.') {
                SCORE++;
                pq.pop();
            }
        }
    }

    Round();
}

