#ifndef CARDS_H
#define CARDS_H

#include <string>
#include <vector>
#include <iostream>
#include "../OrdersFiles/Orders.h"

/**
 * The Deck class is defined for the compiler to be used by the Card class.
 */
class Deck;

/**
 * The Hand class is defined for the compiler to be used by the Deck class.
 */
class Hand;

/**
 * The Card class represents a card in the game. A card can have only one type.
 * Possible types of cards {Bomb, Reinforcement, Blockade, Airlift, Diplomacy}.
 * 
 * Notice: < Diplomacy > is equivalent to the < Negotiate > order.
 * 
 * This Card class contains its card type, a play method which allows the card instance
 * to be played and overloaded operators and constructors.
 */
class Card
{
    public:
        std::string cardType; ///< The type that the card instance holds.
        
        /**
         * Method that plays the card, which creates an order and adds it 
         * to the player's list of orders and then returns the card to the deck.
         * 
         * @param ordersList The orders list to which the created order will be added to.
         * @param playingDeck The deck to which the card will be returned to.
         * @param playingHand The hand from which the card is played.
         */
        void play(OrdersList &ordersList, Deck &playingDeck, Hand &playingHand);

        /**
         * Default constructor of the Card class, creates a default card with "undefinedCardType"
         * for its card type.
         */
        Card();

        /**
         * Parametrized constructor of the Card class, creates a card with a given
         * card type as its parameter.
         * 
         * @param cardType Given card type to the card instance.
         */
        Card(std::string cardType);

        /**
         * Overloaded Copy constructor of the Card class, creates a card given
         * another instance of a card. This is a deep copy.
         * 
         * @param card Card instance that is being copied from.
         */
        Card(const Card& card);

        /**
         * Overloaded Assignment operator which assigns the card type of the assigning
         * card instance to the assigned card instance.
         * 
         * @param card Card instance that is being assigned from.
         */
        void operator=(const Card& card);

        /**
         * Overloaded Stream insertion operator which outputs the card type 
         * of the card instance.
         * 
         * @param COUT The output stream object.
         * @param CARD The card instance that is being outputted.
         */
        friend std::ostream& operator<<(std::ostream& COUT, const Card& CARD);

};

/**
 * The Deck class represents a deck of cards in the game. A deck has a vector that
 * holds multiple card pointers and a card can be drawn with the draw method. It also contains 
 * its respective overloaded operators and constructors.
 */
class Deck
{
    public:
        std::vector<Card*> deckVector; ///< A vector that contains a pointer to each card in the deck.

        /**
         * Method that draws a card from the deck and places
         * it in the hand of the given player.
         * 
         * @param hand The hand in which the drawn card will be placed.
         */
        void draw(Hand& hand);

        /**
         * Default constructor of the Deck class, creates a default deck with 
         * no cards in it.
         */     
        Deck();

        /**
         * Overloaded Copy constructor of the Deck class, creates a deck given
         * another instance of a deck. This is a deep copy.
         * 
         * @param copyPlayingDeck Deck instance that is being copied from.
         */
        Deck(const Deck& copyPlayingDeck);

        /**
         * Overloaded Assignment operator which assigns the deck type of the assigning
         * deck instance to the assigned deck instance.
         * 
         * @param playingDeck Deck instance that is being assigned from.
         */
        void operator=(const Deck& playingDeck);


        /**
         * Overloaded Stream insertion operator which outputs the whole
         * deck of cards
         * 
         * @param COUT The output stream object.
         * @param DECK The card instance that is being outputted.
         */
        friend std::ostream& operator<<(std::ostream& COUT, Deck& DECK);
};

/**
 * The Hand class represents a hand of a player. A hand has a vector that contains
 * the cards that a player holds. It also contains its respective overloaded 
 * operators and constructors.
 */
class Hand
{
    public:
        
        std::vector<Card*> handVector; ///< A vector that contains a pointer to each card in the hand.
        
        /**
         * Default constructor of the Hand class, creates a default hand with 
         * no cards in it.
         */  
        Hand();

        /**
         * Overloaded Copy constructor of the Hand class, creates a hand given
         * another instance of a hand. This is a deep copy.
         * 
         * @param copyHand Hand instance that is being copied from.
         */
        Hand(const Hand& copyHand);

        /**
         * Overloaded Assignment operator which assigns the hand type of the assigning
         * hand instance to the assigned hand instance.
         * 
         * @param hand Hand instance that is being assigned from.
         */ 
        void operator=(const Hand& hand);

        /**
         * Overloaded Stream insertion operator which outputs the whole
         * hand.
         * 
         * @param COUT The output stream object.
         * @param HAND The card instance that is being outputted.
         */
        friend std::ostream& operator<<(std::ostream& COUT, Hand& HAND);
};

#endif