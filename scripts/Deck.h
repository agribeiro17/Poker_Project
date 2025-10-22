#ifndef DECK_H
#define DECK_H

#include "cards.h"
#include <QVector>
#include <random>

class deck_t {
	public:
	std::random_device rd;
    QVector<cards_t*> cards_on_deck;
    QVector<cards_t*> cards_drawn;


    deck_t();
    ~deck_t();
	void shuffle();
    void drawc();
};

#endif//DECK_H
