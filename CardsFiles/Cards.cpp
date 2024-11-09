#include <iostream>
#include <cstdlib>
#include <ctime>
#include "Cards.h"

// <<<< Card Class Definitions >>>>

const std::vector<std::string> cardTypes = {"Bomb", "Reinforcement", "Blockade", "Airlift", "Diplomacy"};


/**
 * Method that plays the card, which creates an order and adds it 
 * to the player's list of orders and then returns the card to the deck.
 * 
 * @param ordersList The orders list to which the created order will be added to.
 * @param playingDeck The deck to which the card will be returned to.
 * @param playingHand The hand from which the card is played.
 * @param cardOrder A string representing the type of order to be created from the card.
 */
void Card::play(OrdersList &ordersList, Deck &playingDeck, Hand &playingHand, std::string cardOrder)
{
    if(playingHand.handVector.size() == 0) ///< If hand is empty.
    {
        std::cout << "Hand is empty, no card available to play" << std::endl;
    }
    else ///< If hand has at least a card.
    {
        // Stores the name of the source territory for the order.
        std::string sourceTName = "";
        // Stores the name of the target territory for the order.
        std::string targetTName = "";
        // Stores the number of army units involved in the order.
        int armies = 0;

        // Check if the order to be issued is an "airlift" type.
        if(cardOrder == "airlift")
        {
            // Prompt the player to choose a source territory for the airlift.
            std::cout << "\nChoose a source territory (army units should be on standby there): ";
            // Get the name of the source territory.
            std::cin >> sourceTName;

            // Prompt the player to choose a target territory to move to with the airlift.
            std::cout << "\nChoose a target territory to move to: ";
            // Get the name of the target territory.
            std::cin >> targetTName;

            // Prompt the player to enter the number of army units to be moved by airlift.
            std::cout << "\nEnter the number of army units to execute an airlift order: ";
            // Get the number of army units for the airlift.
            std::cin >> armies;

            // Create a new AirliftOrder object and add it to the orders list.
            ordersList.ordersVector.push_back(new AirliftOrder(sourceTName, targetTName, armies)); // Need to have some logic that registers the details from the player's input
        }
        // Check if the order to be issued is a "bomb" type.
        else if(cardOrder == "bomb")
        {
            // Prompt the player to choose a target territory to execute a bomb order.
            std::cout << "\nChoose a target territory to execute a bomb order: ";
            // Get the name of the target territory for the bomb.
            std::cin >> targetTName;

            // Create a new BombOrder object and add it to the orders list.
            ordersList.ordersVector.push_back(new BombOrder(targetTName)); // Need to have some logic that registers the details from the player's input
        }
        // Check if the order to be issued is a "blockade" type.
        else if(cardOrder == "blockade")
        {
            // Prompt the player to choose a target territory for the blockade.
            std::cout << "\nChoose a target territory to execute a blockade order: ";
            // Get the name of the target territory for the blockade.
            std::cin >> targetTName;

            // Create a new BlockadeOrder object and add it to the orders list.
            ordersList.ordersVector.push_back(new BlockadeOrder(targetTName)); // Need to have some logic that registers the details from the player's input
        }
        // Check if the order to be issued is a "negotiate" type.
        else if(cardOrder == "negotiate")
        {
            // Declare a variable to store the name of the target player for negotiation.
            std::string targetPName;

            // Prompt the player to choose the target player for the negotiate order.
            std::cout << "\nChoose a target player to execute a negotiate order: ";
            // Get the name of the target player.
            std::cin >> targetPName;

            // Create a new NegotiateOrder object and add it to the orders list.
            ordersList.ordersVector.push_back(new NegotiateOrder(targetPName)); // Need to have some logic that registers the details from the player's input
        }

        playingDeck.deckVector.push_back(this); ///< Return the pointer of the played card at the end of the deck.
        playingHand.handVector.pop_back(); ///< Remove the pointer of the card from the player's hand.
    }
}

/**
 * Default constructor for the Card class, creates a card with a random type.
 */
Card::Card()
{
   //assign cartType at random
    cardType = cardTypes[rand() % cardTypes.size()];
}

/**
 * Parametrized constructor for the Card class, creates a card with the specified type.
 *
 * @param cardType Type of the card to create.
 */
Card::Card(std::string cardType) : cardType(std::move(cardType)) {}

// Copy constructor and assignment operator
Card::Card(const Card &card) : cardType(card.cardType) {}
void Card::operator=(const Card &card) { cardType = card.cardType; }

std::ostream &operator<<(std::ostream &COUT, const Card &CARD)
{
    COUT << "Card type: " << CARD.cardType << std::endl;
    return COUT;
}

// Getters and setters for card type
std::string Card::getCardType() { return cardType; }
void Card::setCardType(std::string newCardType) { cardType = std::move(newCardType); }

// <<<< Deck Class Definitions >>>>

/**
 * Default constructor for the Deck class, initializes deck with random cards.
 */
Deck::Deck()
{
    for (int i = 0; i < 50; ++i) // Arbitrary deck size
    {
        deckVector.push_back(new Card()); // Create a card with a random type
    }
}

/**
 * Draws a card from the deck and places it in the specified hand.
 *
 * @param hand The hand to which the drawn card will be added.
 */
void Deck::draw(Hand &hand)
{
    if (deckVector.empty())
    {
        std::cout << "Deck is empty, no card available to draw\n"
                  << std::endl;
    }
    else
    {
        int index = rand() % deckVector.size();
        hand.handVector.push_back(deckVector[index]); // Add card to hand
        deckVector.erase(deckVector.begin() + index); // Remove card from deck
    }
}

/**
 * Overloaded Copy constructor of the Deck class, creates a deck given
 * another instance of a deck. This is a deep copy.
 * 
 * @param copyPlayingDeck Deck instance that is being copied from.
 */
Deck::Deck(const Deck& copyPlayingDeck)
{
    for (const auto *card : copyPlayingDeck.deckVector)
    {
        deckVector.push_back(new Card(*card));
    }
}

void Deck::operator=(const Deck &playingDeck)
{
    for (auto *card : deckVector)
    {
        delete card;
    }
    deckVector.clear();

    for (const auto *card : playingDeck.deckVector)
    {
        deckVector.push_back(new Card(*card));
    }
}

// Overloaded stream insertion operator
std::ostream &operator<<(std::ostream &COUT, Deck &DECK)
{
    if (DECK.deckVector.empty())
    {
        COUT << " <<< Current Deck >>>\nDeck is empty...\n"
             << std::endl;
    }
    else
    {
        COUT << " <<< Current Deck: " << DECK.deckVector.size() << " Card(s) >>> \n";
        for (const auto *card : DECK.deckVector)
        {
            COUT << *card;
        }
        COUT << std::endl;
    }
    return COUT;
}

// Deck destructor
Deck::~Deck()
{
    for (auto *card : deckVector)
    {
        delete card;
    }
    deckVector.clear();
}

// <<<< Hand Class Definitions >>>>

// Default constructor
Hand::Hand() = default;

// Copy constructor and assignment operator
Hand::Hand(const Hand &copyHand)
{
    for (const auto *card : copyHand.handVector)
    {
        handVector.push_back(new Card(*card));
    }
}

/**
 * Overloaded Assignment operator which assigns the hand type of the assigning
 * hand instance to the assigned hand instance.
 *
 * @param hand Hand instance that is being assigned from.
 */
void Hand::operator=(const Hand& hand)
{
    for (auto *card : handVector)
    {
        delete card;
    }
    handVector.clear();

    for (const auto *card : hand.handVector)
    {
        handVector.push_back(new Card(*card));
    }
}

/**
 * Overloaded Stream insertion operator which outputs the whole
 * hand.
 * 
 * @param COUT The output stream object.
 * @param HAND The card instance that is being outputted.
 */
std::ostream& operator<<(std::ostream& COUT, Hand& HAND)
{
    if (HAND.handVector.empty())
    {
        COUT << " <<< Player's Hand >>>>\nHand is empty...\n"
             << std::endl;
    }
    else
    {
        COUT << " <<< Player's Hand >>>>\n";
        for (const auto *card : HAND.handVector)
        {
            COUT << *card;
        }
        COUT << std::endl;
    }
    return COUT;
}

// Hand destructor
Hand::~Hand()
{
    for (auto *card : handVector)
    {
        delete card;
    }
    handVector.clear();
}
