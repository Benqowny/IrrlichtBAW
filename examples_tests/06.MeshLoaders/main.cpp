#define _IRR_STATIC_LIB_
#include <irrlicht.h>
#include "../source/Irrlicht/COpenGLExtensionHandler.h"

#include "../source/Irrlicht/CGeometryCreator.h"
#include "../source/Irrlicht/CBAWMeshWriter.h"

using namespace irr;
using namespace core;


//!Same As Last Example
class MyEventReceiver : public IEventReceiver
{
public:

	MyEventReceiver()
	{
	}

	bool OnEvent(const SEvent& event)
	{
        if (event.EventType == irr::EET_KEY_INPUT_EVENT && !event.KeyInput.PressedDown)
        {
            switch (event.KeyInput.Key)
            {
            case irr::KEY_KEY_Q: // switch wire frame mode
                exit(0);
                return true;
            default:
                break;
            }
        }

		return false;
	}

private:
};

class SimpleCallBack : public video::IShaderConstantSetCallBack
{
    int32_t mvpUniformLocation;
    int32_t cameraDirUniformLocation;
    int32_t texUniformLocation[4];
    video::E_SHADER_CONSTANT_TYPE mvpUniformType;
    video::E_SHADER_CONSTANT_TYPE cameraDirUniformType;
    video::E_SHADER_CONSTANT_TYPE texUniformType[4];
public:
    SimpleCallBack() : cameraDirUniformLocation(-1), cameraDirUniformType(video::ESCT_FLOAT_VEC3) {}

    virtual void PostLink(video::IMaterialRendererServices* services, const video::E_MATERIAL_TYPE& materialType, const core::array<video::SConstantLocationNamePair>& constants)
    {
        for (size_t i=0; i<constants.size(); i++)
        {
            if (constants[i].name=="MVP")
            {
                mvpUniformLocation = constants[i].location;
                mvpUniformType = constants[i].type;
            }
            else if (constants[i].name=="cameraPos")
            {
                cameraDirUniformLocation = constants[i].location;
                cameraDirUniformType = constants[i].type;
            }
        }
    }

    virtual void OnSetConstants(video::IMaterialRendererServices* services, int32_t userData)
    {
        core::vectorSIMDf modelSpaceCamPos;
        modelSpaceCamPos.set(services->getVideoDriver()->getTransform(video::E4X3TS_WORLD_VIEW_INVERSE).getTranslation());
        services->setShaderConstant(&modelSpaceCamPos,cameraDirUniformLocation,cameraDirUniformType,1);
        services->setShaderConstant(services->getVideoDriver()->getTransform(video::EPTS_PROJ_VIEW_WORLD).pointer(),mvpUniformLocation,mvpUniformType,1);
    }

    virtual void OnUnsetMaterial() {}
};


int main()
{
	// create device with full flexibility over creation parameters
	// you can add more parameters if desired, check irr::SIrrlichtCreationParameters
	irr::SIrrlichtCreationParameters params;
	params.Bits = 24; //may have to set to 32bit for some platforms
	params.ZBufferBits = 24; //we'd like 32bit here
	params.DriverType = video::EDT_OPENGL; //! Only Well functioning driver, software renderer left for sake of 2D image drawing
	params.WindowSize = dimension2d<uint32_t>(1280, 720);
	params.Fullscreen = false;
	params.Vsync = true; //! If supported by target platform
	params.Doublebuffer = true;
	params.Stencilbuffer = false; //! This will not even be a choice soon
	IrrlichtDevice* device = createDeviceEx(params);

	if (device == 0)
		return 1; // could not create selected driver.


	video::IVideoDriver* driver = device->getVideoDriver();

    SimpleCallBack* cb = new SimpleCallBack();
    video::E_MATERIAL_TYPE newMaterialType = (video::E_MATERIAL_TYPE)driver->getGPUProgrammingServices()->addHighLevelShaderMaterialFromFiles("../mesh.vert",
                                                        "","","", //! No Geometry or Tessellation Shaders
                                                        "../mesh.frag",
                                                        3,video::EMT_SOLID, //! 3 vertices per primitive (this is tessellation shader relevant only
                                                        cb, //! Our Shader Callback
                                                        0); //! No custom user data
    cb->drop();



	scene::ISceneManager* smgr = device->getSceneManager();
	driver->setTextureCreationFlag(video::ETCF_ALWAYS_32_BIT, true);
	scene::ICameraSceneNode* camera =
		smgr->addCameraSceneNodeFPS(0,100.0f,0.01f);
	camera->setPosition(core::vector3df(-4,0,0));
	camera->setTarget(core::vector3df(0,0,0));
	camera->setNearValue(0.01f);
	camera->setFarValue(100.0f);
    smgr->setActiveCamera(camera);
	device->getCursorControl()->setVisible(false);
	MyEventReceiver receiver;
	device->setEventReceiver(&receiver);

	io::IFileSystem* fs = device->getFileSystem();
	scene::IMeshWriter* writer = smgr->createMeshWriter(irr::scene::EMWT_BAW);

	// from Criss:
	// here i'm testing baw mesh writer and loader
	// (import from .stl/.obj, then export to .baw, then import from .baw :D)
	// Seems to work for those two simple meshes, but need more testing!

	//! Test Loading of Obj
    scene::ICPUMesh* cpumesh = smgr->getMesh("../../media/extrusionLogo_TEST_fixed.stl");
	// export mesh
	io::IWriteFile* file = fs->createAndWriteFile("extrusionLogo_TEST_fixed.baw");
	writer->writeMesh(file, cpumesh, scene::EMWF_WRITE_COMPRESSED);
	file->drop();
	// end export

	// import .baw mesh (test)
	cpumesh = smgr->getMesh("extrusionLogo_TEST_fixed.baw");
	// end import

    if (cpumesh)
    {
        scene::IGPUMesh* gpumesh = driver->createGPUMeshesFromCPU(std::vector<scene::ICPUMesh*>(1,cpumesh))[0];
        smgr->getMeshCache()->removeMesh(cpumesh);
        smgr->addMeshSceneNode(gpumesh)->setMaterialType(newMaterialType);
        gpumesh->drop();
    }

    cpumesh = smgr->getMesh("../../media/cow.obj");
	// export mesh
	file = fs->createAndWriteFile("cow.baw");
	writer->writeMesh(file, cpumesh, scene::EMWF_WRITE_COMPRESSED);
	file->drop();
	writer->drop();
	// end export

	// import .baw mesh (test)
	cpumesh = smgr->getMesh("cow.baw");
	// end import

    if (cpumesh)
    {
        scene::IGPUMesh* gpumesh = driver->createGPUMeshesFromCPU(std::vector<scene::ICPUMesh*>(1,cpumesh))[0];
        smgr->getMeshCache()->removeMesh(cpumesh);
        smgr->addMeshSceneNode(gpumesh,0,-1,core::vector3df(3.f,1.f,0.f))->setMaterialType(newMaterialType);
        gpumesh->drop();
    }


	uint64_t lastFPSTime = 0;

	while(device->run())
	//if (device->isWindowActive())
	{
		driver->beginScene(true, true, video::SColor(255,0,0,255) );

        //! This animates (moves) the camera and sets the transforms
        //! Also draws the meshbuffer
        smgr->drawAll();

        //! Stress test for memleaks aside from demo how to create meshes that live on the GPU RAM
        {/*
            scene::IGPUMeshBuffer* mb = new scene::IGPUMeshBuffer();
            scene::IGPUMeshDataFormatDesc* desc = driver->createGPUMeshDataFormatDesc();
            mb->setMeshDataAndFormat(desc);
            desc->drop();

            uint16_t indices_indexed16[] = {
                0,1,2,1,2,3,
                4,5,6,5,6,7,
                0,1,4,1,4,5,
                2,3,6,3,6,7,
                0,2,4,2,4,6,
                1,3,5,3,5,7
            };
            video::IGPUBuffer* index = driver->createGPUBuffer(sizeof(indices_indexed16),indices_indexed16);
            desc->mapIndexBuffer(index);
            mb->setIndexType(video::EIT_16BIT);
            mb->setIndexCount(2*3*6);
            mb->setIndexRange(0,7);
            index->drop();

            float attrArr[] = {
                -1.f,-1.f,-1.f,0.f,0.f,
                 1.f,-1.f,-1.f,0.5f,0.f,
                -1.f, 1.f,-1.f,1.f,0.f,
                 1.f, 1.f,-1.f,0.f,0.5f,
                -1.f,-1.f, 1.f,0.5f,0.5f,
                 1.f,-1.f, 1.f,1.f,0.5f,
                -1.f, 1.f, 1.f,0.f,1.f,
                 1.f, 1.f, 1.f,0.5f,1.f
            };
            video::IGPUBuffer* attr0 = driver->createGPUBuffer(sizeof(attrArr),attrArr);
            desc->mapVertexAttrBuffer(attr0,scene::EVAI_ATTR0,scene::ECPA_THREE,scene::ECT_FLOAT,20,0);
            desc->mapVertexAttrBuffer(attr0,scene::EVAI_ATTR1,scene::ECPA_TWO,scene::ECT_FLOAT,20,3*4);
            attr0->drop();

            driver->setTransform(video::ETS_WORLD,core::matrix4());
            driver->setMaterial(material);
            driver->drawMeshBuffer(mb);
            mb->drop();*/
        }

		driver->endScene();

		// display frames per second in window title
		uint64_t time = device->getTimer()->getRealTime();
		if (time-lastFPSTime > 1000)
		{
			std::wostringstream sstr;
			sstr << L"Builtin Nodes Demo - Irrlicht Engine FPS:" << driver->getFPS() << " PrimitvesDrawn:" << driver->getPrimitiveCountDrawn();

			device->setWindowCaption(sstr.str().c_str());
			lastFPSTime = time;
		}
	}

    //create a screenshot
	video::IImage* screenshot = driver->createImage(video::ECF_A8R8G8B8,params.WindowSize);
    glReadPixels(0,0, params.WindowSize.Width,params.WindowSize.Height, GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, screenshot->getData());
    {
        // images are horizontally flipped, so we have to fix that here.
        uint8_t* pixels = (uint8_t*)screenshot->getData();

        const int32_t pitch=screenshot->getPitch();
        uint8_t* p2 = pixels + (params.WindowSize.Height - 1) * pitch;
        uint8_t* tmpBuffer = new uint8_t[pitch];
        for (uint32_t i=0; i < params.WindowSize.Height; i += 2)
        {
            memcpy(tmpBuffer, pixels, pitch);
            memcpy(pixels, p2, pitch);
            memcpy(p2, tmpBuffer, pitch);
            pixels += pitch;
            p2 -= pitch;
        }
        delete [] tmpBuffer;
    }
	driver->writeImageToFile(screenshot,"./screenshot.png");
	screenshot->drop();
	device->sleep(3000);

	device->drop();

	return 0;
}
