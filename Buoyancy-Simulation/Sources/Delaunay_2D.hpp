#pragma once

#include "Common.hpp"


//std::vector<p> generateRandomPoints(int n) {
//
//	float minX = 000.0;
//	float maxX = 50.0;
//	float minY = 00.0;
//	float maxY = 50.0;
//
//
//	// Seed for random number generation
//	std::random_device rd;
//	std::mt19937 gen(rd());
//
//	// Distributions for x and y coordinates
//	std::uniform_real_distribution<> disX(minX, maxX);
//	std::uniform_real_distribution<> disY(minY, maxY);
//
//	std::vector<p> points;
//	points.reserve(n);
//
//	for (int i = 0; i < n; ++i) {
//		float x = disX(gen);
//		float y = disY(gen);
//		points.push_back({ x, y });
//	}
//
//	return points;
//}

struct Triangle {
	p p1, p2, p3;
	p circumcenter;
	float radiusSquared;

	Triangle(const p& a, const p& b, const p& c) : p1(a), p2(b), p3(c) {
		calculateCircumcircle();
	}

	void calculateCircumcircle() {
		// Calculates the circumcenter
		float d = 2 * (p1.x * (p2.y - p3.y) + p2.x * (p3.y - p1.y) + p3.x * (p1.y - p2.y));
		float x = ((p1.x * p1.x + p1.y * p1.y) * (p2.y - p3.y) +
			(p2.x * p2.x + p2.y * p2.y) * (p3.y - p1.y) +
			(p3.x * p3.x + p3.y * p3.y) * (p1.y - p2.y)) / d;
		float y = ((p1.x * p1.x + p1.y * p1.y) * (p3.x - p2.x) +
			(p2.x * p2.x + p2.y * p2.y) * (p1.x - p3.x) +
			(p3.x * p3.x + p3.y * p3.y) * (p2.x - p1.x)) / d;
		circumcenter = p(x, y);

		// Calculates the radius squared
		radiusSquared = (circumcenter.x - p1.x) * (circumcenter.x - p1.x) +
			(circumcenter.y - p1.y) * (circumcenter.y - p1.y);
	}

	//checks if the triangle has a point on its circumcircle
	bool circumcircleContains(const p& point) const {
		float dx = point.x - circumcenter.x;
		float dy = point.y - circumcenter.y;
		return (dx * dx + dy * dy) <= radiusSquared;
	}
};


Triangle createSuperTriangle(const std::vector<p>& points) {
	if (points.empty())
	{
		throw std::runtime_error("Point set is empty");
	}

	// Finding the bounding box
	float minX = std::numeric_limits<float>::max();
	float minY = std::numeric_limits<float>::max();
	float maxX = std::numeric_limits<float>::lowest();
	float maxY = std::numeric_limits<float>::lowest();

	for (const auto& point : points) {
		minX = std::min(minX, point.x);
		minY = std::min(minY, point.y);
		maxX = std::max(maxX, point.x);
		maxY = std::max(maxY, point.y);
	}

	// Calculate center and dimensions
	float centerX = (minX + maxX) / 2;
	float centerY = (minY + maxY) / 2;
	float width = maxX - minX;
	float height = maxY - minY;

	// Calculate radius of the circumcircle of the bounding box
	float radius = std::sqrt(width * width + height * height) / 2;

	// Add a small buffer for numerical stability
	radius *= 100.01f;

	// Calculate vertices of the equilateral triangle
	const float sqrt3 = std::sqrt(3.0f);
	p v1(centerX - sqrt3 * radius, centerY - radius);
	p v2(centerX + sqrt3 * radius, centerY - radius);
	p v3(centerX, centerY + 2 * radius);

	return Triangle(v1, v2, v3);
}


//Triangulates the 2d points. If it's a sphere lidIndices are stored to retriangulate later
//It iterates through the points only once. First it creates a super triangle that englobes every point.
//The delaunay triangulation works by every triangle not having in its circumcircle any other point, so for every 
// point, what we do is to check if it is inside any circumcircle and if so it is added to badTriangles to be deleted later
// From the badTriangles we find the outer edges, that is the edge/s of the triangle that wont be deleted and that will be paired with the current point to form new triangles.
//When the process is finished, then we find the lid indices to triangulate the sphere in another function call, and lastly, we remove the supertriangle and it's edges
std::vector<Triangle> bowyerWatson(std::vector<p>& points, std::vector<unsigned int>& lidIndices) {

    Triangle superTriangle = createSuperTriangle(points);

    std::vector<Triangle> triangles;
    triangles.reserve(2 * points.size()); //there's a theorem that says that it will be close but less than 2

    triangles.push_back(superTriangle);


	for (const p& point : points) {
		std::vector<Triangle> badTriangles;
		badTriangles.reserve(20);

		// Use partition instead of erase-insert. It reorders triangles and returns an iterator where triangles end and badTriangles start 
		std::vector<Triangle>::iterator partitionIt = std::partition(triangles.begin(), triangles.end(),
			[&point](const Triangle& t) { return !t.circumcircleContains(point); });

		badTriangles.insert(badTriangles.end(), partitionIt, triangles.end());
		triangles.erase(partitionIt, triangles.end());

		// Use set instead of unordered_set to avoid needing a hash function
		std::set<std::pair<p, p>> edgeSet;

		for (const Triangle& t : badTriangles) {
			std::pair<p, p> edges[3] = {
				{t.p1, t.p2}, {t.p2, t.p3}, {t.p3, t.p1}
			};

			for (const auto& edge : edges) {
				// Normalize the edge so (p1, p2) and (p2, p1) are treated as the same
				auto normalizedEdge = edge.first < edge.second ? edge : std::pair<p, p>(edge.second, edge.first);
				auto [it, inserted] = edgeSet.insert(normalizedEdge);
				if (!inserted) {
					edgeSet.erase(it);
				}
			}
		}

		// Reserve space for new triangles
		triangles.reserve(triangles.size() + edgeSet.size());

		// Create new triangles
		for (const auto& edge : edgeSet) {
			triangles.emplace_back(point, edge.first, edge.second);
		}
	}

    //this is only useful for the sphere
    std::vector<p> lidPoints;
    std::set<p> uniqueLidPoints;
    for (const auto& t : triangles) {
        if ((t.p1.x == superTriangle.p1.x && t.p1.y == superTriangle.p1.y) ||
            (t.p1.x == superTriangle.p2.x && t.p1.y == superTriangle.p2.y) ||
            (t.p1.x == superTriangle.p3.x && t.p1.y == superTriangle.p3.y) ||
            (t.p2.x == superTriangle.p1.x && t.p2.y == superTriangle.p1.y) ||
            (t.p2.x == superTriangle.p2.x && t.p2.y == superTriangle.p2.y) ||
            (t.p2.x == superTriangle.p3.x && t.p2.y == superTriangle.p3.y) ||
            (t.p3.x == superTriangle.p1.x && t.p3.y == superTriangle.p1.y) ||
            (t.p3.x == superTriangle.p2.x && t.p3.y == superTriangle.p2.y) ||
            (t.p3.x == superTriangle.p3.x && t.p3.y == superTriangle.p3.y)) {

            // Add points that are not part of the super-triangle to lidPoints
            if (!(t.p1 == superTriangle.p1 || t.p1 == superTriangle.p2 || t.p1 == superTriangle.p3))
                uniqueLidPoints.insert(t.p1);
            if (!(t.p2 == superTriangle.p1 || t.p2 == superTriangle.p2 || t.p2 == superTriangle.p3))
                uniqueLidPoints.insert(t.p2);
            if (!(t.p3 == superTriangle.p1 || t.p3 == superTriangle.p2 || t.p3 == superTriangle.p3))
                uniqueLidPoints.insert(t.p3);
        }
    }

    // Convert set to vector
    lidPoints.assign(uniqueLidPoints.begin(), uniqueLidPoints.end());


    //Now that we have the points, we retrieve the lid indices looking for in points
    for (unsigned int i = 0; i < points.size(); ++i)
    {
        for (const auto& lid : lidPoints)
        {
            if (lid == points[i])
            {
                lidIndices.push_back(i);
                break;
            }
        }
    }


    // Remove triangles that share vertices with the super-triangle
    triangles.erase(std::remove_if(triangles.begin(), triangles.end(),
        [&superTriangle](const Triangle& t) {
            return (t.p1.x == superTriangle.p1.x && t.p1.y == superTriangle.p1.y) ||
                (t.p1.x == superTriangle.p2.x && t.p1.y == superTriangle.p2.y) ||
                (t.p1.x == superTriangle.p3.x && t.p1.y == superTriangle.p3.y) ||
                (t.p2.x == superTriangle.p1.x && t.p2.y == superTriangle.p1.y) ||
                (t.p2.x == superTriangle.p2.x && t.p2.y == superTriangle.p2.y) ||
                (t.p2.x == superTriangle.p3.x && t.p2.y == superTriangle.p3.y) ||
                (t.p3.x == superTriangle.p1.x && t.p3.y == superTriangle.p1.y) ||
                (t.p3.x == superTriangle.p2.x && t.p3.y == superTriangle.p2.y) ||
                (t.p3.x == superTriangle.p3.x && t.p3.y == superTriangle.p3.y);
        }), triangles.end());

    return triangles;
}

//Generates the indices for the delaunay. It takes the points, and in order, assigns a index for each one in a map,
//  then it traverses the triangles, filling the indices vector with the index of each point
std::vector<unsigned int> generateIndices(std::vector<p>& points, std::vector<Triangle>& triangles) {
	std::vector<unsigned int> indices;
	std::unordered_map<p, unsigned int, p_hash_multiplicative> umap;
	
	umap.reserve(points.size());
	indices.reserve(triangles.size() * 6);

	for (unsigned int i = 0; i < points.size(); ++i) {
		umap[points[i]] = i;
	}
	
	for (const auto& triangle : triangles) {
		indices.insert(indices.end(),
			{	umap[triangle.p1],
				umap[triangle.p2],
				umap[triangle.p2],
				umap[triangle.p3],
				umap[triangle.p3],
				umap[triangle.p1] 
			});
	}

	return indices;
}



//oye haz esto en templates y ponlo en common
//converts a p3 into a p2 without y value
vector<p> p3ToP2(vector<p3>& input) {
	vector<p> output;
	output.reserve(input.size());
	for (const auto& i : input)
	{
		output.emplace_back(p{ i.x,i.z });
	}
	return output;

}

vector<p3> p2ToP3(const vector<p>& input) {
	vector<p3> output;
	output.reserve(input.size());
	for (const auto& i : input)
	{
		output.emplace_back(p3{ i.x,0,i.y });
	}
	return output;

}


//std::vector<Triangle> bowyerWatson(std::vector<p>& points, std::vector<unsigned int>& lidIndices) {
//
//	Triangle superTriangle = createSuperTriangle(points);
//
//	std::list<Triangle> triangles;
//	//triangles.reserve(2 * points.size()); //there's a theorem that says that it will be close but less than 2
//
//	triangles.push_back(superTriangle);
//
//
//	for (const p& point : points) {
//
//		std::vector<Triangle> badTriangles;
//		badTriangles.reserve(20);
//
//		// If the point is inside any circumcircle, then those triangles aren't valid and go to badTriangles
//		for (auto it = triangles.begin(); it != triangles.end();) {
//			if (it->circumcircleContains(point)) {
//				badTriangles.push_back(*it);
//				it = triangles.erase(it);
//			}
//			else ++it;
//		}
//
//		//print(badTriangles.size());
//		std::vector<std::array<p, 2>> polygonEdges;
//
//		//the logic literally is, if the edge of the badTriangle is only found once, then it is an outer polygon edge. If it is found twice (it's shared between 2 badTriangles),
//		// then it's removed, It is as simple as that 
//		for (const Triangle& t : badTriangles)
//		{
//			std::array<p, 2> edges[3] =
//			{
//				{t.p1, t.p2},
//				{t.p2, t.p3},
//				{t.p3, t.p1}
//			};
//
//			for (const auto& edge : edges)
//			{
//				bool found = false;
//				for (size_t i = 0; i < polygonEdges.size(); ++i)
//				{
//					if ((polygonEdges[i][0] == edge[0] && polygonEdges[i][1] == edge[1]) ||
//						(polygonEdges[i][0] == edge[1] && polygonEdges[i][1] == edge[0]))
//					{
//						polygonEdges.erase(polygonEdges.begin() + i);
//						found = true;
//						break;
//					}
//				}
//				if (!found) {
//					polygonEdges.push_back(edge);
//				}
//			}
//		}
//		for (const auto& edge : polygonEdges)
//		{
//			triangles.push_back(Triangle(point, edge[0], edge[1]));
//		}
//	}
//
//	//this is only useful for the sphere
//	std::vector<p> lidPoints;
//	std::set<p> uniqueLidPoints;
//	for (const auto& t : triangles) {
//		if ((t.p1.x == superTriangle.p1.x && t.p1.y == superTriangle.p1.y) ||
//			(t.p1.x == superTriangle.p2.x && t.p1.y == superTriangle.p2.y) ||
//			(t.p1.x == superTriangle.p3.x && t.p1.y == superTriangle.p3.y) ||
//			(t.p2.x == superTriangle.p1.x && t.p2.y == superTriangle.p1.y) ||
//			(t.p2.x == superTriangle.p2.x && t.p2.y == superTriangle.p2.y) ||
//			(t.p2.x == superTriangle.p3.x && t.p2.y == superTriangle.p3.y) ||
//			(t.p3.x == superTriangle.p1.x && t.p3.y == superTriangle.p1.y) ||
//			(t.p3.x == superTriangle.p2.x && t.p3.y == superTriangle.p2.y) ||
//			(t.p3.x == superTriangle.p3.x && t.p3.y == superTriangle.p3.y)) {
//
//			// Add points that are not part of the super-triangle to lidPoints
//			if (!(t.p1 == superTriangle.p1 || t.p1 == superTriangle.p2 || t.p1 == superTriangle.p3))
//				uniqueLidPoints.insert(t.p1);
//			if (!(t.p2 == superTriangle.p1 || t.p2 == superTriangle.p2 || t.p2 == superTriangle.p3))
//				uniqueLidPoints.insert(t.p2);
//			if (!(t.p3 == superTriangle.p1 || t.p3 == superTriangle.p2 || t.p3 == superTriangle.p3))
//				uniqueLidPoints.insert(t.p3);
//		}
//	}
//
//	// Convert set to vector
//	lidPoints.assign(uniqueLidPoints.begin(), uniqueLidPoints.end());
//
//
//	//Now that we have the points, we retrieve the lid indices looking for in points
//	for (unsigned int i = 0; i < points.size(); ++i)
//	{
//		for (const auto& lid : lidPoints)
//		{
//			if (lid == points[i])
//			{
//				lidIndices.push_back(i);
//				break;
//			}
//		}
//	}
//
//
//	// Remove triangles that share vertices with the super-triangle
//	triangles.remove_if([&superTriangle](const Triangle& t) {
//		return (t.p1.x == superTriangle.p1.x && t.p1.y == superTriangle.p1.y) ||
//			(t.p1.x == superTriangle.p2.x && t.p1.y == superTriangle.p2.y) ||
//			(t.p1.x == superTriangle.p3.x && t.p1.y == superTriangle.p3.y) ||
//			(t.p2.x == superTriangle.p1.x && t.p2.y == superTriangle.p1.y) ||
//			(t.p2.x == superTriangle.p2.x && t.p2.y == superTriangle.p2.y) ||
//			(t.p2.x == superTriangle.p3.x && t.p2.y == superTriangle.p3.y) ||
//			(t.p3.x == superTriangle.p1.x && t.p3.y == superTriangle.p1.y) ||
//			(t.p3.x == superTriangle.p2.x && t.p3.y == superTriangle.p2.y) ||
//			(t.p3.x == superTriangle.p3.x && t.p3.y == superTriangle.p3.y);
//		});
//
//
//	return std::vector<Triangle>(triangles.begin(), triangles.end());
//}