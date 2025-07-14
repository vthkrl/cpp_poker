#include <iostream>
#include <vector>
#include <string>
#include <array>
#include <algorithm>
#include <random>
#include <unordered_map>
#include <chrono>

using namespace std;
using namespace std::chrono;

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
        PokerHands hand_type;

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
            vector<int> ranks;

            for (const auto& card : hand) {
                ranks.push_back(card.rank);
            }

            sort(ranks.begin(), ranks.end());

            if (ranks == vector<int> {2, 3, 4, 5, 14}) {
                return true;
            }

            for (int i = 1; i < 5; i++) {
                if (ranks[i] == ranks[i-1]) {
                    return false;
                }
            }

            for (int i = 1; i < 5; i++) {
                if (ranks[i] - ranks[i-1] != 1) {
                    return false;
                }
            }

            return true;
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

            return pair && three_oak && rankCounts.size() == 2;
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

string pokerHandToString(PokerHands hand) {
    switch(hand) {
        case HIGH_CARD: return "High Card";
        case PAIR: return "Pair";
        case TWO_PAIR: return "Two Pair";
        case THREE_OAK: return "Three of a Kind";
        case STRAIGHT: return "Straight";
        case FLUSH: return "Flush";
        case FULL_HOUSE: return "Full House";
        case FOUR_OAK: return "Four of a Kind";
        case STRAIGHT_FLUSH: return "Straight Flush";
        default: return "Unknown";
    }
}

void checkingHands(int SIMULATIONS) {
    for (int i = 0; i < SIMULATIONS; i++) {
        Deck deck;
        deck.shuffle();

        Hand hand;

        cout << "Testing Hand: \n";

        for (int i = 0; i < 5; i++) {
            hand.hand[i] = deck.remaining_cards[i];
            cout << hand.hand[i].cardToString() << " ";
        }

        cout << "\n";

        PokerHands result = hand.evaluateHand();

        cout << "Evaluated as: " << pokerHandToString(result) << "\n";
    }
}

void simulateHands(int SIMULATIONS) {
    array<int, 9> handCounts = {0};  // Index matches PokerHands enum

    Deck deck;
    Hand hand;

    random_device rd;
    mt19937 g(rd());

    for (int i = 0; i < SIMULATIONS; ++i) {
        // Shuffle the shared deck
        shuffle(deck.remaining_cards.begin(), deck.remaining_cards.end(), g);

        // Take the first 5 cards into the hand
        for (int j = 0; j < 5; j++) {
            hand.hand[j] = deck.remaining_cards[j];
        }

        // Evaluate the hand and count it
        PokerHands result = hand.evaluateHand();
        handCounts[result]++;
    }

    // Display results
    cout << "After " << SIMULATIONS << " simulated hands:\n";
    for (int i = 0; i < handCounts.size(); ++i) {
        cout << pokerHandToString(static_cast<PokerHands>(i)) << ": " << handCounts[i] << "\n";
    }
}

int main() {
    auto start = high_resolution_clock::now();

    simulateHands(100000000);

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<minutes>(end - start);
    cout << "Simulation completed in " << duration.count() << " minutes\n";

    return 0;
}

