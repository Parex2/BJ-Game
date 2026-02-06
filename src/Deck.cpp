#include "../headers/Deck.hpp"

Deck::Deck(){
    ;
}

Deck::Deck(Deck& deck) : _deck(deck._deck){
    ;
}

void Deck::show() const{
    if(_deck.empty()){
        return;
    }
    for(auto& card : _deck){
        card->show();
    }
}


void Deck::create(){
    for(int i = static_cast<int>(DECK_RANGE::BEGIN_TYPE); i < static_cast<int>(DECK_RANGE::END_TYPE) + 1; ++i){
        for(int j = static_cast<int>(DECK_RANGE::BEGIN_FIGURE); j < static_cast<int>(DECK_RANGE::END_FIGURE) + 1; ++j){
            _deck.push_back(std::make_shared<Card>(TYPE(i), FIGURE(j)));
        }
    }
}

void Deck::shuffle(){
    if(_deck.empty()){
        return;
    }
    std::random_device rd;
    std::mt19937 rng(rd());
    std::shuffle(_deck.begin(), _deck.end(), rng);
}

int Deck::get_size(){
    return _deck.size();
}

std::shared_ptr<Card> Deck::draw(){
    if(_deck.empty()){
        return nullptr;
    }
    std::shared_ptr<Card> card = *(_deck.end() - 1);
    _deck.pop_back();
    return card;
}

bool Deck::add_card(std::shared_ptr<Card> card) {
    if(_deck.size() > static_cast<int>(LIMIT::DECK_SIZE)){
        return false;
    }

    _deck.push_back(card);
    return true;
}

bool Deck::merge(std::shared_ptr<Deck> deck){
    if(deck->get_size() < 1 || this == deck.get()){
        return false;
    }

    std::shared_ptr<Card> card = deck->draw();

    while(card){
        _deck.push_back(card);
        card = deck->draw();
    }

    return true;
}