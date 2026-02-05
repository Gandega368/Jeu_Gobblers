# Gobblers – Présentation du projet
Ce fichier décrit le fonctionnement du bot dans le jeu Gobblers et explique les niveaux de difficulté disponibles.
Le jeu se joue sur un plateau 3×3 et oppose un joueur humain contre un bot disposant de plusieurs niveaux de difficulté.
Le projet a été conçu avec une architecture modulaire, séparant clairement la logique du jeu et l’interface utilisateur en console.
## Règles du jeu
Chaque joueur possède des pièces de trois tailles :
- SMALL
- MEDIUM
- LARGE
Une pièce plus grande peut recouvrir une pièce plus petite, mais il est interdit de poser ou déplacer une pièce sur une autre de même taille.

Un joueur gagne lorsqu’il aligne 3 de ses pièces visibles :
en ligne
en colonne
ou en diagonale
## Fonctionnalités
- Mode joueur contre bot
- Plusieurs niveaux de difficulté pour le bot
- Affichage du plateau en console
- Légende et couleurs pour distinguer les joueurs
- Détection automatique de victoire
- Gestion robuste des erreurs (coups invalides)
## Niveaux de difficulté du bot

Le bot peut être configuré avec différents niveaux pour ajuster la difficulté du jeu :

1. Facile (BOT_EASY)  
   - Le bot joue aléatoirement.  
   - Il choisit de poser ou de déplacer une pièce de manière valide.  
   - Ne suit aucune stratégie offensive ou défensive.  
   - Utile pour débuter ou tester le jeu.

2. Difficile (BOT_HARD)  
   - Le bot joue de manière stratégique.  
   - Offensive : tente de compléter une ligne pour gagner dès que possible.  
   - Défensive : bloque les alignements imminents du joueur humain.  
   - Sinon, il place la plus grande pièce disponible sur une case valide.  
   - Représente un vrai challenge pour le joueur.

## Fonctionnement du bot

- Le bot peut poser une pièce depuis sa réserve ou déplacer une pièce déjà sur le plateau.  
- Il ne pose jamais une pièce sur une case où une pièce de même taille existe déjà.  
- Le bot utilise des fonctions internes pour :
  - Détecter les menaces (2 pièces du joueur humain alignées et une case vide).  
  - Détecter les opportunités de victoire (2 pièces du bot alignées et une case vide).  

## Fichiers concernés
- "board.h" / "board.cpp" - logique du plateau et règles du jeu
- "piece.h" / "piece.cpp" - gestion des pièces
- "cell.h" / "cell.cpp" - gestion des cases
- "bot.h" – Déclarations des fonctions et types pour le bot.  
- "bot.cpp" – Implémentation du bot (facile et difficile).  
- "main.cpp" – Interface utilisateur et boucle de jeu, permet de choisir la difficulté du jeu.  


