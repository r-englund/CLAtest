#ifndef IVW_CANVASGLUT_H
#define IVW_CANVASGLUT_H

#include "modules/glut/glutmoduledefine.h"
#include <stdlib.h>
#include "modules/opengl/inviwoopengl.h"
#include <GL/glut.h>
#include "modules/opengl/canvasgl.h"

namespace inviwo {

class IVW_MODULE_GLUT_API CanvasGLUT : public CanvasGL {
public:
    static EventHandler* eventHandler_;

    CanvasGLUT(std::string title, ivec2 dimensions);
    ~CanvasGLUT();

    virtual void initialize();
    virtual void deinitialize();

    static void display(void);
    static void idle(void);
    static void reshape(int width, int height);

    static MouseEvent::MouseButton mapMouseButton(const int mouseButtonGLUT);
    static MouseEvent::MouseState mapMouseState(const int mouseStateGLUT);
    static Event::Modifier mapModifiers(const int modifiersGLUT);

    static void keyboard(unsigned char key, int x, int y);
    static void keyboardSpecial(int key, int x, int y);
    static void mouse(int button, int state, int x, int y);
    static void mouseMotion(int x, int y);

private:
    std::string windowTitle_;
    unsigned int canvasID_;

    MouseEvent::MouseButton mouseButton_;
    MouseEvent::MouseState mouseState_;
    Event::Modifier mouseModifiers_;

    // these members are used to deal with the static
    // callback functions available through GLUT
    static unsigned int canvasCount_;
    static CanvasGLUT* canvases_[];
};

} // namespace

#endif // IVW_CANVASGLUT_H
