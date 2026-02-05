/*Auteur:
Nom: GANDEGA
Prénom: DEMBA
Formation: FISE 2A - TAF ROBIN
Projet: Jeu Gobblers - 2e rendu
*/
#include "board.h"
#include <iostream>
#include <stdexcept>

// ----------------- Constructeurs / Destructeur -----------------

Board::Board()
{
    for (int i = 0; i < NB_SIZE; ++i)
    {
        p1Pieces[i] = NB_INITIAL_PIECES;
        p2Pieces[i] = NB_INITIAL_PIECES;
    }
}

Board::Board(const Board& other)
{
    for (int i = 0; i < DIMENSIONS; ++i)
        for (int j = 0; j < DIMENSIONS; ++j)
            state[i][j] = other.state[i][j];

    for (int i = 0; i < NB_SIZE; ++i)
    {
        p1Pieces[i] = other.p1Pieces[i];
        p2Pieces[i] = other.p2Pieces[i];
    }
}

Board::~Board() {}

// ----------------- Méthodes privées -----------------

Cell& Board::getCellAt(int line, int column)
{
    if (line < 0 || line >= DIMENSIONS || column < 0 || column >= DIMENSIONS)
        throw std::out_of_range("Invalid board coordinates");

    return state[line][column];
}

int* Board::getHousesOfPlayer(Player player)
{
    if (player == PLAYER_1) return p1Pieces;
    if (player == PLAYER_2) return p2Pieces;
    return nullptr;
}

// ----------------- Gestion des joueurs -----------------

Player Board::nextPlayer(Player currentPlayer) const
{
    if (currentPlayer == PLAYER_1) return PLAYER_2;
    if (currentPlayer == PLAYER_2) return PLAYER_1;
    return NO_PLAYER;
}

// ----------------- Accès aux cases -----------------

Player Board::getPlaceHolder(int line, int column)
{
    Cell& cell = getCellAt(line, column);
    if (!cell.canPop()) return NO_PLAYER;
    return cell.peek().getOwner();
}

Size Board::getPieceSize(int line, int column)
{
    Cell& cell = getCellAt(line, column);
    if (!cell.canPop()) return NONE;
    return cell.peek().getSize();
}

int Board::getNbPiecesInHouse(Player checkedPlayer, Size pieceSize)
{
    if (pieceSize == NONE) return 0;

    int* house = getHousesOfPlayer(checkedPlayer);
    if (!house) return 0;

    return house[pieceSize - 1]; // SMALL=1, MEDIUM=2, LARGE=3
}

// ----------------- Placement et déplacement -----------------

StatusEnum Board::placePiece(Player currentPlayer, Size pieceSize, int line, int column)
{
    if (pieceSize == NONE ||
        line < 0 || line >= DIMENSIONS ||
        column < 0 || column >= DIMENSIONS)
        return INVALID_ENTRY;

    int* house = getHousesOfPlayer(currentPlayer);
    if (!house || house[pieceSize - 1] <= 0)
        return INVALID_SOURCE;

    Cell& cell = getCellAt(line, column);
    Piece piece(currentPlayer, pieceSize);

    if (!cell.canPush(piece))
        return INVALID_TARGET;

    cell.push(piece);
    house[pieceSize - 1]--;

    return OK;
}

StatusEnum Board::movePiece(int sourceLine, int sourceColumn,
                            int targetLine, int targetColumn)
{
    if (sourceLine < 0 || sourceLine >= DIMENSIONS ||
        sourceColumn < 0 || sourceColumn >= DIMENSIONS ||
        targetLine < 0 || targetLine >= DIMENSIONS ||
        targetColumn < 0 || targetColumn >= DIMENSIONS)
        return INVALID_ENTRY;

    Cell& sourceCell = getCellAt(sourceLine, sourceColumn);
    Cell& targetCell = getCellAt(targetLine, targetColumn);

    if (!sourceCell.canPop())
        return INVALID_SOURCE;

    Piece movingPiece = sourceCell.pop();

    if (!targetCell.canPush(movingPiece))
    {
        sourceCell.push(movingPiece);
        return INVALID_TARGET;
    }

    targetCell.push(movingPiece);
    return OK;
}

// ----------------- Détection du gagnant -----------------

Player Board::getWinner()
{
    // Lignes et colonnes
    for (int i = 0; i < DIMENSIONS; ++i)
    {
        for (Player p : {PLAYER_1, PLAYER_2})
        {
            bool lineWin = true;
            bool colWin = true;

            for (int j = 0; j < DIMENSIONS; ++j)
            {
                if (getPlaceHolder(i, j) != p) lineWin = false;
                if (getPlaceHolder(j, i) != p) colWin = false;
            }

            if (lineWin || colWin)
                return p;
        }
    }

    // Diagonales
    for (Player p : {PLAYER_1, PLAYER_2})
    {
        bool diag1 = true;
        bool diag2 = true;

        for (int i = 0; i < DIMENSIONS; ++i)
        {
            if (getPlaceHolder(i, i) != p) diag1 = false;
            if (getPlaceHolder(i, DIMENSIONS - 1 - i) != p) diag2 = false;
        }

        if (diag1 || diag2)
            return p;
    }

    return NO_PLAYER;
}

// ----------------- Affichage -----------------

std::ostream& Board::printHouses(std::ostream& stream, Player player)
{
    int* house = getHousesOfPlayer(player);
    if (!house) return stream;

    stream << "House Player " << player << " : ";
    stream << "S=" << house[SMALL - 1] << " ";
    stream << "M=" << house[MEDIUM - 1] << " ";
    stream << "L=" << house[LARGE - 1];

    return stream;
}

std::ostream& operator<<(std::ostream& stream, Board& board)
{
    const std::string RED   = "\033[31m";
    const std::string BLUE  = "\033[34m";
    const std::string RESET = "\033[0m";

    stream << "========== GOBBLERS=========\n\n";
    // ----------------- Légende -----------------
    stream << "Légende :\n";
    stream << RED   << "Joueur 1 : " 
           << "S=x, M=X, L=#" << RESET << "\n";
    stream << BLUE  << "Joueur 2 : " 
           << "S=., M=o, L=O" << RESET << "\n\n";

    // Affichage des maisons
    stream << "========== MAISONS =========\n\n";
    board.printHouses(stream, PLAYER_1);
    stream << "\n";
    board.printHouses(stream, PLAYER_2);
    stream << "\n\n";
    stream << "===========================\n\n";

    // Plateau
    stream << "Plateau :\n";
    stream << " +---+---+---+\n";

    for (int i = 0; i < DIMENSIONS; ++i)
    {
        stream << " |";
        for (int j = 0; j < DIMENSIONS; ++j)
        {
            Size s = board.getPieceSize(i, j);
            Player p = board.getPlaceHolder(i, j);

            if (s == NONE)
            {
                stream << "   |";
            }
            else
            {
                char c;
                std::string color;

                if (p == PLAYER_1) {
                    color = RED;
                    c = (s == SMALL ? SMALLP1 : (s == MEDIUM ? MEDIUMP1 : LARGEP1));
                } else {
                    color = BLUE;
                    c = (s == SMALL ? SMALLP2 : (s == MEDIUM ? MEDIUMP2 : LARGEP2));
                }

                stream << " " << color << c << RESET << " |";
            }
        }
        stream << "\n +---+---+---+\n";
    }

    return stream;
}
