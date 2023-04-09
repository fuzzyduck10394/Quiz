# Quiz
## 1. Jak włączyć program?
Aby włączyć program, w terminalu w katalogu `quiz` należy wpisać następującą komendę: 
>g++ code/main.cpp code/quiz_class/algorithm.cpp code/quiz_class/build.cpp -o quiz.out && ./quiz.out

## 2. Jak stworzyć swój własny zestaw pytań?
W pliku z zestawami pytań muszą być zamieszczone pytania i odpowiedzi w formacie:
>Ile żołądków mają delfiny? -2

Aby wstawić komentarz, należy umieścić poprzedzający go znak `//`. 
>Ile żołądków mają delfiny? -2  *//Tu jest komentarz!*

Po stworzeniu pliku z zestawem pytań, należy umieścić go w katalogu `quiz` oraz wpisać jego nazwę do pliku `source.txt`.

## 3. Personalizacja 
W pierwszym wierszu pliku można zamieścić linię, która określa czy przy sprawdzaniu poprawności odpowiedzi program powinien zwrócić uwagę na znaki białe, wielkie litery i literówki (są one zawsze niedozwolone w wypadku udzielania odpowiedzi, w których znajduje się liczba - np. daty).
Linia ta powinna składać się z trzech wartości logicznych określanych symbolami `0` (fałsz) oraz `1` (prawda), np.:
>1 0 1
Wartości te określają prawdziwość kolejnych stwierdzeń:
- Przy sprawdzaniu zwróć uwagę na **znaki białe**
- Przy sprawdzaniu zwróć uwagę na **wielkie litery**
- Przy sprawdzaniu zwróć uwagę na **literówki**
W przypadku nieumieszczenia tych wartości w pliku, program domyślnie przyjmie każde z tych twierdzeń za **fałszywe**.

## 3. Dodatkowe informacje
- Program działa tylko na urządzeniach z systemem Linux.
- W wypadku popełnienia literówki w odpowiedzi na pytanie, wpisanie znaku `.` anuluje utratę punktów.
- Aby wyjść z programu, w odpowiedzi na pytanie należy wpisać `e`.
