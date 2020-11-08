#include "RenderArrayRRT.h"

RenderArrayRRT::RenderArrayRRT(const ArrayRRT& rrt) : rrt(rrt) {}
RenderArrayRRT::RenderArrayRRT(ArrayRRT&& rrt) : rrt(rrt) {}

RenderArrayRRT::~RenderArrayRRT()
{
	glDeleteProgram(program);
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &edgesIndicesBuffer);
	glDeleteBuffers(1, &pathBuffer);
	glDeleteVertexArrays(1, &vertexArray);
	glDeleteVertexArrays(1, &pathArray);
}

void RenderArrayRRT::initBuffers() noexcept
{
	// Bufory
	glCreateBuffers(1, &vertexBuffer);
	glNamedBufferData(vertexBuffer, rrt.getPoints().size() * sizeof(glm::vec2), rrt.getPoints().data(), GL_STATIC_DRAW);
	glCreateBuffers(1, &edgesIndicesBuffer);
	glNamedBufferData(edgesIndicesBuffer, rrt.getEdges().size() * sizeof(glm::uvec2), rrt.getEdges().data(), GL_STATIC_DRAW);
	// VAO
	glCreateVertexArrays(1, &vertexArray);
	glVertexArrayAttribFormat(vertexArray, 1, 2, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayVertexBuffer(vertexArray, 1, vertexBuffer, 0, sizeof(glm::vec2));
	glEnableVertexArrayAttrib(vertexArray, 1);
	glVertexArrayElementBuffer(vertexArray, edgesIndicesBuffer);
	// VAO i bufor na ścieżkę między wskazanymi punktami
	glCreateBuffers(1, &pathBuffer);
	glCreateVertexArrays(1, &pathArray);
	glVertexArrayAttribFormat(pathArray, 1, 2, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayVertexBuffer(pathArray, 1, vertexBuffer, 0, sizeof(glm::vec2));
	glEnableVertexArrayAttrib(pathArray, 1);
	glVertexArrayElementBuffer(pathArray, pathBuffer);
	// Shadery
	program = glCreateProgram();
	glAttachShader(program, loadShaderFromFile(GL_VERTEX_SHADER, "shader.vert"));
	glAttachShader(program, loadShaderFromFile(GL_FRAGMENT_SHADER, "shader.frag"));
	LinkValidateProgram(program);
}

void RenderArrayRRT::addPoint() noexcept
{
	rrt.addPoint();
}

std::size_t RenderArrayRRT::size() const noexcept
{
	return rrt.size();
}

void RenderArrayRRT::draw(const GLfloat* transform) const noexcept
{
	glPointSize(2);
	glLineWidth(1);
	glBindVertexArray(vertexArray);
	glUseProgram(program);
	glUniformMatrix4fv(0, 1, false, transform);
	glUniform4f(2, 0.5, 0.7, 0.9, 1.0);
	glDrawElements(GL_LINES, rrt.getEdges().size() * sizeof(glm::uvec2), GL_UNSIGNED_INT, nullptr);
	glUniform4f(2, 0.3, 1.0, 0.4, 1.0);
	glDrawArrays(GL_POINTS, 0, rrt.getPoints().size());
	// Ścieżka
	glLineWidth(2);
	glBindVertexArray(pathArray);
	glUniform4f(2, 1.0, 0.4, 0.4, 1.0);
	glDrawElements(GL_LINE_STRIP, pathBufferSize, GL_UNSIGNED_INT, nullptr);
	glUseProgram(0);
	glBindVertexArray(0);
}

void RenderArrayRRT::updatePathBuffer(std::forward_list<std::size_t> list) noexcept
{
	std::vector<std::uint32_t> data(std::cbegin(list), std::cend(list));
	glNamedBufferData(pathBuffer, data.size() * sizeof(std::uint32_t), data.data(), GL_STATIC_DRAW);
	pathBufferSize = data.size();
}
