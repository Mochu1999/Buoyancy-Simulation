//Triangulates the 2d points. If it's a sphere lidIndices are stored to retriangulate later
//It iterates through the points only once. First it creates a super triangle that englobes every point.
//The delaunay triangulation works by every triangle not having in its circumcircle any other point, so for every 
// point, what we do is to check if it is inside any circumcircle and if so it is added to badTriangles to be deleted later
// From the badTriangles we find the outer edges, that is the edge/s of the triangle that wont be deleted and that will be paired with the current point
//  to form new triangles. When the process is finished, then we find the lid indices to triangulate the sphere in another function call, 
// and lastly, we remove the supertriangle and it's edges
std::vector<Triangle> bowyerWatson(std::vector<p>& points, std::vector<unsigned int>& lidIndices) {

	Triangle superTriangle = createSuperTriangle(points);

	std::vector<Triangle> triangles;
	triangles.reserve(2 * points.size()); //there's a theorem that says that it will be close but less than 2

	triangles.push_back(superTriangle);


	for (const p& point : points) {
		std::vector<Triangle> badTriangles;
		badTriangles.reserve(20);

		// Use partition instead of erase-insert. It reorders triangles and returns an iterator where triangles end and badTriangles start.
		// The reorder works by positioning first those triangles that do not circumcircle other points
		std::vector<Triangle>::iterator partitionIt = std::partition(triangles.begin(), triangles.end(),
			[&point](const Triangle& t) { return !t.circumcircleContains(point); });

		badTriangles.insert(badTriangles.end(), partitionIt, triangles.end());
		triangles.erase(partitionIt, triangles.end());


		std::unordered_set<std::pair<p, p>, pair_hash_multiplicative> polygonEdges;
		//the logic literally is, if the edge of the badTriangle is only found once, then it is an outer polygon edge. If it is found 
		// twice (it's shared between 2 badTriangles),then it's removed, It is as simple as that. Using for faster deletions
		for (const Triangle& t : badTriangles)
		{
			std::pair<p, p> edges[3] = { {t.p1, t.p2}, {t.p2, t.p3}, {t.p3, t.p1} };

			for (const auto& edge : edges)
			{
				// Normalize the edge so (p1, p2) and (p2, p1) are treated as the same. Reorder by the arbithatry smaller edge.first going first
				std::pair<p, p> normalizedEdge = edge.first < edge.second ? edge : std::pair<p, p>(edge.second, edge.first);

				//this is a structured binding, inserted is a boolean that returns true if the element is inserted
				auto [it, inserted] = polygonEdges.insert(normalizedEdge);
				if (!inserted) {
					polygonEdges.erase(it);
				}
			}
		}

		//Now we add the new triangles
		triangles.reserve(triangles.size() + polygonEdges.size());

		for (const auto& edge : polygonEdges)
		{
			triangles.emplace_back(point, edge.first, edge.second);
		}
	}

	{
		std::unordered_set<p, p_HashMultiplicative> uniqueLidPoints;

		// Gathers vertices that are connected to supertriangle without being its edges
		for (const auto& t : triangles)
		{
			if (t == superTriangle)
			{
				if (!(t.p1 == superTriangle.p1 || t.p1 == superTriangle.p2 || t.p1 == superTriangle.p3)) uniqueLidPoints.insert(t.p1);
				if (!(t.p2 == superTriangle.p1 || t.p2 == superTriangle.p2 || t.p2 == superTriangle.p3)) uniqueLidPoints.insert(t.p2);
				if (!(t.p3 == superTriangle.p1 || t.p3 == superTriangle.p2 || t.p3 == superTriangle.p3)) uniqueLidPoints.insert(t.p3);
			}
		}

		std::vector<p> lidPoints(uniqueLidPoints.begin(), uniqueLidPoints.end());

		//we store in a map the index of each point. It is needed because later we will want the index only having the point
		std::unordered_map<p, unsigned int, p_HashMultiplicative> pointToIndexMap;
		for (unsigned int i = 0; i < points.size(); ++i)
		{
			pointToIndexMap[points[i]] = i;
		}

		lidIndices.reserve(lidPoints.size());
		//Retrieves the index from the lidpoint
		for (const auto& lid : lidPoints) {
			lidIndices.emplace_back(pointToIndexMap.at(lid));
		}
	}
	triangles.erase(std::remove_if(triangles.begin(), triangles.end(),
		[&superTriangle](const Triangle& t) {
			return t == superTriangle;  // Checks if any vertex matches any in superTriangle
		}), triangles.end());

	return triangles;
}