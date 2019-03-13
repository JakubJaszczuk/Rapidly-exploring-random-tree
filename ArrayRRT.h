#include <vector>
#include <unordered_set>
#include <forward_list>
#include <stack>
#include <glm/glm.hpp>
#include <limits>
#include <random>
#include <iostream>
#include "IRRT.h"
//#include <SFML/Graphics>

#ifndef ArrayRRT_H
#define ArrayRRT_H

class ArrayRRT : public IRRT
{
public:
	std::vector<glm::vec2> points;
	std::vector<glm::uvec2> edges;
	const double maxDistance = std::numeric_limits<double>::infinity();

	bool lineSegmentIntersection(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3, const glm::vec2& p4) const noexcept;
	std::size_t findClosestPoint(const glm::vec2& point) const noexcept;
	glm::vec2 generateRandomPoint() const noexcept;

public:
	ArrayRRT(bool random = false) noexcept;
	ArrayRRT(double maxDistance, bool random = false) noexcept;
	ArrayRRT(double x, double y) noexcept;
	ArrayRRT(double maxDistance, double x, double y) noexcept;
	~ArrayRRT() = default;

	auto& getPoints() const noexcept {return points;}
	auto& getEdges() const noexcept {return edges;}

	void addPoint() noexcept override;
	std::size_t size() const noexcept;

	std::unordered_set<std::size_t> pointNeighbours(std::size_t index) const noexcept;
	std::forward_list<std::size_t> pathBetweenPoints(std::size_t from, std::size_t to) const noexcept;
};

#endif
