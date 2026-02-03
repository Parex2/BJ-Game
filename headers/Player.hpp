#pragma once
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include "Card.hpp"
#include "IDisplay.hpp"

class Player : IDisplay {
    std::string _nickname {"Ryan"};
    std::vector<std::shared_ptr<Card>> _hand;
    bool _is_sorted {false};
    int _id {0};
    int _points {0};
    int _bet{-1};
    int _base_bet{-1};


    public:

    Player() = default;
    Player(std::string);
    Player(std::string, int);

    void sort_hand();
    void add_card(std::shared_ptr<Card>);
    virtual void show() const;
    std::string get_nickname() const;
    int get_id();
    int get_hand_size();
    const std::shared_ptr<Card> get_card(int);
    void clear_hand();
    void show_at(int) const;
    void set_points(int);
    int get_points();
    void add_points(int);
    void set_bet(int);
    int get_bet();
    void set_base_bet(int);
    int get_base_bet();
    std::shared_ptr<Card> remove_last();
    std::shared_ptr<Card> get_last();
    
};