#include <iostream>
#include <ctime>
#include <vector>
using namespace std;


//////////////////////////////////////////////////////////////////////
////////////////////          CARD CLASS          ////////////////////
//////////////////////////////////////////////////////////////////////
class Card{
    public:
        enum rank { ACE = 1, TWO , THREE , FOUR , FIVE , SIX , SEVEN , EIGHT , NINE , TEN ,JACK , QUEEN , KING };
        enum type { CLUBS = 1, DIAMONDS , HEARTS , SPADES };
        int getValue();
        void displayCard();
        Card(rank r, type t);//not a default constructor
        ~Card();
    private:
        rank cardRank;
        type cardType;
};

Card::Card(rank r, type t){
    cardRank = r;
    cardType = t;
}

Card::~Card(){}

int Card::getValue(){
    if (cardRank < 10){// ACE up till TEN will be assigned their corresponding value ase ACE=1. 
        return cardRank;
    } else {//We have to set JACK, QUEEN, and  KING to 10 as their values will be 11, 12, and 13 respectivielly
        return 10;
    }
}

void Card::displayCard(){
    //displaying the card values
    if (cardRank <= 10){//cardRank <=10 will have their rank displayed correctly. Not JACK, QUEEN, and KING
        cout << cardRank;
    } else if(cardRank == 11){
        cout << 'J';
    }else if(cardRank == 12){
        cout << 'Q';
    }else{
        cout << 'K';
    }
    //displaying cardType
    switch (cardType){
        case CLUBS:
            cout << 'C';
            break;
        case DIAMONDS:
            cout << 'D';
            break;
        case HEARTS:
            cout << 'H';
            break;
        case SPADES:
            cout << 'S';
            break;
    }
}
//////////////////////////////////////////////////////////////////////
////////////////////          HAND CLASS          ////////////////////
//////////////////////////////////////////////////////////////////////
class Hand{
    public:
        Hand();//default constructor
        ~Hand();//the destructor of the vector already calls the destructor for every instance of card in the vector
        void add(Card c);
        void clear();
        int getTotal() const;//this function will be used in the isDrawing() from the ComputerPlayer class. Thus we need to put const
        void displayHand();
    private:
        std::vector<Card> myCards;//using std::vector <card> instead of vector<card> because of const
};

Hand::Hand(){}

Hand::~Hand(){}

void Hand::add(Card c){
    myCards.push_back(c);//adding a card using push_back from the vector library
}

int Hand::getTotal() const{
    int total=0;
    for (Card i : myCards){//sum the value for every card in the Hand to get the total. Using for loop in this form since getTotal() is marked as const
        total += i.getValue();
    }
    for(Card j:myCards){
        //to not bust with ACE==11, the total has to be at most 11 since 11 +  10 = 21. Note that we add 10 since ACE == 1 is already in the total.
        //if there is an ACE in the Hand and the total is less or equal to 11, we consider ACE to have the value 11 so we add 10 as ACE==1 is already in the total.
        if(j.getValue()==1 && total<=11)
            total+=10;
    }
    return total;
}

void Hand::clear(){
    myCards.clear();//using clear() from the vector library
}

void Hand::displayHand(){
    int size = myCards.size();//go through every card in Hand and display it.
    for(int i = 0; i<size; i++){//normal for loop is possible as displayHand() is no marked as const
        myCards[i].displayCard();
        cout << " ";//print a space between each card to make the output more readable
    }
}
//////////////////////////////////////////////////////////////////////
////////////////////          DECK CLASS          ////////////////////
//////////////////////////////////////////////////////////////////////
class Deck : public Hand{
    public:
        Deck(){}
        ~Deck(){}
        void populate();
        void shuffle();
        Card deal();
    private:
        vector<Card> myDeck;
};

void Deck:: populate(){
    //generating all cards in the nested for loop.
    for(int i=Card::type::CLUBS;i<=Card::type::SPADES;i++){//CLUBS = 1. Hence, the next element in enum type will be obtained by incrementing i by 1.
        for(int j=Card::rank::ACE;j<=Card::rank::KING;j++){//ACE = 1. Hence, the next element in enum rank will be obtained by incrementing j by 1.
            myDeck.push_back(Card(static_cast<Card::rank>(j), static_cast<Card::type>(i)));//adding each card generated in the Deck by using push_back
            //note: static_cast is used on the rank and type. Otherwise each card will have attribute <int, int> instead of <rank, type>
        }
    }
}

void Deck::shuffle(){
    srand(time(0));//get a random seed
    for (int i=0; i<52 ;i++){
        int r = i + (rand() % (52 -i));//get a random remaining card
        swap(myDeck[i], myDeck[r]);//swap the current card to the randomly chosen card to shuffle
    }
}

Card Deck::deal(){
    Card c = myDeck[myDeck.size() - 1];//pick the last card in the Deck to deal.
    myDeck.pop_back();//take out the dealt card from the Deck so that the next time we deal, we can give the new "last card" to a player.
    return c;
}

/////////////////////////////////////////////////////////////////////////////////
////////////////////          ABSTRACT PLAYER CLASS          ////////////////////
/////////////////////////////////////////////////////////////////////////////////
class AbstractPlayer{
    public:
        AbstractPlayer(){}
        ~AbstractPlayer(){}
        virtual bool isDrawing() const = 0;
        bool isBusted();
        Hand aHand;
};
bool AbstractPlayer::isBusted(){
    if (aHand.getTotal() > 21){//total is bigger than 21 then the player is busted
        return true;
    }
    return false;
}
//////////////////////////////////////////////////////////////////////////////
////////////////////          HUMAN PLAYER CLASS          ////////////////////
//////////////////////////////////////////////////////////////////////////////
class HumanPlayer : public AbstractPlayer{
    public:
        HumanPlayer(){}
        ~HumanPlayer(){}
        bool isDrawing() const override;
        void announce(int casTotal);
};

bool HumanPlayer::isDrawing() const{
    char draw;
    while (true){
        cout<<"Do you want to draw? (y/n) \n";
        cin>>draw;
        if (draw == 'y'){//if input is  'y' then return true and break out of the loop
            return true;
            break;
        }else if (draw == 'n'){//if input is 'no' then return false and break out of the loop
            return false;
            break;
        }else{//otherwise will be asked if the player wishes to draw until valid input is found
            cout << "Invalid input. ";
        }

    }
}

void HumanPlayer::announce(int casTotal){
    if (isBusted()){//if the player busts, then Casino wins
        cout << "Player busts. Casino wins.\n";
    }
    else if (casTotal> 21){//if casino busts, but player didn't bust yet, then the player wins
        cout << "Casino busts. Player wins.\n";
    }
    else if (casTotal == aHand.getTotal()){//if no one busted and same total, then it's a draw/push
        cout << "Push: No one wins.\n";
    }
    else if(aHand.getTotal() < casTotal){//no one has busted and casino has a bigger total, then casino wins
        cout << "Casino has higher total. Casino wins. \n";
    }else if(aHand.getTotal() > casTotal){// no one has busted and plyer has a bigger total, then player wins
        cout << "Player has higher total. Player wins \n";
    }
}
/////////////////////////////////////////////////////////////////////////////////
////////////////////          COMPUTER PLAYER CLASS          ////////////////////
/////////////////////////////////////////////////////////////////////////////////
class ComputerPlayer : public AbstractPlayer{
    public:
        ComputerPlayer(){}
        ~ComputerPlayer(){}
        bool isDrawing() const override;
        int getTotal();
};

bool ComputerPlayer::isDrawing() const{
    if (aHand.getTotal() <= 16){//computer will stop drawing if total is bigger than 16.
        return true;
    }else{
        return false;
    }
}
////////////////////////////////////////////////////////////////////////////////
////////////////////          BLACKJACK GAME CLASS          ////////////////////
////////////////////////////////////////////////////////////////////////////////
class BlackJackGame{
    private:
        Deck m_deck;
        ComputerPlayer m_casino;
        HumanPlayer m_player;
    public:
        //BlackJackGame() = default;
        BlackJackGame(){}
        ~BlackJackGame(){}
        void play();
};

void BlackJackGame::play(){
    //initializing the variables when a new game starts
    int casTot=0;
    int pTot=0;
    m_deck = Deck();//initialize the deck
    m_deck.populate();//populate the deck
    m_deck.shuffle();//suffle the deck
    m_casino.aHand.clear();//clear the Hand of CompterPlayer in case many games were played
    m_player.aHand.clear();//clear the Hand of HumanPlayer in case many games were played

    //deal 1 card to casino and print out the Hand and it's total
    m_casino.aHand.add(m_deck.deal());
    cout << "Casino: ";
    m_casino.aHand.displayHand();
    casTot = m_casino.aHand.getTotal();
    cout << "[" << casTot << "]\n";

    //deal 2 cards to player and print out the Hand and it's total
    m_player.aHand.add(m_deck.deal());
    m_player.aHand.add(m_deck.deal());
    cout <<"Player: ";
    m_player.aHand.displayHand();
    pTot= m_player.aHand.getTotal();
    cout << "[" << pTot << "]\n";

    //The drawings will now begin
    if(m_casino.isBusted() || m_player.isBusted()){//see if one of the players have busted
        m_player.announce(m_casino.aHand.getTotal());//announce the result if someone busted
        return;
    }else{//otherwise we will start drawing.
        while (!m_player.isBusted() && m_player.isDrawing()){//player can draw as long as he's not busted and will still draw (can decide not to draw)
            m_player.aHand.add(m_deck.deal());//deal a card, add it to the player's Hand and display the cards and total
            cout << "Player ";
            m_player.aHand.displayHand();
            int pTot = m_player.aHand.getTotal();
            cout <<'[' << pTot << "]\n";
        }
        if (m_player.isBusted()){// if player is busted then print the result and end this function.
            m_player.announce(m_casino.aHand.getTotal());
            return;
        }
        while (m_casino.isDrawing()){//if player didn't bust, then casino can draw. Casino will stop drawing when it's total>16
            m_casino.aHand.add(m_deck.deal());//deal a card and display cards in Hand along with total
            cout << "Casino: ";
            m_casino.aHand.displayHand();
            int casTot = m_casino.aHand.getTotal();
            cout << "[" << casTot << "]\n";
        }
    m_player.announce(m_casino.aHand.getTotal());//announce the results. This part of the code will be reached when the player hasn't busted. 
                                                 //The announcement when player is busted is made before the casino starts drawing.
    }
}

int main(){
    cout << "\tWelcome to the Comp322 Blackjack game!" << endl << endl;
    BlackJackGame game;
    // The main loop of the game
    bool playAgain = true;
    char answer = 'y';
    while (playAgain){
        game.play();
        // Check whether the player would like to play another round
        cout << "Would you like another round? (y/n): ";
        cin >> answer;
        cout << endl << endl;
        playAgain = (answer == 'y' ? true : false);
    }
    cout <<"Gave over!";
    return 0;
}
