#include "OBJLoader.h"
#include "Utils.h"
#include <fstream>
#include <iostream>
#include <algorithm>

static bool CompareOBJIndexPtr(const OBJIndex* a, const OBJIndex* b);
static inline unsigned int FindNextChar(unsigned int start, const char* str, unsigned int length, char token);
static inline unsigned int ParseOBJIndexValue(const std::string& token, unsigned int start, unsigned int end);
static inline float ParseOBJFloatValue(const std::string& token, unsigned int start, unsigned int end);
static inline std::vector<std::string> SplitString(const std::string& s, char delim);

OBJModel::OBJModel(const std::string& fileName)
{
	std::map<std::string, Material*> materials;
	Material* inUse = new Material();
	hasUVs = false;
	hasNormals = false;
	std::ifstream file;
	file.open(fileName.c_str());
	this->_objContent = GetFileContent(fileName);

	for (auto& line : SplitString(this->_objContent, '\n'))
	{
		unsigned int lineLength = (unsigned int)line.length();

		if (lineLength < 2)
			continue;

		const char* lineCStr = line.c_str();

		switch (lineCStr[0])
		{
		case 'v':
			if (lineCStr[1] == 't')
				this->uvs.push_back(ParseOBJVec2(line));
			else if (lineCStr[1] == 'n')
				this->normals.push_back(ParseOBJVec3(line));
			else if (lineCStr[1] == ' ' || lineCStr[1] == '\t')
				this->vertices.push_back(ParseOBJVec3(line));
			break;
		case 'f':
			CreateOBJFace(line, inUse);
			break;
		default: break;
		};

		if (strncmp(lineCStr, "mtllib", strlen("mtllib")) == 0)
		{
			std::string mtl = SplitString(lineCStr, ' ')[1];

			materials = MapMaterialsFromFile(mtl);
		}

		if (strncmp(lineCStr, "usemtl", strlen("usemtl")) == 0 && materials.size() > 0)
		{
			std::string mat = SplitString(lineCStr, ' ')[1];

			if (Utils::isInMap(materials, mat))
				inUse = materials[mat];
		}
	}
}

void IndexedModel::CalcNormals()
{
	for (int i = 0; i < indices.size(); i += 3)
	{
		int i0 = indices[i];
		int i1 = indices[i + 1];
		int i2 = indices[i + 2];

		glm::vec3 v1 = positions[i1] - positions[i0];
		glm::vec3 v2 = positions[i2] - positions[i0];

		glm::vec3 normal = glm::normalize(glm::cross(v1, v2));

		normals[i0] += normal;
		normals[i1] += normal;
		normals[i2] += normal;
	}

	for (unsigned int i = 0; i < positions.size(); i++)
		normals[i] = glm::normalize(normals[i]);
}

IndexedModel OBJModel::ToIndexedModel()
{
	IndexedModel result;
	IndexedModel normalModel;

	unsigned int numIndices = (unsigned int) OBJIndices.size();

	std::vector<OBJIndex*> indexLookup;

	for (unsigned int i = 0; i < numIndices; i++)
		indexLookup.push_back(&OBJIndices[i]);

	std::sort(indexLookup.begin(), indexLookup.end(), CompareOBJIndexPtr);

	std::map<OBJIndex, unsigned int> normalModelIndexMap;
	std::map<unsigned int, unsigned int> indexMap;

	for (unsigned int i = 0; i < numIndices; i++)
	{
		OBJIndex* currentIndex = &OBJIndices[i];

		glm::vec3 currentPosition = vertices[currentIndex->vertexIndex];
		glm::vec2 currentTexCoord;
		glm::vec3 currentNormal;
		glm::vec4 currentColor = glm::vec4(currentIndex->material->ambient.x,
											currentIndex->material->ambient.y,
											currentIndex->material->ambient.z,
											currentIndex->material->density);

		if (hasUVs)
			currentTexCoord = uvs[currentIndex->uvIndex];
		else
			currentTexCoord = glm::vec2(0, 0);

		if (hasNormals)
			currentNormal = normals[currentIndex->normalIndex];
		else
			currentNormal = glm::vec3(0, 0, 0);

		unsigned int normalModelIndex;
		unsigned int resultModelIndex;

		//Create model to properly generate normals on
		std::map<OBJIndex, unsigned int>::iterator it = normalModelIndexMap.find(*currentIndex);
		if (it == normalModelIndexMap.end())
		{
			normalModelIndex = (unsigned int) normalModel.positions.size();

			normalModelIndexMap.insert(std::pair<OBJIndex, unsigned int>(*currentIndex, normalModelIndex));
			normalModel.positions.push_back(currentPosition);
			normalModel.texCoords.push_back(currentTexCoord);
			normalModel.normals.push_back(currentNormal);
			normalModel.colors.push_back(currentColor);
		}
		else
			normalModelIndex = it->second;

		//Create model which properly separates texture coordinates
		unsigned int previousVertexLocation = FindLastVertexIndex(indexLookup, currentIndex, result);

		if (previousVertexLocation == (unsigned int)-1)
		{
			resultModelIndex = (unsigned int) result.positions.size();

			result.positions.push_back(currentPosition);
			result.texCoords.push_back(currentTexCoord);
			result.normals.push_back(currentNormal);
			result.colors.push_back(currentColor);
		}
		else
			resultModelIndex = previousVertexLocation;

		normalModel.indices.push_back(normalModelIndex);
		result.indices.push_back(resultModelIndex);
		indexMap.insert(std::pair<unsigned int, unsigned int>(resultModelIndex, normalModelIndex));
	}

	if (!hasNormals)
	{
		normalModel.CalcNormals();

		for (unsigned int i = 0; i < result.positions.size(); i++)
			result.normals[i] = normalModel.normals[indexMap[i]];
	}

	return result;
};

std::string OBJModel::GetFileContent(const std::string& file)
{
	std::string vs_text;
	std::ifstream vs_file(file);

	std::string vs_line;
	if (vs_file.is_open())
	{
		while (getline(vs_file, vs_line))
		{
			vs_text += vs_line;
			vs_text += '\n';
		}
		vs_file.close();
	}
	else
	{
		std::cerr << "Failed to open " << file << std::endl;
	}
	return vs_text;
}

unsigned int OBJModel::FindLastVertexIndex(const std::vector<OBJIndex*>& indexLookup, const OBJIndex* currentIndex, const IndexedModel& result)
{
	unsigned int start = 0;
	unsigned int end = (unsigned int)indexLookup.size();
	unsigned int current = (end - start) / 2 + start;
	unsigned int previous = start;

	while (current != previous)
	{
		OBJIndex* testIndex = indexLookup[current];

		if (testIndex->vertexIndex == currentIndex->vertexIndex)
		{
			unsigned int countStart = current;

			for (unsigned int i = 0; i < current; i++)
			{
				OBJIndex* possibleIndex = indexLookup[current - i];

				if (possibleIndex == currentIndex)
					continue;

				if (possibleIndex->vertexIndex != currentIndex->vertexIndex)
					break;

				countStart--;
			}

			for (unsigned int i = countStart; i < indexLookup.size() - countStart; i++)
			{
				OBJIndex* possibleIndex = indexLookup[current + i];

				if (possibleIndex == currentIndex)
					continue;

				if (possibleIndex->vertexIndex != currentIndex->vertexIndex)
					break;
				else if ((!hasUVs || possibleIndex->uvIndex == currentIndex->uvIndex)
					&& (!hasNormals || possibleIndex->normalIndex == currentIndex->normalIndex))
				{
					glm::vec3 currentPosition = vertices[currentIndex->vertexIndex];
					glm::vec2 currentTexCoord;
					glm::vec3 currentNormal;

					if (hasUVs)
						currentTexCoord = uvs[currentIndex->uvIndex];
					else
						currentTexCoord = glm::vec2(0, 0);

					if (hasNormals)
						currentNormal = normals[currentIndex->normalIndex];
					else
						currentNormal = glm::vec3(0, 0, 0);

					for (unsigned int j = 0; j < result.positions.size(); j++)
					{
						if (currentPosition == result.positions[j]
							&& ((!hasUVs || currentTexCoord == result.texCoords[j])
								&& (!hasNormals || currentNormal == result.normals[j])))
						{
							return j;
						}
					}
				}
			}

			return -1;
		}
		else
		{
			if (testIndex->vertexIndex < currentIndex->vertexIndex)
				start = current;
			else
				end = current;
		}

		previous = current;
		current = (end - start) / 2 + start;
	}

	return -1;
}

void OBJModel::CreateOBJFace(const std::string& line, Material *material)
{
	std::vector<std::string> tokens = SplitString(line, ' ');

	this->OBJIndices.push_back(ParseOBJIndex(tokens[1], &this->hasUVs, &this->hasNormals, material));
	this->OBJIndices.push_back(ParseOBJIndex(tokens[2], &this->hasUVs, &this->hasNormals, material));
	this->OBJIndices.push_back(ParseOBJIndex(tokens[3], &this->hasUVs, &this->hasNormals, material));

	if ((int)tokens.size() > 4)
	{
		this->OBJIndices.push_back(ParseOBJIndex(tokens[1], &this->hasUVs, &this->hasNormals, material));
		this->OBJIndices.push_back(ParseOBJIndex(tokens[3], &this->hasUVs, &this->hasNormals, material));
		this->OBJIndices.push_back(ParseOBJIndex(tokens[4], &this->hasUVs, &this->hasNormals, material));
	}
}

std::map<std::string, Material*> OBJModel::MapMaterialsFromFile(const std::string& file)
{
	std::map<std::string, Material*> result;
	std::ifstream fileStream;
	std::string line;
	Material *current = new Material();
	bool waitNext = false;

	fileStream.open(file.c_str());

	if (fileStream.is_open())
	{
		while (fileStream.good())
		{
			getline(fileStream, line); 
			if (line.length() < 2)
				continue;
			std::vector<std::string> words = SplitString(line, ' ');

			if (words[0] == "newmtl")
			{
				if (this->_objContent.find(words[1]) == std::string::npos) {
					waitNext = true;
					continue;
				}
				waitNext = false;
				if (current->density == 0)
					current->density = 1;
				current = new Material();
				result.insert(std::pair<std::string, Material*>(words[1], current));
			}
			else if (words[0] == "Kd" && !waitNext)
				current->ambient = ParseOBJVec3(line);
			else if (words[0] == "Ka" && !waitNext)
				current->diffuse = ParseOBJVec3(line);
			else if (words[0] == "Ks" && !waitNext)
				current->specular = ParseOBJVec3(line);
			else if (words[0] == "Ns" && !waitNext)
				current->highlights = std::stof(words[1]);
			else if (words[0] == "Ni" && !waitNext)
				current->density = std::stof(words[1]);
			else if (words[0] == "d" && !waitNext)
				current->dissolve = std::stof(words[1]);
		}
	}
	else
	{
		std::cerr << "Failed to open " << file << std::endl;
	}
	if (current->density == 0)
		current->density = 1;
	return result;
}

OBJIndex OBJModel::ParseOBJIndex(const std::string& token, bool* hasUVs, bool* hasNormals, Material *material)
{
	unsigned int tokenLength = (unsigned int) token.length();
	const char* tokenString = token.c_str();

	unsigned int vertIndexStart = 0;
	unsigned int vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, '/');

	OBJIndex result;
	result.vertexIndex = ParseOBJIndexValue(token, vertIndexStart, vertIndexEnd);
	result.uvIndex = 0;
	result.normalIndex = 0;
	result.material = material;

	if (vertIndexEnd >= tokenLength)
		return result;

	vertIndexStart = vertIndexEnd + 1;
	vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, '/');

	result.uvIndex = ParseOBJIndexValue(token, vertIndexStart, vertIndexEnd);
	*hasUVs = true;

	if (vertIndexEnd >= tokenLength)
		return result;

	vertIndexStart = vertIndexEnd + 1;
	vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, '/');

	result.normalIndex = ParseOBJIndexValue(token, vertIndexStart, vertIndexEnd);
	*hasNormals = true;

	return result;
}

glm::vec3 OBJModel::ParseOBJVec3(const std::string& line)
{
	unsigned int tokenLength = (unsigned int) line.length();
	const char* tokenString = line.c_str();

	unsigned int vertIndexStart = 2;

	while (vertIndexStart < tokenLength)
	{
		if (tokenString[vertIndexStart] != ' ')
			break;
		vertIndexStart++;
	}

	unsigned int vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, ' ');

	float x = ParseOBJFloatValue(line, vertIndexStart, vertIndexEnd);

	vertIndexStart = vertIndexEnd + 1;
	vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, ' ');

	float y = ParseOBJFloatValue(line, vertIndexStart, vertIndexEnd);

	vertIndexStart = vertIndexEnd + 1;
	vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, ' ');

	float z = ParseOBJFloatValue(line, vertIndexStart, vertIndexEnd);

	return glm::vec3(x, y, z);

	//glm::vec3(atof(tokens[1].c_str()), atof(tokens[2].c_str()), atof(tokens[3].c_str()))
}

glm::vec2 OBJModel::ParseOBJVec2(const std::string& line)
{
	unsigned int tokenLength = (unsigned int) line.length();
	const char* tokenString = line.c_str();

	unsigned int vertIndexStart = 3;

	while (vertIndexStart < tokenLength)
	{
		if (tokenString[vertIndexStart] != ' ')
			break;
		vertIndexStart++;
	}

	unsigned int vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, ' ');

	float x = ParseOBJFloatValue(line, vertIndexStart, vertIndexEnd);

	vertIndexStart = vertIndexEnd + 1;
	vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, ' ');

	float y = ParseOBJFloatValue(line, vertIndexStart, vertIndexEnd);

	return glm::vec2(x, y);
}

static bool CompareOBJIndexPtr(const OBJIndex* a, const OBJIndex* b)
{
	return a->vertexIndex < b->vertexIndex;
}

static inline unsigned int FindNextChar(unsigned int start, const char* str, unsigned int length, char token)
{
	unsigned int result = start;
	while (result < length)
	{
		result++;
		if (str[result] == token)
			break;
	}

	return result;
}

static inline unsigned int ParseOBJIndexValue(const std::string& token, unsigned int start, unsigned int end)
{
	return atoi(token.substr(start, end - start).c_str()) - 1;
}

static inline float ParseOBJFloatValue(const std::string& token, unsigned int start, unsigned int end)
{
	return (float) (atof(token.substr(start, end - start).c_str()));
}

static inline std::vector<std::string> SplitString(const std::string& s, char delim)
{
	std::vector<std::string> elems;

	const char* cstr = s.c_str();
	unsigned int strLength = (unsigned int) s.length();
	unsigned int start = 0;
	unsigned int end = 0;

	while (end <= strLength)
	{
		while (end <= strLength)
		{
			if (cstr[end] == delim)
				break;
			end++;
		}

		elems.push_back(s.substr(start, end - start));
		start = end + 1;
		end = start;
	}

	return elems;
}