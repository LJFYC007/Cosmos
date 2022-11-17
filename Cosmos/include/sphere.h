#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "mesh.h"

#include <vector>

class Sphere
{
public: 
    GLuint lats, lons; GLfloat r;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    Sphere(GLuint Lats = 100, GLuint Lons = 100, GLfloat R = 0.5) : lats(Lats), lons(Lons), r(R) { createSphere(); }

private:
    glm::vec2 getTexCoord(GLfloat u, GLfloat v) { return glm::vec2(v, u); }
    glm::vec3 getTanCoord(GLfloat u, GLfloat v) { return glm::vec3(cos(u) * sin(v), cos(u) * cos(v), -sin(u)); }
    glm::vec3 getBitanCoord(GLfloat u, GLfloat v) { return glm::vec3(sin(u) * cos(v), -cos(u) * sin(v), 0); }
    glm::vec3 getPoint(GLfloat u, GLfloat v) ;
    GLuint getID(GLuint lat, GLuint lon) { if(lon == lons) lon = 0; return lat * lons + lon; }
    void createSphere() ;
};

glm::vec3 Sphere::getPoint(GLfloat u, GLfloat v) {
	GLfloat pi = glm::pi<GLfloat>();
	GLfloat z = r * std::cos(pi * u);
	GLfloat x = r * std::sin(pi * u) * std::cos(2 * pi * v);
	GLfloat y = r * std::sin(pi * u) * std::sin(2 * pi * v);
    return glm::vec3(x, y, z);
}

void Sphere::createSphere() {
    GLfloat lon_step = 1.0f / lons;
    GLfloat lat_step = 1.0f / lats;
    GLuint offset = 0;

    for (unsigned int lat = 0; lat <= lats; ++lat)
        for (unsigned int lon = 0; lon < lons; ++lon) {
            Vertex Point;
            Point.Position = getPoint(lat * lat_step, lon * lon_step); 
            Point.Tangent = getTanCoord(lat * lat_step, lon * lon_step); 
            Point.Bitangent = getBitanCoord(lat * lat_step, lon * lon_step);
            Point.Normal = getPoint(lat * lat_step, lon * lon_step); 
            Point.TexCoords = getTexCoord(lat * lat_step, lon * lon_step);
            vertices.push_back(Point);
        }

	for (unsigned int lat = 0; lat < lats; ++lat)
        for (unsigned int lon = 0; lon < lons; ++lon) {
            if (lat != 0)
            {
				// First Triangle
				indices.push_back(getID(lat, lon));
				indices.push_back(getID(lat, lon + 1));
				indices.push_back(getID(lat + 1, lon + 1));
            }
            if (lat != lats - 1)
            { 
				// Second Triangle
				indices.push_back(getID(lat, lon));
				indices.push_back(getID(lat + 1, lon + 1));
				indices.push_back(getID(lat + 1, lon));
            }
        }
}
