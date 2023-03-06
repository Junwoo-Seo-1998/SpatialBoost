#pragma once
#include <vector>
#include <glm/vec3.hpp>

//Epos 6(Ritter)
constexpr glm::vec3 Normal001_1 = { 1.f, 0.f, 0.f };
constexpr glm::vec3 Normal001_2 = { 0.f, 1.f, 0.f };
constexpr glm::vec3 Normal001_3 = { 0.f, 0.f, 1.f };
//Epos 14
constexpr glm::vec3 Normal111_1 = { 1.f, 1.f, 1.f };
constexpr glm::vec3 Normal111_2 = { 1.f, 1.f, -1.f };
constexpr glm::vec3 Normal111_3 = { 1.f, -1.f, 1.f };
constexpr glm::vec3 Normal111_4 = { 1.f, -1.f, -1.f };
//Epos 26
constexpr glm::vec3 Normal011_1 = { 1.f, 1.f, 0.f };
constexpr glm::vec3 Normal011_2 = { 1.f, -1.f, 0.f };
constexpr glm::vec3 Normal011_3 = { 1.f, 0.f, 1.f };
constexpr glm::vec3 Normal011_4 = { 1.f, 0.f, -1.f };
constexpr glm::vec3 Normal011_5 = { 0.f, 1.f, 1.f };
constexpr glm::vec3 Normal011_6 = { 0.f, 1.f, -1.f };
//EPOS 98
constexpr glm::vec3 Normal012_1 = { 0, 1, 2 };
constexpr glm::vec3 Normal012_2 = { 0, 1, -2 };
constexpr glm::vec3 Normal012_3 = { 0, 2, 1 };
constexpr glm::vec3 Normal012_4 = { 0, 2, -1 };
constexpr glm::vec3 Normal012_5 = { 1, 0, 2 };
constexpr glm::vec3 Normal012_6 = { 1, 0, -2 };
constexpr glm::vec3 Normal012_7 = { 2, 0, 1 };
constexpr glm::vec3 Normal012_8 = { 2, 0, -1 };
constexpr glm::vec3 Normal012_9 = { 1, 2, 0 };
constexpr glm::vec3 Normal012_10 = { 1, -2, 0 };
constexpr glm::vec3 Normal012_11 = { 2, 1, 0 };
constexpr glm::vec3 Normal012_12 = { 2, -1, 0 };

constexpr glm::vec3 Normal112_1 = { 1, 1, 2 };
constexpr glm::vec3 Normal112_2 = { 1, 1, -2 };
constexpr glm::vec3 Normal112_3 = { 1, -1, 2 };
constexpr glm::vec3 Normal112_4 = { -1, 1, 2 };
constexpr glm::vec3 Normal112_5 = { 1, 2, 1 };
constexpr glm::vec3 Normal112_6 = { 1, 2, -1 };
constexpr glm::vec3 Normal112_7 = { 1, -2, 1 };
constexpr glm::vec3 Normal112_8 = { -1, 2, 1 };
constexpr glm::vec3 Normal112_9 = { 2, 1, 1 };
constexpr glm::vec3 Normal112_10 = { 2, 1, -1 };
constexpr glm::vec3 Normal112_11 = { 2, -1, 1 };
constexpr glm::vec3 Normal112_12 = { -2, 1, 1 };

constexpr glm::vec3 Normal122_1 = { 1, 2, 2 };
constexpr glm::vec3 Normal122_2 = { 1, 2, -2 };
constexpr glm::vec3 Normal122_3 = { 1, -2, 2 };
constexpr glm::vec3 Normal122_4 = { -1, 2, 2 };
constexpr glm::vec3 Normal122_5 = { 2, 1, 2 };
constexpr glm::vec3 Normal122_6 = { 2, 1, -2 };
constexpr glm::vec3 Normal122_7 = { 2, -1, 2 };
constexpr glm::vec3 Normal122_8 = { -2, 1, 2 };
constexpr glm::vec3 Normal122_9 = { 2, 2, 1 };
constexpr glm::vec3 Normal122_10 = { 2, 2, -1 };
constexpr glm::vec3 Normal122_11 = { 2, -2, 1 };
constexpr glm::vec3 Normal122_12 = { -2, 2, 1 };

const std::vector<glm::vec3> EPOS98 = { Normal001_1, Normal001_2, Normal001_3, Normal111_1, Normal111_2,
	Normal111_3, Normal111_4, Normal011_1, Normal011_2, Normal011_3, Normal011_4, Normal011_5, Normal011_6,
	Normal012_1, Normal012_2, Normal012_3, Normal012_4, Normal012_5, Normal012_6, Normal012_7, Normal012_8,
	Normal012_9, Normal012_10, Normal012_11, Normal012_12, Normal112_1, Normal112_2, Normal112_3,
	Normal112_4, Normal112_5, Normal112_6, Normal112_7, Normal112_8, Normal112_9, Normal112_10,
	Normal112_11, Normal112_12, Normal122_1, Normal122_2, Normal122_3, Normal122_4, Normal122_5,
	Normal122_6, Normal122_7, Normal122_8, Normal122_9, Normal122_10, Normal122_11, Normal122_12 };