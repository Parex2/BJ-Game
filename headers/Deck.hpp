#pragma once

#include "Card.hpp"
#include "IDisplay.hpp"
#include "important.hpp"
#include <random>
#include <vector>
#include <memory>
#include <algorithm>

class Deck : IDisplay {
    std::vector<std::shared_ptr<Card>> _deck;


    public:

    Deck();
    Deck(Deck&);
    ~Deck() = default;

    virtual void show() const;
    void create();
    void shuffle();
    bool add_card(std::shared_ptr<Card>);
    bool merge(std::shared_ptr<Deck>);
    int get_size();
    std::shared_ptr<Card> draw();
};