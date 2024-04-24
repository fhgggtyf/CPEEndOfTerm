#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <random>
#include <algorithm>

// Check if compiling for Windows
#ifdef _WIN32
#include <io.h>
#include <fcntl.h>

void setUnicodeOutput() {
	// Set the output to Unicode mode
	_setmode(_fileno(stdout), _O_U16TEXT);
}
#else
#include <locale>
#include <codecvt>

void setUnicodeOutput() {
	// Set locale to handle UTF-8 characters
	std::locale::global(std::locale(std::locale(""), new std::codecvt_utf8<wchar_t>));
}
#endif    // For _setmode()

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
			cout << "err";
			suit = static_cast<Suit>(0);
			value = -1;
		}
	}

	wstring getSuitAsSymbol() const {
		switch (suit) {
		case Hearts: return L"♥";
		case Diamonds: return L"♦";
		case Clubs: return L"♣";
		case Spades: return L"♠";
		case Joker: return L"Joker";
		default: return L"Unknown Suit";
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
	int numOfCards = 0;
	wstring name;

public:

	void SetCards(Card value) {
		cards.push_back(value);
	}

	int GetCardNum() {
		return numOfCards;
	}

	wstring GetName() {
		return name;
	}

	bool Validate(int input[2]) const {
		if (input[0] == 2 && input[1] == 13) {
			int count1 = count_if(cards.begin(), cards.end(), [input](const Card& card) {
				return card.getValue() == 13;
				});
			int count2 = count_if(cards.begin(), cards.end(), [input](const Card& card) {
				return card.getValue() == 14;
				});

			wcout << count1 << " " << count2 << endl;
			return count1 == 1 && count2 == 1;
		}
		else {
			int count = count_if(cards.begin(), cards.end(), [input](const Card& card) {
				return card.getValue() == input[1];
				});
			return count >= input[0];
		}

	}

	void MakePlay(int input[2]) {

		if (input[0] == 2 && input[1] == 13) {
			cards.erase(
				remove_if(cards.begin(), cards.end(),
					[input](const Card& card) {
						return card.getValue() == input[1];
					}),
				cards.end()
			);
			cards.erase(
				remove_if(cards.begin(), cards.end(),
					[input](const Card& card) {
						return card.getValue() == input[1] + 1;
					}),
				cards.end()
			);
		}
		else {
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
	}

	virtual int* FreeMove() { return nullptr; };

	virtual int* Move(int* prevCard[PLAYERNUM]) { return nullptr; };

};

vector<Card> GenerateDeck();
vector<Card> Shuffle(vector<Card> deck);
void SplitCards(vector<Card> cards, Participant** participants);
void DisplayHand(vector<Card> cards);
vector<Card> SortCards(vector<Card> cards);
wstring getSymbolUsingValue(int value);
int getValueUsingSymbol(wstring symbol);

class Player : public Participant {

public:
	Player(int i) {
		cards = {};
		numOfCards = 54 / PLAYERNUM;
		name = L"Player " + to_wstring(i + 1);
	}

	int* FreeMove() override {
		wstring* input = new wstring[2];
		int* castedInput = new int[2];

		wcout << name << L"'s cards:" << endl;
		DisplayHand(cards);

		while (true) {
			wcout << L"No one contests you. What do you want to play? " << endl;
			wcin >> input[0] >> input[1];

			castedInput[0] = stoi(input[0]);
			castedInput[1] = getValueUsingSymbol(input[1]);

			wcout << castedInput[0] << " " << castedInput[1];

			if (Validate(castedInput)) {
				MakePlay(castedInput);
				numOfCards -= castedInput[0];
				break;
			}
			else {
				wcout << L"Invalid Input." << endl;
			}
		}

		return castedInput;
	}

	int* Move(int* prevCard[PLAYERNUM]) override {

		// Find previous entry
		int i = 0;
		for (; prevCard[i] == nullptr; i++);
		int* prevEnter = prevCard[i];

		wstring* input = new wstring[2];
		int* castedInput = new int[2];

		wcout << name << L"'s cards:" << endl;
		DisplayHand(cards);

		// Validation
		while (true) {
			wcout << L"Previous player played " << prevEnter[0] << L" number of " << getSymbolUsingValue(prevEnter[1]) << L". What do you want to play? " << endl;
			wcin >> input[0] >> input[1];


			castedInput[0] = stoi(input[0]);
			castedInput[1] = getValueUsingSymbol(input[1]);

			if (castedInput[0] == -1) {
				return nullptr;
			}
			else {
				if (Validate(castedInput) && (castedInput[0] > prevEnter[0] || (castedInput[0] == prevEnter[0] && castedInput[1] > prevEnter[1]))) {
					MakePlay(castedInput);
					numOfCards -= castedInput[0];
					break;
				}
				else {
					wcout << L"Invalid Input." << endl;
				}
			}
		}

		return castedInput;
	}
};

class Computer : public Participant {

public:
	Computer(int i) {
		cards = {};
		numOfCards = 54 / PLAYERNUM;
		name = L"(AI)player " + to_wstring(i + 1);
	}
	int* FreeMove() override {
		cards = SortCards(cards);

		int* input = new int[2];
		input[0] = 1;
		input[1] = cards[0].getValue();

		MakePlay(input);
		numOfCards -= input[0];

		return input;
	}

	int* Move(int* prevCard[PLAYERNUM]) override {

		int* input = new int[2];
		int* xiaow = new int[2];
		int* daw = new int[2];
		xiaow[0] = 1;
		xiaow[1] = 13;
		daw[0] = 1;
		daw[1] = 14;
		cards = SortCards(cards);

		int i=0;
		for (; prevCard[i] == nullptr; i++);
		int* prevEnter = prevCard[i];
		int np = prevEnter[0];//number
		int vp = prevEnter[1];//value

		for (int j = vp+1; j <= 14; j++) {
			input[0] = np;
			input[1] = j;
			if (Validate(input)) {
				MakePlay(input);
				numOfCards -= input[0];

				return input;
			}
		}

		for (int i = np + 1; i <= 4; i++)
			for (int j = 0; j <= 14; j++) {
				input[0] = i;
				input[1] = j;
				if (Validate(input)) {
					MakePlay(input);
					numOfCards -= input[0];

					return input;
				}
			}
		if (Validate(xiaow) == true && Validate(daw) == true) {//Double joker
			input[0] = 2;
			input[1] = 13;

			MakePlay(input);
			numOfCards -= input[0];

			return input;
		}
		return	nullptr;
	}
};

int main() {
	setUnicodeOutput();

	// Create group of participants
	Participant** participants = new Participant * [PLAYERNUM];

	int choice;
	for (int i = 0; i < PLAYERNUM; i++) {
		wcout << L"Add a player (0: AI, 1: Player):";
		cin >> choice;
		if (choice == 1) {
			participants[i] = new Player(i);
		}
		else {
			participants[i] = new Computer(i);
		}
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

		wcout << L"Winner is: " << winner->GetName() << endl;
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
		wcout << L"----";
	}
	wcout << L"------" << endl;

	for (auto it = cards.begin(); it != cards.end(); ++it) {
		if (getSymbolUsingValue(it->getValue()) == L"BJoker") {
			wcout << L"|B  ";
		}
		else if (getSymbolUsingValue(it->getValue()) == L"RJoker") {
			wcout << L"|R  ";
		}
		else {
			if (getSymbolUsingValue(it->getValue()) == L"10") {
				wcout << L"|" << getSymbolUsingValue(it->getValue()) << L" ";
			}
			else {
				wcout << L"|" << getSymbolUsingValue(it->getValue()) << L"  ";
			}
		}
	}
	wcout << L"     |" << endl;

	for (auto it = cards.begin(); it != cards.end(); ++it) {
		if (getSymbolUsingValue(it->getValue()) == L"BJoker" || getSymbolUsingValue(it->getValue()) == L"RJoker") {
			wcout << L"|J  ";
		}
		else {
			wcout << L"|" << it->getSuitAsSymbol() << L"  ";
		}
	}
	wcout << L"     |" << endl;


	for (auto it = cards.begin(); it != cards.end(); ++it) {
		if (getSymbolUsingValue(it->getValue()) == L"BJoker" || getSymbolUsingValue(it->getValue()) == L"RJoker") {
			wcout << L"|O  ";
		}
		else {
			wcout << L"|   ";
		}
	}
	wcout << L"     |" << endl;

	for (auto it = cards.begin(); it != cards.end(); ++it) {
		if (getSymbolUsingValue(it->getValue()) == L"BJoker" || getSymbolUsingValue(it->getValue()) == L"RJoker") {
			wcout << L"|K  ";
		}
		else {
			wcout << L"|   ";
		}
	}
	wcout << L"     |" << endl;

	for (auto it = cards.begin(); it != cards.end(); ++it) {
		if (getSymbolUsingValue(it->getValue()) == L"BJoker" || getSymbolUsingValue(it->getValue()) == L"RJoker") {
			wcout << L"|E  ";
		}
		else {
			wcout << L"|   ";
		}
	}
	wcout << L"     |" << endl;

	for (auto it = cards.begin(); it != cards.end(); ++it) {
		if (getSymbolUsingValue(it->getValue()) == L"BJoker" || getSymbolUsingValue(it->getValue()) == L"RJoker") {
			wcout << L"|R  ";
		}
		else {
			wcout << L"|   ";
		}
	}
	wcout << L"     |" << endl;

	for (auto it = cards.begin(); it != cards.end(); ++it) {
		wcout << L"----";
	}
	wcout << L"------" << endl;



}

vector<Card> SortCards(vector<Card> cards) {
	sort(cards.begin(), cards.end(), [](const Card& a, const Card& b) {
		return a < b; // Compare based on name for sorting
		});

	return cards;
}

wstring getSymbolUsingValue(int value) {
	switch (value) {
	case 0: return L"A";
	case 1: return L"2";
	case 2: return L"3";
	case 3: return L"4";
	case 4: return L"5";
	case 5: return L"6";
	case 6: return L"7";
	case 7: return L"8";
	case 8: return L"9";
	case 9: return L"10";
	case 10: return L"J";
	case 11: return L"Q";
	case 12: return L"K";
	case 13: return L"BJoker";
	case 14: return L"RJoker";
	default: return L"Unknown Value";
	}
}

int getValueUsingSymbol(const wstring symbol) {
	if (symbol == L"A") return 0;
	else if (symbol == L"2") return 1;
	else if (symbol == L"3") return 2;
	else if (symbol == L"4") return 3;
	else if (symbol == L"5") return 4;
	else if (symbol == L"6") return 5;
	else if (symbol == L"7") return 6;
	else if (symbol == L"8") return 7;
	else if (symbol == L"9") return 8;
	else if (symbol == L"10") return 9;
	else if (symbol == L"J") return 10;
	else if (symbol == L"Q") return 11;
	else if (symbol == L"K") return 12;
	else if (symbol == L"BJOKER" || symbol == L"JOKER") return 13;
	else if (symbol == L"RJOKER") return 14;
	else return -1;  // For unknown values
}


