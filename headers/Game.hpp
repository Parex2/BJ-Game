#pragma once


class Game {
    protected:
    int _max_players;
    int _id;
    public:
    Game() = default;
    Game(int max_players) : _max_players(max_players) {};
    ~Game() = default;
    
    virtual void play() = 0;
    virtual int get_id() const noexcept = 0;
    virtual int get_max_players() const noexcept = 0;
    virtual void set_id(int) noexcept = 0;
};