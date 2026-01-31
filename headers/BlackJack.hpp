#include "Game.hpp"
#include "important.hpp"
#include "Deck.hpp"
#include "Player.hpp"
#include <map>

class BlackJack : Game {

    std::vector<std::shared_ptr<Player>> _players;
    const int _deck_capacity = static_cast<int>(GAME_LIMIT::DECK_SIZE_BJ); // describes how many decks you can have in game 8*52 cuz 1 deck is 52 cards
    std::map<int, std::pair<int, int>> _player_hand_val;
    std::map<int, std::pair<bool, bool>> _player_bj_insurance;
    const int _start_points = BET_LIMIT::START_POINTS;
    bool _bets_available {false};

    void _add_player(std::string, int);
    void _add_player(int);
    void _show_players() const;
    void _create_deck(std::shared_ptr<Deck>);
    void _first_draw(std::shared_ptr<Deck>);
    void _player_draw(std::shared_ptr<Player>, std::shared_ptr<Deck>);
    void _calculate_val(std::shared_ptr<Player>, std::shared_ptr<Card>);
    void _calculate_hand_val(std::shared_ptr<Player>);
    void _print_hand_val(std::shared_ptr<Player>);
    void _dealer_draw(std::shared_ptr<Player>, std::shared_ptr<Deck>);
    void _print_dealer_hand(std::shared_ptr<Player>);
    void _print_results(std::shared_ptr<Player>);
    void _check_bj(std::shared_ptr<Player>);
    void _ask_insurance();
    void _clear_all();
    void _ask_bet();

    public:
    BlackJack();
    BlackJack(int);

    virtual void play();
    virtual int get_id() const noexcept;
    virtual int get_max_players() const noexcept;
    virtual void set_id(int) noexcept;
};