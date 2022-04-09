#include "Mesh.h"

renderer::Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices)
{
	this->vertices_ = vertices;
	this->indices_ = indices;
	
	SetupMesh();
}

void renderer::Mesh::Draw(GLuint program_id, bool wire_mode)
{
    //std::cout << "drawing" << std::endl;

    glBindVertexArray(vao_);
    if (wire_mode)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    }
    glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);


}

void renderer::Mesh::SetupMesh()
{

    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);
    glGenBuffers(1, &ebo_);

    glBindVertexArray(vao_);
    // load data into vertex buffers
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    // A great thing about structs is that their memory layout is sequential for all its items.
    // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
    // again translates to 3/2 floats which translates to a byte array.
    glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(Vertex), &vertices_[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(unsigned int), &indices_[0], GL_STATIC_DRAW);

    // set the vertex attribute pointers
    // vertex Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex normal
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

    glBindVertexArray(0);

}
