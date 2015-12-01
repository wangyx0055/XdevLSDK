/*
	TinyDir.

	Copyright © 2005-2012 Cengiz Terzibas

	This library is free software; you can redistribute it and/or modify it under the
	terms of the GNU Lesser General Public License as published by the Free Software
	Foundation; either version 2.1 of the License, or (at your option) any later version.
	This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
	without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
	See the GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License along with
	this library; if not, write to the Free Software Foundation, Inc., 59 Temple Place,
	Suite 330, Boston, MA 02111-1307 USA

	I would appreciate if you report all bugs to: cengiz@terzibas.de
*/

#include "tinydir.h"
#include <stdexcept>
#include <algorithm>
#include <errno.h>
#include <stdio.h>

namespace tdir{

bool sort_name_dir_entitiy(dir_entity i, dir_entity j);

// Sort function.
bool sort_name_dir_entitiy(dir_entity i, dir_entity j) {
	std::string si(i.get_name());
	std::string sj(j.get_name());
	return (si < sj);
}

size_t directory::getNumEntities(){
	return m_entity_list.size();
}

const char* directory::getEntity(long int pos){
	try{
		return	m_entity_list.at(pos).get_name();
	}catch(...){

		return NULL;
	}
}

void directory::reload(){
	if(m_entity_list.size() > 0)
		m_entity_list.clear();

	// Get all directory entities.
	int ret = 0;
	do{
		dir_entity ent;
		ret = read(&ent);
		if(ret != -1)
			m_entity_list.push_back(ent);
	}while(ret != -1);

	rewind();

	std::sort(m_entity_list.begin(),m_entity_list.end(),sort_name_dir_entitiy);
}


int directory::rename (const char* oldname, const char* newname){
	if(::rename(oldname, newname) == -1){
		return -1;
	}
	return 0;
}

#if defined __GNUG__ && !defined _WIN32

const char* directory::parent_dir = "..";
const char* directory::local_dir = ".";

directory::directory() :  m_dir(NULL) {
	m_workingDirectoryBuffer = new char[512];
}

directory::~directory(){
	delete [] m_workingDirectoryBuffer;
}


int directory::change(const char* dirname){
	if(chdir(dirname) == -1)
		return -1;

	if(open() == -1)
		return -1;

	return 0;
}

int directory::make(const char* name){
	if(::mkdir(name, S_IRWXU | S_IRGRP |  S_IROTH | S_IXGRP | S_IXOTH) == -1)
		return -1;

	reload();

	return 0;
}

int directory::remove(const char* name){
	if(rmdir(name) == -1){
		return -1;
	}

	reload();

	return 0;
}



int directory::open(const char* dirname){
	m_dir = opendir(dirname);
	if(NULL == m_dir){
		errno = ENOENT;
		return -1;
	}

	reload();

	return 0;
}

int directory::open(){
	m_dir = opendir(".");
	if(NULL == m_dir){
		errno = ENOENT;
		return -1;
	}

	reload();

	return 0;
}

int directory::close(){
	if(m_dir){
		if(-1 == closedir(m_dir)){
			return -1;
		}
	}
	return 0;
}

int directory::read(dir_entity* entity){
	struct dirent* ent = NULL;
	ent = readdir(m_dir);
	if(ent == NULL){
		if(errno != EBADF)
		return -1;
	}
	dir_entity tmp(ent->d_name);
	*entity = tmp;
	return 0;
}

void directory::seek(long int pos){
	seekdir(m_dir, pos);
}

void directory::rewind(){
	if(m_dir)
		rewinddir(m_dir);

}

long int directory::tell(){
	return telldir(m_dir);
}

const char* directory::getWorkingDirectory(){
	if(getcwd(m_workingDirectoryBuffer, 512) != m_workingDirectoryBuffer){
		return NULL;
	}
	return m_workingDirectoryBuffer;
}

#elif defined _WIN32



directory::directory() : hFile(0) {
	m_workingDirectoryBuffer = new char[512];
}

directory::~directory(){
	delete [] m_workingDirectoryBuffer;
}

const char* directory::getWorkingDirectory(){
	if(_getcwd(m_workingDirectoryBuffer, 512) != m_workingDirectoryBuffer){
		return NULL;
	}
	return m_workingDirectoryBuffer;
}

int directory::change(const char* dirname){
	if(_chdir(dirname) == -1)
		return -1;
	return 0;
}

int directory::make(const char* name){
	if(_mkdir(name) == -1)
		return -1;

	return 0;
}

int directory::open(const char* dirname){
	std::string tmp(dirname);
	std::string slash, all;
	size_t pos = tmp.find("/\\");
	if(pos != std::string::npos){
		slash = "";
		all 	= "*";
	}else{
		slash = "/";
		all 	= "/*";
	}
	tmp += all;
	m_dir.set_name(tmp.c_str());
	if((hFile = _findfirst(tmp.c_str(), &m_info)) == -1L){
		return -1;
	}

	reload();

	return 0;
}

int directory::close(){
	if(hFile){
		_findclose( hFile );
		return 0;
	}
	return -1;
}

int directory::read(dir_entity* entity){
	struct _finddata_t  info;
	if(hFile){
		if(_findnext(hFile, &info) == -1){
			return -1;
		}
	}
	dir_entity tmp;
	tmp.set_name(info.name);
	*entity = tmp;
	return 0;
}

void directory::seek(long int pos){
	struct _finddata_t  info;
	if(hFile){
		for(long int a = 0; a < pos; ++ a){
			if(_findnext(hFile, &info) == -1){
				// Error occurred.
			}
		}
	}
}

void directory::rewind(){
	if(hFile)
	{
		struct _finddata_t  info;
    _findclose(hFile);
    hFile = (long) _findfirst(m_dir.get_name(), &info);
  }

}

long int directory::tell(){
	return 0;
}

int directory::remove(const char* name){
	if(_rmdir(name) == -1){
		return -1;
	}

	reload();

	return 0;
}

#endif

} // End of namespace
