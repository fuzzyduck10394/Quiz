#include <fstream>
#include <vector>
#include <queue>
using namespace std;
// TODO: rygorystyczne spacje i capitale

class quiz {
private:
    vector<pair<string, string>> qs;        // question set
    int SCORE;                              // 
    
    // build.cpp
    void BuildQue(string);                  // writes the vector<pair<string, string>qs using MakeParts() and BegParts() functions

    // algorithm.cpp
    void MixQs();                           // mixes qs's elements using rand()
    bool AskQuestion(int);                  // clears screen, asks question and returns if answer is correct

    void Round();                           // typical quiz's round
    
public:
    quiz();
    void AskParts();                        // gets from the user parts of the quiz
    void StartQuiz();                       // asks all of the questions in random order
};

