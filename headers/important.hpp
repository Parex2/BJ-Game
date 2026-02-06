#pragma once
#include <string>
#include <vector>

enum class FIGURE {
    JOKER = 1,
    ACE = 14,
    KING = 13,
    QUEEN = 12,
    JACK = 11,
    TEN = 10,
    NINE = 9,
    EIGHT = 8,
    SEVEN = 7,
    SIX = 6,
    FIVE = 5,
    FOUR = 4,
    THREE = 3,
    TWO = 2
};

enum class TYPE {
    SPADE,
    DIAMOND,
    HEARTS,
    CLUBS
};

enum class LIMIT {
    DECK_SIZE = 1000,
    BJ = 4
};

enum class DECK_RANGE {
    BEGIN_FIGURE = static_cast<int>(FIGURE::TWO),
    END_FIGURE = static_cast<int>(FIGURE::ACE),
    BEGIN_TYPE = static_cast<int>(TYPE::SPADE),
    END_TYPE = static_cast<int>(TYPE::CLUBS)
};

enum class GAME_LIMIT {
    DECK_SIZE_BJ = 8,
    BJ_DEALER_STOP = 17,
    BJ_BUST = 22,
    BLACK_JACK = 21,

};

struct FLOAT_LIMIT {
    static constexpr float DECK_LOW_LIMIT_PERCENTAGE = 0.2f;
};

struct BET_LIMIT {
    static constexpr int START_POINTS = 5000;
    static constexpr int MIN_BET = 1;
    static constexpr int NO_BET = 0;
};

struct BJ {
    static constexpr int WIN = 1;
    static constexpr int LOSE = 0;
    static constexpr int DRAW = -1;
    static constexpr int DEALER_ID = 0;
    static constexpr float BJ_WIN = 1.5;
    static constexpr int DEALER_INDEX = 0;
    static constexpr int MIN_PLAYERS = 1;
    static constexpr int MAX_PLAYERS = 4;
    static constexpr int MAX_PLAYERS_WITH_DEALER = 5;
    static constexpr int DECK_SIZE = 8;
    static constexpr int BUST = 22;
    static constexpr int BLACK_JACK = 21;
    static constexpr int BLACK_JACK_WIN = 2;

};

namespace important{



inline bool is_number(std::string& number){
    
    auto check_number = [&number]() -> bool {
                        bool is_number = true;
                        for(char& sign : number){
                            if(!std::isdigit(static_cast<unsigned char>(sign))){
                                is_number = false;
                                break;
                            }
                        }
                        return is_number;
                    };
    
    return check_number();
}

}