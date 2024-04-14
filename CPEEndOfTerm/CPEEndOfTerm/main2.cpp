#include <iostream>
#include <cstdlib>  
#include <ctime>    

using namespace std;

class Card;

class Participant;
class Player;
class Computer;

Card* GenerateDeck();
void Shuffle(Card* deck);
void SplitCards(Card* cards, Participant* participants); // might want to add parameter computer
void DisplayHand(Card* cards);

int main() {
    // Initialize players
    Player* player = new Player();
    Computer* cp1 = new Computer("Computer 1");
    Computer* cp2 = new Computer("Computer 2");

    // Create group of participants
    Participant* participants[3] = { player, cp1, cp2};

    // Generate deck
	Card* cards = GenerateDeck();

    // Default start game
	bool gameOn = true;

    // Play history
    int* prevCard[3];

    // Starting index
    int index;

    Participant* winner;

	while (gameOn) {
		Shuffle(cards);
		SplitCards(cards, *participants);

		srand(time(NULL));
		index = rand() % 3;

        // First player play
        prevCard[index % 3] = participants[index % 3]->FreeMove();
        index++;

		while (true) {

            if (prevCard[index % 3] != nullptr) { // If previous move was never contested
                prevCard[index % 3] = participants[index % 3]->FreeMove();  // Have a free move
            }
            else { //If there is a previous move
                prevCard[index % 3] = participants[index % 3]->Move(prevCard);  // Move according to prev move
            }

            // If successfully contested
            if (prevCard[index % 3] != nullptr) {
                // Clear other history
                prevCard[(index+1)% 3] = nullptr;
                prevCard[(index+2)%3] = nullptr;
            }

            // If no more card left
            if (participants[index % 3]->GetCardNum() == 0) {
                // Get winner and break loop
                winner = participants[index % 3];
                break;
            }

            // Advance to next player
            index++;
		}

        cout << "Winner is: " << winner->GetName() << endl;
    }
}

Card* GenerateDeck(){
    Card* cards = new Card[54];

    for (int i = 0; i < 13; i++) {
        for (int j = 0; j < 4; j++) {
            cards[i * 4 + j] = Card(j, i);
        }
    }
    cards[52] = Card(4, 0);
    cards[53] = Card(4, 1);

    return cards;
}

void Shuffle(Card* deck) {
    srand(time(NULL));

    for (int i = 54 - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        swap(deck[i], deck[j]);
    }
}

void SplitCards(Card* cards, Participant* participants) {

	for (int i = 0; i < 18; i++) {
		for (int j = 0; j < 3; j++) {
			participants[j].SetCards(i, cards[j * 18 + i]);
		}

	}
}

void DisplayHand(Card* cards) {

}

class Participant {

protected:
    Card* cards;
    int numOfCards;
    string name;

public:

    void SetCards(int index, Card value) {
        cards[index] = value;
    }

    int GetCardNum() {
        return numOfCards;
    }

    string GetName() {
        return name;
    }

    virtual int* FreeMove() const = 0;

    virtual int* Move(int* prevCard[3]) const = 0;

};

class Player : public Participant {

public:
    Player() {
        cards = new Card[18];
        numOfCards = 18;
        name = "player";
    }

    int* FreeMove() const override {
        int* input = new int[2];

        DisplayHand(cards);

        cout << "No one contests you. What do you want to play? " << endl;
        cin >> input[0] >> input[1];

        // Validation

        return input;
    }

    int* Move(int* prevCard[3]) const override {

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
        
            // If passed:
            return input;
            // Else go back to prompt
        }


    }

};

class Computer : public Participant {

public:
    Computer(string n) {
        cards = new Card[18];
        numOfCards = 18;
        name = n;
    }

    int* FreeMove() const override {

        // No contest AI logic
        sort(cards,cards+18);
    }

    int* Move(int* prevCard[3]) const override {
        sort(cards,cards+18);
        int* input = new int[2];

        // computer AI logic
        int i = 0,cnt=0,t=0;
        for (; prevCard[i] == nullptr; i++);
        int* prevEnter = prevCard[i];
        
        int numOfPlayed=prevEnter[0];
        int valueOfPlayed=prevEnter[1];
        if(numOfPlayed==1 && (valueOfPlayed!=18)){
            for(int i=0;1<18;i++)
            {
                if(cards[i]>valueOfPlayed)
                {
                    input[1]=1;
                    input[2]=cards[i];
                    break;
                }
            }
        }
        else if (numOfPlayed==2 && (valueOfPlayed!=18)){
            for(int i=0;i<18;i++)
            {
                if(cards[i]>valueOfPlayed){
                    t=cards[i];
                    for(int j=i+1;j<18;j++)
                        if(cards[j]==t)
                            cnt++;
                }
                if(cnt>=1){
                    input[1]=2;
                    input[2]=t;
                    break;
                }
            }
        }
        else if(numOfPlayed==3&&(valueOfPlayed!=18)){
            for(int i=0;i<18;i++)
            {
                if(cards[i]>valueOfPlayed){
                    t=cards[i];
                    for(int j=i+1;j<18;j++)
                        if(cards[j]==t)
                            cnt++;
                }
                if(cnt>=2)
                {
                    input[1]=3;
                    input[2]=t;
                    break;
                }
            }
        }
        else{
            for(int i=0;i<18;i++)
            {
                if(cards[i]>valueOfPlayed){
                    t=cards[i];
                    for(int j=i+1;j<18;j++)
                        if(cards[j]==t)
                            cnt++;
                }
                if(cnt>=3){
                    input[1]=4;
                    input[2]=t;
                    break;
                }
                   
            }
        }
        if (valueOfPlayed!=18){
            if(numOfPlayed==1)
            {
                for(int i=0;1<18;i++)
                {
                    t=cards[i];
                    for(int j=i+1;j<18;j++)
                        if(cards[j]==t)
                            cnt++;
                    if(cnt>=1)
                        input[1]=2;
                        input[2]=t;
                        break;
                }
            }
            else if (numOfPlayed==2){
                if(numOfPlayed==1)
                {
                    for(int i=0;1<18;i++)
                    {
                        t=cards[i];
                        for(int j=i+1;j<18;j++)
                            if(cards[j]==t)
                                cnt++;
                        if(cnt>=2)
                            input[1]=3;
                            input[2]=t;
                            break;
                }
            }
            else if (numOfPlayed==3){
                if(numOfPlayed==1)
                {
                    for(int i=0;1<18;i++)
                    {
                        t=cards[i];
                        for(int j=i+1;j<18;j++)
                            if(cards[j]==t)
                                cnt++;
                        if(cnt>=3)
                            input[1]=4;
                            input[2]=t;
                            break;
                }
            }
        }
        else{
            for(int i=0;i<18;i++)
                if(cards[i]== or )//大王的编号 or 小王的编号
                    cnt++;
            if (cnt==2){
            //出大小王
            }
        }
        return input; // Or return nullptr if no bigger
    }
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
        if (s >= 0 && s <= 4 && v >= 0 && v < 13) {
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
};

enum Suit {
    Hearts = 0,
    Diamonds = 1,
    Clubs = 2,
    Spades = 3,
    Joker = 4
};
