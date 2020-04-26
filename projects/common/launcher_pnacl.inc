

#include <GLES2/gl2.h>
#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

// #include "matrix.h"
#include "ppapi/cpp/graphics_3d.h"
#include "ppapi/cpp/instance.h"
#include "ppapi/cpp/module.h"
#include "ppapi/cpp/var.h"
#include "ppapi/cpp/var_array.h"
#include "ppapi/lib/gl/gles2/gl2ext_ppapi.h"
#include "ppapi/utility/completion_callback_factory.h"

#ifdef WIN32
#undef PostMessage
// Allow 'this' in initializer list
#pragma warning(disable : 4355)
#endif



class Graphics3DInstance : public pp::Instance
{
 public:
  explicit Graphics3DInstance(PP_Instance instance)
      : pp::Instance(instance),
        callback_factory_(this),
        width_(0),
        height_(0)
        // frag_shader_(0),
        // vertex_shader_(0),
        // program_(0),
        // texture_loc_(0),
        // position_loc_(0),
        // color_loc_(0),
        // mvp_loc_(0),
        // x_angle_(0),
        // y_angle_(0),
        // animating_(true)
		{}

  osg::ref_ptr<osgViewer::Viewer> _viewer;
  osg::ref_ptr<osg::Group> _root;
  Timer t;
  
  virtual ~Graphics3DInstance()
  {
	  ::onDestory(_viewer.get(),_root);
  }
	
  virtual bool Init(uint32_t argc, const char* argn[], const char* argv[])
  {
    return true;
  }

  virtual void DidChangeView(const pp::View& view) 
  {
    // Pepper specifies dimensions in DIPs (device-independent pixels). To
    // generate a context that is at device-pixel resolution on HiDPI devices,
    // scale the dimensions by view.GetDeviceScale().
    int32_t new_width = view.GetRect().width() * view.GetDeviceScale();
    int32_t new_height = view.GetRect().height() * view.GetDeviceScale();

    if (context_.is_null()) 
	{
      if (!InitGL(new_width, new_height))
	  {
        // failed.
        return;
      }

      // InitShaders();
      // InitBuffers();
      // InitTexture();
	  
	    //setup viewer
        _viewer = new osgViewer::Viewer();
        _viewer->setUpViewerAsEmbeddedInWindow(0,0, new_width, new_width);
        _viewer->realize();

        //setup scene
        _root = new osg::Group();
        _viewer->setSceneData(_root);
        //viewer.setLightingMode(osg::View::NO_LIGHT);
        //_viewer->getCamera()->setClearColor( osg::Vec4(1,0,1,1));
        //_viewer->setThreadingModel(osgViewer::Viewer::SingleThreaded);
		
		
	    ::onCreate(_viewer, _root, 0, 0, new_width, new_width);
	  
	  
      MainLoop(0);
	  
    } 
	else 
	{
      // Resize the buffers to the new size of the module.
      int32_t result = context_.ResizeBuffers(new_width, new_height);
      if (result < 0) 
	  {
        fprintf(stderr,
                "Unable to resize buffers to %d x %d!\n",
                new_width,
                new_height);
        return;
      }
    }

    width_ = new_width;
    height_ = new_height;
    glViewport(0, 0, width_, height_);
  }

  virtual void HandleMessage(const pp::Var& message) 
  {
#if 0
    // A bool message sets whether the cube is animating or not.
    if (message.is_bool()) 
	{
      animating_ = message.AsBool();
      return;
    }

    // An array message sets the current x and y rotation.
    if (!message.is_array())
	{
      fprintf(stderr, "Expected array message.\n");
      return;
    }

    pp::VarArray array(message);
    if (array.GetLength() != 2) 
	{
      fprintf(stderr, "Expected array of length 2.\n");
      return;
    }

    pp::Var x_angle_var = array.Get(0);
    if (x_angle_var.is_int()) 
	{
      x_angle_ = x_angle_var.AsInt();
    } 
	else if (x_angle_var.is_double()) 
	{
      x_angle_ = x_angle_var.AsDouble();
    } 
	else 
	{
      fprintf(stderr, "Expected value to be an int or double.\n");
    }

    pp::Var y_angle_var = array.Get(1);
    if (y_angle_var.is_int()) 
	{
      y_angle_ = y_angle_var.AsInt();
    } 
	else if (y_angle_var.is_double()) 
	{
      y_angle_ = y_angle_var.AsDouble();
    } 
	else 
	{
      fprintf(stderr, "Expected value to be an int or double.\n");
    }
#endif
  }

 private:
 
  bool InitGL(int32_t new_width, int32_t new_height) 
  {
    if (!glInitializePPAPI(pp::Module::Get()->get_browser_interface())) 
	{
      fprintf(stderr, "Unable to initialize GL PPAPI!\n");
      return false;
    }

    const int32_t attrib_list[] = 
	{
      PP_GRAPHICS3DATTRIB_ALPHA_SIZE, 8,
      PP_GRAPHICS3DATTRIB_DEPTH_SIZE, 24,
      PP_GRAPHICS3DATTRIB_WIDTH, new_width,
      PP_GRAPHICS3DATTRIB_HEIGHT, new_height,
      PP_GRAPHICS3DATTRIB_NONE
    };

    context_ = pp::Graphics3D(this, attrib_list);
    if (!BindGraphics(context_)) 
	{
      fprintf(stderr, "Unable to bind 3d context!\n");
      context_ = pp::Graphics3D();
      glSetCurrentContextPPAPI(0);
      return false;
    }

    glSetCurrentContextPPAPI(context_.pp_resource());
    return true;
  }

#if 0
  void InitShaders() 
  {
    frag_shader_ = CompileShader(GL_FRAGMENT_SHADER, kFragShaderSource);
    if (!frag_shader_)
      return;

    vertex_shader_ = CompileShader(GL_VERTEX_SHADER, kVertexShaderSource);
    if (!vertex_shader_)
      return;

    program_ = LinkProgram(frag_shader_, vertex_shader_);
    if (!program_)
      return;

    texture_loc_ = glGetUniformLocation(program_, "u_texture");
    position_loc_ = glGetAttribLocation(program_, "a_position");
    texcoord_loc_ = glGetAttribLocation(program_, "a_texcoord");
    color_loc_ = glGetAttribLocation(program_, "a_color");
    mvp_loc_ = glGetUniformLocation(program_, "u_mvp");
  }

  void InitBuffers() 
  {
    glGenBuffers(1, &vertex_buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(kCubeVerts), &kCubeVerts[0],
                 GL_STATIC_DRAW);

    glGenBuffers(1, &index_buffer_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(kCubeIndexes),
                 &kCubeIndexes[0], GL_STATIC_DRAW);
  }

  void InitTexture() 
  {
    DecompressTexture();
    glGenTextures(1, &texture_);
    glBindTexture(GL_TEXTURE_2D, texture_);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGB,
                 128,
                 128,
                 0,
                 GL_RGB,
                 GL_UNSIGNED_BYTE,
                 &g_texture_data[0]);
  }
  

  void Animate() 
  {
    if (animating_) 
	{
      x_angle_ = fmod(360.0f + x_angle_ + kXAngleDelta, 360.0f);
      y_angle_ = fmod(360.0f + y_angle_ + kYAngleDelta, 360.0f);

      // Send new values to JavaScript.
      pp::VarArray array;
      array.SetLength(2);
      array.Set(0, x_angle_);
      array.Set(1, y_angle_);
      PostMessage(array);
    }
  }

  void Render() 
  {
    glClearColor(0.5, 0.5, 0.5, 1);
    glClearDepthf(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    //set what program to use
    glUseProgram(program_);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_);
    glUniform1i(texture_loc_, 0);

    //create our perspective matrix
    float mvp[16];
    float trs[16];
    float rot[16];

    identity_matrix(mvp);
    const float aspect_ratio = static_cast<float>(width_) / height_;
    glhPerspectivef2(&mvp[0], kFovY, aspect_ratio, kZNear, kZFar);

    translate_matrix(0, 0, kCameraZ, trs);
    rotate_matrix(x_angle_, y_angle_, 0.0f, rot);
    multiply_matrix(trs, rot, trs);
    multiply_matrix(mvp, trs, mvp);
    glUniformMatrix4fv(mvp_loc_, 1, GL_FALSE, mvp);

    //define the attributes of the vertex
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);
    glVertexAttribPointer(position_loc_,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(Vertex),
                          reinterpret_cast<void*>(offsetof(Vertex, loc)));
    glEnableVertexAttribArray(position_loc_);
    glVertexAttribPointer(color_loc_,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(Vertex),
                          reinterpret_cast<void*>(offsetof(Vertex, color)));
    glEnableVertexAttribArray(color_loc_);
    glVertexAttribPointer(texcoord_loc_,
                          2,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(Vertex),
                          reinterpret_cast<void*>(offsetof(Vertex, tex)));
    glEnableVertexAttribArray(texcoord_loc_);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, 0);
  }
#endif

  void MainLoop(int32_t) 
  {
    // Animate();
    // Render();
	
	// glClearColor(0.5, 0.5, 0.5, 1);
    // glClearDepthf(1.0f);
    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//计算更新时间 ms
    double dt = t.GetCounter();
    t.StartCounter();
		
	::onFrame(_viewer.get() , float(dt) / 1000.0f);
	_viewer->frame();
	
    context_.SwapBuffers(
        callback_factory_.NewCallback(&Graphics3DInstance::MainLoop));
  }

  pp::CompletionCallbackFactory<Graphics3DInstance> callback_factory_;
  pp::Graphics3D context_;
  int32_t width_;
  int32_t height_;
  // GLuint frag_shader_;
  // GLuint vertex_shader_;
  // GLuint program_;
  // GLuint vertex_buffer_;
  // GLuint index_buffer_;
  // GLuint texture_;

  // GLuint texture_loc_;
  // GLuint position_loc_;
  // GLuint texcoord_loc_;
  // GLuint color_loc_;
  // GLuint mvp_loc_;

  // float x_angle_;
  // float y_angle_;
  // bool animating_;
  
};


class Graphics3DModule 
: public pp::Module 
{
 public:
  Graphics3DModule() : pp::Module() {}
  virtual ~Graphics3DModule() {}

  virtual pp::Instance* CreateInstance(PP_Instance instance) 
  {
    return new Graphics3DInstance(instance);
  }
};

namespace pp 
{
	Module* CreateModule() { return new Graphics3DModule(); }
}   // namespace pp
