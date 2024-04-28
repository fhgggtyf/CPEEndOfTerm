#include <iostream>
#include <string>
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
			cout << "err";
			suit = static_cast<Suit>(0);
			value = -1;
		}
	}

	string getSuitAsSymbol() const {
		switch (suit) {
		case Hearts: return "H";
		case Diamonds: return "D";
		case Clubs: return "C";
		case Spades: return "S";
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
	int numOfCards = 0;
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
		if (input[0] == 2 && input[1] == 13) {
			int count1 = count_if(cards.begin(), cards.end(), [input](const Card& card) {
				return card.getValue() == 13;
				});
			int count2 = count_if(cards.begin(), cards.end(), [input](const Card& card) {
				return card.getValue() == 14;
				});

			cout << count1 << " " << count2 << endl;
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
string getSymbolUsingValue(int value);
int getValueUsingSymbol(string symbol);
bool checkInput(string input[2]);

class Player : public Participant {

public:
	Player(int i) {
		cards = {};
		numOfCards = 54 / PLAYERNUM;
		name = "Player " + to_string(i + 1);
	}

	int* FreeMove() override {
		string* input = new string[2];
		int* castedInput = new int[2];

		cout << name << "'s cards:" << endl;
		DisplayHand(cards);

		while (true) {
			cout << "No one contests you. What do you want to play? " << endl;

			cin >> input[0] >> input[1];

			while (!checkInput(input)) {
				cout << "Invalid Input" << endl;
				cout << "No one contests you. What do you want to play? " << endl;
				cin >> input[0] >> input[1];
			}

			castedInput[0] = stoi(input[0]);
			castedInput[1] = getValueUsingSymbol(input[1]);

			if (Validate(castedInput)) {
				MakePlay(castedInput);
				numOfCards -= castedInput[0];
				break;
			}
			else {
				cout << "Invalid Input." << endl;
			}
		}

		cout << "Played " << castedInput[0] << " number of " << getSymbolUsingValue(castedInput[1]) << endl;

		return castedInput;
	}

	int* Move(int* prevCard[PLAYERNUM]) override {

		// Find previous entry
		int i = 0;
		for (; prevCard[i] == nullptr; i++);
		int* prevEnter = prevCard[i];

		string* input = new string[2];
		int* castedInput = new int[2];

		cout << name << "'s cards:" << endl;
		DisplayHand(cards);

		// Validation
		while (true) {
			cout << "Previous player played " << prevEnter[0] << " number of " << getSymbolUsingValue(prevEnter[1]) << ". What do you want to play? " << endl;
			cin >> input[0] >> input[1];

			while (!checkInput(input)) {
				cout << "Invalid Input" << endl;
				cout << "Previous player played " << prevEnter[0] << " number of " << getSymbolUsingValue(prevEnter[1]) << ". What do you want to play? " << endl;
				cin >> input[0] >> input[1];
			}

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
					cout << "Card Input Invalid" << endl;
				}
			}
		}

		cout << "Played " << castedInput[0] << " number of " << getSymbolUsingValue(castedInput[1]) << endl;

		return castedInput;
	}
};

class Computer : public Participant {

public:
	Computer(int i) {
		cards = {};
		numOfCards = 54 / PLAYERNUM;
		name = "(AI)player " + to_string(i + 1);
	}
	int* FreeMove() override {
		cout << "No one contests you. What do you want to play? " << endl;
		cards = SortCards(cards);

		int* input = new int[2];
		input[0] = 1;
		input[1] = cards[0].getValue();

		MakePlay(input);
		numOfCards -= input[0];
		cout << "Played " << input[0] << " number of " << getSymbolUsingValue(input[1]) << endl;

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

		cout << "Previous player played " << prevEnter[0] << " number of " << getSymbolUsingValue(prevEnter[1]) << ". What do you want to play? " << endl;

		for (int j = vp+1; j <= 14; j++) {
			input[0] = np;
			input[1] = j;
			if (Validate(input)) {
				MakePlay(input);
				numOfCards -= input[0];

				cout << "Played " << input[0] << " number of " << getSymbolUsingValue(input[1]) << endl;
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

					cout << "Played " << input[0] << " number of " << getSymbolUsingValue(input[1]) << endl;
					return input;
				}
			}
		if (Validate(xiaow) == true && Validate(daw) == true) {//Double joker
			input[0] = 2;
			input[1] = 13;

			MakePlay(input);
			numOfCards -= input[0];
			cout << "Played " << input[0] << " number of " << getSymbolUsingValue(input[1]) << endl;
			return input;
		}
		return nullptr;
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

	choice = -1;

	while (true) {
		cout << "Welcome to the game of See Who's Fastest!" << endl;
		cout << "1) Start Game" << endl;
		cout << "2) Help" << endl;
		cout << "3) Exit" << endl;
		cin >> choice;
		while (!cin) {
			std::cin.clear(); // Clear the error flag
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			cout << "Invalid input" << endl;
			cout << "Welcome to the game of " << endl;
			cout << "1) Start Game" << endl;
			cout << "2) Help" << endl;
			cout << "3) Exit" << endl;
			cin >> choice;
		}

		if (choice == 1) {

			Participant** participants = new Participant * [PLAYERNUM];

			int choice;
			for (int i = 0; i < PLAYERNUM; i++) {
				cout << "Add a player (0: AI, 1: Player):";
				cin >> choice;
				if (choice == 1) {
					participants[i] = new Player(i);
				}
				else {
					participants[i] = new Computer(i);
				}
			}

			while (gameOn) {
				cards = Shuffle(cards);
				SplitCards(cards, participants);

				srand(time(NULL));
				index = rand() % PLAYERNUM;

				// First player play
				cout << participants[index % PLAYERNUM]->GetName() << " got the first move!" << endl;
				prevCard[index % PLAYERNUM] = participants[index % PLAYERNUM]->FreeMove();
				index++;

				while (true) {
					cout << "\n" << participants[index % PLAYERNUM]->GetName() << "'s turn!" << endl;
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
		else if (choice == 2) {
			cout << "Press 1 in the main menu to enter game. The software will give the user 18 cards (from one deck of card) to each player (including two jokers), then a randomly selected player will start the game by discarding one or more cards.The acceptable form of card discarded in this game is only one single card, two or more cards with the same value(e.g. double 5, triple 9, quadruple k, double joker, etc.)Then, the next player(order determined by the loop of user, computer 1, and computer 2; if, for instance, computer 1 is the first player to discard a deck, the next one will be computer 2, then the user, etc.) shall discard cards that’s either 1. bigger in value(e.g. 1 < 2 < ... < k < black joker < red joker) or 2. bigger in quantity of the same card, and this process continues on for every next player.Notice that quantity is always considered before number(e.g.single 5 < double 4 < triple 3 < quadruple 2 < double joker) so that whenever one player discards one single card, another player can discard double cards regardless of the value of the card, and similar things hold for triple cards and quadruple cards.Double joker is considered the biggest card(although with only two cards, double joker is considered bigger than all quadruple cards and so on, this is an exception).For cards in the same quantity, the value of the card is compared(e.g. double 4 < double 6, triple j < triple k, quadruple 5 < quadruple 9, etc.) Overall, the whole pattern of every card is as follows\n1 < 2 < ... < red joker<double 1 < double 2 < ...<double k < triple 1 < triple 2 < ... < triple k < quadruple 1 < quadruple 2 < ... < quadruple k<double joker.\nIf the next player has no card bigger than the previous user, the player shall pass.After the other two players both pass, the player who discarded the biggest card may discard any new card and this process continues on and on(e.g. if one player discards double joker, that player gets to discard a new card by his / her / its preference since double joker is the biggest card, and the other two players can do nothing but pass).The game ends when one of the three players discards all cards, and that player is the winner." << endl;
		}
		else if (choice == 3) {
			cout << "Exiting game..." << endl;
			break;
		}
		else {
			cout << "Invalid Choice" << endl;
		}

	}

	return 0;
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
		cout << "----";
	}
	cout << "------" << endl;

	for (auto it = cards.begin(); it != cards.end(); ++it) {
		if (getSymbolUsingValue(it->getValue()) == "BJoker") {
			cout << "|B  ";
		}
		else if (getSymbolUsingValue(it->getValue()) == "RJoker") {
			cout << "|R  ";
		}
		else {
			if (getSymbolUsingValue(it->getValue()) == "10") {
				cout << "|" << getSymbolUsingValue(it->getValue()) << " ";
			}
			else {
				cout << "|" << getSymbolUsingValue(it->getValue()) << "  ";
			}
		}
	}
	cout << "     |" << endl;

	for (auto it = cards.begin(); it != cards.end(); ++it) {
		if (getSymbolUsingValue(it->getValue()) == "BJoker" || getSymbolUsingValue(it->getValue()) == "RJoker") {
			cout << "|J  ";
		}
		else {
			cout << "|" << it->getSuitAsSymbol() << "  ";
		}
	}
	cout << "     |" << endl;


	for (auto it = cards.begin(); it != cards.end(); ++it) {
		if (getSymbolUsingValue(it->getValue()) == "BJoker" || getSymbolUsingValue(it->getValue()) == "RJoker") {
			cout << "|O  ";
		}
		else {
			cout << "|   ";
		}
	}
	cout << "     |" << endl;

	for (auto it = cards.begin(); it != cards.end(); ++it) {
		if (getSymbolUsingValue(it->getValue()) == "BJoker" || getSymbolUsingValue(it->getValue()) == "RJoker") {
			cout << "|K  ";
		}
		else {
			cout << "|   ";
		}
	}
	cout << "     |" << endl;

	for (auto it = cards.begin(); it != cards.end(); ++it) {
		if (getSymbolUsingValue(it->getValue()) == "BJoker" || getSymbolUsingValue(it->getValue()) == "RJoker") {
			cout << "|E  ";
		}
		else {
			cout << "|   ";
		}
	}
	cout << "     |" << endl;

	for (auto it = cards.begin(); it != cards.end(); ++it) {
		if (getSymbolUsingValue(it->getValue()) == "BJoker" || getSymbolUsingValue(it->getValue()) == "RJoker") {
			cout << "|R  ";
		}
		else {
			cout << "|   ";
		}
	}
	cout << "     |" << endl;

	for (auto it = cards.begin(); it != cards.end(); ++it) {
		cout << "----";
	}
	cout << "------" << endl;



}

vector<Card> SortCards(vector<Card> cards) {
	sort(cards.begin(), cards.end(), [](const Card& a, const Card& b) {
		return a < b; // Compare based on name for sorting
		});

	return cards;
}

string getSymbolUsingValue(int value) {
	switch (value) {
	case 0: return "A";
	case 1: return "2";
	case 2: return "3";
	case 3: return "4";
	case 4: return "5";
	case 5: return "6";
	case 6: return "7";
	case 7: return "8";
	case 8: return "9";
	case 9: return "10";
	case 10: return "J";
	case 11: return "Q";
	case 12: return "K";
	case 13: return "BJoker";
	case 14: return "RJoker";
	default: return "Unknown Value";
	}
}

int getValueUsingSymbol(const string symbol) {
	if (symbol == "A") return 0;
	else if (symbol == "2") return 1;
	else if (symbol == "3") return 2;
	else if (symbol == "4") return 3;
	else if (symbol == "5") return 4;
	else if (symbol == "6") return 5;
	else if (symbol == "7") return 6;
	else if (symbol == "8") return 7;
	else if (symbol == "9") return 8;
	else if (symbol == "10") return 9;
	else if (symbol == "J") return 10;
	else if (symbol == "Q") return 11;
	else if (symbol == "K") return 12;
	else if (symbol == "BJOKER" || symbol == "JOKER") return 13;
	else if (symbol == "RJOKER") return 14;
	else return -1;  // For unknown values
}

bool checkInput(string input[2]) {
	int start = 0;
	// Check for an optional sign at the beginning
	if (input[0][0] == '+' || input[0][0] == '-') {
		start = 1;
		// If there's only a sign and no digits, return false
		if (input[0].length() == 1) {
			return false;
		}
	}

	// Check each character from the start index
	for (int i = start; i < input[0].length(); i++) {
		if (!isdigit(input[0][i])) {
			return false;  // Return false if a non-digit is found
		}
	}

	return true;  // Return true if all characters are valid digits (post-sign if any)
}

