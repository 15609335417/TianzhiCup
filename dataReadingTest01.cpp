#include <iostream>
#include<string>
#include<vector>
#include<map>
#include<list>
#include<dirent.h>
#include"satobs.h"

using namespace std;

void getFilesInFolder(const char* folderPath)
{
	//读文件夹
	DIR* dir;
	struct dirent* ent;
	if ((dir = opendir(folderPath)) == NULL)
	{
		printf("Cna't open folder %s\n！", folderPath);
		return;
	}
	while ((ent = readdir(dir)) != NULL) {//读取目录中的文件
		if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0)continue;//跳过"."和".."
		printf("%s%s\n", folderPath, ent->d_name);//输出文件名
	}
	closedir(dir);//关闭目录

}


int main()
{
	satobs data;
	vector<satobs> dataVector;
	map<UTC, satobs> dataMap;
	list<satobs> dataList;
	vector<map<UTC, satobs>> dataAll;
	map<UTC, vector<satobs>> dataMapAll;
	//auto it = dataMapAll.begin();
	//char FileName[256] = "data\\obsdat\\r_009000101_005001001.ttt";
	//
	//char* saveptr; // 用于保存 strtok_r 的内部状态
	//FILE* obsFile;
	//char buffer[256];
	//
	//if (fopen_s(&obsFile, FileName, "r+b"))
	//{
	//	printf("The observation data file %s was not opened!\n", FileName);
	//	return -1;
	//}	
	//fgets(buffer, 256, obsFile);
	//while (!feof(obsFile))
	//{
	//	//char* token;
	//	data.obsType = strtok_s(buffer, " ", &saveptr);//测量类型
	//	data.stationID = strtok_s(NULL, " ", &saveptr);//测站ID
	//	data.satlliteID = strtok_s(NULL, " ", &saveptr);//卫星ID
	//	data.obsSign = strtok_s(NULL, " ", &saveptr);//数据标识
	//	data.time.year = atoi(strtok_s(NULL, " ", &saveptr));//UTC 年
	//	data.time.day = atoi(strtok_s(NULL, " ", &saveptr));//UTC 年积日
	//	data.time.sod = atof(strtok_s(NULL, " ", &saveptr));//UTC 秒
	//	data.distance = atoi(strtok_s(NULL, " ", &saveptr))+ atof(strtok_s(NULL, " ", &saveptr))*1e-9;//UTC 年积日

	//	dataVector.push_back(data);
	//	dataList.push_back(data);
	//	dataMap[data.time] = data;
	//	fgets(buffer, 256, obsFile);
	//}


	const char* folderPath = "data\\obsdat\\";

	//读文件夹
	DIR* dir;
	struct dirent* ent;
	if ((dir = opendir(folderPath)) == NULL)
	{
		printf("Cna't open folder %s\n！", folderPath);
		return -5;//文件夹读取失败
	}
	while ((ent = readdir(dir)) != NULL)//读取目录中的文件
	{
		char FilePath[256]="";
		if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0)continue;//跳过"."和".."
		//printf("%s%s\n", folderPath, ent->d_name);//输出文件名
		strcat_s(FilePath,256,folderPath);
		strcat_s(FilePath, 256, ent->d_name);
		//printf("%s\n", FilePath);//输出文件名

		//读取每个文件
		char* saveptr=NULL; // 用于保存 strtok_r 的内部状态
		FILE* obsFile;
		char buffer[256];

		if (fopen_s(&obsFile, FilePath, "r+b"))
		{
			printf("The observation data file %s was not opened!\n", FilePath);
			continue;
			//return -1;
		}
		fgets(buffer, 256, obsFile);
		while (!feof(obsFile))
		{
			//saveptr = NULL;
			//char* token;
			//data = satobs();
		
			strcpy_s(data.obsType, strtok_s(buffer, " ", &saveptr));//测量类型
			strcpy_s(data.stationID, strtok_s(NULL, " ", &saveptr));//测站ID
			strcpy_s(data.satlliteID, strtok_s(NULL, " ", &saveptr));//卫星ID
			strcpy_s(data.obsSign, strtok_s(NULL, " ", &saveptr));//数据标识
			//data.obsType = strtok_s(buffer, " ", &saveptr);//测量类型
			//data.stationID = strtok_s(NULL, " ", &saveptr);//测站ID
			//data.satlliteID = strtok_s(NULL, " ", &saveptr);//卫星ID
			//data.obsSign = strtok_s(NULL, " ", &saveptr);//数据标识
			data.time.year = atoi(strtok_s(NULL, " ", &saveptr));//UTC 年
			data.time.day = atoi(strtok_s(NULL, " ", &saveptr));//UTC 年积日
			data.time.sod = atof(strtok_s(NULL, " ", &saveptr));//UTC 秒
			data.distance = atoi(strtok_s(NULL, " ", &saveptr)) + atof(strtok_s(NULL, " ", &saveptr)) * 1e-9;//UTC 年积日

			//dataVector.push_back(data);
			//dataList.push_back(data);
			//dataMap[data.time] = data;
			dataMapAll[data.time].push_back(data);
			memset(buffer, '\0', sizeof(buffer));
			fgets(buffer, 256, obsFile);
		}
		
		//dataAll.push_back(dataMap);
		fclose(obsFile);
		
	}
	closedir(dir);//关闭目录
	auto it = dataMapAll.begin();
	for (; it != dataMapAll.end(); it++)
	{
		int num = 0;
		for (int i = 0; i < it->second.size(); i++)
		{
			for (int j = 0; j < it->second.size(); j++)
			{
				if (strcmp(it->second[i].stationID, it->second[j].satlliteID) == 0 && strcmp(it->second[j].stationID, it->second[i].satlliteID) == 0)
				{
					num++;
				}
			}
		}

	}

	return 0;
}

