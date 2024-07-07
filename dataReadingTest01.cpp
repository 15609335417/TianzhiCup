#include <iostream>
#include<string>
#include<vector>
#include<map>
#include<list>
#include<dirent.h>
#include"satobs.h"

using namespace std;

/*********************
* 字符串去掉空格
* csdn
**********************/
//去除尾部空白字符 包括\t \n \r  
/*
标准的空白字符包括：
' '     (0x20)    space (SPC) 空格符
'\t'    (0x09)    horizontal tab (TAB) 水平制表符
'\n'    (0x0a)    newline (LF) 换行符
'\v'    (0x0b)    vertical tab (VT) 垂直制表符
'\f'    (0x0c)    feed (FF) 换页符
'\r'    (0x0d)    carriage return (CR) 回车符
//windows \r\n linux \n mac \r
*/
char* rtrim(char* str)
{
	if (str == NULL || *str == '\0')
	{
		return str;
	}

	int len = strlen(str);
	char* p = str + len - 1;
	while (p >= str && isspace(*p))
	{
		*p = '\0';
		--p;
	}

	return str;
}


//去除首部空格
char* ltrim(char* str)
{
	if (str == NULL || *str == '\0')
	{
		return str;
	}

	int len = 0;
	char* p = str;
	while (*p != '\0' && isspace(*p))
	{
		++p;
		++len;
	}

	memmove(str, p, strlen(str) - len + 1);

	return str;
}

//去除首尾空格
char* trim(char* str)
{
	str = rtrim(str);
	str = ltrim(str);

	return str;
}

void getFilesInFolder(const char* folderPath, list<string>& filePath)
{
	//读文件夹
	DIR* dir;
	struct dirent* ent;
	char FilePath[256] = "";
	if ((dir = opendir(folderPath)) == NULL)
	{
		printf("Cna't open folder %s\n！", folderPath);
		return;
	}
	while ((ent = readdir(dir)) != NULL) {//读取目录中的文件
		memset(FilePath, '\0', 256);
		if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0)continue;//跳过"."和".."
		if (strncmp(ent->d_name, "sat",3) == 0 || strncmp(ent->d_name, "sta",3) == 0)continue;//跳过"."和".."

		//printf("%s%s\n", folderPath, ent->d_name);//输出文件名
		strcat_s(FilePath, 256, folderPath);
		strcat_s(FilePath, 256, ent->d_name);
		//strcpy_s(FilePath, 256, FilePath);
		filePath.push_back(FilePath);
	}
	closedir(dir);//关闭目录

}

void getFilesInFolder(const char* folderPath, list<string>& rFilePath,list<string>& sFilePath)
{
	//读文件夹
	DIR* dir;
	FILE* fl;
	struct dirent* ent;
	char buffer[256] = "";
	char FilePath[256] = "";
	if ((dir = opendir(folderPath)) == NULL)
	{
		printf("Can't open folder %s\n！", folderPath);
		return;
	}
	while ((ent = readdir(dir)) != NULL) {//读取目录中的文件
		memset(FilePath, '\0', 256);
		// s File
		if (strncmp(ent->d_name, "sat", 3) == 0)
		{
			printf("%s%s\n", folderPath, ent->d_name);//输出文件名
			sprintf_s(FilePath, "%s%s", folderPath, ent->d_name);
			if (fopen_s(&fl, FilePath, "r+b"))
				printf("Can't open observation data file name list %s !", FilePath);
			while (!feof(fl))
			{
				fgets(buffer, 256, fl);
				memset(FilePath, '\0', 256);
				sprintf_s(FilePath, "%s%s", folderPath, buffer);
				trim(FilePath);
				sFilePath.push_back(FilePath);
			}
			fclose(fl);
		}
		// r File
		else if (strncmp(ent->d_name, "sta", 3) == 0)
		{
			printf("%s%s\n", folderPath, ent->d_name);//输出文件名
			sprintf_s(FilePath, "%s%s", folderPath, ent->d_name);
			if (fopen_s(&fl, FilePath, "r+b"))
				printf("Can't open observation data file name list %s !", FilePath);
			while (!feof(fl))
			{
				fgets(buffer, 256, fl);
				memset(FilePath, '\0', 256);
				sprintf_s(FilePath, "%s%s", folderPath, buffer);
				trim(FilePath);
				rFilePath.push_back(FilePath);
			}
			fclose(fl);
		}
		else continue;
	}
	closedir(dir);//关闭目录

}

int main()
{
	satobs data;
	map<int, map<int, list<satobs>>> dataMap;
	vector<int> stationID;
	vector<int> satlliteID;
	vector<char[256]> _filePath;
	list<string> filePath;
	list<string> rFilePath;
	list<string> sFilePath;
	const char* folderPath = "data\\1\\obsdat\\";
	getFilesInFolder(folderPath, rFilePath, sFilePath);
	getFilesInFolder(folderPath, filePath);
	//auto it = filePath.begin();
	auto it = filePath.begin();
	int num = 0;
	for (; it != filePath.end(); it++)
	{
		char FilePath[256] = "";
		it->copy(FilePath, it->size(), 0);

		FILE* obsFile;
		char buffer[256];
		char* saveptr = NULL; // 用于保存 strtok_r 的内部状态

		if (fopen_s(&obsFile, FilePath, "r+b"))
		{
			printf("The observation data file %s was not opened!\n", FilePath);
			continue;
			//return -1;
		}
		fgets(buffer, 256, obsFile);
		while (!feof(obsFile))
		{
			data.obsType = atoi(strtok_s(buffer, " ", &saveptr));//测量类型
			data.stationID = atoi(strtok_s(NULL, " ", &saveptr));//测站ID
			data.satlliteID = atoi(strtok_s(NULL, " ", &saveptr));//卫星ID
			strcpy_s(data.obsSign, strtok_s(NULL, " ", &saveptr));//数据标识
			data.time.year = atoi(strtok_s(NULL, " ", &saveptr));//UTC 年
			data.time.day = atoi(strtok_s(NULL, " ", &saveptr));//UTC 年积日
			data.time.sod = atof(strtok_s(NULL, " ", &saveptr));//UTC 秒
			data.distance = atoi(strtok_s(NULL, " ", &saveptr));
			data.distance += atof(strtok_s(NULL, " ", &saveptr)) * 1e-9;


			dataMap[data.stationID][data.satlliteID].push_back(data);
			memset(buffer, '\0', sizeof(buffer));
			fgets(buffer, 256, obsFile);
		}
		stationID.push_back(data.stationID);
		satlliteID.push_back(data.satlliteID);
		printf("%s\n", FilePath);
		fclose(obsFile);

		//num++;
		//if (num > 600)
		//	break;
	}

	//读文件夹
	//DIR* dir;
	//struct dirent* ent;
	//if ((dir = opendir(folderPath)) == NULL)
	//{
	//	printf("Cna't open folder %s\n！", folderPath);
	//	return -5;//文件夹读取失败
	//}
	//while ((ent = readdir(dir)) != NULL)//读取目录中的文件
	//{
	//	char FilePath[256]="";
	//	if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0)continue;//跳过"."和".."
	//	//printf("%s%s\n", folderPath, ent->d_name);//输出文件名
	//	strcat_s(FilePath,256,folderPath);
	//	strcat_s(FilePath, 256, ent->d_name);
	//	//filePath.push_back(FilePath);
	//	//printf("%s\n", FilePath);//输出文件名

	//	//读取每个文件
	//	char* saveptr=NULL; // 用于保存 strtok_r 的内部状态
	//	FILE* obsFile;
	//	char buffer[256];

	//	if (fopen_s(&obsFile, FilePath, "r+b"))
	//	{
	//		printf("The observation data file %s was not opened!\n", FilePath);
	//		continue;
	//		//return -1;
	//	}
	//	fgets(buffer, 256, obsFile);
	//	while (!feof(obsFile))
	//	{
	//		//saveptr = NULL;
	//		//char* token;
	//		//data = satobs();
	//	
	//		//strcpy_s(data.obsType, strtok_s(buffer, " ", &saveptr));//测量类型
	//		//strcpy_s(data.stationID, strtok_s(NULL, " ", &saveptr));//测站ID
	//		//strcpy_s(data.satlliteID, strtok_s(NULL, " ", &saveptr));//卫星ID
	//		//strcpy_s(data.obsSign, strtok_s(NULL, " ", &saveptr));//数据标识
	//		data.obsType = atoi(strtok_s(buffer, " ", &saveptr));//测量类型
	//		data.stationID = atoi(strtok_s(NULL, " ", &saveptr));//测站ID
	//		data.satlliteID = atoi( strtok_s(NULL, " ", &saveptr));//卫星ID
	//		strcpy_s(data.obsSign, strtok_s(NULL, " ", &saveptr));//数据标识
	//		data.time.year = atoi(strtok_s(NULL, " ", &saveptr));//UTC 年
	//		data.time.day = atoi(strtok_s(NULL, " ", &saveptr));//UTC 年积日
	//		data.time.sod = atof(strtok_s(NULL, " ", &saveptr));//UTC 秒
	//		data.distance = atoi(strtok_s(NULL, " ", &saveptr));
	//		data.distance += atof(strtok_s(NULL, " ", &saveptr)) * 1e-9;

	//		//dataVector.push_back(data);
	//		//dataList.push_back(data);
	//		//dataMap[data.time] = data;
	//		dataMap[data.stationID][data.satlliteID].push_back(data);
	//		//ID[data.stationID] = data.satlliteID;

	//		//dataMapAll[data.time].push_back(data);
	//		//dataMap[data.stationID].push_back(data);
	//		//dataMap[data.satlliteID].push_back(data);
	//		memset(buffer, '\0', sizeof(buffer));
	//		fgets(buffer, 256, obsFile);
	//	}
	//	stationID.push_back(data.stationID);
	//	satlliteID.push_back(data.satlliteID);
	//	//dataAll.push_back(dataMap);
	//	fclose(obsFile);
	//	
	//}
	//closedir(dir);//关闭目录
	//auto stationID = dataMapID.begin();
	//auto satlliteID = dataMapID.begin();
	//for (; stationID != dataMapID.end(); stationID++)
	//{
	//	for (; satlliteID != dataMapID.end(); satlliteID++)
	//	{
	//		auto it = satlliteID->second.begin();
	//		for (; it != satlliteID->second.end(); it++)
	//		{
	//			if (stationID->first == it->first);

	//		}
	//		
	//	}
	//}
	//for (; stationID != dataMapID.end(); stationID++)
	//{
	//	auto satlliteID = stationID->second.begin();
	//	for (; satlliteID != stationID->second.end(); satlliteID++)
	//	{
	//		if(stationID->first==)
	//	}
	//}
	//int num = 0;
	//for (int i = 0; i < stationID.size(); i++)
	//{
	//	for (int j = 0; j < satlliteID.size(); j++)
	//	{
	//		if (stationID[i] == satlliteID[j] )
	//		{
	//			if(stationID[j] == satlliteID[i])
	//				cout<< satlliteID[i] << " ; "<< satlliteID[j] << '\n';
	//			num++;
	//		}
	//	}
	//}

 	return 0;
}

