#ifndef PLAYER_STRATEGIES_H
#define PLAYER_STRATEGIES_H
#include "CardsFiles/Cards.h"
#include "MapFiles/Map.h"
// Forward declaration
class Player;

// Abstract Strategy class
class PlayerStrategy
{
public:
    virtual std::vector<Territory *> toDefend(Player *player) = 0;
    virtual std::vector<Territory *> toAttack(Player *player) = 0;
    virtual void issueOrder(Player *player, const std::string &orderType, Deck *deck) = 0;
    virtual ~PlayerStrategy() = default;
};

class HumanPlayerStrategy : public PlayerStrategy
{
public:
    std::vector<Territory *> toDefend(Player *player) override;
    std::vector<Territory *> toAttack(Player *player) override;

    void issueOrder(Player *player, Deck *deck);

    void issueOrder(Player *player, const std::string &orderType, Deck *deck) override;
};

class AggressivePlayerStrategy : public PlayerStrategy
{
public:
    std::vector<Territory *> toDefend(Player *player) override;
    std::vector<Territory *> toAttack(Player *player) override;
    void issueOrder(Player *player, const std::string &orderType, Deck *deck) override;
};

class BenevolentPlayerStrategy : public PlayerStrategy
{
public:
    std::vector<Territory *> toDefend(Player *player) override;
    std::vector<Territory *> toAttack(Player *player) override;
    void issueOrder(Player *player, const std::string &orderType, Deck *deck) override;
};

class NeutralPlayerStrategy : public PlayerStrategy
{
public:
    std::vector<Territory *> toDefend(Player *player) override;
    std::vector<Territory *> toAttack(Player *player) override;
    void issueOrder(Player *player, const std::string &orderType, Deck *deck) override;
};

class CheaterPlayerStrategy : public PlayerStrategy
{
private:
    std::vector<Player *> *players;

public:
    CheaterPlayerStrategy::CheaterPlayerStrategy(std::vector<Player *> *players) : players(players) {}
    std::vector<Territory *> toDefend(Player *player) override;
    std::vector<Territory *> toAttack(Player *player) override;
    void issueOrder(Player *player, const std::string &orderType, Deck *deck) override;
};

#endif
