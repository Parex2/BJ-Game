#pragma once

class IDisplay{
	public:
		IDisplay() = default;
		~IDisplay() = default;
		virtual void show() const = 0;
};
