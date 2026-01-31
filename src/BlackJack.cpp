#include "../headers/BlackJack.hpp"

BlackJack::BlackJack() : Game(static_cast<int>(LIMIT::BJ)) {
    
}

BlackJack::BlackJack(int max_players) {
    _max_players = max_players < static_cast<int>(LIMIT::BJ) ? max_players : static_cast<int>(LIMIT::BJ);
}

int BlackJack::get_id() const noexcept {
    return _id;
}

int BlackJack::get_max_players() const noexcept {
    return _max_players;
}

void BlackJack::set_id(int id) noexcept{
    _id = id;
}

void BlackJack::_add_player(std::string nickname, int id){
    _players.push_back(std::make_shared<Player>(nickname, id));
    if(_players.back()->get_nickname() != "Dealer"){
        _players.back()->set_points(_start_points);
    }
    
}

void BlackJack::_add_player(int id){
    std::string nickname = "";

    
    while(nickname.empty()){
        std::cout << "Provide a nickname: ";
        std::getline(std::cin, nickname);
        if(nickname == "Dealer"){
            nickname = "";
            std::cout << "Dealer is only one! Try again!" << std::endl;
        }
    }
    
    _add_player(nickname, id);
}

void BlackJack::_show_players() const {
    for(auto& player : _players){
        std::cout << player->get_nickname() << std::endl;
        if(player->get_nickname() == "Dealer"){
            player->show_at(0);
        }else{
            player->show();
        }
        std::cout << std::endl;
    }
    
}

void BlackJack::_create_deck(std::shared_ptr<Deck> deck){
    for(int i = 0; i < _deck_capacity; ++i){
        deck->create();
    }
}

void BlackJack::_first_draw(std::shared_ptr<Deck> deck){
    for(int i = 0; i < (2 * _players.size()); ++i){
        if(_players.at(i % _players.size())->get_bet() > BET_LIMIT::MIN_BET || _players.at(i % _players.size())->get_nickname() == "Dealer"){
            _players.at(i % _players.size())->add_card(deck->draw());
        }
    }
}

void BlackJack::_calculate_val(std::shared_ptr<Player> player, std::shared_ptr<Card> card){
    if(card->get_figure() == FIGURE::ACE){
        _player_hand_val[player->get_id()].first += 11;
        if(_player_hand_val[player->get_id()].first > 21){
            _player_hand_val[player->get_id()].first -= 11;
            _player_hand_val[player->get_id()].first += 1;
        }
        _player_hand_val[player->get_id()].second += 1;
        return;
    }
    _player_hand_val[player->get_id()].first += static_cast<int>(card->get_figure()) > 10 ? 10 :  static_cast<int>(card->get_figure());
    _player_hand_val[player->get_id()].second += static_cast<int>(card->get_figure()) > 10 ? 10 :  static_cast<int>(card->get_figure());
}

void BlackJack::_calculate_hand_val(std::shared_ptr<Player> player){
    for(int i = 0; i < player->get_hand_size(); ++i){
        std::shared_ptr<Card> card = player->get_card(i);
        _calculate_val(player, card);
    }
}

void BlackJack::_check_bj(std::shared_ptr<Player> player){
    _player_bj_insurance[player->get_id()].first = _player_hand_val[player->get_id()].first == static_cast<int>(GAME_LIMIT::BLACK_JACK);
}

void BlackJack::_player_draw(std::shared_ptr<Player> player, std::shared_ptr<Deck> deck){
    std::cout << "----" << player->get_nickname() << "----" << std::endl;
    player->show();
    std::cout << std::endl;

    //_calculate_hand_val(player);
    _print_hand_val(player);
    //_check_bj(player);
    if(_player_bj_insurance[player->get_id()].first || _player_bj_insurance[_players.at(0)->get_id()].first){
        return;
    }
    while((_player_hand_val[player->get_id()].first <= static_cast<int>(GAME_LIMIT::BLACK_JACK) || _player_hand_val[player->get_id()].second <= static_cast<int>(GAME_LIMIT::BLACK_JACK))){
        std::string action;
        std::cout << "Choose action (d -> draw, p -> pass, dd -> double, s -> show points): ";
        std::getline(std::cin, action);
        if(action == "p"){
            break;
        }else if(action == "d" || action == "dd"){
            if(action == "dd"){
                if(player->get_points() < player->get_bet()){
                    std::cout << "You can not double because of your points balance!" << std::endl;
                    continue;
                }
                player->add_points(0 - player->get_bet());
                player->set_bet(2 * player->get_bet());
            }
            std::shared_ptr<Card> card = deck->draw();
            player->add_card(card);
            _calculate_val(player, card);
            player->show();
            std::cout << ' ';
            _print_hand_val(player);
            std::cout << std::endl;
            if(action == "dd"){
                break;
            }
        }else if(action == "s"){
            std::cout << "Balance: " << player->get_points() << std::endl;
        }
    }
}

void BlackJack::_dealer_draw(std::shared_ptr<Player> player, std::shared_ptr<Deck> deck){
    //_calculate_hand_val(player);
    while(_player_hand_val[player->get_id()].first < static_cast<int>(GAME_LIMIT::BJ_DEALER_STOP)){
        player->add_card(deck->draw());
        _calculate_val(player, player->get_card(player->get_hand_size() - 1));
    }

    std::cout << "----" << player->get_nickname() << "----" << std::endl;
    player->show();
    std::cout << std::endl;
    _print_dealer_hand(player);
}

void BlackJack::_print_hand_val(std::shared_ptr<Player> player){
    std::cout << "Total: ";
    if(_player_hand_val[player->get_id()].first != _player_hand_val[player->get_id()].second && _player_hand_val[player->get_id()].first < static_cast<int>(GAME_LIMIT::BJ_BUST)){
        std::cout << _player_hand_val[player->get_id()].first << '/' << _player_hand_val[player->get_id()].second << std::endl;
    }else if(_player_hand_val[player->get_id()].second < static_cast<int>(GAME_LIMIT::BJ_BUST)){
        std::cout << _player_hand_val[player->get_id()].second << std::endl;
    }else{
        std::cout << _player_hand_val[player->get_id()].first << std::endl;
    }
}

void BlackJack::_print_dealer_hand(std::shared_ptr<Player> player){
    std::cout << "Total: ";
    std::cout << _player_hand_val[player->get_id()].first << std::endl;
}

void BlackJack::_print_results(std::shared_ptr<Player> player){
    std::cout << player->get_nickname() << ": ";
    if(_player_bj_insurance[player->get_id()].first && _player_bj_insurance[_players.at(0)->get_id()].first){
        std::cout << "BJ DRAW" << std::endl;
        player->add_points(player->get_bet());
        return;
    }else if(!_player_bj_insurance[player->get_id()].first && _player_bj_insurance[_players.at(0)->get_id()].first){
        std::cout << "BJ LOSE" << std::endl;
        return;
    }
    else if(_player_bj_insurance[player->get_id()].first){
        std::cout << "BJ WIN" << std::endl;
        player->add_points(static_cast<int>(player->get_bet() + (BJ::BJ_WIN * player->get_bet())));
        return;
    }else if(_player_hand_val[player->get_id()].first < static_cast<int>(GAME_LIMIT::BJ_BUST)){
        if(_player_hand_val[player->get_id()].first == _player_hand_val[_players.at(0)->get_id()].first){
            std::cout << "DRAW" << std::endl;
            player->add_points(player->get_bet());
            return;
        }else if(_player_hand_val[player->get_id()].first > _player_hand_val[_players.at(0)->get_id()].first || _player_hand_val[_players.at(0)->get_id()].first >= static_cast<int>(GAME_LIMIT::BJ_BUST)){
            std::cout << "WIN" << std::endl;
            player->add_points(player->get_bet() * 2);
            return;
        }
    }else if(_player_hand_val[player->get_id()].second < static_cast<int>(GAME_LIMIT::BJ_BUST)){
        if(_player_hand_val[player->get_id()].second == _player_hand_val[_players.at(0)->get_id()].first){
            std::cout << "DRAW" << std::endl;
            player->add_points(player->get_bet());
            return;
        }else if(_player_hand_val[player->get_id()].second > _player_hand_val[_players.at(0)->get_id()].first || _player_hand_val[_players.at(0)->get_id()].first >= static_cast<int>(GAME_LIMIT::BJ_BUST)){
            std::cout << "WIN" << std::endl;
            player->add_points(player->get_bet() * 2);
            return;
        }
    }
    std::cout << "LOSE" << std::endl;
}

void BlackJack::_ask_insurance(){
    for(std::shared_ptr<Player> player : _players){
        std::string tmp;
        if(player->get_id() == 0){
            continue;
        }
        while(tmp != "n" && tmp != "y"){
            std::cout << "Insurance (y -> yes, n -> no): ";
            std::getline(std::cin, tmp);
        }
        
        _player_bj_insurance[player->get_id()].second = tmp == "y";
    }
}

void BlackJack::_clear_all(){
    for(std::shared_ptr<Player> player : _players){
        _player_bj_insurance[player->get_id()].first = false;
        _player_bj_insurance[player->get_id()].second = false;
        _player_hand_val[player->get_id()].first = 0;
        _player_hand_val[player->get_id()].second = 0;
        player->set_bet(BET_LIMIT::NO_BET);
    }
}

void BlackJack::_ask_bet(){
    std::string number;
    for(std::shared_ptr<Player> player : _players){
        if(player->get_nickname() == "Dealer"){
            continue;
        }
        std::cout << player->get_nickname() << " bet (points available: " << player->get_points() << "): ";
        std::getline(std::cin, number);
        player->set_bet(std::stoi(number));
        player->add_points(0 - player->get_bet());
        if(player->get_points() < 0){
            player->add_points(player->get_bet());
            player->set_bet(BET_LIMIT::NO_BET);
        }
        if(player->get_bet() > BET_LIMIT::NO_BET){
            _bets_available = true;
        }
    }
}

void BlackJack::play(){
    std::cout << "Adding dealer...\n";
    _add_player("Dealer", BJ::DEALER_ID);

    std::cout << "Adding players...\n";
    for(int i = 0; i < _max_players; ++i){
        _add_player(i + 1);
    }

    std::cout << "Creating deck...\n";
    std::shared_ptr<Deck> deck = std::make_shared<Deck>();

    std::cout << "Adding cards to deck...\n";
    _create_deck(deck);
    int old_size = deck->get_size();
    float deck_percentage = deck->get_size() / old_size;
    deck->shuffle();

    //std::cout << "Deck size is: " << deck->get_size() << std::endl;
    std::cout << "Game begins!" << std::endl;

    while(deck_percentage > FLOAT_LIMIT::DECK_LOW_LIMIT_PERCENTAGE){
        std::cout << "-----------------------------------------------------------------------------------" << std::endl;
        _bets_available = false;
        _ask_bet();
        if(!_bets_available){
            continue;
        }
        _first_draw(deck);
        _show_players();
        if(_players.front()->get_card(0)->get_figure() == FIGURE::ACE){
            _ask_insurance();
        }

        for(std::shared_ptr<Player> player : _players){
            if(player->get_nickname() != "Dealer" && player->get_bet() < BET_LIMIT::MIN_BET){
                continue;
            }
            _calculate_hand_val(player);
            //std::cout << "IS BJ??: " << _player_hand_val[player->get_id()].first << (_player_hand_val[player->get_id()].first == static_cast<int>(GAME_LIMIT::BLACK_JACK)) << std::endl;
            _check_bj(player);
            if(player->get_nickname() == "Dealer"){
                continue;
            }
            _player_draw(player, deck);
        }
        _dealer_draw(_players.front(), deck);
        for(std::shared_ptr<Player> player : _players){
            player->clear_hand();
            if(player->get_nickname() == "Dealer"){
                continue;
            }
            if(player->get_bet() < BET_LIMIT::MIN_BET){
                continue;
            }
            _print_results(player);
        }
        _clear_all();
        deck_percentage = static_cast<float>(deck->get_size()) / static_cast<float>(old_size);
        //break;
    }
    
}