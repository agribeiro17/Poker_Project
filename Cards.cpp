#include "Cards.h"

cards_t::cards_t(int n, QString f) {
    num = n;
    naipe = f;
}

QString cards_t::card_str()
{
    QString c = naipe + QString::number(num);
    return c;
}
