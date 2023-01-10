#pragma once

#include <cstdint>
#include <cmath>
#include <limits>
#include <vector>

struct Straw {
	Straw();	//default constructor
	Straw(double hit_data, size_t _straw_layer, double _straw_r, bool _is_nan); //common constructor
	
	void BuildGrid();	//build grid: boundaries & center ( o )
	std::vector<double> u = {};	//vector containing this grid

	void BuildGrid(const Straw& prev_straw); //for NaN layers takes adjacent straw from next/prev layer of the same type

	double center_coord_u = 0;	//position of the straw center
	size_t straw_layer = 0;	//layer number
	double straw_r = 0;	// straw radius
	bool is_nan = 0; //is hit NaN?
};

Straw::Straw() {

}

//common constructor
Straw::Straw(double hit_data, size_t _straw_layer, double _straw_r, bool _is_nan) :
	straw_layer(_straw_layer), straw_r(_straw_r), is_nan(_is_nan)
{
	if (is_nan) {
		center_coord_u = std::numeric_limits<double>::quiet_NaN();
		return; //if straw_hit is NaN -> skip calculations & assign center as NaN
	}

	center_coord_u = 2 * hit_data * straw_r;//build center of straw
	if (straw_layer % 2 == 1) {
		center_coord_u += straw_r;//odd layer is shifted on 1x radius
	}
	BuildGrid();
}

void Straw::BuildGrid() {
	for (double point = (center_coord_u - straw_r); point <= (center_coord_u + straw_r); point += straw_r) {
		u.push_back(point);
	}
}
//for NaN layers takes adjacent straw from next/prev layer of the same type
//so we can build "pseudo-coordinates" in this layer adjacent to other one
void Straw::BuildGrid(const Straw& prev_or_next_straw) {
	if (prev_or_next_straw.is_nan) {
		return;
	}
	if (straw_layer % 2 == 1) {				//if current straw is from odd layer (1, 3), shift --> wrt prev
		for (const auto& item : prev_or_next_straw.u) {
			u.push_back(item + straw_r);
		}
	}
	else {				//if current straw is from even layer (0, 2), shift  <-- wrt next
		for (const auto& item : prev_or_next_straw.u) {
			u.push_back(item - straw_r);
		}
	}
}

std::vector<Straw> BuildCoordGrid(const std::vector<double>& hits, const double& straw_radius);
std::vector<std::vector<double>> BuildGlobCoordsManifold(const std::vector<Straw>& StrawGrid);
std::vector<std::vector<double>> GeneratePermutations(const std::vector<double>& dataset);
void ShowCoordSetsFromStraws(const std::vector<double>& detHits, const std::vector<Straw>& StrawSet);
void ShowPossibleHitsManifold(const std::vector<std::vector<double>>& MapCrd);
