#ifndef CARDS_H
#define CARDS_H

#include <string>
#include <vector>
#include <iostream>
#include "../OrdersFiles/Orders.h"

class Deck; //Define Deck class for Card class
class Hand; //Define Hand class for Deck class

class Card
{
    public:
        //The type of the card can be "bomb", "reinforcement", "blockade", "airlift", "diplomacy"
        std::string cardType; 
        //Method called to play the card, which creates an order and adds it to the player's list of orders and then returns the card to the deck
        void play(OrdersList &ordersList, Deck &playingDeck, Hand &playingHand);
        //Default constructor
        Card();
        //Parametrized constructor
        Card(std::string cardType);
        //Copy constructor
        Card(const Card& card);
        //Assignment operator overload
        void operator=(const Card& card);
        //Stream insertion operator overload
        friend std::ostream& operator<<(std::ostream& COUT, const Card& CARD);

};

class Deck
{
    public:
        //Deck containing a set of warzone cards
        std::vector<Card> deckVector;
        //Draws a card from the deck
        void draw(Hand& hand);
        //Default constructor
        Deck();
        //Copy constructor
        Deck(const Deck& copyPlayingDeck);
        //Assignment operator overload
        void operator=(const Deck& playingDeck);
        //Stream insertion operator overload
        friend std::ostream& operator<<(std::ostream& COUT, Deck& DECK);
};


class Hand
{
    public:
        //Hand containing a set of Warzone cards
        std::vector<Card> handVector; 
        //Default constructor
        Hand();
        //Copy constructor
        Hand(const Hand& copyHand);
        //Assignment operator overload
        void operator=(const Hand& hand);
        //Stream insertion operator overload
        friend std::ostream& operator<<(std::ostream& COUT, Hand& HAND);
};

#endif