#include "TrackerPoint.h"

#include <iostream>
#include <algorithm>

//Builds coordinate grid: boundaries & center of each straw
//The next step to improve here: limit vector of possible positions
//using V-shaped dependence on time of signal 
std::vector<Straw> BuildCoordGrid(const std::vector<double>& hits, const double& straw_radius) {
	std::vector<Straw> vector_straws;
	vector_straws.reserve(hits.size());

	size_t counter_of_hits = 0;
	for (auto iter = hits.begin(); iter != hits.end(); iter++, counter_of_hits++) {	//create vector of straw objects
		vector_straws.push_back(Straw(*iter, counter_of_hits, straw_radius, std::isnan(*iter)));
	}

	for (auto iter = vector_straws.begin(); iter != vector_straws.end(); iter++) {	//iterate over straws to check for NaNs
		if ((*iter).is_nan) {
			if ((*iter).straw_layer % 2 == 1) {				//if current straw is from odd layer (1, 3), 
				(*iter).BuildGrid(*(iter - 1));				//shift --> wrt prev
			}
			else {								//if current straw is from even layer (0, 2), 
				(*iter).BuildGrid(*(iter + 1));	//shift  <-- wrt next
			}
		}
	}
	return vector_straws;
}
//finds the intersection points for straws with registered hits
//for straws of the same layer with NaN finds points in space around 
//datactor that was registered except from positions corresonding to other detector with NaN
std::vector<std::vector<double>> BuildGlobCoordsManifold(const std::vector<Straw>& StrawGrid) {
	std::vector<std::vector<double>> CoordsMap = {};

	for (size_t layer_type = 0; layer_type < StrawGrid.size(); layer_type += 2) {
		std::vector<double> CoordsManifold = {};
		if (StrawGrid.at(layer_type).is_nan && StrawGrid.at(layer_type + 1).is_nan) {//if both NaNs -> position undefined
			CoordsMap.push_back(CoordsManifold);									//push empty vector {}
			continue;
		}

		else if (StrawGrid.at(layer_type).is_nan) {									//if first layer is nan, find difference
			CoordsManifold.push_back(*(StrawGrid.at(layer_type).u.rbegin()));		//layer 1 - layer 0
			std::set_difference(StrawGrid.at(layer_type + 1).u.begin(), StrawGrid.at(layer_type + 1).u.end(),
				StrawGrid.at(layer_type).u.begin(), StrawGrid.at(layer_type).u.end(),
				std::back_inserter(CoordsManifold));
		}

		else if (StrawGrid.at(layer_type + 1).is_nan) {								//if second layer is nan, -> layer 0 - layer 1
			std::set_difference(StrawGrid.at(layer_type).u.begin(), StrawGrid.at(layer_type).u.end(),
				StrawGrid.at(layer_type + 1).u.begin(), StrawGrid.at(layer_type + 1).u.end(),
				std::back_inserter(CoordsManifold));
			CoordsManifold.push_back(*(StrawGrid.at(layer_type + 1).u.begin()));
		}

		else {
			std::set_intersection(StrawGrid.at(layer_type).u.begin(), StrawGrid.at(layer_type).u.end(), //if no NaNs, -> union 
				StrawGrid.at(layer_type + 1).u.begin(), StrawGrid.at(layer_type + 1).u.end(),
				std::back_inserter(CoordsManifold));
		}
		CoordsMap.push_back(CoordsManifold);
	}
	return CoordsMap;
}
//simple console output for coordinate sets corresponding to each straw
void ShowCoordSetsFromStraws(const std::vector<double>& detHits, const std::vector<Straw>& StrawSet) {
	size_t cnt = 0;
	for (auto& i : StrawSet) {
		std::cout << "Layer: " << cnt << ", coord is " << detHits.at(cnt) << std::endl;
		std::cout << "u: { ";
		if (i.u.size() < 1) std::cout << "}\n";
		for (auto iter = i.u.begin(); iter != i.u.end(); iter++) {
			std::cout << *iter;
			if (iter != i.u.end() - 1) {
				std::cout << ", ";
			}
			else std::cout << " }\n";
		}
		cnt++;
	}
}
//simple console output for possible hit position manifold
void ShowPossibleHitsManifold(const std::vector<std::vector<double>>& MapCrd) {
	size_t cnt = 0;
	for (auto& i : MapCrd) {
		std::cout << "Layer: " << cnt << std::endl;
		std::cout << "{ ";
		if (i.size() < 1) std::cout << "}\n";
		for (auto iter = i.begin(); iter != i.end(); iter++) {
			std::cout << *iter;
			if (iter != i.end() - 1) {
				std::cout << ", ";
			}
			else std::cout << " }\n";
		}

		cnt++;
	}
}

std::vector<std::vector<double>> GeneratePermutations(const std::vector<double>& dataset) {
	std::vector<std::vector<double>> permutations;
	std::vector<double> data(dataset.size());
	std::copy(dataset.begin(), dataset.end(), data.begin());
	permutations.push_back(data);
	for (size_t i = 0; i < data.size(); i++) {
		data.at(i) = std::numeric_limits<double>::quiet_NaN();
		permutations.push_back(data);
		std::copy(dataset.begin(), dataset.end(), data.begin());
	}
	for (size_t i = 0; i < data.size(); i++) {
		std::vector<double> data_buffer(data.size());
		data.at(i) = std::numeric_limits<double>::quiet_NaN();
		std::copy(data.begin(), data.end(), data_buffer.begin());
		for (size_t j = 0; j < data.size(); j++) {
			if (std::isnan(data.at(j)) || j <= i) continue;
			data.at(j) = std::numeric_limits<double>::quiet_NaN();
			permutations.push_back(data);
			data = data_buffer;
		}
		std::copy(dataset.begin(), dataset.end(), data.begin());
	}
	for (size_t i = 0; i < data.size(); i++) {
		for (size_t j = 0; j < data.size(); j++) {
			if (i == j) continue;
			else data.at(data.size() - 1 - j) = std::numeric_limits<double>::quiet_NaN();
		}
		permutations.push_back(data);
		std::copy(dataset.begin(), dataset.end(), data.begin());
	}
	permutations.push_back(std::vector<double>(4, std::numeric_limits<double>::quiet_NaN()));
	return permutations;
}

int main() {
	std::vector<double> detHits = { NAN, 0.0, 2.0, 2.0 };
	std::vector<Straw> StrawSet = BuildCoordGrid(detHits, 0.5);
	std::vector<std::vector<double>> MapCrd = BuildGlobCoordsManifold(StrawSet);

	ShowCoordSetsFromStraws(detHits, StrawSet);
	ShowPossibleHitsManifold(MapCrd);

	//std::vector<std::vector<double>> TestSet = GeneratePermutations({ 1.0, 0.0, 2.0, 2.0 });
	//std::vector<std::vector<double>> CoordsSet = {};
	//for (const auto& set : TestSet) {
	//	CoordsSet.push_back(BuildGlobCoordsManifold(BuildCoordGrid((set)));
	//}

	return 0;
}