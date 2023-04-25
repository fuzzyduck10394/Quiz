#include <iostream>
#include <random>
#include <time.h>
#include <stack>
#include <unordered_set>
#include "quiz_class.h"
using namespace std;

struct comp;
// TODO: polskie znaki
// basic.cpp
bool IsYes(string);
bool IsNo(string);
bool tnInput();
bool IsLetter(char);
bool IsBlank(char);
bool TheSameChar(char, char, bool);
string RawString(string);
void SetToFalse(bool*, int);
bool HasNr(string);
void CheckExit(string, string, bool);
string LowerCase(string);

// for main functions
void AssignPq(int);                                   // assignes pq with i*{i, 0} for i in {0, arg}
void PrintPq();
unordered_set<string> DivideIntoWords(string);        // returns a vector of words and it's size
bool isK(string);                                     // checks if there are "K/" in the line
unordered_set<string> DivideIntoWords(string, bool);  // divides into words by ','

// class
// private:
//      void quiz::MixQs();                     // mixes qs's elements using rand()
//      string quiz::AskQuestion(int);          // clears screen, asks question and returns an answer
//      short CheckAnswer(string, string);      // checks if the answer is correct, returns -1 if the correct answer has already been shown
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
void PrintPq() {

    priority_queue<pair<int, unsigned int>, vector<pair<int, unsigned int>>, comp> pq1;
    pq1 = pq;
    cout << "\n----\n" << pq.size() << "\n";
    while(!pq.empty()) {
        cout << pq.top().first +1<< " , " << pq.top().second << '\n';
        pq.pop();
    }
    pq = pq1;
    cout << "\n----\n";
    cin.get();
}

/**************************************************************************************** class ***********/

/*** private ***/
void quiz::MixQs() {
    srand(time(NULL));
    for (int i=0; i<qs.size()-2; i++) {
        int r = rand() % (qs.size()-i) + i;
        swap(qs[i], qs[r]);
    }
    // for (auto i : qs) cout << i.first << endl;
}

unordered_set<string> DivideIntoWords(string s, bool CAPIT) {
    // TODO: typos
    unordered_set<string> res;
    string word = "";

    if (CAPIT) s = LowerCase(s);
    for (int i=0; i<s.size(); i++) {
        if ((s[i] == ',') && RawString(word).size() != 0) {
            res.insert(word);
            word = "";
        }
        if (i == s.size() -1 && RawString(word).size() != 0) {
            word += s[i];
            res.insert(word);
            word = "";
        }
        if (!IsBlank(s[i]) && s[i] != ',') word += s[i];
    }
    res.insert("end");
    return res;
}
                        // answer, correct
short quiz::CheckAnswer(string a, string c, bool isK = false) {
    // wymienianie odpowiedzi - mozliwa jest dowolna kolejnosc
    if (isK) {
        // TODO: wypisywanie poprawnych odpowiedzi w odwrotnej kolejnosci
        unordered_set<string> a_div = DivideIntoWords(a, CAPIT);
        unordered_set<string> c_div = DivideIntoWords(c, CAPIT);

        // uwaga, naiwne rozwiazanie. TODO
        unordered_set<string> UC_a_div = DivideIntoWords(a, false);
        unordered_set<string> UC_c_div = DivideIntoWords(c, false);

        vector<string> not_correct, not_present;

        for (auto i : c_div) {
            if (i == "end") continue;
            else if (a_div.find(i) == a_div.end()) not_present.push_back(i);
        }
        
        for (auto i : a_div) {
            if (i == "end") continue;
            else if (c_div.find(i) == c_div.end()) not_correct.push_back(i);
        }

        if (not_present.size() == c_div.size()) return false;
        else if (not_present.size() != 0 || not_correct.size() != 0){
            cout << "\nBrakuje: ";
            if (not_present.size() != 0) {
                for (int i=0; i<not_present.size(); i++) {
                    cout << not_present[i];
                    if (i != not_present.size() - 1) cout << ", ";
                }
            }
            if (not_correct.size() != 0) {
                cout << "\nŹle:\t ";
                for (int i=0; i<not_correct.size(); i++) {
                    cout << not_correct[i];
                    if (i != not_correct.size() - 1) cout << ", ";
                }
                cout << '\n';
            }

            return -1;
        }
        else return true;
    }

    else if (BLANK == false) {
        a = RawString(a);
        c = RawString(c);
    }

    // don't able to do typos when answering the date
    const bool typos_allowed = (TYPOS == false && !HasNr(a) && a.size() > 5);

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
    if (question[0] == 'K' && question[1] == '/') question.erase(0,3);
    cout << question << '\n';

    string input;
    getline(cin, input);
    CheckExit(input, qs[ind].second, BLANK);


    return input;
}

bool isK(string question) {
    question = RawString(question);
    if (question.size() >= 2 && question[0] == 'K' && question[1] == '/') return true;
    else return false;
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
        string corr_ans = qs[curr.first].first;
        short correct = CheckAnswer(answer, qs[curr.first].second, isK(corr_ans));

        if (lastf.size() >= F) {
            in_lastf[lastf.front()] = false;
            lastf.pop();
        }
        in_lastf[curr.first] = true;
        lastf.push(curr.first);
        
        if (correct == 1) {
            SCORE++;
            curr.second += priority_v++;
        }
        else {
            // TODO: maybe some updates for a priority of a question which isn't asked correctly?...
            SCORE--;
            
            string correct_answer = qs[curr.first].second;
            // TODO: cout bez poczatkowych spacji czyli np zamiast " leszczynski" to "leszczynski"
            if (correct != -1) cout << correct_answer << '\n';
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
    }
}


/*** public ***/
void quiz::StartQuiz() {
    MixQs();

    for (int i=0; i<qs.size(); i++) {
        string answer = AskQuestion(i);
        short correct = CheckAnswer(answer, qs[i].second, isK(qs[i].first));
        if (correct == 1) {
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
            if (correct != -1) cout << correct_answer << '\n';
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

