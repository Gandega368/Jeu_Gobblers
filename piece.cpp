/*Auteur:
Nom: GANDEGA
Prénom: DEMBA
Formation: FISE 2A - TAF ROBIN
*/

#include "piece.h"

// ----------------- Constructeurs -----------------

// Constructeur par défaut : pièce vide (NONE) et sans propriétaire
Piece::Piece() : owner(NO_PLAYER), size(NONE) {}

// Constructeur avec paramètres
Piece::Piece(Player owner, Size size) : owner(owner), size(size) {}

// Constructeur de copie
Piece::Piece(const Piece& other) : owner(other.owner), size(other.size) {}

// ----------------- Accesseurs -----------------

Player Piece::getOwner() const {
    return owner;
}

Size Piece::getSize() const {
    return size;
}
// ----------------- Opérateur d'affectation -----------------

Piece& Piece::operator=(const Piece& other) {
    if (this != &other) {  // protection contre l'auto-affectation
        owner = other.owner;
        size = other.size;
    }
    return *this;
}
