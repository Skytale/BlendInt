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

#include <BlendInt/AbstractPrimitive.hpp>

namespace BlendInt {

	AbstractPrimitive::AbstractPrimitive ()
	: Object(), m_vertex_buffer(0), m_index_buffer(0), m_program(0)
	{
	}

	void AbstractPrimitive::SetProgram (GLSLProgram* program)
	{
		if(!program) return;
		if(m_program == program) return;

		if(m_program)
			Destroy(m_program);

		m_program = program;
		Retain(m_program);
	}

	void AbstractPrimitive::SetVertexBuffer (GLArrayBuffer* vb)
	{
		if(!vb) return;
		if(m_vertex_buffer == vb) return;

		if(m_vertex_buffer) Destroy(m_vertex_buffer);
		m_vertex_buffer = vb;
		Retain(m_vertex_buffer);
	}

	void AbstractPrimitive::SetIndexBuffer (GLElementArrayBuffer* ib)
	{
		if(!ib) return;
		if(m_index_buffer == ib) return;

		if(m_index_buffer) Destroy(m_index_buffer);
		m_index_buffer = ib;
		Retain(m_index_buffer);
	}

	AbstractPrimitive::~AbstractPrimitive ()
	{
		Destroy(m_vertex_buffer);
		Destroy(m_index_buffer);
		Destroy(m_program);
	}

}

