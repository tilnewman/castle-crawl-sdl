// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// item.cpp
//
#include "item/item.hpp"

#include <ostream>

namespace castlecrawl::item
{
    Item::Item()
        : m_value(0)
        , m_weapon(Weapon::Count)
        , m_armor(Armor::Count)
        , m_misc(Misc::Count)
        , m_name()
        , m_armorMaterial(ArmorMaterial::Count)
        , m_weaponMaterial(WeaponMaterial::Count)
        , m_miscMaterial(MiscMaterial::Count)
        , m_useStrength(UseStrength::Normal)
        , m_armorRating(0)
        , m_damageMin(0)
        , m_damageMax(0)
        , m_useEffect()
        , m_equipEffect()
    {
        m_value = calcValue(); // should be zero for invalid items anyway, consistency, meh
    }

    Item::Item(const Weapon weapon, const WeaponMaterial material)
        : m_value(0)
        , m_weapon(weapon)
        , m_armor(Armor::Count)
        , m_misc(Misc::Count)
        , m_name(toString(weapon))
        , m_armorMaterial(ArmorMaterial::Count)
        , m_weaponMaterial(material)
        , m_miscMaterial(MiscMaterial::Count)
        , m_useStrength(UseStrength::Normal)
        , m_armorRating(0)
        , m_damageMin(baseWeaponDamage(weapon).x + weaponMaterialDamage(material))
        , m_damageMax(baseWeaponDamage(weapon).y + weaponMaterialDamage(material))
        , m_useEffect()
        , m_equipEffect()
    {
        m_value = calcValue();
    }

    Item::Item(
        const Weapon weapon,
        const WeaponMaterial material,
        const std::string & uniqueName,
        const EquipEffect & effect)
        : m_value(0)
        , m_weapon(weapon)
        , m_armor(Armor::Count)
        , m_misc(Misc::Count)
        , m_name(uniqueName)
        , m_armorMaterial(ArmorMaterial::Count)
        , m_weaponMaterial(material)
        , m_miscMaterial(MiscMaterial::Count)
        , m_useStrength(UseStrength::Normal)
        , m_armorRating(0)
        , m_damageMin(baseWeaponDamage(weapon).x + weaponMaterialDamage(material))
        , m_damageMax(baseWeaponDamage(weapon).y + weaponMaterialDamage(material))
        , m_useEffect()
        , m_equipEffect(effect)
    {
        m_value = calcValue();
    }

    Item::Item(const Armor armor, const ArmorMaterial material)
        : m_value(0)
        , m_weapon(Weapon::Count)
        , m_armor(armor)
        , m_misc(Misc::Count)
        , m_name(toString(armor))
        , m_armorMaterial(material)
        , m_weaponMaterial(WeaponMaterial::Count)
        , m_miscMaterial(MiscMaterial::Count)
        , m_useStrength(UseStrength::Normal)
        , m_armorRating(baseArmorRating(armor) + armorMaterialRating(material))
        , m_damageMin(0)
        , m_damageMax(0)
        , m_useEffect()
        , m_equipEffect()
    {
        m_value = calcValue();
    }

    Item::Item(
        const Armor armor,
        const ArmorMaterial material,
        const std::string & uniqueName,
        const EquipEffect & effect)
        : m_value(0)
        , m_weapon(Weapon::Count)
        , m_armor(armor)
        , m_misc(Misc::Count)
        , m_name(uniqueName)
        , m_armorMaterial(material)
        , m_weaponMaterial(WeaponMaterial::Count)
        , m_miscMaterial(MiscMaterial::Count)
        , m_useStrength(UseStrength::Normal)
        , m_armorRating(baseArmorRating(armor) + armorMaterialRating(material))
        , m_damageMin(0)
        , m_damageMax(0)
        , m_useEffect()
        , m_equipEffect(effect)
    {
        m_value = calcValue();
    }

    Item::Item(
        const Misc misc,
        const MiscMaterial material,
        const UseStrength strength,
        const UseEffect & useEffect,
        const EquipEffect & equipEffect)
        : m_value(0)
        , m_weapon(Weapon::Count)
        , m_armor(Armor::Count)
        , m_misc(misc)
        , m_name(toString(misc))
        , m_armorMaterial(ArmorMaterial::Count)
        , m_weaponMaterial(WeaponMaterial::Count)
        , m_miscMaterial(material)
        , m_useStrength(strength)
        , m_armorRating(0)
        , m_damageMin(0)
        , m_damageMax(0)
        , m_useEffect(useEffect)
        , m_equipEffect(equipEffect)
    {
        m_value = calcValue();
    }

    const std::string Item::name() const
    {
        std::string str;
        str.reserve(100);

        if (isArmor())
        {
            if (!isMagical())
            {
                str += toString(m_armorMaterial);
                str += ' ';
            }
        }
        else if (isWeapon())
        {
            if (!isMagical())
            {
                str += toString(m_weaponMaterial);
                str += ' ';
            }
        }
        else // must be misc
        {
            if (isUseable() && (m_useStrength != UseStrength::Normal))
            {
                str += toString(m_useStrength);
                str += ' ';
            }

            if (isUseable())
            {
                if (m_useEffect.health > 0)
                {
                    str += "Healing ";
                }
                else
                {
                    str += "Mana ";
                }
            }
            else
            {
                str += toString(m_miscMaterial);
                str += ' ';
            }
        }

        str += m_name;
        return str;
    }

    const std::string Item::description() const
    {
        std::string str;
        str.reserve(200);

        str += name();
        str += " is";

        if (isWeapon())
        {
            str += " a";

            if (isMagical())
            {
                str += " magical";
            }

            str += " weapon that does between ";
            str += std::to_string(m_damageMin);
            str += " and ";
            str += std::to_string(m_damageMax);
            str += " damage";
        }
        else if (isArmor())
        {
            if (isMagical())
            {
                str += " magical";
            }

            str += " armor that has a rating of ";
            str += std::to_string(m_armorRating.get());
        }
        else // must be a misc item
        {
            str += " a magical item";

            if (isUseable())
            {
                str += " that is useable";
            }

            const auto eqCount = equipCount();
            if (eqCount > 0)
            {
                str += " that you can equip";

                if (eqCount > 1)
                {
                    str += ' ';
                    str += std::to_string(eqCount);
                    str += " of";
                }
            }
        }

        if (!m_equipEffect.empty())
        {
            str += " and grants ";

            const EquipEffect & ef = m_equipEffect;

            // clang-format off
                if (ef.acc > 0) { str += "+" + std::to_string(ef.acc) + "acc, "; }
                if (ef.arc > 0) { str += "+" + std::to_string(ef.arc) + "arc, "; }
                if (ef.dex > 0) { str += "+" + std::to_string(ef.dex) + "dex, "; }
                if (ef.dmg > 0) { str += "+" + std::to_string(ef.dmg) + "dmg, "; }
                if (ef.lck > 0) { str += "+" + std::to_string(ef.lck) + "lck, "; }
                if (ef.str > 0) { str += "+" + std::to_string(ef.str) + "str, "; }
            // clang-format on

            str.pop_back();
            str.pop_back();
        }

        str += '.';
        return str;
    }

    int Item::calcValue() const
    {
        int value = 0;

        value += ((m_armorRating * m_armorRating) * 5_armor).get();
        value += ((m_damageMin * m_damageMin) * 5);
        value += ((m_damageMax * m_damageMax) * 5);

        if (isUseable())
        {
            value += 10;
        }

        if (isEquipable())
        {
            value += 4;
        }

        value += miscMaterialValue(m_miscMaterial);

        value += (m_equipEffect.total() * m_equipEffect.total());
        value += (m_equipEffect.total() * 250);

        value += ((m_useEffect.total() / 2) * m_useEffect.total());
        value += (m_useEffect.total() * 25);

        return value;
    }

    std::ostream & operator<<(std::ostream & os, const Item & item)
    {
        os << '[' << item.name();

        if (item.isArmor())
        {
            os << ",Armor";
        }

        if (item.isWeapon())
        {
            os << ",Weapon";
        }

        if (item.isMisc())
        {
            os << ",Misc";
        }

        if (item.armorRating() != 0_armor)
        {
            os << ",armorRating=" << item.armorRating();
        }

        if ((item.damageMin() != 0) || (item.damageMax() != 0))
        {
            os << ",damage=" << item.damageMin() << '-' << item.damageMax();
        }

        if (item.isMisc())
        {
            if (item.isUseable())
            {
                os << ",useable";
            }

            if (item.equipCount() > 1)
            {
                os << ",equip_count=" << item.equipCount();
            }
            else if (item.equipCount() == 1)
            {
                os << ",equipable";
            }
        }

        const EquipEffect & ef = item.equipEffect();
        const UseEffect & uf = item.useEffect();

        // clang-format off
        if (ef.acc > 0) { os << ",+" << ef.acc << "acc"; }
        if (ef.arc > 0) { os << ",+" << ef.arc << "arc"; }
        if (ef.dex > 0) { os << ",+" << ef.dex << "dex"; }
        if (ef.dmg > 0) { os << ",+" << ef.dmg << "dmg"; }
        if (ef.lck > 0) { os << ",+" << ef.lck << "lck"; }
        if (ef.str > 0) { os << ",+" << ef.str << "str"; }
        //
        if (uf.health > 0) { os << ",+" << uf.health << "health"; }
        if (uf.mana > 0)   { os << ",+" << uf.mana << "mana";     }
        // clang-format on

        os << ",value=" << item.value();

        os << ']';
        return os;
    }

} // namespace castlecrawl::item
