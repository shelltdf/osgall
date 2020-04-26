
#include <osg/PositionAttitudeTransform>
#include <osg/ShapeDrawable>
#include <osg/Shape>
#include <osg/MatrixTransform>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/StateSetManipulator>
#include <osgGA/TrackballManipulator>

#include <osgEarth/MapNode>

#include <osgEarthUtil/ExampleResources>
#include <osgEarthUtil/EarthManipulator>
#include <osgEarthUtil/AutoClipPlaneHandler>
#include <osgEarthUtil/LogarithmicDepthBuffer>

#include <osgEarthDrivers/tms/TMSOptions>
#include <osgEarthDrivers/xyz/XYZOptions>
#include <osgEarthDrivers/feature_ogr/OGRFeatureOptions>
#include <osgEarthDrivers/model_feature_geom/FeatureGeomModelOptions>
#include <osgEarthDrivers/sky_simple/SimpleSkyOptions>
#include <osgEarthDrivers/sky_gl/GLSkyOptions>
#include <osgEarthDrivers/cache_filesystem/FileSystemCache>
//#include <osgEarthDrivers/sky_gl/>

//#include <osgEarthSymbology/AGG.h>

#include <osgEarthAnnotation/CircleNode>
#include <osgEarthAnnotation/EllipseNode>
#include <osgEarthAnnotation/FeatureNode>
#include <osgEarthAnnotation/ImageOverlay>
#include <osgEarthAnnotation/LabelNode>
#include <osgEarthAnnotation/LocalGeometryNode>
#include <osgEarthAnnotation/ModelNode>
#include <osgEarthAnnotation/PlaceNode>
#include <osgEarthAnnotation/RectangleNode>

#include <osgEarthAnnotation/AnnotationRegistry>


#include "../../common/launcher.h"
#include "../../common/use_osg_static_library.h"

#include "GLES2ShaderGenVisitor.h"
#include "InstancedObject.h"


using namespace osgEarth;
using namespace osgEarth::Drivers;
using namespace osgEarth::Features;
using namespace osgEarth::Symbology;
using namespace osgEarth::Util;


#define IMAGERY_URL      "http://readymap.org/readymap/tiles/1.0.0/22/"
#define ELEVATION_URL    "http://readymap.org/readymap/tiles/1.0.0/9/"
#define BUILDINGS_URL    "./data/boston_buildings_utm19.shp"
//#define RESOURCE_LIB_URL "./data/resources/textures_us/catalog.xml"
#define RESOURCE_LIB_URL "http://localhost:5103/data/resources/textures_us/catalog.xml"
#define STREETS_URL      "./data/boston-scl-utm19n-meters.shp"
#define PARKS_URL        "./data/boston-parks.shp"
#define TREE_MODEL_URL   "./data/loopix/tree4.osgb"



//for annotation
USE_OSGEARTH_REGISTER_ANNOTATION(circle)
USE_OSGEARTH_REGISTER_ANNOTATION(ellipse)
USE_OSGEARTH_REGISTER_ANNOTATION(feature)
USE_OSGEARTH_REGISTER_ANNOTATION(imageoverlay)
USE_OSGEARTH_REGISTER_ANNOTATION(label)
USE_OSGEARTH_REGISTER_ANNOTATION(local_geometry)
USE_OSGEARTH_REGISTER_ANNOTATION(model)
USE_OSGEARTH_REGISTER_ANNOTATION(place)
USE_OSGEARTH_REGISTER_ANNOTATION(rectangle)



void addImagery(osgEarth::Map* map)
{
    // add a TMS imagery layer:
    osgEarth::Drivers::TMSOptions imagery;
    imagery.url() = IMAGERY_URL;
    map->addImageLayer( new osgEarth::ImageLayer("ReadyMap imagery", imagery) );
}


void addElevation(osgEarth::Map* map)
{
    // add a TMS elevation layer:
    osgEarth::Drivers::TMSOptions elevation;
    elevation.url() = ELEVATION_URL;
    map->addElevationLayer( new osgEarth::ElevationLayer("ReadyMap elevation", elevation) );
}

void addBuildings(Map* map)
{
    // create a feature source to load the building footprint shapefile.
    OGRFeatureOptions feature_opt;
    feature_opt.name() = "buildings";
    feature_opt.url() = BUILDINGS_URL;
    feature_opt.buildSpatialIndex() = true;
    
    // a style for the building data:
    Style buildingStyle;
    buildingStyle.setName( "buildings" );

    // Extrude the shapes into 3D buildings.
    ExtrusionSymbol* extrusion = buildingStyle.getOrCreate<ExtrusionSymbol>();
    extrusion->heightExpression() = NumericExpression( "3.5 * max( [story_ht_], 1 )" );
    extrusion->flatten() = true;
    extrusion->wallStyleName() = "building-wall";
    extrusion->roofStyleName() = "building-roof";

    PolygonSymbol* poly = buildingStyle.getOrCreate<PolygonSymbol>();
    poly->fill()->color() = Color::White;

    // Clamp the buildings to the terrain.
    AltitudeSymbol* alt = buildingStyle.getOrCreate<AltitudeSymbol>();
    alt->clamping() = alt->CLAMP_TO_TERRAIN;
    alt->binding() = alt->BINDING_VERTEX;

    // a style for the wall textures:
    Style wallStyle;
    wallStyle.setName( "building-wall" );
    SkinSymbol* wallSkin = wallStyle.getOrCreate<SkinSymbol>();
    wallSkin->library() = "us_resources";
    wallSkin->addTag( "building" );
    wallSkin->randomSeed() = 1;

    // a style for the rooftop textures:
    Style roofStyle;
    roofStyle.setName( "building-roof" );
    SkinSymbol* roofSkin = roofStyle.getOrCreate<SkinSymbol>();
    roofSkin->library() = "us_resources";
    roofSkin->addTag( "rooftop" );
    roofSkin->randomSeed() = 1;
    roofSkin->isTiled() = true;

    // assemble a stylesheet and add our styles to it:
    StyleSheet* styleSheet = new StyleSheet();
    styleSheet->addStyle( buildingStyle );
    styleSheet->addStyle( wallStyle );
    styleSheet->addStyle( roofStyle );
    
    // load a resource library that contains the building textures.
    ResourceLibrary* reslib = new ResourceLibrary( "us_resources", RESOURCE_LIB_URL );
    styleSheet->addResourceLibrary( reslib );

    // set up a paging layout for incremental loading. The tile size factor and
    // the visibility range combine to determine the tile size, such that
    // tile radius = max range / tile size factor.
    FeatureDisplayLayout layout;
    layout.tileSizeFactor() = 52.0;
    layout.addLevel( FeatureLevel(0.0f, 20000.0f, "buildings") );

    // create a model layer that will render the buildings according to our style sheet.
    FeatureGeomModelOptions fgm_opt;
    fgm_opt.featureOptions() = feature_opt;
    fgm_opt.styles() = styleSheet;
    fgm_opt.layout() = layout;

    map->addModelLayer( new ModelLayer( "buildings", fgm_opt ) );
}


void addStreets(Map* map)
{
    // create a feature source to load the street shapefile.
    OGRFeatureOptions feature_opt;
    feature_opt.name() = "streets";
    feature_opt.url() = STREETS_URL;
    feature_opt.buildSpatialIndex() = true;

    // a resampling filter will ensure that the length of each segment falls
    // within the specified range. That can be helpful to avoid cropping 
    // very long lines segments.
    //feature_opt.filters().push_back( new ResampleFilter(0.0, 25.0) );

    // a style:
    Style style;
    style.setName( "streets" );

    // Render the data as translucent yellow lines that are 7.5m wide.
    LineSymbol* line = style.getOrCreate<LineSymbol>();
    line->stroke()->color() = Color(Color::Yellow, 0.5f);
    line->stroke()->width() = 7.5f;
    line->stroke()->widthUnits() = Units::METERS;

    // Clamp the lines to the terrain.
    AltitudeSymbol* alt = style.getOrCreate<AltitudeSymbol>();
    alt->clamping() = alt->CLAMP_TO_TERRAIN;

    // Apply a depth offset to avoid z-fighting. The "min bias" is the minimum
    // apparent offset (towards the camera) of the geometry from its actual position.
    // The value here was chosen empirically by tweaking the "oe_doff_min_bias" uniform.
    RenderSymbol* render = style.getOrCreate<RenderSymbol>();
    render->depthOffset()->minBias() = 6.6f;

    // Set up a paging layout. The tile size factor and the visibility range combine
    // to determine the tile size, such that tile radius = max range / tile size factor.
    FeatureDisplayLayout layout;
    layout.tileSizeFactor() = 7.5f;
    layout.maxRange()       = 5000.0f;

    // create a model layer that will render the buildings according to our style sheet.
    FeatureGeomModelOptions fgm_opt;
    fgm_opt.featureOptions() = feature_opt;
    fgm_opt.layout() = layout;
    fgm_opt.styles() = new StyleSheet();
    fgm_opt.styles()->addStyle( style );

    map->addModelLayer( new ModelLayer("streets", fgm_opt) );
}

osg::Program* loadProgram(const std::string& vs_string, const std::string& fs_string)
{
    osg::Program* p = new osg::Program();

    osg::Shader* vs = new osg::Shader(osg::Shader::VERTEX, vs_string);
    osg::Shader* fs = new osg::Shader(osg::Shader::FRAGMENT, fs_string);

    p->addShader(vs);
    p->addShader(fs);

    return p;
}


void GLES_SS(osg::StateSet* ss)
{
	//#ifdef OSG_GLES2_AVAILABLE
#if 1

	//GLES2
    //root->getOrCreateStateSet()->setAttributeAndModes(loadProgram(
    ss->setAttributeAndModes(loadProgram(
		//vs
		"attribute vec4 osg_MultiTexCoord0;                                     \n"
        "varying vec4 color;                                                    \n"
        "varying vec4 uv;                                                       \n"
		"const vec3 lightPos      =vec3(0.0, 0.0, 10.0);                        \n"
		"const vec4 cessnaColor   =vec4(0.8, 0.8, 0.8, 1.0);                    \n"
		"const vec4 lightAmbient  =vec4(0.1, 0.1, 0.1, 1.0);                    \n"
		"const vec4 lightDiffuse  =vec4(1.0, 1.0, 1.0, 1.0);                    \n"
		"const vec4 lightSpecular =vec4(0.8, 0.8, 0.8, 1.0);                    \n"
		"void DirectionalLight(in vec3 normal,                                  \n"
		"                      in vec3 ecPos,                                   \n"
		"                      inout vec4 ambient,                              \n"
		"                      inout vec4 diffuse,                              \n"
		"                      inout vec4 specular)                             \n"
		"{                                                                      \n"
		"     float nDotVP;                                                     \n"
		"     vec3 L = normalize(gl_ModelViewMatrix*vec4(lightPos, 0.0)).xyz;   \n"
		"     nDotVP = max(0.0, dot(normal, L));                                \n"
		"                                                                       \n"
		"     if (nDotVP > 0.0) {                                               \n"
		"       vec3 E = normalize(-ecPos);                                     \n"
		"       vec3 R = normalize(reflect( L, normal ));                       \n"
		"       specular = pow(max(dot(R, E), 0.0), 16.0) * lightSpecular;      \n"
		"     }                                                                 \n"
		"     ambient  = lightAmbient;                                          \n"
		"     diffuse  = lightDiffuse * nDotVP;                                 \n"
		"}                                                                      \n"
		"void main() {                                                          \n"
		"    vec4 ambiCol = vec4(0.0);                                          \n"
		"    vec4 diffCol = vec4(0.0);                                          \n"
		"    vec4 specCol = vec4(0.0);                                          \n"
		"    gl_Position   = gl_ModelViewProjectionMatrix * gl_Vertex;          \n"
		"    vec3 normal   = normalize(gl_NormalMatrix * gl_Normal);            \n"
		"    vec4 ecPos    = gl_ModelViewMatrix * gl_Vertex;                    \n"
		"    DirectionalLight(normal, ecPos.xyz, ambiCol, diffCol, specCol);    \n"
		"    color = cessnaColor * (ambiCol + diffCol + specCol);               \n"
		"    uv = osg_MultiTexCoord0;                                           \n"
		"}                                                                      \n"
        "\n"
        ,
		
		//fs
        "precision mediump float;                  \n"
		"uniform sampler2D tex;                    \n"
		"varying mediump vec4 uv;                  \n"
		"varying mediump vec4 color;               \n"
		"void main() {                             \n"
		//"  gl_FragColor = color;                   \n"
		"  gl_FragColor = color * texture2D( tex, uv.st ); \n"
		"}                                         \n"
        "\n"
    )
    //, osg::StateAttribute::ON | osg::StateAttribute::PROTECTED);
    //, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
    , osg::StateAttribute::ON );
	
	
#endif
}

//osg::ref_ptr<osgEarth::Drivers::SimpleSky::SkyNode> skyNode;
//osg::ref_ptr<osgEarth::Drivers::GLSky::GLSkyNode> skyNode;


void initSkyNode(osgViewer::Viewer* viewer, osgEarth::MapNode* mapNode)
{
osg::ref_ptr<osgEarth::Drivers::SimpleSky::SkyNode> skyNode;

	osgEarth::Drivers::SimpleSky::SimpleSkyOptions sky;
	//osgEarth::Drivers::GLSky::GLSkyOptions sky;
	//sky.atmosphericLighting() = false;
	sky.ambient() = 1.0;
	skyNode = osgEarth::Drivers::SimpleSky::SkyNode::create(sky, mapNode);
	//skyNode = new osgEarth::Drivers::GLSky::GLSkyNode(sky, mapNode);


	skyNode->setDateTime(osgEarth::DateTime::DateTime(2000, 1, 1, 5 + 8.0f));

	skyNode->setMoonVisible(true);
	skyNode->setSunVisible(true);
	skyNode->setStarsVisible(true);

	skyNode->attach(viewer, 0);

	osgEarth::insertGroup(skyNode.get(), mapNode);
}


#if 1
#include <osgText/String>
#include <osgText/Text>
class myTextNode
{
public:
    myTextNode(const osg::Vec3& pos)
    {
        mPAT = new osg::PositionAttitudeTransform();
        mGeode = new osg::Geode();
        mText = new osgText::Text();
        mGeode->addDrawable(mText);
        mPAT->addChild(mGeode);
        mPAT->setPosition(pos);


        mText->setCharacterSize(24);
        mText->setCharacterSizeMode(osgText::Text::SCREEN_COORDS);
    }

    osg::PositionAttitudeTransform* mPAT;
    osg::Geode* mGeode;
    osgText::Text* mText;
};
#endif


 //float r = 0.0;
 //osg::MatrixTransform* mt = 0;

 //osgViewer::Viewer* g_viewer = 0;
 //osg::Group* g_root = 0;

 osgUtil::GLES2ShaderGenVisitor* sgv = 0;


class FileNotifyHandler 
    : public osg::NotifyHandler
{
public:
    FileNotifyHandler()
    {
        errfile.open("osgnoify_err.txt");
        outfile.open("osgnoify_out.txt");
    }

    void notify(osg::NotifySeverity severity, const char *message)
    {
        if (severity <= osg::WARN)
        {
            //fputs(message, stderr);
            errfile << message;
        }
        else
        {
            //fputs(message, stdout);
            outfile << message;
        }
    }

    std::ofstream errfile;
    std::ofstream outfile;

};


std::vector<osg::Vec4f> pos;
std::vector<osg::Quat> rot;
osg::ref_ptr<InstancedObject> node;
float a = 1.0;
float r = 1.0;

void onCreate(
    osgViewer::Viewer* viewer, osg::Group* root
    , float x, float y, float w, float h
    )
{
	//g_viewer = viewer;
	//g_root = root;


    osg::NotifyHandler* nh = osg::getNotifyHandler();
    if(nh)
    {
        FileNotifyHandler* fnh = dynamic_cast<FileNotifyHandler*>(nh);
        if (!fnh)
        {
            osg::setNotifyHandler(new FileNotifyHandler());
        }
    }
    else
    {
        osg::setNotifyHandler(new FileNotifyHandler());
    }


#if 1
	//osgUtil::GLES2ShaderGenVisitor* sgv = 0;
	sgv = new osgUtil::GLES2ShaderGenVisitor();
#endif

	
    viewer->getCamera()->setClearColor(osg::Vec4(1, 0, 0, 1));
    //viewer->setThreadingModel(osgViewer::Viewer::SingleThreaded);

#if 0

    //BOX
    osg::ShapeDrawable* sd = new osg::ShapeDrawable(
        new osg::Box(osg::Vec3(0, 0, 0), 1.0));
    osg::Geode* geode = new osg::Geode();
    geode->addDrawable(sd);
    //root->addChild(geode);

    osg::MatrixTransform* mt = new osg::MatrixTransform();
    mt->addChild(geode);
    root->addChild(mt);


    GLES_SS(root->getOrCreateStateSet());

    //sgv->apply(*mapNode);
	//root->accept(*sgv);

#endif

#if 1

    //std::vector<osg::Vec4f> pos;
    //std::vector<osg::Quat> rot;
    for (size_t i = 0; i < 2; i++)
    {
        for (size_t j = 0; j < 2; j++)
        {
            for (size_t k = 0; k < 2; k++)
            {
                pos.push_back(osg::Vec4(i, j, k, 0));

                osg::Quat q;
                osg::Vec3 v(0, 0, 0); v.normalize();
                q.makeRotate(osg::Y_AXIS, v);
                rot.push_back(q);
            }
        }
    }

    //load file 
    /*osg::ref_ptr<InstancedObject>*/ node = new InstancedObject(pos, rot,false);

    root->addChild(node);
#endif


#if 0
    //load font
    //osgText::Font* font = osgText::readFontFile("../data/_osgxi/_font/msyh.ttf");


    //HUD
    osg::Camera* hud = new osg::Camera();
    hud->setReferenceFrame(osg::Camera::ABSOLUTE_RF);
    hud->setProjectionMatrixAsOrtho(0, 640, 0, 480, -1, 1);
    hud->setClearMask(GL_DEPTH_BUFFER_BIT);
    hud->getOrCreateStateSet()->setMode(GL_LIGHTING, false);
    root->addChild(hud);

    //////////////////////////////////////////////////////////////////////////
    //中文显示的四种接口 （三种可行）
    //////////////////////////////////////////////////////////////////////////

    myTextNode* MTN_01 = new myTextNode(osg::Vec3(0, 24 * 0, 0));
    //MTN_01->mText->setFont(font);
    MTN_01->mText->setText(L"01_ABC中文显示123 wchar_t");
    hud->addChild(MTN_01->mPAT);

    myTextNode* MTN_02 = new myTextNode(osg::Vec3(0, 24 * 1, 0));
    //MTN_02->mText->setFont(font);
    MTN_02->mText->setText(L"02_ABC中文显示123 std::string + ENCODING");
    hud->addChild(MTN_02->mPAT);

    myTextNode* MTN_03 = new myTextNode(osg::Vec3(0, 24 * 2, 0));
    //MTN_03->mText->setFont(font);
    MTN_03->mText->setText(std::string("03_ABC中文显示123 std::string"));//这个接口貌似无法实现中文正常显示
    hud->addChild(MTN_03->mPAT);

    myTextNode* MTN_04 = new myTextNode(osg::Vec3(0, 24 * 3, 0));
    osgText::String osgStr(L"04_ABC中文显示123");
    //MTN_04->mText->setFont(font);
    MTN_04->mText->setText(osgStr);
    hud->addChild(MTN_04->mPAT);

#endif


	//add earth
	//Map* map = new Map();
	
	// addImagery( map );
    // addElevation( map );
	
	//map node
	//MapNode* mapNode = new MapNode( map );
     //root->addChild( mapNode );
	
	//
	// EarthManipulator* manip = new EarthManipulator();
    // viewer->setCameraManipulator( manip );
	
	// zoom to a good startup position
    // manip->setViewpoint( Viewpoint(
        // "Home",
        // -71.0763, 42.34425, 0,   // longitude, latitude, altitude
         // 24.261, -21.6, 3450.0), // heading, pitch, range
         // 5.0 );                    // duration

    // This will mitigate near clip plane issues if you zoom in close to the ground:
    // LogarithmicDepthBuffer buf;
    // buf.install( viewer->getCamera() );
	

    //// add the state manipulator
    //viewer->addEventHandler( new osgGA::StateSetManipulator(viewer->getCamera()->getOrCreateStateSet()) );
    //// add the stats handler
    //viewer->addEventHandler(new osgViewer::StatsHandler);
	

#if 1
	osgGA::TrackballManipulator* tm = new osgGA::TrackballManipulator();
    viewer->setCameraManipulator(tm);


    // add the state manipulator
    viewer->addEventHandler(new osgGA::StateSetManipulator(viewer->getCamera()->getOrCreateStateSet()));

    // add the thread model handler
    viewer->addEventHandler(new osgViewer::ThreadingHandler);

    // add the window size toggle handler
    viewer->addEventHandler(new osgViewer::WindowSizeHandler);

    // add the stats handler
    osgViewer::StatsHandler* sh = new osgViewer::StatsHandler();
    sh->setKeyEventTogglesOnScreenStats('S');
    viewer->addEventHandler(sh);

    // add the help handler
    //viewer->addEventHandler(new osgViewer::HelpHandler(arguments.getApplicationUsage()));

    // add the record camera path handler
    viewer->addEventHandler(new osgViewer::RecordCameraPathHandler);

    // add the LOD Scale handler
    viewer->addEventHandler(new osgViewer::LODScaleHandler);

    // add the screen capture handler
    viewer->addEventHandler(new osgViewer::ScreenCaptureHandler);



	//前置更新
    viewer->frame();
    // osgViewer::GraphicsWindow* gw = dynamic_cast<osgViewer::GraphicsWindow*>( viewer->getCamera()->getGraphicsContext() );
    // if ( gw )
    {
        // Send window size event for initializing
        // int x, y, w, h; gw->getWindowRectangle( x, y, w, h );
        viewer->getEventQueue()->windowResize( x, y, w, h );
		viewer->getCamera()->setViewport(x, y, w, h);

        viewer->getCamera()->getGraphicsContext()->resized(x, y, w, h);

    }
#endif
	
}

void onFrame(osgViewer::Viewer* viewer, double dt)
{
     //r += dt * 90;
     //mt->setMatrix(
     //    osg::Matrix::rotate(osg::DegreesToRadians(r), osg::Vec3(0, 1, 0))
     //    //* osg::Matrix::translate(0, 0, -3)
     //    );
		
		
    pos.clear();
    rot.clear();
    for (size_t i = 0; i < 10; i++)
    {
        for (size_t j = 0; j < 10; j++)
        {
            for (size_t k = 0; k < 10; k++)
            {
                pos.push_back(osg::Vec4(i*sin(a), j*sin(a), k, 1.0));

                osg::Quat q;
                osg::Vec3 v(0, 1, 0);
                v = v * osg::Matrix::rotate(r, osg::Z_AXIS);
                v.normalize();
                q.makeRotate(osg::Y_AXIS, v);
                rot.push_back(q);

            }
        }
    }

    node->update(pos, rot);

    //a += 0.01;
    r += 0.01;

}

void onDestory(osgViewer::Viewer* viewer, osg::Group* root)
{
}


void onMessage(osgViewer::Viewer* viewer,std::string str)
{
	//remove all
	viewer->getSceneData()->asGroup()->removeChildren(0,viewer->getSceneData()->asGroup()->getNumChildren());

#if 0
    osgEarth::Drivers::FileSystemCacheOptions cacheOptions;
    cacheOptions.rootPath() = "d:/aaaaa";

    MapOptions mapOptions;
    mapOptions.cache() = cacheOptions;


	//add earth
    osgEarth::Map* map = new osgEarth::Map(mapOptions);
	map->getProfile();

	addImagery( map );
	addElevation( map );
	addBuildings( map );
    addStreets( map );


	//map node
	osgEarth::MapNode* mapNode = new osgEarth::MapNode( map );
	//g_root->addChild( mapNode );
	viewer->getSceneData()->asGroup()->addChild( mapNode );


	//sky
	initSkyNode(viewer, mapNode);

	//shader
	//GLES_SS(mapNode->getOrCreateStateSet());

	//mapNode->accept(*sgv);
	//sgv->apply(*mapNode);


	//
	osgEarth::Util::EarthManipulator* manip = new osgEarth::Util::EarthManipulator();
	viewer->setCameraManipulator( manip );

	// zoom to a good startup position
	manip->setViewpoint( osgEarth::Viewpoint(
		"Home",
		-71.0763, 42.34425, 0,   // longitude, latitude, altitude
		24.261, -21.6, 3450.0), // heading, pitch, range
		5.0 );                    // duration

	// This will mitigate near clip plane issues if you zoom in close to the ground:
	// LogarithmicDepthBuffer buf;
	// buf.install( viewer->getCamera() );


	//// add the state manipulator
	//viewer->addEventHandler( new osgGA::StateSetManipulator(viewer->getCamera()->getOrCreateStateSet()) );
	//// add the stats handler
	//viewer->addEventHandler(new osgViewer::StatsHandler);
#else

    int argc = 2;
    //std::vector< std::string > argv;
    char** argv = new char*[2];
    argv[0] = "exe";
    argv[1] = "D:/_work_bt/_osgall-code_32bit_static/trunk/prebuild/gwaldron-osgearth-25ce0e1/tests/annotation.earth";


    osg::ArgumentParser arguments(&argc, argv);
    osg::Node* node = MapNodeHelper().load(arguments, viewer);

    //osg::Node* node = osgDB::readNodeFile("D:/_work_bt/_osgall-code_32bit_static/trunk/prebuild/gwaldron-osgearth-25ce0e1/tests/annotation.earth");

    if (node)
    {
        //viewer->setSceneData(node);

        //viewer.getCamera()->setSmallFeatureCullingPixelSize(-1.0f);

        //return viewer.run();

        viewer->getSceneData()->asGroup()->addChild(node);

        //GLES_SS(viewer->getSceneData()->getOrCreateStateSet());
    }


    //
    osgEarth::Util::EarthManipulator* manip = new osgEarth::Util::EarthManipulator();
    viewer->setCameraManipulator(manip);


    //
    MapNode* mapNode = MapNode::findMapNode(node);
    if (!mapNode)
    {
        viewer->getCamera()->addCullCallback(new osgEarth::Util::AutoClipPlaneCullCallback(mapNode));
    }

#endif

}


void onSize(osgViewer::Viewer* viewer,int w, int h)
{
#if 0
	//g_root->accept(*sgv);

	//osgText::Font* font = osgText::readFontFile("data/msyh.ttf");

	//g_viewer->getCamera()->setViewport(0, 0, 100, 100);

	g_viewer->getEventQueue()->windowResize( 0, 0, w, h );
	g_viewer->getCamera()->setViewport(0, 0, w, h);
	//g_viewer->getCamera()->setProjectionMatrixAsPerspective(osg::DegreesToRadians(30.0f),float(w)/float(h),0.001,1000);
	g_viewer->getCamera()->setProjectionMatrixAsPerspective(30.0f,float(w)/float(h),0.001,1000);
#endif

	viewer->getEventQueue()->windowResize( 0, 0, w, h );
	viewer->getCamera()->setViewport(0, 0, w, h);
	//viewer->getCamera()->setProjectionMatrixAsPerspective(osg::DegreesToRadians(30.0f),float(w)/float(h),0.001,1000);
	viewer->getCamera()->setProjectionMatrixAsPerspective(30.0f,float(w)/float(h),0.001,1000);

    viewer->getCamera()->getGraphicsContext()->resized(0, 0, w, h);

}


void onMouse(osgViewer::Viewer* viewer,int type ,int moustbutton, int moustbuttonstate, int x, int y)
{

    viewer->getEventQueue()->getCurrentEventState()->setGraphicsContext(viewer->getCamera()->getGraphicsContext());

	if(type == 1) //click
	{
		osgGA::GUIEventAdapter::MouseButtonMask mm = osgGA::GUIEventAdapter::MouseButtonMask(0);
		if(moustbutton==1) mm = osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON;
		if(moustbutton==2) mm = osgGA::GUIEventAdapter::MIDDLE_MOUSE_BUTTON;
		if(moustbutton==3) mm = osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON;

		if(moustbuttonstate)
		{
			viewer->getEventQueue()->mouseButtonPress(x,y,mm);
		}
		else
		{
			viewer->getEventQueue()->mouseButtonRelease(x,y,mm);
		}
	}

	if(type == 2) //move
	{
		viewer->getEventQueue()->mouseMotion(x,y);
	}

	if(type == 3)
	{
	}

	if(type == 4) //scroll
	{
		if(y > 0)
		{
			viewer->getEventQueue()->mouseScroll(osgGA::GUIEventAdapter::SCROLL_UP);
		}
		else
		{
			viewer->getEventQueue()->mouseScroll(osgGA::GUIEventAdapter::SCROLL_DOWN);
		}
	}

    if (type == 5) //double click
    {
        osgGA::GUIEventAdapter::MouseButtonMask mm = osgGA::GUIEventAdapter::MouseButtonMask(0);
        if (moustbutton == 1) mm = osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON;
        if (moustbutton == 2) mm = osgGA::GUIEventAdapter::MIDDLE_MOUSE_BUTTON;
        if (moustbutton == 3) mm = osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON;


        viewer->getEventQueue()->mouseDoubleButtonPress(x, y, mm);
    }
}

void onKey(osgViewer::Viewer* viewer, int keycode, int keystate, int x, int y)
{
    TCHAR szLog[512] = { 0 };
    sprintf(szLog, ("onKey [%d, %d ,%c]\n"), keycode, keystate, char(keycode));
    OutputDebugString(szLog);



    if (keystate > 0)
    {
        viewer->getEventQueue()->keyPress(keycode);
    }
    else
    {
        viewer->getEventQueue()->keyRelease(keycode);
    }



}