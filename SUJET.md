# Projet de logique: solveur SAT et Sudoku

> Version du 16/10/2016

L'objectif de ce projet est de se familiariser avec des techniques de base de test de satisfiabilité booléennne via l'implémentation d'un solveur SAT et d'utiliser la résolution SAT pour résoudre des [Sudoku](https://fr.wikipedia.org/wiki/Sudoku).

Le projet se découpe ainsi en deux parties: une partie solveur SAT et une partie modélisation en formule conjonctive (par fois appelée _cnf_) du problème de planification.

## Modalités

Le projet peut être réalisé en binôme, mais __pas__ en groupe de 3 étudiants ou plus.

> _Remplir lors de la première séance la case [tomuss](http://tomusss.univ-lyon1.fr) `num_binome` avec le numéro d'étudiant de votre binôme (8 chiffres).
> Si vous n'avez pas de binôme, laissez cette case vide._

> _Déposer sur [tomuss](http://tomusss.univ-lyon1.fr) dans la case `rendu_prj_pt_1_2` une archive contenant le projet pour les 2 première parties ([codage du solveur naïf](#résolution-de-problème-sat) et [modélisation du problème de Sudoku](#problème-de-sodoku)) pour le **27/11/2016**._

> _La version finale du projet (avec toutes les parties) doit être déposée sur [tomuss](http://tomusss.univ-lyon1.fr) dans la case `rendu_prj_final` pour le **18/12/2016**._

> _Les archives à déposer doivent avoir la même structure que l'[archive fournie](http://liris.cnrs.fr/~ecoquery/files/inf3034l-liflc-projet-2016.zip). Attention en particulier à ne pas déposer d'archive contenant des résidus de compilation ou des exécutables. Utiliser la commande `make clean` avant de créer l'archive à déposer._

Il est demandé de ne pas héberger vos sources sur un dépôt public. L'UCBL met à votre disposition une forge avec un hébergement de dépôts mercurial.

* Pour créer un projet: [http://forge.univ-lyon1.fr/projects/new](http://forge.univ-lyon1.fr/projects/new)
* Pour être ajouté à un projet, un étudiant doit s'être connecté au moins une fois sur [http://forge.univ-lyon1.fr](http://forge.univ-lyon1.fr).
* Le manager du projet pourra alors l'ajouter via Configuration → Membres.
* L'url mercurial de votre projet est accessible depuis Configuration → Dépôt
* Voir l'[aide](http://liris.cnrs.fr/ecoquery/dokuwiki/doku.php?id=enseignement:aide:forge) sur la forge pour un scénario d'utilisation pour des TPs/projets
* Voir les cours de [LIFAP4](http://liris.cnrs.fr/alexandre.meyer/public_html/www/doku.php?id=lifap4), ex LIF7.
* [Quelques slides](http://liris.cnrs.fr/ecoquery/dokuwiki/lib/exe/fetch.php?media=enseignement:maven-forge-ic.pdf) pour la gestion de projet (regarder la partie forge)

Le projet fourni peut être récupéré via la commande suivante exécutée dans le répertoire cloné d'un dépôt mercurial vierge:

	hg pull https://forge.univ-lyon1.fr/hg/inf3034l-liflc-projet-2016

Le projet est par ailleurs également téléchargeable ici: [lien archive](http://liris.cnrs.fr/~ecoquery/files/inf3034l-liflc-projet-2016.zip)

Le projet contient ce sujet, des exemples et un code de départ en C++.
Il est demandé de regarder le code fourni afin d'avoir une idée de ce qui est déjà implémenté et de ce qui reste à faire.

La base de code fournie suppose que l'on travail dans un environnement de type Unix (Linux, MacOSX).
Bien que le code soit assez générique vis-à-vis du système d'exploitation, aucune garantie n'est donnée sur le fonctionnement sous Windows et le projet sera testé sous Linux et/ou MacOSX.

## Planning conseillé

Le travail pour ce projet peut être divisé en 4 parties correspondant chacune à une séance de TP et du travail autour.
Après la première séance de TP, il est ainsi conseillé d'anticiper chaque séance afin d'avoir déjà des questions prêtes en début de créneau de TP. 

* 17/10: Découverte du sujet et [codage du solveur naïf](#résolution-de-problème-sat)
* 14/11: [Modélisation du problème de Sudoku](#problème-de-sodoku)
* 28/11: [Optimisations du solveur SAT](#amélioration-du-solveur-sat)
* 12/12: [Amélioration du codage du Sudoku](#amélioration-du-codage-du-sudoku)

## Résolution de problème SAT

Cette partie consiste à écrire un programme qui lit une formule conjonctive au format [Dimacs](http://www.satcompetition.org/2009/format-benchmarks2009.html) et génère un fichier indiquant si la formule est satisfiable.
Dans le cas où la formule est satisfiable, le fichier contiendra également les littéraux vrais dans un des modèles de la formule.

Le format Dimacs se présente de la manière suivante:

* Les lignes commençant par `c` sont des commentaires.
* Une ligne de la forme `p cnf n m` indique que la formule conjonctive contiendra $n$ variables et $m$ clauses. Les variables sont numérotées de $1$ à $n$. Une telle ligne est optionnelle.
* Les autres lignes représentent des clauses et sont de la forme $i_1~i_2~i_3~...~i_k~0$, où les $i$ représentent des littéraux. Si $i$ est positif alors $i$ est le numéro de la variable associée au littéral et le littéral est positif. Si $i$ est négatif alors le littéral est négatif et la variable a pour numéro $-i$.

Par exemple, la formule conjonctive $(p\vee q\vee \neg r)\wedge(\neg p \vee \neg q)\wedge(\neg p\vee r)\wedge(\neg p\vee q\vee \neg r)$ sera représentée par le fichier suivant (avec $p$ a pour numéro $1$, $q$ a pour numéro $2$ et $r$ a pour numéro $3$):

	p cnf 3 4
	1 2 -3 0
	-1 -2 0
	-1 3 0
	-1 2 -3 0

Les fichiers résultat ont la forme suivante:

* La première ligne est soit `SAT` si la formule conjonctive est satisfiable, soit `UNSAT` dans le cas contraire.
* Si la formule conjonctive est satisfiable, la seconde ligne contient les littéraux à affectés à vrai dans un modèle de la formule. Le format de cette seconde ligne est identique au format des clauses dimacs.

Par exemple, un modèle de la formule précédente est $I$ avec $I(p)$ est faux (donc le littéral $\neg p$ est affecté à vrai), $I(q)$ est vrai (donc le littéral $q$ est affecté à vrai) et $I(r)$ est faux (donc le littéral $\neg r$ est affecté à vrai). Ce résultat correspond au fichier suivant:

	SAT
	-1 2 -3 0
	
Les fonctions permettant de lire le format Dimacs et d'écrire le résultat sont déjà écrites dans le fichier `dimacs.cpp`.
Le fichier `sat.cpp` contient des squelettes de fonctions pour implémenter le solveur SAT.

### Structures de données pour représenter une formule conjonctive

Les formules conjonctives ayant une forme très régulière (conjonction de clauses), elle peuvent être représentée simplement par un tableau (`vector`) de clauses.
Les clauses sont elles-mêmes représentées comme des ensembles de littéraux.
Les littéraux sont représentés par des entiers positifs (pairs pour les littéraux négatifs, impairs pour les littéraux négatifs).

De même, chaque variable se voit attribué un numéro unique.
Attention les entiers utilisés pour numéroter les littéraux et les variables sont différents.
Si une variable $p$ est numérotée avec un entier $n$, le littéral positif correspondant (_i.e._ $p$) est numéroté avec l'entier $2n$, alors que le littéral négatif correspondant (_i.e._ $\neg p$) est numéroté avec l'entier $2n+1$.

> _Attention, ce codage sous forme d'entiers est donc différent du codage Dimacs. Les fonctions fournies pour lire et écrire ces fichiers s'occupent de faire la conversion correctement._

Afin de permettre de faire facilement évoluer le code (en particulier afin d'ajouter facilement des éléments additionnels), on encapsule ces types dans des `struct`[^1].

On obtient les structures définies dans le fichier `sat.hpp`.

<div class="question">
Coder les fonctions `var2lit`, `lit2var`, `positif` et `oppose`.
</div>

### Digression: tests unitaires et affichage

Le projet est fourni avec une bibliothèque de tests unitaires: [unittest-cpp](https://github.com/unittest-cpp/unittest-cpp).

Des exemples de tests unitaires se trouvent dans les fichiers `test-exemples.cpp` et `test-code-fourni.cpp`. Les tests sont exécutés via la commande `make test`[^2].

> _Il est à noter que tout fichier nommé `test-xxx.cpp` sera considéré comme contenant des tests unitaires.
> En particulier les fonctions définies dans ces fichiers ne seront pas inclues dans les autres exécutables._

<div class="question">
Lancer `make test`. Un test échoue. Trouver et supprimer ce test.

Écrire des tests pour vérifier le bon fonctionnement des fonctions traitant des littéraux.
</div>

Par la suite, lorque l'on demandera de tester une fonction, cela sous-entendra qu'il faut le faire via un test unitaire.

### Solveur naïf

Dans un premier temps, on va implémenter un solveur naïf qui évoluera ensuite au gré des optimisations vers un solveur plus efficace.


#### Evaluation de clauses et de formule conjonctive

La structure `etat_t` est utilisée pour conserver les informations concernant l'exploration courante de l'espace de recherche, comme la valeur affectée à chaque variable ou le numéro de la dernière variable sur laquelle on a effectué un choix.

Au cours de l'exploration de l'espace de recherche, on peut représenter les interprétations des variables propositionnelles par un tableau qui à chaque numéro de variable fait correspondre une valeur (de type `val_t`):

* `vrai` si la variable est affectée à vrai
* `faux` si la variable est affectée à faux
* `indeterminee` si la variable n'est pas affectée

Le type `val_t` est une [énumaration C++](http://fr.cppreference.com/w/cpp/language/enum). Il est ainsi possible d'utiliser les valeurs dans un `switch`.

<div class="question">
Coder la fonction `init_etat` afin d'initialiser le tableaux des valeurs en mettant toutes les variables à la valeur `indeterminee`.
Le numero de la dernière variable affectée pourra par exemple être initalisé à -1.

Coder la fonction `valeur` qui calcule la valeur d'un littéral. Tester la fonction.
</div>

La valeur d'une clause peut être déterminée comme suit:

* `vrai` si elle possède un littéral de la valeur courante est `vrai`,
* `faux` si tous ses littéraux sont `faux`,
* `indeterminee` dans les autres cas.

<div class="question">
Coder la fonction `evaluer_clause` et la tester.
</div>

La valeur d'une formule conjonctive peut être déterminée comme suit:

* `faux` si elle possède une clause qui s'évalue à `faux`,
* `vrai` si toutes ses clauses s'évaluent à `vrai`,
* `indeterminee` dans les autres cas.

<div class="question">
Coder la fonction `evaluer_cnf` et la tester.
</div>

> _Le test des fonctions sur les formules conjonctives peut utiliser des fichiers au format Dimacs. Il suffit de s'inspirer de la fonction `main` de `main-sat.cpp` pour cela ou des tests dans `test-code-fourni.cpp`. Attention aux noms des fichiers qui doivent être relatifs au répertoire `c++` (depuis lequel est exécuté `run-test`)._

#### Exploration de l'espace de recherche

Afin d'explorer l'espace de recherche on peut procéder en suivant le pseudo-code suivant:

	fonction chercher(etat, cnf):
		l = choisit_litteral(etat, cnf)
		ret_arr = affecte(etat, l)

	    val_cnf = evaluer_cnf(etat, cnf)

        si val_cnf == vrai
               || (val_cnf == indeterminee && chercher(etat, cnf)) alors
		    renvoyer true
		fin si

	    retour_arriere(etat, cnf, ret_arr)

	    l = oppose(l)
	    ret_arr = affecte(etat, l)

	    val_cnf = evaluer_cnf(etat, cnf)

        si val_cnf == vrai
           || (val_cnf == indeterminee && chercher(etat, cnf)) alors
		    renvoyer true
		sinon
		    retour_arriere(etat, cnf, ret_arr)
		    renvoyer false
		fin si
	fin chercher

La fonction `choisit_litteral` choisit un littéral à affecter à `vrai`, _i.e._ une variable et une valeur pour cette variable.

La fonction `affecte` modifie l'état de l'exploration en changeant la valeur de la variable.
Elle change également la dernière variable affectée.
Plus tard, cette fonction pourra faire des déductions comme de la propagation unitaire.
La fonction renvoie les informations nécessaires au retour arrière, c'est à dire pour le moment l'avant dernière variable affectée et le littéral affecté.

La fonction `retour_arriere` utilise les informations précédentes pour remettre l'etat à sa valeur avant la dernière affectation de littéral.

Ces fonctions sont à implémenter dans `sat.cpp`. Les tests peuvent utiliser des formules conjonctives lues dans un fichier dimacs de manière similaire à ce qui se fait dans la fonction `main` du fichier `main-sat.cpp`.

<div class="question">
Implémenter la fonction `affecte`, puis la fonction `retour_arriere` et les tester.
</div>

Le choix du littéral à affecter peut se faire de manière heuristique pour tenter de rendre l'exploration plus efficace.
Pour ce solveur naïf, on prendra la première variable qui n'a pas encore de valeur et lui donnera la valeur `vrai`.
On pourra ici utiliser la dernière variable affectée (numéro $n$) pour connaître la variable à affecter ensuite (numéro $n+1$).

<div class="question">
Implémenter la fonction `choisi_litteral` et la tester.
</div>

La fonction chercher permet de terminer l'implémentation du solveur naïf.


<div class="question">
Implémenter la fonction `chercher` et la tester.
</div>



## Problème de Sodoku

Le problème de [Sodoku](https://fr.wikipedia.org/wiki/Sudoku) consiste à placer des nombres sur une grille $n^2\times n^2$ (pour le sudoku classique $n=3$).

Un problème de Sudoku sera représenté par un fichier représentant la grille initiale à compléter:

* ce fichier contiendra $n^2$ lignes non vide;
* chaque ligne non vide représente une ligne de la grille contenant $n^2$ nombres;
* les nombres strictement positifs correspondent aux cases déjà remplies
* les `0` correspondent aux cases à remplir

Les régions sont des sous-grilles carrées de taille $n\times n$ découpant la grille initiale en $n\times n$ parties disjointes.
On suppose que les lignes et les colonnes sont numérotées à parir de $0$ en partant du haut et de la gauche.
Les coordonnées du coin supérieur gauche de chaque région sont ainsi de la forme $(i\times n, j \times n)$ où $0\leq i\leq n-1$ et $0\leq j\leq n-1$.

Compléter une grille de Sudoku se fait en respectant les 3 contraintes suivantes:

* Les nombres sur une même ligne sont tous différents
* Les nombres sur une même colonne sont tous différents
* Les nombres dans une même région sont tous différents
* Les nombres placés sur la grille sont tous compris entre $1$ et $n^2$ inclus.

Pour résoudre un Sudoku, on procède comme suit:

1. Un premier programme, `sudoku2dimacs`, va convertir un problème de Sudoku en formule conjonctive (sou s forme de fichier Dimacs), telle que la solution du Sudoku correspond au modèle de cette formule (un problème de Sudoku traditionel n'admet normalement qu'une seule solution).
2. Le programme `sat` calcule un éventuel modèle de la formule conjonctive.
3. Un dernier programme `res2sudoku`, convertit le résultat obtenu en grille de sudoku complétée.

Les fonctions `lit_sudoku` et `ecrit_sudoku` permettent de lire et écrire des grilles de sudoku.
Le fichier `exemples/sudoku/grille-test1.txt` contient un exemple de grille de sudoku $3^3\times 3^3$

### Variables booléennes pour le problème de sudoku

Afin de coder le problème  de Sudoku sous forme de formule conjonctive, on introduit $n^2$ variables booléennes par case de la grille.
On note la variable $p_{i,j}^v$ la $v^{ième}$ variable de la case située à la position $(i,j)$ dans la grille (avec $0\leq i\leq n^2-1$,$0\leq i\leq n^2-1$ et $1\leq v\leq n^2$).
On utilisera la convention que la variable $p_{i,j}^v$ prend la valeur vrai si et seulement si la case $(i,j)$ voit être remplie avec la valeur $v$.

On suppose que l'on numérote les variables comme suit (où $num(p)$ est le numéro de la variable $p$):

* $num(p_{0,0}^1)=0$
* $num(p_{i,j}^v) = num(p_{i,j}^{v-1}) + 1$ si $v \geq 2$
* $num(p_{i,j}^1) = num(p_{i,j-1}^{n^2}) + 1$ si $j \geq 1$
* $num(p_{i,0}^1) = num(p_{i-1,n^2-1}^{n^2}) + 1$ si $i\geq 1$.

Autrement dit: les variables d'une même case sont numérotées consécutivement.
Le numéro de la première variable d'une case suit immédiatement le numéro de la dernière variable de la case précédente, les cases étant considérées lignes par ligne.

Par exemple, si on considère une grille de Sudoku de taille $2^2\times 2^2$:

* $num(p_{0,0}^1)=0$
* $num(p_{0,0}^4)=3$
* $num(p_{0,1}^1)=4$
* $num(p_{0,3}^4)=15$
* $num(p_{1,0}^1)=16$
* $num(p_{3,3}^4)=63$

Sur une grille de taille $3^3\times 3^3$, $num(p_{0,8}^9)=80$ et $num(p_{8,8}^9)=728$.

<div class="question">
Coder et tester la fonction `l_case_v(n2,i,j,v)` qui renvoie le littéral positif associé à la variable codant la valeur `v` dans la case (`i`,`j`) si le nombre le nombre de lignes dans la grille de Sudoku est `n2` (_i.e._ $n^2$).
Cette fonction doit avoir un temps de calcul constant.
</div>

### Contrainte "exactement $1$ vrai parmi $n$"

Afin de coder le problème de Sudoku sous forme d'un ensemble de clauses plus facilement, on introduit une *contrainte* appelée "exactement $1$ vrai parmi $n$".
Une contrainte peut être vue comme une formule contraignant les valeurs de certaines variables.
Dans le cas de  "exactement $1$ vrai parmi $n$", cette formule va imposer que parmi $n$ littéraux, un littéral prend la valeur vrai, les autres prenant la valeur faux.

En s'appuyant sur les remarques suivantes, déduire un ensemble de clause codant cette contrainte:

* Un moins un des littéraux doit être vrai (1 clause)
* Deux littéraux ne peuvent pas être vrais en même temps ($\frac{n(n-1)}{2}$ clauses binaires)

<div class="question">
Coder la fonction `exactement_1_parmi_n`. Si votre solveur SAT est fonctionnel, il est possible de tester ces clauses en utilisant la fonction `cherche`.
</div>

### Codage des contraintes de case, de ligne, de colonne et de région

La première chose dont on veut s'assurer est que toutes les cases sont remplies et que l'on a pas placé 2 nombres différents dans la même case. Pour cela il suffit que pour chaque case il y ait exactement une des variables codant les valeur de la case à vrai.

<div class="question">
Commencer à coder la fonction `genere_probleme_sat` en utilisant pour chaque case du sudoku la fonction `exactement_1_parmi_n` afin d'assurer que chaque case possède exactement une valeur.
</div>

Chaque ligne doit comporter des nombres différents. Comme il y a autant de nombres que la taille de la ligne, chaque nombre doit apparaître exactement une fois. Donc pour chaque nombre $v$, pour chaque ligne $i$ il y a exactement une case $(i,j)$ dans la ligne pour laquelle la variable $p^v_{i,j}$ vaut vrai.

<div class="question">
Continuer le codage de la fonction `genere_probleme_sat` en utilisant pour chaque ligne et chaque nombre la fonction `exactement_1_parmi_n` afin d'assurer que chaque valeur est placée exactement une fois sur chaque ligne.
</div>

On peut remarquer qu'assurer que toutes les valeurs sont différentes sur chaque colonne et dans chaque région est similaire au cas des lignes.

<div class="question">
Coder les contraintes sur les colonnes et les régions dans `genere_probleme_sat`.
</div>

Enfin il reste à prendre en compte les nombres déjà placés sur la grille.
Si la case $i,j$ a la valeur $v$, il suffit de forcer la valeur de la variable $p^v_{i,j}$ à vrai en utilisant une clause unitaire.

<div class="question">
Terminer la fonction `genere_probleme_sat` en générant une clause unitaire par case déjà remplie dans la grille.
</div>
## Amélioration du solveur SAT

[TODO: à rédiger]

## Amélioration du codage du Sudoku

[TODO: à rédiger]

[^1]: TODO: ou des classes ?

[^2]: Cette commande compile et lance l'exécutable `run-tests` depuis le répertoire `c++`.

<div class="license">Auteur: [Emmanuel Coquery](http://liris.cnrs.fr/ecoquery/)<br/> <a href="http://creativecommons.org/licenses/by-nc-sa/3.0/" rel="license"><img src="https://licensebuttons.net/l/by-nc-sa/3.0/80x15.png" alt="CC Attribution-Noncommercial-Share Alike 3.0 Unported" /></a> Ce document est placé sous les termes de la licence suivante : <bdi><a href="http://creativecommons.org/licenses/by-nc-sa/3.0/" rel="license" class="urlextern">CC Attribution-Noncommercial-Share Alike 3.0 Unported</a></bdi></div>
