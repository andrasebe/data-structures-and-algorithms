[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/3SRI_-8J)
[![Open in Codespaces](https://classroom.github.com/assets/launch-codespace-2972f46106e565e64193e422d61a12cf1da4916b45550586e14ef0a7c637dd04.svg)](https://classroom.github.com/open-in-codespaces?assignment_repo_id=23595005)
# Proiect PA 2025

Repository-ul conține scheletul proiectului și testele publice.

## Structură

`include/`
Headerele pentru modulele cerute.

`src/`
Fișierele pe care trebuie să le completați.

`tests/public/`
Teste publice pentru fiecare pas. Aici găsiți fișierele de intrare și ieșirea așteptată.

`build/`
Conține binarele pentru rularea testelor publice:
- `build/public_pas1`
- `build/public_pas2`
- `build/public_pas3`
- `build/public_pas4`

## Rulare teste publice

Pentru a rula toate testele publice:

```bash
make public-test
```

Pentru a rula testele publice doar pentru un pas:

```bash
./build/public_pas1 tests/public/pas1/entitati.csv tests/public/pas1/relatii.csv tests/public/pas1/interogari.txt
./build/public_pas2 tests/public/pas2/entitati.csv tests/public/pas2/relatii.csv tests/public/pas2/interogari.txt
./build/public_pas3 tests/public/pas3/entitati.csv tests/public/pas3/relatii.csv tests/public/pas3/interogari.txt
./build/public_pas4 tests/public/pas4/entitati.csv tests/public/pas4/relatii.csv tests/public/pas4/interogari.txt
```

Fiecare test public are și un fișier `expected.txt` cu rezultatul așteptat.

## Teste publice și teste private

Există două tipuri de teste:
- teste publice, incluse în acest repository;
- teste private, folosite la evaluare.

Faptul că testele publice trec nu garantează punctaj maxim.

## Explicații

graph_add_node: prin această funcție reușesc să adaug un nou nod în graf. atunci când se depășește capacitatea, tabloul se dublează, garantând astfel O(1) amortizat /inserare. Dacă aș fi extins cu +1, atunci s-ar fi produs O(n^2) pentru n noduri.

EXISTS<entitate> : verifică dacă entitatea există în graf.
EDGE<sursă><destinatie> : verifica daca exista muchie directa.
NEIGHBORS<entitate> : afiseaza vecinii directi.
PATH<sursa><destinatie> : drum minim ca numar de muchii (BFS).
DIJKSTRA<sursa><destinatie> : drumul minim ca, cost

graph_add_edge: muchiile sunt inserate la finalul listei pentru a pastra ordinea de inserare din fisierul de intrare (astfel se afiseaza corect NEIGHBORS); parcurg toata lista de muchii a sursei pana la capat; O(grad(src))

bst_insert (construieste indexul)/ bst_search (foloseste indexul) : am ales un BST indexat, deoarece toate interogarile necesita localizarea unei entitati dupa nume inainte de orice operatie. Fara BST, fiecare interogare ar face o scanare liniara O(n), astfel, folosind BST, cautarea devine O(log n) prin comparatii lexicografice stanga-dreapta. Nodurile BST stocheaza doar un pointer spre GraphNode fara duplicarea datelor.

queue_enqueue/ queue_dequeue: lista inlantuita permite crestere dinamica FARA realloc, deoarece nr de interogari din fisier nu este cunoscut la momentul alocarii. mentinerea pointerilor front si rear garanteaza ca operatiile sunt O(1) (nu se parcurge lista). coada este folosita in process_all_queries ca buffer intermediar. buffer-ul garanteaza ca mai intai am construit tot graful, iar mai apoi raspund la intrebari. queue_dequeue returneaza pointerul la date, iar process_all_queries apeleaza free(linie) dupa procesare

heap_push : insereaza perechea (node_id, dist) la finalul tabloului si apeleaza sift_up pentru a reastaura proprietatea de min-heap (fiecare nod este mai mic sau egal decat copiii sai).

heap_pop : extrage minimul, muta ultimul element pe pozitia 0 si apeleaza sift_down; returneaza mereu nodul cu distanta minima

sift_up : urca elementul nou inserat si il compara cu parintele pana cand heap-ul este valid; sift_down: coboara elementul mutat pe pozitia 0 si il inlocuieste cu cel mai mic copil; ambele sunt O(log n)

process_path_bfs : gaseste drumul cu nr minim de muchii intre 2 noduri folosind visited[] (nodurile vizitate) si parent[] (vectorul prin care reconstruiesc calea); complexitate O(n+m) n-nr noduri, m-nr muchii (algortimul viziteaza fiecare nod si fiecare muchie O DATA); foloseste un array cu front/rear in loc de lista inlantuita, deoarece evita alocarile dinamice /element

am scris de la 0 functia print_path pentru a respecta limita de 40 linii si pentru ca nu exista o functie reutilizabila care sa construiasca si sa afiseze o cale din vectorul parent[]

process_dijkstra : gaseste drumul cu cost minim intre 2 noduri folosind dist[] (distante minime) si parinte[] (vectorul prin care reconstruiesc calea). atunci cand se gaseste o distanta mai buna pentru nod nu se modifica elementul vechi din heap, ci se insereaza un element nou cu distanta actualizata. elementele depasite sunt ignorate prin tehnica "lazy deletion" care elimina necesitatea unui index invers si simplifica implementarea fata de decrease_key; complexitate O((n+m) log n)

print_dijkstra_path: am scris aceasta functie pentru a reconstrui si afisa calea astfel incat sa respect limita de 40 linii 
/functie

str_to_relation_type : functia este apelata la citirea fisierului de intrare; transforma sirul "works_at" in enum-ul WORKS_AT pentru a putea apela graph_add_edge. functia relation_type_to_str este apelata la afisare in graph_print pentru a transforma enum-ul inapoi in sir lizibil

graph_free : parcurge fiecare nod din graf si elibereaza lista sa de muchii nod cu nod, numele entitatii, tabloul de noduri si structura Graph

bst_free : elibereaza memoria ocupata de nodurile BST printr-o traversare recursiva post-ordine: frunzele sunt eliberate primele, urcand treptat spre radacina; astfel, niciun nod nu este accesat dupa ce a fost eliberat. aceasta functie elibereaza doar structura de indexare, deoarece datele entitatilor apartin grafului si sunt gestionate de graph_free.