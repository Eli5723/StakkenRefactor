#pragma once

#include <type_traits>

#include <types.h>
#include <SFML/Network/Packet.hpp>

template <typename Enum, std::enable_if_t<std::is_enum_v<Enum>>* = nullptr>
sf::Packet& operator<<(sf::Packet& packet, Enum e)
{
  using Underlying = std::underlying_type_t<Enum>;
  packet << static_cast<Underlying>(e);
  return packet;
}

template <typename Enum, std::enable_if_t<std::is_enum_v<Enum>>* = nullptr>
sf::Packet& operator>>(sf::Packet& packet, Enum& e)
{
  using Underlying = std::underlying_type_t<Enum>;

  Underlying value{};
  packet >> value;

  e = static_cast<Enum>(value);
  return packet;
}
