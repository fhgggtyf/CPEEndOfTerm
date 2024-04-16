#include <iostream>
#include <cstdlib>  
#include <ctime>  
#include <vector>
#include <random>
#include <algorithm> 

using namespace std;

#define PLAYERNUM 3

enum Suit {
	Hearts = 0,
	Diamonds = 1,
	Clubs = 2,
	Spades = 3,
	Joker = 4
};

class Card {
private:
	Suit suit;
	int value;

public:
	Card() {
		suit = static_cast<Suit>(0);
		value = 0;
	}

	Card(int s, int v) {
		if (s >= 0 && s <= 4 && v >= 0 && v <= 14) {
			suit = static_cast<Suit>(s);
			value = v;
		}
		else {
			throw invalid_argument("Invalid suit or value");
		}
	}

	string getSuitAsString() const {
		switch (suit) {
		case Hearts: return "Hearts";
		case Diamonds: return "Diamonds";
		case Clubs: return "Clubs";
		case Spades: return "Spades";
		case Joker: return "Joker";
		default: return "Unknown Suit";
		}
	}

	int getValue() const {
		return value;
	}

	// Overload < operator
	bool operator<(const Card& other) const {
		if (value != other.value) {
			return value < other.value;
		}
		else {
			return (int)suit < (int)other.suit;
		}
	}

	// Overload > operator
	bool operator>(const Card& other) const {
		if (value != other.value) {
			return value > other.value;
		}
		else {
			return (int)suit > (int)other.suit;
		}
	}
};


class Participant {

protected:
	vector<Card> cards;
	int numOfCards;
	string name;

public:

	void SetCards(Card value) {
		cards.push_back(value);
	}

	int GetCardNum() {
		return numOfCards;
	}

	string GetName() {
		return name;
	}

	bool Validate(int input[2]) const {
		int count = count_if(cards.begin(), cards.end(), [input](const Card& card) {
			return card.getValue() == input[1];
			});
		return count >= input[0];
	}

	void MakePlay(int input[2]) {
		int count = 0;
		cards.erase(
			remove_if(cards.begin(), cards.end(),
				[&count, input](const Card& card) {
					if (count < input[0] && card.getValue() == input[1]) {
						++count;
						return true;  
					}
					return false;  
				}),
			cards.end()
		);
	}

	virtual int* FreeMove() { return nullptr; };

	virtual int* Move(int* prevCard[PLAYERNUM]) { return nullptr; };

};

vector<Card> GenerateDeck();
vector<Card> Shuffle(vector<Card> deck);
void SplitCards(vector<Card> cards, Participant** participants);
void DisplayHand(vector<Card> cards);
vector<Card> SortCards(vector<Card> cards);

class Player : public Participant {

public:
	Player(int i) {
		cards = {};
		numOfCards = 54 / PLAYERNUM;
		name = "player" + i;
	}

	int* FreeMove() override {
		int* input = new int[2];

		DisplayHand(cards);

		cout << "No one contests you. What do you want to play? " << endl;
		cin >> input[0] >> input[1];

		if (Validate(input)) {
			MakePlay(input);
			numOfCards -= input[0];
		}

		return input;
	}

	int* Move(int* prevCard[PLAYERNUM]) override {

		// Find previous entry
		int i = 0;
		for (; prevCard[i] == nullptr; i++);
		int* prevEnter = prevCard[i];

		int* input = new int[2];

		DisplayHand(cards);

		cout << "Previous entry is " << prevEnter[0] << " " << prevEnter[1] << ". What do you want to play? " << endl;
		cin >> input[0] >> input[1];
		if (input[0] == -1) {
			return nullptr;
		}
		else {
			// Validation
			if (Validate(input) && (input[0] > prevEnter[0] || (input[0] == prevEnter[0] && input[1] > prevEnter[1]))) {
				MakePlay(input);
				numOfCards -= input[0];
			}
			// If passed:
			return input;
		}
	}

};

class Computer : public Participant {

public:
	Computer(int i) {
		cards = {};
		numOfCards = 54 / PLAYERNUM;
		name = "(AI)player" + i;
	}

	int* FreeMove() override {

	}

	int* Move(int* prevCard[PLAYERNUM]) override {
		int* input = new int[2];
		int i;
		input = nullptr;
		for (; prevCard[i] == nullptr; i++);
		int* prevEnter = prevCard[i];
		int np = prevEnter[0];//number
		int vp = prevEnter[1];//value
		int t, flag, cnt = 0;

		if (np == 1) {// case of one card
			flag = 1;
			for (int i = 0; i < numOfCards; i++)//just output 1card
				if (cards[i] > vp)
				{
					flag = 0;
					input[1] = 1;
					input[2] = cards[i];
				}
			if (flag == 1) {//just output 2 card
				flag = 2;
				for (int i = 0; i < 18; i++)
				{
					if (flag == 1)
						break;
					t = cards[i];
					for (int j = i + 1; j < 18; j++)
						if (cards[j] == t)
							cnt++;
					if (cnt >= 1) {
						flag = 1;
						input[1] = 2;
						input[2] = t;
						break;
					}
				}
			}
			if (flag == 2) {// output3 cards
				flag = 3;
				for (int i = 0; i < 18; i++)
				{
					if (flag == 2)
						break;
					t = cards[i];//find
					for (int j = i + 1; j < 18; j++)
						if (cards[j] == t)
							cnt++;
					if (cnt >= 2) {
						flag = 2;
						input[1] = 3;
						input[2] = t;
					}
				}
			}
			if (flag == 3) {//output 4cards
				flag = 4;
				for (int i = 0; i < 18; i++)
				{
					if (flag == 3)
						break;
					t = cards[i];
					for (int j = i + 1; j < 18; j++)
						if (cards[j] == t)
							cnt++;
					if (cnt >= 3) {
						flag = 3;
						input[1] = 4;
						input[2] = t;
					}
				}
			}
		}
		if (np == 2) {//player 2 card
			flag = 2;
			for (int i = 0; i < 18; i++)//output2card
			{
				t = cards[i];
				if (t >= vp) {
					if (flag == 1)
						break;
					for (int j = i + 1; j < 18; j++)
						if (cards[j] == t)
							cnt++;
					if (cnt >= 1) {
						flag = 1;
						input[1] = 2;
						input[2] = t;
					}
				}

				if (flag == 2) {//output3 card
					flag = 3;
					for (int i = 0; i < 18; i++)
					{
						if (flag == 2)
							break;
						t = cards[i];
						for (int j = i + 1; j < 18; j++)
							if (cards[j] == t)
								cnt++;
						if (cnt >= 2) {
							flag = 2;
							input[1] = 3;
							input[2] = t;
						}
					}
				}
				if (flag == 3) {//output4card
					flag = 4;
					for (int i = 0; i < 18; i++)
					{
						if (flag == 3)
							break;
						t = cards[i];
						for (int j = i + 1; j < 18; j++)
							if (cards[j] == t)
								cnt++;
						if (cnt >= 3) {
							flag = 3;
							input[1] = 4;
							input[2] = t;
						}
					}
				}
			}
			if (np == 3) {//player 3 card
				flag = 3;
				for (int i = 0; i < 18; i++)//find 3 card
				{
					if (flag == 4)
						break;
					t = cards[i];
					if (t >= vp) {
						for (int j = i + 1; j < 18; j++)
							if (cards[j] == t)
								cnt++;
						if (cnt >= 2) {
							flag = 4;
							input[1] = 2;
							input[2] = t;
						}
					}

				}
				if (flag == 3) {//find 4 card
					flag = 4;
					for (int i = 0; i < 18; i++)
					{
						if (flag == 2)
							break;
						t = cards[i];
						for (int j = i + 1; j < 18; j++)
							if (cards[j] == t)
								cnt++;
						if (cnt >= 3) {
							flag = 2;
							input[1] = 4;
							input[2] = t;
						}
					}
				}
			}
			if (np == 4) {
				flag = 4;
				for (int i = 0; i < 18; i++)
				{
					if (flag == 1)
						break;
					t = cards[i];
					if (t >= vp) {
						for (int j = i + 1; j < 18; j++)
							if (cards[j] == t)
								cnt++;
						if (cnt >= 3) {
							flag = 1;
							input[1] = 2;
							input[2] = t;
						}
					}
				}
			}
			return input;

		}
	}
};

int main() {


	// Create group of participants
	Participant** participants = new Participant * [PLAYERNUM];

	int choice;
	for (int i = 0; i < PLAYERNUM; i++) {
		cout << "Add a player (0: AI, 1: Player):";
		cin >> choice;
		if (choice == 1) {
			participants[i] = new Player(i);
		}
		//else {
		//	participants[i] = new Computer(i);
		//}
	}

	// Generate deck
	vector<Card> cards = GenerateDeck();

	// Default start game
	bool gameOn = true;

	// Play history
	int** prevCard = new int* [PLAYERNUM];

	// Now allocate each sub-array
	for (int i = 0; i < PLAYERNUM; i++) {
		prevCard[i] = nullptr;
	}

	// Starting index
	int index;

	Participant* winner;

	while (gameOn) {
		cards = Shuffle(cards);
		SplitCards(cards, participants);

		srand(time(NULL));
		index = rand() % PLAYERNUM;

		// First player play
		prevCard[index % PLAYERNUM] = participants[index % PLAYERNUM]->FreeMove();
		index++;

		while (true) {

			if (prevCard[index % PLAYERNUM] != nullptr) { // If previous move was never contested
				prevCard[index % PLAYERNUM] = participants[index % PLAYERNUM]->FreeMove();  // Have a free move
			}
			else { //If there is a previous move
				prevCard[index % PLAYERNUM] = participants[index % PLAYERNUM]->Move(prevCard);  // Move according to prev move
			}

			// If successfully contested
			if (prevCard[index % PLAYERNUM] != nullptr) {

				for (int i = 0; i < PLAYERNUM - 1; i++) {
					prevCard[(index + i + 1) % PLAYERNUM] = nullptr;
				}
				// Clear other history
			}

			// If no more card left
			if (participants[index % PLAYERNUM]->GetCardNum() == 0) {
				// Get winner and break loop
				winner = participants[index % PLAYERNUM];
				break;
			}

			// Advance to next player
			index++;
		}

		cout << "Winner is: " << winner->GetName() << endl;
		gameOn = false;
	}
}

vector<Card> GenerateDeck() {
	vector<Card> cards;
	cards.reserve(54);  // Pre-allocate memory for 52 cards + 2 jokers to avoid reallocations

	// Loop over suits and values to create standard cards
	for (int suit = 0; suit < 4; suit++) {  // Loop over 4 suits
		for (int value = 0; value < 13; value++) {  // Loop over 13 values (0 to 12)
			cards.emplace_back(suit, value);  // Use emplace_back for efficiency
		}
	}

	// Add jokers to the deck
	cards.emplace_back(4, 13);  // First Joker
	cards.emplace_back(4, 14);  // Second Joker

	return cards;
}

vector<Card> Shuffle(vector<Card> deck) {
	// Create a random engine; using the random_device to seed it provides some level of randomness
	random_device rd; // Obtain a random number from hardware
	default_random_engine eng(rd()); // Seed the engine

	// Shuffle the vector
	shuffle(deck.begin(), deck.end(), eng);
	return deck;
}

void SplitCards(vector<Card> cards, Participant** participants) {

	for (int i = 0; i < 54 / PLAYERNUM; i++) {
		for (int j = 0; j < PLAYERNUM; j++) {
			participants[j]->SetCards(cards[54 / PLAYERNUM * j + i]);
		}

	}
}

void DisplayHand(vector<Card> cards) {

	cards = SortCards(cards);

	for (auto it = cards.begin(); it != cards.end(); ++it) {
		cout << it->getValue() << " ";
	}
}


vector<Card> SortCards(vector<Card> cards) {
	sort(cards.begin(), cards.end(), [](const Card& a, const Card& b) {
		return a < b; // Compare based on name for sorting
		});

	return cards;
}


