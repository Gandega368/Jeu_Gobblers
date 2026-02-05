/*Auteur:
Nom: GANDEGA
Prénom: DEMBA
Formation: FISE 2A - TAF ROBIN
Projet: Jeu Gobblers - 2e rendu
*/
#include "cell.h"
#include <stdexcept> // Pour std::runtime_error

// ----------------- Constructeurs -----------------

Cell::Cell() : index(-1) { 
    // Initialisation de la pile : toutes les pièces vides
    for(int i = 0; i < NB_SIZE; ++i) {
        stack[i] = Piece(); // Pièce vide
    }
}

Cell::Cell(const Cell& other) : index(other.index) {
    for(int i = 0; i <= index; ++i) {
        stack[i] = other.stack[i];
    }
}

// ----------------- Vérification et modification de la pile -----------------

// Peut-on empiler cette pièce sur la pile actuelle ?
bool Cell::canPush(const Piece& piece) {
    if (index >= NB_SIZE - 1) return false; // pile pleine
    if (index == -1) return true;           // pile vide, tout est possible
    return piece.getSize() > stack[index].getSize(); // taille supérieure au sommet
}

// Empile la pièce (on suppose que canPush a été vérifié)
void Cell::push(const Piece& piece) {
    if(!canPush(piece)) {
        throw std::runtime_error("Cannot push piece: either stack full or piece too small.");
    }
    ++index;
    stack[index] = piece;
}

// Peut-on dépiler une pièce ?
bool Cell::canPop() {
    return index >= 0;
}

// Dépile la pièce du sommet et la retourne
Piece& Cell::pop() {
    if(!canPop()) {
        throw std::runtime_error("Cannot pop: stack empty.");
    }
    return stack[index--];
}

// Retourne la pièce du sommet sans la dépiler
Piece& Cell::peek() {
    if(index < 0) {
        throw std::runtime_error("Cannot peek: stack empty.");
    }
    return stack[index];
}

// ----------------- Opérateur d'affectation -----------------

Cell& Cell::operator=(const Cell& other) {
    if(this != &other) {
        index = other.index;
        for(int i = 0; i <= index; ++i) {
            stack[i] = other.stack[i];
        }
    }
    return *this;
}
