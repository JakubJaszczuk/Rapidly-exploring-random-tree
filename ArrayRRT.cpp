#include "ArrayRRT.h"

ArrayRRT::ArrayRRT(bool random) noexcept
{
	if(random)
	{
		points.push_back(generateRandomPoint());
	}
	else
	{
		points.push_back(glm::vec2(0.5, 0.5));
	}
}

ArrayRRT::ArrayRRT(double maxDistance, bool random) noexcept : maxDistance(maxDistance)
{
	if(random)
	{
		points.push_back(generateRandomPoint());
	}
	else
	{
		points.push_back(glm::vec2(0.5, 0.5));
	}
}

ArrayRRT::ArrayRRT(double x, double y) noexcept
{
	points.push_back(glm::vec2(x, y));
}

ArrayRRT::ArrayRRT(double maxDistance, double x, double y) noexcept : maxDistance(maxDistance)
{
	points.push_back(glm::vec2(x, y));
}

bool ArrayRRT::lineSegmentIntersection(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3, const glm::vec2& p4) const noexcept
{
	auto d1 = (p3.x - p1.x) * (p2.y - p1.y) - (p3.y - p1.y) * (p2.x - p1.x);
	auto d2 = (p4.x - p1.x) * (p2.y - p1.y) - (p4.y - p1.y) * (p2.x - p1.x);
	if((d1 <= 0 && d2 <= 0) || (d1 >= 0 && d2 >= 0))
	{
		return false;
	}
	d1 = (p1.x - p3.x) * (p4.y - p3.y) - (p1.y - p3.y) * (p4.x - p3.x);
	d2 = (p2.x - p3.x) * (p4.y - p3.y) - (p2.y - p3.y) * (p4.x - p3.x);
	if((d1 < 0 && d2 <= 0) || (d1 > 0 && d2 >= 0))
	{
		return false;
	}
	return true;
}

std::size_t ArrayRRT::findClosestPoint(const glm::vec2& point) const noexcept
{
	std::size_t result = 0;
	double currentDistance = std::numeric_limits<double>::infinity();
	double distance;
	for(size_t i = 0; i < points.size(); ++i)
	{
		distance = glm::distance(point, points[i]);
		if(distance < currentDistance)
		{
			currentDistance = distance;
			result = i;
		}
	}
	return result;
}

glm::vec2 ArrayRRT::generateRandomPoint() const noexcept
{
	glm::vec2 p;
	std::random_device rd;
	std::knuth_b random(rd());
	std::uniform_real_distribution<> distribution(0.0, 1.0);
	double x = distribution(random);
	double y = distribution(random);
	return glm::vec2(x, y);
}

void ArrayRRT::addPoint() noexcept
{
	// Generuj losowy punkt
	auto point = generateRandomPoint();
	// Znajdź najbliższy punkt należący do drzewa
	auto closestPoint = findClosestPoint(point);
	// Zakończ jeżeli punkt jest za daleko
	if(glm::distance(point, points[closestPoint]) > maxDistance) return;
	// Sprawdż czy krawędż jest możliwa do utworzenia
	bool intersection = false;
	for(size_t i = 0; i < edges.size() && intersection == false; ++i)
	{
		intersection = lineSegmentIntersection(point, points[closestPoint], points[edges[i].x], points[edges[i].y]);
	}
	if(!intersection)
	{
		points.push_back(point);
		edges.push_back(glm::ivec2(points.size()-1, closestPoint));
	}
}

std::size_t ArrayRRT::size() const noexcept
{
	return points.size();
}

std::unordered_set<std::size_t> ArrayRRT::pointNeighbours(std::size_t index) const noexcept
{
	std::unordered_set<std::size_t> s;
	// Iteruj po wszystkich krwędziach
	for(const auto& e : edges)
	{
		// Daj ten drugi indeks punktu
		if(e[0] == index)
		{
			s.insert(e[1]);
		}
		if(e[1] == index)
		{
			s.insert(e[0]);
		}
	}
	return s;
}

std::forward_list<std::size_t> ArrayRRT::pathBetweenPoints(std::size_t from, std::size_t to) const noexcept
{
	// Kolejny punkt i ścieżka dotychczasowa
	using top_t = std::pair<std::size_t, std::forward_list<std::size_t>>;
	std::stack<top_t> stack;
	stack.push(top_t(from, std::forward_list<std::size_t>()));
	while(!stack.empty()){
		auto [top, path] = stack.top();
		stack.pop();
		// Jeżeli znaleziono to zwróć ścieżkę
		if(top == to)
		{
			path.push_front(top);
			return path;
		}
		// Jeżeli nie to dodaj nieodwiedzonych sąsiadów do stosu
		else
		{
			auto neighbours = pointNeighbours(top);
			for(auto point : neighbours)
			{
				// Zabezpieczenie przed "undefined behavior" i pobraniem już odwiedzonego
				if(path.empty() || point != path.front())
				{
					std::forward_list<std::size_t> temp(path);
					temp.push_front(top);
					stack.push(top_t(point, temp));
				}
			}
		}
	}
	return std::forward_list<std::size_t>();
}
