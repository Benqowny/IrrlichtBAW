// Copyright (C) 2018 Mateusz "DevSH" Kielan
// This file is part of the "BAW Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __C_OPEN_GL_MULTISAMPLE_TEXTURE_ARRAY_H_INCLUDED__
#define __C_OPEN_GL_MULTISAMPLE_TEXTURE_ARRAY_H_INCLUDED__

#include "IrrCompileConfig.h"
#include "IMultisampleTexture.h"
#include "COpenGLTexture.h"
#include "COpenGLExtensionHandler.h"

#ifdef _IRR_COMPILE_WITH_OPENGL_


namespace irr
{
namespace video
{

class COpenGLMultisampleTextureArray : public COpenGLTexture, public IMultisampleTexture
{
    protected:

    public:
        //! constructor
        COpenGLMultisampleTextureArray(GLenum internalFormat, const uint32_t& samples, const uint32_t* size, const bool& fixedSampleLocations);


        //! returns the opengl texture type
        virtual GLenum getOpenGLTextureType() const {return GL_TEXTURE_2D_MULTISAMPLE_ARRAY;}


        //!
        virtual const E_VIRTUAL_TEXTURE_TYPE getVirtualTextureType() const {return EVTT_2D_MULTISAMPLE;}

        //!
        core::dimension2du getRenderableSize() const {return core::dimension2du(TextureSize[0],TextureSize[1]);}


        //! returns driver type of texture (=the driver, that created it)
        virtual E_DRIVER_TYPE getDriverType() const {return EDT_OPENGL;}

        //!
        virtual const uint32_t* getSize() const {return TextureSize;}

        //! Returns size of the texture.
        virtual const E_DIMENSION_COUNT getDimensionality() const {return EDC_THREE;}

        //! returns color format of texture
        virtual ECOLOR_FORMAT getColorFormat() const {return ColorFormat;}

        //!
        virtual const E_MULTISAMPLE_TEXTURE_TYPE getTextureType() const {return EMTT_2D_ARRAY;}

        //! sampleCount of 0 indicates not 0 samples but the same amount as old texture
        virtual bool resize(const uint32_t* size, const uint32_t& sampleCount=0);

        //!
        virtual bool resize(const uint32_t* size, const uint32_t& sampleCount, const bool& fixedSampleLocations);

        //!
        virtual uint32_t getSampleCount() const {return SampleCount;}

        //!
        virtual bool usesFixedSampleLocations() const {return FixedSampleLocations;}

    protected:
        uint32_t TextureSize[3];
        uint32_t SampleCount;
        bool FixedSampleLocations;

        GLint InternalFormat;
        ECOLOR_FORMAT ColorFormat;
};

} // end namespace video
} // end namespace irr

#endif // _IRR_COMPILE_WITH_OPENGL_
#endif


