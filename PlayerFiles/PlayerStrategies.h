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
    virtual std::vector<Territory *> toDefend() = 0;
    virtual std::vector<Territory *> toAttack() = 0;
    virtual void issueOrder(Deck *deck) = 0;
    virtual ~PlayerStrategy() = default;
    explicit PlayerStrategy(Player* p) : player(p) {}

    bool isIssuingOrders() const;
    void setIssuingOrders(bool isIssuingOrders);

protected:
    Player* player;  // The player using this strategy
    bool issuingOrders = false;

    // Flags for strategy order issuing
    bool deploying = true;
    bool advancing = true;
    bool airlifting = true;
    bool blockading = true;
    bool bombing = true;
};

class HumanPlayerStrategy : public PlayerStrategy
{
public:
    explicit HumanPlayerStrategy(Player* p) : PlayerStrategy(p) {}

    std::vector<Territory *> toDefend() override;
    std::vector<Territory *> toAttack() override;

    void issueOrder(Deck *deck) override;
};

class AggressivePlayerStrategy : public PlayerStrategy
{
public:
    explicit AggressivePlayerStrategy(Player* p) : PlayerStrategy(p) {}

    std::vector<Territory *> toDefend() override;
    std::vector<Territory *> toAttack() override;
    void issueOrder(Deck *deck) override;
};

class BenevolentPlayerStrategy : public PlayerStrategy
{
public:
    explicit BenevolentPlayerStrategy(Player* p) : PlayerStrategy(p) {}

    std::vector<Territory *> toDefend() override;
    std::vector<Territory *> toAttack() override;
    void issueOrder(Deck *deck) override;
};

class NeutralPlayerStrategy : public PlayerStrategy
{
public:
    explicit NeutralPlayerStrategy(Player* p) : PlayerStrategy(p) {}

    std::vector<Territory *> toDefend() override;
    std::vector<Territory *> toAttack() override;
    void issueOrder(Deck *deck) override;
};

class CheaterPlayerStrategy : public PlayerStrategy
{

public:
    explicit CheaterPlayerStrategy(Player* p) : PlayerStrategy(p) {}

    std::vector<Territory *> toDefend() override;
    std::vector<Territory *> toAttack() override;
    void issueOrder(Deck *deck) override;
};

#endif
