#include <iostream>
#include <cstdlib>
#include <ctime>
#include "Cards.h"
#include "../OrdersFiles/Orders.h"


// <<<< Card Class Definitions >>>> 


//Method called to play the card, which creates an order and adds it to the player's list of orders and then returns the card to the deck
void Card::play(OrdersList &ordersList, Deck &playingDeck, Hand &playingHand)
{
    if(playingHand.handVector.size() == 0) //If hand is empty
    {
        std::cout << "Hand is empty, no card available to play" << std::endl;
    }
    else //If hand has at least a card
    {
        Order tempOrder;//Create a new order
        /*Add tempOrder to the Orders List of the player,
        this message is temporary since OrderFiles hasn't been
        worked on yet.
        */ 
        playingDeck.deckVector.push_back(*this); //Return the played card at the end of the deck
        playingHand.handVector.pop_back(); //Remove the card from the player's hand
    }
}

//Default constructor
//>>>> Do we need a default constructor for Card class?? <<<<
Card::Card()
{
    cardType = "undefinedCardType";
}

//Parametrized constructor
Card::Card(std::string cardType)
{
    this->cardType = cardType;
}

//Copy constructor
Card::Card(const Card& card)
{
    this->cardType = card.cardType;
}

//Assignment operator overload
void Card::operator=(const Card& card)
{
    this->cardType = card.cardType;
}

//Card Stream insertion operator overload
std::ostream& operator<<(std::ostream& COUT, const Card& CARD)
{
    COUT << "Card type: " << CARD.cardType << std::endl;
    return COUT;
}


// <<<< Deck Class Definitions >>>> 


//Draws a card from the deck
void Deck::draw(Hand& hand)
{
    /* <<<< Note >>>>
       TODO: We should check the condition if the deck isn't empty before randomly choosing a card
    */
    if(this->deckVector.size() == 0)
    {
        std::cout << "Deck is empty, no card available to draw\n" << std::endl;
    }
    else
    {
        srand(time(0)); //Generate seed based on computer time for a random number when using rand()
        int index = rand() % this->deckVector.size(); //Generate a random number between 0 and (DECK_VECTOR_SIZE - 1)
        hand.handVector.push_back(this->deckVector[index]); //Append randomly chosen card to the player's hand
        this->deckVector.erase(this->deckVector.begin() + index); //Remove randomly chosen card from the deck.
    }
}

//Default constructor
Deck::Deck()
{
    //Initializing empty deck
}

//Copy constructor
Deck::Deck(const Deck& copyDeck)
{
    for(int i = 0; i < copyDeck.deckVector.size(); i++)
    {
        this->deckVector.push_back(copyDeck.deckVector[i]);//Append all contents of passed deck to our deck
    }
}

//Assignment operator overload
void Deck::operator=(const Deck& deck)
{
    this->deckVector.clear(); //Clear the contents of our deck
    for(int i = 0; i < deck.deckVector.size(); i++)
    {
        this->deckVector.push_back(deck.deckVector[i]);//Append all contents of passed deck to our deck
    }
}

//Deck Stream insertion operator overload
std::ostream& operator<<(std::ostream& COUT, Deck& DECK)
{
    if(DECK.deckVector.size() == 0) //If deck is empty
    {
        COUT << " <<< Current Deck >>>\n";
        COUT << "Deck is empty...\n" << std::endl;
        return COUT;
    }
    else //If deck has cards
    {
        COUT << " <<< Current Deck: "<< DECK.deckVector.size() << " Card(s) >>> \n";
        for(int i = 0; i < DECK.deckVector.size(); i++)
        {
            COUT << DECK.deckVector[i];
        }
        COUT << std::endl;

        return COUT;
    }
}


// <<<< Hand Class Definitions >>>> 


//Default constructor
Hand::Hand()
{
    //Initializing empty hand
}

//Copy constructor
Hand::Hand(const Hand& copyHand)
{
    for(int i = 0; i < copyHand.handVector.size(); i++)
    {
        this->handVector.push_back(copyHand.handVector[i]);//Append all contents of passed hand to our hand
    }
}

//Assignment operator overload
void Hand::operator=(const Hand& hand)
{
    this->handVector.clear(); //Clear the contents of our hand
    for(int i = 0; i < hand.handVector.size(); i++)
    {
        this->handVector.push_back(hand.handVector[i]);//Append all contents of passed hand to our hand
    }
}

//Hand Stream insertion operator overload
std::ostream& operator<<(std::ostream& COUT, Hand& HAND)
{
    if(HAND.handVector.size() == 0) //If hand is empty
    {
        COUT << " <<< Player's Hand >>>>\n";
        COUT << "Hand is empty...\n" << std::endl;
        return COUT;
    }
    else //If hand has cards
    {
        COUT << " <<< Player's Hand >>>>\n";
        for(int i = 0; i < HAND.handVector.size(); i++)
        {
            COUT << HAND.handVector[i];
        }
        COUT << std::endl;

        return COUT;
    }  
}
