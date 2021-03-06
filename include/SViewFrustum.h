// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __S_VIEW_FRUSTUM_H_INCLUDED__
#define __S_VIEW_FRUSTUM_H_INCLUDED__

#include "plane3d.h"
#include "vectorSIMD.h"
#include "line3d.h"
#include "aabbox3d.h"
#include "matrix4.h"
#include "IVideoDriver.h"

namespace irr
{
namespace scene
{

	//! Defines the view frustum. That's the space visible by the camera.
	/** The view frustum is enclosed by 6 planes. These six planes share
	eight points. A bounding box around these eight points is also stored in
	this structure.
	*/
	struct SViewFrustum
	{
		enum VFPLANES
		{
			//! Far plane of the frustum. That is the plane farest away from the eye.
			VF_FAR_PLANE = 0,
			//! Near plane of the frustum. That is the plane nearest to the eye.
			VF_NEAR_PLANE,
			//! Left plane of the frustum.
			VF_LEFT_PLANE,
			//! Right plane of the frustum.
			VF_RIGHT_PLANE,
			//! Bottom plane of the frustum.
			VF_BOTTOM_PLANE,
			//! Top plane of the frustum.
			VF_TOP_PLANE,

			//! Amount of planes enclosing the view frustum. Should be 6.
			VF_PLANE_COUNT
		};


		//! Default Constructor.
		SViewFrustum() {}

		//! Copy Constructor.
		SViewFrustum(const SViewFrustum& other);

		//! This constructor creates a view frustum based on a projection and/or view matrix.
		/** @param mat Source matrix. */
		SViewFrustum(const core::matrix4& mat);

		//! Modifies frustum as if it was constructed with a matrix.
		/** @param mat Source matrix.
		@see @ref SViewFrustum(const core::matrix4&)
		*/
		inline void setFrom(const core::matrix4& mat);

		//! Transforms the frustum by the matrix
		/** @param mat: Matrix by which the view frustum is transformed.*/
		void transform(const core::matrix4& mat);

		//! @returns the point which is on the far left upper corner inside the the view frustum.
		core::vector3df_SIMD getFarLeftUp() const;

		//! @returns the point which is on the far left bottom corner inside the the view frustum.
		core::vector3df_SIMD getFarLeftDown() const;

		//! @returns the point which is on the far right top corner inside the the view frustum.
		core::vector3df_SIMD getFarRightUp() const;

		//! @returns the point which is on the far right bottom corner inside the the view frustum.
		core::vector3df_SIMD getFarRightDown() const;

		//! @returns the point which is on the near left upper corner inside the the view frustum.
		core::vector3df_SIMD getNearLeftUp() const;

		//! @returns the point which is on the near left bottom corner inside the the view frustum.
		core::vector3df_SIMD getNearLeftDown() const;

		//! @returns the point which is on the near right top corner inside the the view frustum.
		core::vector3df_SIMD getNearRightUp() const;

		//! @returns the point which is on the near right bottom corner inside the the view frustum.
		core::vector3df_SIMD getNearRightDown() const;

		//! @returns a bounding box enclosing the whole view frustum.
		const core::aabbox3d<float> &getBoundingBox() const;

		//! Recalculates the bounding box member based on the planes.
		inline void recalculateBoundingBox();

		//! Clips a line to the view frustum.
		/** @return True if the line was clipped, false if not. */
		bool clipLine(core::line3d<float>& line) const;

		//! Is point lying within the frustum?
		/** @returns Whether point is lying within the frustum. */
		bool cullPoint(const core::vector3d<float>& point) const;

		//! The position of the camera.
		core::vector3df_SIMD cameraPosition;

		//! All planes enclosing the view frustum.
		core::plane3d<float> planes[VF_PLANE_COUNT];

		//! Bounding box around the view frustum.
		core::aabbox3d<float> boundingBox;

	private:
	};


	inline SViewFrustum::SViewFrustum(const SViewFrustum& other)
	{
		cameraPosition=other.cameraPosition;
		boundingBox=other.boundingBox;

		uint32_t i;
		for (i=0; i<VF_PLANE_COUNT; ++i)
			planes[i]=other.planes[i];
	}

	inline SViewFrustum::SViewFrustum(const core::matrix4& mat)
	{
		setFrom ( mat );
	}


	inline void SViewFrustum::transform(const core::matrix4& mat)
	{
		for (uint32_t i=0; i<VF_PLANE_COUNT; ++i)
			mat.transformPlane(planes[i]);

		mat.transformVect(cameraPosition.getAsVector3df());
		recalculateBoundingBox();
	}


	inline core::vector3df_SIMD SViewFrustum::getFarLeftUp() const
	{
		core::vector3df_SIMD p;
		planes[scene::SViewFrustum::VF_FAR_PLANE].getIntersectionWithPlanes(
			planes[scene::SViewFrustum::VF_TOP_PLANE],
			planes[scene::SViewFrustum::VF_LEFT_PLANE], p.getAsVector3df());

		return p;
	}

	inline core::vector3df_SIMD SViewFrustum::getFarLeftDown() const
	{
		core::vector3df_SIMD p;
		planes[scene::SViewFrustum::VF_FAR_PLANE].getIntersectionWithPlanes(
			planes[scene::SViewFrustum::VF_BOTTOM_PLANE],
			planes[scene::SViewFrustum::VF_LEFT_PLANE], p.getAsVector3df());

		return p;
	}

	inline core::vector3df_SIMD SViewFrustum::getFarRightUp() const
	{
		core::vector3df_SIMD p;
		planes[scene::SViewFrustum::VF_FAR_PLANE].getIntersectionWithPlanes(
			planes[scene::SViewFrustum::VF_TOP_PLANE],
			planes[scene::SViewFrustum::VF_RIGHT_PLANE], p.getAsVector3df());

		return p;
	}

	inline core::vector3df_SIMD SViewFrustum::getFarRightDown() const
	{
		core::vector3df_SIMD p;
		planes[scene::SViewFrustum::VF_FAR_PLANE].getIntersectionWithPlanes(
			planes[scene::SViewFrustum::VF_BOTTOM_PLANE],
			planes[scene::SViewFrustum::VF_RIGHT_PLANE], p.getAsVector3df());

		return p;
	}

	inline core::vector3df_SIMD SViewFrustum::getNearLeftUp() const
	{
		core::vector3df_SIMD p;
		planes[scene::SViewFrustum::VF_NEAR_PLANE].getIntersectionWithPlanes(
			planes[scene::SViewFrustum::VF_TOP_PLANE],
			planes[scene::SViewFrustum::VF_LEFT_PLANE], p.getAsVector3df());

		return p;
	}

	inline core::vector3df_SIMD SViewFrustum::getNearLeftDown() const
	{
		core::vector3df_SIMD p;
		planes[scene::SViewFrustum::VF_NEAR_PLANE].getIntersectionWithPlanes(
			planes[scene::SViewFrustum::VF_BOTTOM_PLANE],
			planes[scene::SViewFrustum::VF_LEFT_PLANE], p.getAsVector3df());

		return p;
	}

	inline core::vector3df_SIMD SViewFrustum::getNearRightUp() const
	{
		core::vector3df_SIMD p;
		planes[scene::SViewFrustum::VF_NEAR_PLANE].getIntersectionWithPlanes(
			planes[scene::SViewFrustum::VF_TOP_PLANE],
			planes[scene::SViewFrustum::VF_RIGHT_PLANE], p.getAsVector3df());

		return p;
	}

	inline core::vector3df_SIMD SViewFrustum::getNearRightDown() const
	{
		core::vector3df_SIMD p;
		planes[scene::SViewFrustum::VF_NEAR_PLANE].getIntersectionWithPlanes(
			planes[scene::SViewFrustum::VF_BOTTOM_PLANE],
			planes[scene::SViewFrustum::VF_RIGHT_PLANE], p.getAsVector3df());

		return p;
	}

	inline const core::aabbox3d<float> &SViewFrustum::getBoundingBox() const
	{
		return boundingBox;
	}

	inline void SViewFrustum::recalculateBoundingBox()
	{
		boundingBox.reset ( cameraPosition.getAsVector3df() );

		boundingBox.addInternalPoint(getFarLeftUp().getAsVector3df());
		boundingBox.addInternalPoint(getFarRightUp().getAsVector3df());
		boundingBox.addInternalPoint(getFarLeftDown().getAsVector3df());
		boundingBox.addInternalPoint(getFarRightDown().getAsVector3df());
	}

	//! This constructor creates a view frustum based on a projection
	//! and/or view matrix.
	inline void SViewFrustum::setFrom(const core::matrix4& mat)
	{
		// left clipping plane
		planes[VF_LEFT_PLANE].Normal.X = mat[3 ] + mat[0];
		planes[VF_LEFT_PLANE].Normal.Y = mat[7 ] + mat[4];
		planes[VF_LEFT_PLANE].Normal.Z = mat[11] + mat[8];
		planes[VF_LEFT_PLANE].D =        mat[15] + mat[12];

		// right clipping plane
		planes[VF_RIGHT_PLANE].Normal.X = mat[3 ] - mat[0];
		planes[VF_RIGHT_PLANE].Normal.Y = mat[7 ] - mat[4];
		planes[VF_RIGHT_PLANE].Normal.Z = mat[11] - mat[8];
		planes[VF_RIGHT_PLANE].D =        mat[15] - mat[12];

		// top clipping plane
		planes[VF_TOP_PLANE].Normal.X = mat[3 ] - mat[1];
		planes[VF_TOP_PLANE].Normal.Y = mat[7 ] - mat[5];
		planes[VF_TOP_PLANE].Normal.Z = mat[11] - mat[9];
		planes[VF_TOP_PLANE].D =        mat[15] - mat[13];

		// bottom clipping plane
		planes[VF_BOTTOM_PLANE].Normal.X = mat[3 ] + mat[1];
		planes[VF_BOTTOM_PLANE].Normal.Y = mat[7 ] + mat[5];
		planes[VF_BOTTOM_PLANE].Normal.Z = mat[11] + mat[9];
		planes[VF_BOTTOM_PLANE].D =        mat[15] + mat[13];

		// far clipping plane
		planes[VF_FAR_PLANE].Normal.X = mat[3 ] - mat[2];
		planes[VF_FAR_PLANE].Normal.Y = mat[7 ] - mat[6];
		planes[VF_FAR_PLANE].Normal.Z = mat[11] - mat[10];
		planes[VF_FAR_PLANE].D =        mat[15] - mat[14];

		// near clipping plane
		planes[VF_NEAR_PLANE].Normal.X = mat[2];
		planes[VF_NEAR_PLANE].Normal.Y = mat[6];
		planes[VF_NEAR_PLANE].Normal.Z = mat[10];
		planes[VF_NEAR_PLANE].D =        mat[14];

		// normalize normals
		uint32_t i;
		for ( i=0; i != VF_PLANE_COUNT; ++i)
		{
			const float len = -core::reciprocal_squareroot(
					planes[i].Normal.getLengthSQ());
			planes[i].Normal *= len;
			planes[i].D *= len;
		}

		// make bounding box
		recalculateBoundingBox();
	}

	//! Clips a line to the frustum
	inline bool SViewFrustum::clipLine(core::line3d<float>& line) const
	{
		bool wasClipped = false;
		for (uint32_t i=0; i < VF_PLANE_COUNT; ++i)
		{
			if (planes[i].classifyPointRelation(line.start) == core::ISREL3D_FRONT)
			{
				line.start = line.start.getInterpolated(line.end,
						planes[i].getKnownIntersectionWithLine(line.start, line.end));
				wasClipped = true;
			}
			if (planes[i].classifyPointRelation(line.end) == core::ISREL3D_FRONT)
			{
				line.end = line.start.getInterpolated(line.end,
						planes[i].getKnownIntersectionWithLine(line.start, line.end));
				wasClipped = true;
			}
		}
		return wasClipped;
	}


	inline bool SViewFrustum::cullPoint(const core::vector3d<float>& point) const
	{
		for (uint32_t i = 0; i < VF_PLANE_COUNT; ++i)
		{
			if (planes[i].classifyPointRelation(point) == core::ISREL3D_FRONT)
				return true;
		}
		return false;
	}


} // end namespace scene
} // end namespace irr

#endif

