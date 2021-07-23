#include <iostream>
#include <algorithm>
#include <ctime>
#include <random>
#include <array>

using namespace std ;

enum class CardSuit
{
  SUIT_CLUB,
  SUIT_DIAMOND,
  SUIT_HEART,
  SUIT_SPADE,

  MAX_SUITS
};

enum class CardRank
{
  RANK_2,
  RANK_3,
  RANK_4,
  RANK_5,
  RANK_6,
  RANK_7,
  RANK_8,
  RANK_9,
  RANK_10,
  RANK_JACK,
  RANK_QUEEN,
  RANK_KING,
  RANK_ACE,

  MAX_RANKS
};

struct Card
{
  CardRank rank{};
  CardSuit suit{};
};

void printCard(const Card& card)
{
  switch (card.rank)
  {
  case CardRank::RANK_2:
    std::cout << '2';
    break;
  case CardRank::RANK_3:
    std::cout << '3';
    break;
  case CardRank::RANK_4:
    std::cout << '4';
    break;
  case CardRank::RANK_5:
    std::cout << '5';
    break;
  case CardRank::RANK_6:
    std::cout << '6';
    break;
  case CardRank::RANK_7:
    std::cout << '7';
    break;
  case CardRank::RANK_8:
    std::cout << '8';
    break;
  case CardRank::RANK_9:
    std::cout << '9';
    break;
  case CardRank::RANK_10:
    std::cout << 'T';
    break;
  case CardRank::RANK_JACK:
    std::cout << 'J';
    break;
  case CardRank::RANK_QUEEN:
    std::cout << 'Q';
    break;
  case CardRank::RANK_KING:
    std::cout << 'K';
    break;
  case CardRank::RANK_ACE:
    std::cout << 'A';
    break;
  default:
    std::cout << '?';
    break;
  }

  switch (card.suit)
  {
  case CardSuit::SUIT_CLUB:
    std::cout << 'C' << ' ';
    break;
  case CardSuit::SUIT_DIAMOND:
    std::cout << 'D' << ' ';
    break;
  case CardSuit::SUIT_HEART:
    std::cout << 'H' << ' ';
    break;
  case CardSuit::SUIT_SPADE:
    std::cout << 'S' << ' ';
    break;
  default:
    std::cout << '?';
    break;
  }
}

using deck_type = std::array<Card, 52>;
using index_type = deck_type::size_type;

deck_type createDeck()
{
  deck_type deck{};
  index_type card{ 0 };

  auto suits{ static_cast<int>(CardSuit::MAX_SUITS) };
  auto ranks{ static_cast<int>(CardRank::MAX_RANKS) };

  for (int suit{ 0 }; suit < suits; ++suit)
  {
    for (int rank{ 0 }; rank < ranks; ++rank)
    {
      deck[card].suit = static_cast<CardSuit>(suit);
      deck[card].rank = static_cast<CardRank>(rank);
      ++card;
    }
  }

  return deck;
}

int getCardValue(const Card& card)
{
  if (card.rank <= CardRank::RANK_10)
  {
    return (static_cast<int>(card.rank) + 2);
  }

  switch (card.rank)
  {
  case CardRank::RANK_JACK:
  case CardRank::RANK_QUEEN:
  case CardRank::RANK_KING:
    return 10;
  case CardRank::RANK_ACE:
    return 11;
  default:
    return 0;
  }
}

void shuffleDeck(deck_type& deck)
{
  static std::mt19937 mt{ static_cast<std::mt19937::result_type>(std::time(nullptr)) };

  std::shuffle(deck.begin(), deck.end(), mt);
}

int checkAces(Card &card , int &numberOfAces)
{
  if(card.rank == CardRank::RANK_ACE )
  {
     numberOfAces++ ;
  }
  return numberOfAces ;
}

int updateSum(int sum , int &numberOfAces)
{
   if(numberOfAces > 0)
   {
       for(int i{numberOfAces} ; sum > 21 && i > 0 ; --i)
       {
          sum -= 10 ;
          --numberOfAces ;
       }
   }
   return sum ;
}

int singleRound(deck_type &deck , int &bet)
{
   int dealerAces{} ;
   checkAces(deck[2], dealerAces) ;
   int sumDealer{updateSum(getCardValue(deck[2]), dealerAces)};
   int playerAces{} ;
   checkAces(deck[0], playerAces) ;
   checkAces(deck[1], playerAces) ;
   int sumPlayer{updateSum(getCardValue(deck[0]) + getCardValue(deck[1]), playerAces)};

   cout << " Your cards: " ;
   printCard(deck[0]) ;
   printCard(deck[1]) ;
   cout << "\n One of dealer's card: " ;
   printCard(deck[2]) ;

   int hit{0}, flag{0} ;
   for(int currentIndex{3} ; (sumDealer <= 17)&&(sumPlayer <= 21)&&(sumDealer < sumPlayer) ; ++currentIndex)
   {
       if(sumPlayer == 21)
       {
           cout << " BlackJack!\n" ;
           break ;
       }
       for(int hit{1} ; (sumPlayer < 21)&&(hit!=0)&&(flag==0); currentIndex++)
       {
           cout << "\n (hit/stand) type(1/0): " ;
           cin >> hit ;

            if(hit == 1)
            {
                 checkAces(deck[currentIndex], playerAces) ;
                 sumPlayer = updateSum(sumPlayer + getCardValue(deck[currentIndex]), playerAces) ;
                 cout << " New card is: " ;
                 printCard(deck[currentIndex]) ;
                 cout <<'('<< sumPlayer <<')' ;
            }
            else if(hit == 0)
                {
                  ++flag;
                  break;
                }

       }
       if(sumPlayer > 21)
       {
           cout << "\n You lost because you are busted! " ;
           bet -= bet ;
           break ;
       }
       checkAces(deck[currentIndex], dealerAces) ;
       cout <<" dealer's card: " ;
       printCard(deck[currentIndex]) ;
       sumDealer = updateSum(sumDealer + getCardValue(deck[currentIndex]), dealerAces) ;
       cout <<'('<< sumDealer <<')'<<'\n' ;
   }

   if(sumDealer > 21)
   {
       cout << "\n dealer busted..you win! " ;
       bet += bet ;
   }
   else if(sumDealer > sumPlayer)
   {
       cout << "\n dealer wins! " ;
       bet -= bet ;
   }
   else if(sumDealer == sumPlayer) cout << "\n tie.." ;
   else if(sumDealer < sumPlayer && sumPlayer <= 21)
   {
       cout << "\n you win!" ;
       bet += bet ;
   }
   return bet ;
}

void fullGame(deck_type &deck)
{
    int money{1000} ;
    while(money > 0)
    {
        cout << "\n you have: $" << money << "\n place your bet: " ;
        int bet{};
        cin >> bet ;
        money -= bet ;
        int cashOut{} ;
        if(money >= 0 && cashOut == 0)
        {
            shuffleDeck(deck) ;
            money += singleRound(deck, bet);
        }
        else
        {
            cout << "\n GAME OVER" ;
            break ;
        }
        cout << "\n you have: $" << money << "\n Do you want to cash out now?" << "\n (yes/no) type (1/0): " ;
        cin >> cashOut ;
        if(cashOut == 1)
        {
            int playAgain{} ;
            cout << "\n you cashed out $" << money << "\n do you want to play again?" << "\n (yes/no) type (1/0): ";
            cin >> playAgain ;
            if(playAgain == 1) fullGame(deck) ;
            else if(playAgain == 0) break ;
        }

    }
    cout << "\n GAME OVER" ;
}

int main()
{
  auto deck{ createDeck() };
  shuffleDeck(deck) ;
  fullGame(deck);

  return 0;
}
