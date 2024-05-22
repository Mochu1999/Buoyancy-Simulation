#pragma once

#include "Common.hpp"


#include "Polygons.hpp"

struct AuxSquare {
	vector<vector<Polygons>> AllSquares;
	vector<Polygons> hola;
	vector<Polygons> modelSquare;

	AuxSquare() {
		modelSquare.clear();
		modelSquare.resize(6);

		modelSquare[0].addSet({ {-0.2f, -0.2f, -0.2f}, { 0.2f,-0.2f,-0.2f }, { 0.2f,-0.2f,0.2f }, { -0.2f,-0.2f,0.2f }, { -0.2f,-0.2f,-0.2f } });
		modelSquare[1].addSet({ {-0.2f, 0.2f, -0.2f}, { 0.2f,0.2f,-0.2f }, { 0.2f,0.2f,0.2f }, { -0.2f,0.2f,0.2f }, { -0.2f,0.2f,-0.2f } });
		modelSquare[2].addSet({ {-0.2f, -0.2f, -0.2f}, { 0.2f,-0.2f,-0.2f }, { 0.2f,0.2f,-0.2f }, {-0.2f, 0.2f, -0.2f}, { -0.2f,-0.2f,-0.2f } });
		modelSquare[3].addSet({ {-0.2f, -0.2f, 0.2f}, { 0.2f,-0.2f,0.2f }, { 0.2f,0.2f,0.2f }, {-0.2f, 0.2f, 0.2f}, { -0.2f,-0.2f,0.2f } });
		modelSquare[4].addSet({ {-0.2f, -0.2f, -0.2f}, { -0.2f,-0.2f,0.2f }, { -0.2f,0.2f,0.2f }, {-0.2f, 0.2f, -0.2f}, { -0.2f,-0.2f,-0.2f } });
		modelSquare[5].addSet({ {0.2f, -0.2f, -0.2f}, { 0.2f,-0.2f,0.2f }, { 0.2f,0.2f,0.2f }, {0.2f, 0.2f, -0.2f}, { 0.2f,-0.2f,-0.2f } });

		
	}
	void addSquare(vector<Polygons>& square, p3 pos) 
	{
		square.clear();
		square.resize(6);
		
		square[0].addSet({ p3{-0.2f, -0.2f, -0.2f}+pos, p3{ 0.2f,-0.2f,-0.2f }+pos, p3{ 0.2f,-0.2f,0.2f } +pos,p3{ -0.2f,-0.2f,0.2f }+pos, p3{ -0.2f,-0.2f,-0.2f } + pos });
		square[1].addSet({ p3{-0.2f, 0.2f, -0.2f}+pos, p3{ 0.2f,0.2f,-0.2f } +pos,p3{ 0.2f,0.2f,0.2f }+pos, p3{ -0.2f,0.2f,0.2f } +pos,p3{ -0.2f,0.2f,-0.2f } +pos});
		square[2].addSet({ p3{-0.2f, -0.2f, -0.2f} + pos, p3{ 0.2f,-0.2f,-0.2f } + pos, p3{ 0.2f,0.2f,-0.2f } + pos,p3{-0.2f, 0.2f, -0.2f} + pos, p3{ -0.2f,-0.2f,-0.2f} + pos });
		square[3].addSet({ p3{-0.2f, -0.2f, 0.2f}+pos, p3{ 0.2f,-0.2f,0.2f } +pos,p3{ 0.2f,0.2f,0.2f } + pos, p3{-0.2f, 0.2f, 0.2f} + pos,p3{ -0.2f,-0.2f,0.2f } + pos });
		square[4].addSet({ p3{-0.2f, -0.2f, -0.2f } + pos, p3{ -0.2f,-0.2f,0.2f } + pos, p3{ -0.2f,0.2f,0.2f } + pos,p3{-0.2f, 0.2f, -0.2f} + pos, p3{ -0.2f,-0.2f,-0.2f } + pos });
		square[5].addSet({ p3{0.2f, -0.2f, -0.2f} + pos, p3{ 0.2f,-0.2f,0.2f } + pos,p3{ 0.2f,0.2f,0.2f } + pos, p3{0.2f, 0.2f, -0.2f} +pos,p3{ 0.2f,-0.2f,-0.2f } +pos});
	}
	void draw(vector<p3> positions) {

		for (auto& pos : positions)
		{
			addSquare(hola, pos);

			/*AllSquares.clear();
			AllSquares.resize(positions.size());

			for (int i = 0; i < positions.size(); i++)
			{
				AllSquares[i] = modelSquare;
			}*/
			//}
			//for (int i = 0; i < positions.size(); i++)
			//{
			//	for (vector<Polygons> square : AllSquares)
			//	{
			//		for (Polygons surface : square)
			//		{
			//			for (p3 vertex : surface.positions)
			//			{
			//				vertex = vertex+positions[i];
			//			}
			//		}
			//	}



			for (auto& surface : hola) {
				surface.draw();
			}
			/*for (vector<Polygons> square : AllSquares)
			{
				for (auto& surface : hola) {
					surface.draw();
				}
			}*/
		}
	}
};
