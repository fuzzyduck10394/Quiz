#include <iostream>
#include <random>
#include <time.h>
#include "quiz_class.h"
using namespace std;
/**************************************************************************************** basic **********/
bool TheSameChar(char a, char b) {
    return (a == b || int(a)+32 == int(b) || int(a)-32 == int(b));
}

/*************************************************************************************** for * main ******/
void quiz::MixQs() {
    srand(time(NULL));

    for (int i=0; i<qs.size()-1; i++) {
        int r = rand() % (qs.size()-i) + i;
        swap(qs[i], qs[r]);
    }

    // DEBUG
    /* for (auto i:qs) {
        cout << i.first << '\n' << i.second << "\n\n";
    }*/
}

bool quiz::AskQuestion(int ind) {
    cout << qs[ind].first;
    string input;
    getline(cin, input);

    input = RawString(input);
    string correct = RawString(qs[ind].second);

    if (input.size() != correct.size()) return false;

    for (int i = 0; i < input.size(); i++) {
        if (!TheSameChar(input[i], correct[i])) return false;
    }

    return true;
}


/**************************************************************************************** main ***********/
void quiz::FirstRound() {
    MixQs();
    for (int i=0; i<qs.size(); i++) {
        bool correct = AskQuestion(i);
        if (correct) {
            SCORE++;
            swap(qs[i], qs[qs.size()-1]);
            qs.pop_back();

            if (qs.empty()) {
                cout << "Brawo! Odpowiedziales dobrze na wszystkie pytania za pierwszym razem.\n";
                exit(0);
            }
        }
        else {
            SCORE--;
        }
    }
}

void quiz::Round() {

}
