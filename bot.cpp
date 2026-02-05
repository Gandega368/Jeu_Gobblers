/*Auteur:
Nom: GANDEGA
Prénom: DEMBA
Formation: FISE 2A - TAF ROBIN
Projet: Jeu Gobblers - 2e rendu
*/
#include "bot.h"
#include "board.h"
#include <cstdlib>
#include <ctime>
#include <iostream>

// -----------------------------------------------------------------------------
// Structure pour représenter une case vide stratégique (menace ou opportunité)
struct Threat {
    int row;
    int col;
};

// ---------------- Outils ----------------

// Retourne une coordonnée aléatoire du plateau
static int randomCoord() {
    return rand() % DIMENSIONS;
}

// -----------------------------------------------------------------------------
// Déclarations internes
static bool detectThreat(Board* game, Player human, Threat& threat);
static bool detectWinningOpportunity(Board* game, Player bot, Threat& opportunity);

// ================= BOT EASY =================
// Coup aléatoire mais valide (ne pose pas sur une pièce trop grande ou de même taille)
static BotMove botEasy(Board* game, Player botPlayer) {
    BotMove move;
    move.isPlacement = true;

    // Vérifier si le bot peut placer une pièce
    bool canPlace = game->getNbPiecesInHouse(botPlayer, SMALL) > 0 ||
                    game->getNbPiecesInHouse(botPlayer, MEDIUM) > 0 ||
                    game->getNbPiecesInHouse(botPlayer, LARGE) > 0;

    move.isPlacement = canPlace && (rand() % 2 == 0);

    // ---------- PLACEMENT ----------
    if (move.isPlacement) {
        // Choix taille valide
        do {
            move.size = rand() % 3 + 1; // 1=SMALL, 2=MEDIUM, 3=LARGE
        } while (game->getNbPiecesInHouse(botPlayer, static_cast<Size>(move.size)) == 0);

        // Cherche une case compatible
        while (true) {
            move.tgtRow = randomCoord();
            move.tgtCol = randomCoord();
            if (game->getPieceSize(move.tgtRow, move.tgtCol) < move.size) break;
        }
        return move;
    }

    // ---------- DEPLACEMENT ----------
    move.isPlacement = false;

    // Choisir une pièce à déplacer
    while (true) {
        move.srcRow = randomCoord();
        move.srcCol = randomCoord();
        if (game->getPlaceHolder(move.srcRow, move.srcCol) == botPlayer) break;
    }
    move.size = game->getPieceSize(move.srcRow, move.srcCol);

    // Chercher une case cible valide
    while (true) {
        move.tgtRow = randomCoord();
        move.tgtCol = randomCoord();
        if (game->getPieceSize(move.tgtRow, move.tgtCol) < move.size) break;
    }

    return move;
}

// ================= BOT HARD =================
static BotMove botHard(Board* game, Player botPlayer) {
    BotMove move;
    Threat threat;

    //OFFENSIVE : tenter de gagner
    if (detectWinningOpportunity(game, botPlayer, threat)) {
        move.isPlacement = true;

        if (game->getNbPiecesInHouse(botPlayer, LARGE) > 0)
            move.size = LARGE;
        else if (game->getNbPiecesInHouse(botPlayer, MEDIUM) > 0)
            move.size = MEDIUM;
        else if (game->getNbPiecesInHouse(botPlayer, SMALL) > 0)
            move.size = SMALL;
        else
            return botEasy(game, botPlayer);

        if (game->getPieceSize(threat.row, threat.col) < move.size) {
            move.tgtRow = threat.row;
            move.tgtCol = threat.col;
            return move;
        }
    }

    // DÉFENSE : bloquer une victoire imminente de l'humain
    if (detectThreat(game, PLAYER_1, threat)) {
        move.isPlacement = true;

        if (game->getNbPiecesInHouse(botPlayer, LARGE) > 0)
            move.size = LARGE;
        else if (game->getNbPiecesInHouse(botPlayer, MEDIUM) > 0)
            move.size = MEDIUM;
        else if (game->getNbPiecesInHouse(botPlayer, SMALL) > 0)
            move.size = SMALL;
        else
            return botEasy(game, botPlayer);

        if (game->getPieceSize(threat.row, threat.col) < move.size) {
            move.tgtRow = threat.row;
            move.tgtCol = threat.col;
            return move;
        }
    }

    //OFFENSIVE SIMPLE : poser la plus grosse pièce sur une case valide
    move.isPlacement = true;

    if (game->getNbPiecesInHouse(botPlayer, LARGE) > 0)
        move.size = LARGE;
    else if (game->getNbPiecesInHouse(botPlayer, MEDIUM) > 0)
        move.size = MEDIUM;
    else if (game->getNbPiecesInHouse(botPlayer, SMALL) > 0)
        move.size = SMALL;
    else
        return botEasy(game, botPlayer);

    for (int i = 0; i < DIMENSIONS; i++) {
        for (int j = 0; j < DIMENSIONS; j++) {
            if (game->getPieceSize(i, j) < move.size) {
                move.tgtRow = i;
                move.tgtCol = j;
                return move;
            }
        }
    }

    //Fallback
    return botEasy(game, botPlayer);
}

// ================= API PUBLIQUE =================
BotMove botPlay(Board* game, Player botPlayer, BotDifficulty difficulty) {
    if (difficulty == BOT_EASY)
        return botEasy(game, botPlayer);
    else
        return botHard(game, botPlayer);
}

// -----------------------------------------------------------------------------
// Détecter une menace (2 pièces humaines alignées, case vide)
static bool detectThreat(Board* game, Player human, Threat& threat) {
    int lines[8][3][2] = {
        {{0,0},{0,1},{0,2}},
        {{1,0},{1,1},{1,2}},
        {{2,0},{2,1},{2,2}},
        {{0,0},{1,0},{2,0}},
        {{0,1},{1,1},{2,1}},
        {{0,2},{1,2},{2,2}},
        {{0,0},{1,1},{2,2}},
        {{0,2},{1,1},{2,0}}
    };

    for (auto& line : lines) {
        int countHuman = 0;
        int emptyIndex = -1;

        for (int i = 0; i < 3; i++) {
            int r = line[i][0];
            int c = line[i][1];

            if (game->getPlaceHolder(r, c) == human)
                countHuman++;
            else
                emptyIndex = i;
        }

        if (countHuman == 2 && emptyIndex != -1) {
            threat.row = line[emptyIndex][0];
            threat.col = line[emptyIndex][1];
            return true;
        }
    }
    return false;
}

// -----------------------------------------------------------------------------
// Détecter opportunité de victoire pour le bot
static bool detectWinningOpportunity(Board* game, Player bot, Threat& opportunity) {
    int lines[8][3][2] = {
        {{0,0},{0,1},{0,2}},
        {{1,0},{1,1},{1,2}},
        {{2,0},{2,1},{2,2}},
        {{0,0},{1,0},{2,0}},
        {{0,1},{1,1},{2,1}},
        {{0,2},{1,2},{2,2}},
        {{0,0},{1,1},{2,2}},
        {{0,2},{1,1},{2,0}}
    };

    for (auto& line : lines) {
        int countBot = 0;
        int emptyIndex = -1;

        for (int i = 0; i < 3; i++) {
            int r = line[i][0];
            int c = line[i][1];

            if (game->getPlaceHolder(r, c) == bot)
                countBot++;
            else
                emptyIndex = i;
        }

        if (countBot == 2 && emptyIndex != -1) {
            opportunity.row = line[emptyIndex][0];
            opportunity.col = line[emptyIndex][1];
            return true;
        }
    }
    return false;
}
