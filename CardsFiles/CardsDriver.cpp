#include <iostream>
#include <cstdlib>
#include <string>
#include "Cards.h"
#include "../OrdersFiles/Orders.h"


/*
A free function named testCards() that creates a deck of cards of all different kinds, then
creates a hand object that is filled by drawing cards from the deck by repeatedly calling its
draw() method, then calls the play() method of all cards in the hand, resulting in the cards
being put back in the deck.
*/
void testCards()
{
    std::cout << " <<< Creation of deck... >>> \n\n";
    Deck deck_of_cards; //Create a deck 
    int size_of_deck = 10; //Determine a size for the deck

    //String array with all possible types of cards
    std::string type_of_cards[] = {"bomb", "reinforcement", "blockade", "airlift", "diplomacy"}; 

    srand(time(0)); //Generate a random seed for the rand() function.
                    //Time(0) is using the system's time to generate different seeds.

    
    for(int i = 0; i < size_of_deck; i++)
    {
        int choose_random_card_type = rand() % 5; //A random index from 0 to 4 (all 5 possible card types)
                                                  //using rand() to generage the deck of cards
        
        Card tempCard(type_of_cards[choose_random_card_type]); //Create a random card type that
                                                               //will be added to the deck
        
        deck_of_cards.deckVector.push_back(tempCard); //Add current tempCard to the deck of cards
    }

    std::cout << deck_of_cards; //Display the deck

    std::cout << " <<< Creation of a hand... >>> \n\n";

    Hand hand_holding_cards; //Create a hand

    std::cout << " <<< Drawing 5 cards from the deck... >>> \n\n";

    for(int i = 0; i < 5; i++) //Call draw() 5 times to draw 5 cards at random.
    {
        deck_of_cards.draw(hand_holding_cards);
    }

    std::cout << hand_holding_cards; //Display the hand after drawing 5 random cards

    std::cout << deck_of_cards; //Display the deck after 5 random cards have been drawn

    std::cout << " <<< Playing all 5 cards from the hand... >>> \n\n";

    //Call play() on all cards in the hand to "play" them, which then places them back
    //in the deck
    for(int i = 0; i < 5; i++) 
    {
        OrdersList tempOrderList; //Create a tempOrderList
        hand_holding_cards.handVector[i].play(tempOrderList, deck_of_cards, hand_holding_cards);
    }

    std::cout << hand_holding_cards; //Display the hand after playing all 5 cards

    std::cout << deck_of_cards; //Display the deck after 5 cards have been played

}

int main()
{
    testCards();
    
    return 0;
}

