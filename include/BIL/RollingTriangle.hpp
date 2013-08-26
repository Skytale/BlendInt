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

#ifndef ROLLINGTRIANGLE_H_
#define ROLLINGTRIANGLE_H_

#include <BIL/Drawable.hpp>

namespace BIL {

	/**
	 * This object is for test only
	 */
	class RollingTriangle: public BIL::Drawable
	{
	public:
		RollingTriangle (Traceable *parent = NULL);
		virtual ~RollingTriangle ();

	protected:
		virtual void Render ();

	private:
		RollingTriangle (const RollingTriangle& orig);
		RollingTriangle& operator = (const RollingTriangle& orig);
	};

} /* namespace BIL */
#endif /* ROLLINGTRIANGLE_H_ */