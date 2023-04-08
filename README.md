# Quiz
### 1. Jak to działa?
Aby korzystać z programu, w terminalu w katalogu `quiz` należy wpisać następującą komendę: 
>g++ code/*.cpp code/quiz_class/*.cpp -o quiz.out && ./quiz.out

### 2. Jak stworzyć swój własny zestaw pytań?
W pliku z zestawami pytań muszą być zamieszczone pytania i odpowiedzi w formacie:
>Ile żołądków mają delfiny? -2

Aby program działał poprawnie, należy przestrzegać kilku zasad:
- (na razie) program nie obsługuje znaków polskich w systemie Windows,
- do każdego pytania musi być załączona odpowiedz poprzedzona znakiem `-`. W innym wypadku program je zignoruje.

Aby wstawić komentarz, należy umieścić **na początku** linii znak `/`. Na tę chwilę program nie obsługuje następującego formatu komentarza:
>Ile żołądków mają delfiny? -2  */ Źle umieszczony komentarz!*

Po stworzeniu pliku z zestawem pytań, należy umieścić go w katalogu `quiz` oraz wpisać jego nazwę do pliku `source.txt`.

### 3. Dodatkowe informacje
- Odpowiadając na pytania, nie należy martwić się o znaki białe i wielkie litery. Można wyłączyć tę opcję... 
