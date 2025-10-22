#ifndef POKER_STATE_MACHINE_H
#define POKER_STATE_MACHINE_H

#include "Table.h"
#include <QObject>

//MANAGING THE STATE TRANSITION FOR POKER GAME//

class Poker_State_Machine : public QObject
{
    Q_OBJECT

public:
    Poker_State_Machine(QObject *parent = nullptr, table_t *tb);

    //States for the Poker Game
    enum State{
        initialState,
        preFlopState,
        flopState,
        turnState,
        riverState,
        showDownState,
        endOfHandState,
        idleState,
        exit
    };

    enum PlayerDecisionState{
        betState,
        callState,
        checkState,
        foldState,
        raiseState,
        allInState,
        out
    };

    //Allows to transition from current state to the new state(one of the states in the enum)
    void transitionToState(State newState);

    void transitionToPlayerState(PlayerDecisionState newPlayerState);

    //handling the logic of a specific state
    void handleState();

    void gameCycle();

    //First requirements for the game to start(setting up the game, big blind, small blind dealer, etc)
    void startGame();

    //Dealing the two cards per player
    void dealHoleCards();

    void playerDecision();

    // Dealling the community cards depending on the round (flop, turn, river)
    void communityCards();

    //handle the winning pot at the end and distribute chips to the winning player(s), also split plot if necessary(tie)
    void handlePot();

    //The final bets have already been placed and its to see who is the winner amongst the players
    void revealHand();

    // In the showdown the ranking of the hands are calculated to see who has the highest hand(winner)
    void checkHandRankings();

    //check if the player has enough chips to play at the end of each round(for example if they go all in and lose)
    void checkChips();

    //waiting logic
    void idleLogic();




private:

    //stores the current state of the Poker_State_Machine obj., allowing it to hold one of the predefined states
    State currentState;
    PlayerDecisionState currentPlayerDecisionState;
    table_t *table;
};

#endif // POKER_STATE_MACHINE_H
