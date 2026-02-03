#include "../headers/Deck.hpp"
#include "../headers/Player.hpp"
#include "../headers/BlackJack.hpp"
#include "../headers/BlackJackNew.hpp"
#include <sqlite3.h>
int main(){
	//std::shared_ptr<sqlite3> db = std::make_shared<sqlite3>();
	sqlite3* db = nullptr;

	sqlite3_stmt* stmt = nullptr;

	sqlite3_open("test.db", &db);
	sqlite3_close(db);

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
