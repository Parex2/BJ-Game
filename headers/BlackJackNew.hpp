#pragma once
#include "Deck.hpp"
#include "Game.hpp"
#include "Player.hpp"
#include "important.hpp"
#include <array>


class BlackJackNew : Game {
    struct Token {
        std::shared_ptr<Token> next;
        std::shared_ptr<Token> previous;
        std::shared_ptr<Player> player;
        std::shared_ptr<Token> parent {nullptr};
        int min_value {0};
        int max_value {0};
        bool insurance {false};
        bool bj {false};
        int final_value {0};
        int result {BJ::DRAW};
        int insurance_bet {0};
        bool soft_seventeen {false};
        Token() = default;
        Token(std::shared_ptr<Player>);
        ~Token() = default;
    };

    std::shared_ptr<Token> _head;
    std::shared_ptr<Token> _last_token;
    int _player_count;
    std::array<std::shared_ptr<Player>, BJ::MAX_PLAYERS_WITH_DEALER> _players = {};
    std::shared_ptr<Deck> _deck;
    bool _bets_on {false};
    

    std::shared_ptr<Token> _create_token(std::shared_ptr<Player>);
    void _add_token(std::shared_ptr<Token>);
    void _add_in_place(std::shared_ptr<Token>, std::shared_ptr<Token>);
    void _count_max(int&, std::shared_ptr<Card>, bool&);
    void _count_min(int&, std::shared_ptr<Card>);
    void _first_draw(std::shared_ptr<Deck>);
    void _ask_bet();
    void _print_sumarry_bets();
    void _print_first_hand();
    void _ask_insurance();
    void _ask_split(std::shared_ptr<Token>);
    void _players_draw(std::shared_ptr<Deck>);
    void _count(std::shared_ptr<Token>);
    void _print_total(std::shared_ptr<Token>);
    void _clear_all();
    void _delete(std::shared_ptr<Token>);
    void _get_results();
    void _result(std::shared_ptr<Token>, int);
    void _return_insurance(std::shared_ptr<Token>);
    void _get_final_value(std::shared_ptr<Token>);
    void _print_results();
    void _dealer_draw(std::shared_ptr<Token>, std::shared_ptr<Deck>);
    void _print_dealer_hand();
    

    public:
    BlackJackNew() = default;
    BlackJackNew(int);
    ~BlackJackNew() = default;

    virtual void play();
    virtual int get_id() const noexcept;
    virtual int get_max_players() const noexcept; //to be removed
    virtual void set_id(int) noexcept;

};