#pragma once

#include<iostream>
#include<string>
#include<vector>
#include<list>
#include<map>
#include<fstream>
#include<filesystem>
#include<regex>

// OpenGL
#include"GL/glew.h"
#include"GLFW/glfw3.h"

// GLM
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/string_cast.hpp"
#include "glm/gtx/quaternion.hpp"

// STB Image
#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include"stb_image.h"

// ASSIMP 
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// Local Include
#include"Util.h"
#include"InputHandler.h"
#include"InputDelegator.h"
#include"Light.h"