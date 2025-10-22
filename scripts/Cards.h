#ifndef CARDS_H
#define CARDS_H

#include <QString>

using namespace std;

class cards_t {
	public:
	int num;
    QString naipe;

    cards_t(int n, QString f);
    QString card_str();
};

#endif//CARDS_H
