
#include <osg/PositionAttitudeTransform>
#include <osg/ShapeDrawable>
#include <osg/Shape>
#include <osg/MatrixTransform>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osgViewer/Viewer>
#include <osgViewer/CompositeViewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/StateSetManipulator>

#include "../../common/launcher.h"
#include "../../common/use_osg_static_library.h"


osg::Program* loadProgram(const std::string& vs_string, const std::string& fs_string)
{
    osg::Program* p = new osg::Program();

    osg::Shader* vs = new osg::Shader(osg::Shader::VERTEX, vs_string);
    osg::Shader* fs = new osg::Shader(osg::Shader::FRAGMENT, fs_string);

    p->addShader(vs);
    p->addShader(fs);

    return p;
}



#if 1
#include <osgText/String>
#include <osgText/Text>
class myTextNode
{
public:
    myTextNode(const osg::Vec3& pos,osgText::Font* font = 0)
    {
        mPAT = new osg::PositionAttitudeTransform();
        mGeode = new osg::Geode();
        mText = new osgText::Text();
        mGeode->addDrawable(mText);
        mPAT->addChild(mGeode);
        mPAT->setPosition(pos);


        mText->setCharacterSize(24);
        mText->setCharacterSizeMode(osgText::Text::SCREEN_COORDS);

		if(font)
		{
			mText->setFont(font);
		}
    }

    osg::PositionAttitudeTransform* mPAT;
    osg::Geode* mGeode;
    osgText::Text* mText;
};
#endif

//
//float r = 0.0;
//osg::MatrixTransform* mt = 0;
//
//osgViewer::Viewer* g_viewer = 0;
//osg::Group* g_root = 0;

osgViewer::ViewerBase* g_viewer = 0;


class CB
	:public osg::NodeCallback
{
public:
	CB():r(0){}
	virtual ~CB(){}

	float r;

	virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
	{

		r += 0.1;

		osg::MatrixTransform* mt = dynamic_cast<osg::MatrixTransform*>(node);
		if(mt)
		{
			mt->setMatrix(
				osg::Matrix::rotate(osg::DegreesToRadians(r), osg::Vec3(0, 1, 0))
				* osg::Matrix::translate(0, 0, -3)
				);
		}

	}

};

void onCreate(
    osgViewer::Viewer* viewer, osg::Group* root
    , float x, float y, float w, float h
    )
{

	//int msgboxID = MessageBox(
 //       NULL,
 //       "onCreate",
 //       "onCreate",
 //       MB_ICONWARNING | MB_CANCELTRYCONTINUE | MB_DEFBUTTON2
 //   );


	//g_viewer = viewer;
	//g_root = root;
	
#ifdef OSG_GLES2_AVAILABLE

	//GLES2
    root->getOrCreateStateSet()->setAttributeAndModes(loadProgram(
		//vs
		"attribute vec4 osg_MultiTexCoord0;                                     \n"
        "varying vec4 color;                                                    \n"
        "varying vec4 uv;                                                       \n"
		"const vec3 lightPos      =vec3(0.0, 0.0, 10.0);                        \n"
		"const vec4 cessnaColor   =vec4(0.8, 0.8, 0.8, 1.0);                    \n"
		"const vec4 lightAmbient  =vec4(0.1, 0.1, 0.1, 1.0);                    \n"
		"const vec4 lightDiffuse  =vec4(0.4, 0.4, 0.4, 1.0);                    \n"
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
		"  gl_FragColor = color;                   \n"
		"  gl_FragColor = color * texture2D( tex, uv.st ); \n"
		"}                                         \n"
        "\n"
    ), osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
	
	
#endif
	
    viewer->getCamera()->setClearColor(osg::Vec4(1, 0, 1, 1));
    viewer->setThreadingModel(osgViewer::Viewer::SingleThreaded);

    //BOX
    osg::ShapeDrawable* sd = new osg::ShapeDrawable(
        new osg::Box(osg::Vec3(0, 0, 0), 1.0));
    osg::Geode* geode = new osg::Geode();
    geode->addDrawable(sd);
    //root->addChild(geode);

    osg::MatrixTransform* mt = new osg::MatrixTransform();
    mt->addChild(geode);
    root->addChild(mt);

	mt->addUpdateCallback(new CB());

#if 1
    //load font
    //osgText::Font* font = osgText::readFontFile("../data/_osgxi/_font/msyh.ttf");
    osgText::Font* font = osgText::readFontFile("D:/_work_bt/_aliyun_svn/myproject/project_bim/data/test/msyh.ttf");


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

    myTextNode* MTN_01 = new myTextNode(osg::Vec3(0, 24 * 0, 0),font);
    //MTN_01->mText->setFont(font);
    MTN_01->mText->setText(L"01_ABC中文显示123 wchar_t");
    hud->addChild(MTN_01->mPAT);

    myTextNode* MTN_02 = new myTextNode(osg::Vec3(0, 24 * 1, 0),font);
    //MTN_02->mText->setFont(font);
    MTN_02->mText->setText(L"02_ABC中文显示123 std::string + ENCODING");
    hud->addChild(MTN_02->mPAT);

    myTextNode* MTN_03 = new myTextNode(osg::Vec3(0, 24 * 2, 0),font);
    //MTN_03->mText->setFont(font);
    MTN_03->mText->setText(std::string("03_ABC中文显示123 std::string"));//这个接口貌似无法实现中文正常显示
    hud->addChild(MTN_03->mPAT);

    myTextNode* MTN_04 = new myTextNode(osg::Vec3(0, 24 * 3, 0),font);
    osgText::String osgStr(L"04_ABC中文显示123");
    //MTN_04->mText->setFont(font);
    MTN_04->mText->setText(osgStr);
    hud->addChild(MTN_04->mPAT);

#endif

    //// add the state manipulator
    //viewer->addEventHandler( new osgGA::StateSetManipulator(viewer->getCamera()->getOrCreateStateSet()) );
    //// add the stats handler
    //viewer->addEventHandler(new osgViewer::StatsHandler);
	
#if 0
	//前置更新
    viewer->frame();
    // osgViewer::GraphicsWindow* gw = dynamic_cast<osgViewer::GraphicsWindow*>( viewer->getCamera()->getGraphicsContext() );
    // if ( gw )
    {
        // Send window size event for initializing
        // int x, y, w, h; gw->getWindowRectangle( x, y, w, h );
        viewer->getEventQueue()->windowResize( x, y, w, h );
		viewer->getCamera()->setViewport(x, y, w, h);
    }
#endif

}

void onFrame(osgViewer::Viewer* viewer, double dt)
{
	g_viewer = viewer;

    //r += dt * 90;
    //mt->setMatrix(
    //    osg::Matrix::rotate(osg::DegreesToRadians(r), osg::Vec3(0, 1, 0))
    //    * osg::Matrix::translate(0, 0, -3)
    //    );
}

void onDestory(osgViewer::Viewer* viewer, osg::Group* root)
{
}

void onMessage(std::string str)
{

}


void onSize(osgViewer::Viewer* viewer,int w, int h)
{
#if 0
	//osgText::Font* font = osgText::readFontFile("D:/_work_bt/_aliyun_svn/myproject/project_bim/data/test/msyh.ttf");

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

	//if(g_viewer)
	//{
	//	g_viewer->getEventQueue()->windowResize( 0, 0, w, h );
	//	g_viewer->getCamera()->setViewport(0, 0, w, h);
	//	g_viewer->getCamera()->setProjectionMatrixAsPerspective(30.0f,float(w)/float(h),0.001,1000);
	//}

}

void onMouse(int type ,int moustbutton, int moustbuttonstate, int x, int y)
{

}
