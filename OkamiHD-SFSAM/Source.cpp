#include <iostream>
#include <fstream>
#include <string>

const int saveSlotCount = 30;
const int saveSlotSize = 0x1729F + 1;
const int saveFileSize = (saveSlotSize * saveSlotCount);

std::string saveSlotString = "SaveSlot";

char saveSlotBuffer[saveSlotSize];

void splitSaveFile()
{
	

	std::ifstream sizeCheck("OKAMI", std::ifstream::ate | std::ifstream::binary);
	if (!sizeCheck.bad() && (sizeCheck.tellg()) == saveFileSize)
	{
		sizeCheck.close();
		std::ifstream infile("OKAMI", std::ifstream::binary);

		for (int i = 0; i < saveSlotCount; i++)
		{
			std::ofstream outfile((saveSlotString + std::to_string(i + 1)), std::ofstream::binary);


			infile.seekg((saveSlotSize * i));
			infile.read(saveSlotBuffer, saveSlotSize);
			outfile.write(saveSlotBuffer, saveSlotSize);

			outfile.close();
		}
	}

	
}

void mergeSaveSlots() 
{
	for (int i = 0; i < saveSlotCount; i++)
	{
		std::ifstream sizeCheck((saveSlotString + std::to_string(i + 1)), std::ifstream::binary);
		if (sizeCheck.bad() && (sizeCheck.tellg()) != saveSlotSize)
		{
			std::cout << "Invalid save slot found." << std::endl;
			sizeCheck.close();
			return;
		}
		sizeCheck.close();
	}
	std::ofstream outfile("OKAMI", std::ofstream::binary);
	for (int i = 0; i < saveSlotCount; i++)
	{
		std::ifstream infile((saveSlotString + std::to_string(i + 1)), std::ifstream::binary);
		infile.read(saveSlotBuffer, saveSlotSize);
		outfile.write(saveSlotBuffer, saveSlotSize);
		infile.close();
	}
}

int main()
{
	std::cout <<
		"Please enter \"s\" to split an existing save file or \"m\" to merge save slots into a new save file." 
		<< std::endl;

	char command;
	command = getchar();
	if (command == 's')
	{
		splitSaveFile();
	}
	if (command == 'm')
	{
		mergeSaveSlots();
	}

}