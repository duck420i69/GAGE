#include "pch.h"
#include "Globals.h"

uint32_t Globals::gScreenWidth = 1600, Globals::gScreenHeight = 900;

std::string Globals::gScreenTitle = "Hello world";

std::unique_ptr<Scene> Globals::gCurrentScene = nullptr;

std::vector<std::shared_ptr<GameObject>> Globals::LoadAllGameObjects( const std::string& file_name)
{
	using namespace std;
	ifstream stream;
	stream.exceptions(ifstream::failbit | ifstream::badbit);
	std::vector<std::shared_ptr<GameObject>> result;

	try
	{
		stream.open(file_name);

		string name;
		int z_index;
		glm::vec3 pos;
		glm::vec3 rot;
		glm::vec3 scale;
		size_t component_count;
		shared_ptr<GameObject> game_object;

		while (!stream.eof())
		{
			getline(stream, name);
			stream >> z_index;
			stream >> pos.x >> pos.y >> pos.z;
			stream >> rot.x >> rot.y >> rot.z;
			stream >> scale.x >> scale.y >> scale.z;
			stream >> component_count;
			game_object = make_shared<GameObject>(name, Transform(pos, rot, scale), z_index);
			
			for (int i = 0; i < component_count; i++)
			{

			}

		}
		
		stream.close();
	}
	catch (std::ifstream::failure& e)
	{
		Logger::error("Failed to open save file: {}, error message: {}", file_name, e.what());
	}

	return result;
}

void Globals::SaveAllGameObjects(const std::vector<std::shared_ptr<GameObject>>& game_objects, const std::string& file_name)
{
	using namespace std;
	ofstream stream;
	stream.exceptions(ofstream::failbit | ofstream::badbit);

	try
	{
		stream.open(file_name);

		for (const shared_ptr<GameObject>& game_object : game_objects)
		{
			stream << game_object->mName << "\n";
			stream << game_object->mZIndex << " ";
			stream << game_object->mTransform.mPos.x << " ";
			stream << game_object->mTransform.mPos.y << " ";
			stream << game_object->mTransform.mPos.z << " ";
			stream << game_object->mTransform.mRot.x << " ";
			stream << game_object->mTransform.mRot.y << " ";
			stream << game_object->mTransform.mRot.z << " ";
			stream << game_object->mTransform.mScale.x << " ";
			stream << game_object->mTransform.mScale.y << " ";
			stream << game_object->mTransform.mScale.z << " ";
			stream << game_object->mComponents.size() << "\n";

			for (const std::shared_ptr<Component>& component : game_object->mComponents)
			{
				component->ParseComponent(stream);
			}
			stream << "\n";
		}
		stream.close();
	}
	catch (std::ostream::failure& e)
	{
		Logger::error("Failed to open save file: {}, error message: {}", file_name, e.what());
	}

}
