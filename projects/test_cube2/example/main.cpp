
#include <osg/PositionAttitudeTransform>
#include <osg/ShapeDrawable>
#include <osg/Shape>
#include <osg/MatrixTransform>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/StateSetManipulator>

#include "../../common/launcher.h"
#include "../../common/use_osg_static_library.h"


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


float r = 0.0;
osg::MatrixTransform* mt = 0;


void onCreate(
    osgViewer::Viewer* viewer, osg::Group* root
    , float x, float y, float w, float h
    )
{
    viewer->getCamera()->setClearColor(osg::Vec4(1, 0, 1, 1));
    viewer->setThreadingModel(osgViewer::Viewer::SingleThreaded);

    //BOX
    osg::ShapeDrawable* sd = new osg::ShapeDrawable(
        new osg::Box(osg::Vec3(0, 0, 0), 1.0));
    osg::Geode* geode = new osg::Geode();
    geode->addDrawable(sd);
    //root->addChild(geode);

    /*osg::MatrixTransform**/ mt = new osg::MatrixTransform();
    mt->addChild(geode);
    root->addChild(mt);


#if 1
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

    //// add the state manipulator
    //viewer->addEventHandler( new osgGA::StateSetManipulator(viewer->getCamera()->getOrCreateStateSet()) );
    //// add the stats handler
    //viewer->addEventHandler(new osgViewer::StatsHandler);
}

void onFrame(osgViewer::Viewer* viewer, double dt)
{
    r += dt * 90;
    mt->setMatrix(
        osg::Matrix::rotate(osg::DegreesToRadians(r), osg::Vec3(0, 1, 0))
        * osg::Matrix::translate(0, 0, -3)
        );
}

void onDestory(osgViewer::Viewer* viewer, osg::Group* root)
{
}

