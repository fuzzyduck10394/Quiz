# Quiz
### 1. Jak włączyć program?
Aby włączyć program, w terminalu w katalogu `quiz` należy wpisać następującą komendę: 
>g++ code/main.cpp code/quiz_class/algorithm.cpp code/quiz_class/build.cpp -o quiz.out && ./quiz.out

### 2. Jak stworzyć swój własny zestaw pytań?
W pliku z zestawami pytań muszą być zamieszczone pytania i odpowiedzi w formacie:
>Ile żołądków mają delfiny? -2

Aby program działał poprawnie, należy przestrzegać kilku zasad:
- (na razie) program nie obsługuje znaków polskich w systemie Windows,
- do każdego pytania musi być załączona odpowiedź poprzedzona znakiem `-`. W przeciwnym wypadku program je zignoruje.

Aby wstawić komentarz, należy umieścić poprzedzający go znak `//`. 
>Ile żołądków mają delfiny? -2  *//Tu jest komentarz!*

Po stworzeniu pliku z zestawem pytań, należy umieścić go w katalogu `quiz` oraz wpisać jego nazwę do pliku `source.txt`.

### 3. Dodatkowe informacje
- Odpowiadając na pytania, nie należy martwić się o znaki białe i wielkie litery. Można wyłączyć tę opcję... 
