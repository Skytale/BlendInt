/*
 * This file is part of BIL (Blender Interface Library).
 *
 * BIL (Blender Interface Library) is free software: you can
 * redistribute it and/or modify it under the terms of the GNU Lesser
 * General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BIL (Blender Interface Library) is distributed in the hope that it
 * will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BIL.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#ifndef _BIL_ABSTRACTSLIDER_HPP_
#define _BIL_ABSTRACTSLIDER_HPP_

#include <BIL/Widget.hpp>
#include <BIL/Types.hpp>

namespace BIL {

	/**
	 * @brief The abstract class for slider widgets
	 */
	class AbstractSlider: public Widget
	{

	public:

		AbstractSlider(Orientation orientation, Drawable* parent = 0);

		virtual ~AbstractSlider ();

		void set_range (int value1, int value2);

		void set_minimum (int minimum);
		
		int minimum () const {return m_minimum;}

		void set_maximum (int maximum);

		int maximum () const {return m_maximum;}

		void set_value (int value);

		int value () const {return m_value;}
		
		void set_step (int step);

		int step () const {return m_step;}

		void set_orientation (Orientation orientation);

		Orientation orientation () const {return m_orientation;}

	protected:

		virtual void render () = 0;

	private:

		int m_step;

		int m_value;

		int m_minimum;
		int m_maximum;

		Orientation m_orientation;

	};

}

#endif	// _BIL_ABSTRACTSLIDER_HPP_
