#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdio>
#include <algorithm>
#include <sstream>
#include <map>
#include "WinningHand.h"

using namespace std;

WinningHand::WinningHand(){

}

void WinningHand::collectNumbers(string hand) {
    int j = 0, index = 0;



        for (int i = 0; i <= hand.length() - 1; i++) {

            j = -1;
            if (hand[i] >= '2' && hand[i] <= '9') {
                cardNumArray[index] = (hand[i] - '0');
                index++;
            }
            else if (hand[i] == '1' && hand[i + 1] == '0') {
                j = 10;
                i++;
            }
            else if (hand[i] == '1' && hand[i + 1] == '1') {
                j = 11;
                i++;
            }
            else if (hand[i] == '1' && hand[i + 1] == '2') {
                j = 12;
                i++;
            }
            else if (hand[i] == '1' && hand[i + 1] == '3') {
                j = 13;
                i++;
            }
            else if (hand[i] == '1' && hand[i + 1] == '4') {
                j = 14;
                i++;
            }
            else {
                switch (hand[i])
                {
                case 'T':
                    j = int(hand[i]) - 74;

                    break;
                case 'J':
                    j = int(hand[i]) - 63;

                    break;
                case 'Q':
                    j = int(hand[i]) - 69;

                    break;
                case 'K':
                    j = int(hand[i]) - 62;

                    break;
                case 'A':
                    j = int(hand[i]) - 51;

                    break;
                default:
                    break;
                }
            }
            if (j != -1)
            {
                cardNumArray[index] = j;
                index++;
            }
        }
        for (int i = 0; i <= 6; i++) {
            if (cardNumArray[i] > highCard)
                highCard = cardNumArray[i];
        }
    }

void WinningHand::organizeCards(int arrayToSort[]) {
    sort(arrayToSort, arrayToSort + 7);
    for (int i = 0; i < 7; ++i) {
        cardNumArray[i] = arrayToSort[i];
    }
}

void WinningHand::removeCards(string hand) {
    collectNumbers(hand);
    for (int i = 0; i <= 4; i++) {
        if (cardNumArray[i] == cardNumArray[i + 1] && cardNumArray[i + 1] == cardNumArray[i + 2]) {
            cardNumArray[i] = 0;
            cardNumArray[i + 1] = 0;
            cardNumArray[i + 2] = 0;
        }
    }
}

void WinningHand::isHighCard(string hand) {
    collectNumbers(hand);
    organizeCards(cardNumArray);

    handHigh = cardNumArray[6];
}

bool WinningHand::straight(string hand) {
    collectNumbers(hand);
    organizeCards(cardNumArray);
    int count = 0;
    vector<int> handHighs;

    if (cardNumArray[0] == 2 && cardNumArray[1] == 3 && cardNumArray[2] == 4 && cardNumArray[3] == 5 && cardNumArray[6] == 14)
    {
        handHigh = 5;
        return true;
    }

    else {
        for (int i = 6; i >= 0; i--) {
            if (cardNumArray[i] == cardNumArray[i - 1] + 1) {

                handHighs.push_back(cardNumArray[i]);
                count++;
            }
            else if ((cardNumArray[i] != cardNumArray[i - 1] + 1) && (count < 4)) {
                handHighs.clear();
                count = 0;
            }
        }
        if (count >= 4) {
            handHigh = *max_element(handHighs.begin(), handHighs.end());
            return true;
        }
        else
            return false;
    }

    return false;
}

bool WinningHand::flush(string hand) {
    int countD = 0, countC = 0, countH = 0, countS = 0, j = 0;
    vector<int> highD, highC, highH, highS;

    for (int i = 0; i <= hand.length() - 1; i++) {

        if (hand[i] == 'D') {
            countD++;

            if (hand[i+1] >= '2' && hand[i+1] <= '9')
                j = (hand[i+1] - '0');
            else if (hand[i+1] == '1' && hand[i + 2] == '0') {
                j = 10;
                i++;
            }
            else if (hand[i+1] == '1' && hand[i + 2] == '1') {
                j = 11;
                i++;
            }
            else if (hand[i+1] == '1' && hand[i + 2] == '2') {
                j = 12;
                i++;
            }
            else if (hand[i+1] == '1' && hand[i + 2] == '3') {
                j = 13;
                i++;
            }
            else if (hand[i+1] == '1' && hand[i + 2] == '4') {
                j = 14;
                i++;
            }
            else {
                switch (hand[i+1]) {
                case 'T':
                    j = int(hand[i+1]) - 74;

                    break;
                case 'J':
                    j = int(hand[i+1]) - 63;

                    break;
                case 'Q':
                    j = int(hand[i+1]) - 69;

                    break;
                case 'K':
                    j = int(hand[i+1]) - 62;

                    break;
                case 'A':
                    j = int(hand[i+1]) - 51;

                    break;
                default:
                    break;

                }
            }
            highD.push_back(j);
        }

        else if (hand[i] == 'C') {
            countC++;

            if (hand[i + 1] >= '2' && hand[i + 1] <= '9')
                j = (hand[i + 1] - '0');
            else if (hand[i + 1] == '1' && hand[i + 2] == '0') {
                j = 10;
                i++;
            }
            else if (hand[i + 1] == '1' && hand[i + 2] == '1') {
                j = 11;
                i++;
            }
            else if (hand[i + 1] == '1' && hand[i + 2] == '2') {
                j = 12;
                i++;
            }
            else if (hand[i + 1] == '1' && hand[i + 2] == '3') {
                j = 13;
                i++;
            }
            else if (hand[i + 1] == '1' && hand[i + 2] == '4') {
                j = 14;
                i++;
            }
            else {
                switch (hand[i + 1]) {
                case 'T':
                    j = int(hand[i + 1]) - 74;

                    break;
                case 'J':
                    j = int(hand[i + 1]) - 63;

                    break;
                case 'Q':
                    j = int(hand[i + 1]) - 69;

                    break;
                case 'K':
                    j = int(hand[i + 1]) - 62;

                    break;
                case 'A':
                    j = int(hand[i + 1]) - 51;

                    break;
                default:
                    break;

                }
            }
            highC.push_back(j);
        }

        else if (hand[i] == 'H') {
            countH++;

            if (hand[i + 1] >= '2' && hand[i + 1] <= '9')
                j = (hand[i + 1] - '0');
            else if (hand[i + 1] == '1' && hand[i + 2] == '0') {
                j = 10;
                i++;
            }
            else if (hand[i + 1] == '1' && hand[i + 2] == '1') {
                j = 11;
                i++;
            }
            else if (hand[i + 1] == '1' && hand[i + 2] == '2') {
                j = 12;
                i++;
            }
            else if (hand[i + 1] == '1' && hand[i + 2] == '3') {
                j = 13;
                i++;
            }
            else if (hand[i + 1] == '1' && hand[i + 2] == '4') {
                j = 14;
                i++;
            }
            else {
                switch (hand[i + 1]) {
                case 'T':
                    j = int(hand[i + 1]) - 74;

                    break;
                case 'J':
                    j = int(hand[i + 1]) - 63;

                    break;
                case 'Q':
                    j = int(hand[i + 1]) - 69;

                    break;
                case 'K':
                    j = int(hand[i + 1]) - 62;

                    break;
                case 'A':
                    j = int(hand[i + 1]) - 51;

                    break;
                default:
                    break;

                }
            }
            highH.push_back(j);
        }

        else if (hand[i] == 'S') {
            countS++;

            if (hand[i + 1] >= '2' && hand[i + 1] <= '9')
                j = (hand[i + 1] - '0');
            else if (hand[i + 1] == '1' && hand[i + 2] == '0') {
                j = 10;
                i++;
            }
            else if (hand[i + 1] == '1' && hand[i + 2] == '1') {
                j = 11;
                i++;
            }
            else if (hand[i + 1] == '1' && hand[i + 2] == '2') {
                j = 12;
                i++;
            }
            else if (hand[i + 1] == '1' && hand[i + 2] == '3') {
                j = 13;
                i++;
            }
            else if (hand[i + 1] == '1' && hand[i + 2] == '4') {
                j = 14;
                i++;
            }
            else {
                switch (hand[i + 1]) {
                case 'T':
                    j = int(hand[i + 1]) - 74;

                    break;
                case 'J':
                    j = int(hand[i + 1]) - 63;

                    break;
                case 'Q':
                    j = int(hand[i + 1]) - 69;

                    break;
                case 'K':
                    j = int(hand[i + 1]) - 62;

                    break;
                case 'A':
                    j = int(hand[i + 1]) - 51;

                    break;
                default:
                    break;

                }
            }
            highS.push_back(j);
        }
    }

    if (countD >= 5) {
        handHigh = *max_element(highD.begin(), highD.end());
        return true;
    }
    else if (countC >= 5) {
        handHigh = *max_element(highC.begin(), highC.end());
        return true;
    }
    else if (countH >= 5) {
        handHigh = *max_element(highH.begin(), highH.end());
        return true;
    }
    else if (countS >= 5) {
        handHigh = *max_element(highS.begin(), highS.end());
        return true;
    }
    else
        return false;
}

int WinningHand::ofAKind(string hand) {
    collectNumbers(hand);
    organizeCards(cardNumArray);
    int threeKind = 0, fourKind = 0;

    for (int i = 0; i <= 3; i++) {
        if (cardNumArray[i] == cardNumArray[i + 1] && cardNumArray[i + 1] == cardNumArray[i + 2] && cardNumArray[i + 2] == cardNumArray[i + 3]) {
            fourKind += 1;
            handHigh = cardNumArray[i + 3];
        }
    }
    if (fourKind == 1) {
        removeCards(hand);
        return 4;
    }
    else {
        for (int i = 0; i <= 4; i++) {
            if (cardNumArray[i] == cardNumArray[i + 1] && cardNumArray[i + 1] == cardNumArray[i + 2]) {
                threeKind += 1;
                handHigh = cardNumArray[i+2];
            }
        }
        if (fourKind < 1 && threeKind >= 1) {
            removeCards(hand);
            return 3;
        }
    }
    return 0;
}

int WinningHand::pairs(string hand) {
    collectNumbers(hand);
    int pairCount = 0;
    int pairIndices[6] = { -1, -1, -1, -1, -1, -1 }; // Initialize with invalid indices

    for (int i = 0; i <= 6; i++) {
        if (cardNumArray[i] == 0) {
            // Skip if the current card is already marked as part of a pair
            continue;
        }

        for (int j = i + 1; j <= 6; j++) {
            if (cardNumArray[i] == cardNumArray[j]) {
                pairIndices[pairCount * 2] = i;
                pairIndices[pairCount * 2 + 1] = j;
                pairCount++;
                if (cardNumArray[j] > handHigh1)
                    handHigh2 = handHigh1;
                handHigh1 = cardNumArray[j];

                // Mark the pair cards as 0 to indicate they've been identified
                cardNumArray[i] = 0;
                cardNumArray[j] = 0;

                // Break inner loop to check for the next pair
                break;
            }
        }
    }

    if (pairCount > 2)
        pairCount = 2;

    return pairCount;
}

bool WinningHand::isRoyalFlush(string hand) {
    collectNumbers(hand);
    organizeCards(cardNumArray);

    if (cardNumArray[2] == 10 && cardNumArray[3] == 11 && cardNumArray[4] == 12 && cardNumArray[5] == 13 && cardNumArray[6] == 14) {
        handHigh = 14;
        return true;
    }
    else
        return false;
}

int WinningHand::handRank(string hand) {
    bool flushTrue = flush(hand);
    bool straightTrue = straight(hand);
    bool royalTrue = isRoyalFlush(hand);

    if (straightTrue)
    {
        if (flushTrue)
        {
            if (royalTrue)
            {
                return 10; // Royal Flush
            }
            else
            {
                return 9; // Straight Flush
            }
        }
        else
        {
            return 5; // Straight
        }
    }

    else if (flushTrue)
    {
        return 6; // Flush
    }

    int kind = ofAKind(hand);
    int pair = pairs(hand);

    if (kind == 4) {
        return 8; // Four of a Kind
    }
    else if (kind == 3 && pair >= 2)             // pair >= 2 because 1 pair comes from the 3 kind
        return 7; // Full House

    else if (kind == 3 && pair < 2)             // same reason
        return 4; // Three of a Kind

    else if (pair > 0) {
        if (pair == 2) {
            return 3; // Two Pairs
        }
        else
            return 2; // Pair
    }
    else
        return 1; // High Card
}

vector<int> WinningHand::compareRank(const vector<string>& hands) {
    vector<int> ranks, handHighs;
    int ofKindHigh = 0, pairHigh = 0, pairLow = 0;
    int x;

    for (const string& hand : hands) {
        collectNumbers(hand);
        organizeCards(cardNumArray);
        int rank = handRank(hand);
        ranks.push_back(rank);

        if (rank == 10) {                             // Royal Flush
            isRoyalFlush(hand);
            handHighs.push_back(handHigh);
        }
        else if (rank == 9) {                         // Straight Flush
            straight(hand);
            handHighs.push_back(handHigh);
        }
        else if (rank == 8) {                         // 4 Of A Kind
            ofAKind(hand);
            handHighs.push_back(handHigh);
        }
        else if (rank == 7) {                         // Full House
            ofAKind(hand);
            if (handHigh > ofKindHigh) {
                ofKindHigh = handHigh;
                handHighs.push_back(handHigh);
                pairs(hand);
                if (handHigh1 > pairHigh)
                    pairHigh = handHigh1;
            }
            else if (handHigh == ofKindHigh) {
                pairs(hand);
                if (handHigh1 > pairHigh) {
                    pairHigh = handHigh1;
                    handHighs.push_back(handHigh1);
                }
                else if (handHigh1 == pairHigh) {
                    handHighs.push_back(handHigh1);
                }
            }
        }
        else if (rank == 6) {                         // Flush
            flush(hand);
            handHighs.push_back(handHigh);
        }
        else if (rank == 5) {                         // Straight
            straight(hand);
            handHighs.push_back(handHigh);
        }
        else if (rank == 4) {                         // 3 Of A Kind
            ofAKind(hand);
            handHighs.push_back(handHigh);
        }
        else if (rank == 3) {                         // 2 Pairs
            pairs(hand);
            if (handHigh1 > pairHigh) {
                pairHigh = handHigh1;
                handHighs.push_back(handHigh1);
                pairLow = handHigh2;
            }

            else if (handHigh1 == pairHigh) {
                if (handHigh2 > pairLow) {
                    pairLow = handHigh2;
                    x = handHigh1;
                    replace(handHighs.begin(), handHighs.end(), x, 0);
                    handHighs.push_back(handHigh2);
                }
                else if (handHigh2 == pairLow) {
                    x = handHigh1;
                    replace(handHighs.begin(), handHighs.end(), x, handHigh2);
                    handHighs.push_back(handHigh2);
                }
                else
                    handHighs.push_back(0);
            }
        }
        else if (rank == 2) {                         // 1 Pair
            pairs(hand);

            if (handHigh1 > pairHigh) {
                pairHigh = handHigh1;
                handHighs.push_back(handHigh1);
            }

            else if (handHigh1 == pairHigh) {
                isHighCard(hand);
                replace(handHighs.begin(), handHighs.end(), handHigh1, handHigh);
            }
        }
        else if (rank == 1) {                         // High Card
            isHighCard(hand);
            handHighs.push_back(handHigh);
        }
    }

    int maxRank = *max_element(ranks.begin(), ranks.end());
    int maxHandHigh = *max_element(handHighs.begin(), handHighs.end());

    vector<int> winners;
    for (int i = 0; i < ranks.size(); ++i) {
        if (ranks[i] == maxRank && handHighs[i] == maxHandHigh) {
            winners.push_back(i + 1);
        }
    }

    return winners;
}

int WinningHand::playPoker(const vector<string>& hands) {
    vector<int> ranks;
    vector<int> handHighs;

    for (const string& hand : hands) {
        collectNumbers(hand);
        organizeCards(cardNumArray);
        int rank = handRank(hand);

        ranks.push_back(rank);
        handHighs.push_back(handHigh);
    }

    int maxRank = *max_element(ranks.begin(), ranks.end());
    auto maxRankIndex = find(ranks.begin(), ranks.end(), maxRank);
    vector<int> winner;

    if (count(ranks.begin(), ranks.end(), maxRank) > 1) {
        return -1;
    }

    else {
        return distance(ranks.begin(), maxRankIndex) + 1;
    }
}

vector<int> WinningHand::determineWinningHand()
{
    vector<string> totalHands;
    vector<int> winners;
    // check for each Player
    foreach(Client_t *pl, this->table->player_list)
    {
        // Check if player did not FOLD
        if(pl->status != "Fold")
        {
            //Check If at the end of the round (River(5 community cards)
            if(this->table->cards_on_table.size() == 5)
            {
                // Combine the hole cards as well as the community cards to the be evaluated in the "playPoker" function
                QString playerHoleCards = pl->hand.at(0)->naipe + QString::number(pl->hand.at(0)->num) + pl->hand.at(1)->naipe + QString::number(pl->hand.at(1)->num);

                // Combine the player's hole cards with the community cards
                QString combinedHand = playerHoleCards;
                foreach(cards_t *card, this->table->cards_on_table) {
                    combinedHand += card->naipe + QString::number(card->num);
                }
                //QString playersHands = this->player->giveHandStr() + this->table->cards_on_table()

                qDebug() << "Combined Hand for Player " << pl->nickname << ": " << combinedHand; // Log combined hand
                // totalHands is a vector with all the strings to then be evaluated in the 'playPoker' function
                totalHands.push_back(combinedHand.toStdString()); //toStdString() is a method provided by Qt's QString class. It converts the QString object combinedHand into a std::string (a standard C++ string)
            }
        }
    }

    int winner = playPoker(totalHands);
//    qDebug() << "Winner ID: " << winner;
//    winners.push_back(winner);
//    return winners;

    if (winner > 0) {
        winners.push_back(winner);
        qDebug() << "Winner ID: " << winner; // Log winner's ID
        return winners;
    }
    else {
        winners = compareRank(totalHands);

        qDebug() << "Winner(s) ID: " << winners; // Log winner's ID
        return winners;
    }
}

