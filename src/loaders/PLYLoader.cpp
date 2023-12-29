#include "PLYLoader.h"

#include "tinyply.h"


auto PLYLoader::load(const std::string& filename) -> PointCloud 
{
	PointCloud result;

	std::unique_ptr<std::istream> fileStream;
	std::vector<uint8_t> byteBuffer;
	std::shared_ptr<tinyply::PlyData> plyPoints, plyColors;
	unsigned baseIndex;
	float* pointsRawFloat = nullptr;
	double* pointsRawDouble = nullptr;
	uint8_t* colorsRaw;

	try
	{
		//const std::string filename = filename;
		fileStream.reset(new std::ifstream(filename, std::ios::binary));

		if (!fileStream || fileStream->fail()) {
			spdlog::error("[PLYLoader] Error creating filestream. {}", filename);
			return result;
		}

		fileStream->seekg(0, std::ios::end);
		const float size_mb = fileStream->tellg() * float(1e-6);
		fileStream->seekg(0, std::ios::beg);

		tinyply::PlyFile file;
		file.parse_header(*fileStream);

		try { 
			plyPoints = file.request_properties_from_element("vertex", { "x", "y", "z" }); 
		}
		catch (const std::exception& e) {
			spdlog::error("[PLYLoader] tinyply exception: {}", e.what());
		}

		try { 
			plyColors = file.request_properties_from_element("vertex", { "red", "green", "blue" }); 
		}
		catch (const std::exception& e) {
			spdlog::error("[PLYLoader] tinyply exception: {}", e.what());
		}

		file.read(*fileStream);
		{
			const bool isDouble = plyPoints->t == tinyply::Type::FLOAT64;
			const size_t numPoints = plyPoints->count;
			const size_t numPointsBytes = numPoints * (!isDouble ? sizeof(float) : sizeof(double)) * 3;

			const size_t numColors = plyColors->count;
			const size_t numColorsBytes = numColors * 1 * 3;

			//Memcpy
			if (!isDouble)
			{
				pointsRawFloat = new float[numPoints * 3];
				std::memcpy(pointsRawFloat, plyPoints->buffer.get(), numPointsBytes);
			}
			else
			{
				pointsRawDouble = new double[numPoints * 3];
				std::memcpy(pointsRawDouble, plyPoints->buffer.get(), numPointsBytes);
			}
			colorsRaw = new uint8_t[numColors * 3];
			std::memcpy(colorsRaw, plyColors->buffer.get(), numColorsBytes);

			// Allocate space
			result.points.resize(numPoints);
			if (!isDouble)
			{
				spdlog::info("[PLYLoader] points format: double");
				for (unsigned index = 0; index < numPoints; ++index)
				{
					baseIndex = index * 3;

					result.points[index] = PointModel{ glm::vec3(pointsRawFloat[baseIndex], pointsRawFloat[baseIndex + 1], pointsRawFloat[baseIndex + 2]),
												glm::vec3(colorsRaw[baseIndex], colorsRaw[baseIndex + 1], colorsRaw[baseIndex + 2]) / 256.f };
					result.aabb.update(result.points[index].position);
				}
			}
			else
			{
				spdlog::info("[PLYLoader] points format: float");
				for (unsigned index = 0; index < numPoints; ++index)
				{
					baseIndex = index * 3;

					result.points[index] = PointModel{ glm::vec3(pointsRawDouble[baseIndex], pointsRawDouble[baseIndex + 1], pointsRawDouble[baseIndex + 2]),
												glm::vec3(colorsRaw[baseIndex], colorsRaw[baseIndex + 1], colorsRaw[baseIndex + 2]) / 256.f };
					result.aabb.update(result.points[index].position);
				}
			}

			spdlog::info("[PLYLoader] point count: {}", result.points.size());
			spdlog::info("[PLYLoader] point cloud AABB : min {} {}   max {} {} ", result.aabb.min().x, result.aabb.min().y, result.aabb.max().x, result.aabb.max().y);
		}
	}
	catch (const std::exception& e)
	{
		spdlog::error("[PLYLoader] tinyply exception: {}", e.what());
	}
	return result;
}