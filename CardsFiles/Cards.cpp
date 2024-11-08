#include <iostream>
#include <cstdlib>
#include <ctime>
#include "Cards.h"

// <<<< Card Class Definitions >>>>

const std::vector<std::string> cardTypes = {"Bomb", "Reinforcement", "Blockade", "Airlift", "Diplomacy"};

/**
 * Default constructor for the Card class, creates a card with a random type.
 */
Card::Card()
{
    // Seed the random generator only once (in main or some setup function)
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
    std::srand(static_cast<unsigned int>(std::time(nullptr))); // Seed RNG
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

// Copy constructor and assignment operator
Deck::Deck(const Deck &copyPlayingDeck)
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

void Hand::operator=(const Hand &hand)
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

// Overloaded stream insertion operator
std::ostream &operator<<(std::ostream &COUT, Hand &HAND)
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
