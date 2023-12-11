// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// inventory.cpp
//
#include "item/inventory.hpp"

#include "misc/check-macros.hpp"

namespace castlecrawl::item
{
    Inventory::Inventory()
        : m_items()
    {}

    void Inventory::add(const ItemVec_t & items)
    {
        for (const Item & item : items)
        {
            m_items.push_back(item);
        }
    }

    void Inventory::remove(const std::size_t index)
    {
        M_CHECK(index < m_items.size(), "Tried to remove(" << index << ") out of bounds index!");
        m_items.erase(std::begin(m_items) + static_cast<std::ptrdiff_t>(index));
    }

    const std::string Inventory::equip(const std::size_t index)
    {
        M_CHECK(index < m_items.size(), "Tried to equip(" << index << ") out of bounds index!");

        const Item & item = m_items[index];

        if (item.isWeapon() && hasWeaponEquipped())
        {
            return "Already has a weapon equipped.";
        }
        else if (item.isArmor())
        {
            if (hasEquipped(item.armorType()))
            {
                std::string str;
                str += "Already has ";
                str += toString(item.armorType());
                str += " equipped.";
                return str;
            }
        }
        else if (item.isMisc())
        {
            if (!isMiscEquipable(item.miscType()))
            {
                std::string str;
                str += toString(item.miscType());
                str += " can't be equipped.";
                return str;
            }

            const std::size_t count = miscEquipCount(item.miscType());
            if (countOfEquipped(item.miscType()) >= count)
            {
                std::string str;
                str += "Already has ";
                str += std::to_string(count);
                str += " equipped.";
                return str;
            }
        }

        m_eqItems.push_back(m_items[index]);
        m_items.erase(std::begin(m_items) + static_cast<std::ptrdiff_t>(index));
        return "";
    }

    void Inventory::unequip(const std::size_t index)
    {
        M_CHECK(index < m_eqItems.size(), "Tried to unequip(" << index << ") out of bounds index!");
        m_items.push_back(m_eqItems[index]);
        m_eqItems.erase(std::begin(m_eqItems) + static_cast<std::ptrdiff_t>(index));
    }

    const EquipEffect Inventory::totalEquipEffects() const
    {
        EquipEffect equipEffect;

        for (const Item & item : m_eqItems)
        {
            equipEffect += item.equipEffect();
        }

        return equipEffect;
    }

    Armor_t Inventory::armorRating() const
    {
        Armor_t armorRating{ 0_armor };

        for (const Item & item : m_eqItems)
        {
            if (item.isArmor())
            {
                armorRating += item.armorRating();
            }
        }

        return armorRating;
    }

    bool Inventory::hasWeaponEquipped() const
    {
        for (const Item & item : m_eqItems)
        {
            if (item.isWeapon())
            {
                return true;
            }
        }

        return false;
    }

    const std::optional<Item> Inventory::weaponEquipped() const
    {
        for (const Item & item : m_eqItems)
        {
            if (item.isWeapon())
            {
                return item;
            }
        }

        return std::nullopt;
    }

    bool Inventory::hasEquipped(const Armor armor) const
    {
        for (const Item & item : m_eqItems)
        {
            if (item.armorType() == armor)
            {
                return true;
            }
        }

        return false;
    }

    std::size_t Inventory::countOfEquipped(const Misc misc) const
    {
        std::size_t count = 0;

        for (const Item & item : m_eqItems)
        {
            if (item.miscType() == misc)
            {
                ++count;
            }
        }

        return count;
    }

} // namespace castlecrawl::item
