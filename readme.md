# **PROJET IA**
Le projet utilise la bibliothèque SFML pour créer un jeu où des ennemis contrôlés par l'IA interagissent avec un joueur. Il y a deux types d'ennemis : EnemyBT (utilisant un arbre de comportement) et GOAPEnemy (utilisant la planification d'actions basée sur des objectifs). Le pathfinding est une composante essentielle pour permettre aux ennemis de naviguer dans l'environnement de jeu.
# Fonctionnement des Ennemis
## **EnemyBT**

EnemyBT utilise un arbre de comportement (Behavior Tree) pour déterminer ses actions. Un arbre de comportement est une structure de données qui permet de modéliser des comportements complexes de manière hiérarchique et modulaire.
### **Contexte d'utilisation**

EnemyBT est utilisé pour des comportements d'ennemis qui nécessitent des décisions basées sur des conditions spécifiques, comme la distance au joueur ou les points de vie restants. L'arbre de comportement permet de structurer ces décisions de manière claire et extensible. L'arbre de comportement est composé de nœuds de séquence et de sélection, ainsi que de nœuds d'action et de condition. Les nœuds de séquence exécutent leurs enfants dans l'ordre jusqu'à ce que l'un d'eux échoue, tandis que les nœuds de sélection exécutent leurs enfants jusqu'à ce que l'un d'eux réussisse. Les nœuds d'action effectuent des actions spécifiques, et les nœuds de condition vérifient des conditions spécifiques.
Fonctionnement

#### **Patrouille** : L'ennemi suit un chemin prédéfini de points de patrouille. Si l'ennemi atteint un point de patrouille, il se dirige vers le suivant. Ce comportement est utilisé lorsque l'ennemi n'a pas détecté le joueur ou n'est pas en danger.
####	**Poursuite** : Si l'ennemi détecte le joueur et que le chemin est dégagé, il se dirige vers le joueur. Ce comportement est déclenché lorsque le joueur entre dans le rayon de vue de l'ennemi.
####	**Fuite** : Si l'ennemi est en danger (par exemple, si ses points de vie sont bas), il s'éloigne du joueur. Ce comportement est utilisé pour éviter la confrontation directe lorsque l'ennemi est désavantagé.
---
### **GOAPEnemy**
GOAPEnemy utilise la planification d'actions basée sur des objectifs (Goal-Oriented Action Planning, GOAP) pour déterminer ses actions. GOAP est une technique d'IA qui permet aux agents de planifier une série d'actions pour atteindre un objectif.
### Contexte d'utilisation
GOAPEnemy est utilisé pour des comportements d'ennemis qui nécessitent une planification flexible et dynamique. Cette approche est idéale pour des ennemis qui doivent s'adapter à des situations changeantes et prendre des décisions complexes pour atteindre leurs objectifs.L'ennemi évalue ses objectifs et planifie une série d'actions pour les atteindre. Cette planification est flexible et permet à l'ennemi de s'adapter à différentes situations en temps réel.
Conclusion
Fonctionnement
####	**Détection du Joueur** : L'ennemi utilise des rayons de détection pour savoir si le joueur est à portée de vue ou d'attaque. Si le joueur est détecté, l'ennemi peut décider de le poursuivre ou de l'attaquer.
####	**Poursuite** : Si le joueur est détecté et que l'ennemi décide de le poursuivre, il se dirige vers la position du joueur. Ce comportement est utilisé pour engager le joueur dans un combat.
####	**Fuite** : Si l'ennemi est en danger, il peut décider de fuir dans une direction opposée au joueur. Ce comportement est utilisé pour éviter la confrontation directe lorsque l'ennemi est désavantagé.
####	**Attaque** : Si le joueur est à portée d'attaque, l'ennemi peut décider de l'attaquer. Ce comportement est utilisé pour infliger des dégâts au joueur.
---
Les deux types d'ennemis, EnemyBT et GOAPEnemy, utilisent des techniques d'IA avancées pour déterminer leurs actions et interagir avec le joueur. EnemyBT utilise un arbre de comportement pour modéliser des comportements hiérarchiques, tandis que GOAPEnemy utilise la planification d'actions basée sur des objectifs pour planifier des séries d'actions dynamiques. Ces techniques permettent de créer des ennemis intelligents et réactifs dans le jeu.
---
# Fonctionnement du Pathfinding
### Contexte d'utilisation
Le pathfinding est utilisé pour permettre aux ennemis de naviguer efficacement dans l'environnement de jeu, en évitant les obstacles et en trouvant le chemin le plus court vers leur destination. Il est essentiel pour les comportements de poursuite, de fuite et de patrouille des ennemis.
Fonctionnement
#### **Vérification de Chemin** : Les ennemis utilisent la méthode isPathClear pour vérifier si le chemin entre deux points est dégagé. Cela permet de déterminer si l'ennemi peut se déplacer directement vers un point ou s'il doit trouver un autre chemin.
#### **Initialisation** : La classe Pathfinding est initialisée avec une référence à la grille de jeu (Grid). Cette grille contient des informations sur les cellules marchables et non marchables.
#### **Calcul de Chemin** : La méthode findPath est utilisée pour calculer le chemin entre deux points dans la grille. Elle utilise A*, pour trouver le chemin le plus court en tenant compte des obstacles.
#### **Mise à Jour des Positions** : Les ennemis mettent à jour leur position en suivant le chemin calculé par le pathfinding. Ils ajustent leur direction et leur vitesse en fonction des points de cheminement.

Le pathfinding permet aux ennemis de naviguer de manière intelligente et réactive dans l'environnement de jeu, en évitant les obstacles et en atteignant leurs objectifs de manière efficace.
