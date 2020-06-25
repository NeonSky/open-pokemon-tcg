#include "card_playmat.hpp"

using namespace open_pokemon_tcg::game::view;

CardPlaymat::CardPlaymat(std::unique_ptr<IPlaymat> playmat) : _playmat(playmat.release()) {}
CardPlaymat::~CardPlaymat() {}
