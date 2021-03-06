cmake_minimum_required(VERSION 3.16)
project(BezierCurves)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)
include_directories(include/GL)
include_directories(src)
include_directories(src/Base)
include_directories(src/Canvas)
include_directories(src/Label)
include_directories(src/Managers)
include_directories(src/Panel)
include_directories(src/Vectors)

add_executable(BezierCurves
        include/GL/freeglut.h
        include/GL/freeglut_ext.h
        include/GL/freeglut_std.h
        include/GL/glut.h
        src/Base/CanvasObject.cpp
        src/Base/CanvasObject.h
        src/Base/IKeyboard.h
        src/Base/IMouse.h
        src/Base/IRender.h
        src/Base/ITransformable.h
        src/Canvas/gl_canvas2d.cpp
        src/Canvas/gl_canvas2d.h
        src/Managers/GlobalManager.cpp
        src/Managers/GlobalManager.h
        src/Panel/Panel.cpp
        src/Panel/Panel.h
        src/Vectors/Float2.cpp
        src/Vectors/Float2.h
        src/Vectors/Float3.cpp
        src/Vectors/Float3.h
        src/Vectors/Float4.cpp
        src/Vectors/Float4.h
        src/Curve/BezierCurve.cpp
        src/Curve/BezierCurve.h
        src/Curve/BezierCurveManager.cpp
        src/Curve/BezierCurveManager.h
        src/main.cpp
        src/Scene/Scene.cpp
        src/Scene/Scene.h
        src/Utilities.cpp
        src/Utilities.h
        src/ControlPoint/ControlPoint.cpp
        src/ControlPoint/ControlPoint.h
        src/ConvexHull/ConvexHull.cpp
        src/ConvexHull/ConvexHull.h
        src/Fatorial.cpp
        src/Fatorial.h
        src/BlendingFunctionsGraph/BlendingFunctionsGraph.cpp
        src/BlendingFunctionsGraph/BlendingFunctionsGraph.h
        src/Button/Button.cpp
        src/Button/Button.h)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})

target_link_libraries(BezierCurves ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})
