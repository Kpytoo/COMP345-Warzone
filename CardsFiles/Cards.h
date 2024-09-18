#include <string>
#include <vector>


class Deck
{
public:



};


class Card
{
public:
    std::string cardType; //The type of the card can be "bomb", "reinforcement", "blockade", "airlift", "diplomacy"
    Card(){}

    void play(){ //method called to play the card, which creates an order and adds it to the player's list of orders and then returns the card to the deck

    }

};

class Hand
{
public:
    std::vector <Card> cardsInHand; //Cards that a player has in hand
};