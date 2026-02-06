#include "../headers/Deck.hpp"
#include "../headers/Player.hpp"
#include "../headers/BlackJack.hpp"
#include "../headers/BlackJackNew.hpp"
#include "../headers/DataBase.hpp"
int main(){
	std::string number;
	while(true){
		std::cout << "Enter player count: ";
		std::flush(std::cout);
		std::getline(std::cin, number);
		if(important::is_number(number)){
			break;
		}
	}
	
	
	std::shared_ptr<BlackJackNew> bj_new = std::make_shared<BlackJackNew>(std::stoi(number));
	bj_new->play();

	
	return 0;
}
