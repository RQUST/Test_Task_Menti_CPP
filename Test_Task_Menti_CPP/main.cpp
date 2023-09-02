#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <algorithm>
#include <ctime>
#include <iomanip>
#include <cctype>

struct Object
{
	std::string object_name;
	double x = 0.0;
	double y = 0.0;
	std::string object_type;
	std::string object_time_created;
};

// Comparator to compare objects by name within the same type

bool CompareByNameWithinType(const Object& obj1, const Object& obj2)
{
	if (obj1.object_type == obj2.object_type)
	{
		return obj1.object_name < obj2.object_name;
	}
	return false;
}

// Comparator to compare objects by distance within the same type
bool CompareByDistanceWithinType(const Object& obj1, const Object& obj2)
{
	if (obj1.object_type == obj2.object_type)
	{
		double dist1 = std::sqrt((obj1.x - obj1.y) * (obj1.x - obj1.y));
		double dist2 = std::sqrt((obj2.x - obj2.y) * (obj2.x - obj2.y));
		return dist1 < dist2;
	}
	return false;
}

// Comparator to compare objects by time within the same type
bool CompareByTimeWithinType(const Object& obj1, const Object& obj2)  
{
	std::tm time1 = {};
	std::istringstream ss1(obj1.object_time_created);
	ss1 >> std::get_time(&time1, "%d.%m.%Y");

	std::tm time2 = {};
	std::istringstream ss2(obj2.object_time_created);
	ss2 >> std::get_time(&time2, "%d.%m.%Y");

	if (ss1.fail() || ss2.fail()) {
		// Parsing error handling
		return false;
	}

	return std::mktime(&time1) < std::mktime(&time2);
}

bool compareByTime(const Object& obj1, const Object& obj2)
{
	std::tm time1 = {};
	std::istringstream ss1(obj1.object_time_created);
	ss1 >> std::get_time(&time1, "%d.%m.%Y");

	std::tm time2 = {};
	std::istringstream ss2(obj2.object_time_created);
	ss2 >> std::get_time(&time2, "%d.%m.%Y");

	if (ss1.fail() || ss2.fail()) {
		// Parsing error handling
		return false;
	}

	return std::mktime(&time1) < std::mktime(&time2);
}

void groupAndSortObjects(std::vector<Object>& objects, bool sortByDistance, bool sortByTime)
{
	// Create a map to store objects of the same object_type together
	std::map<std::string, std::vector<Object>> typeGroups;

	for (const Object& obj : objects)
	{
		// If the object name starts with a digit, special symbol, or foreign language,
		// put it in a separate group with the name "#other"
		if (!std::isalpha(obj.object_name[0]) &&
			!std::isalnum(obj.object_name[0]) &&
			(obj.object_name[0] >= 'À' && obj.object_name[0] <= 'z'))
		{
			// Change the object_type to "#other"
			Object newObj = obj;
			newObj.object_type = "#other";
			typeGroups["#other"].push_back(newObj);
		}
		else
		{
			typeGroups[obj.object_type].push_back(obj);
		}
	}

	objects.clear();

	for (auto& group : typeGroups)
	{
		if (group.second.size() >= 2) // Check if there are enough objects to create a group
		{
			// Choose the comparator based on the sortByDistance and sortByTime parameters
			if (sortByDistance && !sortByTime) {
				std::sort(group.second.begin(), group.second.end(), CompareByDistanceWithinType);
			}
			else if (!sortByDistance && sortByTime) {
				std::sort(group.second.begin(), group.second.end(), CompareByTimeWithinType);
			}
			else {
				std::sort(group.second.begin(), group.second.end(), CompareByNameWithinType);
			}

			objects.insert(objects.end(), group.second.begin(), group.second.end());
		}
		else
		{
			// If there are fewer than 2 objects of this type, put them in a group called "various"
			for (const Object& obj : group.second)
			{
				Object newObj = obj;
				newObj.object_type = "various";
				objects.push_back(newObj);
			}
		}
	}
}

int main()
{
	std::ifstream inputFile("input.txt");
	if (!inputFile.is_open())
	{
		std::cerr << "Error opening the input file." << std::endl;
		return 1;
	}

	std::vector<Object> objects;
	std::string line;

	while (std::getline(inputFile, line))
	{
		Object obj;
		std::stringstream ss(line);
		ss >> obj.object_name >> obj.x >> obj.y >> obj.object_type >> obj.object_time_created;
		objects.push_back(obj);
	}

	// Prompt the user to choose the sorting type
	std::cout << "Select sorting type (1 - by distance, 2 - by name, 3 - by time): ";
	int sortingChoice;
	std::cin >> sortingChoice;
	bool sortByDistance = false;
	bool sortByTime = false;

	switch (sortingChoice)
	{
	case 1:
		sortByDistance = true;
		break;
	case 2:
		break;
	case 3:
		sortByTime = true;
		break;
	default:
		std::cerr << "Invalid choice. Sorting by name by default." << std::endl;
		break;
	}

	groupAndSortObjects(objects, sortByDistance, sortByTime);

	// Open the "output.txt" file for writing
	std::ofstream outputFile("output.txt");
	if (!outputFile.is_open())
	{
		std::cerr << "Error opening the output file." << std::endl;
		return 1;
	}

	std::string currentObjectType; // To track the current object_type

	// Write data to the "output.txt" file with distance calculation and newline on object_type change
	for (const Object& obj : objects)
	{
		double distance = std::sqrt((obj.x - obj.y) * (obj.x - obj.y)); // Calculate distance

		if (obj.object_type != currentObjectType)
		{
			outputFile << std::endl; // Newline on object_type change
			currentObjectType = obj.object_type;
		}

		outputFile << obj.object_name << " " << obj.x << " " << obj.y << " "
			<< obj.object_type << " " << obj.object_time_created << "  \t  "
			<< "Distance: " << distance << std::endl;
	}

	return 0;
}
