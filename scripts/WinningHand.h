#pragma once
#ifndef WINNINGHAND_H
#define WINNINGHAND_H

#include <string>
#include <vector>
#include <sstream>
#include "Table.h"

using namespace std;

class WinningHand
{
    //friend class TestWinningHand;

public:
    WinningHand();

    void collectNumbers(string hand);
    void organizeCards(int arrayToSort[]);
    void removeCards(string hand);
    void isHighCard(string hand);
    bool straight(string hand);
    bool flush(string hand);
    int ofAKind(string hand);
    int pairs(string hand);
    bool isRoyalFlush(string hand);
    int handRank(string hand);
    vector<int> compareRank(const vector<string>& hands);
    int playPoker(const vector<string>& hands);
    vector<int> determineWinningHand();
    table_t *table;

private:
    int cardNumArray[7] = {};
    int handHigh = 0, highCard = 0, handHigh1 = 0, handHigh2 = 0;
    Players_t *player;
};

#endif // WINNINGHAND_H
