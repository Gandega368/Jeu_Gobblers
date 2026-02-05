
// -----------------------------------------------------------------------------
// Niveaux de difficulté du bot :
// BOT_EASY : joue aléatoirement, pas de stratégie, coup toujours valide
// BOT_HARD : tente de gagner si possible, bloque le joueur humain,
//            sinon place la plus grande pièce disponible
// -----------------------------------------------------------------------------
#ifndef BOT_H
#define BOT_H

#include "playerEnum.h" 
class Board; // Forward declaration
// Structure pour décrire un coup
struct BotMove {
    bool isPlacement; // true = poser, false = déplacer
    int size;         // taille de la pièce (SMALL=1, MEDIUM=2, LARGE=3)
    int srcRow;       // ligne source (pour déplacer)
    int srcCol;       // colonne source
    int tgtRow;       // ligne cible
    int tgtCol;       // colonne cible
};

// Niveaux de difficulté
enum BotDifficulty { BOT_EASY, BOT_HARD};

// Fonction principale pour générer le coup du bot
BotMove botPlay(Board* game, Player botPlayer, BotDifficulty difficulty);

#endif
