#include <Assets/BGShader.h>

#include <SDL2/SDL.h>

#include <string>
#include <streambuf>
#include <fstream>

#include <Rendering/ScreenQuad.h>

using std::string;

namespace Assets {

// Initialize vertex shader; only run once at the start of the program
const char* vertexShaderSource = R"(
    #version 330 core
    layout(location = 0) in vec3 a_Pos;
    
    out vec4 v_Pos;
    
    void main() {
    
        gl_Position = vec4(a_Pos, 1);
    
    };
)";
static GLuint vertexShader;
void BGShader::Init(){
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, 0);
	glCompileShader(vertexShader);

    int success = 0;
	char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
	}
}

// Load new fragment shader
BGShader* BGShader::Load(const std::filesystem::path& path){
	using BufIt = std::istreambuf_iterator<char>;
	std::ifstream in(path);
	string fragmentSourceString = string(BufIt(in), BufIt());
	const char* fragmentSource = fragmentSourceString.c_str();
	int success = 0;
	char infoLog[512];

	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, 0);
	glCompileShader(fragmentShader);
  	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		SDL_Log("%s", infoLog);

        return nullptr;
	}

	int id = glCreateProgram();
	glAttachShader(id, vertexShader);
	glAttachShader(id, fragmentShader);
	glLinkProgram(id);
	glDeleteShader(fragmentShader);


	BGShader* newShader = new BGShader;
	newShader->id = id;
	newShader->name = path.stem();

    return newShader;
}

void BGShader::use() {
	glUseProgram(id);
}

void BGShader::draw(float* res,float time) {
	use();
	glUniform2fv(glGetUniformLocation(id, "iResolution"), 1, res);
	glUniform1f(glGetUniformLocation(id, "iTime"), time);
	ScreenQuad::Draw();
}

}
