#ifndef STATEEDITOR_HPP_INCLUDED
#define STATEEDITOR_HPP_INCLUDED
//
// state-editor.hpp
//
#include "map-types.hpp"
#include "state.hpp"

#include <string>

namespace castlecrawl
{
    struct Context;

    struct MapEntry_t
    {
        MapPos_t pos{ 0, 0 };
        SDL_Rect rect{ 0, 0, 0, 0 };
    };

    class StateEditor : public StateBase
    {
      public:
        StateEditor();
        virtual ~StateEditor() final {}

        virtual State which() const final { return State::Editor; }
        virtual void onEnter(const Context & context) final;
        virtual void onExit(const Context & context) final;
        virtual void update(const Context & context, const float frameTimeSec) final;
        virtual void draw(const Context & context) const final;
        virtual void handleEvent(const Context & context, const SDL_Event & event) final;

      private:
        void resetMap(const Context & context);
        void placeEditCursor(const Context & context);
        void setMapChar(const MapPos_t & pos, const char ch);
        void fadeText(const Context & context, const std::string & text);

        void editMap(
            const Context & context, const bool isShift, const char upper, const char lower);

        void save() const;
        const std::string mapCharToName(const char ch) noexcept;
        void updateDragRect();
        void updateDragSelectedMapCells(const Context & context);

      private:
        MapChars_t m_mapChars;
        MapPos_t m_editPos;
        Floor m_floor;
        SDL_Rect m_editRect;
        SDL_Texture * m_keyTexturePtr;
        SDL_Point m_keyScreenPos;
        SDL_Texture * m_fadeTexturePtr;
        SDL_Point m_fadeScreenPos;
        SDL_Texture * m_titleTexturePtr;
        SDL_Point m_titleScreenPos;
        Uint8 m_fadeTextAlpha;
        bool m_isDragging;
        SDL_Point m_dragPosStart;
        SDL_Point m_dragPosStop;
        SDL_Rect m_dragRect;
        std::vector<MapEntry_t> m_dragSelectedEntrys;
    };

} // namespace castlecrawl

#endif // STATEEDITOR_HPP_INCLUDED
