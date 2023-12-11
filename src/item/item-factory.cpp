// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// item-factory.cpp
//
#include "item/item-factory.hpp"

#include "misc/check-macros.hpp"
#include "misc/context.hpp"
#include "misc/random.hpp"
#include "player/player.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <set>
#include <vector>

namespace castlecrawl::item
{

    const std::string Treasure::description() const
    {
        std::string str;
        str.reserve(200);

        str += "You find";

        if ((0 == gold) && items.empty())
        {
            str += " nothing.";
            return str;
        }

        if (gold > 0)
        {
            str += ' ';
            str += std::to_string(gold);
            str += " gold";
        }

        if (items.empty())
        {
            str += '.';
        }
        else
        {
            if (gold > 0)
            {
                str += " and";
            }

            str += ":\n";

            for (const Item & item : items)
            {
                str += "    ";
                str += item.name();
                str += '\n';
            }
        }

        return str;
    }

    ItemFactory::ItemFactory()
        : m_lowestValue(0)
        , m_textExtent()
    {}

    const ItemVec_t ItemFactory::makeAll() const
    {
        ItemVec_t items;
        items.reserve(1000); // acutally 883 as of 2022-3-13

        for (const Item & item : makeWeapons())
        {
            items.push_back(item);
        }

        for (const Item & item : makeArmor())
        {
            items.push_back(item);
        }

        for (const Item & item : makeMisc())
        {
            items.push_back(item);
        }

        for (const Item & item : makeCustom())
        {
            items.push_back(item);
        }

        // not required, only for speed
        std::sort(std::begin(items), std::end(items), [](const Item & A, const Item & B) {
            return (A.value() < B.value());
        });

        return items;
    }

    const ItemVec_t ItemFactory::makeWeapons() const
    {
        ItemVec_t items;

        items.reserve(
            static_cast<std::size_t>(Weapon::Count) *
            static_cast<std::size_t>(WeaponMaterial::Count));

        for (int i = 0; i < static_cast<int>(Weapon::Count); ++i)
        {
            const auto type = static_cast<Weapon>(i);
            for (int m = 0; m < static_cast<int>(WeaponMaterial::Count); ++m)
            {
                const auto material = static_cast<WeaponMaterial>(m);
                items.emplace_back(type, material);
            }
        }

        return items;
    }

    const ItemVec_t ItemFactory::makeArmor() const
    {
        ItemVec_t items;

        items.reserve(
            static_cast<std::size_t>(Armor::Count) *
            static_cast<std::size_t>(ArmorMaterial::Count));

        for (int i = 0; i < static_cast<int>(Armor::Count); ++i)
        {
            const auto type = static_cast<Armor>(i);
            for (int m = 0; m < static_cast<int>(ArmorMaterial::Count); ++m)
            {
                const auto material = static_cast<ArmorMaterial>(m);
                items.emplace_back(type, material);
            }
        }

        return items;
    }

    const ItemVec_t ItemFactory::makeMisc() const
    {
        ItemVec_t items;

        items.reserve(
            (static_cast<std::size_t>(Misc::Count) *
             static_cast<std::size_t>(MiscMaterial::Count)) +
            9);

        // these are the equipable misc items
        for (int i = 0; i < static_cast<int>(Misc::Count); ++i)
        {
            const auto type = static_cast<Misc>(i);
            if (requiredMiscMaterial(type) != MiscMaterial::Count)
            {
                continue;
            }

            for (int m = 0; m < static_cast<int>(MiscMaterial::Magic); ++m)
            {
                const auto material = static_cast<MiscMaterial>(m);

                items.emplace_back(
                    type,
                    material,
                    UseStrength::Normal,
                    UseEffect{},
                    miscMaterialEquipEffect(material));
            }
        }

        // these are the useable misc items
        // clang-format off
        items.push_back(Item(Misc::Potion, MiscMaterial::Magic, UseStrength::Weak,   {.health=8}, {}));
        items.push_back(Item(Misc::Potion, MiscMaterial::Magic, UseStrength::Normal, {.health=16}, {}));
        items.push_back(Item(Misc::Potion, MiscMaterial::Magic, UseStrength::Strong, {.health=32}, {}));
        //
        items.push_back(Item(Misc::Potion, MiscMaterial::Magic, UseStrength::Weak,   {.mana=5}, {}));
        items.push_back(Item(Misc::Potion, MiscMaterial::Magic, UseStrength::Normal, {.mana=10}, {}));
        items.push_back(Item(Misc::Potion, MiscMaterial::Magic, UseStrength::Strong, {.mana=20}, {}));
        //
        items.push_back(Item(Misc::Herbs, MiscMaterial::Magic, UseStrength::Weak,   {.health=5}, {}));
        items.push_back(Item(Misc::Herbs, MiscMaterial::Magic, UseStrength::Normal, {.health=10}, {}));
        items.push_back(Item(Misc::Herbs, MiscMaterial::Magic, UseStrength::Strong, {.health=20}, {}));
        // clang-format on

        return items;
    }

    const ItemVec_t ItemFactory::makeCustom() const
    {
        // All items, custom magical or not, must have a unique name!

        ItemVec_t items;
        items.reserve(200); // actually 110 as of 2022-3-13

        // weapons

        for (int i = 0; i < static_cast<int>(Weapon::Count); ++i)
        {
            const auto type = static_cast<Weapon>(i);

            items.emplace_back(
                type,
                WeaponMaterial::Obsidian,
                std::string(toString(type)).append(" of Night"),
                EquipEffect{ .arc = 3, .dmg = 3 });
        }

        for (int i = 0; i < static_cast<int>(Weapon::Count); ++i)
        {
            const auto type = static_cast<Weapon>(i);

            items.emplace_back(
                type,
                WeaponMaterial::Obsidian,
                std::string("Dark ").append(toString(type)),
                EquipEffect{ .arc = 1, .dmg = 1, .str = 1 });
        }

        for (int i = 0; i < static_cast<int>(Weapon::Count); ++i)
        {
            const auto type = static_cast<Weapon>(i);

            items.emplace_back(
                type,
                WeaponMaterial::Steel,
                std::string("Savage ").append(toString(type)),
                EquipEffect{ .acc = 1, .dmg = 4, .str = 2 });
        }

        items.push_back(Item(
            Weapon::Dagger, WeaponMaterial::Steel, "Backstabber Dagger", { .acc = 2, .dmg = 7 }));

        items.push_back(Item(
            Weapon::Shortsword, WeaponMaterial::Gold, "Lucky Shortsword", { .acc = 4, .lck = 10 }));

        items.push_back(Item(Weapon::Dagger, WeaponMaterial::Bronze, "Hobo Dagger", { .lck = 2 }));

        items.push_back(Item(
            Weapon::Scythe,
            WeaponMaterial::Gold,
            "Sythe of the Lich",
            { .acc = 3, .arc = 7, .dmg = 7 }));

        items.push_back(
            Item(Weapon::Handaxe, WeaponMaterial::Steel, "Maniac Handaxe", { .dmg = 5 }));

        items.push_back(Item(Weapon::Mace, WeaponMaterial::Steel, "Brute Mace", { .dmg = 4 }));

        items.push_back(
            Item(Weapon::Warhammer, WeaponMaterial::Silver, "Requiem Warhammer", { .dmg = 6 }));

        items.push_back(Item(
            Weapon::Longsword,
            WeaponMaterial::Silver,
            "Knightly Longsword",
            { .acc = 3, .dmg = 6 }));

        items.push_back(
            Item(Weapon::Waraxe, WeaponMaterial::Bronze, "Orcish Waraxe", { .dmg = 3 }));

        items.push_back(Item(
            Weapon::Claymore, WeaponMaterial::Obsidian, "Macabre Waraxe", { .arc = 2, .dmg = 4 }));

        items.push_back(Item(
            Weapon::Shortsword,
            WeaponMaterial::Steel,
            "Scoundrel Shortsword",
            { .dmg = 6, .lck = 3 }));

        items.push_back(
            Item(Weapon::Waraxe, WeaponMaterial::Steel, "Wicked Waraxe", { .dmg = 5, .lck = 1 }));

        items.push_back(
            Item(Weapon::Dagger, WeaponMaterial::Obsidian, "Diabolic Dagger", { .dmg = 10 }));

        items.push_back(Item(
            Weapon::Longsword,
            WeaponMaterial::Silver,
            "Dancing Longsword",
            { .acc = 4, .dmg = 8 }));

        items.push_back(Item(
            Weapon::Longsword, WeaponMaterial::Steel, "Samurai Longsword", { .acc = 2, .dmg = 2 }));

        items.push_back(
            Item(Weapon::Claymore, WeaponMaterial::Obsidian, "Gloom Claymore", { .dmg = 4 }));

        items.push_back(
            Item(Weapon::Dagger, WeaponMaterial::Silver, "Pirate Dagger", { .dmg = 2, .lck = 2 }));

        items.push_back(Item(
            Weapon::Longsword, WeaponMaterial::Silver, "Pirate Longsword", { .dmg = 2, .lck = 2 }));

        items.push_back(
            Item(Weapon::Dagger, WeaponMaterial::Steel, "Bone Dagger", { .arc = 1, .dmg = 2 }));

        items.push_back(Item(
            Weapon::Handaxe, WeaponMaterial::Bronze, "Villan's Handaxe", { .dmg = 1, .str = 1 }));

        // armor

        for (int i = 0; i < static_cast<int>(Armor::Count); ++i)
        {
            const auto type = static_cast<Armor>(i);

            items.emplace_back(
                type,
                ArmorMaterial::DragonScale,
                std::string("Dragon Slayer ").append(toString(type)),
                EquipEffect{ .acc = 3, .dmg = 3, .str = 3 });
        }

        for (int i = 0; i < static_cast<int>(Armor::Count); ++i)
        {
            const auto type = static_cast<Armor>(i);

            items.emplace_back(
                type,
                ArmorMaterial::Silver,
                std::string(toString(type)).append(" of Nobility"),
                EquipEffect{ .acc = 1, .lck = 2, .str = 3 });
        }

        for (int i = 0; i < static_cast<int>(Armor::Count); ++i)
        {
            const auto type = static_cast<Armor>(i);

            items.emplace_back(
                type,
                ArmorMaterial::Steel,
                std::string("Gladiator ").append(toString(type)),
                EquipEffect{ .acc = 1, .str = 1 });
        }

        for (int i = 0; i < static_cast<int>(Armor::Count); ++i)
        {
            const auto type = static_cast<Armor>(i);

            items.emplace_back(
                type,
                ArmorMaterial::Gold,
                std::string("Kingly ").append(toString(type)),
                EquipEffect{ .acc = 3, .str = 3 });
        }

        for (int i = 0; i < static_cast<int>(Armor::Count); ++i)
        {
            const auto type = static_cast<Armor>(i);

            items.emplace_back(
                type,
                ArmorMaterial::Leather,
                std::string("Druid ").append(toString(type)),
                EquipEffect{ .arc = 2, .lck = 2 });
        }

        for (int i = 0; i < static_cast<int>(Armor::Count); ++i)
        {
            const auto type = static_cast<Armor>(i);

            items.emplace_back(
                type,
                ArmorMaterial::Leather,
                std::string("Ranger ").append(toString(type)),
                EquipEffect{ .acc = 2, .lck = 1, .str = 3 });
        }

        items.push_back(Item(Armor::Boots, ArmorMaterial::Leather, "Nimble Boots", { .dex = 5 }));

        items.push_back(
            Item(Armor::Bracers, ArmorMaterial::Leather, "Swift Bracers", { .dex = 7 }));

        items.push_back(
            Item(Armor::Gauntlets, ArmorMaterial::Leather, "Mongoose Gauntlets", { .dex = 9 }));

        items.push_back(
            Item(Armor::Gauntlets, ArmorMaterial::Gold, "Glory Gauntlets", { .acc = 3, .str = 5 }));

        items.push_back(
            Item(Armor::Cuirass, ArmorMaterial::Silver, "Cuirass of Last Rites", { .arc = 7 }));

        items.push_back(
            Item(Armor::Greaves, ArmorMaterial::Leather, "Burglar Greaves", { .lck = 7 }));

        items.push_back(
            Item(Armor::Boots, ArmorMaterial::Leather, "Sorcerer Boots", { .arc = 5, .lck = 1 }));

        items.push_back(Item(
            Armor::Cuirass, ArmorMaterial::Silver, "Sorcerer Cuirass", { .arc = 5, .lck = 1 }));

        items.push_back(Item(
            Armor::Greaves, ArmorMaterial::Silver, "Sorcerer Greaves", { .arc = 5, .lck = 1 }));

        items.push_back(Item(Armor::Boots, ArmorMaterial::Leather, "Shaman Boots", { .arc = 2 }));

        items.push_back(
            Item(Armor::Cuirass, ArmorMaterial::Leather, "Shaman Cuirass", { .arc = 2 }));

        items.push_back(
            Item(Armor::Greaves, ArmorMaterial::Leather, "Shaman Greaves", { .arc = 2 }));

        items.push_back(Item(Armor::Helm, ArmorMaterial::Leather, "Monster Helm", { .str = 8 }));

        items.push_back(Item(Armor::Boots, ArmorMaterial::Leather, "Bearskin Boots", { .str = 1 }));

        items.push_back(
            Item(Armor::Bracers, ArmorMaterial::Leather, "Bearskin Bracers", { .str = 1 }));

        items.push_back(
            Item(Armor::Cuirass, ArmorMaterial::Leather, "Bearskin Cuirass", { .str = 1 }));

        items.push_back(
            Item(Armor::Greaves, ArmorMaterial::Leather, "Bearskin Greaves", { .str = 1 }));

        items.push_back(
            Item(Armor::Boots, ArmorMaterial::Leather, "Beastcall Boots", { .arc = 1, .str = 3 }));

        items.push_back(Item(
            Armor::Bracers, ArmorMaterial::Leather, "Beastcall Bracers", { .arc = 1, .str = 3 }));

        items.push_back(Item(
            Armor::Cuirass, ArmorMaterial::Leather, "Beastcall Cuirass", { .arc = 1, .str = 3 }));

        items.push_back(Item(
            Armor::Greaves, ArmorMaterial::Leather, "Beastcall Greaves", { .arc = 1, .str = 3 }));

        return items;
    }

    void ItemFactory::validateAll(const ItemVec_t & items) const
    {
        // loook for duplicate names
        std::set<std::string> names;
        for (const Item & item : items)
        {
            const std::string name = item.name();

            const auto iter = names.find(name);
            if (iter != std::end(names))
            {
                std::cout << "Error: Two items had the same name: " << item << '\n';
            }
            else
            {
                names.insert(name);
            }
        }

        // throw if any item is invalid
        for (const Item & item : items)
        {
            throwIfInvalid(item);
        }
    }

    const TextExtent ItemFactory::findTextExtents(const ItemVec_t & items) const
    {
        TextExtent extents;

        for (const Item & item : items)
        {
            const std::string name = item.name();

            if (name.size() > extents.longest_name)
            {
                extents.longest_name = name.size();
            }

            const std::string desc = item.description();
            if (desc.size() > extents.longest_desc)
            {
                extents.longest_desc = desc.size();
            }
        }

        return extents;
    }

    void ItemFactory::processAll()
    {
        const ItemVec_t items{ makeAll() };

        validateAll(items);

        m_textExtent = findTextExtents(items);

        m_lowestValue = std::begin(items)->value();
    }

    void ItemFactory::printSummaries() const
    {
        const ItemVec_t items{ makeAll() };

        // write out all items to a spreadsheet
        {
            std::ofstream fileStream("items.csv", std::ios_base::trunc);

            for (const Item & item : items)
            {
                if (item.isArmor())
                {
                    fileStream << "armor,";
                }
                else if (item.isWeapon())
                {
                    fileStream << "weapon,";
                }
                else
                {
                    fileStream << "misc,";
                }

                fileStream << std::to_string(item.value()) << ',';

                if (item.isMagical())
                {
                    fileStream << "magic,";
                }
                else
                {
                    fileStream << "non-magic,";
                }

                fileStream << item.name() << ',';
                fileStream << item.description() << "\n";
            }
        }

        std::cout << std::endl << "All Useable Names:" << std::endl;

        for (const Item & item : items)
        {
            if (item.isUseable())
            {
                std::cout << '\t' << item.value() << "\t" << item.name() << '\n';
            }
        }

        std::cout << std::endl << "All Non-Magical:" << std::endl;

        for (const Item & item : items)
        {
            if (!item.isMagical())
            {
                std::cout << '\t' << item.value() << "\t" << item << '\n';
            }
        }

        std::cout << std::endl << "All Magical Weapons and Armor:" << std::endl;

        for (const Item & item : items)
        {
            if (!item.isMagical())
            {
                continue;
            }

            if (item.isWeapon() || item.isArmor())
            {
                std::cout << '\t' << item.value() << '\t' << item.description() << '\n';
            }
        }

        std::cout << std::endl;

        std::cout << items.size() << " total items" << std::endl << std::endl;

        std::cout << "longest name=" << m_textExtent.longest_name << std::endl;
        std::cout << "longest desc=" << m_textExtent.longest_desc << std::endl;

        std::cout << "An Item is " << sizeof(Item) << "bytes" << std::endl;
        std::cout << std::endl;
    }

    void ItemFactory::throwIfInvalid(const Item & item) const
    {
        const std::size_t typeCount = [&]() {
            std::size_t count{ 0 };
            if (item.isArmor())
            {
                ++count;
            }

            if (item.isWeapon())
            {
                ++count;
            }

            if (item.isMisc())
            {
                ++count;
            }

            return count;
        }();

        M_CHECK(
            (typeCount == 1),
            "Item's basic type of Weapon/Armor/Misc count is "
                << typeCount << " (not essential and mutually exclusive): " << item);

        M_CHECK(!item.name().empty(), "Item has no name: " << item);

        if (item.isUseable())
        {
            M_CHECK(!item.isEquipable(), "Item is Useable AND Equipable: " << item);
        }

        if (item.isEquipable())
        {
            M_CHECK(!item.isUseable(), "Item is Equipable AND Useable: " << item);
        }

        if (item.isArmor())
        {
            M_CHECK((item.armorRating() >= 1_armor), "Armor Item's amor_rating invalid: " << item);

            M_CHECK(
                (item.armorMaterial() != ArmorMaterial::Count),
                "Armor Item has no material: " << item);

            M_CHECK(item.isEquipable(), "Armor Item SHOULD be Equipable but is not: " << item);

            if (item.isMagical())
            {
                M_CHECK(
                    (item.name() != toString(item.armorType())),
                    "Magical armor has a default name:" << item);

                M_CHECK(
                    (item.equipEffect().total() > 0), "Magical armor has no equip effect:" << item);
            }
        }

        if (item.isWeapon())
        {
            M_CHECK((item.damageMin() > 0), "Weapon Item's damage_min invalid: " << item);

            M_CHECK(
                (item.damageMax() >= item.damageMin()),
                "Weapon Item's damage_max is not >= than the min: " << item);

            M_CHECK(
                (item.weaponMaterial() != WeaponMaterial::Count),
                "Weapon Item has no material: " << item);

            M_CHECK(item.isEquipable(), "Weapon Item SHOULD be Equipable but is not: " << item);

            if (item.isMagical())
            {
                M_CHECK(
                    (item.name() != toString(item.weaponType())),
                    "Magical weapon has a default name:" << item);

                M_CHECK(
                    (item.equipEffect().total() > 0),
                    "Magical weapon has no equip effect:" << item);
            }
        }

        if (item.isMisc())
        {
            if (item.isUseable())
            {
                M_CHECK(
                    (item.useEffect().total() > 0), "Useable item has no use effects: " << item);

                M_CHECK(
                    (item.miscMaterial() == MiscMaterial::Magic),
                    "Misc item Potions and Herbs MUST have the material 'Magic': " << item);
            }
            else
            {
                M_CHECK(
                    (item.miscMaterial() != MiscMaterial::Magic),
                    "This Misc item must NOT have the material 'Magic': " << item);
            }
        }

        M_CHECK((item.value() > 0), "Item's Value is zero or less: " << item);
    }

    const Treasure ItemFactory::randomTreasureFind(Context & context) const
    {
        Treasure treasure;

        // establish how much value this random find is worth
        const int valuePerLevel{ 100 };
        int value = context.player.level() * valuePerLevel;
        value += context.random.fromTo(0, valuePerLevel);
        value = context.random.fromTo(0, value);

        // determine how much will be gold
        const int valueOfGold = context.random.fromTo(0, value);
        treasure.gold = (valueOfGold / 5);
        value -= valueOfGold;

        // use remaining value to add items
        while (value >= m_lowestValue)
        {
            ItemVec_t items = makeAll();

            // remove items worth too much
            items.erase(
                std::remove_if(
                    std::begin(items),
                    std::end(items),
                    [&](const Item & item) { return (item.value() > value); }),
                std::end(items));

            // remove items already found
            for (const Item & alreadyFoundItem : treasure.items)
            {
                items.erase(
                    std::remove_if(
                        std::begin(items),
                        std::end(items),
                        [&](const Item & item) {
                            return (item.name() == alreadyFoundItem.name());
                        }),
                    std::end(items));
            }

            if (items.empty())
            {
                break;
            }

            const Item & item = treasure.items.emplace_back(context.random.from(items));

            value -= item.value();
        }

        return treasure;
    }

    const Treasure ItemFactory::randomHerbFind(Context & context) const
    {
        Treasure treasure;

        ItemVec_t miscItems{ makeMisc() };

        miscItems.erase(
            std::remove_if(
                std::begin(miscItems),
                std::end(miscItems),
                [](const Item & item) { return (item.miscType() != Misc::Herbs); }),
            std::end(miscItems));

        treasure.items.push_back(context.random.from(miscItems));

        return treasure;
    }

} // namespace castlecrawl::item
