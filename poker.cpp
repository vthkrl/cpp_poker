#include <iostream>
#include <vector>
#include <string>
#include <array>
#include <algorithm>
#include <random>
#include <unordered_map>

using namespace std;

enum class Suits {
    SPADES, CLUBS, HEARTS, DIAMONDS
};

array<Suits, 4> suitArray = {Suits::SPADES, Suits::CLUBS, Suits::HEARTS, Suits::DIAMONDS};

array<string, 4> suitSymbols = {"♠", "♣", "♥", "♦"};

string getRankName(int rank);
string getSuitSymbol(Suits suit);

class Card {
    public:
        Suits suit;
        int rank;
    
        string cardToString() {
            string rankStr = getRankName(rank);
            string suitStr = getSuitSymbol(suit);

            return rankStr + suitStr;
        }
};

class Deck {
    public:
        vector<Card> remaining_cards = generate_deck();
        
        vector<Card> generate_deck() {
            vector<Card> deck = {};

            for (int i = 0; i < 4; i++) {
                for (int j = 2; j < 15; j++) {
                    Card new_card;
                    new_card.suit = suitArray[i];
                    new_card.rank = j;
                    deck.push_back(new_card);
                }
            }

            return deck;
        }

        void shuffle() {
            random_device rd;
            mt19937 g(rd());
            std::shuffle(remaining_cards.begin(), remaining_cards.end(), g);
        }
};

class HeldCards {
    public:
        array<Card, 2> held_cards;
    
};

enum PokerHands {
    HIGH_CARD, PAIR, TWO_PAIR, THREE_OAK, STRAIGHT, FLUSH, FULL_HOUSE, FOUR_OAK, STRAIGHT_FLUSH
};

class Hand {
    public:
        array<Card, 5> hand;
        vector<Card> relevant_cards;

        unordered_map<int, int> getRankCounts() {
            unordered_map<int, int> rankCounts;

            for (int i = 0; i < 5; i++) {
                rankCounts[hand[i].rank]++;
            }

            return rankCounts;
        }

        bool checkPair() {
            unordered_map<int, int> rankCounts = getRankCounts();

            int pairs = 0;

            for (const auto& p : rankCounts) {
                if (p.second == 2) {
                    pairs++;
                }
            }

            return pairs == 1;
        }

        bool check2Pair() {
            unordered_map<int, int> rankCounts = getRankCounts();

            int pairs = 0;

            for (const auto& p : rankCounts) {
                if (p.second == 2) {
                    pairs++;
                }
            }

            return pairs == 2;
        }

        bool check3OAK() {
            unordered_map<int, int> rankCounts = getRankCounts();

            bool three_oak = false;

            for (const auto& p : rankCounts) {
                if (p.second == 3) {
                    three_oak = true;
                }
            }

            return three_oak;
        }

        bool checkStraight() {

        }

        bool checkFlush() {
            unordered_map<Suits, int> suitCounts;

            for (int i = 0; i < 5; i++) {
                suitCounts[hand[i].suit]++;
            }

            for (const auto& p : suitCounts) {
                if (p.second == 5) {
                    return true;
                }
            }

            return false;
        }

        bool checkFullHouse() {
            unordered_map<int, int> rankCounts = getRankCounts();

            bool pair = false;
            bool three_oak = false;

            for (const auto& p : rankCounts) {
                if (p.second == 3) {
                    three_oak = true;
                }

                else if (p.second == 2) {
                    pair = true;
                }
            }

            return pair && three_oak;
        }

        bool check4OAK() {
            unordered_map<int, int> rankCounts = getRankCounts();

            bool four_oak = false;

            for (const auto& p : rankCounts) {
                if (p.second == 4) {
                    four_oak = true;
                }
            }

            return four_oak;
        }

        bool checkStraightFlush() {
            return checkStraight() && checkFlush();
        }

        PokerHands evaluateHand() {
            if (checkStraightFlush()) return STRAIGHT_FLUSH;
            if (check4OAK()) return FOUR_OAK;
            if (checkFullHouse()) return FULL_HOUSE;
            if (checkFlush()) return FLUSH;
            if (checkStraight()) return STRAIGHT;
            if (check3OAK()) return THREE_OAK;
            if (check2Pair()) return TWO_PAIR;
            if (checkPair()) return PAIR;
            return HIGH_CARD;
        }
};

string getSuitSymbol(Suits suit) {
    return suitSymbols[static_cast<int>(suit)];
}

string getRankName(int rank) {
    if (rank >= 2 && rank <= 10) return to_string(rank);
    switch(rank) {
        case 11: return "J";
        case 12: return "Q";
        case 13: return "K";
        case 14: return "A";
        default: return "?";
    }
}

int main() {
    Deck deck;
    deck.shuffle();
    
    for (int i = 0; i < deck.remaining_cards.size(); i++) {
        cout << i << ". " << deck.remaining_cards[i].cardToString() << endl;
    }
}