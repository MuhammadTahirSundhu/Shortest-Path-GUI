#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
using namespace std;

class Score {
public:
	int score;

	Score()
	{
		score = 0;
	}

	void scoreUpdate(char val)
	{
        if (val == '1') {
            this->score += 10;
        }
        else if (val == '2') {
            this->score += 20;
        }
        else if (val == '3') {
            this->score += 30;
        }
        else if (val == '4') {
            this->score += 40;
        }
        else if (val == '5') {
            this->score -= 30;
        }
        else if (val == '6') {
            this->score += 60;
        }
    }
	
	int getScore()
	{
		return this->score;
	}


};

