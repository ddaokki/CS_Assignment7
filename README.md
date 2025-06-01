💡 Computer Graphics HW7: OpenGL Phong Shading

🛠️ Compilation & Run Instructions
Open the solution file (OpenGLPhongShader.sln) in Visual Studio 2019 or later.

Build the project (Ctrl + Shift + B)

Run the executable (F5)

Make sure the following dependencies are linked:

GLEW

GLFW

GLM

Place the shader files Phong.vert, Phong.frag in the output directory (./x64/Debug/)

📦 Dependencies
Library	Usage
GLEW	OpenGL extension loading
GLFW	Context & window management
GLM	Matrix and vector math


🖼️ Output
Result: smooth green Phong-shaded sphere with correct specular highlights and depth
Matches HW6-Q3 shading output

HW6 CPU Result	HW7 GPU Result

📁 File Structure

├── main_Phong_Shader.cpp
├── Phong.vert
├── Phong.frag
├── hw7_output.png
├── include/
│   └── glm/, glew/, glfw/
└── lib/
    └── glew32.lib, glfw3.lib, opengl32.lib
📌 Notes
Shader constants (lighting, material) are set in main.cpp and passed as uniforms.

No deprecated glBegin/glVertex or fixed-function lighting used.

Final image resolution: 512×512
