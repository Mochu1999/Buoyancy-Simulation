#pragma once

#include "Common.hpp"


#include "Polygons.hpp"


struct AuxSquare {

	vector<vector<Polygons>> allSquares;

	void addSet(vector<p3> positions) {

		allSquares.clear();
		allSquares.resize(positions.size());

		for (int i = 0; i < positions.size(); i++)
		{
			allSquares[i] = createSquare(positions[i]);

		}

	}

	vector<Polygons> createSquare(p3 pos)
	{
		vector<Polygons> square;
		square.resize(6);

		square[0].addSet({ p3{-0.2f, -0.2f, -0.2f} + pos, p3{ 0.2f,-0.2f,-0.2f } + pos, p3{ 0.2f,-0.2f,0.2f } + pos,p3{ -0.2f,-0.2f,0.2f } + pos, p3{ -0.2f,-0.2f,-0.2f } + pos });
		square[1].addSet({ p3{-0.2f, 0.2f, -0.2f} + pos, p3{ 0.2f,0.2f,-0.2f } + pos,p3{ 0.2f,0.2f,0.2f } + pos, p3{ -0.2f,0.2f,0.2f } + pos,p3{ -0.2f,0.2f,-0.2f } + pos });
		square[2].addSet({ p3{-0.2f, -0.2f, -0.2f} + pos, p3{ 0.2f,-0.2f,-0.2f } + pos, p3{ 0.2f,0.2f,-0.2f } + pos,p3{-0.2f, 0.2f, -0.2f} + pos, p3{ -0.2f,-0.2f,-0.2f} + pos });
		square[3].addSet({ p3{-0.2f, -0.2f, 0.2f} + pos, p3{ 0.2f,-0.2f,0.2f } + pos,p3{ 0.2f,0.2f,0.2f } + pos, p3{-0.2f, 0.2f, 0.2f} + pos,p3{ -0.2f,-0.2f,0.2f } + pos });
		square[4].addSet({ p3{-0.2f, -0.2f, -0.2f } + pos, p3{ -0.2f,-0.2f,0.2f } + pos, p3{ -0.2f,0.2f,0.2f } + pos,p3{-0.2f, 0.2f, -0.2f} + pos, p3{ -0.2f,-0.2f,-0.2f } + pos });
		square[5].addSet({ p3{0.2f, -0.2f, -0.2f} + pos, p3{ 0.2f,-0.2f,0.2f } + pos,p3{ 0.2f,0.2f,0.2f } + pos, p3{0.2f, 0.2f, -0.2f} + pos,p3{ 0.2f,-0.2f,-0.2f } + pos });
		
		return square;
	}
	void draw() {

		for (auto& square : allSquares)
		{
			for (auto& surface : square) 
			{
				surface.draw();
			}
		}
	}
};


//struct AuxSquare {
//
//	vector<Polygons> modelSquare;
//
//	void createSquare(vector<Polygons>& square, p3 pos)
//	{
//
//		square.resize(6);
//
//		square[0].addSet({ p3{-0.2f, -0.2f, -0.2f} + pos, p3{ 0.2f,-0.2f,-0.2f } + pos, p3{ 0.2f,-0.2f,0.2f } + pos,p3{ -0.2f,-0.2f,0.2f } + pos, p3{ -0.2f,-0.2f,-0.2f } + pos });
//		square[1].addSet({ p3{-0.2f, 0.2f, -0.2f} + pos, p3{ 0.2f,0.2f,-0.2f } + pos,p3{ 0.2f,0.2f,0.2f } + pos, p3{ -0.2f,0.2f,0.2f } + pos,p3{ -0.2f,0.2f,-0.2f } + pos });
//		square[2].addSet({ p3{-0.2f, -0.2f, -0.2f} + pos, p3{ 0.2f,-0.2f,-0.2f } + pos, p3{ 0.2f,0.2f,-0.2f } + pos,p3{-0.2f, 0.2f, -0.2f} + pos, p3{ -0.2f,-0.2f,-0.2f} + pos });
//		square[3].addSet({ p3{-0.2f, -0.2f, 0.2f} + pos, p3{ 0.2f,-0.2f,0.2f } + pos,p3{ 0.2f,0.2f,0.2f } + pos, p3{-0.2f, 0.2f, 0.2f} + pos,p3{ -0.2f,-0.2f,0.2f } + pos });
//		square[4].addSet({ p3{-0.2f, -0.2f, -0.2f } + pos, p3{ -0.2f,-0.2f,0.2f } + pos, p3{ -0.2f,0.2f,0.2f } + pos,p3{-0.2f, 0.2f, -0.2f} + pos, p3{ -0.2f,-0.2f,-0.2f } + pos });
//		square[5].addSet({ p3{0.2f, -0.2f, -0.2f} + pos, p3{ 0.2f,-0.2f,0.2f } + pos,p3{ 0.2f,0.2f,0.2f } + pos, p3{0.2f, 0.2f, -0.2f} + pos,p3{ 0.2f,-0.2f,-0.2f } + pos });
//	}
//	void draw(vector<p3> positions) {
//
//		for (auto& pos : positions)
//		{
//			vector<Polygons> square;
//			createSquare(square, pos);
//
//			for (auto& surface : square) {
//				surface.draw();
//			}
//
//		}
//	}
//};