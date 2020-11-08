#include "IOpenGLDrawable.h"
#include "ArrayRRT.h"
#include "includeGlew.hpp"
#include <GL/gl.h>
#include "shader.hpp"

class RenderArrayRRT
{
public:
	ArrayRRT rrt;
	std::size_t pathBufferSize{0};
	GLuint vertexBuffer;
	GLuint edgesIndicesBuffer;
	GLuint vertexArray;
	GLuint pathBuffer;
	GLuint pathArray;
	GLuint program;

public:
	// Odziedzicz wszystkie konstruktory
	RenderArrayRRT(const ArrayRRT& rrt);
	RenderArrayRRT(ArrayRRT&& rrt);
	RenderArrayRRT(const RenderArrayRRT& other) = default;
	RenderArrayRRT(RenderArrayRRT&& other) = default;
	RenderArrayRRT& operator=(const RenderArrayRRT& other) = default;
	RenderArrayRRT& operator=(RenderArrayRRT&& other) = default;
	virtual ~RenderArrayRRT();

	void initBuffers() noexcept;
	void addPoint() noexcept;
	std::size_t size() const noexcept;
	void draw(const GLfloat* transform) const noexcept;
	void updatePathBuffer(std::forward_list<std::size_t> list) noexcept;
};
