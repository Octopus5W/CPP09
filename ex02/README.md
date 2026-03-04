# ex02 - PmergeMe

## Objectif du sujet

Trier une séquence d'entiers positifs passée en arguments, en appliquant un **merge-insert sort**
(algorithme de **Ford-Johnson**), puis comparer le temps de traitement entre:

- `std::vector`
- `std::list`

Le programme affiche:

- la séquence avant tri (`Before`)
- la séquence après tri (`After`)
- le temps de traitement pour `vector` et `list` en microsecondes

---

## Compilation et exécution

Depuis le dossier `ex02`:

```bash
make
./PmergeMe 3 5 9 7 4
```

Exemple de test avec beaucoup d'éléments:

```bash
./PmergeMe $(seq 1000 -1 1)
```

---

## Format d'entrée

Le programme attend des entiers strictement positifs.

Cas invalides:

- token non numérique (`a`, `12x`, etc.)
- valeur `<= 0`
- dépassement de `INT_MAX`

En cas d'erreur, le programme affiche `Error` et retourne `1`.

---

## Principe de l'algorithme Ford-Johnson

L'idée est de réduire le nombre de comparaisons d'insertion en procédant par étapes.

### 1. Former des paires

On regroupe les éléments deux par deux.
Pour chaque paire `(x, y)`:

- `b = min(x, y)` (à gauche)
- `a = max(x, y)` (à droite)

Donc chaque paire vérifie `a >= b`.

Si la taille est impaire, le dernier élément est gardé de côté (`odd`).

### 2. Trier récursivement les `a`

On extrait tous les `a` puis on les trie avec le même algorithme.
Ce tri crée la base de la **chaîne principale**.

### 3. Réassocier les `b` aux `a` triés

Après tri des `a`, on reconstruit la séquence des `b` correspondante, en gérant correctement les doublons.

### 4. Construire la chaîne principale

La chaîne contient d'abord les `a` triés.
Chaque `a_i` reçoit un label de paire pour retrouver sa position plus tard.

### 5. Insérer les `b` selon l'ordre Jacobsthal

- `b1` est inséré en premier.
- Les autres `b` sont insérés selon l'ordre dérivé de Jacobsthal (et non pas simplement 2,3,4,...).
- Chaque `b_i` est inséré par `lower_bound` dans l'intervalle `[begin, position(a_i))`.

La borne `position(a_i)` est valide car `b_i <= a_i`.

### 6. Insérer l'élément impair

S'il existe un `odd`, il est inséré en dernier dans toute la chaîne.

Le résultat final est la séquence triée.

---

## Structure des fichiers

- `main.cpp`
  - vérifie les arguments
  - appelle `PmergeMe::run(argc, argv)`

- `PmergeMe.hpp`
  - déclaration de la classe
  - helpers parse + Ford-Johnson (`vector` et `list`)

- `PmergeMe.cpp`
  - parsing robuste des entrées
  - implémentation Ford-Johnson sur `std::vector<int>`
  - implémentation Ford-Johnson sur `std::list<int>`
  - mesure de temps (`std::clock`) et affichage

---

## Pourquoi `list` peut être très lente

Même avec un algorithme correct, la version `std::list` peut devenir très lente sur de grosses tailles
(ex: `100000`) car:

- nombreuses recherches linéaires (`find label`, borne d'insertion)
- insertions fréquentes dans la chaîne
- mauvaise localité mémoire par rapport à `vector`

C'est normal dans ce sujet: le but est justement de comparer les conteneurs et leur comportement réel.

---

## Complexité (pratique)

- Le Ford-Johnson optimise le nombre de comparaisons théoriques.
- L'implémentation concrète avec insertions dynamiques entraîne un coût pratique élevé.
- Sur grandes tailles, on observe un comportement proche de `O(n^2)` dans cette version.

Donc:

- pour la validation du sujet: OK
- pour du benchmark "gros volume": attendu d'être lent, surtout côté `list`

---

## Commandes utiles de test

Test simple:

```bash
./PmergeMe 3 5 9 7 4
```

Test déjà trié:

```bash
./PmergeMe 1 2 3 4 5 6 7 8
```

Test inverse:

```bash
./PmergeMe $(seq 1000 -1 1)
```

Mesurer sans bruit console:

```bash
time ./PmergeMe $(seq 5000 -1 1) > /dev/null
```

Comparer plusieurs tailles:

```bash
for n in 1000 2000 5000 10000; do
  echo "N=$n"
  ./PmergeMe $(seq $n -1 1) | tail -n 2
done
```

---

## Résumé

Cette implémentation respecte le sujet `CPP09/ex02`:

- parsing propre
- tri Ford-Johnson fonctionnel
- double implémentation `vector` et `list`
- affichage des temps comparatifs

Elle est adaptée à l'évaluation du module, même si elle n'est pas conçue pour être ultra-performante sur des tailles extrêmes.
