#include "BlackjackGame.h"

#include <random>
#include <algorithm>

vector<Card> BlackjackGame::createDeck()
{
    vector<Card> newDeck;

    string suits[] = {"Heart", "Diamond", "Club", "Spade"};
    string ranks[] = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"};
    int values[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10, 11};

    for (const string &suit : suits)
    {
        for (int i = 0; i < 13; i++)
        {
            newDeck.push_back({ranks[i], suit, values[i]});
        }
    }

    return newDeck;
}

void BlackjackGame::shuffleDeck()
{
    random_device rd;
    mt19937 g(rd());
    shuffle(deck.begin(), deck.end(), g);
}

Card BlackjackGame::drawCard()
{
    Card card = deck.back();
    deck.pop_back();
    return card;
}

int BlackjackGame::calculateHandValue(const vector<Card> &hand) const
{
    int value = 0;
    int aceCount = 0;

    for (const Card &card : hand)
    {
        value += card.value;
        if (card.rank == "A")
        {
            aceCount++;
        }
    }

    while (value > 21 && aceCount > 0)
    {
        value -= 10;
        aceCount--;
    }

    return value;
}

bool BlackjackGame::isBlackjack(const vector<Card> &hand) const
{
    return hand.size() == 2 && calculateHandValue(hand) == 21;
}

void BlackjackGame::startRound(int inputBet)
{
    if (isGameOver())
        return;

    bet = inputBet;
    roundActive = true;
    dealerHidden = true;
    dealerTurn = false;
    doubleDownUsed = false;

    deck = createDeck();
    shuffleDeck();

    playerHand.clear();
    dealerHand.clear();

    playerHand.push_back(drawCard());
    dealerHand.push_back(drawCard());
    playerHand.push_back(drawCard());
    dealerHand.push_back(drawCard());

    message = "Round started. Choose Hit, Stand, or Double Down.";

    if (isBlackjack(playerHand) || isBlackjack(dealerHand))
    {
        dealerHidden = false;
        settleBlackjack();
    }
}

void BlackjackGame::hit()
{
    if (!roundActive || dealerTurn)
        return;

    playerHand.push_back(drawCard());

    if (calculateHandValue(playerHand) > 21)
    {
        chips -= bet;
        losses++;
        message = "Bust! You lose.";
        endRound();
    }
    else
    {
        message = "You drew a card.";
    }
}

void BlackjackGame::doubleDown()
{
    if (!canDoubleDown())
        return;

    bet *= 2;
    doubleDownUsed = true;
    playerHand.push_back(drawCard());

    if (calculateHandValue(playerHand) > 21)
    {
        chips -= bet;
        losses++;
        message = "Double Down! Bust! You lose.";
        endRound();
    }
    else
    {
        message = "Double Down! Dealer's turn.";
        beginDealerTurn();
    }
}

void BlackjackGame::beginDealerTurn()
{
    if (!roundActive)
        return;

    dealerHidden = false;
    dealerTurn = true;
    message = "Dealer's turn.";
}

bool BlackjackGame::dealerNeedsCard() const
{
    return dealerTurn && calculateHandValue(dealerHand) < 17;
}

void BlackjackGame::dealerDrawOne()
{
    if (!dealerNeedsCard())
        return;

    dealerHand.push_back(drawCard());
    message = "Dealer drew a card.";
}

void BlackjackGame::settleNormalRound()
{
    if (!roundActive)
        return;

    int playerTotal = calculateHandValue(playerHand);
    int dealerTotal = calculateHandValue(dealerHand);

    dealerTurn = false;

    if (dealerTotal > 21)
    {
        chips += bet;
        wins++;
        message = "Dealer bust! You win.";
    }
    else if (playerTotal > dealerTotal)
    {
        chips += bet;
        wins++;
        message = "You win!";
    }
    else if (playerTotal < dealerTotal)
    {
        chips -= bet;
        losses++;
        message = "Dealer wins.";
    }
    else
    {
        ties++;
        message = "Tie.";
    }

    endRound();
}

void BlackjackGame::settleBlackjack()
{
    bool playerBJ = isBlackjack(playerHand);
    bool dealerBJ = isBlackjack(dealerHand);

    if (playerBJ && dealerBJ)
    {
        ties++;
        message = "Both Blackjack! Tie.";
    }
    else if (playerBJ)
    {
        int payout = bet + bet / 2;
        if (payout == 0)
            payout = bet;

        chips += payout;
        wins++;
        message = "BLACKJACK! You win 3:2 payout.";
    }
    else
    {
        chips -= bet;
        losses++;
        message = "Dealer has Blackjack. You lose.";
    }

    endRound();
}

void BlackjackGame::endRound()
{
    roundActive = false;
    dealerTurn = false;
    round++;

    if (chips <= 0)
    {
        message += " GAME OVER. No chips left.";
    }
    else if (chips >= 20)
    {
        message += " Finish! You reached $20 or more!";
    }
}

bool BlackjackGame::canDoubleDown() const
{
    return roundActive && !dealerTurn && playerHand.size() == 2 && chips >= bet * 2 && !doubleDownUsed;
}

int BlackjackGame::getPlayerTotal() const
{
    return calculateHandValue(playerHand);
}

int BlackjackGame::getDealerTotal() const
{
    if (dealerHidden)
    {
        return dealerHand.empty() ? 0 : dealerHand[0].value;
    }
    return calculateHandValue(dealerHand);
}
