#include "../headers/Player.hpp"

Player::Player(std::string nickname) : _nickname(nickname){
    
}

Player::Player(std::string nickname, int id) : _nickname(nickname), _id(id) {}

void Player::sort_hand(){
    if(_is_sorted){
        return;
    }
    std::sort(_hand.begin(), _hand.end(), [](std::shared_ptr<Card> c1, std::shared_ptr<Card> c2)
                                            {
                                                if(c1->get_figure() == c2->get_figure()){
                                                    return static_cast<int>(c1->get_type()) > static_cast<int>(c2->get_type());
                                                }
                                                return static_cast<int>(c1->get_figure()) > static_cast<int>(c2->get_figure());
                                            });
    _is_sorted = true;
}

void Player::add_card(std::shared_ptr<Card> card){
    if(card == nullptr){
        return;
    }
    _is_sorted = false;
    _hand.push_back(card);
}

void Player::show() const{
    std::cout << "Hand: ";
    for(auto& card : _hand){
        card->show();
        std::cout << ' ';
    }
}

std::string Player::get_nickname() const{
    return _nickname;
}

int Player::get_id(){
    return _id;
}

int Player::get_hand_size(){
    return _hand.size();
}

const std::shared_ptr<Card> Player::get_card(int index){
    return _hand.at(index);
}

void Player::clear_hand(){
    _hand.clear();
    _hand.shrink_to_fit();
}

void Player::show_at(int pos) const{
    if(pos < 0 || pos >= _hand.size()){
        return;
    }
    _hand.at(pos)->show();
}

int Player::get_points(){
    return _points;
}

void Player::set_points(int points) {
    _points = points;
}

void Player::add_points(int points){
    _points += points;
}

void Player::set_bet(int bet){
    if(bet > -1){
        _bet = bet;
    }
}

int Player::get_bet(){
    return _bet;
}

std::shared_ptr<Card> Player::remove_last(){
    std::shared_ptr<Card> card = _hand.back();
    _hand.pop_back();
    return card;
}

std::shared_ptr<Card> Player::get_last(){
    return _hand.back();
}
