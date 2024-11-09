#include <iostream>
#include <cstdlib>
#include <ctime>
#include "Cards.h"


// <<<< Card Class Definitions >>>> 


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
 * Default constructor of the Card class, creates a default card with "undefinedCardType"
 * for its card type.
 */
Card::Card()
{
    cardType = "undefinedCardType";
}

/**
 * Parametrized constructor of the Card class, creates a card with a given
 * card type as its parameter.
 * 
 * @param cardType Given card type to the card instance.
 */
Card::Card(std::string cardType)
{
    this->cardType = cardType;
}

/**
 * Overloaded Copy constructor of the Card class, creates a card given
 * another instance of a card. This is a deep copy.
 * 
 * @param card Card instance that is being copied from.
 */
Card::Card(const Card& card)
{
    this->cardType = card.cardType;
}

/**
 * Overloaded Assignment operator which assigns the card type of the assigning
 * card instance to the assigned card instance.
 * 
 * @param card Card instance that is being assigned from.
 */
void Card::operator=(const Card& card)
{
    this->cardType = card.cardType;
}

/**
 * Overloaded Stream insertion operator which outputs the card type 
 * of the card instance.
 * 
 * @param COUT The output stream object.
 * @param CARD The card instance that is being outputted.
 */
std::ostream& operator<<(std::ostream& COUT, const Card& CARD)
{
    COUT << "Card type: " << CARD.cardType << std::endl;
    return COUT;
}


// <<<< Deck Class Definitions >>>> 


/**
 * Method that draws a card from the deck and places
 * it in the hand of the given player.
 * 
 * @param hand The hand in which the drawn card will be placed.
 */
void Deck::draw(Hand& hand)
{
    if(this->deckVector.size() == 0)
    {
        std::cout << "Deck is empty, no card available to draw\n" << std::endl;
    }
    else
    {
        srand(time(0)); ///< Generate seed based on computer time for a random number when using rand().
        int index = rand() % this->deckVector.size(); ///< Generate a random number between 0 and (DECK_VECTOR_SIZE - 1).
        hand.handVector.push_back(this->deckVector[index]); ///< Append randomly chosen card to the player's hand.
        this->deckVector.erase(this->deckVector.begin() + index); ///< Remove randomly chosen card from the deck.
    }
}

/**
 * Default constructor of the Deck class, creates a default deck with 
 * no cards in it.
 */    
Deck::Deck()
{
    ///< Initializing empty deck
}

/**
 * Overloaded Copy constructor of the Deck class, creates a deck given
 * another instance of a deck. This is a deep copy.
 * 
 * @param copyPlayingDeck Deck instance that is being copied from.
 */
Deck::Deck(const Deck& copyPlayingDeck)
{
    for(int i = 0; i < copyPlayingDeck.deckVector.size(); i++)
    {
        this->deckVector.push_back(new Card(*copyPlayingDeck.deckVector[i])); ///< Append all contents of passed deck to our deck.
    }
}

/**
 * Overloaded Assignment operator which assigns the deck type of the assigning
 * deck instance to the assigned deck instance.
 * 
 * @param playingDeck Deck instance that is being assigned from.
 */
void Deck::operator=(const Deck& playingDeck)
{
    for (Card* cardPointer : deckVector){ ///< Free memory leaks by freeing each card instance.
        delete cardPointer;
    }
    this->deckVector.clear(); ///< Clear the contents of our deck.
    for(int i = 0; i < playingDeck.deckVector.size(); i++)
    {
        this->deckVector.push_back(new Card(*playingDeck.deckVector[i])); ///< Append all contents of passed deck to our deck.
    }
}

/**
 * Overloaded Stream insertion operator which outputs the whole
 * deck of cards
 * 
 * @param COUT The output stream object.
 * @param DECK The card instance that is being outputted.
 */
std::ostream& operator<<(std::ostream& COUT, Deck& DECK)
{
    if(DECK.deckVector.size() == 0) ///< If deck is empty.
    {
        COUT << " <<< Current Deck >>>\n";
        COUT << "Deck is empty...\n" << std::endl;
        return COUT;
    }
    else ///< If deck has cards.
    {
        COUT << " <<< Current Deck: "<< DECK.deckVector.size() << " Card(s) >>> \n";
        for(int i = 0; i < DECK.deckVector.size(); i++)
        {
            COUT << *(DECK.deckVector[i]); ///< Dereference the card pointer to print out the card.
        }
        COUT << std::endl;

        return COUT;
    }
}

/**
 * Destructor for the Hand class. Frees all dynamically allocated memory used for Cards.
 */
Deck::~Deck() {
    for (Card* cardPointer : deckVector){ ///< Free memory leaks by freeing each card instance.
        delete cardPointer;
    }
    deckVector.clear(); ///< Reset the deck to size 0.
}


// <<<< Hand Class Definitions >>>> 


/**
 * Default constructor of the Hand class, creates a default hand with 
 * no cards in it.
 */  
Hand::Hand()
{
    ///< Initializing empty hand
}

/**
 * Overloaded Copy constructor of the Hand class, creates a hand given
 * another instance of a hand. This is a deep copy.
 * 
 * @param copyHand Hand instance that is being copied from.
 */
Hand::Hand(const Hand& copyHand)
{
    for(int i = 0; i < copyHand.handVector.size(); i++)
    {
        this->handVector.push_back(new Card(*copyHand.handVector[i])); ///< Append all contents of passed hand to our hand.
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
    for (Card* cardPointer : handVector){ ///< Free memory leaks by freeing each card instance.
        delete cardPointer;
    }
    this->handVector.clear(); ///< Clear the contents of our hand.
    for(int i = 0; i < hand.handVector.size(); i++)
    {
        this->handVector.push_back(new Card(*hand.handVector[i])); ///< Append all contents of passed hand to our hand.
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
    if(HAND.handVector.size() == 0) ///< If hand is empty.
    {
        COUT << " <<< Player's Hand >>>>\n";
        COUT << "Hand is empty...\n" << std::endl;
        return COUT;
    }
    else ///< If hand has cards.
    {
        COUT << " <<< Player's Hand >>>>\n";
        for(int i = 0; i < HAND.handVector.size(); i++)
        {
            COUT << *(HAND.handVector[i]) << std::endl; ///< Dereference the card pointer to print out the card.
        }
        COUT << std::endl;

        return COUT;
    }  
}

/**
 * Destructor for the Hand class. Frees all dynamically allocated memory used for Cards.
 */
Hand::~Hand() {
    for (Card* cardPointer : handVector){ ///< Prevent memory leaks by deleting each card instance.
        delete cardPointer;
    }
    handVector.clear(); ///< Reset the hand to size 0.
}
