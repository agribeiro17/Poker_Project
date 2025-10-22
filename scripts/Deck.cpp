#include "Deck.h"
#include <algorithm>
#include <random>

using namespace std;

deck_t::deck_t() {
    QVector<QString> naipes{ "H", "C", "D" , "S" };
	for (int j = 0; j < 4; j++) {
        for (int i = 2; i < 15; i++) {
			cards_t* card = new cards_t(i, naipes[j]);
			cards_on_deck.push_back(card);
        }
    }
}

deck_t::~deck_t()
{
    this->cards_on_deck.empty();
}

void deck_t::shuffle() {
    std::shuffle(cards_on_deck.begin(), cards_on_deck.end(), rd);
}

void deck_t::drawc()
{
    QVector<QString> naipes{ "H", "C", "D" , "S" };
    for (int j = 0; j < 4; j++) {
        for (int i = 2; i < 15; i++) {
            cards_t* card = new cards_t(i, naipes[j]);
            cards_on_deck.push_back(card);
        }
    }
}

