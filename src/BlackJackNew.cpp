#include "../headers/BlackJackNew.hpp"

//public methods

BlackJackNew::BlackJackNew(int player_count){
    if(player_count < BJ::MIN_PLAYERS){
        _player_count = BJ::MIN_PLAYERS;
    }else if(player_count > BJ::MAX_PLAYERS){
        _player_count = BJ::MAX_PLAYERS;
    }else{
        _player_count = player_count;
    }
    _players.at(BJ::DEALER_INDEX) = std::make_shared<Player>("Dealer", BJ::DEALER_ID);
    _head = _create_token(_players.at(BJ::DEALER_INDEX));
    _last_token = _head;
}

BlackJackNew::Token::Token(std::shared_ptr<Player> player){
    this->player = player;
}

void BlackJackNew::set_id(int id) noexcept{
    _id = id;
}

int BlackJackNew::get_id() const noexcept{
    return _id;
}

int BlackJackNew::get_max_players() const noexcept {
    return _max_players;
}

void BlackJackNew::play() {
    for(int i = BJ::DEALER_INDEX + 1; i < _player_count + 1; ++i){
        std::string nickname;
        std::cout << "Enter nickname: ";
        std::getline(std::cin, nickname);
        _players.at(i) = std::make_shared<Player>(nickname, i);
        _players.at(i)->set_points(BET_LIMIT::START_POINTS);
        _add_token(_create_token(_players.at(i)));
        std::cout << _players.at(i)->get_nickname() << " in game!" << std::endl;
    }

    _deck = std::make_shared<Deck>();

    for(int i = 0; i < BJ::DECK_SIZE; ++i){
        _deck->create();
    }

    _deck->shuffle();

    int start_size = _deck->get_size();

    float size_porcentage = static_cast<float>(_deck->get_size()) / static_cast<float>(start_size);

    while(size_porcentage > FLOAT_LIMIT::DECK_LOW_LIMIT_PERCENTAGE){
        _bets_on = false;
        _ask_bet();
        _print_sumarry_bets();
        if(!_bets_on){
            std::cout << "NO BETS!" << std::endl;
            continue;
        }
        _first_draw(_deck);
        _print_first_hand();
        _ask_insurance();
        _players_draw(_deck);
        _dealer_draw(_head, _deck);
        _get_results();
        _print_dealer_hand();
        _print_results();
        _clear_all();




        size_porcentage = static_cast<float>(_deck->get_size()) / static_cast<float>(start_size);
    }
}

//private methods

void BlackJackNew::_add_token(std::shared_ptr<Token> token){
    _last_token->next = token;
    token->previous = _last_token;
    _last_token = token;
}

std::shared_ptr<BlackJackNew::Token> BlackJackNew::_create_token(std::shared_ptr<Player> player) {
    return std::make_shared<Token>(player);
}

void BlackJackNew::_count_max(int& hand_val, std::shared_ptr<Card> card, bool& soft){
    if(card->get_figure() == FIGURE::ACE){
        int old_val = hand_val;
        hand_val += 11;
        if(hand_val > BJ::BUST){
            hand_val -= 11;
            ++hand_val;
        }
        if(!soft && (hand_val - old_val) == 11){
            soft = true;
        }
        return;
    }

    hand_val += static_cast<int>(card->get_figure()) >= 10 ? 10 : static_cast<int>(card->get_figure());
}

void BlackJackNew::_count_min(int& hand_val, std::shared_ptr<Card> card){
    if(card->get_figure() == FIGURE::ACE){
        ++hand_val;
        return;
    }
    hand_val += static_cast<int>(card->get_figure()) >= 10 ? 10 : static_cast<int>(card->get_figure());
}

void BlackJackNew::_first_draw(std::shared_ptr<Deck> deck){
    std::shared_ptr<Token> token = _head;
    while(token){
        if(token->player->get_bet() < BET_LIMIT::MIN_BET && token->player->get_id() != BJ::DEALER_ID){
            token = token->next;
            continue;
        }

        token->player->add_card(deck->draw());
        _count(token);

        token->player->add_card(deck->draw());
        _count(token);

        token->bj = token->max_value == BJ::BLACK_JACK ? true : false;

        token = token->next;

    }
}

void BlackJackNew::_ask_bet(){
    for(std::shared_ptr<Player> player : _players){
        if(!player){
            break;
        }
        if(player->get_id() == BJ::DEALER_ID){
            continue;
        }
        while(true){
            std::string number;
            while(true){
                std::cout << player->get_nickname() << " bet (available points: " << player->get_points() << "): ";
                std::getline(std::cin, number);
                if(important::is_number(number)){
                    break;
                }
            }
            int number_int = std::stoi(number);
            
            if(number_int > player->get_points()){
                std::cout << "Not enough points!" << std::endl;
            }else if(number_int > BET_LIMIT::MIN_BET){
                player->set_bet(number_int);
                player->add_points(0 - number_int);
                _bets_on = true;
                break;
            }

            player->set_bet(number_int);
            break;
        }

    }
}

void BlackJackNew::_print_sumarry_bets(){
    std::cout << "----------------" << std::endl;
    std::cout << "| SUMMARY BETS |" << std::endl;
    std::cout << "----------------" << std::endl;
    
    for(std::shared_ptr<Player> player : _players){
        if(!player){
            break;
        }
        if(player->get_id() == BJ::DEALER_ID){
            continue;
        }
        std::cout << player->get_nickname() << " bet: " << player->get_bet() << std::endl;
        std::cout << player->get_nickname() << " points after bet: " << player->get_points() << std::endl;
    }
}

void BlackJackNew::_print_first_hand(){
    std::cout << "----------------" << std::endl;
    std::cout << "| FIRST HAND   |" << std::endl;
    std::cout << "----------------" << std::endl;

    for(std::shared_ptr<Player> player : _players){
        if(!player){
            break;
        }
        if(player->get_id() == BJ::DEALER_ID){
            std::cout << player->get_nickname() << std::endl;
            std::cout << "Hand: ";
            player->get_card(0)->show();
            std::cout << std::endl;
            continue;
        }
        std::cout << player->get_nickname() << std::endl;
        player->show();
        std::cout << std::endl;
    }
}

void BlackJackNew::_ask_insurance(){

    if(_head->player->get_card(0)->get_figure() != FIGURE::ACE){
        return;
    }

    std::shared_ptr<Token> token = _head->next;

    if(token->player->get_points() < (token->player->get_bet() / 2) || token->parent){
        return;
    }

    std::cout << "----------------" << std::endl;
    std::cout << "|   INSURANCE   |" << std::endl;
    std::cout << "----------------" << std::endl;

    

    while(token){
        std::string action;
        std::cout << token->player->get_nickname() << " (y -> yes, n->no): ";
        std::getline(std::cin, action);
        if(action == "y"){
            token->insurance = true;
            token->insurance_bet = (token->player->get_bet()/2);
            token->player->add_points(0 - token->insurance_bet);
            token = token->next;
        }else if(action == "n"){
            token = token->next;
        }
    }

}

void BlackJackNew::_players_draw(std::shared_ptr<Deck> deck){
    if(_head->bj){
        return;
    }
    std::shared_ptr<Token> token = _head->next;

    while(token){
        if(token->bj){
            token = token->next;
            continue;
        }
        if(token->player->get_hand_size() == 2){
            if(token->player->get_card(0) == token->player->get_card(1)) {
                _ask_split(token);
                std::cout << "Split asked" << std::endl;
            }
        }
        if(token->player->get_hand_size() == 1){
            token->player->add_card(deck->draw());
            _count(token);
            continue;
        }
        while(token->min_value < BJ::BUST || token->max_value < BJ::BUST){
            std::cout << token->player->get_nickname() << " -> DRAWING" << std::endl;
            token->player->show();
            std::cout << std::endl;
            _print_total(token);
            std::string action;
            std::cout << "Choose action(d -> draw, p -> pass, dd -> double): ";
            std::getline(std::cin, action);
            if(action == "d"){
                token->player->add_card(deck->draw());
                _count(token);
            }else if(action == "p"){
                break;
            }else if(action == "dd" && token->player->get_hand_size() == 2){
                if(token->parent){
                    token->parent->player->add_points(0 - token->parent->player->get_bet());
                    token->parent->player->set_bet(token->parent->player->get_bet() + token->parent->player->get_bet());
                }else{
                    token->player->add_points(0 - token->player->get_bet());
                    token->player->set_bet(token->player->get_bet() + token->player->get_bet());
                }
                token->player->add_card(deck->draw());
                _count(token);
                break;
            }
        }
        token->player->show();
        std::cout << std::endl;
        _print_total(token);
        token = token->next;
    }
}

void BlackJackNew::_ask_split(std::shared_ptr<Token> token){
    while(true){
        std::string action;
        std::cout << token->player->get_nickname() << " split(y->yes, n->no): ";
        std::getline(std::cin, action);
        if(action == "y"){
            break;
        }else if(action == "n"){
            return;
        }
    }
    std::shared_ptr<Player> new_player = std::make_shared<Player>(token->player->get_nickname(), token->player->get_id());
    new_player->add_card(token->player->remove_last());

    token->min_value = 0;
    token->max_value = 0;

    _count(token);

    std::shared_ptr<Token> dst = _create_token(new_player);

    if(!token->parent){
        dst->parent = token;
    }else{
        dst->parent = token->parent;
    }

    _count(dst);

    _add_in_place(token, dst);

}

void BlackJackNew::_add_in_place(std::shared_ptr<Token> src, std::shared_ptr<Token> dst){
    std::shared_ptr<Token> next_token = src->next;
    next_token->previous = dst;
    dst->next = next_token;
    dst->previous = src;
}

void BlackJackNew::_count(std::shared_ptr<Token> token){
    _count_max(token->max_value, token->player->get_last(), token->soft_seventeen);
    _count_min(token->min_value, token->player->get_last());
}

void BlackJackNew::_print_total(std::shared_ptr<Token> token){
    std::cout << "Total: ";
    if(token->max_value == token->min_value){
        std::cout << token->max_value << std::endl;
    }else if(token->max_value >= BJ::BUST){
        std::cout << token->min_value << std::endl;
    }else if(token->max_value >= BJ::BUST && token->min_value >= BJ::BUST){
        std::cout << "BUST" << std::endl;
    }else{
        std::cout << token->max_value << "/" << token->min_value << std::endl;
    }
}

void BlackJackNew::_clear_all(){
    std::shared_ptr<Token> token = _head;
    std::shared_ptr<Token> to_delete;

    while(token){
        if(token->parent){
            to_delete = token;
            token->parent->next = token->next;
            token = token->parent->next;
            _delete(to_delete);
            continue;
        }
        token->player->clear_hand();
        token->player->set_bet(0);
        token->insurance = false;
        token->bj = false;
        token->max_value = 0;
        token->min_value = 0;
        token->final_value = 0;
        token->result = 0;
        token->soft_seventeen = false;
        token = token->next;
    }
}

void BlackJackNew::_delete(std::shared_ptr<Token> to_delete){
    to_delete.reset();
}

void BlackJackNew::_get_results(){
    std::shared_ptr<Token> token = _head->next;

    while(token){
        _get_final_value(token);
        if(_head->bj){
            if(token->bj){
                _result(token, BJ::DRAW);
                _return_insurance(token);
            }else{
                _result(token, BJ::LOSE);
                _return_insurance(token);
            }
        }else if(token->bj){
            _result(token, BJ::BLACK_JACK_WIN);
        }else if(token->final_value > _head->final_value && token->final_value < BJ::BUST || token->final_value < BJ::BUST && _head->final_value >= BJ::BUST){
            _result(token, BJ::WIN);
        }else if(token->final_value >= BJ::BUST || token->final_value < _head->final_value || _head->final_value >= BJ::BUST){
            _result(token, BJ::LOSE);
        }else{
            _result(token, BJ::DRAW);
        }
        token = token->next;
    }
}

void BlackJackNew::_result(std::shared_ptr<Token> token, int result){
    std::shared_ptr<Player> player;
    if(token->parent){
        player = token->parent->player;
    }else{
        player = token->player;
    }

    if(result == BJ::BLACK_JACK_WIN){
        player->add_points(player->get_bet() + player->get_bet() * BJ::BJ_WIN);
        token->result = BJ::BLACK_JACK_WIN;
    }else if(result == BJ::WIN){
        player->add_points(player->get_bet() + player->get_bet());
        token->result = BJ::WIN;
    }else if(result == BJ::DRAW){
        player->add_points(player->get_bet());
        token->result = BJ::DRAW;
    }else if(result == BJ::LOSE){
        token->result = BJ::LOSE;
    }
}

void BlackJackNew::_return_insurance(std::shared_ptr<Token> token){
    if(!token->insurance){
        return;
    }

    std::shared_ptr<Player> player;
    if(token->parent){
        //player = token->parent->player;
        return;
    }else{
        player = token->player;
    }

    player->add_points(token->insurance_bet * 2);

}

void BlackJackNew::_get_final_value(std::shared_ptr<Token> token){
    if(token->max_value < BJ::BUST){
        token->final_value = token->max_value;
    }else if(token->min_value < BJ::BUST){
        token->final_value = token->min_value;
    }else{
        token->final_value = token->max_value;
    }
}

void BlackJackNew::_print_results(){
    std::shared_ptr<Token> token = _head->next;

    while(token){
        std::cout << token->player->get_nickname();

        if(token->parent){
            std::cout << " (other hand): ";
        }else{
            std::cout << ": ";
        }

        switch(token->result){
            case BJ::BLACK_JACK_WIN:
                std::cout << "BLACK JACK" << std::endl;
                break;
            case BJ::WIN:
                std::cout << "WIN" << std::endl;
                break;
            case BJ::LOSE:
                std::cout << "LOSE" << std::endl;
                break;
            case BJ::DRAW:
                std::cout << "DRAW" << std::endl;
                break;
        }

        token = token->next;

    }
}

void BlackJackNew::_dealer_draw(std::shared_ptr<Token> token, std::shared_ptr<Deck> deck){
    std::shared_ptr<Token> player_token = token->next;

    _get_final_value(token);

    while(player_token){
        _get_final_value(player_token);
        if(!player_token->bj && player_token->final_value < BJ::BUST){
            break;
        }
        if(!player_token->next){
            return;
        }
        player_token = player_token->next;
    }
    while(true){
        
        if(token->final_value >= 17){
            if(/*token->player->get_card(0)->get_figure() == FIGURE::ACE &&*/ token->final_value == 17 && token->soft_seventeen/*token->player->get_hand_size() == 2*/){
                token->player->add_card(deck->draw());
                _count(token);
                continue;
            }
            break;
        }

        token->player->add_card(deck->draw());
        _count(token);
        _get_final_value(token);
    }
}

void BlackJackNew::_print_dealer_hand(){
    std::cout << _head->player->get_nickname() << std::endl;
    _head->player->show();
    std::cout << std::endl;
    std::cout << "Total: " << _head->final_value << std::endl;
}