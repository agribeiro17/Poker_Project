#ifndef MACHSTATE_H
#define MACHSTATE_H

#include "Table.h"
#include "Players.h"

typedef enum{
    init,
    ante,
    Hcards,
    bet,
    flop,
    turn,
    river,
    winner,
    reset
}  MachStates;

typedef enum{
    Waiting,
    Decision,
    Fold,
    Call,
    Check,
    Raise,
    Allin
}  BetStates;

typedef enum{
    Player1,
    Player2,
    Player3,
    Player4,
    Player5,
    Player6,
    Player7,
    Player8,
    Wait
}  Whoplays;

class Whoplay
{
public:
    table_t *table;
    Whoplays WcurrentState = Player1;
    Whoplay(table_t *t);
    void updwhoplays();
    bool round = false;
    bool firstround = true;
    bool fp = true;
};


class MachState
{
public:
    table_t *table;
    Whoplay *smwp;
    MachState(table_t *t, Whoplay *wp);
    void updMach();
    MachStates currentState = init;
    MachStates prevstate = init;
    Players_t *wplayer;
    Players_t *winplayer;
};


class Bet
{
public:
    table_t *table;
    Whoplay *smwp;
    Bet(table_t *t, Whoplay *wp);
    void updBet();
    BetStates BcurrentState = Waiting;

};

#endif // MACHSTATE_H
