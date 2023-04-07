#include <fstream>
#include <vector>
using namespace std;

class quiz {
    vector<pair<string, string>> qs;        // question set
    int Q;                                  // number of active lines in file
    string SRC;                             // srcfile's content
    fstream srcfile, file;                  // 
         

    /****************************GETTING STARTED****************************/
    /*basic*/
    string GetExactLine(int);               // getline but for the exact line in file
    string RawString(string);               // removes blank chars
    bool ActiveLine(string);                // checks if the line is not a comment and not empty
    pair<string, string> ToQs(string);      // partitioning line into a question and it's answer
    void SetToFalse(bool*, int);            // sets dynamic bool aray to false
                                        
    /*for main*/
    bool CorrectInput(string);              // checks if numbers of parts are typed correctly
    vector<int> BegParts();                 // returns a vector of lines numbers in the file for parts beginning
    vector<int> MakeParts();                // math function: returns sizes for the parts
                                       
    /*main*/
    void OpenFiles();                       // fstream srcfile, called by constructor
    void BuildQue(string);                  // writes the qs vector using MakeParts() and BegParts()
    
public:
    quiz();
    void AskParts();                        // UI, gets parts
    vector<pair<string, string>> GetQs();   // 
                                            
    void TestFun();
};
