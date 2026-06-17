#ifndef BLACKJACKGAME_H
#define BLACKJACKGAME_H

#include <vector>

#include "Card.h"

class BlackjackGame
{
private:
    vector<Card> deck;
    vector<Card> playerHand;
    vector<Card> dealerHand;

    int chips = 10;
    int bet = 1;
    int round = 1;

    int wins = 0;
    int losses = 0;
    int ties = 0;

    bool roundActive = false;
    bool dealerHidden = true;
    bool dealerTurn = false;
    bool doubleDownUsed = false;

    string message = "If you can turn $10 into $20 or more, you win.";

public:
    vector<Card> createDeck();

    void shuffleDeck();

    Card drawCard();

    int calculateHandValue(const vector<Card> &hand) const;

    bool isBlackjack(const vector<Card> &hand) const;

    void startRound(int inputBet);

    void hit();

    void doubleDown();

    void beginDealerTurn();

    bool dealerNeedsCard() const;

    void dealerDrawOne();

    void settleNormalRound();

    void settleBlackjack();

    void endRound();

    bool canDoubleDown() const;

    int getChips() const { return chips; }
    int getBet() const { return bet; }
    int getRound() const { return round; }
    int getWins() const { return wins; }
    int getLosses() const { return losses; }
    int getTies() const { return ties; }

    int getPlayerTotal() const;

    int getDealerTotal() const;

    const vector<Card> &getPlayerHand() const { return playerHand; }
    const vector<Card> &getDealerHand() const { return dealerHand; }

    bool isDealerHidden() const { return dealerHidden; }
    bool isRoundActive() const { return roundActive; }
    bool isDealerTurn() const { return dealerTurn; }

    bool isGameOver() const
    {
        return chips <= 0 || chips >= 20;
    }

    string getMessage() const
    {
        return message;
    }
};

#endif // BLACKJACKGAME_H
