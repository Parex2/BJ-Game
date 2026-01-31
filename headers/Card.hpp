#pragma once
#include "IDisplay.hpp"
#include "important.hpp"
#include <iostream>

class Card : virtual IDisplay {
	private:
		TYPE _card_type;
		FIGURE _card_figure;
	
	public:
		Card(TYPE card_type, FIGURE card_figure);
		~Card();

		virtual void show() const;
		TYPE get_type() const;
		FIGURE get_figure() const;
};
