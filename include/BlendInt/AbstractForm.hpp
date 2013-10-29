/*
 * This file is part of BlendInt (a Blender-like Interface Library in
 * OpenGL).
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is free
 * software: you can redistribute it and/or modify it under the terms
 * of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is
 * distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General
 * Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BlendInt.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#ifndef _BLENDINT_ABSTRACTFORM_HPP_
#define _BLENDINT_ABSTRACTFORM_HPP_

#include <list>
#include <set>
#include <string>

#include <BlendInt/EventHandler.hpp>

#include <BlendInt/Point.hpp>
#include <BlendInt/Size.hpp>
#include <BlendInt/Padding.hpp>
#include <BlendInt/Margin.hpp>

#include <BlendInt/Types.hpp>

#include <Cpp/Events.hpp>

#ifdef DEBUG
#include <map>
using std::map;
#endif

namespace BlendInt {

	class ContextManager;
	class AbstractForm;
	class Rect;

	struct WidgetTheme;

	enum ParentType {
		ParentUnknown,
		ParentContextManager,
		ParentForm
	};

	enum BasicPropertyType {
		BasicPropertyUnknown = 0,
		BasicPropertyLayer,
		BasicPropertySize,
		BasicPropertyMinimalSize,
		BasicPropertyRoundCorner,
		BasicPropertyVisibility,
		BasicPropertyExpandX,
		BasicPropertyExpandY,
		BasicPropertyPosition,
		BasicPropertyLast = BasicPropertyPosition
	};

	union ParentPointer {
		ParentPointer ()
		: nameless(0)
		{

		}
		ContextManager* context;
		AbstractForm* form;
		void* nameless;
	};

	struct Parent {
		Parent ()
		: type(ParentUnknown)
		{

		}
		ParentType type;
		ParentPointer object;

		DISALLOW_COPY_AND_ASSIGN(Parent);
	};

	class AbstractForm: public EventHandler
	{
		DISALLOW_COPY_AND_ASSIGN(AbstractForm);

	public:

		friend class ContextManager;

		AbstractForm ();

		/**
		 * @brief Default constructor
		 * @param parent
		 *
		 * A drawable instance without a parent will be registered in
		 * a static list -- solo, it's usually a pop-up widget such as
		 * context menu, message box
		 */
		AbstractForm (AbstractForm* parent);

		virtual ~AbstractForm ();

		bool bind (AbstractForm* child);

		bool unbind (AbstractForm* child);

		/**
		 * @brief unbind this and set parent to 0
		 */
		void unbind ();

		bool bind_to (ContextManager* parent);

		bool bind_to (AbstractForm* parent);

		/**
		 * @brief if the root of this Drawable object is bounded to ContextManager
		 * @return
		 */
		bool is_bound ();

		const Size& size () const;

		void resize (int w, int h);

		void resize (const Size& size);

		const Point& pos () const;

		void set_pos (int x, int y);

		void set_pos (const Point& pos);

		int z () const
		{
			return m_z;
		}

		void reset_z (int z);

		void set_roundcorner (int type);

		int roundcorner () const;

		void set_corner_radius (float radius);

		float corner_radius () const {return m_corner_radius;}

		void set_expand_x (bool expand) {m_expand_x = expand;}

		bool expand_x () const {return m_expand_x;}

		void set_expand_y (bool expand) {m_expand_y = expand;}

		bool expand_y () const {return m_expand_y;}

		void set_expand (bool expand) {m_expand_x = expand; m_expand_y = expand;}

		bool visible () const;

		void set_visible (bool visible);

		void show ();

		void hide ();

		const std::string& name () const;

		void set_name (const std::string& name);

		const Size& preferred_size () const {return m_preferred_size;}

		void set_preferred_size (const Size& size);

		void set_preferred_size (int width, int height);

		const Size& minimal_size () const {return m_minimal_size;}

		void set_minimal_size (const Size& size);

		void set_minimal_size (int width, int height);

		const Parent* parent () const {return &m_parent;}

		bool in_layout () const {return m_in_layout;}

	protected:	// member functions

		bool contain (const Coord2d& cursor);

		/**
		 * @brief Update data for render, used in size, shape change only
		 *
		 * @return true - accept this update, false - discard this update
		 */
		virtual bool update (int type, const void* property) = 0;

		/**
		 * @brief just change m_z simply
		 * @param z
		 */
		void set_z_simple (int z);

		/**
		 * @brief set Drawable object position without checking layout
		 * @param obj
		 * @param x
		 * @param y
		 *
		 * This function should be called in Layout only
		 */
		void set_pos_priv (AbstractForm* obj, int x, int y);

		/**
		 * @brief set Drawable object position without checking layout
		 * @param obj
		 * @param pos
		 *
		 * This function should be called in layout only
		 */
		void set_pos_priv (AbstractForm* obj, const Point& pos);

		void set_in_layout (AbstractForm* obj, bool status) {obj->m_in_layout = status;}

		void resize_priv (AbstractForm* obj, int width, int height);

		void resize_priv (AbstractForm* obj, const Size& size);

	protected:
		// member variables

		/**
		 * @brief the depth(layer) of the widget
		 */
		int m_z;

		int m_roundcorner;

		/**
		 * @brief the round corner radius
		 *
		 * should be 0.0, 1.0, 2.0 etc
		 */
		float m_corner_radius;

		bool m_visible;

		bool m_in_layout;

		bool m_expand_x;

		bool m_expand_y;

		Size m_size;

		Size m_preferred_size;

		Size m_minimal_size;

		Point m_pos;

		std::string m_name;

		Parent m_parent;

		std::set<AbstractForm*> m_children;

		Cpp::ConnectionScope m_events;

#ifdef DEBUG
	public:

		static AbstractForm* find (uint64_t id);

		static unsigned int map_size ()
		{
			return obj_map.size();
		}

		static const map<uint64_t, AbstractForm*>& get_map ()
		{
			return obj_map;
		}

		static void reset ()
		{
			id_last = 1;
			obj_map.clear();
		}

	private:

		inline bool register_in_map ();

		inline bool unregister_from_map ();

		// member variables

		uint64_t m_id; /** A unique ID for object */

		// static member variables
		static uint64_t id_last;

		static map<uint64_t, AbstractForm*> obj_map;

	public:

		static void print ();

#endif

	};

} /* namespace BlendInt */
#endif /* _BIL_DRAWABLE_H_ */