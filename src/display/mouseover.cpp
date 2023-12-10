// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// mouseover.cpp
//
#include "display/mouseover.hpp"

#include "map/map.hpp"
#include "map/player-display.hpp"
#include "misc/context.hpp"
#include "display/font.hpp"
#include "display/layout.hpp"
#include "display/sdl-manager.hpp"
#include "misc/sdl-util.hpp"

#include <SFML/Graphics/Text.hpp>

namespace castlecrawl
{

    Mouseover::Mouseover()
        : m_elapsedTimeSec(0.0f)
        , m_willDisplay(false)
        , m_mousePos{ 0, 0 }
        , m_textTexturePtr(nullptr)
        , m_textScreenPos{ 0, 0 }
    {}

    void Mouseover::setup(const Context &) {}

    void Mouseover::teardown() { util::destroyTexture(m_textTexturePtr); }

    void Mouseover::reset()
    {
        m_elapsedTimeSec = 0.0f;
        m_willDisplay = false;
    }

    void Mouseover::update(const Context & context, const float frameTimeSec)
    {
        m_elapsedTimeSec += frameTimeSec;
        if (m_elapsedTimeSec >= 2.0f)
        {
            m_elapsedTimeSec = 0.0f;

            if (m_willDisplay == false)
            {
                m_willDisplay = true;
                setupDisplay(context);
            }
        }
    }

    void Mouseover::handleMouseMovedEvent(const SDL_Point & newPos)
    {
        reset();
        m_mousePos = newPos;
    }

    void Mouseover::draw(const Context & context) const
    {
        if (!m_willDisplay)
        {
            return;
        }

        if (m_textTexturePtr != nullptr)
        {
            context.sdl.blit(m_textTexturePtr, m_textScreenPos.x, m_textScreenPos.y);
        }
    }

    void Mouseover::setupDisplay(const Context & context)
    {
        const MapPos_t mapPos = context.map.screenPosToMapPos(context, m_mousePos);
        const MapCell mapCell = context.map.cell(mapPos);

        std::string message;

        if (context.player.position() == mapPos)
        {
            message = "you";
        }
        else if ((mapCell.object_char != ' ') && (mapCell.object_char != '.'))
        {
            if (mapCell.object_char == 'l')
            {
                message = "lava";
            }
            else if (mapCell.object_char == 'w')
            {
                message = "water";
            }
            else if (mapCell.object_char == 'd')
            {
                message = "unlocked door";
            }
            else if (mapCell.object_char == 'D')
            {
                message = "locked door";
            }
            else if (mapCell.object_char == 'r')
            {
                message = "solid rock";
            }
            else if (mapCell.object_char == 's')
            {
                message = "stairs leading down";
            }
            else if (mapCell.object_char == 'S')
            {
                message = "stairs leading up";
            }
            else if (mapCell.object_char == 'b')
            {
                message = "barrel";
            }
            else if (mapCell.object_char == 'c')
            {
                message = "chest";
            }
            else if (mapCell.object_char == 'k')
            {
                message = "coffin";
            }
            else if (mapCell.object_char == '1')
            {
                message = "bag of snakes";
            }
            else if (mapCell.object_char == '3')
            {
                message = "spiderweb";
            }
            else if (mapCell.object_char == '5')
            {
                message = "barrel of goblins";
            }
            else if (mapCell.object_char == '7')
            {
                message = "bats";
            }
            else if (mapCell.object_char == '9')
            {
                message = "skeleton grave";
            }
            else if (mapCell.object_char == ';')
            {
                message = "demon doorway";
            }
            else if (mapCell.object_char == ']')
            {
                message = "dragon inferno";
            }
        }

        if (message.empty())
        {
            m_willDisplay = false;
            return;
        }

        const FontSize fontSize = FontSize::Small;
        const FontExtent fontExtent = context.font.extent(fontSize);

        util::destroyTexture(m_textTexturePtr);
        m_textTexturePtr = context.font.makeTexture(context, fontSize, message);

        m_textScreenPos.x = (m_mousePos.x + fontExtent.letter_size.x);
        m_textScreenPos.y = (m_mousePos.y - fontExtent.letter_size.y);
    }

} // namespace castlecrawl
