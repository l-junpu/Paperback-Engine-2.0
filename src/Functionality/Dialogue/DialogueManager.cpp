/**********************************************************************************
*\file         DialogueManager.cpp
*\brief        DialogueManager.cpp
*
*\author	   Mok Wen Qing, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
					 or disclosure of this file or its contents without the prior
					 written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#include "DialogueManager.h"
#include <fstream>
#include <filesystem>
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"

DialogueManager::DialogueManager()
{
	//load every single dialogue
	LoadDialogueJson("../../resources/dialogue/test.dialogue", "test");

	LoadDialogueJson("../../resources/dialogue/Level1-A.dialogue", "Level1-A");
	LoadDialogueJson("../../resources/dialogue/Level1-B.dialogue", "Level1-B");
	LoadDialogueJson("../../resources/dialogue/Level1-C.dialogue", "Level1-C");
	LoadDialogueJson("../../resources/dialogue/Level1-D.dialogue", "Level1-D");
	LoadDialogueJson("../../resources/dialogue/Level1-E.dialogue", "Level1-E");

	LoadDialogueJson("../../resources/dialogue/Level2-A.dialogue", "Level2-A");
	LoadDialogueJson("../../resources/dialogue/Level2-B.dialogue", "Level2-B");
	LoadDialogueJson("../../resources/dialogue/Level2-C.dialogue", "Level2-C");
	LoadDialogueJson("../../resources/dialogue/Level2-D.dialogue", "Level2-D");

	LoadDialogueJson("../../resources/dialogue/Level3-A.dialogue", "Level3-A");
	LoadDialogueJson("../../resources/dialogue/Level3-B.dialogue", "Level3-B");
	LoadDialogueJson("../../resources/dialogue/Level3-C.dialogue", "Level3-C");

}

void DialogueManager::LoadDialogueJson(std::string file, std::string dialogue_name)
{
	std::ifstream input_file {file};

	if (input_file.is_open())
	{
		std::stringstream file_buffer{};
		file_buffer << input_file.rdbuf();
		input_file.close();

		rapidjson::Document doc;
		doc.Parse(file_buffer.str().c_str());
		const rapidjson::Value& dialogue_info = doc;

		Dialogue dialogue{};

		for (rapidjson::Value::ConstMemberIterator it = dialogue_info.MemberBegin(); it != dialogue_info.MemberEnd(); ++it)
		{
			Line line{};
			
			line.m_AudioFile = it->value.GetString();		//"Audio File": "path here"
			++it;

			line.m_CameraName = it->value.GetString();		//"Camera Name": "name here"
			++it;

			std::string speaker = it->name.GetString();		//"ST/JT": "speech here"

			if (speaker == "ST")
			{
				line.m_Speaker = Line::Speaker::RED;
			}

			else if (speaker == "JT")
			{
				line.m_Speaker = Line::Speaker::BLUE;
			}

			line.m_Content = it->value.GetString();
			dialogue.m_Lines.push_back(line);
		}

		m_Dialogues[dialogue_name] = dialogue;
	}

}

DialogueManager& DialogueManager::GetInstanced()
{
	static DialogueManager dm;
	return dm;
}