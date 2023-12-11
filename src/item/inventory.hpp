#ifndef INVENTORY_HPP_INCLUDED
#define INVENTORY_HPP_INCLUDED
//
// inventory.hpp
//
#include "item/item.hpp"

#include <optional>
#include <vector>

namespace castlecrawl::item
{

    class Inventory
    {
      public:
        Inventory();

        void add(const Item & item) { m_items.push_back(item); }
        void add(const ItemVec_t & items);
        void remove(const std::size_t index);

        // returns an error message if fails
        const std::string equip(const std::size_t index);

        void unequip(const std::size_t index);

        const ItemVec_t & unItems() const { return m_items; }
        const ItemVec_t & eqItems() const { return m_eqItems; }

        const EquipEffect totalEquipEffects() const;
        Armor_t armorRating() const;
        bool hasWeaponEquipped() const;
        const std::optional<Item> weaponEquipped() const;

      private:
        bool hasEquipped(const Armor armor) const;
        std::size_t countOfEquipped(const Misc misc) const;

      private:
        ItemVec_t m_items;
        ItemVec_t m_eqItems;
    };

} // namespace castlecrawl::item

#endif // INVENTORY_HPP_INCLUDED
