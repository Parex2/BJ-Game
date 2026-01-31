#include "../headers/Card.hpp"

Card::Card(TYPE card_type, FIGURE card_figure){
	this->_card_type = card_type;
	this->_card_figure = card_figure;

}

Card::~Card() {
	
}

void Card::show() const {
	switch(this->_card_figure){
		case FIGURE::TWO:
			std::cout << "Two ";
			break;
		case FIGURE::THREE:
			std::cout << "Three ";
			break;
		case FIGURE::FOUR:
			std:: cout << "Four ";
			break;
		case FIGURE::FIVE:
			std::cout << "Five ";
			break;
		case FIGURE::SIX:
			std::cout << "Six ";
			break;
		case FIGURE::SEVEN:
			std::cout << "Seven ";
			break;
		case FIGURE::EIGHT:
			std::cout << "Eight ";
			break;
		case FIGURE::NINE:
			std::cout << "Nine ";
			break;
		case FIGURE::TEN:
			std::cout << "Ten ";
			break;
		case FIGURE::JACK:
			std::cout << "Jack ";
			break;
		case FIGURE::QUEEN:
			std::cout << "Queen ";
			break;
		case FIGURE::KING:
			std::cout << "King ";
			break;
		case FIGURE::ACE:
			std::cout << "Ace ";
			break;
	}

	switch(this->_card_type){
		case TYPE::CLUBS:
			std::cout << u8"\u2663";
			break;
		case TYPE::DIAMOND:
			std::cout << u8"\u2666";
			break;
		case TYPE::HEARTS:
			std::cout << u8"\u2665";
			break;
		case TYPE::SPADE:
			std::cout << u8"\u2660";
			break;
	}
}

TYPE Card::get_type() const {
	return this->_card_type;
}

FIGURE Card::get_figure() const {
	return this->_card_figure;
}
