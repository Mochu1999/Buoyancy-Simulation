#pragma once


using namespace std;


struct ShaderProgramSource {
	std::string VertexSource;
	std::string FragmentSource;
};

struct Shader {

	std::string filePath;
	unsigned int ID;


	Shader(const std::string& filepath)
		:filePath(filepath), ID(0) { //we are maintaining filePath only for debugging purpouses
		ShaderProgramSource source = parseShader(filepath);
		ID = createShader(source.VertexSource, source.FragmentSource);

		
	}

	ShaderProgramSource parseShader(const string& filepath) { //Converts a .shader into 2 separate streams and each one into strings for glShaderSource in compileShader
		ifstream stream(filepath);

		enum class ShaderType {
			NONE = -1, VERTEX = 0, FRAGMENT = 1
		};

		string line;
		stringstream ss[2];
		ShaderType type = ShaderType::NONE;
		while (getline(stream, line)) {
			if (line.find("#shader") != std::string::npos) { //npos is the constant that will be given if it was not found, otherwise it will give a position, not a bool
				if (line.find("vertex") != std::string::npos)
					type = ShaderType::VERTEX;
				else if (line.find("fragment") != string::npos)
					type = ShaderType::FRAGMENT;

			}
			else { //what isn't in #shader line is included in the stream
				ss[(int)type] << line << '\n'; //the enum positionates vertex in 0 and fr in 1
			}
		}

		return { ss[0].str(),ss[1].str() }; //str converts streams into strings

	}


	unsigned int compileShader(unsigned int type, const string& source) {
		unsigned int id = glCreateShader(type); //you specifies type of shader to create

		const char* src = &source[0];//you will use this pointer to point to the start of your source, char bc string is in chars, equivalent to const char* src = source.c_str()

		glShaderSource(id, 1, &src, nullptr); //Sets the source code in a shader object id 
		//[1] number of elements in the string 
		//[3] if I had an array of source code, this would set the lenght of it to avoid mixing it with other things, I understand 
		glCompileShader(id);

		//Error handling from sintax
		int result;
		glGetShaderiv(id, GL_COMPILE_STATUS, &result); //if status of id is successful it will give TRUE
		if (!result) {
			int length;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length); //Gets the length from the error
			//char* message = (char*)alloca(length * sizeof(char)); 
			GLchar message[1024]; //from a example, i think it sets the size of the message, validate
			glGetShaderInfoLog(id, length, &length, message);
			cout << "Failed to compile" << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << endl;
			cout << message << endl;
			glDeleteShader(id);
			return 0;
		}

		return id;
	}

	unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader) { 	//creates shader with their string
		unsigned int program = glCreateProgram();// A program object is an object to which shader objects can be attached   and returns a non-zero value by which it can be referenced (unless error, then 0)


		unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
		compileErrors(vs, "VERTEX SHADER");
		unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);
		compileErrors(fs, "VERTEX SHADER");


		//linking them:
		glAttachShader(program, vs);
		glAttachShader(program, fs);
		glLinkProgram(program);
		glValidateProgram(program);// This validates the program to ensure that the shaders are compatible with the current OpenGL context
		glDeleteShader(vs); // deletes the shader, they are occupying memory with intermediate files
		glDeleteShader(fs);
		return program;
	}
	~Shader() {
		glDeleteProgram(ID);
	}
	void Bind() const {
		glUseProgram(ID);
	}

	void Unbind() const {
		glUseProgram(0);
	}


	// Checks if the different Shaders have compiled properly
	void compileErrors(unsigned int shader, const char* type)
	{
		// Stores status of compilation
		GLint hasCompiled;
		// Character array to store error message in
		char infoLog[1024];
		if (type != "PROGRAM")
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
			if (hasCompiled == GL_FALSE)
			{
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "SHADER_COMPILATION_ERROR for:" << type << "\n" << infoLog << std::endl;
			}
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &hasCompiled);
			if (hasCompiled == GL_FALSE)
			{
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "SHADER_LINKING_ERROR for:" << type << "\n" << infoLog << std::endl;
			}
		}
	}
};




GLFWwindow* initialize() {
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/*glfwWindowHint(GLFW_DEPTH_BITS, 32);

	glfwWindowHint(GLFW_SAMPLES, 4);*/
	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "Buoyancy Simulation 3D", NULL, NULL);

	//glEnable(GL_MULTISAMPLE);

	glfwMakeContextCurrent(window);
	//glewExperimental = GL_TRUE;
	glewInit();
	glfwSwapInterval(1);

	////depth buffering
	glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LESS);


	glEnable(GL_BLEND);//blend for alpha opacity, lets blending pixels in the same position
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);



	////// Enable polygon offset fill
	////glEnable(GL_POLYGON_OFFSET_FILL);
	////// Apply the offset. These values might need tweaking for your specific use case
	////glPolygonOffset(1.0f, 1.0f);

	return window;
}





//#define CHECK_GL_ERROR() {\
//    GLenum err = glGetError();\
//    while (err != GL_NO_ERROR) {\
//        std::cout << "OpenGL error: " << std::hex << err << std::dec << " line:" << __LINE__ << std::endl;\
//        err = glGetError();\
//    }\
//}
//std::set<std::string> printedErrors;
//
//void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) { //Only adds errors once
//	std::string errorMsg(message);
//
//	if (printedErrors.find(errorMsg) == printedErrors.end()) {
//		std::cerr << message << std::endl;
//
//		printedErrors.insert(errorMsg);
//	}
//}