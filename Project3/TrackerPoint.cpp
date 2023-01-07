#include "TrackerPoint.h"

#include <iostream>
#include <algorithm>


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

//double GlobX(const std::vector<double>& hits, const std::vector<Straw>& StrawGrid) {
//
//}
//
//double BuildGlobalX(const std::vector<double>& hits, const std::vector<Straw>& StrawGrid) {
//	double globalX = 0;
//	if (isnan(hits.at(0)) && isnan(hits.at(1))) {
//		return std::numeric_limits<double>::quiet_NaN();
//	}
//	if (!isnan(hits.at(0)) && !isnan(hits.at(1))) {
//		std::vector<double> rangeX = {};
//		std::set_intersection(StrawGrid.at(0).u.begin(), StrawGrid.at(0).u.end(),
//			StrawGrid.at(1).u.begin(), StrawGrid.at(1).u.end(),
//			std::back_inserter(rangeX));
//		if (rangeX.size() < 1) return std::numeric_limits<double>::quiet_NaN();
//		for (auto& i : rangeX) {
//			globalX += i;
//		}
//		return globalX /= rangeX.size();
//	}
//	else if (isnan(hits.at(0))) {
//		std::vector<double> rangeX = {};
//		std::vector<double> missingStrawGrid = StrawGrid.at(1).u;
//		for (auto& item : missingStrawGrid) {
//			item -= StrawGrid.at(0).straw_r;
//		}
//		std::set_difference(missingStrawGrid.begin(), missingStrawGrid.end(),
//			StrawGrid.at(1).u.begin(), StrawGrid.at(1).u.end(),
//			std::back_inserter(rangeX));
//		for (auto& i : rangeX) {
//			globalX += i;
//		}
//		return globalX /= rangeX.size();
//	}
//	else if (isnan(hits.at(1))) {
//		std::vector<double> rangeX = {};
//		std::vector<double> missingStrawGrid = StrawGrid.at(0).u;
//		for (auto& item : missingStrawGrid) {
//			item -= StrawGrid.at(1).straw_r;
//		}
//		std::set_difference(StrawGrid.at(0).u.begin(), StrawGrid.at(0).u.end(),
//			missingStrawGrid.begin(), missingStrawGrid.end(),
//			std::back_inserter(rangeX));
//		for (auto& i : rangeX) {
//			globalX += i;
//		}
//		return globalX /= rangeX.size();
//	}
//	return std::numeric_limits<double>::quiet_NaN();
//}

//double BuildGlobalY(const std::vector<double>& hits, const std::vector<Straw>& StrawGrid) {
//	double globalY = 0;
//	if (isnan(hits.at(2)) && isnan(hits.at(3))) {
//		return std::numeric_limits<double>::quiet_NaN();
//	}
//	if (!isnan(hits.at(2)) && !isnan(hits.at(3))) {
//		std::vector<double> rangeY = {};
//		std::set_intersection(StrawGrid.at(2).u.begin(), StrawGrid.at(2).u.end(),
//			StrawGrid.at(3).u.begin(), StrawGrid.at(3).u.end(),
//			std::back_inserter(rangeY));
//		if (rangeY.size() < 1) return std::numeric_limits<double>::quiet_NaN();
//		for (auto& i : rangeY) {
//			globalY += i;
//		}
//		return globalY /= rangeY.size();
//	}
//	else if (isnan(hits.at(2))) {
//		std::vector<double> rangeX = {};
//		std::vector<double> missingStrawGrid = StrawGrid.at(3).u;
//		for (auto& item : missingStrawGrid) {
//			item += StrawGrid.at(2).straw_r;
//		}
//		std::set_difference(missingStrawGrid.begin(), missingStrawGrid.end(),
//			StrawGrid.at(3).u.begin(), StrawGrid.at(3).u.end(),
//			std::back_inserter(rangeX));
//		for (auto& i : rangeX) {
//			globalY += i;
//		}
//		return globalY /= rangeX.size();
//	}
//	else if (isnan(hits.at(3))) {
//		std::vector<double> rangeX = {};
//		std::vector<double> missingStrawGrid = StrawGrid.at(2).u;
//		for (auto& item : missingStrawGrid) {
//			item -= StrawGrid.at(3).straw_r;
//		}
//		std::set_difference(StrawGrid.at(2).u.begin(), StrawGrid.at(2).u.end(),
//			missingStrawGrid.begin(), missingStrawGrid.end(),
//			std::back_inserter(rangeX));
//		for (auto& i : rangeX) {
//			globalY += i;
//		}
//		return globalY /= rangeX.size();
//	}
//	return std::numeric_limits<double>::quiet_NaN();
//}
//
//std::vector<double> TrackPoint(const std::vector<double>& hits) {
//	const double straw_radius = 1.0;
//	const double straw_length = 10.0;
//
//	std::vector<Straw> SetOfCoordinates = BuildCoordGrid(hits, straw_radius, straw_length);
//	std::vector<double> coords;
//	coords.push_back(BuildGlobalX(hits, SetOfCoordinates));
//	coords.push_back(BuildGlobalY(hits, SetOfCoordinates));
//	if (isnan(coords.at(0)) && isnan(coords.at(1))) coords.push_back(std::numeric_limits<double>::quiet_NaN());
//	else coords.push_back(4. * straw_radius);
//
//	return coords;
//}


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
	std::vector<double> detHits = { 1.0, 0.0, 2.0, 2.0 };
	std::vector<Straw> StrawSet = BuildCoordGrid(detHits, 1.0);

	size_t cnt = 0;
	for (auto& i : StrawSet) {
		std::cout << "Layer: " << cnt << ", coord is " << detHits.at(cnt) << std::endl;
		std::cout << "u: { ";
		for (auto iter = i.u.begin(); iter != i.u.end(); iter++) {
			std::cout << *iter;
			if (iter != i.u.end() - 1) {
				std::cout << ", ";
			}
			else std::cout << " }\n";
		}
		cnt++;
	}
	//std::vector<std::vector<double>> TestSet = GeneratePermutations({ 1.0, 0.0, 2.0, 2.0 });
	//std::vector<std::vector<double>> CoordsSet = {};
	//for (const auto& set : TestSet) {
	//	CoordsSet.push_back(TrackPoint(set));
	//}

	//std::vector<std::vector<double>> prm = GeneratePermutations({ 0.0, 1.0, 0.0, 1.0});
	/*for (const auto& layer : detHits) {
		std::cout << "Layer No.: " << layer.straw_layer << std::endl;
		for (const auto& i : layer.u) {
			std::cout << i << " ";
		}
		std::cout << std::endl;
		for (const auto& i : layer.v) {
			std::cout << i << " ";
		}
		std::cout << std::endl << "----------------------------------------\n";
	}*/
	/*for (size_t i = 0; i < CoordsSet.size(); i++) {
		for (const auto& coord : TestSet.at(i)) {
			std::cout << coord << " ";
		}
		std::cout << " <=========> ";
		for (const auto& coord : CoordsSet.at(i)) {
			std::cout << coord << " ";
		}
		std::cout << std::endl;
	}*/

	return 0;
}