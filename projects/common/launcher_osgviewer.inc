//pc方式
int niubi_entry(niubi_entry_arg& arg)
{
    //LOGI("niubi_entry");
    printf("niubi_entry");

    //setup viewer
    osgViewer::Viewer viewer;
#if OSG_VERSION_LESS_THAN(3,1,0)
    viewer.setUpViewInWindow(100, 100, 1024, 768); //这里在320版本时导致启动的RESIZE无法接收
#else
    viewer.realize();
    //viewer.frame();
#endif


    //create scene
    osg::Group* root = new osg::Group();
    viewer.setSceneData(root);
    //viewer.setLightingMode(osg::View::NO_LIGHT);
    viewer.getCamera()->setClearColor(osg::Vec4(0.2, 0.2, 0.2, 1.0));
    //viewer.getCamera()->setClearMask(GL_DEPTH_BUFFER_BIT);


    //::onCreate(&viewer);
    osgViewer::GraphicsWindow* gw = dynamic_cast<osgViewer::GraphicsWindow*>(viewer.getCamera()->getGraphicsContext());
    if (gw)
    {
        int x, y, w, h;
        gw->getWindowRectangle(x, y, w, h);
        ::onCreate(&viewer, root, x, y, w, h);
    }

    //resize
    if (gw)
    {
        int x, y, w, h;
        gw->getWindowRectangle(x, y, w, h);
        viewer.getEventQueue()->windowResize(x, y, w, h);
    }


    //LOGI("loop");
    bool inited = false;
    Timer t;
    while (!viewer.done())
    {
        //LOGI("loop 1");
        //printf("loop");

        //计算更新时间 ms
        double dt = t.GetCounter();
        t.StartCounter();

        ////for android
        //if (!inited && g_window->isShow())
        //{
        //	//LOGI("onCreate");
        //	::onCreate();
        //	inited = true;
        //}

        //LOGI("loop 2");

        //g_window->frame( float(dt)/1000.0f );//s
        //g_window->sleep(0);

        ::onFrame(&viewer, float(dt) / 1000.0f);
        viewer.frame();

        //LOGI("loop 3");
        //LOGI("%d",g_window->isShow());
    }

    //	LOGI("onDestory");
    ::onDestory(&viewer, root);

    return 0;
}