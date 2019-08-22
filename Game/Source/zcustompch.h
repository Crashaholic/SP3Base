#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <memory>
#include <functional>
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <algorithm>

#include <gl/glew.h>
#include <GLFW/glfw3.h>

#include <Vector3.h>
#include <Mtx44.h>
#include <MyMath.h>
#include <Logging.h>
#include <SingletonTemplate.h>
#include <timer.h>
#include <MatrixStack.h>
#include <Collider/Collider.h>

#include "Vertex.h"
#include "../SoundEngine/SoundEngine.h"
#include "Utility.h"
#include "Light.h"
#include "Material.h"
#include "Mesh.h"
#include "MeshBuilder.h"
#include "shader.hpp"
#include "LoadOBJ.h"
#include "LoadTGA.h"
#include "Camera.h"
