/*Auteur:
Nom: GANDEGA
Prénom: DEMBA
Formation: FISE 2A - TAF ROBIN
Projet: Jeu Gobblers - 2e rendu
*/
#include <iostream>
#include <string>
#include <limits>
#include <cstdlib>
#include <ctime>
#include "board.h"
#include "bot.h"

using namespace std;

int main()
{
    Board* game = new Board();
    Player currentPlayer = PLAYER_1;
    Player botPlayer = PLAYER_2;
    BotDifficulty botDifficulty;
    StatusEnum status;
    Player winner;
    int l1, c1, l2, c2, s;
    bool actionValidee;

    // ---------------- Explication des niveaux de difficultés du jeu ----------------
    cout << "Bienvenue dans GOBBLERS !\n\n";
    cout << "Niveaux de difficulté du bot :\n";
    cout << "1 - Facile : le bot joue de manière aléatoire.\n";
    cout << "2 - Difficile : le bot joue stratégiquement, bloque vos menaces et tente de gagner.\n\n";

    // ---------------- Choix de la difficulté ----------------
    cout << "Choisissez la difficulté du bot :\n";
    cout << "1 - Facile\n2 - Difficile\nVotre choix : ";
    int diffChoice = 0;
    while (true) {
        cin >> diffChoice;
        if (diffChoice == 1) {
            botDifficulty = BOT_EASY;
            break;
        } else if (diffChoice == 2) {
            botDifficulty = BOT_HARD;
            break;
        } else {
            cout << "Erreur - Choix invalide. Entrez 1 (Facile) ou 2 (Difficile) : ";
        }
    }

    srand(time(nullptr)); // initialisation du RNG pour le bot

    do {
        cout << "\n\n\n" << *game << endl;

        // Vérification du gagnant
        winner = game->getWinner();
        if (winner != NO_PLAYER) {
            cout << "Victoire du joueur !!!!!!!" << (winner == PLAYER_1 ? 1 : 2) << " !" << endl;
            delete game;
            return 0;
        }

        cout << "Joueur " << (currentPlayer == PLAYER_1 ? 1 : 2) << endl;
        actionValidee = false;

        while (!actionValidee) {
            // ---------------- BOT ----------------
            if (currentPlayer == botPlayer) {
                BotMove move = botPlay(game, botPlayer, botDifficulty);

                if (move.isPlacement) {
                    status = game->placePiece(botPlayer, static_cast<Size>(move.size), move.tgtRow, move.tgtCol);
                    cout << "Bot place piece size " << move.size
                         << " at " << move.tgtRow << "," << move.tgtCol << endl;
                } else {
                    status = game->movePiece(move.srcRow, move.srcCol, move.tgtRow, move.tgtCol);
                    cout << "Bot moves piece from " << move.srcRow << "," << move.srcCol
                         << " to " << move.tgtRow << "," << move.tgtCol << endl;
                }
                actionValidee = (status == OK);
                continue;
            }

            // ---------------- JOUEUR HUMAIN ----------------
            cout << "Action choice. 1: Place, 2: Move, q: Quit" << endl;
            string input;
            cin >> input;
            if (input == "q") {
                delete game;
                return 0;
            } else if (input != "1" && input != "2") {
                cout << "Erreur - Action invalide. Choisissez 1, 2 ou q.\n";
                continue;
            }

            // ================= PLACEMENT =================
            if (input == "1") {
                cout << "Taille (1=SMALL, 2=MEDIUM, 3=LARGE) : ";
                cin >> s;

                cout << "Position (ligne colonne) : ";
                cin >> l1 >> c1;

                // INTERDICTION : même taille
                Size targetSize = game->getPieceSize(l1, c1);
                if (targetSize == static_cast<Size>(s)) {
                    cout << "Erreur - Impossible de poser une pièce sur une autre de même taille.\n";
                    continue;
                }

                status = game->placePiece(currentPlayer, static_cast<Size>(s), l1, c1);
            }

            // ================= DEPLACEMENT =================
            else if (input == "2") {
                cout << "Source (ligne colonne) : ";
                cin >> l1 >> c1;

                if (game->getPlaceHolder(l1, c1) != currentPlayer) {
                    cout << "Erreur - Cette pièce ne vous appartient pas.\n";
                    continue;
                }

                cout << "Cible (ligne colonne) : ";
                cin >> l2 >> c2;

                // INTERDICTION : même taille
                Size sourceSize = game->getPieceSize(l1, c1);
                Size targetSize = game->getPieceSize(l2, c2);
                if (targetSize == sourceSize) {
                    cout << "Erreur - Impossible de déplacer une pièce sur une autre de même taille.\n";
                    continue;
                }

                status = game->movePiece(l1, c1, l2, c2);
            }

            // ============ Gestion des erreurs ============
            if (status != OK) {
                cout << "X Erreur : ";
                switch (status) {
                    case INVALID_SOURCE: cout << "source invalide\n"; break;
                    case INVALID_TARGET: cout << "cible invalide\n"; break;
                    case INVALID_ENTRY:  cout << "entrée invalide\n"; break;
                    default: break;
                }
                continue;
            }

            // Action réussie
            actionValidee = true;
        }

        // Changement de joueur uniquement si action validée
        currentPlayer = game->nextPlayer(currentPlayer);

    } while (true);

    return 0;
}
