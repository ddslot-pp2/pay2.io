#include "cards.hpp"
#include <iostream>
cards::cards(int deck_count) {
  deck_count_ = deck_count;
}

void cards::shuffle() {
  q_.clear();

  auto start = static_cast<int>(card_type::SPADE_ACE);
  auto end = static_cast<int>(card_type::END);

  for(auto i=0; i<deck_count_; ++i) {
    for(auto j=start; j<end; ++j) {
      auto c = static_cast<card_type>(j);
      q_.push_back(c);
    }
  }
 
  std::random_device rd;
  std::mt19937 g(rd());
  std::shuffle(q_.begin(), q_.end(), g);

  /*
  for(const auto& i : q_) {
    auto c = static_cast<int>(i);
    std::cout << c << std::endl;
  }
  */
}

std::pair<cards::card_type, int> cards::get_card() {
  return std::make_pair(card_type::SPADE_ACE, 5);
}
