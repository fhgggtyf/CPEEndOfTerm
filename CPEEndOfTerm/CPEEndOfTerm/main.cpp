#include<iostream>

using namespace std;

int main() {

}

class Card {
private:
    Suit suit; 
    int value;  

public:
    Card(Suit s, int v) : suit(s), value(v) {}

    string getSuitAsString() const {
        switch (suit) {
        case Hearts: return "Hearts";
        case Diamonds: return "Diamonds";
        case Clubs: return "Clubs";
        case Spades: return "Spades";
        default: return "Unknown Suit";
        }
    }

    int getValue() const {
        return value;
    }
};

enum Suit {
    Hearts,
    Diamonds,
    Clubs,
    Spades
};