#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Utilities.h"
#include "cinder/params/Params.h"
#include "cinder/Camera.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/gl/Batch.h"


using namespace ci;
using namespace ci::app;
using namespace std;

class world_to_screenApp : public App {
  public:
	void setup() override;
	void mouseMove( MouseEvent event ) override;
	void update() override;
	void draw() override;
    void prepareSettings( Settings *settings );
    
    vec2 worldToScreen( const vec3 &worldCoord, CameraPersp pCam, vec2 screenSize);
    vec2 worldToScreen( const vec3 &worldCoord, CameraOrtho oCam, vec2 screenSize);
    
    // PARAMS
    params::InterfaceGlRef	mParams;
    quat mSceneRotation;
    bool isOrtho, mEnableLight;
    
    // CAMERAS
    CameraPersp	mPcam;
    CameraOrtho mOcam;
    float mPcamDistance, mOcamDistance;
    vec3 mEye, mCenter, mUp;
    
//  LIGHTING
//    vec3					mLightPos;
//    Color					mLightColor;
//    float					mAmbientScale, mSpecScale, mSpecPow;
//    gl::VboMeshRef			mLightMesh;
//    gl::BatchRef			mLightBatch;
//    gl::GlslProgRef			mLightShader;

};

void world_to_screenApp::prepareSettings( Settings *settings )
{
    settings->setWindowSize( 1280, 720 );
    settings->setFrameRate( 60.0f );
}

void world_to_screenApp::setup()
{
    mEnableLight = true;
    isOrtho = false;
    
    mPcamDistance = 400.0f;
    mEye			= vec3( 0.0f, 0.0f, mPcamDistance );
    mCenter			= vec3(0.0f, 1.0f, 0.0f);
    mUp				= vec3(0.0f, 1.0f, 0.0f);
    mPcam.setPerspective( 75.0f, getWindowAspectRatio(), 5.0f, 2000.0f );
    mOcam.setOrtho( 0, getWindowWidth(), getWindowHeight(), 0, -1, 1 );
    
    gl::enableAlphaBlending();
    gl::enableDepthRead();
    gl::enableDepthWrite();
    
    // SETUP GUI PARAMS
    mParams = params::InterfaceGl::create( "World to Space", vec2( 200, 320 ) );
    mParams->addParam( "Scene Rotation", &mSceneRotation, "opened=1" );
    mParams->addSeparator();
    mParams->addParam( "Eye Distance", &mPcamDistance, "min=-5000.0 max=10500.0 step=50.0 keyIncr=s keyDecr=w" );
    mParams->addSeparator();
    mParams->addParam( "Toggle Orthogonal Camera", &isOrtho, "key=o" );
    mParams->addSeparator();
    
    
//  SETUP LIGHTING
//    
//    try
//    {
//        mLightShader = gl::GlslProg::create(loadAsset("basic_lighting.vert"), loadAsset("basic_lighting.frag"));
//    }
//    catch (const gl::GlslProgExc &e)
//    {
//        console() << e.what() << endl;
//    }
//
//    mLightMesh = gl::VboMesh::create(geom::Sphere().subdivisions(16).center(mLightPos).radius(0.01f));
//    mLightBatch = gl::Batch::create(mLightMesh, mLightShader);
//    mLightBatch->getGlslProg()->uniform("LightColor", mLightColor);
//    mParams->addParam( "Enable Lights", &mEnableLight, "key=l" );
//    mParams->addSeparator();
//    mParams->addParam("Light Color", &mLightColor);
//    mParams->addParam("Ambient Strength", &mAmbientScale);
//    mParams->addParam("Specular Strength", &mSpecScale);
//    mParams->addParam("Specular Power", &mSpecPow);
    
}

void world_to_screenApp::mouseMove(MouseEvent event)
{
}

void world_to_screenApp::update()
{
    // UPDATE CAMERAS
    mEye = vec3( 0.0f, 0.0f, mPcamDistance );
    mPcam.lookAt( mEye, mCenter, mUp );
    
    // Compute orthogonal cam parameters
    float tParamX = getWindowWidth() / 2.0 ;
    float tParamY = getWindowHeight() / 2.0 ;
    
    mOcam.setOrtho( -tParamX, tParamX, -tParamY, tParamY, -1000, 1000 );
    mOcam.lookAt( vec3( 0, 2, 0 ), vec3(0,0,0) );

    if(isOrtho)
        gl::setMatrices( mOcam );
    else
        gl::setMatrices( mPcam );

    mSceneRotation.y = 0.5*sin(getElapsedSeconds());
    gl::rotate( mSceneRotation );
}


void world_to_screenApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) );
    
    //TEST W2S TRANSFORMS
    cout<<"Perspective w2s: "<<worldToScreen(vec3(10,10,10), mPcam, getWindowSize())<<endl;
    cout<<"Orthogonal w2s: "<<worldToScreen(vec3(10,10,10), mOcam, getWindowSize())<<endl;

    
//    DISPLAY LIGHTING
//    mLightBatch->getGlslProg()->uniform("LightColor", mLightColor);
//    mLightBatch->draw();
    
    gl::drawCube(vec3(0, 0, 0), vec3(100, 100, 100));
    
    mParams->draw();
}


vec2 world_to_screenApp::worldToScreen( const vec3 &worldCoord, CameraPersp pCam, vec2 screenSize){
    return pCam.worldToScreen(worldCoord, screenSize.x, screenSize.y);
}

vec2 world_to_screenApp::worldToScreen( const vec3 &worldCoord, CameraOrtho oCam, vec2 screenSize){
    return oCam.worldToScreen(worldCoord, screenSize.x, screenSize.y);
}

CINDER_APP( world_to_screenApp, RendererGl )



